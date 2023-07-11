#!/usr/bin/csh -x

flex hw1.lex
gcc -ll lex.yy.c
./a.out < t1.in > my_res.out
diff my_res.out t1.out
./a.out < t2.in > my_res2.out
diff my_res2.out t2.out
./a.out < t3.in > my_res3.out
diff my_res3.out t3.out
./a.out < t4.in > my_res4.out
diff my_res4.out t4.out
./a.out < t5.in > my_res5.out
diff my_res5.out t5.out
./a.out < t6.in > my_res6.out
diff my_res6.out t6.out
./a.out < t7.in > my_res7.out
diff my_res7.out t7.out
./a.out < t8.in > my_res8.out
diff my_res8.out t8.out
./a.out < t9.in > my_res9.out
diff my_res9.out t9.out
./a.out < t10.in > my_res10.out
diff my_res10.out t10.out
./a.out < t11.in > my_res11.out
diff my_res11.out t11.out
./a.out < t12.in > my_res12.out
diff my_res12.out t12.out
./a.out < t13.in > my_res13.out
diff my_res13.out t13.out
./a.out < t14.in > my_res14.out
diff my_res14.out t14.out
./a.out < t15.in > my_res15.out
diff my_res15.out t15.out
./a.out < t16.in > my_res16.out
diff my_res16.out t16.out
./a.out < t17.in > my_res17.out
diff my_res17.out t17.out
./a.out < t18.in > my_res18.out
diff my_res18.out t18.out
./a.out < t19.in > my_res19.out
diff my_res19.out t19.out
./a.out < t20.in > my_res20.out
diff my_res20.out t20.out
./a.out < t21.in > my_res21.out
diff my_res21.out t21.out
./a.out < t22.in > my_res22.out
diff my_res22.out t22.out
./a.out < t23.in > my_res23.out
diff my_res23.out t23.out
./a.out < t24.in > my_res24.out
diff my_res24.out t24.out
./a.out < t25.in > my_res25.out
diff my_res25.out t25.out
./a.out < t26.in > my_res26.out
diff my_res26.out t26.out
./a.out < t27.in > my_res27.out
diff my_res27.out t27.out
./a.out < t28.in > my_res28.out
diff my_res28.out t28.out
./a.out < t29.in > my_res29.out
diff my_res29.out t29.out
./a.out < t30.in > my_res30.out
diff my_res30.out t30.out
./a.out < t31.in > my_res31.out
diff my_res31.out t31.out
./a.out < t32.in > my_res32.out
diff my_res32.out t32.out


