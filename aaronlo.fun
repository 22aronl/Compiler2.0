#this tests the stack frame of the function, ensuring no variable overwritten on conditional

fun function(x) {
    if(x == 0) {
        y = 1
    }
    z = 2
    print(x)
    if(x == 0) {
        print(y)
    }
    print(z)

    a = 3
    b = 4

    if(x == 0) {
        y = 5
        print(y)
    }

    print(z)
    print(a)
    print(b)
    return a
}

fun main() {
#insure that function stack frame resets properly
    p = 8
    x = function(1)
#should print 1 2 2 3 4
    a = 9
#should print 0 1 2 5 2 3 4
    y = function(0)
    b = 10
#should print 1 2 3 4
    z = function(1)
    c = 11

#should print 8 3 9 3 10 3 11
    print(p)
    print(x)
    print(a)
    print(y)
    print(b)
    print(z)
    print(c)
}