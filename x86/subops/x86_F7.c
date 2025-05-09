#include "data.h"

void ASM_F7NOT(RM *rm, s32 disp, u8 val) {
    if (rm->isPtr) {
        u64 reg = ASM_getReg(rm->areg, rm->atype);
        if (rm->disp == 1) disp = (s8)disp;
        
        switch (rm->otype) {
            case R_Bit16: { STACK16(temp, reg + disp); *temp = ~*temp; break; }
            case R_Bit32: { STACK32(temp, reg + disp); *temp = ~*temp; break; }
            case R_Bit64: { STACK64(temp, reg + disp); *temp = ~*temp; break; }
            default: break;
        }

        ASM_rmPrint("NOT", rm, disp, v_None, false);
    } else {
        switch (rm->otype) {
            case R_Bit16: regs[rm->areg].x = ~regs[rm->areg].x; break;
            case R_Bit32: regs[rm->areg].e = ~regs[rm->areg].e; regs[rm->areg].eh = 0; break;
            case R_Bit64: regs[rm->areg].r = ~regs[rm->areg].r; break;
            default: break;
        }
        printf("NOT %s", ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

ASM_valFunc ASM_F7Funcs[8] = {
    0, 0, ASM_F7NOT, 0,
    0, 0, 0, 0,
};