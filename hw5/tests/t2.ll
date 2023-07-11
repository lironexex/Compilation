define i32 @fib(i32) {
	%stack = alloca [50 x i32]
	%t00 = getelementptr inbounds [50 x i32], [50 x i32]* %stack, i32 0, i32 0     ; BYTE n; (%t00)
	store i32 0, i32* %t00                                                         ; n = 0 (init with default value)
	 store i32 %0, i32* %t00                                                       ; loading argument number -1 into stack
; if start
	%reg0 = load i32, i32* %t00                                                    ; load: %reg0 = n
	%reg1 = add i32 0, 0                                                           ; %reg1 = 0
	%reg2 = icmp eq i32 %reg0, %reg1                                               
	br i1 %reg2, label %label_16, label %label_10                                                  ; %reg0 == %reg1
label_10:
	%reg3 = load i32, i32* %t00                                                    ; load: %reg3 = n
	%reg4 = add i32 1, 0                                                           ; %reg4 = 1
	%reg5 = icmp eq i32 %reg3, %reg4                                               
	br i1 %reg5, label %label_16, label %label_20                                                  ; %reg3 == %reg4
; true block start
label_16:
	%reg6 = add i32 1, 0                                                           ; %reg6 = 1
	ret i32 %reg6
	br label %label_20                                                                     ; exit if
label_20:
	%reg7 = load i32, i32* %t00                                                    ; load: %reg7 = n
	%reg8 = add i32 1, 0
	%reg9 = sub i32 %reg7, %reg8
	%reg10 = trunc i32 %reg9 to i8
	%reg11 = zext i8 %reg10 to i32
	%reg12 = call i32 @fib(i32 %reg11)                                             ; function fib is called with params: (i32 %reg11)
	%reg13 = load i32, i32* %t00                                                   ; load: %reg13 = n
	%reg14 = add i32 2, 0
	%reg15 = sub i32 %reg13, %reg14
	%reg16 = trunc i32 %reg15 to i8
	%reg17 = zext i8 %reg16 to i32
	%reg18 = call i32 @fib(i32 %reg17)                                             ; function fib is called with params: (i32 %reg17)
	%reg19 = add i32 %reg12, %reg18
	ret i32 %reg19
	ret i32 0
}
define i32 @main() {
	%stack = alloca [50 x i32]
	%reg20 = add i32 0, 0
	%t11 = getelementptr inbounds [50 x i32], [50 x i32]* %stack, i32 0, i32 0     ; BYTE i; (%t11)
	store i32 0, i32* %t11                                                         ; i = 0 (init with default value)
	store i32 %reg20, i32* %t11                                                    ; load: i = %reg20
	br label %label_44
label_44:
; while #1 start
	%reg21 = load i32, i32* %t11                                                   ; load: %reg21 = i
	%reg22 = add i32 10, 0                                                         ; %reg22 = 10
	%reg23 = icmp slt i32 %reg21, %reg22                                           
	br i1 %reg23, label %label_51, label %label_75                                                 ; %reg21 < %reg22
; true block start
label_51:
	%reg24 = load i32, i32* %t11                                                   ; load: %reg24 = i
	%reg25 = call i32 @fib(i32 %reg24)                                             ; function fib is called with params: (i32 %reg24)
	%reg26 = call i32 @printi(i32 %reg25)                                          ; function printi is called with params: (i32 %reg25)
; if start
	%reg27 = load i32, i32* %t11                                                   ; load: %reg27 = i
	%reg28 = add i32 1, 0                                                          ; %reg28 = 1
	%reg29 = add i32 %reg27, %reg28
	%reg30 = add i32 10, 0                                                         ; %reg30 = 10
	%reg31 = icmp slt i32 %reg29, %reg30                                           
	br i1 %reg31, label %label_63, label %label_67                                                 ; %reg29 < %reg30
; true block start
label_63:
	%reg32 = getelementptr [2 x i8], [2 x i8]* @.str32, i32 0, i32 0
	%reg33 = call i32 @print(i8* %reg32)                                           ; function print is called with params: (i8* %reg32)
	br label %label_67                                                                     ; exit if
label_67:
	%reg34 = load i32, i32* %t11                                                   ; load: %reg34 = i
	%reg35 = add i32 1, 0
	%reg36 = add i32 %reg34, %reg35
	%reg37 = trunc i32 %reg36 to i8
	%reg38 = zext i8 %reg37 to i32
	store i32 %reg38, i32* %t11                                                    ; load: i = %reg38
	br label %label_44                                                                     ; while #1 end
label_75:
	%reg39 = getelementptr [2 x i8], [2 x i8]* @.str39, i32 0, i32 0
	%reg40 = call i32 @print(i8* %reg39)                                           ; function print is called with params: (i8* %reg39)
	ret i32 0
}
declare i32 @printf(i8*, ...)        ; declare printf from C
declare void @exit(i32)              ; declare exit from C
@.int_specific = constant [4 x i8] c"%d\0A\00"
@.str_specific = constant [4 x i8] c"%s\0A\00"
define i32 @printi(i32) {
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.int_specific, i32 0, i32 0), i32 %0)
ret i32 0
}
define i32 @print(i8*) {
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specific, i32 0, i32 0), i8* %0)
ret i32 0
}
@.printerror = constant [23 x i8] c"Error division by zero\00"
@.str32 = constant [2 x i8] c"*\00"
@.str39 = constant [2 x i8] c"*\00"
