#this tests recursion and scoping and reserved keyword checking

fun funRecursion(x, y) {
    if (x < y) {
        print(x)
        return x
    }
    return funRecursion(x/y, y)
}

fun whileAlternator(y) {
    return !y
}

fun main() {
    x = funRecursion(4096, 2)
    print(x)

    y = x && 0 || x > !x

    y = 1
    while (x < 4096) {
        y = whileAlternator(y)
        x = x * 2
        print(x)
    }

    if (y) {
        print(y)
    } else {
        print(0000000)
    }
}
