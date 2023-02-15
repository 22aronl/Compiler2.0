
# tests that operations are implemented properly
fun f2(a, b) {
    c = !a
    d = !c
    e = !!d
    f = !!!e
    g = f
    h = a * b + a - a / b
    i = a > b
    j = a < b
    k = a >= b
    l = a <= b
    m = a && b
    n = a || b
    o = a % b
    p = a == b
    q = a != b
    print(a)
    print(b)
    print(c)
    print(d)
    print(e)
    print(f)
    print(g)
    print(h)
    print(i)
    print(j)
    print(k)
    print(l)
    print(m)
    print(n)
    print(o)
    print(p)
    print(q)
    print(11111)
}

#label collisions
fun printf(a)
{
    print(1)
    return 77
}

# printn returns 0
fun f3()
{
    printf = 10
    print(printf)
    printf(printf)
    return 22
}

# if nesting
fun f4()
{
    a = 0
    b = 10
    c = 0
    if (a < 1)
    {
        if (1 > a)
        {
            if (b > 1)
            {
                if (1 < b)
                {
                    if (c > 1)
                    {
                        if (1 < c)
                        {
                            print(1)
                            return 15
                        }
                        else 
                        {
                            print(2)
                            return 14
                        }
                    }
                    else 
                    {
                        print(3)
                        return 13
                    }
                }
                else 
                {
                    print(4)
                    return 12
                }
            }
            else 
            {
                print(5)
                return 11
            }
        }
        else 
        {
            print(6)
            return 10
        }
    }
    else 
    {
        print(7)
        return 9
    }
    return 8
}

fun f5()
{
    a = 10
    b = 20
    c = 30
    d = 40
    sum = 0
    while (a > 0)
    {
        while (b > 0)
        {
            while (c > 0)
            {
                while (d > 0)
                {
                    sum = sum + 1
                    d = d - 1
                }
                d = 40
                c = c - 1
            }
            c = 30
            b = b - 1
        }
        b = 20
        a = a - 1
    }

    print(sum + sum * sum)
    return sum
}

fun f6()
{
    a = 1
    while (a > 0)
    {
        if (a % 5 == 0)
        {
            return a
        }
        else
        {
            a = a * (a + 1)
            print(a)
        }
    }
}

fun main() {
    print(f2(20, 10))
    print(f2(0, 3))
    print(f3())
    print(f4())
    print(f5())
    print(f6())
}

