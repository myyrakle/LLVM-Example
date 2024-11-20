;전방선언
declare i32 @printf(i8* nocapture, ...) nounwind

;출력할 문자열
@format = constant [3 x i8] c"%d\0A"

define i32 @myAdd(i32 %lhs, i32 %rhs)
{
	%result = add i32 %lhs, %rhs

	ret i32  %result
}

define i32 @main()
{
	%format = getelementptr [3 x i8], [3 x i8]* @format, i64 0, i64 0

	%val = call i32 @myAdd(i32 50, i32 44)

	call i32(i8*, ...) @printf(i8* %format, i32 %val)

	ret i32 0
}
