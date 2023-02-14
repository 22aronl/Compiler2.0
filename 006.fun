# hello reader :)

# things to watch out for in this test:
#   - labels don't interfere with common label names
#   - functions return 0 (make sure you move to %rax!)
fun labelInterferenceTest() {
# variable names to check for interference of common labels
    printf = 1
    main = 2
    callq = 3
    format = 4

# the following should print

# 1
    print(printf)

# 2
    print(main)

# 3
    print(callq)

# 4
    print(format)
}

# you can't push large constants, so make sure to move them to a register first!
fun testParamOverflowUnderflow(a, b, c) {
    print(a)
    print(b)
    print(c)
}

# tests for pushing large functions
fun pushLargeTest() {
    a = 0 - 1
    if (a) {
        print(a)
    }

# be careful when pushing (0-1) to stack
# you'll need to move it into a register first
    if ((0 - 1) % 2) {
        print(1)
    } else {
        print(0)
    }
}

# tests large multiplications, divisions, mods
fun testMulDivLg() {
    print((0 - 1) * (0 - 1))
    print((0 - 1) / (0 - 1))
    print((0 - 1) % (0 - 1))
}

# tests while loops, if, mod, add, sub
fun fibDp(c) {
    a = 1
    b = 1
    while(c > 0) {
        if (c % 2 == 1) {
            a = a + b
        } else {
            b = a + b
        }
        c = c - 1
    }

# 144
    print(a)
# 89
    print(b)
}

fun main() {
# <3 ASCII Art :D
    print(118881111111888811)
    print(181118811118111881)
    print(811111181181111188)
    print(888111118811111118)
    print(188111111111111881)
    print(111881111111118111)
    print(111188811118881111)
    print(111111881188811111)
    print(111111188881111111)

    labelInterferenceTest()

# this should also print 0
    print(testParamOverflowUnderflow(0-1, 18446744073709551615, 0))

    print(11111111111111111)
    testMulDivLg()

    fibDp(10)
}
# comment below with no characters after it
#
