# bfjit - Brainfuck JIT compiler

This project contains compiler tools for [Brainfuck](https://en.wikipedia.org/wiki/Brainfuck) - an esoteric programming language created in 1993 by Urban Müller.

The main goal of the project was to develop a Just-In-Time compiler for the language, but it also includes an interpreter.

### Hello World!
```brainfuck
++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
```
```pwsh
"Hello World!"
```

Assembly instructions generated using [FASM (flat assembler)](https://flatassembler.net/) - an assembly language compiler for the x86 architecture processors.

### Missing features:
- Support other targets besides Windows x86-64.
- Emit to an executable (Ahead-Of-Time compilation).