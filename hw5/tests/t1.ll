define i32 @printByValue(i32) {
	%stack = alloca [50 x i32]
	%t00 = getelementptr inbounds [50 x i32], [50 x i32]* %stack, i32 0, i32 0     ; BOOL val; (%t00)
	store i32 0, i32* %t00                                                         ; val = 0 (init with default value)
	 store i32 %0, i32* %t00                                                       ; loading argument number -1 into stack
; if start
	%reg0 = load i32, i32* %t00                                                    ; load: %reg0 = val
	%reg1 = trunc i32 %reg0 to i1                                                  ; cast bool to i32
	br i1 %reg1, label %label_10, label %label_16                                                  ; val
; true block start
label_10:
	%reg2 = getelementptr [12 x i8], [12 x i8]* @.str2, i32 0, i32 0
	%reg3 = call i32 @print(i8* %reg2)                                             ; function print is called with params: (i8* %reg2)
; true block end
	br label %label_21
; false block start
label_16:
	%reg4 = getelementptr [13 x i8], [13 x i8]* @.str4, i32 0, i32 0
	%reg5 = call i32 @print(i8* %reg4)                                             ; function print is called with params: (i8* %reg4)
; false block end
	br label %label_21                                                                     ; br exit if
label_21:
	ret i32 0
}
define i32 @main() {
	%stack = alloca [50 x i32]
	br label %label_27
label_27:
	br label %label_31                                                                     ; true label: label_27
label_29:
	br label %label_31                                                                     ; false label: label_29
label_31:
	%reg6 = phi i1 [ 1, %label_27], [ 0, %label_29 ]
	%reg7 = zext i1 %reg6 to i32                                                   ; bool sign extend
	%reg8 = add i32 %reg7, 0                                                       ; special cmd
	%reg9 = call i32 @printByValue(i32 %reg8)                                      ; function printByValue is called with params: (i32 %reg8)
	br label %label_39
label_37:
	br label %label_41                                                                     ; true label: label_37
label_39:
	br label %label_41                                                                     ; false label: label_39
label_41:
	%reg10 = phi i1 [ 1, %label_37], [ 0, %label_39 ]
	%reg11 = zext i1 %reg10 to i32                                                 ; bool sign extend
	%reg12 = add i32 %reg11, 0                                                     ; special cmd
	%reg13 = call i32 @printByValue(i32 %reg12)                                    ; function printByValue is called with params: (i32 %reg12)
; if start
	br label %label_49
; true block start
label_49:
	%reg14 = getelementptr [5 x i8], [5 x i8]* @.str14, i32 0, i32 0
	%reg15 = call i32 @print(i8* %reg14)                                           ; function print is called with params: (i8* %reg14)
	br label %label_53                                                                     ; exit if
label_53:
; if start
	br label %label_56
label_56:
	br label %label_65
label_58:
	br label %label_61
; true block start
label_61:
	%reg16 = getelementptr [5 x i8], [5 x i8]* @.str16, i32 0, i32 0
	%reg17 = call i32 @print(i8* %reg16)                                           ; function print is called with params: (i8* %reg16)
	br label %label_65                                                                     ; exit if
label_65:
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
@.str2 = constant [12 x i8] c"val is true\00"
@.str4 = constant [13 x i8] c"val is false\00"
@.str14 = constant [5 x i8] c"true\00"
@.str16 = constant [5 x i8] c"true\00"
