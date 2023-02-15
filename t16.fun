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

fun main()
{ 
    print(f6())
}
