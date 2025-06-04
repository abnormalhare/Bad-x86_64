#include "../data.h"

void ASM_81ADD(RM *rm, s32 disp, u32 val) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);
        
        switch (rm->otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; *temp += val; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; *temp += val; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; *temp += val; break; }
            default: break;
        }

        rm->val = val;
        rm->valtype = (rm->otype == R_Bit16) ? (R_Bit16) : (R_Bit32);
        ASM_rmPrint("ADD", rm, disp, v_Val, false);
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
        
        if (rm->otype == R_Bit16) {
            printf("ADD %s, 0x%.4X", ASM_getRegName(rm->areg, rm->atype), (u16)val);
        } else {
            printf("ADD %s, 0x%.8X", ASM_getRegName(rm->areg, rm->atype), (u32)val);
        }
    }

    switch (rm->otype) {
        case R_Bit16: res.x = prev.x + val;
        case R_Bit32: res.e = prev.e + val;
        case R_Bit64: res.r = prev.r + (u32)val;
        default: break;
    }

    ASM_setFlags(&prev, &res, rm->otype, false);
    ASM_rexPrint();
}

void ASM_81AND(RM *rm, s32 disp, u32 val) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        switch (rm->otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; *temp &= val; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; *temp &= val; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; *temp &= val; break; }
            default: break;
        }

        rm->val = val;
        rm->valtype = (rm->otype == R_Bit16) ? (R_Bit16) : (R_Bit32);
        ASM_rmPrint("AND", rm, disp, v_Val, false);
    } else {
        switch (rm->otype) {
            case R_Bit16:
                prev.x = regs[rm->areg].x; regs[rm->areg].x &= val;
                break;
            case R_Bit32:
                prev.e = regs[rm->areg].e; regs[rm->areg].e &= val; regs[rm->areg].eh = 0;
                break;
            case R_Bit64:
                prev.r = regs[rm->areg].r; regs[rm->areg].r &= val;
                break;
            default: break;
        }
        
        if (rm->otype == R_Bit16) {
            printf("AND %s, 0x%.4X", ASM_getRegName(rm->areg, rm->atype), (u16)val);
        } else {
            printf("AND %s, 0x%.8X", ASM_getRegName(rm->areg, rm->atype), val);
        }
    }

    switch (rm->otype) {
        case R_Bit16: res.x = prev.x & val;
        case R_Bit32: res.e = prev.e & val;
        case R_Bit64: res.r = prev.r & val;
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

void ASM_81SUB(RM *rm, s32 disp, u32 val) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        switch (rm->otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; *temp -= val; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; *temp -= val; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; *temp -= val; break; }
            default: break;
        }

        rm->val = val;
        rm->valtype = (rm->otype == R_Bit16) ? (R_Bit16) : (R_Bit32);
        ASM_rmPrint("SUB", rm, disp, v_Val, false);
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
        
        if (rm->otype == R_Bit16) {
            printf("SUB %s, 0x%.4X", ASM_getRegName(rm->areg, rm->atype), (u16)val);
        } else {
            printf("SUB %s, 0x%.8X", ASM_getRegName(rm->areg, rm->atype), val);
        }
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

void ASM_81CMP(RM *rm, s32 disp, u32 val) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        switch (rm->otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; break; }
            default: break;
        }

        rm->val = val;
        rm->valtype = (rm->otype == R_Bit16) ? (R_Bit16) : (R_Bit32);
        ASM_rmPrint("CMP", rm, disp, v_Val, false);
    } else {
        switch (rm->otype) {
            case R_Bit16:
                prev.x = regs[rm->areg].x;
                break;
            case R_Bit32:
                prev.e = regs[rm->areg].e;
                break;
            case R_Bit64:
                prev.r = regs[rm->areg].r;
                break;
            default: break;
        }
        
        if (rm->otype == R_Bit16) {
            printf("CMP %s, 0x%.4X", ASM_getRegName(rm->areg, rm->atype), (u16)val);
        } else {
            printf("CMP %s, 0x%.8X", ASM_getRegName(rm->areg, rm->atype), val);
        }
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

ASM_val32Func ASM_81Funcs[8] = {
    ASM_81ADD, 0, 0, 0,
    ASM_81AND, ASM_81SUB, 0, ASM_81CMP,
};