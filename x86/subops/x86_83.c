#include "../data.h"

void ASM_83ADD(RM *rm, s32 disp, u8 val) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = val;
        rm->valtype = R_Bit8;
        ASM_rmPrint("ADD", rm, disp, v_Val, false);

        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); prev.x = *s; *s += val; break; }
            case R_Bit32: { STACK(u32, s, fdisp); prev.e = *s; *s += val; break; }
            case R_Bit64: { STACK(u64, s, fdisp); prev.r = *s; *s += val; break; }
            default: break;
        }
    } else {
        switch (rm->otype) {
            case R_Bit16:
                prev.x = regs[rm->areg].x; regs[rm->areg].x += val;
                break;
            case R_Bit32:
                prev.e = regs[rm->areg].e; regs[rm->areg].e += val; regs[rm->areg].eh = 0;
                break;
            case R_Bit64:
                prev.r = regs[rm->areg].r; regs[rm->areg].r += val;
                break;
            default: break;
        }
        
        printf("ADD %s, 0x%.2X", ASM_getRegName(rm->areg, rm->atype), val);
    }

    switch (rm->otype) {
        case R_Bit16: res.x = prev.x + val;
        case R_Bit32: res.e = prev.e + val;
        case R_Bit64: res.r = prev.r + val;
        default: break;
    }

    ASM_setFlags(&prev, &res, rm->otype, false);
    ASM_rexPrint();
}

void ASM_83OR(RM *rm, s32 disp, u8 val) {
    Reg prev = { 0 };
    Reg res = { 0 };

    s8 val2 = val;
    s32 nval = val2;
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = val;
        rm->valtype = R_Bit8;
        ASM_rmPrint("OR", rm, disp, v_Val, false);
        
        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); prev.x = *s; *s |= nval; break; }
            case R_Bit32: { STACK(u32, s, fdisp); prev.e = *s; *s |= nval; break; }
            case R_Bit64: { STACK(u64, s, fdisp); prev.r = *s; *s |= nval; break; }
            default: break;
        }
    } else {
        switch (rm->otype) {
            case R_Bit16:
                prev.x = regs[rm->areg].x; regs[rm->areg].x |= nval;
                break;
            case R_Bit32:
                prev.e = regs[rm->areg].e; regs[rm->areg].e |= nval; regs[rm->areg].eh = 0;
                break;
            case R_Bit64:
                prev.r = regs[rm->areg].r; regs[rm->areg].r |= nval;
                break;
            default: break;
        }
        
        printf("OR %s, 0x%.2X", ASM_getRegName(rm->areg, rm->atype), val);
    }

    switch (rm->otype) {
        case R_Bit16: res.x = prev.x | nval;
        case R_Bit32: res.e = prev.e | nval;
        case R_Bit64: res.r = prev.r | nval;
        default: break;
    }

    u8 fa = f.f.af;
    u8 fc = f.f.cf;
    ASM_setFlags(&prev, &res, rm->otype, false);
    f.f.af = fa;
    f.f.cf = fc;
    f.f.of = 0;

    ASM_rexPrint();
}

void ASM_83AND(RM *rm, s32 disp, u8 val) {
    Reg prev = { 0 };
    Reg res = { 0 };

    s8 sval = (s8)val;
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = sval;
        rm->valtype = R_Bit8;
        ASM_rmPrint("AND", rm, disp, v_Val, false);
        
        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); prev.x = *s; *s &= sval; break; }
            case R_Bit32: { STACK(u32, s, fdisp); prev.e = *s; *s &= sval; break; }
            case R_Bit64: { STACK(u64, s, fdisp); prev.r = *s; *s &= sval; break; }
            default: break;
        }
    } else {
        switch (rm->otype) {
            case R_Bit16:
                prev.x = regs[rm->areg].x; regs[rm->areg].x &= sval;
                break;
            case R_Bit32:
                prev.e = regs[rm->areg].e; regs[rm->areg].e &= sval; regs[rm->areg].eh = 0;
                break;
            case R_Bit64:
                prev.r = regs[rm->areg].r; regs[rm->areg].r &= sval;
                break;
            default: break;
        }
        
        printf("AND %s, 0x%.2X", ASM_getRegName(rm->areg, rm->atype), sval);
    }

    switch (rm->otype) {
        case R_Bit16: res.x = prev.x & sval;
        case R_Bit32: res.e = prev.e & sval;
        case R_Bit64: res.r = prev.r & sval;
        default: break;
    }

    u8 fa = f.f.af;
    u8 fc = f.f.cf;
    ASM_setFlags(&prev, &res, rm->otype, false);
    f.f.af = fa;
    f.f.cf = fc;
    f.f.of = 0;

    ASM_rexPrint();
}

void ASM_83SUB(RM *rm, s32 disp, u8 val) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = val;
        rm->valtype = R_Bit8;
        ASM_rmPrint("SUB", rm, disp, v_Val, false);

        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); prev.x = *s; *s -= val; break; }
            case R_Bit32: { STACK(u32, s, fdisp); prev.e = *s; *s -= val; break; }
            case R_Bit64: { STACK(u64, s, fdisp); prev.r = *s; *s -= val; break; }
            default: break;
        }
    } else {
        switch (rm->otype) {
            case R_Bit16:
                prev.x = regs[rm->areg].x; regs[rm->areg].x -= val;
                break;
            case R_Bit32:
                prev.e = regs[rm->areg].e; regs[rm->areg].e -= val; regs[rm->areg].eh = 0;
                break;
            case R_Bit64:
                prev.r = regs[rm->areg].r; regs[rm->areg].r -= val;
                break;
            default: break;
        }
        
        printf("SUB %s, 0x%.2X", ASM_getRegName(rm->areg, rm->atype), val);
    }

    switch (rm->otype) {
        case R_Bit16: res.x = prev.x - val;
        case R_Bit32: res.e = prev.e - val;
        case R_Bit64: res.r = prev.r - val;
        default: break;
    }

    ASM_setFlags(&prev, &res, rm->otype, true);
    ASM_rexPrint();
}

void ASM_83CMP(RM *rm, s32 disp, u8 val) {
    Reg prev = { 0 };
    Reg res = { 0 };

    s8 sval = val;
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = val;
        rm->valtype = R_Bit8;
        ASM_rmPrint("CMP", rm, disp, v_Val, false);

        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); prev.x = *s; break; }
            case R_Bit32: { STACK(u32, s, fdisp); prev.e = *s; break; }
            case R_Bit64: { STACK(u64, s, fdisp); prev.r = *s; break; }
            default: break;
        }
    } else {
        switch (rm->otype) {
            case R_Bit16: prev.x = regs[rm->areg].x; break;
            case R_Bit32: prev.e = regs[rm->areg].e; break;
            case R_Bit64: prev.r = regs[rm->areg].r; break;
            default: break;
        }
        
        printf("CMP %s, 0x%.2X", ASM_getRegName(rm->areg, rm->atype), val);
    }

    switch (rm->otype) {
        case R_Bit16: { s16 _res = (s16)prev.x - sval; res.x = _res; break; }
        case R_Bit32: { s32 _res = (s32)prev.e - sval; res.e = _res; break; }
        case R_Bit64: { s64 _res = (s64)prev.r - sval; res.r = _res; break; }
        default: break;
    }

    ASM_setFlags(&prev, &res, rm->otype, true);
    ASM_rexPrint();
}

ASM_valFunc ASM_83Funcs[8] = {
    ASM_83ADD, ASM_83OR , 0, 0,
    ASM_83AND, ASM_83SUB, 0, ASM_83CMP,
};