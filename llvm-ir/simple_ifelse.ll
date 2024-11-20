;전방선언
declare i32 @puts(i8* nocapture) nounwind

;출력할 문자열
@foo = constant [3 x i8] c"FOO"
@bar = constant [3 x i8] c"BAR"

define i32  @main()
{
	;문자열을 포인터로 캐스팅
	%foo = getelementptr [3 x i8], [3 x i8]* @foo, i64 0, i64 0
	%bar = getelementptr [3 x i8], [3 x i8]* @bar, i64 0, i64 0

	%num = add i32 40, 4
	%condition = icmp eq i32 %num, 44
	br i1 %condition, label %TrueLabel, label %FalseLabel
    TrueLabel:
        call i32 @puts(i8* %foo)
	br label %EndIfLabel
    FalseLabel:
        call i32 @puts(i8* %bar)
	br label %EndIfLabel
    EndIfLabel:
	ret i32 0
}
