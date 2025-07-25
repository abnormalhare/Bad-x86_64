#include "../data.h"

void ASM_0F_BABT(RM *rm, s32 disp, u8 val) {
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = val;
        rm->valtype = R_Bit8;
        ASM_rmPrint("BT", rm, disp, v_Val, false);
        
        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); f.f.cf = ((*s & (1 << val)) == (1 << val)); break; }
            case R_Bit32: { STACK(u32, s, fdisp); f.f.cf = ((*s & (1 << val)) == (1 << val)); break; }
            case R_Bit64: { STACK(u64, s, fdisp); f.f.cf = ((*s & (1 << val)) == (1 << val)); break; }
            default: break;
        }
    } else {
        switch (rm->otype) {
            case R_Bit16: f.f.cf = ((regs[rm->areg].x & (1 << val)) == (1 << val)); break;
            case R_Bit32: f.f.cf = ((regs[rm->areg].e & (1 << val)) == (1 << val)); break;
            case R_Bit64: f.f.cf = ((regs[rm->areg].r & (1 << val)) == (1 << val)); break;
            default: break;
        }
        printf("BT %s, 0x%.2X", ASM_getRegName(rm->areg, rm->atype), val);
    }

    f.f.of = 0; f.f.sf = 0; f.f.zf = 0; f.f.af = 0; f.f.pf = 0;

    ASM_rexPrint();
    ASM_end();
}

ASM_valFunc ASM_0F_BAFuncs[4] = {
    ASM_0F_BABT, 0, 0, 0,
};