#This tests everything about function parameters

fun f1(a) {
    print(a)
}

fun f2(a) {
    a = 500
}

fun f3(a, b) {
    return a + b
}

fun f4(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19) {
    print(a0)
    print(a1)
    print(a2)
    print(a3)
    print(a4)
    print(a5)
    print(a6)
    print(a7)
    print(a8)
    print(a9)
    print(a10)
    print(a11)
    print(a12)
    print(a13)
    print(a14)
    print(a15)
    print(a16)
    print(a17)
    print(a18)
    print(a19)
    a0 = 10
    a1 = 20
    a2 = 30
    a3 = 40
    a4 = 50
    a5 = 60
    a6 = 70
    a7 = 80
    a8 = 90
    a9 = 100
    a10 = 110
    a11 = 120
    a12 = 130
    a13 = 140
    a14 = 150
    a15 = 160
    a16 = 170
    a17 = 180
    a18 = 190
    a19 = 200
}

fun f6(a) {
    print(a)
    if(a != 0) {
        f6(a-1)
    }
}

fun main() {
#basic parameter passthrough - should print 1
    f1(1)


#local scoping for parameter - should print 2
    b = 2
    f2(b)
    print(b)

#local scoping for parameter - should print 3
    a = 3
    f2(a)
    print(a)

# function as a parameter - should print 6
    print(f3(1, f3(2, 3)))

# function as a parameter - should print 10
    print(f3(1, f3(2, f3(3, 4))))

# preservation of parameters - should print 1 to 20 twice
    a0 = 1
    a1 = 2
    a2 = 3
    a3 = 4
    a4 = 5
    a5 = 6
    a6 = 7
    a7 = 8
    a8 = 9
    a9 = 10
    a10 = 11
    a11 = 12
    a12 = 13
    a13 = 14
    a14 = 15
    a15 = 16
    a16 = 17
    a17 = 18
    a18 = 19
    a19 = 20
    f4(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19)
    print(a0)
    print(a1)
    print(a2)
    print(a3)
    print(a4)
    print(a5)
    print(a6)
    print(a7)
    print(a8)
    print(a9)
    print(a10)
    print(a11)
    print(a12)
    print(a13)
    print(a14)
    print(a15)
    print(a16)
    print(a17)
    print(a18)
    print(a19)

#local scoping for recursive parameters - should print 5 to 0
    f6(5)
}

#Comment with no newline at end
