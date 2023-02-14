# This tests #dealing with ##many parameters and #values
fun f1(p1, p2,p3,     p4, p5,		p6, p7,p8,p9,p10)
{
	if(p1 == 0)
	{
		print(p1+p2+p3+p4+p5+p6+p7+p8+p9+p10)
	}
	a=1
	b=2
	c=3
	d=4
	e=5
	f=6
	g=7
	h=8
	i=9
	print(i-h+g-f+e-d+c-b+a)
}

# Tests if and whiles within functions
fun f2()
{
	i = 1
	while(i > 10)
	{
		i = i / 10
	}
	while(i < 10)
	{
		i = i + 1
	}
	if(i != 10)
	{
		return 0 + 	1
	}
	else
	{
		return 10 - 1
	}
}

#Tests functions calling other functions, simple operations, and local variables with the same name as those in other functions
fun f3()
{
	a = f1(0,10 ,10,10,10,10,10,10,10,11)
	print(a)
	a = 101
	print(a+2)
	print(a-2)
	print(a*2)
	print(a/2)
	print(a%2)
	print(a&&2)
	print(!a)
	print(!a&&2)
	print(a||2)
	print(a < 2)
	print(!a < 2)
	print(a <= 2)
	print(a > 2)
	print(a >= 2)
}

fun f4(v1, v2)
{
	if((v1+v2)/2)
	{
		return v1+v2
	}
}

fun fibo100(first,  second)
{
	if(!(first > 100 || second > 100))
	{
		print(first + second)
		fibo100(second, first+second)
	}
}

fun main()
{
	print(f2())
	f3()
# Test with big numbers
	a = 18446744073709551615/18446744073709551615
	print(a)
	a = 18446744073709551615 + 1
	print(a)
	a = 18446744073709551615%18446744073709551615
	print(a)
# Order of operations
	A = 15
	c = 3
	orderOps1 = (((A* 2*!(0&& c) + 8)))
	print(orderOps1+10)

	orderOps2= ((((4))-3))

	print(orderOps2)
	orderOps3 = 0 || 1 && 0 != 0 <= 7-8+13%5*!0
	print(orderOps3)
	orderOps4 = 2*(f4(4,5) - f4(0,1)) + A
	print(orderOps4)
#Bad variable names
	ifValif = 10
	elseValelse = 5
	returnValreturn = 3
	funValwhile = 2
	print(ifValif + elseValelse + returnValreturn + funValwhile)
	
	fibo100(0,1)
}

