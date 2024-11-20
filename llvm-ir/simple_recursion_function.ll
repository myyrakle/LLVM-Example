;전방선언
declare i32 @puts(i8* nocapture) nounwind

;출력할 문자열
@boom = constant [5 x i8] c"Boom!"

define void @doBoom(i32 %counter)
{
	%cond = icmp ne i32 %counter, 0
	br i1 %cond, label %If, label %Else

	If:
	%boom = getelementptr [5 x i8], [5 x i8]* @boom, i64 0, i64 0
	call i32 @puts(i8* %boom)

	%next = sub i32 %counter, 1
	call void @doBoom(i32 %next)
	ret void

	Else:
	ret void
}



define i32 @main()
{
	call void @doBoom(i32 5)

	ret i32 0
}
