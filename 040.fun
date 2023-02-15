# Test function names used as labels
fun printf(x) {
    print(x)
}
fun text(y) {
    print(y)
}
fun data(z) {
    print(z)
}
fun format(a) {
    print(a)
}
fun global(b) {
    print(b)
}

# Declare several local variables in the scope
fun severalVariables() {
    a = 1
    b = 2
    c = 3
    d = 4
    e = 5
    f = 6
    g = 7
    h = 8
    i = 9
    j = 10
    k = 11
    l = 12
    m = 13
    n = 14
    o = 15
    p = 16
    q = 17
    r = 18
    s = 19
    t = 20
    u = 21
    v = 22
    w = 23
    x = 24
    y = 25
    z = 26
}

# Force postfix notation to push many variables onto the stack
fun overflowPostfix() {
    x = (!1 + (2 * (3 / (4 - 1 + (5 % (1 + (6 <= (7 < (8 >= (9 > (10 == (11 != (12 && (13 || (14)))))))))))))))
    print(x)
}

# Test weird comment locations
fun weirdComments() 
# Weird Location 1
{
    print(1)
    if(1) 
    # Weird Location 2
    {
        print(2)
    }
    # Weird Location 3
    else
    # Weird Location 4
    {
        print(3)
    }

    x = 0
    while(x < 5) 
    # Weird Location 5
    {
        x = x + 1
        print(x)
    }
}

# Test weird variable names
fun weirdVariableNames() {
    ifelse = 1
    whiletrue = 2
    print(ifelse + whiletrue)
    printf = 5
    print(printf)

}

# Test correct return values among nested ifs and whiles
fun nestedIfsAndWhiles(num) {
    if(num >= 5) {
        i = 5
        while(i > 0) {
            num = num + 1
            if(num == 20) {
                if(i > 3) {
                    return 3
                }
                return 2
            }
            i = i - 1
        }
    } else {
        return 1
    }

    return 0
}

# Test prime factorization for fun
fun primeFactorization(n) {
    i = 2
    while(i * i <= n) {
        while(n % i == 0) {
            print(i)
            n = n / i
        }
        i = i + 1
    }
    if(n > 1) {
        print(n)
    }
}

fun main() {
    severalVariables()
    overflowPostfix()
    weirdComments()
    weirdVariableNames()

    print(nestedIfsAndWhiles(3))
    print(nestedIfsAndWhiles(5))
    print(nestedIfsAndWhiles(3))
    print(nestedIfsAndWhiles(15))
    print(nestedIfsAndWhiles(20))

    primeFactorization(2)
    primeFactorization(10)
    primeFactorization(505)
    primeFactorization(65537)

    printf(1)
    text(2)
    data(3)
    format(4)
    global(5)
}
