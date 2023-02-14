#test that functions can have certain names
fun mov(){
    print(5)
}
fun lea(){
    print(6)
}
fun eax(){
    print(7)
}
fun label(){
    print(8)
}
fun puts(){
    print(9)
}

fun stress(x){
    v1 = 1
    v2 = 2
    v3 = 3
    v4 = 4
    v5 = 5
    v6 = 6
    v7 = 7
    v8 = 8
    v9 = 9
    v10 = 10
    v11 = 11
    v12 = 12
    v13 = 13
    v14 = 14
    v15 = 15
    v16 = 16

    while(x > 0){
        v1 = v1 * v9
        v2 = v2 * v10
        v3 = v3 * v11
        v4 = v4 * v12
        v5 = v5 * v13
        v6 = v6 * v14
        v7 = v7 * v15
        v8 = v8 * v16
        x = x - 1
    }
    print(v1)
    print(v2)
    print(v3)
    print(v4)
    print(v5)
    print(v6)
    print(v7)
    print(v8)
}

fun main(){
    print(1111111111)
    mov()
    lea()
    eax()
    label()
    puts()
    print(2222222222)
#test that you use the right comparison instructions (signed vs unsigned)
#and that you can handle large numbers
#2^64-1 > 0
    print(18446744073709551615 > 0)
    print(18446744073709551615 >= 0)
    print(18446744073709551615 < 0)
    print(18446744073709551615 <= 0)
    print(0 > 18446744073709551615)
    print(0 >= 18446744073709551615)
    print(0 < 18446744073709551615)
    print(0 <= 18446744073709551615)
    print(3333333333)
#for booleans, check that 2^63 (i.e. binary 1 with 63 zeros after it) counts as true
    print(0 && 0)
    print(0 && 9223372036854775808)
    print(9223372036854775808 && 0)
    print(9223372036854775808 && 9223372036854775808)
    print(0 || 0)
    print(0 || 9223372036854775808)
    print(9223372036854775808 || 0)
    print(9223372036854775808 || 9223372036854775808)
    print(4444444444)
#test overflowing multiplication
    print(18446744073709551615 * 2)
    print(9223372036854775808 * 2)

#not my original idea: make sure that you don't use multiplication for checking and because it could overflow to 0
    print(18446744073709551615 && 2)
    print(18446744073709551615 || 2)

    print(5555555555)
#try to hack into the matrix by modifying registers
    eax = 2
    rax = eax * 5
    rsp = 4325209385
    rip = 80238049875032
    print(eax % rax % rsp % rip)

#test crazy parenthesis x100 and x10 with addition
    print((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((10))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))
    print((1+(1+(1+(1+(1+(1+(1+(1+(1+(1)))))))))))

#if you call this function with a very large number, it becomes a stress test
    stress(2)
}

