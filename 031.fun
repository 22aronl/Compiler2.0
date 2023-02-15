########### testing # many # hashtags # in # a # comment

# make sure we can name the function 'format'
fun format() {
# make sure if doesn't return if the statement is false
    if(0) {
        return 2
    }
# make sure while terminates upon return
    while(1) {
        return 1
    }
}

# basic if, parameters, return test
fun a(y, z) 
# comment to test spacing issues
{
    x = 5
    y = x
    if(z) {
        return y + x
    }
    else 
# comment to test spacing issues    


    {
        return x
    }
}

# testing that you can call functions declared below
fun b(x) {
    if(x <= 3) {
        return 3 * x
    }
    return c(x - 1)
}

fun c(x) {
#testing that functions can call each other
    if(x <= 2) {
        return 2 * x
    }
    return b(x - 1)
}

fun main() {
    print(format())
    print(a(1, 2))
    print(a(2, 0))

#testing if variables can be the same name as functions
    a = 3
    format = 5

#testing basic precedence, operators, etc
    print(format - a % (format / 2))
    print(format < a >= format - format)
    print(a(1, 1) - format % a)
    print(a(1, 1) % (2 * format() + format))

#testing that functions can call each other
    print(b(5))
    print(b(6))
}
