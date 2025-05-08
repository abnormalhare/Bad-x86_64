#include "data.h"

//    MOVUPS r/m(xmm128), xmm
// F3 MOVSS  r/m(xmm32), xmm
// 66 MOVUPD r/m(xmm128), xmm
// F2 MOVSD  r/m(xmm64), xmm
void ASM_0F_11(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Float128);
    ASM_incIP(3, &rm);

    u64 reg = 0;
    if (rm.isPtr) {
        reg = ASM_getReg(rm.areg, rm.atype);
    } else {
        rm.atype = R_Float128;
    }
    rm.otype = R_Float128;
    
    if (rm.disp == 1) data->disp = (s8)data->disp;

    if (doub) { // MOVSD
        if (rm.isPtr) {
            STACK64F(temp, reg + data->disp);
            *temp = fregs[rm.oreg].d[0];
            ASM_rmPrint("MOVSD", &rm, data->disp, v_Reg, false);
        } else {
            fregs[rm.areg].d[0] = fregs[rm.oreg].d[0];
            printf("MOVSD %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    } else if (oper) { // MOVUPD
        if (rm.isPtr) {
            STACK64F(temp, reg + data->disp);
            STACK64F(temp2, reg + data->disp + 8);
            *temp  = fregs[rm.oreg].d[0];
            *temp2 = fregs[rm.oreg].d[1];

            ASM_rmPrint("MOVUPD", &rm, data->disp, v_Reg, false);
        } else {
            fregs[rm.areg].xi = fregs[rm.oreg].xi;
            printf("MOVUPD %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    } else if (sing) { // MOVSS
        if (rm.isPtr) {
            STACK32F(temp, reg + data->disp);
            *temp = fregs[rm.oreg].f[0];
            ASM_rmPrint("MOVSS", &rm, data->disp, v_Reg, false);
        } else {
            fregs[rm.areg].f[0] = fregs[rm.oreg].f[0];
            printf("MOVSS %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
        }
    } else { // MOVUPS
        if (rm.isPtr) {
            STACK32F(temp1, reg + data->disp + 0x0);
            STACK32F(temp2, reg + data->disp + 0x4);
            STACK32F(temp3, reg + data->disp + 0x8);
            STACK32F(temp4, reg + data->disp + 0xC);
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

    if (rm.isPtr) {
        // MOVHPS: Load high quadword from memory into the high part of the XMM register
        u64 reg = ASM_getReg(rm.areg, rm.atype);
        if (rm.disp == 1) data->disp = (s8)data->disp;

        STACK64(temp, reg + data->disp);
        __m128 temp128 = _mm_castsi128_ps(fregs[rm.oreg].xi); // Cast __m128i to __m128
        temp128 = _mm_loadh_pi(temp128, (const __m64 *)temp);
        fregs[rm.oreg].xi = _mm_castps_si128(temp128); // Cast back to __m128i

        rm.otype = R_Float128;
        ASM_rmPrint("MOVHPS", &rm, data->disp, v_Reg, true);
    } else {
        // MOVLHPS: Move low quadword of one XMM register to the high quadword of another
        __m128 src = _mm_castsi128_ps(fregs[rm.areg].xi); // Source XMM register
        __m128 dest = _mm_castsi128_ps(fregs[rm.oreg].xi); // Destination XMM register
        dest = _mm_movelh_ps(dest, src); // Move low quadword of src to high quadword of dest
        fregs[rm.oreg].xi = _mm_castps_si128(dest); // Cast back to __m128i

        rm.atype = R_Float128;
        rm.otype = R_Float128;
        printf("MOVLHPS %s, %s", ASM_getRegName(rm.oreg, rm.otype), ASM_getRegName(rm.areg, rm.atype));
    }

    ASM_rexPrint();
    ASM_end();
}

void ASM_0F_1F(Data *data) {
    // NOP
}

void ASM_0F_29(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Float128);
    ASM_incIP(3, &rm);

    u64 reg = 0;
    if (rm.isPtr) reg = ASM_getReg(rm.areg, rm.atype);
    else rm.atype = R_Float128;
    rm.otype = R_Float128;

    if (rm.disp == 1) data->disp = (s8)data->disp;

    // check if aligned
    if ((reg + data->disp) / 4 + 4 != reg + data->disp) {
        printf("GENERAL PROTECTION FAULT: 0F 29");
        exit(EXIT_FAILURE);
    }
    
    if (rm.isPtr) {
        if (!oper) {
            STACK32F(temp1, reg + data->disp + 0x0);
            STACK32F(temp2, reg + data->disp + 0x4);
            STACK32F(temp3, reg + data->disp + 0x8);
            STACK32F(temp4, reg + data->disp + 0xC);
            *temp1 = fregs[rm.oreg].f[0];
            *temp2 = fregs[rm.oreg].f[1];
            *temp3 = fregs[rm.oreg].f[2];
            *temp4 = fregs[rm.oreg].f[3];
            ASM_rmPrint("MOVAPS", &rm, data->disp, v_Reg, false);
        } else {
            STACK64F(temp1, reg + data->disp + 0x0);
            STACK64F(temp2, reg + data->disp + 0x8);
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
}

// CMOVZ r(16-64), r/m(16-64)
void ASM_0F_44(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(3, &rm);
    
    if (rm.isPtr) {
        u64 reg = ASM_getReg(rm.areg, rm.atype);
        if (rm.disp == 1) data->disp = (s8)data->disp;

        if (f.f.zf) {
            switch (rm.otype) {
                case R_Bit16: { STACK16(temp, reg + data->disp); regs[rm.oreg].x = *temp; break; }
                case R_Bit32: { STACK32(temp, reg + data->disp); regs[rm.oreg].e = *temp; regs[rm.oreg].eh = 0; break; }
                case R_Bit64: { STACK64(temp, reg + data->disp); regs[rm.oreg].r = *temp; break; }
                default: break;
            }
        } else if (rm.otype == R_Bit32) {
            regs[rm.oreg].eh = 0;
        }

        ASM_rmPrint("CMOVZ", &rm, data->disp, v_Reg, true);
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
        u64 reg = ASM_getReg(rm.areg, rm.atype);
        if (rm.disp == 1) data->disp = (s8)data->disp;

        if (!f.f.zf) {
            switch (rm.otype) {
                case R_Bit16: { STACK16(temp, reg + data->disp); regs[rm.oreg].x = *temp; break; }
                case R_Bit32: { STACK32(temp, reg + data->disp); regs[rm.oreg].e = *temp; regs[rm.oreg].eh = 0; break; }
                case R_Bit64: { STACK64(temp, reg + data->disp); regs[rm.oreg].r = *temp; break; }
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

// MOVD mm/xmm, r/m(32) / MOVQ mm/xmm, r/m(64)
void ASM_0F_6E(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_MMX);
    ASM_incIP(3, &rm);

    if (rm.isPtr) {
        u64 reg = ASM_getReg(rm.areg, rm.atype);
        if (rm.disp == 1) data->disp = (s8)data->disp;

        switch (rm.otype) {
            case R_MMX: {
                if (rm.atype == R_Bit32) {
                    STACK32(temp, reg + data->disp);
                    xregs[rm.oreg].u = *temp;
                } else {
                    STACK64(temp, reg + data->disp);
                    xregs[rm.oreg].u = *temp;
                }
            }
            case R_Float128: {
                if (rm.atype == R_Bit32) {
                    STACK32(temp, reg + data->disp);
                    fregs[rm.oreg].xi = _mm_cvtsi32_si128(*temp);
                } else {
                    STACK64(temp, reg + data->disp);
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

// JNB long
void ASM_0F_83(Data *data) {
    Reg conv = { .e = data->val };
    ASM_incIP(IS_OP(6, 4), NULL);

    if (!oper) {
        printf("JNB 0x%.4X", conv.e);
    } else {
        printf("JNB 0x%.2X", conv.l);
    }
    
    if (f.f.cf) { // if carry, dont jump
        ASM_rexPrint();
        ASM_end();
        return;
    }

    if (!oper) {
        regs[16].x += conv.e;
    } else {
        regs[16].e += conv.l;
    }
    printf(" -> PASSED");


    ASM_rexPrint();
    ASM_end();

    ASM_codeFunc func = ASM_getCurrFunc();
    func();
}

// JZ long
bool ASM_0F_85(Data *data) {
    Reg conv = { .e = data->val };
    ASM_incIP(IS_OP(6, 4), NULL);

    if (!oper) {
        printf("JNZ 0x%.4X", conv.e);
    } else {
        printf("JNZ 0x%.2X", conv.l);
    }
    
    if (f.f.zf) { // if it is zero, dont jump
        ASM_rexPrint();
        ASM_end();
        return false;
    }

    if (!oper) {
        regs[16].x += conv.e;
    } else {
        regs[16].e += conv.l;
    }
    printf(" -> PASSED");

    ASM_rexPrint();
    ASM_end();
    return true;
}

void ASM_0F_95(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit8);
    ASM_incIP(2, &rm);

    if (rm.isPtr) {
        u64 reg = ASM_getReg(rm.areg, rm.atype);
        if (rm.disp == 1) data->disp = (s8)data->disp;

        STACK8(temp, reg + data->disp);
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

// IMUL r(16-64), r/m(16-64)
void ASM_0F_AF(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(3, &rm);

    Reg prev = { 0 };
    Reg res = { 0 };
    s64 val = 0;

    if (rm.isPtr) {
        u64 reg = ASM_getReg(rm.areg, rm.atype);

        switch (rm.otype) {
            case R_Bit16: {
                STACK16(temp, reg + data->disp); prev.x = *temp;
                val = (s16)*temp * (s16)regs[rm.oreg].x;
                regs[rm.oreg].x = val;
                break;
            }
            case R_Bit32: {
                STACK32(temp, reg + data->disp); prev.e = *temp;
                val = (s32)*temp * (s32)regs[rm.oreg].e;
                regs[rm.oreg].e = val;
                break; 
            }
            case R_Bit64: {
                STACK64(temp, reg + data->disp); prev.r = *temp;
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
        u64 reg = ASM_getReg(rm.areg, rm.atype);
        if (rm.disp == 1) data->disp = (s8)data->disp;

        switch (rm.otype) {
            case R_Bit16: {
                STACK16(temp, reg + data->disp); prev.x = *temp;
                if (*temp == regs[0].x) *temp = regs[rm.oreg].x;
                else *temp = regs[0].x;
                break;
            }
            case R_Bit32: {
                STACK32(temp, reg + data->disp); prev.e = *temp;
                if (*temp == regs[0].e) *temp = regs[rm.oreg].e;
                else *temp = regs[0].e;
                break;
            }
            case R_Bit64: {
                STACK64(temp, reg + data->disp); prev.r = *temp;
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

// MOVZX r/m(16-64), r(8)
void ASM_0F_B6(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(3, &rm);

    RegType type = (rex.enable != 0 && rm.oreg >= 4 && rm.oreg < 8) ? R_Bit8H : R_Bit8;
    u64 oreg = ASM_getReg(rm.oreg, type);

    if (rm.isPtr) {
        u64 reg =  ASM_getReg(rm.areg, rm.atype);

        switch (rm.otype) {
            case R_Bit16: { STACK16(temp, reg + data->disp); *temp = oreg; break; }
            case R_Bit32: { STACK32(temp, reg + data->disp); *temp = oreg; break; }
            case R_Bit64: { STACK64(temp, reg + data->disp); *temp = oreg; break; }
            default: break;
        }

        rm.otype = type;
        ASM_rmPrint("MOVZX", &rm, data->disp, v_Reg, false);
    } else {
        switch (rm.otype) {
            case R_Bit16: regs[rm.areg].x = oreg; break;
            case R_Bit32: regs[rm.areg].e = oreg; regs[rm.areg].eh = 0; break;
            case R_Bit64: regs[rm.areg].r = oreg; break;
            default: break;
        }
        printf("MOVZX %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, type));
    }

    ASM_rexPrint();
    ASM_end();
}

ASM_dataFunc ASM_0FFuncs[0x100] = {
/* 0X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 1X */ 0, ASM_0F_11, 0, 0, 0, 0, ASM_0F_16, 0, 0, 0, 0, 0, 0, 0, 0, ASM_0F_1F, 
/* 2X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, ASM_0F_29, 0, 0, 0, 0, 0, 0, 
/* 3X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 4X */ 0, 0, 0, 0, 0, ASM_0F_45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 5X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 6X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ASM_0F_6E, 0, 
/* 7X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 8X */ 0, 0, 0, ASM_0F_83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 9X */ 0, 0, 0, 0, 0, ASM_0F_95, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* AX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ASM_0F_AF, 
/* BX */ 0, ASM_0F_B1, 0, 0, 0, 0, ASM_0F_B6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* CX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* DX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* EX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* FX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};