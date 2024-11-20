;전방선언
declare i32 @puts(i8* nocapture) nounwind

;출력할 문자열
@boom = constant [6 x i8] c"Boom!!"

define i32  @main()
{
	;문자열을 포인터로 캐스팅
	%boom = getelementptr [6 x i8], [6 x i8]* @boom, i64 0, i64 0

	%i = alloca i32
	store i32 0, i32* %i
	br label %BeginLoopLabel
    BeginLoopLabel:
	%current = load i32, i32* %i
	%cond = icmp slt i32 %current, 5
	br i1 %cond, label %LoopStatementLabel, label %EndLoopLabel
    LoopStatementLabel:
        call i32 @puts(i8* %boom)
	%next = add i32 %current, 1
	store i32 %next, i32* %i
	br label %BeginLoopLabel
    EndLoopLabel:
	ret i32 0
}
