# This test ensures that stacked calls to functions with large numbers of parameters function correctly

# Taken from "Elegy for Hallownest" from Hollow Knight by Team Cherry
fun a(In, wilds, beyond, they, speak, your, name, with, reverence, and, regret){
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


# You should get 31
fun main(){
    print (a(1, 2, 4, 2, 5, 6, 7, 8, 9, 10, 3))
}
