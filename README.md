# RISC-V Disassembler

In 2023 I wrote about 200 lines of C++ that turn RISC-V machine code back into instructions. It reads 32-bit RV32I words from a binary file and prints one line per instruction, with register numbers.

It covers register arithmetic, loads, immediate arithmetic, stores, branches and `JAL`. The comment at the top of `main.cpp` still says `//work in progress`, and the code has known bugs: load immediates are not sign-extended, so `lw a0, -4(s0)` comes out with an offset of 4092, and the B-type immediate is assembled wrong, so backward branches look like long forward ones.

## The 2026 port

The finished version is a JavaScript port with the bugs fixed. It decodes all six formats, the 40 base instructions plus RV32M, and prints register names. Its test lines were assembled with `llvm-mc`. It runs in the browser at [schliewesystems.com/lab](https://schliewesystems.com/lab), and nothing you paste or open is uploaded.

The write-up: [Reading RISC-V by hand: finishing a disassembler from 2023](https://schliewesystems.com/blog/reading-risc-v-by-hand).

## Build and run

Open `binary-file.sln` in Visual Studio, or build it with any C++17 compiler:

```sh
clang++ -std=c++17 -o disasm main.cpp
./disasm    # reads test_instructions.bin from the current folder
```

`test_instructions.bin` holds six instructions: a `nop`, an `addi`, an `add`, a `sub`, a `jalr` and a `jal`.
