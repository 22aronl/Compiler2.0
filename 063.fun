#test all operations
fun operations(){
    print(90-9)
    print(90+9)
    print(90*9)
    print(90/9)
    print(90%9)
    print(90+9*9)
    print((90+9)*9)
    print(!90)
    print(!!90)
    print(90<=90)
    print(90<90)
    print(90>=90)
    print(90>90)
    print(90==90)
    print(90!=90)
    print(1 && 1)
    print(1 || 0)
}

#test if, while statements
fun loopytest(){
    i = 0
    while(i<10){
        print(i)
        i = i + 1
    }
    i = 0
    while(i<10){
        if(i%2 == 0){
            print(i)
        }
        i = i + 1
    }
    i = 0
    while(i<10){
        if(i%2 == 0){
            print(i)
        }else{
            print(i+1)
        }
        i = i + 1
    }
    i = 0
    while(i<10){
        if(i%2 == 0){
            print(i)
        }else{
            if(i%3 == 0){
                print(i+1)
            }else{
                print(i+2)
            }
        }
        i = i + 1
    }

    if (1){
    print(5000)
}
if (0){
    print(10000)
}
i = 5
while (i > 0){
    i = i -1
    print(i)
}

ifA = 5
print(ifA)

if(0){
    print(69)
} else{
    print(42)
}

}

#test functions
fun add(a,b){
    c = 100
    return a+b+c
}

fun simple(a,b){
    print(1+add(a,b))
}

fun rec(lol){
    if(lol == 0){
        print(0)
    } else{
        print(lol)
        rec(lol-1)
    }
}


fun rec2(r){
    print(r)
    if(r == 0){
        return 0
    }
    return rec2(r-1) + r
}

fun one() {
    print(1)
    return 1
}

fun zero() {
    print(0)
    return 0
}

fun testBool() {
    print(one() && zero())
    print(one() || zero())
    print(!zero())
    print(!one())
    if(one() && zero()){
    print(1)
} else{
    print(0)
}

if(one() || zero()){
    print(1)
} else{
    print(0)
}

while(zero()){}

one = 11
zero = 22
print(one*zero)
printif = one * one()
ifif = one() * zero
elseif = zero() * one()
returnif = 1000*(one() + zero()) +  100*(one() && zero())
funif = 1000*(((one() + zero()) + 100*(one() && zero())) + 10*(one() * zero())) + (one() || zero())

print(printif)
print(ifif)
print(elseif)
print(returnif)
print(funif)

}

fun main(){
#test all operations
    operations()
#test if, while statements
    loopytest()
#test functions
    simple(1,2)
    rec(10)
    print(rec2(10))
    testBool()
}
