# test recursive function calls with many parameters

fun f8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz) {
	print(0)
}

fun f7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz) {
	f8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz)
}

fun f6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz) {
	f7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz)
}


fun f5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz) {
	f6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz)
}


fun f4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz) {
	f5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz)
}


fun f3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz) {
	f4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz)
}

fun f2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz) {
	f3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz)
}

fun f1(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz) {
	f2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz)
}

# test for conflicting labelling convention  
fun iffun(x, y) {
	if(x == y) {
		print(x+y)
	} else {
		print(x*y)
	}
}
fun mainif() {
	print(1)
}
fun mainif0() {
	print(1)
}
fun mainif1() {
	print(1)
}
fun mainelse() {
	print(1)
}
fun iffunif() {
	print(1)
}
fun iffunif0() {
	print(1)
}
fun iffunif1() {
	print(1)
}
fun iffunelse() {
	print(1)
}
fun IF() {
	print(0)
}
fun IF0() {
	print(0)
}
fun IF1() {
	print(0)
}
fun IF2() {
	print(0)
}
fun if0() {
	print(0)
}
fun if1() {
	print(0)
}
fun if2() {
	print(0)
}


fun whilefun(x, y) {
	a = 0
	while(a < 10) {
		if(x == y) {
			print(x+y)
		} else {
			print(x*y)
		}
		a = a+1
	}
}
fun mainwhile() {
	print(1)
}
fun mainwhile0() {
	print(1)
}
fun mainwhile1() {
	print(1)
}
fun whilefunwhile() {
	print(1)
}
fun whilefunwhile0() {
	print(1)
}
fun whilefunwhile1() {
	print(1)
}
fun WHILE() {
	print(0)
}
fun WHILE0() {
	print(0)
}
fun WHILE1() {
	print(0)
}
fun while0() {
	print(0)
}
fun while1() {
	print(0)
}

# test recursive function calls
fun whileloop(x) {
	if(x > 0) {
		whileloop(x-1)
		print(x)
	}
	return x
}

fun main() {
	f1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52)
	print(whileloop(100))
	
	x = 0
	y = 0
	if(x == y) {
		print(x-y)
	} else {
		print(x+x-y+y)
	}
	while(x < 10) {
		while(y < 10) {
			iffun(x, y)
			mainif()
			mainif0()
			mainif1()
			mainelse()
			iffunif()
			iffunif0()
			iffunif1()
			iffunelse()
			IF()
			IF0()
			IF1()
			IF2()
			if0()
			if1()
			if2()
			whilefun(x, y)
			mainwhile()
			mainwhile0()
			mainwhile1()
			whilefunwhile()
			whilefunwhile0()
			whilefunwhile1()
			WHILE()
			WHILE0()
			WHILE1()
			while0()
			while1()
			y = y+1
			
		}
		x = x+1
	}
}
