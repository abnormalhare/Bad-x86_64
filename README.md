# Bad x86_64
The worst way to make an x86_64 emulator. This is a library. The file to include is `x86.c`.

## How to use
```c
#include "x86/x86.h"

// These are codefuncs. Store these in the stack to call them.
void asm_func(void) {
  // ASM_XX() -> An opcode or prefix. This is how the emulator runs.

  // functions with an X, L, or H require the specific opcode run to be input
  // (these functions are: 4X, 5L, 5H, and BH)
  ASM_4X(0x48);
  ASM_01(0xC0, 0x00, NULL); // 0x48 0x01 0xC0 => add rax, rax

  ASM_C3();
}

void setupStack() {
  { STACK8(temp, 0); *temp = 'M'; } // this is how to store data into the stack
  { STACK16(temp, 8); *temp = 0x3F40; }
  { STACK32(temp, 0x24); *temp = 0x46404938; }
  { STACK64(temp, 0x1000); *temp = (u64)&asm_func; } // s and u types are predefined! (s8, s16..., u8, u16...)
}

int main() {
  // initialize the stack
  ASM_init();
  setupStack();

  ASM_incIP(0x1000, NULL); // set instruction pointer
  ASM_codeFunc func = ASM_getCurrFunc(); // get the function at the IP
  func(); // call the function

  // optionally: you can end the program that prints out the final status of the registers
  ASM_regPrint();
}
```

Checkout checklist.txt to see the currently implemented opcodes!