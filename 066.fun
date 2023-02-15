# this test looks for basic functionality on the spec
# the recursion also tries confusing the compiler into messing up its registers for non-stack machines with alternating inputs

# if and while loop testing
fun prime(p) {
    if(p <= 2) {
        return 1
    }
    if(p % 2 == 0) {
        return 0
    }
    i = 3
    while((i * i) <= p) {
        if(p % i == 0) {
            return 0
        }
        i = i + 2
    }
    return 1
}

# recursive alternating testing
fun gcd(a, b) {
    if(b == 0) {
        return a
    }
    return gcd(b, a % b)
}

fun checkGCDPrime(a, b) {
    print(a)
    print(b)

    g = gcd(a, b)
    print(prime(g))
}

fun main() {
    size = 20
    x = 1
    while(x <= size) {
        y = x
        while(y <= size) {
            checkGCDPrime(x,y)
            y = y + 1
        }
        x = x + 1
    }
}
