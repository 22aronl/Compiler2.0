fun screwy(a) {
    if(a >= 2) {
        x = 2
        print(a)
    } else {
        screwy(a + 1)
    }
    print(a)
}

fun main() {
    screwy(1)
    screwy(3)
}