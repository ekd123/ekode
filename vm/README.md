# Virtual Machine

This is a simple virtual machine. For learning purposes, not intended for
production use.

Try `make test`.

As opposite to XiaoVM which makes use of GLib, this VM is complete ANSI C.

## Goals

* a compiler which turns human-readable code into bytecode
* a bytecode interpreter
* a ASM-like language (later C-like perhaps)

## 1st Design

The first design is just for fun. It's just used to show how stupid I'm
before I really set out to read CSAPP. Mainly modelled after XiaoVM.

### Registers

All are 32-bit wide.

use  no.
data 1~5
pc   6

PC behaves exactly like PC.

### Instructions

NOP: 0x00 do nothing and move forward
MOV: 0x01
ADD: 0x02
SUB: 0x03
MUL: 0x04
DIV: 0x05
JMP: 0x06 you probably want LOOP instead of JMP
LOOP: 0x07 reg: how many times (do not modify), data: jump back
DIE: 0x08

Numbers are little-endian.

"MOV EAX, 100" is (1+1+4)=6 bytes long.

### Example
offset  instruction
0    MOV 1, 100  01 01 100d        ; move 100 into register 1
1    MOV 2, 30   01 02 30d         ; move 30 into register 2
2    ADD 1, 2    02 01 01 00 00 00 ; do it
3    MOV 3, 5
4    LOOP 3, 2
5    DIE
6    JMP #, 0    06 00 01 00 00 00 ; go back, this is a infinite loop
