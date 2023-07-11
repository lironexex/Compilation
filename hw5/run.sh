#!/usr/bin/csh -x

clear
clear

make
./hw5 < t1.in >&t1.ll
lli t1.ll > t1.res
diff t1.res t1.out

./hw5 < t2.in >&t2.ll
lli t2.ll > t2.res
diff t2.res t2.out

./hw5 < t3.in >&t3.ll
lli t3.ll > t3.res
diff t3.res t3.out

./hw5 < t3_no_else.in >&t3_no_else.ll
lli t3_no_else.ll > t3_no_else.res
diff t3_no_else.res t3_no_else.out

cat t3.ll
