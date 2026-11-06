var b = 10
var c = 20
func foo(bar: Int){
    b = b + bar * 2
    c = b + 500
}

foo(bar:5)

print(b)
print(c)
