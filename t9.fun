fun pow(x, n) {
        if (!n) {
                return 1
        }
        return x * pow(x, n - 1)
}
fun main() {
        print(pow(2, 16))
}
