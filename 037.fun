# testing name collisions
fun text() {
    return 1
}

fun data() {
    return 2
}

fun global() {
    return 3
}

fun start() {
    start = 2
    return start
}
fun format(rdi, rsi) {
    return rdi + rsi
}
# testing more name collisions with recursion
fun wow(wow) {
    if (wow == 0) {
        return 10
    } else {
        return wow(wow - 1) + 1
    }
}
# test copy by value
fun modifyme(myreg) {
    myreg = 4
}
fun main() {
    format = 4
# 2
    print(start())
# 3
    myreg = global()
    modifyme(myreg)
    print(myreg)
# 5
    print(text() + format(start(), data()))
# 12
    print(format(format(format, format), format))
# 14
    print(wow(format))
}
