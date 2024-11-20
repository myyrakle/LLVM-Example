declare i32 @printf(i8* nocapture, ...) nounwind

@format = constant [3 x i8] c"%d\0A"

;type alias
%int = type i32

define %int @main()
{
	%format = getelementptr [3 x i8], [3 x i8]* @format, i64 0, i64 0

	%value = add %int 1000, 999

	call %int(i8*, ...) @printf(i8* %format, %int %value)

	ret %int 0
}
