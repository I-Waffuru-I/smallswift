
# notes

voor multiline, extra var naar 'result' maken voor context bij te houden? bvb depth, en die meegeven aan scope structs

## LInks

[bison simpleC](https://wischik.com/lu/programmer/re-ast.html#header)
[swiftlang](https://docs.swift.org/latest/documentation/the-swift-programming-language)

# Tasks
## Basics
Basisniveau. Deze features moet je minstens ondersteunen:

- [x] types Int en Bool
    - [x] Int literals: enkel decimaal
    - [x] -Int literal is separate from MIN operation
- [x] Bool literals true en false
- [ ] Variable en constant definities, met of zonder vermelding van het type. Declaraties zonder initiële waarde hoef je niet te ondersteunen.
    - [ ] Met type
    - [x] Zonder type
- [ ] functions en function calls (call expressions). Ook recursieve calls!
- [ ] return statement
- [x] operator expressions: enkel de operators vermeld in de files lexer.c en tokens.h, zie beneden
- [x] Assignment statement (in Swift is een assignment eigenlijk een expressie die tot een soort NULL waarde evalueert, maar dat moet je zo niet implementeren).
- [ ] conditions
    - [ ] if-else 
    - [ ] while loops (while-statement)
- [x] print als ingebouwde functie
- [x] expressions als statements
- [x] line comments



## Intermediate
Deze features zijn optioneel. Je kan er ook uit selecteren; je hoeft ze niet allemaal te doen. Maar wees op de verdediging wel duidelijk wat je wel, en wat je niet ondersteunt. 

- [x] block comments (dit wordt uitgelegd in de lex-manual)
- [ ] optional types, samen met het "if let" statement
- [ ] strings
- [ ] tuples
- [ ] arrays, samen met for loops (for-in-statement)
- [ ] sets
- [ ] dictionaries
- [ ] break, continue
- [ ] enumerations
- [ ] function overloading


