#include "../data.h"

void ASM_F7NOT(RM *rm, s32 disp, u8 val) {
    if (rm->isPtr) {
        s64 fdisp = ASM_getDisp(rm, disp);
        
        switch (rm->otype) {
            case R_Bit16: { STACK16(temp, fdisp); *temp = ~*temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); *temp = ~*temp; break; }
            case R_Bit64: { STACK64(temp, fdisp); *temp = ~*temp; break; }
            default: break;
        }

        ASM_rmPrint("NOT", rm, disp, v_None, false);
    } else {
        switch (rm->otype) {
            case R_Bit16: regs[rm->areg].x = ~regs[rm->areg].x; break;
            case R_Bit32: regs[rm->areg].e = ~regs[rm->areg].e; regs[rm->areg].eh = 0; break;
            case R_Bit64: regs[rm->areg].r = ~regs[rm->areg].r; break;
            default: break;
        }
        printf("NOT %s", ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

void ASM_F7NEG(RM *rm, s32 disp, u8 val) {
    if (rm->isPtr) {
        s64 fdisp = ASM_getDisp(rm, disp);
        
        switch (rm->otype) {
            case R_Bit16: { STACK16(temp, fdisp); *temp = -*temp; break; }
            case R_Bit32: { STACK32(temp, fdisp); *temp = -*temp; break; }
            case R_Bit64: { STACK64(temp, fdisp); *temp = -*temp; break; }
            default: break;
        }

        ASM_rmPrint("NEG", rm, disp, v_None, false);
    } else {
        switch (rm->otype) {
            case R_Bit16: regs[rm->areg].x = -regs[rm->areg].x; break;
            case R_Bit32: regs[rm->areg].e = -regs[rm->areg].e; regs[rm->areg].eh = 0; break;
            case R_Bit64: regs[rm->areg].r = -regs[rm->areg].r; break;
            default: break;
        }
        printf("NEG %s", ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

ASM_valFunc ASM_F7Funcs[8] = {
    0, 0, ASM_F7NOT, ASM_F7NEG,
    0, 0, 0, 0,
};