#include "../data.h"

void ASM_0F_01_2(Data data) {
    if (data.rm_code == 0xD0) {
        u64 ret = _xgetbv(regs[1].e);
        regs[0].e = (u32)(ret >>  0); regs[0].eh = 0;
        regs[2].e = (u32)(ret >> 32); regs[0].eh = 0;

        printf("XGETBV");
    } else {
        printf("UNIMPLEMENTED OPCODE: 0F 01 /2 (%X)", data.rm_code);
    }

    ASM_rexPrint();
    ASM_end();
}

ASM_dataFunc ASM_0F_01Funcs[8] = {
    0, 0, ASM_0F_01_2, 0,
    0, 0, 0, 0,
};