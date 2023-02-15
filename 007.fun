# Test 1: this test ensures that stacked calls to functions with large numbers of parameters function correctly

# Taken from "Elegy for Hallownest" from Hollow Knight by Team Cherry
fun a(In, wilds, beyond, they, speak, your, name, with, reverence, and, regret){
    x = 6
    print(x)
    return b(In, wilds, beyond, they, speak, your, name, with, reverence, and, regret, 0)
}

fun b(For, none, could, tame, our, savage, souls, yet, you, the, challenge, met){
    return c(For, none, could, tame, our, savage, souls, yet, you, the, challenge)
}

fun c(Under, palest, watch, you, taught, we, changed, base, instincts, were, redeemed){
    return d(Under, palest, watch, you, taught, we, changed, base, instincts, were, redeemed, 0, 0)
}

fun d(A, world, you, gave, to, bug, and, beast, as, they, had, never, dreamed){
    return A - world + you/gave * (to + bug) - and + beast + as / (they % had)
}

# This test ensures that nested if statements and complex conditional statements work correctly
fun test2(){
    if(1){
        print(1)
        if(!!(5 == 5 > 3)){
            print(1)
        }
        print(0)
        if(((8+4) / 3 == 4) || test2Helper()){
            print(1)
        }
    }
}

# Ensures short circuiting is not utilized
fun test2Helper(){
    print(8)
}

# Test 3: A simple stress test
fun stressTest(a){
    if(!a){
        return 0
    }
    x = 0
    while(x < 1000000){
        x = x + 1
    }
    print(a)
    stressTest(a - 1)
}

# You should get:
# 6 (from test 1)
# 31 (from test 1)
# 1 (from test 2)
# 0 (from test 2)
# 8 (from test 2)
# 1 (from test 2)
# Numbers from 200 to 1 (from test 3) 
fun main(){
    print (a(1, 2, 4, 2, 5, 6, 7, 8, 9, 10, 3))
    test2()
    stressTest(200)
}


