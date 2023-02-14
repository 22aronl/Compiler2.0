#do you overflow properly? can you handle recursion?
fun pow(x, n) {
	if(!n) {
		return 1
	}
	return x * pow(x, n - 1)
}
#do you do unsigned comparison?
fun cmp() {
	return 0 - 1 < 0
}
#do you do unsigned division?
fun div() {
	return (0 - 1) / 2
}
#can you handle big statements?
fun big() {
	return 0 + (1 - (2 * (3 % (4 / (5 || (6 && (7 > (8 < (9 >= (10 <= (11 == (12 != (!13)))))))))))))
}
#can you handle nested loops?
fun loops() {
	x = 0
	while (x < 2) {
		y = 0
		while(y < 2) {
			print(x)
			print(y)
			y = y + 1
		}
		x = x + 1
	}
}
#can you handle nested if elses?
fun ifelses(x, y) {
	if (x) {
		if(y){
			return 0
		}
		else {
			return 1
		}
	}
	else {
		if (y) {
			return 2
		}
		else {
			return 3
		}
	}
}

#can you handle many parameters?
fun params(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z) {
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
	print(r)
	print(s)
	print(t)
	print(u)
	print(v)
	print(w)
	print(x)
	print(y)
	print(z)
}
#can you handle many locals?
fun locals() {
	a = 0
	b = 1
	c = 2
	d = 3
	e = 4
	f = 5
	g = 6
	h = 7
	i = 8
	j = 9
	k = 10
	l = 11
	m = 12
	n = 13
	o = 14
	p = 15
	q = 16
	r = 17
	s = 18
	t = 19
	u = 20
	v = 21
	w = 22
	x = 23
	y = 24
	z = 25
	params(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z)
}

#can you handle weird function names?
fun printf() {}
fun data() {}
fun text() {}
fun global() {}

#putting it all together
fun main() {
	print(pow(3, 1000))
	print(cmp())
	print(div())
	print(big())
	loops()
	print(ifelses(0, 0))
	print(ifelses(0, 1))
	print(ifelses(1, 0))
	print(ifelses(1, 1))
	params(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25)
	locals()
	print(printf())
	print(data())
	print(text())
	print(global())
	print(0)
}
