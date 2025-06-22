#include "../data.h"



// TEST r/m(16-64), imm(8)
void ASM_F6TEST(RM *rm, s32 disp, u8 val) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = val;
        rm->valtype = R_Bit8;
        ASM_rmPrint("TEST", rm, disp, v_Val, false);

        STACK(u8, s, fdisp);
        prev.l = *s;
    } else {
        switch (rm->otype) {
            case R_Bit8: prev.l = regs[rm->areg].l; break;
            case R_Bit8H: prev.l = regs[rm->areg].h; break;
            default: break;
        }
        printf("TEST %s, %.2X", ASM_getRegName(rm->areg, rm->atype), val);
    }

    res.l = prev.l & val;

    f.f.zf = (res.l == 0);
    f.f.sf = ((res.l & 0x80) == 0x80);
    f.f.pf = ASM_getParity(res.l);
    f.f.of = 0;
    f.f.cf = 0;

    ASM_rexPrint();
    ASM_end();
}

ASM_valFunc ASM_F6Funcs[8] = {
    ASM_F6TEST, ASM_F6TEST, 0, 0,
    0, 0, 0, 0,
};