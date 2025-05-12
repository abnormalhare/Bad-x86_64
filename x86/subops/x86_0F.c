#include "../data.h"

#include "x86_0F_01.c"
void ASM_0F_01(Data *data) {
    RM ret = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    data->rm = &ret;
    ASM_incIP(3, &ret);

    if (ASM_0F_01Funcs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: 0F 01 /%X", ret.reg + 4);
        exit(EXIT_FAILURE);
    }
    ASM_0F_01Funcs[ret.reg](data);

    ASM_end();
}

//    MOVUPS xmm, r/m(xmm128)
// F3 MOVSS  xmm, r/m(xmm32)
// 66 MOVUPD xmm, r/m(xmm128)
// F2 MOVSD  xmm, r/m(xmm64)
void ASM_0F_10(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Float128);
    ASM_incIP(3, &rm);

    rm.otype = R_Float128;
    if (!rm.isPtr) rm.atype = R_Float128;
    
    if (rm.disp == 1) data->disp = (s8)data->disp;

    if (doub) { // MOVSD
        if (rm.isPtr) {
            s64 fdisp = ASM_getDisp(&rm, data->disp);

            STACK64F(temp, fdisp);
            fregs[rm.oreg].d[0] = *temp;
            ASM_rmPrint("MOVSD", &rm, data->disp, v_Reg, true);
        } else {
            fregs[rm.oreg].d[0] = fregs[rm.areg].d[0];
            printf("MOVSD %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
        }
    } else if (oper) { // MOVUPD
        if (rm.isPtr) {
            s64 fdisp = ASM_getDisp(&rm, data->disp);

            STACK64F(temp,  fdisp);
            STACK64F(temp2, fdisp + 8);
            fregs[rm.oreg].d[0] = *temp;
            fregs[rm.oreg].d[1] = *temp2;

            ASM_rmPrint("MOVUPD", &rm, data->disp, v_Reg, true);
        } else {
            fregs[rm.oreg].xi = fregs[rm.areg].xi;
            printf("MOVUPD %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
        }
    } else if (sing) { // MOVSS
        if (rm.isPtr) {
            s64 fdisp = ASM_getDisp(&rm, data->disp);

            STACK32F(temp, fdisp);
            fregs[rm.oreg].f[0] = *temp;
            ASM_rmPrint("MOVSS", &rm, data->disp, v_Reg, true);
        } else {
            fregs[rm.oreg].f[0] = fregs[rm.areg].f[0];
            printf("MOVSS %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
        }
    } else { // MOVUPS
        if (rm.isPtr) {
            s64 fdisp = ASM_getDisp(&rm, data->disp);

            STACK32F(temp1, fdisp + 0x0);
            STACK32F(temp2, fdisp + 0x4);
            STACK32F(temp3, fdisp + 0x8);
            STACK32F(temp4, fdisp + 0xC);
            fregs[rm.oreg].f[0] = *temp1;
            fregs[rm.oreg].f[1] = *temp2;
            fregs[rm.oreg].f[2] = *temp3;
            fregs[rm.oreg].f[3] = *temp4;

            ASM_rmPrint("MOVUPS", &rm, data->disp, v_Reg, true);
        } else {
            fregs[rm.oreg].xi = fregs[rm.areg].xi;
            printf("MOVUPS %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
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
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Float128);
    ASM_incIP(3, &rm);

    rm.otype = R_Float128;
    if (!rm.isPtr) rm.atype = R_Float128;

    if (doub) { // MOVSD
        if (rm.isPtr) {
            s64 fdisp = ASM_getDisp(&rm, data->disp);

            STACK64F(temp, fdisp);
            *temp = fregs[rm.oreg].d[0];
            ASM_rmPrint("MOVSD", &rm, data->disp, v_Reg, false);
        } else {
            fregs[rm.areg].d[0] = fregs[rm.oreg].d[0];
            printf("MOVSD %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    } else if (oper) { // MOVUPD
        if (rm.isPtr) {
            s64 fdisp = ASM_getDisp(&rm, data->disp);

            STACK64F(temp,  fdisp);
            STACK64F(temp2, fdisp + 8);
            *temp  = fregs[rm.oreg].d[0];
            *temp2 = fregs[rm.oreg].d[1];

            ASM_rmPrint("MOVUPD", &rm, data->disp, v_Reg, false);
        } else {
            fregs[rm.areg].xi = fregs[rm.oreg].xi;
            printf("MOVUPD %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    } else if (sing) { // MOVSS
        if (rm.isPtr) {
            s64 fdisp = ASM_getDisp(&rm, data->disp);

            STACK32F(temp, fdisp);
            *temp = fregs[rm.oreg].f[0];
            ASM_rmPrint("MOVSS", &rm, data->disp, v_Reg, false);
        } else {
            fregs[rm.areg].f[0] = fregs[rm.oreg].f[0];
            printf("MOVSS %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    } else { // MOVUPS
        if (rm.isPtr) {
            s64 fdisp = ASM_getDisp(&rm, data->disp);

            STACK32F(temp1, fdisp + 0x0);
            STACK32F(temp2, fdisp + 0x4);
            STACK32F(temp3, fdisp + 0x8);
            STACK32F(temp4, fdisp + 0xC);
            *temp1 = fregs[rm.oreg].f[0];
            *temp2 = fregs[rm.oreg].f[1];
            *temp3 = fregs[rm.oreg].f[2];
            *temp4 = fregs[rm.oreg].f[3];

            ASM_rmPrint("MOVUPS", &rm, data->disp, v_Reg, false);
        } else {
            fregs[rm.areg].xi = fregs[rm.oreg].xi;
            printf("MOVUPS %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    }

    ASM_rexPrint();
    ASM_end();
}

// MOV(L)HPS xmm, mm/xmm
// NOTE: MOVHPD and MOVSHDUP unimplemented
void ASM_0F_16(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Float128);
    ASM_incIP(3, &rm);

    if (rm.isPtr) { // MOVHPS
        s64 fdisp = ASM_getDisp(&rm, data->disp);

        STACK64(temp, fdisp);
        __m128 temp128 = _mm_castsi128_ps(fregs[rm.oreg].xi);
        temp128 = _mm_loadh_pi(temp128, (const __m64 *)temp);
        fregs[rm.oreg].xi = _mm_castps_si128(temp128);

        rm.otype = R_Float128;
        ASM_rmPrint("MOVHPS", &rm, data->disp, v_Reg, true);
    } else { // MOVLHPS
        __m128 src = _mm_castsi128_ps(fregs[rm.areg].xi);
        __m128 dest = _mm_castsi128_ps(fregs[rm.oreg].xi);
        dest = _mm_movelh_ps(dest, src);
        fregs[rm.oreg].xi = _mm_castps_si128(dest);

        rm.atype = R_Float128;
        rm.otype = R_Float128;
        printf("MOVLHPS %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
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
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Float128);
    ASM_incIP(3, &rm);

    rm.otype = R_Float128;
    if (!rm.isPtr) rm.atype = R_Float128;

    s64 fdisp = ASM_getDisp(&rm, data->disp);

    // check if aligned
    if (((fdisp) / 4) * 4 != fdisp) {
        printf("GENERAL PROTECTION FAULT: %.8X", (u32)fdisp);
        exit(EXIT_FAILURE);
    }
    
    if (rm.isPtr) {
        if (!oper) {
            STACK32F(temp1, fdisp + 0x0);
            STACK32F(temp2, fdisp + 0x4);
            STACK32F(temp3, fdisp + 0x8);
            STACK32F(temp4, fdisp + 0xC);
            fregs[rm.oreg].f[0] = *temp1;
            fregs[rm.oreg].f[1] = *temp2;
            fregs[rm.oreg].f[2] = *temp3;
            fregs[rm.oreg].f[3] = *temp4;
            ASM_rmPrint("MOVAPS", &rm, data->disp, v_Reg, true);
        } else {
            STACK64F(temp1, fdisp + 0x0);
            STACK64F(temp2, fdisp + 0x8);
            fregs[rm.oreg].d[0] = *temp1;
            fregs[rm.oreg].d[1] = *temp2;
            ASM_rmPrint("MOVAPD", &rm, data->disp, v_Reg, true);
        }

    } else {
        fregs[rm.oreg].xi = fregs[rm.areg].xi;
        if (!oper)
            printf("MOVAPS %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
        else
            printf("MOVAPD %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// MOVAPS r/m(xmm), xmm
// MOVAPD r/m(xmm), xmm
void ASM_0F_29(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Float128);
    ASM_incIP(3, &rm);

    rm.otype = R_Float128;
    if (!rm.isPtr) rm.atype = R_Float128;

    s64 fdisp = ASM_getDisp(&rm, data->disp);

    // check if aligned
    if (((fdisp) / 4) * 4 != fdisp) {
        printf("GENERAL PROTECTION FAULT: %.8X", (u32)fdisp);
        exit(EXIT_FAILURE);
    }
    
    if (rm.isPtr) {

        if (!oper) {
            STACK32F(temp1, fdisp + 0x0);
            STACK32F(temp2, fdisp + 0x4);
            STACK32F(temp3, fdisp + 0x8);
            STACK32F(temp4, fdisp + 0xC);
            *temp1 = fregs[rm.oreg].f[0];
            *temp2 = fregs[rm.oreg].f[1];
            *temp3 = fregs[rm.oreg].f[2];
            *temp4 = fregs[rm.oreg].f[3];
            ASM_rmPrint("MOVAPS", &rm, data->disp, v_Reg, false);
        } else {
            STACK64F(temp1, fdisp + 0x0);
            STACK64F(temp2, fdisp + 0x8);
            *temp1 = fregs[rm.oreg].d[0];
            *temp2 = fregs[rm.oreg].d[1];
            ASM_rmPrint("MOVAPD", &rm, data->disp, v_Reg, false);
        }

    } else {
        fregs[rm.areg].xi = fregs[rm.oreg].xi;
        if (!oper)
            printf("MOVAPS %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        else
            printf("MOVAPD %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
    }

    ASM_rexPrint();
    ASM_end();
}

void ASM_0F_31(Data *data) {
    ASM_incIP(2, NULL);

    Reg temp;
    temp.r = __rdtsc();

    regs[2].e = temp.eh; regs[2].eh = 0;
    regs[0].e = temp.e;  regs[0].eh = 0;

    printf("RDTSC");
    ASM_rexPrint();
    ASM_end();
}

// CMOVNB r(16-64), r/m(16-64)
void ASM_0F_43(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(3, &rm);
    
    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, data->disp);

        if (f.f.cf) {
            switch (rm.otype) {
                case R_Bit16: { STACK16(temp, fdisp); regs[rm.oreg].x = *temp; break; }
                case R_Bit32: { STACK32(temp, fdisp); regs[rm.oreg].e = *temp; regs[rm.oreg].eh = 0; break; }
                case R_Bit64: { STACK64(temp, fdisp); regs[rm.oreg].r = *temp; break; }
                default: break;
            }
        } else if (rm.otype == R_Bit32) {
            regs[rm.oreg].eh = 0;
        }

        ASM_rmPrint("CMOVNB", &rm, data->disp, v_Reg, true);
    } else {
        if (f.f.cf) {
            switch (rm.otype) {
                case R_Bit16: regs[rm.oreg].x = regs[rm.areg].x; break;
                case R_Bit32: regs[rm.oreg].e = regs[rm.areg].e; regs[rm.oreg].eh = 0; break;
                case R_Bit64: regs[rm.oreg].r = regs[rm.areg].r; break;
                default: break;
            }
        } else if (rm.otype == R_Bit32) {
            regs[rm.oreg].eh = 0;
        }
        printf("CMOVNB %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// CMOVZ r(16-64), r/m(16-64)
void ASM_0F_44(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(3, &rm);
    
    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, data->disp);

        if (f.f.zf) {
            switch (rm.otype) {
                case R_Bit16: { STACK16(temp, fdisp); regs[rm.oreg].x = *temp; break; }
                case R_Bit32: { STACK32(temp, fdisp); regs[rm.oreg].e = *temp; regs[rm.oreg].eh = 0; break; }
                case R_Bit64: { STACK64(temp, fdisp); regs[rm.oreg].r = *temp; break; }
                default: break;
            }
        } else if (rm.otype == R_Bit32) {
            regs[rm.oreg].eh = 0;
        }

        ASM_rmPrint("CMOVZ", &rm, data->disp, v_Reg, true);
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
        s64 fdisp = ASM_getDisp(&rm, data->disp);

        if (!f.f.zf) {
            switch (rm.otype) {
                case R_Bit16: { STACK16(temp, fdisp); regs[rm.oreg].x = *temp; break; }
                case R_Bit32: { STACK32(temp, fdisp); regs[rm.oreg].e = *temp; regs[rm.oreg].eh = 0; break; }
                case R_Bit64: { STACK64(temp, fdisp); regs[rm.oreg].r = *temp; break; }
                default: break;
            }
        } else if (rm.otype == R_Bit32) {
            regs[rm.oreg].eh = 0;
        }

        ASM_rmPrint("CMOVNZ", &rm, data->disp, v_Reg, true);
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
        s64 fdisp = ASM_getDisp(&rm, data->disp);

        if (!oper) {
            STACK32(temp1, fdisp + 0x0);
            STACK32(temp2, fdisp + 0x4);
            STACK32(temp3, fdisp + 0x8);
            STACK32(temp4, fdisp + 0xC);
            *temp1 ^= fregs[rm.oreg].u[0];
            *temp2 ^= fregs[rm.oreg].u[1];
            *temp3 ^= fregs[rm.oreg].u[2];
            *temp4 ^= fregs[rm.oreg].u[3];

            ASM_rmPrint("XORPS", &rm, data->disp, v_Reg, false);
        } else {
            STACK64(temp1, fdisp + 0x0);
            STACK64(temp2, fdisp + 0x8);
            *temp1 ^= fregs[rm.oreg].ul[0];
            *temp2 ^= fregs[rm.oreg].ul[1];

            ASM_rmPrint("XORPD", &rm, data->disp, v_Reg, false);
        }
    } else {

        if (!oper) {
            __m128 a = _mm_castsi128_ps(fregs[rm.areg].xi);
            __m128 b = _mm_castsi128_ps(fregs[rm.oreg].xi);
            a = _mm_xor_ps(a, b);
            fregs[rm.areg].xi = _mm_castps_si128(a);
            printf("XORPS %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        } else {
            __m128d a = _mm_castsi128_pd(fregs[rm.areg].xi);
            __m128d b = _mm_castsi128_pd(fregs[rm.oreg].xi);
            a = _mm_xor_pd(a, b);
            fregs[rm.areg].xi = _mm_castpd_si128(a);
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
        s64 fdisp = ASM_getDisp(&rm, data->disp);

        switch (rm.otype) {
            case R_MMX: {
                if (rm.atype == R_Bit32) {
                    STACK32(temp, fdisp);
                    xregs[rm.oreg].u = *temp;
                } else {
                    STACK64(temp, fdisp);
                    xregs[rm.oreg].u = *temp;
                }
            }
            case R_Float128: {
                if (rm.atype == R_Bit32) {
                    STACK32(temp, fdisp);
                    fregs[rm.oreg].xi = _mm_cvtsi32_si128(*temp);
                } else {
                    STACK64(temp, fdisp);
                    fregs[rm.oreg].xi = _mm_cvtsi64_si128(*temp);
                }
            }
            default: break;
        }

        if (rm.otype == R_MMX) {
            ASM_rmPrint("MOVD", &rm, data->disp, v_Reg, true);
        } else {
            ASM_rmPrint("MOVQ", &rm, data->disp, v_Reg, true);
        }
    } else {
        rm.atype = IS_W(R_Bit32, R_Bit64);
        switch (rm.otype) {
            case R_MMX:
                if (rm.atype == R_Bit32) {
                    xregs[rm.oreg].u = regs[rm.areg].e;
                } else {
                    xregs[rm.oreg].u = regs[rm.areg].r;
                }
                break;
            case R_Float128:
                if (rm.atype == R_Bit32) {
                    fregs[rm.oreg].xi = _mm_cvtsi32_si128(regs[rm.areg].e);
                } else {
                    fregs[rm.oreg].xi = _mm_cvtsi64_si128(regs[rm.areg].r);
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

    if (sing) { // MOVDQU
        if (rm.isPtr) {
            rm.atype = R_Bit32;

            s64 fdisp = ASM_getDisp(&rm, data->disp);
            STACK64F(temp, fdisp);
            fregs[rm.oreg].d[0] = *temp;
            fregs[rm.oreg].d[1] = 0.0;

            ASM_rmPrint("MOVQ", &rm, data->disp, v_Reg, true);
        } else {
            rm.atype = R_Float128;

            fregs[rm.oreg].d[0] = fregs[rm.areg].d[0];
            fregs[rm.oreg].d[1] = 0;

            printf("MOVQ %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
        }
    } else if (oper) { // MOVD/Q xmm
        rm.atype = IS_W(R_Bit32, R_Bit64);

        if (rm.isPtr) {
            s64 fdisp = ASM_getDisp(&rm, data->disp);

            if (rex.w) {
                STACK64(temp, fdisp);
                *temp = fregs[rm.oreg].ul[0];
                ASM_rmPrint("MOVQ", &rm, data->disp, v_Reg, false);
            } else {
                STACK32(temp, fdisp);
                *temp = fregs[rm.oreg].u[0];
                ASM_rmPrint("MOVD", &rm, data->disp, v_Reg, false);
            }
        } else {
            if (rex.w) {
                regs[rm.areg].r = fregs[rm.oreg].ul[0];
                printf("MOVQ %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
            } else {
                regs[rm.areg].e = fregs[rm.oreg].u[0];
                regs[rm.areg].eh = 0;
                printf("MOVD %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
            }
        }
    } else { // MOVD/Q mm
        rm.otype = R_MMX;
        rm.atype = IS_W(R_Bit32, R_Bit64);

        if (rm.isPtr) {
            s64 fdisp = ASM_getDisp(&rm, data->disp);

            if (rex.w) {
                STACK64(temp, fdisp);
                *temp = xregs[rm.oreg].u;
                ASM_rmPrint("MOVQ", &rm, data->disp, v_Reg, false);
            } else {
                STACK32(temp, fdisp);
                *temp = (u32)xregs[rm.oreg].u;
                ASM_rmPrint("MOVD", &rm, data->disp, v_Reg, false);
            }
        } else {
            if (rex.w) {
                regs[rm.areg].r = xregs[rm.oreg].u;
                printf("MOVQ %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
            } else {
                regs[rm.areg].e = (u32)xregs[rm.oreg].u;
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
            s64 fdisp = ASM_getDisp(&rm, data->disp);

            STACK64F(temp, fdisp);
            *temp = fregs[rm.oreg].d[0];
            ASM_rmPrint("MOVDQU", &rm, data->disp, v_Reg, false);
        } else {
            fregs[rm.areg].d[0] = fregs[rm.oreg].d[0];
            printf("MOVDQU %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    } else if (oper) { // MOVDQA
        s64 fdisp = ASM_getDisp(&rm, data->disp);

        if ((fdisp) / 4 * 4 != fdisp) {
            printf("GENERAL PROTECTION FAULT: %.8X", (u32)fdisp);
            exit(EXIT_FAILURE);
        }
        if (rm.isPtr) {
            STACK64F(temp, fdisp);
            *temp = fregs[rm.oreg].d[0];
            ASM_rmPrint("MOVDQA", &rm, data->disp, v_Reg, false);
        } else {
            fregs[rm.areg].d[0] = fregs[rm.oreg].d[0];
            printf("MOVDQA %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    } else { // MOVQ
        rm.otype = R_MMX;

        if (rm.isPtr) {
            s64 fdisp = ASM_getDisp(&rm, data->disp);

            STACK64(temp, fdisp);
            *temp = xregs[rm.oreg].u;
            ASM_rmPrint("MOVQ", &rm, data->disp, v_Reg, false);
        } else {
            rm.atype = R_MMX;
            xregs[rm.areg].u = xregs[rm.oreg].u;
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

void ASM_0F_95(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit8);
    ASM_incIP(2, &rm);

    if (rm.isPtr) {
        s64 fdisp = ASM_getDisp(&rm, data->disp);

        STACK8(temp, fdisp);
        *temp = (!f.f.zf);

        ASM_rmPrint("SETNZ", &rm, data->disp, v_None, false);
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

// CPUID
void ASM_0F_A2(Data *data) {
    int cpuInfo[4];

    ASM_incIP(2, NULL);

    __cpuidex(cpuInfo, regs[0].e, regs[1].e);

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
        s64 fdisp = ASM_getDisp(&rm, data->disp);

        switch (rm.otype) {
            case R_Bit16: {
                STACK16(temp, fdisp); prev.x = *temp;
                val = (s16)*temp * (s16)regs[rm.oreg].x;
                regs[rm.oreg].x = val;
                break;
            }
            case R_Bit32: {
                STACK32(temp, fdisp); prev.e = *temp;
                val = (s32)*temp * (s32)regs[rm.oreg].e;
                regs[rm.oreg].e = val;
                break; 
            }
            case R_Bit64: {
                STACK64(temp, fdisp); prev.r = *temp;
                val = (s32)*temp * (s32)regs[rm.oreg].r;
                regs[rm.oreg].r = val;
                break;
            }
            default: break;
        }

        ASM_rmPrint("IMUL", &rm, data->disp, v_Reg, true);
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
        s64 fdisp = ASM_getDisp(&rm, data->disp);

        switch (rm.otype) {
            case R_Bit16: {
                STACK16(temp, fdisp); prev.x = *temp;
                if (*temp == regs[0].x) *temp = regs[rm.oreg].x;
                else *temp = regs[0].x;
                break;
            }
            case R_Bit32: {
                STACK32(temp, fdisp); prev.e = *temp;
                if (*temp == regs[0].e) *temp = regs[rm.oreg].e;
                else *temp = regs[0].e;
                break;
            }
            case R_Bit64: {
                STACK64(temp, fdisp); prev.r = *temp;
                if (*temp == regs[0].r) *temp = regs[rm.oreg].r;
                else *temp = regs[0].r;
                break;
            }
            default: break;
        }

        ASM_rmPrint("CMPXCHG", &rm, data->disp, v_Reg, false);
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
        s64 fdisp = ASM_getDisp(&rm, data->disp);

        if (type == R_Bit8) {
            switch (rm.otype) {
                case R_Bit16: { STACK16(temp, fdisp); regs[rm.oreg].l = *temp; break; }
                case R_Bit32: { STACK32(temp, fdisp); regs[rm.oreg].l = *temp; break; }
                case R_Bit64: { STACK64(temp, fdisp); regs[rm.oreg].l = *temp; break; }
                default: break;
            }
        } else {
            switch (rm.otype) {
                case R_Bit16: { STACK16(temp, fdisp); regs[rm.oreg].h = *temp; break; }
                case R_Bit32: { STACK32(temp, fdisp); regs[rm.oreg].h = *temp; break; }
                case R_Bit64: { STACK64(temp, fdisp); regs[rm.oreg].h = *temp; break; }
                default: break;
            }
        }

        rm.ptrtype = type;
        ASM_rmPrint("MOVZX", &rm, data->disp, v_Reg, true);
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
        s64 fdisp = ASM_getDisp(&rm, data->disp);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, fdisp); regs[rm.oreg].x = *temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); regs[rm.oreg].x = *temp; break; }
            case R_Bit64: { STACK64(temp, fdisp); regs[rm.oreg].x = *temp; break; }
            default: break;
        }

        rm.ptrtype = R_Bit16;
        ASM_rmPrint("MOVZX", &rm, data->disp, v_Reg, true);
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