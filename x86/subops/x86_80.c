#include "../data.h"

// OR r/m(8), imm(8)
void ASM_80OR(RM *rm, s32 disp, u8 val) {
    Reg prev = { 0 };
    Reg res = { 0 };

    s8 sval = (s8)val;
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);
        
        STACK(u8, s, fdisp);
        prev.l = *s;
        *s |= sval;

        rm->val = sval;
        rm->valtype = R_Bit8;
        ASM_rmPrint("OR", rm, disp, v_Val, false);
    } else {
        switch (rm->atype) {
            case R_Bit8:  prev.l = regs[rm->areg].l; regs[rm->areg].l |= sval; break;
            case R_Bit8H: prev.l = regs[rm->areg].h; regs[rm->areg].h |= sval; break;
            default: break;
        }
        
        printf("OR %s, 0x%.2X", ASM_getRegName(rm->areg, rm->atype), sval);
    }

    res.l = prev.l & sval;

    u8 fa = f.f.af;
    u8 fc = f.f.cf;
    ASM_setFlags(&prev, &res, rm->otype, false);
    f.f.af = fa;
    f.f.cf = fc;
    f.f.of = 0;

    ASM_rexPrint();
}

// AND r/m(8), imm(8)
void ASM_80AND(RM *rm, s32 disp, u8 val) {
    Reg prev = { 0 };
    Reg res = { 0 };

    s8 sval = (s8)val;
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);
        
        STACK(u8, s, fdisp);
        prev.l = *s;
        *s &= sval;

        rm->val = sval;
        rm->valtype = R_Bit8;
        ASM_rmPrint("AND", rm, disp, v_Val, false);
    } else {
        switch (rm->atype) {
            case R_Bit8:  prev.l = regs[rm->areg].l; regs[rm->areg].l &= sval; break;
            case R_Bit8H: prev.l = regs[rm->areg].h; regs[rm->areg].h &= sval; break;
            default: break;
        }
        
        printf("AND %s, 0x%.2X", ASM_getRegName(rm->areg, rm->atype), sval);
    }

    res.l = prev.l & sval;

    u8 fa = f.f.af;
    u8 fc = f.f.cf;
    ASM_setFlags(&prev, &res, rm->otype, false);
    f.f.af = fa;
    f.f.cf = fc;
    f.f.of = 0;

    ASM_rexPrint();
}

void ASM_80CMP(RM *rm, s32 disp, u8 val) {
    Reg prev = { 0 };
    Reg res = { 0 };

    s8 sval = val;
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        STACK(u8, s, fdisp);
        prev.l = *s;

        rm->val = val;
        rm->valtype = R_Bit8;
        ASM_rmPrint("CMP", rm, disp, v_Val, false);
    } else {
        switch (rm->otype) {
            case R_Bit8:  prev.l = regs[rm->areg].l; break;
            case R_Bit8H: prev.l = regs[rm->areg].h; break;
            default: break;
        }
        
        printf("CMP %s, 0x%.2X", ASM_getRegName(rm->areg, rm->atype), val);
    }

    s8 _res = (s8)prev.l - sval;
    res.l = _res;

    ASM_setFlags(&prev, &res, rm->otype, true);
    ASM_rexPrint();
}

ASM_valFunc ASM_80Funcs[8] = {
    0, ASM_80OR, 0, 0,
    ASM_80AND, 0, 0, ASM_80CMP,
};