
// var blc = 5
// do {
//     blc = blc * 10
// }
//print(blc)

var b = 10
var c = 20

func foo(bar: Int)-> Int{
    b = b + bar * 2
    c = b + 500
    if (b < c) {
        return c
    }
    return b
}

func foo2(bar: Int) {
    foo(bar:bar);
    foo(bar:bar*10);

/*
struct Closure : Callable {
  FunDeclare *decl;
  context bij declaration
  std::shared_ptr<Context> env;

  Closure(FunDeclare *fn, std::shared_ptr<Context> parent)
      : decl(fn), env(parent) {}

  Value call(std::unordered_map<str, Value> &args, Context &c) override;
}
*/
    func foo3() {
        print(9999);
    }
    foo3()
    print(bar)
}
let f = foo2
print(foo)
print(foo(bar:1))

f(bar:5)



