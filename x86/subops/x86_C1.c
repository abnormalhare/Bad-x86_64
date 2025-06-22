#include "../data.h"

void ASM_C1SHL(RM *rm, s32 disp, u8 val) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = val;
        rm->valtype = R_Bit8;
        ASM_rmPrint("SHL", rm, disp, v_Val, false);

        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); prev.x = *s; *s <<= val; break; }
            case R_Bit32: { STACK(u32, s, fdisp); prev.e = *s; *s <<= val; break; }
            case R_Bit64: { STACK(u64, s, fdisp); prev.r = *s; *s <<= val; break; }
            default: break;
        }
    } else {
        switch (rm->otype) {
            case R_Bit16:
                prev.x = regs[rm->areg].x; regs[rm->areg].x <<= val;
                break;
            case R_Bit32:
                prev.e = regs[rm->areg].e; regs[rm->areg].e <<= val; regs[rm->areg].eh = 0;
                break;
            case R_Bit64:
                prev.r = regs[rm->areg].r; regs[rm->areg].r <<= val;
                break;
            default: break;
        }
        
        printf("SHL %s, 0x%.2X", ASM_getRegName(rm->areg, rm->atype), val);
    }

    switch (rm->otype) {
        case R_Bit16: res.x = prev.x << val;
        case R_Bit32: res.e = prev.e << val;
        case R_Bit64: res.r = prev.r << val;
        default: break;
    }

    Flags s = { .f.of = f.f.of, .f.af = f.f.af, .f.cf = f.f.cf };
    ASM_setFlags(&prev, &res, rm->otype, false);
    f.f.of = s.f.of; f.f.af = s.f.af; f.f.cf = s.f.cf;
    ASM_rexPrint();
}

void ASM_C1SHR(RM *rm, s32 disp, u8 val) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = val;
        rm->valtype = R_Bit8;
        ASM_rmPrint("SHR", rm, disp, v_Val, false);

        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); prev.x = *s; *s >>= val; break; }
            case R_Bit32: { STACK(u32, s, fdisp); prev.e = *s; *s >>= val; break; }
            case R_Bit64: { STACK(u64, s, fdisp); prev.r = *s; *s >>= val; break; }
            default: break;
        }
    } else {
        switch (rm->otype) {
            case R_Bit16:
                prev.x = regs[rm->areg].x; regs[rm->areg].x >>= val;
                break;
            case R_Bit32:
                prev.e = regs[rm->areg].e; regs[rm->areg].e >>= val; regs[rm->areg].eh = 0;
                break;
            case R_Bit64:
                prev.r = regs[rm->areg].r; regs[rm->areg].r >>= val;
                break;
            default: break;
        }
        
        printf("SHR %s, 0x%.2X", ASM_getRegName(rm->areg, rm->atype), val);
    }

    switch (rm->otype) {
        case R_Bit16: res.x = prev.x >> val;
        case R_Bit32: res.e = prev.e >> val;
        case R_Bit64: res.r = prev.r >> val;
        default: break;
    }

    Flags s = { .f.of = f.f.of, .f.af = f.f.af, .f.cf = f.f.cf };
    ASM_setFlags(&prev, &res, rm->otype, false);
    f.f.of = s.f.of; f.f.af = s.f.af; f.f.cf = s.f.cf;
    ASM_rexPrint();
}

ASM_valFunc ASM_C1Funcs[8] = {
    0, 0, 0, 0,
    ASM_C1SHL, ASM_C1SHR, 0, 0,
};