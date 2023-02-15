fun a(x)
{
	y = x
	while(y + 1000 - 10000 < 1000000)
	{
		x = x + 2
		z = x + x + 2+ 4+ 5 - 3
		y = y + x + z
	}
	b(y)
}

fun b(x)
{
	x = y
	z = y
	while(y > 10)
	{
		y = y - 1
		z = z + 2
		z = z - y + 2		
	}

}

fun main1()
{
	x = 1000000
	while(x > 0)
	{
		x = x - 1
		a(x)
	}
	
}

fun main2()
{
	x = 0
	while(x < 15)
	{
		x = x + 1
		main1()
	}

}

fun main()
{
	main2()
	print(3)
}
