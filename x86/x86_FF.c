#include "data.h"

void ASM_FFCALL(RM *rm, s32 disp) {
    ASM_codeFunc func;
    u8 s;

    ASM_incIP(2, rm);
    regs[4].r -= 8;
    STACK64(temp, regs[4].e);
    *temp = regs[16].e;

    u64 reg = ASM_getReg(rm->areg, rm->atype);
    
    if (rm->isPtr) {
        if (rm->disp == 1) disp = (s8)disp;
        if (rm->atype == R_Seg) { s = rm->areg; rm->areg = 16; }
        
        switch (rm->atype) {
            case R_Bit16: { regs[16].x = reg + disp; break; }
            case R_Bit32: { regs[16].e = reg + disp; break; }
            case R_Bit64: { regs[16].r = reg + disp; break; }
            default: break;
        }

        if (rm->atype == R_Seg) rm->areg = s;

        ASM_rmPrint("CALL cs:", rm, disp, v_None, false);
        ASM_rexPrint();
    } else {
        switch (rm->atype) {
            case R_Bit16: { regs[16].x = regs[rm->areg].x; break; }
            case R_Bit32: { regs[16].e = regs[rm->areg].e; break; }
            case R_Bit64: { regs[16].r = regs[rm->areg].r; break; }
            default: break;
        }

        printf("CALL cs:%s", ASM_getRegName(rm->areg, rm->atype));
        ASM_rexPrint();
    }
        
    func = ASM_getFunc(regs[16].e);

    func(); // hopefully this wont cause a stack overflow
}

void ASM_FFJMP(RM *rm, s32 disp) {
    ASM_codeFunc func;
    u8 s;

    ASM_incIP(2, rm);

    u64 reg = ASM_getReg(rm->areg, rm->atype);
    
    if (rm->isPtr) {
        if (rm->disp == 1) disp = (s8)disp;
        if (rm->atype == R_Seg) { s = rm->areg; rm->areg = 16; }
        
        switch (rm->atype) {
            case R_Bit16: { regs[16].x = reg + disp; break; }
            case R_Bit32: { regs[16].e = reg + disp; break; }
            case R_Bit64: { regs[16].r = reg + disp; break; }
            default: break;
        }

        if (rm->atype == R_Seg) rm->areg = s;
        ASM_rmPrint("JMP cs:", rm, disp, v_None, false);
        ASM_rexPrint();
    } else {
        switch (rm->atype) {
            case R_Bit16: { regs[16].x = regs[rm->areg].x; break; }
            case R_Bit32: { regs[16].e = regs[rm->areg].e; break; }
            case R_Bit64: { regs[16].r = regs[rm->areg].r; break; }
            default: break;
        }

        printf("JMP cs:%s", ASM_getRegName(rm->areg, rm->atype));
        ASM_rexPrint();
    }

    func = ASM_getFunc(regs[16].e);

    func(); // hopefully this wont cause a stack overflow
}

ASM_baseFunc ASM_FFFuncs[8] = {
    0, 0, ASM_FFCALL, 0,
    ASM_FFJMP, 0, 0, 0,
};