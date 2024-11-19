#include <stdio.h>
#include "llvm/Support/SourceMgr.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/TargetParser/Host.h"

const auto source = R"(; 이게 주석임
; 문자열 상수를 전역상수처럼 선언함
@.str = private unnamed_addr constant [13 x i8] c"hello world\0A\00"

; puts 함수의 외부 선언
declare i32 @puts(i8* nocapture) nounwind

; main 함수 정의
define i32 @main()
{   
     ; i32()*
     ; [13 x i8]를 i8 *로 변환함...

    %cast210 = getelementptr [13 x i8],[13 x i8]* @.str, i64 0, i64 0

    ; puts 함수를 호출해서 stdout에 문자열을 출력함
    call i32 @puts(i8* %cast210)

    ret i32 0
}

; 이름 붙인 메타데이터
!0 = !{i32 42, null, !"string"}
!foo = !{!0} )";

int main()
{
    llvm::LLVMContext llvm_context;

    llvm::MemoryBufferRef buffer(llvm::StringRef(source), "input");
    llvm::SMDiagnostic err;

    auto llvm_module = llvm::parseIR(buffer, err, llvm_context);
    llvm_module->setPICLevel(llvm::PICLevel::SmallPIC);
    llvm_module->setPIELevel(llvm::PIELevel::Level::Large);

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto target_triple = llvm::sys::getDefaultTargetTriple();
    printf(">> target triple: %s\n", target_triple.c_str());

    llvm_module->setTargetTriple(target_triple);

    std::string error;
    auto Target = llvm::TargetRegistry::lookupTarget(target_triple, error);

    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!Target)
    {
        llvm::errs() << error;
        return 1;
    }

    auto cpu = "generic";
    auto features = "";
    llvm::TargetOptions target_options;

    auto relocation_model = std::optional<llvm::Reloc::Model>(llvm::Reloc::Model::PIC_);
    auto target_machine =
        Target->createTargetMachine(target_triple, cpu, features, target_options, relocation_model);

    const auto data_layout = target_machine->createDataLayout();
    printf(">> data layout: %s\n", data_layout.getStringRepresentation().c_str());
    llvm_module->setDataLayout(data_layout);

    auto output_filename = "output.o";

    std::error_code error_code;
    llvm::raw_fd_ostream dest(output_filename, error_code, llvm::sys::fs::OF_None);

    if (error_code)
    {
        llvm::errs() << "Could not open file: " << error_code.message();
        return 1;
    }

    llvm::legacy::PassManager pass_manager;
    auto output_file_type = llvm::CodeGenFileType::ObjectFile;

    if (target_machine->addPassesToEmitFile(pass_manager, dest, nullptr, output_file_type))
    {
        llvm::errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
    }

    pass_manager.run(*llvm_module);
    dest.flush();

    llvm::outs() << "Wrote " << output_filename << "\n";

    return 0;
}
