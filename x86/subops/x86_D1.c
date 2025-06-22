#include "../data.h"

void ASM_D1SHR(RM *rm, s32 disp) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = 1;
        rm->valtype = R_Bit8;
        ASM_rmPrint("SHR", rm, disp, v_Val, false);

        switch (rm->otype) {
            case R_Bit16: {
                STACK(u16, s, fdisp);
                prev.x = *s; *s >>= 1;
                break;
            } case R_Bit32: {
                STACK(u32, s, fdisp);
                prev.x = *s; *s >>= 1;
                break;
            } case R_Bit64: {
                STACK(u64, s, fdisp);
                prev.x = *s; *s >>= 1;
                break;
            } default: break;
        }
    } else {
        switch (rm->otype) {
            case R_Bit16:
                prev.x = regs[rm->areg].x; regs[rm->areg].x >>= 1;
                break;
            case R_Bit32:
                prev.e = regs[rm->areg].e; regs[rm->areg].e >>= 1;
                break;
            case R_Bit64:
                prev.r = regs[rm->areg].r; regs[rm->areg].r >>= 1;
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