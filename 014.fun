# this test tests order of evaluation in expressions and that you don't short circuit

fun printMultiple(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) {
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
}

fun identity(x) {
    print(x)
    return x
}

fun main() {
    printMultiple(
        identity(1),
        identity(2),
        identity(3) + identity(4),
        identity(5) || identity(6),
        identity(0) && identity(7),
        identity(1) + identity(2) / identity(3) + (identity(4) % (identity(5) * ((identity(6) - identity(7)) + identity(8))) <= identity(9) >= identity(10) == identity(11)),
        identity(1) == (identity(2) / identity(3)),
        identity(8),
        identity(9),
        identity(10)
    )
}
