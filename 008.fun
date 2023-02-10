# this is a brief test of some edge cases (and of comments, implicitly)

fun printf() {
    print(1234567890)
}

fun main() {
# 1: A literal with value a value that requires more than 32 bits.
# this is an edge case if you're pushing literal values onto the stack because the "push"
# instruction only works with <= 32 bit integers. To fix this, move ("mov") the literal
# to a register first and then push that register onto the stack.
    big = 18446744073709551615
    bigSum = 184467440737095516 + 284467440737095516 + 384467440737095516

    print(big)
    print(bigSum)

# 2: A function called printf. Make sure this doesn't conflict with C's printf, which you
# may be calling from your assembly code.
    printf()

# 3: the "mul" and especially "div" instructions are a little different than the other
# operators, so I test them here with some big numbers.
    print(big / 10)
    print(big % 10)
    print(big % (big / 2))
    print(3 * 184467440737095516)

}
