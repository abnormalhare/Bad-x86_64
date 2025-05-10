#include "x86.h"

// ADD r/m(16-64), r(16-64)
void ASM_01(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    Reg prev = { 0 };
    Reg res = { 0 };

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; *temp += regs[rm.oreg].x; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; *temp += regs[rm.oreg].e; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; *temp += regs[rm.oreg].r; break; }
            default: break;
        }

        ASM_rmPrint("ADD", &rm, disp, v_Reg, false);
    } else {
        switch (rm.otype) {
            case R_Bit16: prev.x = regs[rm.areg].x; regs[rm.areg].x += regs[rm.oreg].x; break;
            case R_Bit32: prev.x = regs[rm.areg].e; regs[rm.areg].e += regs[rm.oreg].e; regs[rm.areg].eh = 0; break;
            case R_Bit64: prev.x = regs[rm.areg].r; regs[rm.areg].r += regs[rm.oreg].r; break;
            default: break;
        }
        printf("ADD %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
    }

    switch (rm.otype) {
        case R_Bit16: res.x = prev.x + regs[rm.oreg].x; prev.x = regs[rm.oreg].x; break;
        case R_Bit32: res.e = prev.e + regs[rm.oreg].e; prev.e = regs[rm.oreg].e; break;
        case R_Bit64: res.r = prev.r + regs[rm.oreg].r; prev.r = regs[rm.oreg].r; break;
        default: break;
    }

    ASM_setFlags(&prev, &res, rm.otype, false);
    ASM_rexPrint();
    ASM_end();
}

// ADD r(16-64), r/m(16-64)
void ASM_03(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    Reg prev = { 0 };
    Reg res = { 0 };

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; regs[rm.oreg].x += *temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; regs[rm.oreg].e += *temp; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; regs[rm.oreg].r += *temp; break; }
            default: break;
        }

        ASM_rmPrint("ADD", &rm, disp, v_Reg, true);
    } else {
        switch (rm.otype) {
            case R_Bit16: prev.x = regs[rm.areg].x; regs[rm.oreg].x += regs[rm.areg].x; break;
            case R_Bit32: prev.x = regs[rm.areg].e; regs[rm.oreg].e += regs[rm.areg].e; regs[rm.oreg].eh = 0; break;
            case R_Bit64: prev.x = regs[rm.areg].r; regs[rm.oreg].r += regs[rm.areg].r; break;
            default: break;
        }
        printf("ADD %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    switch (rm.otype) {
        case R_Bit16: res.x = prev.x + regs[rm.oreg].x; break;
        case R_Bit32: res.e = prev.e + regs[rm.oreg].e; break;
        case R_Bit64: res.r = prev.r + regs[rm.oreg].r; break;
        default: break;
    }

    ASM_setFlags(&prev, &res, rm.otype, false);
    ASM_rexPrint();
    ASM_end();
}

// 0x06-07 invalid

// OR r(16-64), r/m(16-64)
void ASM_0B(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    Reg prev = { 0 };
    Reg res = { 0 };

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; regs[rm.oreg].x |= *temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; regs[rm.oreg].e |= *temp; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; regs[rm.oreg].r |= *temp; break; }
            default: break;
        }

        ASM_rmPrint("OR", &rm, disp, v_Reg, true);
    } else {
        switch (rm.otype) {
            case R_Bit16: prev.x = regs[rm.areg].x; regs[rm.oreg].x |= regs[rm.areg].x; break;
            case R_Bit32: prev.x = regs[rm.areg].e; regs[rm.oreg].e |= regs[rm.areg].e; regs[rm.oreg].eh = 0; break;
            case R_Bit64: prev.x = regs[rm.areg].r; regs[rm.oreg].r |= regs[rm.areg].r; break;
            default: break;
        }
        printf("OR %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    switch (rm.otype) {
        case R_Bit16: res.x = prev.x | regs[rm.oreg].x; break;
        case R_Bit32: res.e = prev.e | regs[rm.oreg].e; break;
        case R_Bit64: res.r = prev.r | regs[rm.oreg].r; break;
        default: break;
    }

    ASM_setFlags(&prev, &res, rm.otype, false);
    f.f.cf = 0; f.f.of = 0;
    ASM_rexPrint();
    ASM_end();
}

void ASM_0C(u8 val) {
    ASM_incIP(2, NULL);

    Reg prev = { .l = regs[0].l };
    Reg res = { 0 };

    regs[0].l |= val;
    res.l = prev.l | val;

    printf("OR al, 0x%.2X", val);
    
    ASM_setFlags(&prev, &res, R_Bit8, false);
    f.f.cf = 0; f.f.of = 0;
    ASM_rexPrint();
    ASM_end();
}

// 0x0E invalid

#include "subops/x86_0F.c"
void ASM_0F(u8 index, Data *data) {
    if (ASM_0FFuncs[index] == 0) {
        printf("UNIMPLEMENTED OPCODE: 0F %X", index);
        exit(EXIT_FAILURE);
    }

    ASM_0FFuncs[index](data);

    ASM_end();
}

// 0x16-17 invalid

// 0x1E-1F invalid

// AND r(16-64), r/m(16-64)
void ASM_23(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; regs[rm.oreg].x &= *temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; regs[rm.oreg].e &= *temp; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; regs[rm.oreg].r &= *temp; break; }
            default: break;
        }

        ASM_rmPrint("AND", &rm, disp, v_Reg, true);
    } else {
        switch (rm.otype) {
            case R_Bit16: prev.x = regs[rm.areg].x; regs[rm.oreg].x &= regs[rm.areg].x; break;
            case R_Bit32: prev.e = regs[rm.areg].e; regs[rm.oreg].e &= regs[rm.areg].e; regs[rm.oreg].eh = 0; break;
            case R_Bit64: prev.r = regs[rm.areg].r; regs[rm.oreg].r &= regs[rm.areg].r; break;
            default: break;
        }
        printf("AND %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    switch (rm.otype) {
        case R_Bit16: res.x = prev.x & regs[rm.oreg].x; prev.x = regs[rm.oreg].x; break;
        case R_Bit32: res.e = prev.e & regs[rm.oreg].e; prev.e = regs[rm.oreg].e; break;
        case R_Bit64: res.r = prev.r & regs[rm.oreg].r; prev.r = regs[rm.oreg].r; break;
        default: break;
    }

    u8 fa = f.f.af;
    ASM_setFlags(&prev, &res, rm.otype, false);
    f.f.af = fa;
    f.f.cf = 0; f.f.of = 0;

    ASM_rexPrint();
    ASM_end();
}

// 0x27 invalid

// SUB r(16-64), r/m(16-64)
void ASM_2B(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    Reg prev = { 0 };
    Reg res = { 0 };

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; regs[rm.oreg].x -= *temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; regs[rm.oreg].e -= *temp; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; regs[rm.oreg].r -= *temp; break; }
            default: break;
        }

        ASM_rmPrint("SUB", &rm, disp, v_Reg, true);
    } else {
        switch (rm.otype) {
            case R_Bit16: prev.x = regs[rm.areg].x; regs[rm.oreg].x -= regs[rm.areg].x; break;
            case R_Bit32: prev.x = regs[rm.areg].e; regs[rm.oreg].e -= regs[rm.areg].e; regs[rm.oreg].eh = 0; break;
            case R_Bit64: prev.x = regs[rm.areg].r; regs[rm.oreg].r -= regs[rm.areg].r; break;
            default: break;
        }
        printf("SUB %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    switch (rm.otype) {
        case R_Bit16: res.x = prev.x - regs[rm.oreg].x; break;
        case R_Bit32: res.e = prev.e - regs[rm.oreg].e; break;
        case R_Bit64: res.r = prev.r - regs[rm.oreg].r; break;
        default: break;
    }

    ASM_setFlags(&prev, &res, rm.otype, true);
    ASM_rexPrint();
    ASM_end();
}

// 0x2F invalid

// XOR r/m(16-64), r(16-64)
void ASM_31(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; *temp ^= regs[rm.oreg].x; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; *temp ^= regs[rm.oreg].e; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; *temp ^= regs[rm.oreg].r; break; }
            default: break;
        }

        ASM_rmPrint("XOR", &rm, disp, v_Reg, false);
    } else {
        switch (rm.otype) {
            case R_Bit16: prev.x = regs[rm.areg].x; regs[rm.areg].x ^= regs[rm.oreg].x; break;
            case R_Bit32: prev.e = regs[rm.areg].e; regs[rm.areg].e ^= regs[rm.oreg].e; regs[rm.areg].eh = 0; break;
            case R_Bit64: prev.r = regs[rm.areg].r; regs[rm.areg].r ^= regs[rm.oreg].r; break;
            default: break;
        }
        printf("XOR %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
    }

    switch (rm.otype) {
        case R_Bit16: res.x = prev.x ^ regs[rm.oreg].x; break;
        case R_Bit32: res.e = prev.e ^ regs[rm.oreg].e; break;
        case R_Bit64: res.r = prev.r ^ regs[rm.oreg].r; break;
        default: break;
    }

    u8 fa = f.f.af;
    u8 fc = f.f.cf;
    ASM_setFlags(&prev, &res, rm.otype, false);
    f.f.af = fa;
    f.f.cf = fc;
    f.f.of = 0;

    ASM_rexPrint();
    ASM_end();
}

// XOR r(8), r/m(8)
void ASM_32(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit8);
    ASM_incIP(2, &rm);
    
    Reg prev = { 0 };
    Reg res = { 0 };

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        STACK8(temp, fdisp);
        if (rm.otype == R_Bit8H) {
            prev.l = regs[rm.oreg].h;
            res.l = *temp;
            regs[rm.oreg].h ^= *temp;
        } else {
            prev.l = regs[rm.oreg].l;
            res.l = *temp;
            regs[rm.oreg].l ^= *temp;
        }

        ASM_rmPrint("XOR", &rm, disp, v_Reg, true);
    } else {
        if (rm.otype == R_Bit8H) {
            prev.l = regs[rm.oreg].h;
            regs[rm.oreg].h ^= (rm.atype == R_Bit8H) ? regs[rm.areg].h : regs[rm.areg].l;
        } else {
            prev.l = regs[rm.oreg].l;
            regs[rm.oreg].l ^= (rm.atype == R_Bit8H) ? regs[rm.areg].h : regs[rm.areg].l;
        }
        
        printf("XOR %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    res.l ^= prev.l;
    
    u8 fa = f.f.af;
    u8 fc = f.f.cf;
    ASM_setFlags(&prev, &res, rm.otype, false);
    f.f.af = fa;
    f.f.cf = fc;
    f.f.of = 0;

    ASM_rexPrint();
    ASM_end();
}

// XOR r(16-64), r/m(16-64)
void ASM_33(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; regs[rm.oreg].x ^= *temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; regs[rm.oreg].e ^= *temp; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; regs[rm.oreg].r ^= *temp; break; }
            default: break;
        }

        ASM_rmPrint("XOR", &rm, disp, v_Reg, true);
    } else {
        switch (rm.otype) {
            case R_Bit16: prev.x = regs[rm.areg].x; regs[rm.oreg].x ^= regs[rm.areg].x; break;
            case R_Bit32: prev.e = regs[rm.areg].e; regs[rm.oreg].e ^= regs[rm.areg].e; regs[rm.oreg].eh = 0; break;
            case R_Bit64: prev.r = regs[rm.areg].r; regs[rm.oreg].r ^= regs[rm.areg].r; break;
            default: break;
        }
        printf("XOR %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    switch (rm.otype) {
        case R_Bit16: res.x = prev.x ^ regs[rm.oreg].x; prev.x = regs[rm.oreg].x; break;
        case R_Bit32: res.e = prev.e ^ regs[rm.oreg].e; prev.e = regs[rm.oreg].e; break;
        case R_Bit64: res.r = prev.r ^ regs[rm.oreg].r; prev.r = regs[rm.oreg].r; break;
        default: break;
    }

    u8 fa = f.f.af;
    u8 fc = f.f.cf;
    ASM_setFlags(&prev, &res, rm.otype, false);
    f.f.af = fa;
    f.f.cf = fc;
    f.f.of = 0;

    ASM_rexPrint();
    ASM_end();
}

// 0x37 invalid

// CMP r/m(16-64), r(16-64)
void ASM_39(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; break; }
            default: break;
        }

        ASM_rmPrint("CMP", &rm, disp, v_Reg, false);
    } else {
        switch (rm.otype) {
            case R_Bit16: prev.x = regs[rm.areg].x; break;
            case R_Bit32: prev.e = regs[rm.areg].e; break;
            case R_Bit64: prev.r = regs[rm.areg].r; break;
            default: break;
        }
        printf("CMP %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
    }

    switch (rm.otype) {
        case R_Bit16: res.x = prev.x - regs[rm.oreg].x; break;
        case R_Bit32: res.e = prev.e - regs[rm.oreg].e; break;
        case R_Bit64: res.r = prev.r - regs[rm.oreg].r; break;
        default: break;
    }

    ASM_setFlags(&prev, &res, rm.otype, true);
    ASM_rexPrint();
    ASM_end();
}

// CMP r(16-64), r/m(16-64)
void ASM_3B(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    Reg prev = { 0 };
    Reg res = { 0 };
    
    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; break; }
            default: break;
        }

        ASM_rmPrint("CMP", &rm, disp, v_Reg, true);
    } else {
        switch (rm.otype) {
            case R_Bit16: prev.x = regs[rm.areg].x; break;
            case R_Bit32: prev.e = regs[rm.areg].e; break;
            case R_Bit64: prev.r = regs[rm.areg].r; break;
            default: break;
        }
        printf("CMP %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    switch (rm.otype) {
        case R_Bit16: res.x = regs[rm.oreg].x - prev.x; prev.x = regs[rm.oreg].x; break;
        case R_Bit32: res.e = regs[rm.oreg].e - prev.e; prev.e = regs[rm.oreg].e; break;
        case R_Bit64: res.r = regs[rm.oreg].r - prev.r; prev.r = regs[rm.oreg].r; break;
        default: break;
    }

    ASM_setFlags(&prev, &res, rm.otype, true);
    ASM_rexPrint();
    ASM_end();
}

void ASM_3D(s32 val) {
    RegType type = IS_OP(IS_W(R_Bit32, R_Bit64), R_Bit16);
    Reg res = {0};

    switch (type) {
        case R_Bit16:
            ASM_incIP(4, NULL);
            val = (s16)(u32)val;
            res.x = regs[0].x - val;
            break;
        case R_Bit32:
            ASM_incIP(5, NULL);
            res.e = regs[0].e - val;
            break;
        case R_Bit64:
            ASM_incIP(6, NULL);
            res.r = regs[0].r - val;
            break;

        default: break;

    }

    if (type != R_Bit16) printf("CMP %s, 0x%.8X", ASM_getRegName(0, type), val);
    else                 printf("CMP %s, 0x%.4X", ASM_getRegName(0, type), val);

    ASM_setFlags(&regs[0], &res, type, true);
    ASM_rexPrint();
    ASM_end();
}

// 0x3F invalid

// REX Prefix
void ASM_4X(u8 in_rex) {
    rex = *(REXPrefix *)&in_rex;
}

// PUSH r(16/64)
void ASM_5L(u8 in) {
    ASM_incIP(IS_REX(1, 2), NULL);

    u8 reg = in % 8;

    regs[4].r -= IS_OP(8, 2);
    STACK64(temp, regs[4].e);

    *temp = IS_B(IS_OP(regs[reg].r, regs[reg].x), IS_OP(regs[reg + 8].r, regs[reg + 8].x));

    printf("PUSH %s", ASM_getRegName(IS_B(reg, reg + 8), IS_OP(R_Bit64, R_Bit16)));
    ASM_rexPrint();
    ASM_end();
}

// POP r(16/64)
void ASM_5H(u8 in) {
    ASM_incIP(1, NULL);

    u8 reg = in % 8;
    reg = IS_B(reg, reg + 8);

    STACK64(temp, regs[4].e);
    regs[4].r += IS_OP(8, 2);

    if (!oper) {
        regs[reg].r = *temp;
    } else {
        regs[reg].x = *temp;
    }

    printf("POP %s", ASM_getRegName(reg, IS_OP(R_Bit64, R_Bit16)));
    ASM_rexPrint();
    ASM_end();
}

// 0x60-62 invalid

// MOVSXD r(32/64), r/m(32)
void ASM_63(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16:
            case R_Bit32: { STACK32(temp, fdisp); regs[rm.oreg].e = *temp; regs[rm.oreg].eh = 0; break; }
            case R_Bit64: { STACK32(temp, fdisp); regs[rm.oreg].r = *temp; break; }
            default: break;
        }

        rm.ptrtype = R_Bit32;
        ASM_rmPrint("MOVSXD", &rm, disp, v_Reg, true);
    } else {
        switch (rm.otype) {
            case R_Bit16:
            case R_Bit32: regs[rm.oreg].e = regs[rm.areg].e; regs[rm.oreg].eh = 0; break;
            case R_Bit64: regs[rm.oreg].r = regs[rm.areg].e; break;
            default: break;
        }
        
        rm.atype = R_Bit32;
        printf("MOVSXD %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// FS Prefix
void ASM_64(void) {
    fs = true;
}

// GS Prefix
void ASM_65(void) {
    gs = true;
}

// Operation Prefix
void ASM_66(void) {
    oper = true;
}

// Address Prefix
void ASM_67(void) {
    addr = true;
}

// JB rel(8)
bool _ASM_72(s8 val) {
    ASM_incIP(2, NULL);
    
    printf("JB 0x%.2X", (u8)val);
    if (!f.f.cf) { // if not carry, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    regs[16].e += val;
    printf(" -> PASSED");
    
    ASM_rexPrint();
    ASM_end();
    return true;
}

// JNB rel(8)
bool _ASM_73(s8 val) {
    ASM_incIP(2, NULL);
    
    printf("JNB 0x%.2X", (u8)val);
    if (f.f.cf) { // if carry, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    regs[16].e += val;
    printf(" -> PASSED");
    
    ASM_rexPrint();
    ASM_end();
    return true;
}

// JZ rel(8)
bool _ASM_74(s8 val) {
    ASM_incIP(2, NULL);
    
    printf("JZ 0x%.2X", (u8)val);
    if (!f.f.zf) { // if it is not zero, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    regs[16].e += val;
    printf(" -> PASSED");
    
    ASM_rexPrint();
    ASM_end();
    return true;
}

// JNZ rel(8)
bool _ASM_75(s8 val) {
    ASM_incIP(2, NULL);
    
    printf("JNZ 0x%.2X", (u8)val);
    if (val > 0x7F) {
        printf(" (loop)");
    }
    if (f.f.zf) { // if it is zero, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    regs[16].e += val;
    printf(" -> PASSED");
    
    ASM_rexPrint();
    ASM_end();
    return true;
}

// JBE rel(8)
bool _ASM_76(s8 val) {
    ASM_incIP(2, NULL);
    
    printf("JBE 0x%.2X",(u8) val);
    if (val > 0x7F) {
        printf(" (loop)");
    }
    if (!f.f.zf && !f.f.cf) { // if it is not zero AND no carry, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    regs[16].e += val;
    printf(" -> PASSED");
    
    ASM_rexPrint();
    ASM_end();
    return true;
}

// JA rel(8)
bool _ASM_77(s8 val) {
    ASM_incIP(2, NULL);
    
    printf("JA 0x%.2X",(u8) val);
    if (val > 0x7F) {
        printf(" (loop)");
    }
    if (f.f.zf || f.f.cf) { // if it is zero OR carry, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    regs[16].e += val;
    printf(" -> PASSED");
    
    ASM_rexPrint();
    ASM_end();
    return true;
}

// JNS rel(8)
bool _ASM_79(s8 val) {
    ASM_incIP(2, NULL);
    
    printf("JS 0x%.2X",(u8) val);
    if (val > 0x7F) {
        printf(" (loop)");
    }
    if (f.f.sf) { // if not sign, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    regs[16].e += val;
    printf(" -> PASSED");
    
    ASM_rexPrint();
    ASM_end();
    return true;
}

// JL rel(8)
bool _ASM_7C(s8 val) {
    ASM_incIP(2, NULL);
    
    printf("JL 0x%.2X",(u8) val);
    if (val > 0x7F) {
        printf(" (loop)");
    }
    if (f.f.sf == f.f.of) { // if sign equals overflow, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    regs[16].e += val;
    printf(" -> PASSED");
    
    ASM_rexPrint();
    ASM_end();
    return true;
}

#include "subops/x86_80.c"
void ASM_80(u8 rm, u8 sib, s32 disp, u8 val) {
    RM ret = ASM_getRM(rm, sib, R_Bit8);
    ASM_incIP(3, &ret);

    if (ASM_80Funcs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: 80 /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_80Funcs[ret.reg](&ret, disp, val);

    ASM_end();
}

#include "subops/x86_81.c"
void ASM_81(u8 rm, u8 sib, s32 disp, u32 val) {
    RM ret = ASM_getRM(rm, sib, R_Bit32);
    ASM_incIP(IS_OP(6, 4), &ret);

    if (ASM_81Funcs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: 81 /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_81Funcs[ret.reg](&ret, disp, val);

    ASM_end();
}

// 0x82 invalid

#include "subops/x86_83.c"
void ASM_83(u8 rm, u8 sib, s32 disp, u8 val) {
    RM ret = ASM_getRM(rm, sib, R_Bit32);
    ASM_incIP(3, &ret);

    if (ASM_83Funcs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: 83 /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_83Funcs[ret.reg](&ret, disp, val);

    ASM_end();
}

// TEST r/m(16-64), r(16-64)
void ASM_85(u8 rm_code, u8 sib, s32 disp) {
    Reg prev = { 0 };
    Reg res = { 0 };

    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);
    
    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); prev.x = *temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); prev.e = *temp; break; }
            case R_Bit64: { STACK64(temp, fdisp); prev.r = *temp; break; }
            default: break;
        }

        ASM_rmPrint("TEST", &rm, disp, v_Reg, false);
    } else {
        switch (rm.otype) {
            case R_Bit16: prev.x = regs[rm.areg].x; break;
            case R_Bit32: prev.e = regs[rm.areg].e; break;
            case R_Bit64: prev.r = regs[rm.areg].r; break;
            default: break;
        }
        printf("TEST %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
    }

    switch (rm.otype) {
        case R_Bit16: res.x = prev.x & regs[rm.oreg].x; break;
        case R_Bit32: res.e = prev.e & regs[rm.oreg].e; break;
        case R_Bit64: res.r = prev.r & regs[rm.oreg].r; break;
        default: break;
    }

    u8 fa = f.f.af;
    ASM_setFlags(&prev, &res, rm.otype, false);
    f.f.af = fa;

    ASM_rexPrint();
    ASM_end();
}

// MOV r/m(8), r(8)
void ASM_88(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit8);
    ASM_incIP(2, &rm);

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        STACK8(temp, fdisp);
        *temp = (rm.otype == R_Bit8H) ? regs[rm.oreg].h : regs[rm.oreg].l;

        ASM_rmPrint("MOV", &rm, disp, v_Reg, false);
    } else {
        if (rm.atype == R_Bit8H) {
            regs[rm.areg].h = (rm.otype == R_Bit8H) ? regs[rm.oreg].h : regs[rm.oreg].l;
        } else {
            regs[rm.areg].l = (rm.otype == R_Bit8H) ? regs[rm.oreg].h : regs[rm.oreg].l;
        }
        
        printf("MOV %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// MOV r/m(16-64), r(16-64)
void ASM_89(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); *temp = regs[rm.oreg].x; break; }
            case R_Bit32: { STACK32(temp, fdisp); *temp = regs[rm.oreg].e; break; }
            case R_Bit64: { STACK64(temp, fdisp); *temp = regs[rm.oreg].r; break; }
            default: break;
        }

        ASM_rmPrint("MOV", &rm, disp, v_Reg, false);
    } else {
        switch (rm.otype) {
            case R_Bit16: regs[rm.areg].x = regs[rm.oreg].x; break;
            case R_Bit32: regs[rm.areg].e = regs[rm.oreg].e; regs[rm.areg].eh = 0; break;
            case R_Bit64: regs[rm.areg].r = regs[rm.oreg].r; break;
            default: break;
        }
        printf("MOV %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// MOV r(8), r/m(8)
void ASM_8A(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit8);
    ASM_incIP(2, &rm);

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        STACK8(temp, fdisp);
        if (rm.otype == R_Bit8H) {
            regs[rm.oreg].h = *temp;
        } else {
            regs[rm.oreg].l = *temp;
        }

        ASM_rmPrint("MOV", &rm, disp, v_Reg, true);
    } else {
        if (rm.otype == R_Bit8H) {
            regs[rm.oreg].h = (rm.atype == R_Bit8H) ? regs[rm.areg].h : regs[rm.areg].l;
        } else {
            regs[rm.oreg].l = (rm.atype == R_Bit8H) ? regs[rm.areg].h : regs[rm.areg].l;
        }
        
        printf("MOV %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// MOV r(16-64), r/m(16-64)
void ASM_8B(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); regs[rm.oreg].x = *temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); regs[rm.oreg].e = *temp; regs[rm.oreg].eh = 0; break; }
            case R_Bit64: { STACK64(temp, fdisp); regs[rm.oreg].r = *temp; break; }
            default: break;
        }

        ASM_rmPrint("MOV", &rm, disp, v_Reg, true);
    } else {
        switch (rm.otype) {
            case R_Bit16: regs[rm.oreg].x = regs[rm.areg].x; break;
            case R_Bit32: regs[rm.oreg].e = regs[rm.areg].e; regs[rm.oreg].eh = 0; break;
            case R_Bit64: regs[rm.oreg].r = regs[rm.areg].r; break;
            default: break;
        }
        printf("MOV %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// LEA r(16-64), r/m(16-64)
void ASM_8D(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { regs[rm.oreg].x = fdisp; break; }
            case R_Bit32: { regs[rm.oreg].e = fdisp; regs[rm.oreg].eh = 0; break; }
            case R_Bit64: { regs[rm.oreg].r = fdisp; break; }
            default: break;
        }

        ASM_rmPrint("LEA", &rm, disp, v_Reg, true);
    } else {
        printf("ERROR: LEA of register");
        exit(EXIT_FAILURE);
    }

    ASM_rexPrint();
    ASM_end();
}

// NOP
void ASM_90(void) {
    ASM_incIP(1, NULL);

    printf("NOP");
    ASM_rexPrint();
    ASM_end();
}

// MOV r(8), imm(8)
void ASM_BL(u8 in, u8 val) {
    u8 r = (in % 8);
    r = IS_B(r, r + 8);
    
    if (r >= 4 && r < 8) regs[r].h = val;
    else                 regs[r].l = val;
    
    ASM_incIP(IS_REX(2, 3), NULL);
    printf("MOV %s, 0x%.2X", ASM_getRegName(r, (r >= 4 && r < 8) ? R_Bit8H : R_Bit8), (u8)val);

    ASM_rexPrint();
    ASM_end();
}

// 0x9A invalid

// MOV r(16-64), imm(16-64)
void ASM_BH(u8 in, u64 val) {
    u8 r = (in % 8);
    r = IS_B(r, r + 8);
    if (oper) {
        regs[r].x = val;
        ASM_incIP(IS_REX(4, 5), NULL);
        printf("MOV %s, 0x%.4X", ASM_getRegName(r, R_Bit16), regs[r].x);
        ASM_rexPrint();
    } else if (rex.w) {
        regs[r].r = val;
        ASM_incIP(10, NULL);
        printf("MOV %s, 0x%.16llX", ASM_getRegName(r, R_Bit64), regs[r].r);
        ASM_rexPrint();
    } else {
        regs[r].e = val;
        ASM_incIP(IS_REX(5, 6), NULL);
        printf("MOV %s, 0x%.8X", ASM_getRegName(r, R_Bit32), regs[r].e);
        ASM_rexPrint();
    }
    ASM_end();
}

#include "subops/x86_C1.c"
void ASM_C1(u8 rm, u8 sib, s32 disp, u8 val) {
    RM ret = ASM_getRM(rm, sib, R_Bit32);
    ASM_incIP(3, &ret);

    if (ASM_C1Funcs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: C1 /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_C1Funcs[ret.reg](&ret, disp, val);

    ASM_end();
}

// RETN
void _ASM_C3(void) {
    STACK64(temp, regs[4].e);
    regs[4].r += 8;
    regs[16].e = *temp;
    printf("RETN");
    ASM_rexPrint();
    printf("---\n\n(%.8X)\n", regs[16].e);
    ASM_end();
}

// 0xC4-C5 invalid

// MOV r/m(8), imm(8)
void ASM_C6(u8 rm_code, u8 sib, s32 disp, u8 val) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit8);
    ASM_incIP(3, &rm);

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        STACK8(temp, fdisp);
        *temp = val;

        rm.val = val;
        rm.valtype = R_Bit8;
        ASM_rmPrint("MOV", &rm, disp, v_Val, false);
    } else {
        if (rm.atype == R_Bit8H) {
            regs[rm.areg].h = val;
        } else {
            regs[rm.areg].l = val;
        }
        
        printf("MOV %s, %.2X", ASM_getRegName(rm.areg, rm.atype), val);
    }

    ASM_rexPrint();
    ASM_end();
}

// MOV r/m(16-64), imm(16/32)
void ASM_C7(u8 rm_code, u8 sib, s32 disp, u32 val) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP((rm.otype == R_Bit16) ? 4 : 6, &rm);

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); *temp = (u16)val; break; }
            case R_Bit32: { STACK32(temp, fdisp); *temp = (u32)val; break; }
            case R_Bit64: { STACK64(temp, fdisp); *temp = (u32)val; break; }
            default: break;
        }

        rm.val = val;
        rm.valtype = (rm.otype == R_Bit16) ? R_Bit16 : R_Bit32;
        ASM_rmPrint("MOV", &rm, disp, v_Val, false);
    } else {
        switch (rm.otype) {
            case R_Bit16: regs[rm.areg].x = (u16)val; break;
            case R_Bit32: regs[rm.areg].e = (u32)val; regs[rm.areg].eh = 0; break;
            case R_Bit64: regs[rm.areg].r = (u32)val; break;
            default: break;
        }
        printf("MOV %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
    }

    ASM_rexPrint();
    ASM_end();
}

#include "subops/x86_D1.c"
void ASM_D1(u8 rm, u8 sib, s32 disp) {
    RM ret = ASM_getRM(rm, sib, R_Bit32);
    ASM_incIP(2, &ret);

    if (ASM_D1Funcs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: D1 /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_D1Funcs[ret.reg](&ret, disp);

    ASM_end();
}

// 0xD4-D6 invalid

// CALL rel(16/32)
void ASM_E8(u32 val) {
    ASM_codeFunc func;
    Reg conv;

    ASM_incIP(IS_OP(5, 3), NULL);
    regs[4].e -= 8;
    STACK64(temp, regs[4].e);
    *temp = regs[16].e;

    conv.e = val;

    if (!oper) {
        regs[16].e = (s32)regs[16].e + (s32)conv.e;
        printf("CALL rip + 0x%.4X", conv.e);
    } else {
        regs[16].x = (s32)regs[16].x + (s16)conv.x;
        printf("CALL rip + 0x%.2X", conv.x);
    }
    ASM_rexPrint();

    func = ASM_getFunc(regs[16].e);
    func();
}

// JMP rel(16/32)
bool ASM_E9(u32 val, bool call) {
    ASM_codeFunc func;
    Reg conv;

    ASM_incIP(IS_OP(5, 3), NULL);

    conv.e = val;
    if (!oper) {
        regs[16].e = (s32)regs[16].e + (s32)conv.e;
        printf("JMP 0x%.4X", conv.e);
    } else {
        regs[16].x = (s32)regs[16].x + (s16)conv.x;
        printf("JMP 0x%.2X", conv.x);
    }
    ASM_rexPrint();

    if (call) {
        func = ASM_getFunc(regs[16].e);
        func();
    }
}

// 0xEA invalid

// JMP rel(8)
void _ASM_EB(uint8_t val) {
    ASM_codeFunc func;
    Reg conv;

    ASM_incIP(2, NULL);

    conv.e = val;
    if (!oper) {
        s32 val = (s32)regs[16].e + (s8)conv.l;
        regs[16].e = val;
        printf("JMP 0x%.4X", conv.e);
    } else {
        s16 val = (s16)regs[16].x + (s8)conv.l;
        regs[16].x = val;
        printf("JMP 0x%.2X", conv.x);
    }
    ASM_rexPrint();
}
#define ASM_EB(val, t) _ASM_EB(val); goto JMP_##t

// LOCK, maybe implement in the far future
void ASM_F0(void) {
    lock = true;
}

void ASM_F2(void) {
    doub = true;
}

void ASM_F3(void) {
    sing = true;
}

#include "subops/x86_F6.c"
void ASM_F6(u8 rm, u8 sib, s32 disp, u8 val) {
    RM ret = ASM_getRM(rm, sib, R_Bit8);
    ASM_incIP(3, &ret);

    if (ASM_F6Funcs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: F6 /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_F6Funcs[ret.reg](&ret, disp, val);

    ASM_end();
}

#include "subops/x86_F7.c"
void ASM_F7(u8 rm, u8 sib, s32 disp, u8 val) {
    RM ret = ASM_getRM(rm, sib, R_Bit32);
    ASM_incIP(3, &ret);

    if (ASM_F7Funcs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: F7 /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_F7Funcs[ret.reg](&ret, disp, val);

    ASM_end();
}

#include "subops/x86_FF.c"
void ASM_FF(u8 rm, u8 sib, s32 disp) {
    RM ret = ASM_getRM(rm, sib, R_Bit32);

    if (ASM_FFFuncs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: FF /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_FFFuncs[ret.reg](&ret, disp);

    ASM_end();
}