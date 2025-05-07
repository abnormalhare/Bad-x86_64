#include "data.h"

// CMOVZ r(16-64), r/m(16-64)
void ASM_0F_44(Data *data) {
    RM rm = ASM_getRM(data->rm_code, data->sib, R_Bit32);
    ASM_incIP(2, &rm);
    
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

ASM_dataFunc ASM_0FFuncs[0x100] = {
/* 0X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 1X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 2X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 3X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 4X */ 0, 0, 0, 0, 0, ASM_0F_45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 5X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 6X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 7X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 8X */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* 9X */ 0, 0, 0, 0, 0, ASM_0F_95, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* AX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* BX */ 0, ASM_0F_B1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* CX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* DX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* EX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
/* FX */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};