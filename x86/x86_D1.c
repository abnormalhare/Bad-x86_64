#include "data.h"

void ASM_D1SHR(RM *rm, s32 disp) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u64 reg = ASM_getReg(rm->areg, rm->atype);
        if (rm->disp == 1) disp = (s8)disp;

        STACK8(temp, reg + disp); prev.l = *temp; *temp >>= 1;

        rm->val = 1;
        rm->valtype = R_Bit8;
        ASM_rmPrint("SHR", rm, disp, v_Val, false);
    } else {
        switch (rm->otype) {
            case R_Bit8:
                prev.l = regs[rm->areg].l; regs[rm->areg].l >>= 1;
                break;
            case R_Bit8H:
                prev.l = regs[rm->areg].h; regs[rm->areg].h >>= 1;
                break;
            default: break;
        }
        
        printf("SHR %s, 0x%.2X", ASM_getRegName(rm->areg, rm->atype), 1);
    }

    res.l = prev.l >> 1;

    Flags s = { .f.af = f.f.af };
    ASM_setFlags(&prev, &res, rm->otype, false);
    f.f.cf = prev.l & 1;
    f.f.af = s.f.af;
    ASM_rexPrint();
}

ASM_baseFunc ASM_D1Funcs[8] = {
    0, 0, 0, 0,
    0, ASM_D1SHR, 0, 0,
};