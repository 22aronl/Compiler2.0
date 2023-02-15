# ===== ASCII Letters =====
# each character is converted from black and white to a 15x8 grid of 1s and 0s
# this makes a total of 120 bits, which can be split into 2 60-bit integers and converted to base 10 to be stored as literals in fun
# each 60-bit 15x4 chunk is stored like (high order bits) <line 4><line 3><line 2><line 1> (low order bits), where each line's low-order bits are the right of the output

# prints 60 bits of ascii data, with each line having a width of 15
fun printAscii(data) {
	width = 15
	height = 4

	y = 0
	while (y < height) {
# convert each line to 7s as 0s and 8s as 1s to make the art
		line = 0
		placeValue = 1
		x = 0
		while (x < width) {
			line = line + placeValue * (7 + data % 2)
			data = data / 2
			placeValue = placeValue * 10
			x = x + 1
		}
		print(line)
		y = y + 1
	}
}

# prints 2 ascii characters, used to shorten each letter function
fun pa(n1, n2) {
	printAscii(n1)
	printAscii(n2)
}

# all of the following functions print a single letter, based on their generated bit patterns
fun printA() {
	pa(19140642028388352, 2207714641904)
}
fun printB() {
	pa(70932845248905216, 2181877728280)
}
fun printC() {
	pa(72060918409199616, 1082433210368)
}
fun printD() {
	pa(108934140424880128, 4363788225560)
}
fun printE() {
	pa(71495743662981120, 2190466876416)
}
fun printF() {
	pa(54044844862734336, 1649317775344)
}
fun printG() {
	pa(216176106485055488, 2181944318072)
}
fun printH() {
	pa(143837037505282048, 3324406139928)
}
fun printI() {
	pa(4503737070518272, 137443147904)
}
fun printJ() {
	pa(1125934273921024, 1030845628448)
}
fun printK() {
	pa(49540901606260736, 1125318133440)
}
fun printL() {
	pa(54044844846219264, 2181893719552)
}
fun printM() {
	pa(235743019877138432, 6747610028908)
}
fun printN() {
	pa(122445763925770240, 3358769024216)
}
fun printO() {
	pa(216598318950121472, 2181944842252)
}
fun printP() {
	pa(36874347297112064, 1099545184240)
}
fun printQ() {
	pa(216598318950121472, 6757581385897996)
}
fun printR() {
	pa(143555562560815104, 3324406926432)
}
fun printS() {
	pa(67555111168638976, 2181877727280)
}
fun printT() {
	pa(4503737200279552, 137443147904)
}
fun printU() {
	pa(108934140393160704, 2164764970008)
}
fun printV() {
	pa(55735369760768000, 481046823776)
}
fun printW() {
	pa(156654506432593920, 3874146229036)
}
fun printX() {
	pa(15763526442811392, 3324323037632)
}
fun printY() {
	pa(11260699976925184, 137443147904)
}
fun printZ() {
	pa(6755433867378688, 4389506908544)
}
fun printSpace() {
	pa(0, 0)
}

fun printHelloWorld() {
	printH()
	printE()
	printL()
	printL()
	printO()

	printSpace()

	printW()
	printO()
	printR()
	printL()
	printD()
}

# ===== Other Tests =====

# prints multiple lines out from parameters, in order
# this ensures that you are properly storing/retrieving parameter values, past what would likely fit inside of registers
fun printMultiple(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) {
	print(a1)
	print(a2)
	print(a3)
	print(a4)
	print(a5)
	print(a6)
	print(a7)
	print(a8)
	print(a9)
	print(a10)
}

# prints and returns the argument, used for the side effect during math operations
fun identity(x) {
	print(x)
	return x
}

fun main() {
# this test tests order of evaluation in expressions and that you don't short circuit
# the below block is a commented version of the following function call with arguments separated onto multiple lines for readability (because splitting the actual call into multiple lines is not allowed)
#	printMultiple(
#		identity(1),
#		identity(2),
#		identity(3) + identity(4),
#		identity(5) || identity(6),
#		identity(0) && identity(7),
#		identity(1) + identity(2) / identity(3) + (identity(4) % (identity(5) * ((identity(6) - identity(7)) + identity(8))) <= identity(9) >= identity(10) == identity(11)),
#		identity(1) == (identity(2) / identity(3)),
#		identity(8),
#		identity(9),
#		identity(10)
#	)
	printMultiple(identity(1), identity(2), identity(3) + identity(4), identity(5) || identity(6), identity(0) && identity(7), identity(1) + identity(2) / identity(3) + (identity(4) % (identity(5) * ((identity(6) - identity(7)) + identity(8))) <= identity(9) >= identity(10) == identity(11)), identity(1) == (identity(2) / identity(3)), identity(8), identity(9), identity(10))

# these next lines ensure that you are using only unsigned operators (including comparison and division)

# if you fail these comparisons, make sure that you are using below/above (unsigned) instructions instead of less/greater (signed)
	print(1 < 0-1)
	print(1 <= 0-1)
	print(0-1 > 1)
	print(0-1 >= 1)

# signed multiplication is not different, and can be done with either imul or mul
# if you fail this, make sure you are using the div instruction, to perform unsigned division and modulo
	print(2 * (0 - 1))
	print((0-1) / 20)
	print((0-1) % 20)

# ASCII letters for fun
	print(1111111111111111111)
	printHelloWorld()
}
