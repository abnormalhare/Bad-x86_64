#include "../data.h"

#include "../alu.h"

void ASM_81ADD(RM *rm, s32 disp, u32 val) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = val;
        rm->valtype = (rm->otype == R_Bit16) ? (R_Bit16) : (R_Bit32);
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
        
        if (rm->otype == R_Bit16) {
            printf("ADD %s, 0x%.4X", ASM_getRegName(rm->areg, rm->atype), (u16)val);
        } else {
            printf("ADD %s, 0x%.8X", ASM_getRegName(rm->areg, rm->atype), (u32)val);
        }
    }

    switch (rm->otype) {
        case R_Bit16: ASM_ADD_FLAGS(16, prev.x, val, (prev.x - val));
        case R_Bit32: ASM_ADD_FLAGS(32, prev.e, val, (prev.e - val));
        case R_Bit64: ASM_ADD_FLAGS(64, prev.r, val, (prev.r - val));
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

        rm->val = val;
        rm->valtype = (rm->otype == R_Bit16) ? (R_Bit16) : (R_Bit32);
        ASM_rmPrint("AND", rm, disp, v_Val, false);

        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); prev.x = *s; *s &= val; break; }
            case R_Bit32: { STACK(u32, s, fdisp); prev.e = *s; *s &= val; break; }
            case R_Bit64: { STACK(u64, s, fdisp); prev.r = *s; *s &= val; break; }
            default: break;
        }
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
        case R_Bit16: ASM_BIT_FLAGS(16, prev.x, val, (prev.x - val));
        case R_Bit32: ASM_BIT_FLAGS(32, prev.e, val, (prev.e - val));
        case R_Bit64: ASM_BIT_FLAGS(64, prev.r, val, (prev.r - val));
        default: break;
    }

    ASM_rexPrint();
}

void ASM_81SUB(RM *rm, s32 disp, u32 val) {
    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = val;
        rm->valtype = (rm->otype == R_Bit16) ? (R_Bit16) : (R_Bit32);
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
        
        if (rm->otype == R_Bit16) {
            printf("SUB %s, 0x%.4X", ASM_getRegName(rm->areg, rm->atype), (u16)val);
        } else {
            printf("SUB %s, 0x%.8X", ASM_getRegName(rm->areg, rm->atype), val);
        }
    }

    switch (rm->otype) {
        case R_Bit16: ASM_SUB_FLAGS(16, prev.x, val, (prev.x - val));
        case R_Bit32: ASM_SUB_FLAGS(32, prev.e, val, (prev.e - val));
        case R_Bit64: ASM_SUB_FLAGS(64, prev.r, val, (prev.r - val));
        default: break;
    }

    ASM_rexPrint();
}

void ASM_81XOR(RM *rm, s32 disp, u32 val) {
    Reg prev = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = val;
        rm->valtype = (rm->otype == R_Bit16) ? (R_Bit16) : (R_Bit32);
        ASM_rmPrint("XOR", rm, disp, v_Val, false);

        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); prev.x = *s; *s ^= val; break; }
            case R_Bit32: { STACK(u32, s, fdisp); prev.e = *s; *s ^= val; break; }
            case R_Bit64: { STACK(u64, s, fdisp); prev.r = *s; *s ^= val; break; }
            default: break;
        }
    } else {
        switch (rm->otype) {
            case R_Bit16:
                prev.x = regs[rm->areg].x; regs[rm->areg].x ^= val;
                break;
            case R_Bit32:
                prev.e = regs[rm->areg].e; regs[rm->areg].e ^= val; regs[rm->areg].eh = 0;
                break;
            case R_Bit64:
                prev.r = regs[rm->areg].r; regs[rm->areg].r ^= val;
                break;
            default: break;
        }
        
        if (rm->otype == R_Bit16) {
            printf("XOR %s, 0x%.4X", ASM_getRegName(rm->areg, rm->atype), (u16)val);
        } else {
            printf("XOR %s, 0x%.8X", ASM_getRegName(rm->areg, rm->atype), val);
        }
    }

    switch (rm->otype) {
        case R_Bit16: ASM_BIT_FLAGS(16, prev.x, val, (prev.x ^ val));
        case R_Bit32: ASM_BIT_FLAGS(32, prev.e, val, (prev.e ^ val));
        case R_Bit64: ASM_BIT_FLAGS(64, prev.r, val, (prev.r ^ val));
        default: break;
    }
    
    ASM_rexPrint();
}

void ASM_81CMP(RM *rm, s32 disp, u32 val) {
    Reg prev = { 0 };
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        rm->val = val;
        rm->valtype = (rm->otype == R_Bit16) ? (R_Bit16) : (R_Bit32);
        ASM_rmPrint("CMP", rm, disp, v_Val, false);

        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); prev.x = *s; break; }
            case R_Bit32: { STACK(u32, s, fdisp); prev.e = *s; break; }
            case R_Bit64: { STACK(u64, s, fdisp); prev.r = *s; break; }
            default: break;
        }
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
        case R_Bit16: ASM_SUB_FLAGS(16, prev.x, val, (prev.x - val));
        case R_Bit32: ASM_SUB_FLAGS(32, prev.e, val, (prev.e - val));
        case R_Bit64: ASM_SUB_FLAGS(64, prev.r, val, (prev.r - val));
        default: break;
    }

    ASM_rexPrint();
}

ASM_val32Func ASM_81Funcs[8] = {
    ASM_81ADD, 0, 0, 0,
    ASM_81AND, ASM_81SUB, ASM_81XOR, ASM_81CMP,
};