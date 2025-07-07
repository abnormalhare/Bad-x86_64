#include "../data.h"

void ASM_FFINC(RM *rm, s32 disp) {
    Reg prev = { 0 };
    Reg res = { 0 };

    ASM_incIP(2, rm);

    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        ASM_rmPrint("INC", rm, disp, v_None, false);
        
        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); prev.x = (*s)++; break; }
            case R_Bit32: { STACK(u32, s, fdisp); prev.e = (*s)++; break; }
            case R_Bit64: { STACK(u64, s, fdisp); prev.r = (*s)++; break; }
            default: break;
        }
    } else {
        switch (rm->otype) {
            case R_Bit16: { prev.x = regs[rm->areg].x++; break; }
            case R_Bit32: { prev.e = regs[rm->areg].e++; break; }
            case R_Bit64: { prev.r = regs[rm->areg].r++; break; }
            default: break;
        }
        printf("INC %s", ASM_getRegName(rm->areg, rm->atype));
    }

    res.l = prev.l + 1;

    Flags s = { .f.df = f.f.df, .f.iF = f.f.iF, .f.cf = f.f.cf };
    ASM_setFlags(&prev, &res, rm->otype, false);
    f.f.df = s.f.df; f.f.iF = s.f.iF; f.f.cf = s.f.cf;

    ASM_rexPrint();
}

void ASM_FFDEC(RM *rm, s32 disp) {
    Reg prev = { 0 };
    Reg res = { 0 };

    ASM_incIP(2, rm);

    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);

        ASM_rmPrint("DEC", rm, disp, v_None, false);
        
        switch (rm->otype) {
            case R_Bit16: { STACK(u16, s, fdisp); prev.x = (*s)--; break; }
            case R_Bit32: { STACK(u32, s, fdisp); prev.e = (*s)--; break; }
            case R_Bit64: { STACK(u64, s, fdisp); prev.r = (*s)--; break; }
            default: break;
        }
    } else {
        switch (rm->otype) {
            case R_Bit16: { prev.x = regs[rm->areg].x--; break; }
            case R_Bit32: { prev.e = regs[rm->areg].e--; break; }
            case R_Bit64: { prev.r = regs[rm->areg].r--; break; }
            default: break;
        }
        printf("DEC %s", ASM_getRegName(rm->areg, rm->atype));
    }

    res.l = prev.l - 1;

    Flags s = { .f.df = f.f.df, .f.iF = f.f.iF, .f.cf = f.f.cf };
    ASM_setFlags(&prev, &res, rm->otype, false);
    f.f.df = s.f.df; f.f.iF = s.f.iF; f.f.cf = s.f.cf;

    ASM_rexPrint();
}

void ASM_FFCALL(RM *rm, s32 disp) {
    ASM_codeFunc func;
    
    ASM_incIP(2, rm);
    regs[4].r -= 8;
    STACK(u64, s, regs[4].e);
    *s = regs[16].e;
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);
        STACK(u64, fi, fdisp);

        ASM_rmPrint("CALL cs:", rm, *fi, v_None, false);
        
        switch (rm->atype) {
            case R_Bit16: { regs[16].x = *fi; break; }
            case R_Bit32: { regs[16].e = *fi; break; }
            case R_Bit64: { regs[16].r = *fi; break; }
            default: break;
        }
        ASM_rexPrint();
    } else {
        switch (rm->atype) {
            case R_Bit16: { STACK(u16, fi, regs[rm->areg].x); regs[16].x = *fi; break; }
            case R_Bit32: { STACK(u32, fi, regs[rm->areg].x); regs[16].e = *fi; break; }
            case R_Bit64: { STACK(u64, fi, regs[rm->areg].x); regs[16].r = *fi; break; }
            default: break;
        }

        printf("CALL cs:%s", ASM_getRegName(rm->areg, rm->atype));
        ASM_rexPrint();
    }
    
    func = ASM_getCurrFunc();

    func(); // hopefully this wont cause a stack overflow
}

void ASM_FFJMP(RM *rm, s32 disp) {
    ASM_codeFunc func;
    u8 s;

    ASM_incIP(2, rm);
    
    if (rm->isPtr) {
        u32 fdisp = ASM_getDisp(rm, disp);
        STACK(u64, fi, fdisp);
        
        if (rm->atype == R_Seg) rm->areg = s;

        ASM_rmPrint("JMP cs:", rm, *fi, v_None, false);
        
        switch (rm->atype) {
            case R_Bit16: { regs[16].x = *fi; break; }
            case R_Bit32: { regs[16].e = *fi; break; }
            case R_Bit64: { regs[16].r = *fi; break; }
            default: break;
        }

        ASM_rexPrint();
    } else {
        switch (rm->atype) {
            case R_Bit16: { STACK(u16, fi, regs[rm->areg].x); regs[16].x = *fi; break; }
            case R_Bit32: { STACK(u32, fi, regs[rm->areg].x); regs[16].e = *fi; break; }
            case R_Bit64: { STACK(u64, fi, regs[rm->areg].x); regs[16].r = *fi; break; }
            default: break;
        }

        printf("JMP cs:%s", ASM_getRegName(rm->areg, rm->atype));
        ASM_rexPrint();
    }

    func = ASM_getCurrFunc();

    func(); // hopefully this wont cause a stack overflow
}

ASM_baseFunc ASM_FFFuncs[8] = {
    ASM_FFINC, ASM_FFDEC, ASM_FFCALL, 0,
    ASM_FFJMP, 0, 0, 0,
};