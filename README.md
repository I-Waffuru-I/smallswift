
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
- [x] Variable en constant definities, met of zonder vermelding van het type. Declaraties zonder initiële waarde hoef je niet te ondersteunen.
    - [x] Met type
    - [x] Zonder type
- [x] functions en function calls (call expressions). Ook recursieve calls!
    - [x] function definition 
    - [x] function call 
    - [ ] return statement
- [x] operator expressions: enkel de operators vermeld in de files lexer.c en tokens.h, zie beneden
- [x] Assignment statement (in Swift is een assignment eigenlijk een expressie die tot een soort NULL waarde evalueert, maar dat moet je zo niet implementeren).
- [ ] blocks
    - [x] if-else 
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

## Extra
Sommige waren heel snel gemaakt en als 'bonus' erbij gedaan. 

- [x] do scope. Eens ge een scope hebt voor functies/conditions, is het eenvoudig een nieuwe te maken voor dit
- [x] cstd 'library' die ge kunt inladen door die function te callen. Hier kunnen vanalle types en default functions in zitten, niet enkel print
