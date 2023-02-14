# This is meant to test expressions, including support for
# various types of operators and valid of order of operations

# this prints a really long test expression
fun mainExpression() {
	print((((197361848195278345 > 3876945517329847512) && (3986030017927818654 / 850269298998047709) >= (17910819152759043330 < 15240317884807446591 > 253286750849729078)) / ((13513863325326322571 > 17776389246715158994) - (4314563260089586126 / 3924198571757183096 * 11497521505014767558))) + (((1298479366471561174 && 14016086023329313655 || 2873683290347397692) && (1968556188444835252 * 363339266310839843)) || ((18252789917089688905 / 1299237511954815107) > (1909971575427556573 * 12597992292475906095 >= 5194197173356764415) > (10963659998546509911 + 9762053928979814944 < 10344062207243189361)) + ((4777771452934460886 != 4361513933523450812) >= (15400062089261990373 + 5654605985610049772 * 5754109558981180675))) - (((16577876119443879560 * 8677882841490522039 / 3937250486977817219) + (1573754159894583665 && 7238552898561084409)) + ((7644697298035914677 - 12900603446427625875) / (18411831573798526244 >= 16493121055688997424)) <= ((12045559166026500859 != 15734110462241000468) + (13634138038222558553 * 8960840459719728235 || 12814385849325011156) <= (16018939845035943665 < 14006010353185382273))))
}

# this breaks the above expression down into its components and
# prints the components as it goes so you can debug the above
# expression by using its components
fun brokenDownExpression() {
	a = 197361848195278345 > 3876945517329847512
	print(a)
	b = 3986030017927818654 / 850269298998047709
	print(b)
	c = a && b
	print(c)
	d = 17910819152759043330 < 15240317884807446591
	print(d)
	e = d > 253286750849729078
	print(e)
	f = c >= e
	print(f)
	g = 13513863325326322571 > 17776389246715158994
	print(g)
	h = 4314563260089586126 / 3924198571757183096
	print(h)
	i = h * 11497521505014767558
	print(i)
	j = g - i
	print(j)
	k = f / j
	print(k)
	l = 14016086023329313655 || 2873683290347397692
	print(l)
	m = 1298479366471561174 && l
	print(m)
	n = 1968556188444835252 * 363339266310839843
	print(n)
	o = m && n
	print(o)
	p = 18252789917089688905 / 1299237511954815107
	print(p)
	q = 12597992292475906095 >= 5194197173356764415
	print(q)
	r = 1909971575427556573 * q
	print(r)
	s = p > r
	print(s)
	t = 9762053928979814944 < 10344062207243189361
	print(t)
	u = 10963659998546509911 + t
	print(u)
	v = s > u
	print(v)
	w = o || v
	print(w)
	x = 4777771452934460886 != 4361513933523450812
	print(x)
	y = 15400062089261990373 + 5654605985610049772
	print(y)
	z = y * 5754109558981180675
	print(z)
	ab = x >= z
	print(ab)
	bb = w + ab
	print(bb)
	cb = k + bb
	print(cb)
	db = 7644697298035914677 - 12900603446427625875
	print(db)
	eb = 18411831573798526244 >= 16493121055688997424
	print(eb)
	fb = db / eb
	print(fb)
	gb = 8960840459719728235 || 12814385849325011156
	print(gb)
	hb = 13634138038222558553 * gb
	print(hb)
	ib = 16018939845035943665 < 14006010353185382273
	print(ib)
	jb = hb <= ib
	print(jb)
	kb = 12045559166026500859 != 15734110462241000468
	print(kb)
	lb = kb + jb
	print(lb)
	mb = fb <= lb
	print(mb)
	nb = 16577876119443879560 * 8677882841490522039
	print(nb)
	ob = nb / 3937250486977817219
	print(ob)
	pb = 1573754159894583665 && 7238552898561084409
	print(pb)
	qb = ob + pb
	print(qb)
	rb = qb + mb
	print(rb)
	sb = cb - rb
	print(sb)
}

fun main() {
	mainExpression()
	brokenDownExpression()
}