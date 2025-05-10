#include "../data.h"



// TEST r/m(16-64), r(16-64)
void ASM_F6TEST(RM *rm, s32 disp, u8 val) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        s64 fdisp = ASM_getDisp(rm, disp);

        STACK8(temp, fdisp);
        prev.l = *temp;

        rm->val = val;
        rm->valtype = R_Bit8;
        ASM_rmPrint("TEST", rm, disp, v_Val, false);
    } else {
        switch (rm->otype) {
            case R_Bit8: prev.l = regs[rm->areg].l; break;
            case R_Bit8H: prev.l = regs[rm->areg].h; break;
            default: break;
        }
        printf("TEST %s, %.2X", ASM_getRegName(rm->areg, rm->atype), val);
    }

    res.l = prev.l & val;

    u8 fa = f.f.af;
    ASM_setFlags(&prev, &res, rm->otype, false);
    f.f.af = fa;

    ASM_rexPrint();
    ASM_end();
}

ASM_valFunc ASM_F6Funcs[8] = {
    ASM_F6TEST, ASM_F6TEST, 0, 0,
    0, 0, 0, 0,
};