@.string_specifier = internal constant [4 x i8] c"%s\0A\00" 
 @.int_specifier = internal constant [4 x i8] c"%d\0A\00"
@.zero_div_error = internal constant [23 x i8] c"Error division by zero\00"
@.str0 = internal constant [21 x i8] c"shouldn't be printed\00"
@.str1 = internal constant [18 x i8] c"should be printed\00"

define void @print(i8*) {
  call i32 (i8*, ...) @printf(
    i8* getelementptr ([4 x i8], [4 x i8]* @.string_specifier, i32 0, i32 0), i8* %0)
  ret void
}

define void @printi(i32) {
  call i32 (i8*, ...) @printf(
      i8* getelementptr ([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)
  ret void
}

define void @_zeroDiv() {
	%1 = getelementptr [23 x i8], [23 x i8]* @.zero_div_error, i32 0, i32 0
	call void @print(i8* %1)
	call void @exit(i32 1)
	ret void
}

define void @explainWhileElse(i32) {
explainWhileElse_entry:
	%_stackPtr1 = alloca [50 x i32]
	%_1 = getelementptr [50 x i32], [50 x i32]* %_stackPtr1, i32 0, i32 0
	store i32 %0, i32* %_1
	%_2 = getelementptr [50 x i32], [50 x i32]* %_stackPtr1, i32 0, i32 1
	store i32 0, i32* %_2
	; wrapStatement
	br label %label_14
	br label %label_14
; Marker
label_14:
	br label %label_17
; Marker
label_17:
	%_3 = load i32, i32* %_2
	%_4 = load i32, i32* %_1
	%_5 = icmp slt i32 %_3, %_4
	br i1 %_5, label %label_24, label %label_47
	br label %label_24
; Marker
label_24:
	%_6 = load i32, i32* %_2
	call void @printi(i32 %_6)
	br label %label_29
; Marker
label_29:
	%_7 = load i32, i32* %_2
	%_8 = add i32 %_7, 1
	store i32 %_8, i32* %_2
	br label %label_35
; Marker
label_35:
	%_9 = load i32, i32* %_2
	%_10 = icmp eq i32 %_9, 5
	br i1 %_10, label %label_41, label %label_17
	br label %label_41
; Marker
label_41:
	br label %label_52
	; Narker
	br label %label_17
	br label %label_47
; Marker
label_47:
	%_11 = getelementptr [21 x i8], [21 x i8]* @.str0, i32 0, i32 0
	call void @print(i8* %_11)
	br label %label_52
; Marker
label_52:
	br label %label_55
; Marker
label_55:
	br label %label_58
; Marker
label_58:
	%_12 = load i32, i32* %_2
	%_13 = load i32, i32* %_1
	%_14 = icmp slt i32 %_12, %_13
	br i1 %_14, label %label_65, label %label_88
	br label %label_65
; Marker
label_65:
	%_15 = load i32, i32* %_2
	call void @printi(i32 %_15)
	br label %label_70
; Marker
label_70:
	%_16 = load i32, i32* %_2
	%_17 = add i32 %_16, 1
	store i32 %_17, i32* %_2
	br label %label_76
; Marker
label_76:
	%_18 = load i32, i32* %_2
	%_19 = icmp eq i32 %_18, 5
	br i1 %_19, label %label_82, label %label_58
	br label %label_82
; Marker
label_82:
	br label %label_93
	; Narker
	br label %label_58
	br label %label_88
; Marker
label_88:
	%_20 = getelementptr [18 x i8], [18 x i8]* @.str1, i32 0, i32 0
	call void @print(i8* %_20)
	br label %label_93
; Marker
label_93:
	br label %label_95
label_95:
	ret void
}

define void @main() {
main_entry:
	%_stackPtr2 = alloca [50 x i32]
	%_21 = getelementptr [50 x i32], [50 x i32]* %_stackPtr2, i32 0, i32 0
	store i32 7, i32* %_21
	; wrapStatement
	br label %label_107
	br label %label_107
; Marker
label_107:
	%_22 = load i32, i32* %_21
	call void @explainWhileElse(i32 %_22)
	br label 0x0p+0bel_111
label_111:
	ret void
}
declare i32 @printf(i8*, ...)
declare void @exit(i32)