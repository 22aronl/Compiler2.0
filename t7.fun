fun recursion(a) {
    if(a > 0) {
        print(a)
        recursion(a - 1)
    }
    else {
        print(a)
    }
}

fun main() {
    print(recursion(5))
}