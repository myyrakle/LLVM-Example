;전방선언
declare i32 @puts(i8* nocapture) nounwind

;출력할 문자열
@boom = constant [5 x i8] c"Boom!"

define void @doBoom()
{
	%boom = getelementptr [5 x i8], [5 x i8]* @boom, i64 0, i64 0

	call i32 @puts(i8* %boom)

	ret void
}

define i32 @main()
{
	call void() @doBoom()

	ret i32 0
}
