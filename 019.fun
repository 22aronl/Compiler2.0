# Prints 1234567890, and then the section number
# This can be used as a separator for sections
# This should make debugging easier
# It also implicitly checks function calls
fun separate(x) {
    print(1234567890)
    print(x)
}

# Focuses on testing operations
fun operationTest() {
# Tests basic printing
    separate(1)
    print(0)
    print(12)
    print(345)

# Tests arithmetic operators
    separate(2)
    print(1 + 2)
    print(8 - 5)
    print(6 * 7)
    print(9 / 4)
    print(10 % 3)

# Tests &&, ||, <, <=, >, >=, ==, !=
    separate(3)
    print((1 && 2) * 10000 + (0 && 3) * 1000 + (4 && 0) * 100 + (5 && 5) * 10 + (0 && 0))
    print((1 || 2) * 10000 + (0 || 3) * 1000 + (4 || 0) * 100 + (5 || 5) * 10 + (0 || 0))
    print((1 < 2) * 100 + (2 < 1) * 10 + (0 < 0))
    print((1 <= 2) * 100 + (2 <= 1) * 10 + (0 <= 0))
    print((1 > 2) * 100 + (2 > 1) * 10 + (0 > 0))
    print((1 >= 2) * 100 + (2 >= 1) * 10 + (0 >= 0))
    print((1 == 2) * 100 + (2 == 1) * 10 + (0 == 0))
    print((1 != 2) * 100 + (2 != 1) * 10 + (0 != 0))

# Tests ! (and ensures that double nots work as intended)
    separate(4)
    print(!0)
    print(!1)
    print(!2)
    print(!!0)
    print(!!1)
    print(!!2)
    print(!!!0)
    print(!!!1)
    print(!!!2)

# Tests order of operations: arithmetic operations
    separate(5)
    print(1 + 2 * 3)
    print(4 - 6 / 5)
    print(7 * 8 % 9)
    print(1 - 2 + 3)
    print(0 / 7 + 6 * 5 - 10 % 4)
    print(8 * 9 / 2 + 3 % 1 - 4)

# Tests order of operations: including parentheses
    separate(6)
    print((((0))))
    print((1 + 2) * 3)
    print((6 - 4) / 5)
    print(7 * (8 % 9))
    print(3 - (2 + 1))
    print(11 / (5 + 6) * (10 - 7) % 4)
    print(8 * ((9 / 2) + 3 % (4 - 1 - 1)))

# Tests order of operations: &&, ||, !
    separate(7)
    print(0 && 1 || 1)
    print(!1 && !0 || !0)
    print(!1 && 1 || 1)
    print(0 && !0 || !0)

# Tests order of operations: including parentheses
    separate(8)
    print((0 && 1) || 1)
    print(0 && (1 || 1))
    print(!(1 && 1) || 1)
    print(!(1 && 1 || 1))
    print(0 && !(0 || !0))

# Tests order of operations: <, <=, >, >=, ==, !=
    separate(9)
    print(0 > 1 <= 0)
    print(0 >= 1 < 1)
    print(0 <= 1 < 1)
    print(0 < 1 <= 1)
    print(0 == 1 == 0)
    print(0 != 1 != 0)
    print(1 == 2 < 1)
    print(2 != 2 > 1)
    print(1 == 2 >= 2)
    print(1 != 2 <= 0)

# Tests order of operations: including parentheses
    separate(10)
    print(0 > (1 <= 0))
    print(0 >= (1 < 1))
    print(0 <= (1 < 1))
    print(0 < (1 <= 1))
    print(0 != 0 == (0 == 0))
    print(1 == 0 != (1 == 0))
    print((1 == 2) < 1)
    print((2 != 2) > 1)
    print((1 == 2) >= 2)
    print((1 != 2) <= 0)

# Tests order of operations: all operations combined
    separate(11)
    print(1 + 2 <= 3 * 4)
    print(7 >= 5 % !!6)
    print((9 / 8 == (11 - 10)) > (0) && 13 || (15 < 14 + 12 != 12))

# Tests large numbers (to ensure that all operations involve unsigned integers)
    separate(12)
    print((0 - 1) / 5)
    print(5 / (0 - 1))
    print((0 - 1) / ((0 - 1) / 2))
    print((0 - 1) * (0 - 1))
    print((0 - 1) % 9)
    print(9 % (0 - 1))
    print((0 - 1) % ((0 - 1) / 2))

# Tests awkward spacing
    separate(13)
    print(1     +          2)
    print(3+4)
    print(5+        6)
    print(7        +8)
}

# Tests other statements
fun statementTests() {
# Tests if statements and nested statements
    separate(1)
    x = 10
    y = 20
    if (x < 20) {
        print(x)
    }

    if (y < 10) {
        print(y)
    }

    if (0) {
        print(0)
        if (0) {
            print(1)
        }
        print(2)
    }

    if (0) {
        print(3)
        if (1) {
            print(4)
        }
        print(5)
    }

    if (1) {
        print(6)
        if (0) {
            print(7)
        }
        print(8)
    }

    if (1) {
        print(9)
        if (1) {
            print(10)
        }
        print(11)
    }

# Tests if else statements and nested statements
    separate(2)
    if (1) {
        print(1)
    } else {
        print(2)
    }

    if (0) {
        print(3)
    } else {
        print(4)
    }

    if (0) {
        if (0) {
            print(5)
        } else {
            print(6)
        }
    } else {
        if (0) {
            print(7)
        } else {
            print(8)
        }
    }

    if (0) {
        if (0) {
            print(9)
        } else {
            print(10)
        }
    } else {
        if (1) {
            print(11)
        } else {
            print(12)
        }
    }

    if (0) {
        if (1) {
            print(13)
        } else {
            print(14)
        }
    } else {
        if (0) {
            print(15)
        } else {
            print(16)
        }
    }

    if (0) {
        if (1) {
            print(17)
        } else {
            print(18)
        }
    } else {
        if (1) {
            print(19)
        } else {
            print(20)
        }
    }

    if (1) {
        if (0) {
            print(21)
        } else {
            print(22)
        }
    } else {
        if (0) {
            print(23)
        } else {
            print(24)
        }
    }

    if (1) {
        if (0) {
            print(25)
        } else {
            print(26)
        }
    } else {
        if (1) {
            print(27)
        } else {
            print(28)
        }
    }

    if (1) {
        if (1) {
            print(29)
        } else {
            print(30)
        }
    } else {
        if (0) {
            print(31)
        } else {
            print(32)
        }
    }

    if (1) {
        if (1) {
            print(33)
        } else {
            print(34)
        }
    } else {
        if (1) {
            print(35)
        } else {
            print(36)
        }
    }

# Tests while statements and nested while statements
    separate(3)
    n = 0
    while (n < 5) {
        print(n)
        n = n + 1
    }

    n = 0
    while (n > 0) {
        print(n)
        n = n + 1
    }

    n = 0
    m = 0
    while (n < 2) {
        m = 0
        while (m <= 2) {
            print(n * 10 + m)
            m = m + 1
        }
        n = n + 1
    }
}

# Main method
fun main() {
    operationTest()
    statementTests()
}

