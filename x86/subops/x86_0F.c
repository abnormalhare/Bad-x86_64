#include "../data.h"

#include "x86_0F_01.c"
void ASM_0F_01(Data *data) {
    u32 fdisp = ASM_setupOp(&data->rm, data->rm_code, data->sib, data->sdisp, 3, R_Bit32);

    if (ASM_0F_01Funcs[data->rm.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: 0F 01 /%X", data->rm.reg + 4);
        exit(EXIT_FAILURE);
    }
    ASM_0F_01Funcs[data->rm.reg](data);

    ASM_end();
}

//    MOVUPS xmm, r/m(xmm128)
// F3 MOVSS  xmm, r/m(xmm32)
// 66 MOVUPD xmm, r/m(xmm128)
// F2 MOVSD  xmm, r/m(xmm64)
void ASM_0F_10(Data *data) {
    u32 fdisp = ASM_setupOp(&data->rm, data->rm_code, data->sib, data->sdisp, 3, R_Float128);

    data->rm.otype = R_Float128;
    if (!data->rm.isPtr) data->rm.atype = R_Float128;
    
    if (data->rm.disp == 1) data->disp = (s8)data->disp;

    if (doub) { // MOVSD
        if (data->rm.isPtr) {
            ASM_rmPrint("MOVSD", &data->rm, data->disp, v_Reg, true);
            STACK(f64, s, fdisp);
            xregs[data->rm.oreg].d[0] = *s;
        } else {
            xregs[data->rm.oreg].d[0] = xregs[data->rm.areg].d[0];
            printf("MOVSD %s, %s", ASM_getRegName(data->rm.oreg, data->rm.otype), ASM_getRegName(data->rm.areg, data->rm.atype));
        }
    } else if (oper) { // MOVUPD
        if (data->rm.isPtr) {
            ASM_rmPrint("MOVUPD", &data->rm, data->disp, v_Reg, true);

            STACK(f64, s,  fdisp);
            STACK(f64, s2, fdisp);
            xregs[data->rm.oreg].d[0] = *s;
            xregs[data->rm.oreg].d[1] = *s2;
        } else {
            xregs[data->rm.oreg].d[0] = xregs[data->rm.areg].d[0];
            xregs[data->rm.oreg].d[1] = xregs[data->rm.areg].d[1];
            printf("MOVUPD %s, %s", ASM_getRegName(data->rm.oreg, data->rm.otype), ASM_getRegName(data->rm.areg, data->rm.atype));
        }
    } else if (sing) { // MOVSS
        if (data->rm.isPtr) {
            u32 fdisp = ASM_getDisp(&data->rm, data->disp);
            ASM_rmPrint("MOVSS", &data->rm, data->disp, v_Reg, true);

            STACK(f32, s, fdisp);
            xregs[data->rm.oreg].f[0] = *s;
        } else {
            xregs[data->rm.oreg].f[0] = xregs[data->rm.areg].f[0];
            printf("MOVSS %s, %s", ASM_getRegName(data->rm.oreg, data->rm.otype), ASM_getRegName(data->rm.areg, data->rm.atype));
        }
    } else { // MOVUPS
        if (data->rm.isPtr) {
            u32 fdisp = ASM_getDisp(&data->rm, data->disp);

            ASM_rmPrint("MOVUPS", &data->rm, data->disp, v_Reg, true);

            STACK(f32, s1, fdisp + 0x0);
            STACK(f32, s2, fdisp + 0x4);
            STACK(f32, s3, fdisp + 0x8);
            STACK(f32, s4, fdisp + 0xC);
            xregs[data->rm.oreg].f[0] = *s1;
            xregs[data->rm.oreg].f[1] = *s2;
            xregs[data->rm.oreg].f[2] = *s3;
            xregs[data->rm.oreg].f[3] = *s4;
        } else {
            xregs[data->rm.oreg].xi = xregs[data->rm.areg].xi;
            printf("MOVUPS %s, %s", ASM_getRegName(data->rm.oreg, data->rm.otype), ASM_getRegName(data->rm.areg, data->rm.atype));
        }
    }

    ASM_rexPrint();
    ASM_end();
}

//    MOVUPS r/m(xmm128), xmm
// F3 MOVSS  r/m(xmm32), xmm
// 66 MOVUPD r/m(xmm128), xmm
// F2 MOVSD  r/m(xmm64), xmm
void ASM_0F_11(Data *data) {
    u32 fdisp = ASM_setupOp(&data->rm, data->rm_code, data->sib, data->sdisp, 3, R_Float128);

    data->rm.otype = R_Float128;
    if (!data->rm.isPtr) data->rm.atype = R_Float128;

    if (doub) { // MOVSD
        if (data->rm.isPtr) {
            u32 fdisp = ASM_getDisp(&data->rm, data->disp);
            ASM_rmPrint("MOVSD", &data->rm, data->disp, v_Reg, false);

            STACK(f64, s, fdisp);
            *s = xregs[data->rm.oreg].d[0];
        } else {
            xregs[data->rm.areg].d[0] = xregs[data->rm.oreg].d[0];
            printf("MOVSD %s, %s", ASM_getRegName(data->rm.areg, data->rm.atype), ASM_getRegName(data->rm.oreg, data->rm.otype));
        }
    } else if (oper) { // MOVUPD
        if (data->rm.isPtr) {
            u32 fdisp = ASM_getDisp(&data->rm, data->disp);

            ASM_rmPrint("MOVUPD", &data->rm, data->disp, v_Reg, false);

            STACK(f64, s,  fdisp);
            STACK(f64, s2, fdisp);
            *s  = xregs[data->rm.oreg].d[0];
            *s2 = xregs[data->rm.oreg].d[1];
        } else {
            xregs[data->rm.areg].xi = xregs[data->rm.oreg].xi;
            printf("MOVUPD %s, %s", ASM_getRegName(data->rm.areg, data->rm.atype), ASM_getRegName(data->rm.oreg, data->rm.otype));
        }
    } else if (sing) { // MOVSS
        if (data->rm.isPtr) {
            u32 fdisp = ASM_getDisp(&data->rm, data->disp);
            ASM_rmPrint("MOVSS", &data->rm, data->disp, v_Reg, false);

            STACK(f32, s, fdisp);
            *s = xregs[data->rm.oreg].f[0];
        } else {
            xregs[data->rm.areg].f[0] = xregs[data->rm.oreg].f[0];
            printf("MOVSS %s, %s", ASM_getRegName(data->rm.areg, data->rm.atype), ASM_getRegName(data->rm.oreg, data->rm.otype));
        }
    } else { // MOVUPS
        if (data->rm.isPtr) {
            u32 fdisp = ASM_getDisp(&data->rm, data->disp);

            ASM_rmPrint("MOVUPS", &data->rm, data->disp, v_Reg, false);

            STACK(f32, s1, fdisp);
            STACK(f32, s2, fdisp);
            STACK(f32, s3, fdisp);
            STACK(f32, s4, fdisp);
            *s1 = xregs[data->rm.oreg].f[0];
            *s2 = xregs[data->rm.oreg].f[1];
            *s3 = xregs[data->rm.oreg].f[2];
            *s4 = xregs[data->rm.oreg].f[3];
        } else {
            xregs[data->rm.areg].xi = xregs[data->rm.oreg].xi;
            printf("MOVUPS %s, %s", ASM_getRegName(data->rm.areg, data->rm.atype), ASM_getRegName(data->rm.oreg, data->rm.otype));
        }
    }

    ASM_rexPrint();
    ASM_end();
}

// MOVLHPS xmm, xmm
// MOVHPS xmm, m(64)
// MOVHPD xmm m(64)
void ASM_0F_16(Data *data) {
    u32 fdisp = ASM_setupOp(&data->rm, data->rm_code, data->sib, data->sdisp, 3, R_Float128);

    if (oper) {
        if (data->rm.isPtr) {
            ASM_rmPrint("MOVHPD", &data->rm, data->disp, v_Reg, true);

            STACK(u64, s, fdisp);
            u128_set64(&xregs[data->rm.oreg].xi, *s, xregs[data->rm.oreg].ul[0]);

            data->rm.otype = R_Float128;
        } else {
            printf("GENERAL PROTECTION FAULT: %.8X", (u32)fdisp);
        }
    } else if (sing) {
        printf("ERROR: SSE2 and 3 ARE UNIMPLEMENTED");
        exit(EXIT_FAILURE);
    } else {
        if (data->rm.isPtr) { // MOVHPS
            data->rm.otype = R_Float128;
            ASM_rmPrint("MOVHPS", &data->rm, data->disp, v_Reg, true);

            STACK(u64, s, fdisp);
            u128_set64(&xregs[data->rm.oreg].xi, *s, xregs[data->rm.oreg].ul[0]);
        } else { // MOVLHPS
            u128_set64(&xregs[data->rm.oreg].xi, xregs[data->rm.areg].ul[1], xregs[data->rm.oreg].ul[0]);

            data->rm.atype = R_Float128;
            data->rm.otype = R_Float128;
            printf("MOVLHPS %s, %s", ASM_getRegName(data->rm.oreg, data->rm.otype), ASM_getRegName(data->rm.areg, data->rm.atype));
        }
    }

    ASM_rexPrint();
    ASM_end();
}

// NOP
void ASM_0F_1F(Data *data) {
    ASM_rexPrint();
    ASM_end();
}

// MOVAPS xmm, r/m(xmm)
// MOVAPD xmm, r/m(xmm)
void ASM_0F_28(Data *data) {
    u32 fdisp = ASM_setupOp(&data->rm, data->rm_code, data->sib, data->sdisp, 3, R_Float128);

    data->rm.otype = R_Float128;
    if (!data->rm.isPtr) data->rm.atype = R_Float128;

    // check if aligned
    if (((fdisp) / 4) * 4 != fdisp) {
        printf("GENERAL PROTECTION FAULT: %.8X", (u32)fdisp);
        exit(EXIT_FAILURE);
    }
    
    if (data->rm.isPtr) {
        if (!oper) {
            ASM_rmPrint("MOVAPS", &data->rm, data->disp, v_Reg, true);
            STACK(f32, s1, fdisp + 0x0);
            STACK(f32, s2, fdisp + 0x4);
            STACK(f32, s3, fdisp + 0x8);
            STACK(f32, s4, fdisp + 0xC);
            xregs[data->rm.oreg].f[0] = *s1;
            xregs[data->rm.oreg].f[1] = *s2;
            xregs[data->rm.oreg].f[2] = *s3;
            xregs[data->rm.oreg].f[3] = *s4;
        } else {
            ASM_rmPrint("MOVAPD", &data->rm, data->disp, v_Reg, true);
            STACK(f64, s1, fdisp + 0x0);
            STACK(f64, s2, fdisp + 0x8);
            xregs[data->rm.oreg].d[0] = *s1;
            xregs[data->rm.oreg].d[1] = *s2;
        }

    } else {
        xregs[data->rm.oreg].xi = xregs[data->rm.areg].xi;
        if (!oper)
            printf("MOVAPS %s, %s", ASM_getRegName(data->rm.oreg, data->rm.otype), ASM_getRegName(data->rm.areg, data->rm.atype));
        else
            printf("MOVAPD %s, %s", ASM_getRegName(data->rm.oreg, data->rm.otype), ASM_getRegName(data->rm.areg, data->rm.atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// MOVAPS r/m(xmm), xmm
// MOVAPD r/m(xmm), xmm
void ASM_0F_29(Data *data) {
    u32 fdisp = ASM_setupOp(&data->rm, data->rm_code, data->sib, data->sdisp, 3, R_Float128);

    data->rm.otype = R_Float128;
    if (!data->rm.isPtr) data->rm.atype = R_Float128;

    // check if aligned
    if (((fdisp) / 4) * 4 != fdisp) {
        printf("GENERAL PROTECTION FAULT: %.8X", (u32)fdisp);
        exit(EXIT_FAILURE);
    }
    
    if (data->rm.isPtr) {

        if (!oper) {
            ASM_rmPrint("MOVAPS", &data->rm, data->disp, v_Reg, false);
            STACK(f32, s1, fdisp + 0x0);
            STACK(f32, s2, fdisp + 0x4);
            STACK(f32, s3, fdisp + 0x8);
            STACK(f32, s4, fdisp + 0xC);
            *s1 = xregs[data->rm.oreg].f[0];
            *s2 = xregs[data->rm.oreg].f[1];
            *s3 = xregs[data->rm.oreg].f[2];
            *s4 = xregs[data->rm.oreg].f[3];
        } else {
            ASM_rmPrint("MOVAPD", &data->rm, data->disp, v_Reg, false);
            STACK(f64, s1, fdisp + 0x0);
            STACK(f64, s2, fdisp + 0x8);
            *s1 = xregs[data->rm.oreg].d[0];
            *s2 = xregs[data->rm.oreg].d[1];
        }

    } else {
        xregs[data->rm.areg].xi = xregs[data->rm.oreg].xi;
        if (!oper)
            printf("MOVAPS %s, %s", ASM_getRegName(data->rm.areg, data->rm.atype), ASM_getRegName(data->rm.oreg, data->rm.otype));
        else
            printf("MOVAPD %s, %s", ASM_getRegName(data->rm.areg, data->rm.atype), ASM_getRegName(data->rm.oreg, data->rm.otype));
    }

    ASM_rexPrint();
    ASM_end();
}

u64 _ASM_call___rdtsc(void) {
#ifdef _WIN64
    return __rdtsc();
#else
    int edx, eax;

    __asm__ volatile (
        "rdtsc\n\t"
            : "=d" (edx), "=a" (eax)
    : );

    return ((u64)edx << 32) + eax;
#endif
}

void ASM_0F_31(Data *data) {
    ASM_incIP(2, NULL);

    Reg temp;
    temp.r = _ASM_call___rdtsc();

    regs[2].e = temp.eh; regs[2].eh = 0;
    regs[0].e = temp.e;  regs[0].eh = 0;

    printf("RDTSC");
    ASM_rexPrint();
    ASM_end();
}

// CMOVNB r(16-64), r/m(16-64)
void ASM_0F_43(Data *data) {
    u32 fdisp = ASM_setupOp(&data->rm, data->rm_code, data->sib, data->sdisp, 3, R_Bit32);
    
    if (data->rm.isPtr) {
        ASM_rmPrint("CMOVNB", &data->rm, data->disp, v_Reg, true);

        if (f.f.cf) {
            switch (data->rm.otype) {
                case R_Bit16: { STACK(u16, s, fdisp); regs[data->rm.oreg].x = *s; break; }
                case R_Bit32: { STACK(u32, s, fdisp); regs[data->rm.oreg].e = *s; regs[data->rm.oreg].eh = 0; break; }
                case R_Bit64: { STACK(u64, s, fdisp); regs[data->rm.oreg].r = *s; break; }
                default: break;
            }
        } else if (data->rm.otype == R_Bit32) {
            regs[data->rm.oreg].eh = 0;
        }
    } else {
        if (f.f.cf) {
            switch (data->rm.otype) {
                case R_Bit16: regs[data->rm.oreg].x = regs[data->rm.areg].x; break;
                case R_Bit32: regs[data->rm.oreg].e = regs[data->rm.areg].e; regs[data->rm.oreg].eh = 0; break;
                case R_Bit64: regs[data->rm.oreg].r = regs[data->rm.areg].r; break;
                default: break;
            }
        } else if (data->rm.otype == R_Bit32) {
            regs[data->rm.oreg].eh = 0;
        }
        printf("CMOVNB %s, %s", ASM_getRegName(data->rm.oreg, data->rm.otype), ASM_getRegName(data->rm.areg, data->rm.atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// CMOVZ r(16-64), r/m(16-64)
void ASM_0F_44(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(3, &rm);
    
    if (rm.isPtr) {
        u32 fdisp = ASM_getDisp(&rm, data->disp);

        ASM_rmPrint("CMOVZ", &rm, data->disp, v_Reg, true);

        if (f.f.zf) {
            switch (rm.otype) {
                case R_Bit16: { STACK(u16, s, fdisp); regs[rm.oreg].x = *s; break; }
                case R_Bit32: { STACK(u32, s, fdisp); regs[rm.oreg].e = *s; regs[rm.oreg].eh = 0; break; }
                case R_Bit64: { STACK(u64, s, fdisp); regs[rm.oreg].r = *s; break; }
                default: break;
            }
        } else if (rm.otype == R_Bit32) {
            regs[rm.oreg].eh = 0;
        }
    } else {
        if (f.f.zf) {
            switch (rm.otype) {
                case R_Bit16: regs[rm.oreg].x = regs[rm.areg].x; break;
                case R_Bit32: regs[rm.oreg].e = regs[rm.areg].e; regs[rm.oreg].eh = 0; break;
                case R_Bit64: regs[rm.oreg].r = regs[rm.areg].r; break;
                default: break;
            }
        } else if (rm.otype == R_Bit32) {
            regs[rm.oreg].eh = 0;
        }
        printf("CMOVZ %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// CMOVNZ r(16-64), r/m(16-64)
void ASM_0F_45(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(2, &rm);
    
    if (rm.isPtr) {
        u32 fdisp = ASM_getDisp(&rm, data->disp);

        ASM_rmPrint("CMOVNZ", &rm, data->disp, v_Reg, true);

        if (!f.f.zf) {
            switch (rm.otype) {
                case R_Bit16: { STACK(u16, s, fdisp); regs[rm.oreg].x = *s; break; }
                case R_Bit32: { STACK(u32, s, fdisp); regs[rm.oreg].e = *s; regs[rm.oreg].eh = 0; break; }
                case R_Bit64: { STACK(u64, s, fdisp); regs[rm.oreg].r = *s; break; }
                default: break;
            }
        } else if (rm.otype == R_Bit32) {
            regs[rm.oreg].eh = 0;
        }
    } else {
        if (!f.f.zf) {
            switch (rm.otype) {
                case R_Bit16: regs[rm.oreg].x = regs[rm.areg].x; break;
                case R_Bit32: regs[rm.oreg].e = regs[rm.areg].e; regs[rm.oreg].eh = 0; break;
                case R_Bit64: regs[rm.oreg].r = regs[rm.areg].r; break;
                default: break;
            }
        } else if (rm.otype == R_Bit32) {
            regs[rm.oreg].eh = 0;
        }
        printf("CMOVNZ %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    ASM_rexPrint();
    ASM_end();
}

void ASM_0F_57(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Float128);
    ASM_incIP(3, &rm);

    rm.otype = R_Float128;
    if (!rm.isPtr) rm.atype = R_Float128;

    if (rm.isPtr) {
        u32 fdisp = ASM_getDisp(&rm, data->disp);

        if (!oper) {
            ASM_rmPrint("XORPS", &rm, data->disp, v_Reg, false);
            
            STACK(u32, s1, fdisp + 0x0);
            STACK(u32, s2, fdisp + 0x4);
            STACK(u32, s3, fdisp + 0x8);
            STACK(u32, s4, fdisp + 0xC);
            *s1 ^= xregs[rm.oreg].u[0];
            *s2 ^= xregs[rm.oreg].u[1];
            *s3 ^= xregs[rm.oreg].u[2];
            *s4 ^= xregs[rm.oreg].u[3];
        } else {
            ASM_rmPrint("XORPD", &rm, data->disp, v_Reg, false);
            
            STACK(u64, s1, fdisp + 0x0);
            STACK(u64, s2, fdisp + 0x8);
            *s1 ^= xregs[rm.oreg].ul[0];
            *s2 ^= xregs[rm.oreg].ul[1];
        }
    } else {
        if (!oper) {
            xregs[rm.areg].u[0] ^= xregs[rm.oreg].u[0];
            xregs[rm.areg].u[1] ^= xregs[rm.oreg].u[1];
            xregs[rm.areg].u[2] ^= xregs[rm.oreg].u[2];
            xregs[rm.areg].u[3] ^= xregs[rm.oreg].u[3];
            
            printf("XORPS %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        } else {
            xregs[rm.areg].ul[0] ^= xregs[rm.oreg].ul[0];
            xregs[rm.areg].ul[1] ^= xregs[rm.oreg].ul[1];

            printf("XORPD %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    }

    ASM_rexPrint();
    ASM_end();
}

// MOVD mm/xmm, r/m(32) / MOVQ mm/xmm, r/m(64)
void ASM_0F_6E(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_MMX);
    ASM_incIP(3, &rm);

    if (rm.isPtr) {
        u32 fdisp = ASM_getDisp(&rm, data->disp);

        if (rm.otype == R_MMX) {
            ASM_rmPrint("MOVD", &rm, data->disp, v_Reg, true);
        } else {
            ASM_rmPrint("MOVQ", &rm, data->disp, v_Reg, true);
        }

        switch (rm.otype) {
            case R_MMX: {
                if (rm.atype == R_Bit32) {
                    STACK(u32, s, fdisp);
                    mregs[rm.oreg].u = (u64)*s;
                } else {
                    STACK(u64, s, fdisp);
                    mregs[rm.oreg].u = *s;
                }
            }
            case R_Float128: {
                if (rm.atype == R_Bit32) {
                    STACK(u32, s, fdisp);
                    u128_set64(&xregs[rm.oreg].xi, 0, (u64)*s);
                } else {
                    STACK(u64, s, fdisp);
                    u128_set64(&xregs[rm.oreg].xi, 0, *s);
                }
            }
            default: break;
        }
    } else {
        rm.atype = IS_W(R_Bit32, R_Bit64);
        switch (rm.otype) {
            case R_MMX:
                if (rm.atype == R_Bit32) {
                    mregs[rm.oreg].u = regs[rm.areg].e;
                } else {
                    mregs[rm.oreg].u = regs[rm.areg].r;
                }
                break;
            case R_Float128:
                if (rm.atype == R_Bit32) {
                    u128_set64(&xregs[rm.oreg].xi, 0, (u64)regs[rm.areg].e);
                } else {
                    u128_set64(&xregs[rm.oreg].xi, 0, regs[rm.areg].r);
                }
                break;
            default: break;
        }
        
        if (rm.otype == R_MMX) {
            printf("MOVD %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
        } else {
            printf("MOVQ %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
        }
    }

    ASM_rexPrint();
    ASM_end();
}

// MOVD/Q r/m(32/64), mm
// MOVD/Q r/m(32/64), xmm
// MOVQ   xmm, r/m(xmm)
void ASM_0F_7E(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Float128);
    ASM_incIP(3, &rm);

    rm.otype = R_Float128;

    if (sing) { // MOVQ xmm
        if (rm.isPtr) {
            rm.atype = R_Bit32;

            u32 fdisp = ASM_getDisp(&rm, data->disp);
            ASM_rmPrint("MOVQ", &rm, data->disp, v_Reg, true);
            
            STACK(u64, s, fdisp);
            xregs[rm.oreg].ul[0] = *s;
            xregs[rm.oreg].ul[1] = 0;
        } else {
            rm.atype = R_Float128;

            xregs[rm.oreg].ul[0] = xregs[rm.areg].ul[0];
            xregs[rm.oreg].ul[1] = 0;

            printf("MOVQ %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
        }
    } else if (oper) { // MOVD/Q xmm
        rm.atype = IS_W(R_Bit32, R_Bit64);

        if (rm.isPtr) {
            u32 fdisp = ASM_getDisp(&rm, data->disp);

            if (rex.w) {
                ASM_rmPrint("MOVQ", &rm, data->disp, v_Reg, false);
                STACK(u64, s, fdisp);
                *s = xregs[rm.oreg].ul[0];
            } else {
                ASM_rmPrint("MOVD", &rm, data->disp, v_Reg, false);
                STACK(u32, s, fdisp);
                *s = xregs[rm.oreg].u[0];
            }
        } else { // MOVQ
            if (rex.w) {
                regs[rm.areg].r = xregs[rm.oreg].ul[0];
                printf("MOVQ %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
            } else {
                regs[rm.areg].e = xregs[rm.oreg].u[0];
                regs[rm.areg].eh = 0;
                printf("MOVD %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
            }
        }
    } else { // MOVD/Q mm
        rm.otype = R_MMX;
        rm.atype = IS_W(R_Bit32, R_Bit64);

        if (rm.isPtr) {
            u32 fdisp = ASM_getDisp(&rm, data->disp);

            if (rex.w) {
                ASM_rmPrint("MOVQ", &rm, data->disp, v_Reg, false);
                STACK(u64, s, fdisp);
                *s = mregs[rm.oreg].u;
            } else {
                ASM_rmPrint("MOVD", &rm, data->disp, v_Reg, false);
                STACK(u32, s, fdisp);
                *s = (u32)mregs[rm.oreg].u;
            }
        } else {
            if (rex.w) {
                regs[rm.areg].r = mregs[rm.oreg].u;
                printf("MOVQ %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
            } else {
                regs[rm.areg].e = (u32)mregs[rm.oreg].u;
                regs[rm.areg].eh = 0;
                printf("MOVD %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
            }
        }
    }

    ASM_rexPrint();
    ASM_end();
}

// MOVQ r/m(mm), mm
// MOVDQA r/m(xmm), xmm
// MOVDQU r/m(xmm), xmm
void ASM_0F_7F(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Float128);
    ASM_incIP(3, &rm);

    rm.otype = R_Float128;
    if (!rm.isPtr) rm.atype = R_Float128;

    if (sing) { // MOVDQU
        if (rm.isPtr) {
            u32 fdisp = ASM_getDisp(&rm, data->disp);
            ASM_rmPrint("MOVDQU", &rm, data->disp, v_Reg, false);

            STACK(f64, s, fdisp);
            *s = xregs[rm.oreg].d[0];
        } else {
            xregs[rm.areg].d[0] = xregs[rm.oreg].d[0];
            printf("MOVDQU %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    } else if (oper) { // MOVDQA
        u32 fdisp = ASM_getDisp(&rm, data->disp);

        if ((fdisp) / 4 * 4 != fdisp) {
            printf("GENERAL PROTECTION FAULT: %.8X", (u32)fdisp);
            exit(EXIT_FAILURE);
        }
        if (rm.isPtr) {
            ASM_rmPrint("MOVDQA", &rm, data->disp, v_Reg, false);
            STACK(f64, s, fdisp);
            *s = xregs[rm.oreg].d[0];
        } else {
            xregs[rm.areg].d[0] = xregs[rm.oreg].d[0];
            printf("MOVDQA %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    } else { // MOVQ
        rm.otype = R_MMX;

        if (rm.isPtr) {
            u32 fdisp = ASM_getDisp(&rm, data->disp);
            ASM_rmPrint("MOVQ", &rm, data->disp, v_Reg, false);

            STACK(u64, s, fdisp);
            *s = mregs[rm.oreg].u;
        } else {
            rm.atype = R_MMX;
            mregs[rm.areg].u = mregs[rm.oreg].u;
            printf("MOVQ %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    }

    ASM_rexPrint();
    ASM_end();
}

// JB long
bool _ASM_0F_82(Data *data) {
    Reg conv = { .e = data->val };
    ASM_incIP(IS_OP(6, 4), NULL);

    if (!oper)  printf("JB 0x%.4X", conv.e);
    else        printf("JB 0x%.2X", conv.x);
    
    if (!f.f.cf) { // if not carry, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    if (!oper)  regs[16].e += conv.e;
    else        regs[16].x += conv.x;
    
    printf(" -> PASSED");

    ASM_rexPrint();
    ASM_end();

    if (data->call) {
        ASM_codeFunc func = ASM_getCurrFunc();
        func();
    }
    return true;
}

void ASM_0F_82(Data *data) {
    _ASM_0F_82(data);
}

// JNB long
bool _ASM_0F_83(Data *data) {
    Reg conv = { .e = data->val };
    ASM_incIP(IS_OP(6, 4), NULL);

    if (!oper)  printf("JNB 0x%.4X", conv.e);
    else        printf("JNB 0x%.2X", conv.x);
    
    if (f.f.cf) { // if carry, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    if (!oper)  regs[16].e += conv.e;
    else        regs[16].x += conv.x;
    
    printf(" -> PASSED");

    ASM_rexPrint();
    ASM_end();

    if (data->call) {
        ASM_codeFunc func = ASM_getCurrFunc();
        func();
    }
    return true;
}

void ASM_0F_83(Data *data) {
    _ASM_0F_83(data);
}

// JZ long
bool _ASM_0F_84(Data *data) {
    Reg conv = { .e = data->val };
    ASM_incIP(IS_OP(6, 4), NULL);

    if (!oper)  printf("JZ 0x%.4X", conv.e);
    else        printf("JZ 0x%.2X", conv.x);
    
    if (!f.f.zf) { // if not zero, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    if (!oper)  regs[16].e += conv.e;
    else        regs[16].x += conv.x;
    
    printf(" -> PASSED");

    ASM_rexPrint();
    ASM_end();
    
    if (data->call) {
        ASM_codeFunc func = ASM_getCurrFunc();
        func();
    }
    return true;
}

void ASM_0F_84(Data *data) {
    _ASM_0F_84(data);
}

// JNZ long
bool _ASM_0F_85(Data *data) {
    Reg conv = { .e = data->val };
    ASM_incIP(IS_OP(6, 4), NULL);

    if (!oper)  printf("JNZ 0x%.4X", conv.e);
    else        printf("JNZ 0x%.2X", conv.x);
    
    if (f.f.zf) { // if it is zero, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    if (!oper)  regs[16].e += conv.e;
    else        regs[16].x += conv.x;
    
    printf(" -> PASSED");

    ASM_rexPrint();
    ASM_end();
    
    if (data->call) {
        ASM_codeFunc func = ASM_getCurrFunc();
        func();
    }
    return true;
}

void ASM_0F_85(Data *data) {
    _ASM_0F_85(data);
}

// JA long
bool _ASM_0F_87(Data *data) {
    Reg conv = { .e = data->val };
    ASM_incIP(IS_OP(6, 4), NULL);

    if (!oper)  printf("JA 0x%.4X", conv.e);
    else        printf("JA 0x%.2X", conv.x);
    
    if (f.f.cf || f.f.zf) { // if carry or zero, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    if (!oper)  regs[16].e += conv.e;
    else        regs[16].x += conv.x;
    
    printf(" -> PASSED");

    ASM_rexPrint();
    ASM_end();
    
    if (data->call) {
        ASM_codeFunc func = ASM_getCurrFunc();
        func();
    }
    return true;
}

void ASM_0F_87(Data *data) {
    _ASM_0F_87(data);
}

// JS long
bool _ASM_0F_88(Data *data) {
    Reg conv = { .e = data->val };
    ASM_incIP(IS_OP(6, 4), NULL);

    if (!oper)  printf("JS 0x%.4X", conv.e);
    else        printf("JS 0x%.2X", conv.x);
    
    if (!f.f.sf) { // if not sign, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    if (!oper)  regs[16].e += conv.e;
    else        regs[16].x += conv.x;
    
    printf(" -> PASSED");

    ASM_rexPrint();
    ASM_end();
    
    if (data->call) {
        ASM_codeFunc func = ASM_getCurrFunc();
        func();
    }
    return true;
}

void ASM_0F_88(Data *data) {
    _ASM_0F_88(data);
}

// JNS long
bool _ASM_0F_89(Data *data) {
    Reg conv = { .e = data->val };
    ASM_incIP(IS_OP(6, 4), NULL);

    if (!oper)  printf("JNS 0x%.4X", conv.e);
    else        printf("JNS 0x%.2X", conv.x);
    
    if (f.f.sf) { // if sign, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    if (!oper)  regs[16].e += conv.e;
    else        regs[16].x += conv.x;
    
    printf(" -> PASSED");

    ASM_rexPrint();
    ASM_end();
    
    if (data->call) {
        ASM_codeFunc func = ASM_getCurrFunc();
        func();
    }
    return true;
}

void ASM_0F_89(Data *data) {
    _ASM_0F_89(data);
}

// JLE long
bool _ASM_0F_8E(Data *data) {
    Reg conv = { .e = data->val };
    ASM_incIP(IS_OP(6, 4), NULL);

    if (!oper)  printf("JLE 0x%.4X", conv.e);
    else        printf("JLE 0x%.2X", conv.x);
    
    if (!f.f.zf && f.f.sf == f.f.of) { // if not equal AND sign == overflow (not less)
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    if (!oper)  regs[16].e += conv.e;
    else        regs[16].x += conv.x;
    
    printf(" -> PASSED");

    ASM_rexPrint();
    ASM_end();
    
    if (data->call) {
        ASM_codeFunc func = ASM_getCurrFunc();
        func();
    }
    return true;
}

void ASM_0F_8E(Data *data) {
    _ASM_0F_8E(data);
}

void ASM_0F_95(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit8);
    ASM_incIP(2, &rm);

    if (rm.isPtr) {
        u32 fdisp = ASM_getDisp(&rm, data->disp);

        ASM_rmPrint("SETNZ", &rm, data->disp, v_None, false);

        STACK(u8, s, fdisp);
        *s = (!f.f.zf);
    } else {
        switch (rm.atype) {
            case R_Bit8:  regs[rm.areg].l = (!f.f.zf); break;
            case R_Bit8H: regs[rm.areg].h = (!f.f.zf); break;
            default: break;
        }

        printf("SETNZ %s", ASM_getRegName(rm.areg, rm.atype));
    }
    
    ASM_rexPrint();
    ASM_end();
}

void _ASM_call_cpuid(int *cpuInfo, u32 eax, u32 ecx) {
#ifdef _WIN64
    __cpuidex(cpuInfo, regs[0].e, regs[1].e);
#else
    __asm__ volatile(
        "cpuid\n\t"
            : "=a" (cpuInfo[0]), "=b" (cpuInfo[1]), "=c" (cpuInfo[2]), "=d" (cpuInfo[3])
            : "a" (eax), "c" (ecx)
    : );
#endif
}

// CPUID
void ASM_0F_A2(Data *data) {
    int cpuInfo[4];

    ASM_incIP(2, NULL);

    _ASM_call_cpuid(cpuInfo, regs[0].e, regs[1].e);

    regs[0].e = cpuInfo[0]; regs[0].eh = 0;
    regs[3].e = cpuInfo[1]; regs[3].eh = 0;
    regs[1].e = cpuInfo[2]; regs[1].eh = 0;
    regs[2].e = cpuInfo[3]; regs[2].eh = 0;

    printf("CPUID");
    ASM_rexPrint();
    ASM_end();
}

// IMUL r(16-64), r/m(16-64)
void ASM_0F_AF(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(3, &rm);

    Reg prev = { 0 };
    Reg res = { 0 };
    s64 val = 0;

    if (rm.isPtr) {
        u32 fdisp = ASM_getDisp(&rm, data->disp);

        ASM_rmPrint("IMUL", &rm, data->disp, v_Reg, true);

        switch (rm.otype) {
            case R_Bit16: {
                STACK(u16, s, fdisp); prev.x = *s;
                val = (s16)*s * (s16)regs[rm.oreg].x;
                regs[rm.oreg].x = val;
                break;
            }
            case R_Bit32: {
                STACK(u32, s, fdisp); prev.e = *s;
                val = (s32)*s * (s32)regs[rm.oreg].e;
                regs[rm.oreg].e = val;
                break; 
            }
            case R_Bit64: {
                STACK(u64, s, fdisp); prev.r = *s;
                val = (s32)*s * (s32)regs[rm.oreg].r;
                regs[rm.oreg].r = val;
                break;
            }
            default: break;
        }
    } else {
        switch (rm.otype) {
            case R_Bit16:
                prev.x = regs[rm.areg].x;
                val = (s16)regs[rm.areg].x * (s16)regs[rm.oreg].x;
                regs[rm.oreg].x = val;
                break;
            case R_Bit32:
                prev.x = regs[rm.areg].e;
                val = (s32)regs[rm.areg].e * (s32)regs[rm.oreg].e;
                regs[rm.oreg].e = val;
                regs[rm.oreg].eh = 0;
                break;
            case R_Bit64:
                prev.x = regs[rm.areg].r;
                val = (s64)regs[rm.areg].r * (s64)regs[rm.oreg].r;
                regs[rm.oreg].r = val;
                break;
            default: break;
        }
        printf("IMUL %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    switch (rm.otype) {
        case R_Bit16: res.x = val; break;
        case R_Bit32: res.e = val; break;
        case R_Bit64: res.r = val; break;
        default: break;
    }

    ASM_setFlags(&prev, &res, rm.otype, false);
    ASM_rexPrint();
    ASM_end();
}

void ASM_0F_B1(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(3, &rm);

    Reg prev = { 0 };
    Reg res = { 0 };

    if (rm.isPtr) {
        u32 fdisp = ASM_getDisp(&rm, data->disp);

        ASM_rmPrint("CMPXCHG", &rm, data->disp, v_Reg, false);

        switch (rm.otype) {
            case R_Bit16: {
                STACK(u16, s, fdisp); prev.x = *s;
                if (*s == regs[0].x) *s = regs[rm.oreg].x;
                else *s = regs[0].x;
                break;
            }
            case R_Bit32: {
                STACK(u32, s, fdisp); prev.e = *s;
                if (*s == regs[0].e) *s = regs[rm.oreg].e;
                else *s = regs[0].e;
                break;
            }
            case R_Bit64: {
                STACK(u64, s, fdisp); prev.r = *s;
                if (*s == regs[0].r) *s = regs[rm.oreg].r;
                else *s = regs[0].r;
                break;
            }
            default: break;
        }
    } else {
        switch (rm.otype) {
            case R_Bit16:
                prev.x = regs[rm.areg].x;
                if (regs[rm.areg].x == regs[0].x) regs[rm.areg].x = regs[rm.oreg].x;
                else regs[rm.areg].x = regs[0].x;
                break;
            case R_Bit32:
                prev.e = regs[rm.areg].e;
                if (regs[rm.areg].e == regs[0].e) regs[rm.areg].e = regs[rm.oreg].e;
                else regs[rm.areg].e = regs[0].e;
                break;
            case R_Bit64:
                prev.r = regs[rm.areg].r;
                if (regs[rm.areg].r == regs[0].r) regs[rm.areg].r = regs[rm.oreg].r;
                else regs[rm.areg].r = regs[0].r;
                break;
            default: break;
        }
        printf("CMPXCHG %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
    }
    
    switch (rm.otype) {
        case R_Bit16: res.x = regs[rm.oreg].x - prev.x; break;
        case R_Bit32: res.e = regs[rm.oreg].e - prev.e; break;
        case R_Bit64: res.r = regs[rm.oreg].r - prev.r; break;
        default: break;
    }

    ASM_setFlags(&prev, &res, rm.otype, true);

    ASM_rexPrint();
    ASM_end();
}

// MOVZX r(16-64), r/m(8)
void ASM_0F_B6(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(3, &rm);

    RegType type = (rex.enable != 0 && rm.oreg >= 4 && rm.oreg < 8) ? R_Bit8H : R_Bit8;

    if (rm.isPtr) {
        u32 fdisp = ASM_getDisp(&rm, data->disp);

        rm.ptrtype = type;
        ASM_rmPrint("MOVZX", &rm, data->disp, v_Reg, true);

        if (type == R_Bit8) {
            switch (rm.otype) {
                case R_Bit16: { STACK(u16, s, fdisp); regs[rm.oreg].l = *s; break; }
                case R_Bit32: { STACK(u32, s, fdisp); regs[rm.oreg].l = *s; break; }
                case R_Bit64: { STACK(u64, s, fdisp); regs[rm.oreg].l = *s; break; }
                default: break;
            }
        } else {
            switch (rm.otype) {
                case R_Bit16: { STACK(u16, s, fdisp); regs[rm.oreg].h = *s; break; }
                case R_Bit32: { STACK(u32, s, fdisp); regs[rm.oreg].h = *s; break; }
                case R_Bit64: { STACK(u64, s, fdisp); regs[rm.oreg].h = *s; break; }
                default: break;
            }
        }
    } else {
        if (type == R_Bit8) {
            switch (rm.otype) {
                case R_Bit16: regs[rm.oreg].l = regs[rm.areg].x; break;
                case R_Bit32: regs[rm.oreg].l = regs[rm.areg].e; break;
                case R_Bit64: regs[rm.oreg].l = regs[rm.areg].r; break;
                default: break;
            }
        } else {
            switch (rm.otype) {
                case R_Bit16: regs[rm.oreg].h = regs[rm.areg].x; break;
                case R_Bit32: regs[rm.oreg].h = regs[rm.areg].e; break;
                case R_Bit64: regs[rm.oreg].h = regs[rm.areg].r; break;
                default: break;
            }
        }
        
        printf("MOVZX %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, type));
    }

    ASM_rexPrint();
    ASM_end();
}

// MOVZX r(16-64), r/m(16)
void ASM_0F_B7(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(3, &rm);

    if (rm.isPtr) {
        u32 fdisp = ASM_getDisp(&rm, data->disp);

        rm.ptrtype = R_Bit16;
        ASM_rmPrint("MOVZX", &rm, data->disp, v_Reg, true);

        switch (rm.otype) {
            case R_Bit16: { STACK(u16, s, fdisp); regs[rm.oreg].x = *s; break; }
            case R_Bit32: { STACK(u32, s, fdisp); regs[rm.oreg].x = *s; break; }
            case R_Bit64: { STACK(u64, s, fdisp); regs[rm.oreg].x = *s; break; }
            default: break;
        }
    } else {
        switch (rm.otype) {
            case R_Bit16: regs[rm.oreg].x = regs[rm.areg].x; break;
            case R_Bit32: regs[rm.oreg].x = regs[rm.areg].e; break;
            case R_Bit64: regs[rm.oreg].x = regs[rm.areg].r; break;
            default: break;
        }
        
        printf("MOVZX %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, R_Bit16));
    }

    ASM_rexPrint();
    ASM_end();
}

#include "x86_0F_BA.c"
void ASM_0F_BA(Data *data) {
    RM ret = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(4, &ret);

    if (ret.reg < 4) {
        printf("UNDEFINED OPCODE: 0F BA /%X", ret.reg);
        exit(EXIT_FAILURE);
    }

    ret.reg -= 4;

    if (ASM_0F_BAFuncs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: 0F BA /%X", ret.reg + 4);
        exit(EXIT_FAILURE);
    }
    ASM_0F_BAFuncs[ret.reg](&ret, data->disp, data->val);

    ASM_end();
}

ASM_dataFunc ASM_0FFuncs[0x100] = {
/* 0X */ 0, ASM_0F_01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 1X */ ASM_0F_10, ASM_0F_11, 0, 0, 0, 0, ASM_0F_16, 0, 0, 0, 0, 0, 0, 0, 0, ASM_0F_1F, 
/* 2X */ 0, 0, 0, 0, 0, 0, 0, 0, ASM_0F_28, ASM_0F_29, 0, 0, 0, 0, 0, 0, 
/* 3X */ 0, ASM_0F_31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 4X */ 0, 0, 0, ASM_0F_43, ASM_0F_44, ASM_0F_45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 5X */ 0, 0, 0, 0, 0, 0, 0, ASM_0F_57, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 6X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ASM_0F_6E, 0, 
/* 7X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ASM_0F_7E, ASM_0F_7F, 
/* 8X */ 0, 0, 0, ASM_0F_83, ASM_0F_84, ASM_0F_85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 9X */ 0, 0, 0, 0, 0, ASM_0F_95, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* AX */ 0, 0, ASM_0F_A2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ASM_0F_AF, 
/* BX */ 0, ASM_0F_B1, 0, 0, 0, 0, ASM_0F_B6, ASM_0F_B7, 0, 0, ASM_0F_BA, 0, 0, 0, 0, 0, 
/* CX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* DX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* EX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* FX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};