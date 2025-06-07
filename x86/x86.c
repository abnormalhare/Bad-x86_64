#include "x86.h"

// ADD r/m(8), r(8)
void ASM_00(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_ADD_u8(rm, fdisp, false);

    if (rm->isPtr) {
        ASM_rmPrint("ADD", rm, disp, v_Reg, false);
    } else {
        printf("ADD %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// ADD r/m(16-64), r(16-64)
void ASM_01(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_ADD_u16(rm, fdisp, false); break;
        case R_Bit32: ASM_ADD_u32(rm, fdisp, false); break;
        case R_Bit64: ASM_ADD_u64(rm, fdisp, false); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("ADD", rm, disp, v_Reg, false);
    } else {
        printf("ADD %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// ADD r(8), r/m(8)
void ASM_02(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_ADD_u8(rm, fdisp, true);

    if (rm->isPtr) {
        ASM_rmPrint("ADD", rm, disp, v_Reg, true);
    } else {
        printf("ADD %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// ADD r(16-64), r/m(16-64)
void ASM_03(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_ADD_u16(rm, fdisp, true); break;
        case R_Bit32: ASM_ADD_u32(rm, fdisp, true); break;
        case R_Bit64: ASM_ADD_u64(rm, fdisp, true); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("ADD", rm, disp, v_Reg, true);
    } else {
        printf("ADD %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// ADD al, imm(8)
void ASM_04(u8 imm) {
    ASM_incIP(2, NULL);
    
    u8 a = 0; u8 b = 0; u8 res = 0;
    a = regs[0].l; b = imm;
    regs[0].l = (a += b);

    ASM_ADD_FLAGS(8, a, b, res);
    printf("ADD al, 0x%.2X", imm);

    ASM_rexPrint();
    ASM_end();
}

// ADD r/e/ax, imm(16-32)
void ASM_05(u32 imm) {
    if (oper) { ASM_incIP(4, NULL);
        if (imm > UINT16_MAX) {
            printf("ERROR: Improper immediate size");
            exit(EXIT_FAILURE);
        }
        u16 a = 0; u16 b = 0; u16 res = 0;
        a = regs[0].x; b = imm;
        regs[0].x = res = (a + b);

        ASM_ADD_FLAGS(16, a, b, res);
        printf("ADD ax, 0x%.4X", imm);
    } else if (rex.w) { ASM_incIP(6, NULL);
        u64 a = 0; u64 b = 0; u64 res = 0;
        a = regs[0].r; b = imm;
        regs[0].r = res = (a + b);

        ASM_ADD_FLAGS(64, a, b, res);
        printf("ADD rax, 0x%.8X", imm);
    } else { ASM_incIP(5, NULL);
        u32 a = 0; u32 b = 0; u32 res = 0;
        a = regs[0].e; b = imm;
        regs[0].e = res = (a + b);

        ASM_ADD_FLAGS(32, a, b, res);
        printf("ADD eax, 0x%.8X", imm);
    }

    ASM_rexPrint();
    ASM_end();
}

// 0x06-07 invalid

// OR r/m(8), r(8)
void ASM_08(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_OR_u8(rm, fdisp, false);

    if (rm->isPtr) {
        ASM_rmPrint("OR", rm, disp, v_Reg, false);
    } else {
        printf("OR %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// OR r/m(16-64), r(16-64)
void ASM_09(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_OR_u16(rm, fdisp, false); break;
        case R_Bit32: ASM_OR_u32(rm, fdisp, false); break;
        case R_Bit64: ASM_OR_u64(rm, fdisp, false); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("OR", rm, disp, v_Reg, false);
    } else {
        printf("OR %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// OR r(8), r/m(8)
void ASM_0A(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_OR_u8(rm, fdisp, true);

    if (rm->isPtr) {
        ASM_rmPrint("OR", rm, disp, v_Reg, true);
    } else {
        printf("OR %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// OR r(16-64), r/m(16-64)
void ASM_0B(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_OR_u16(rm, fdisp, true); break;
        case R_Bit32: ASM_OR_u32(rm, fdisp, true); break;
        case R_Bit64: ASM_OR_u64(rm, fdisp, true); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("OR", rm, disp, v_Reg, true);
    } else {
        printf("OR %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// OR al, imm(8)
void ASM_0C(u8 imm) {
    ASM_incIP(2, NULL);
    
    u8 a = 0; u8 b = 0; u8 res = 0;
    a = regs[0].l; b = imm;
    regs[0].l = (a |= b);

    ASM_BIT_FLAGS(8, a, b, res);
    printf("OR al, 0x%.2X", imm);

    ASM_rexPrint();
    ASM_end();
}

// OR r/e/ax, imm(16-32)
void ASM_0D(u32 imm) {
    if (oper) { ASM_incIP(4, NULL);
        if (imm > UINT16_MAX) {
            printf("ERROR: Improper immediate size");
            exit(EXIT_FAILURE);
        }
        u16 a = 0; u16 b = 0; u16 res = 0;
        a = regs[0].x; b = imm;
        regs[0].x = res = (a | b);

        ASM_BIT_FLAGS(16, a, b, res);
        printf("OR ax, 0x%.4X", imm);
    } else if (rex.w) { ASM_incIP(6, NULL);
        u64 a = 0; u64 b = 0; u64 res = 0;
        a = regs[0].r; b = imm;
        regs[0].r = res = (a | b);

        ASM_BIT_FLAGS(64, a, b, res);
        printf("OR rax, 0x%.8X", imm);
    } else { ASM_incIP(5, NULL);
        u32 a = 0; u32 b = 0; u32 res = 0;
        a = regs[0].e; b = imm;
        regs[0].e = res = (a | b);

        ASM_BIT_FLAGS(32, a, b, res);
        printf("OR eax, 0x%.8X", imm);
    }

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

// ADC r/m(8), r(8)
void ASM_10(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_ADC_u8(rm, fdisp, false);

    if (rm->isPtr) {
        ASM_rmPrint("ADC", rm, disp, v_Reg, false);
    } else {
        printf("ADC %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// ADC r/m(16-64), r(16-64)
void ASM_11(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_ADC_u16(rm, fdisp, false); break;
        case R_Bit32: ASM_ADC_u32(rm, fdisp, false); break;
        case R_Bit64: ASM_ADC_u64(rm, fdisp, false); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("ADC", rm, disp, v_Reg, false);
    } else {
        printf("ADC %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// ADC r(8), r/m(8)
void ASM_12(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_ADC_u8(rm, fdisp, true);

    if (rm->isPtr) {
        ASM_rmPrint("ADC", rm, disp, v_Reg, true);
    } else {
        printf("ADC %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// ADC r(16-64), r/m(16-64)
void ASM_13(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_ADC_u16(rm, fdisp, true); break;
        case R_Bit32: ASM_ADC_u32(rm, fdisp, true); break;
        case R_Bit64: ASM_ADC_u64(rm, fdisp, true); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("ADC", rm, disp, v_Reg, true);
    } else {
        printf("ADC %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// ADC al, imm(8)
void ASM_14(u8 imm) {
    ASM_incIP(2, NULL);
    
    u8 a = 0; u8 b = 0; u8 res = 0;
    a = regs[0].l; b = imm;
    regs[0].l = (a += b + f.f.cf);

    ASM_ADC_FLAGS(8, a, b, res);
    printf("ADC al, 0x%.2X", imm);

    ASM_rexPrint();
    ASM_end();
}

// ADC r/e/ax, imm(16-32)
void ASM_15(u32 imm) {
    if (oper) { ASM_incIP(4, NULL);
        if (imm > UINT16_MAX) {
            printf("ERROR: Improper immediate size");
            exit(EXIT_FAILURE);
        }
        u16 a = 0; u16 b = 0; u16 res = 0;
        a = regs[0].x; b = imm;
        regs[0].x = res = (a + b + f.f.cf);

        ASM_ADC_FLAGS(16, a, b, res);
        printf("ADC ax, 0x%.4X", imm);
    } else if (rex.w) { ASM_incIP(6, NULL);
        u64 a = 0; u64 b = 0; u64 res = 0;
        a = regs[0].r; b = imm;
        regs[0].r = res = (a + b + f.f.cf);

        ASM_ADC_FLAGS(64, a, b, res);
        printf("ADC rax, 0x%.8X", imm);
    } else { ASM_incIP(5, NULL);
        u32 a = 0; u32 b = 0; u32 res = 0;
        a = regs[0].e; b = imm;
        regs[0].e = res = (a + b + f.f.cf);

        ASM_ADC_FLAGS(32, a, b, res);
        printf("ADC eax, 0x%.8X", imm);
    }

    ASM_rexPrint();
    ASM_end();
}

// 0x16-17 invalid

// SBB r/m(8), r(8)
void ASM_18(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_SBB_u8(rm, fdisp, false);

    if (rm->isPtr) {
        ASM_rmPrint("SBB", rm, disp, v_Reg, false);
    } else {
        printf("SBB %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// SBB r/m(16-64), r(16-64)
void ASM_19(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_SBB_u16(rm, fdisp, false); break;
        case R_Bit32: ASM_SBB_u32(rm, fdisp, false); break;
        case R_Bit64: ASM_SBB_u64(rm, fdisp, false); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("SBB", rm, disp, v_Reg, false);
    } else {
        printf("SBB %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// SBB r(8), r/m(8)
void ASM_1A(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_SBB_u8(rm, fdisp, true);

    if (rm->isPtr) {
        ASM_rmPrint("SBB", rm, disp, v_Reg, true);
    } else {
        printf("SBB %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// SBB r(16-64), r/m(16-64)
void ASM_1B(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_SBB_u16(rm, fdisp, true); break;
        case R_Bit32: ASM_SBB_u32(rm, fdisp, true); break;
        case R_Bit64: ASM_SBB_u64(rm, fdisp, true); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("SBB", rm, disp, v_Reg, true);
    } else {
        printf("SBB %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// SBB al, imm(8)
void ASM_1C(u8 imm) {
    ASM_incIP(2, NULL);
    
    u8 a = 0; u8 b = 0; u8 res = 0;
    a = regs[0].l; b = imm;
    regs[0].l = (a -= b + f.f.cf);

    ASM_SBB_FLAGS(8, a, b, res);
    printf("SBB al, 0x%.2X", imm);

    ASM_rexPrint();
    ASM_end();
}

// SBB r/e/ax, imm(16/32)
void ASM_1D(u32 imm) {
    if (oper) { ASM_incIP(4, NULL);
        if (imm > UINT16_MAX) {
            printf("ERROR: Improper immediate size");
            exit(EXIT_FAILURE);
        }
        u16 a = 0; u16 b = 0; u16 res = 0;
        a = regs[0].x; b = imm;
        regs[0].x = (a -= b + f.f.cf);

        ASM_SBB_FLAGS(16, a, b, res);
        printf("SBB ax, 0x%.4X", imm);
    } else if (rex.w) { ASM_incIP(6, NULL);
        u64 a = 0; u64 b = 0; u64 res = 0;
        a = regs[0].r; b = imm;
        regs[0].r = (a -= b + f.f.cf);

        ASM_SBB_FLAGS(64, a, b, res);
        printf("SBB rax, 0x%.8X", imm);
    } else { ASM_incIP(5, NULL);
        u32 a = 0; u32 b = 0; u32 res = 0;
        a = regs[0].e; b = imm;
        regs[0].e = (a -= b + f.f.cf);

        ASM_SBB_FLAGS(32, a, b, res);
        printf("SBB eax, 0x%.8X", imm);
    }

    ASM_rexPrint();
    ASM_end();
}

// 0x1E-1F invalid

// AND r/m(8), r(8)
void ASM_20(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_AND_u8(rm, fdisp, false);

    if (rm->isPtr) {
        ASM_rmPrint("AND", rm, disp, v_Reg, false);
    } else {
        printf("AND %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// AND r/m(16-64), r(16-64)
void ASM_21(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_AND_u16(rm, fdisp, false); break;
        case R_Bit32: ASM_AND_u32(rm, fdisp, false); break;
        case R_Bit64: ASM_AND_u64(rm, fdisp, false); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("AND", rm, disp, v_Reg, false);
    } else {
        printf("AND %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// AND r(8), r/m(8)
void ASM_22(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_AND_u8(rm, fdisp, true);

    if (rm->isPtr) {
        ASM_rmPrint("AND", rm, disp, v_Reg, true);
    } else {
        printf("AND %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// AND r(16-64), r/m(16-64)
void ASM_23(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_AND_u16(rm, fdisp, true); break;
        case R_Bit32: ASM_AND_u32(rm, fdisp, true); break;
        case R_Bit64: ASM_AND_u64(rm, fdisp, true); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("AND", rm, disp, v_Reg, true);
    } else {
        printf("AND %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// AND al, imm(8)
void ASM_24(u8 imm) {
    ASM_incIP(2, NULL);
    
    u8 a = 0; u8 b = 0; u8 res = 0;
    a = regs[0].l; b = imm;
    regs[0].l = (a &= b);

    ASM_BIT_FLAGS(8, a, b, res);
    printf("AND al, 0x%.2X", imm);

    ASM_rexPrint();
    ASM_end();
}

// AND r/e/ax, imm(16-32)
void ASM_25(u32 imm) {
    if (oper) { ASM_incIP(4, NULL);
        if (imm > UINT16_MAX) {
            printf("ERROR: Improper immediate size");
            exit(EXIT_FAILURE);
        }
        u16 a = 0; u16 b = 0; u16 res = 0;
        a = regs[0].x; b = imm;
        regs[0].x = res = (a & b);

        ASM_BIT_FLAGS(16, a, b, res);
        printf("AND ax, 0x%.4X", imm);
    } else if (rex.w) { ASM_incIP(6, NULL);
        u64 a = 0; u64 b = 0; u64 res = 0;
        a = regs[0].r; b = imm;
        regs[0].r = res = (a & b);

        ASM_BIT_FLAGS(64, a, b, res);
        printf("AND rax, 0x%.8X", imm);
    } else { ASM_incIP(5, NULL);
        u32 a = 0; u32 b = 0; u32 res = 0;
        a = regs[0].e; b = imm;
        regs[0].e = res = (a & b);

        ASM_BIT_FLAGS(32, a, b, res);
        printf("AND eax, 0x%.8X", imm);
    }

    ASM_rexPrint();
    ASM_end();
}

// NULL
void ASM_26(void) {
    null = true;
}

// 0x27 invalid

// SUB r/m(8), r(8)
void ASM_28(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_SUB_u8(rm, fdisp, false);

    if (rm->isPtr) {
        ASM_rmPrint("SUB", rm, disp, v_Reg, false);
    } else {
        printf("SUB %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// SUB r/m(16-64), r(16-64)
void ASM_29(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_SUB_u16(rm, fdisp, false); break;
        case R_Bit32: ASM_SUB_u32(rm, fdisp, false); break;
        case R_Bit64: ASM_SUB_u64(rm, fdisp, false); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("SUB", rm, disp, v_Reg, false);
    } else {
        printf("SUB %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// SUB r(8), r/m(8)
void ASM_2A(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_SUB_u8(rm, fdisp, true);

    if (rm->isPtr) {
        ASM_rmPrint("SUB", rm, disp, v_Reg, true);
    } else {
        printf("SUB %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// SUB r(16-64), r/m(16-64)
void ASM_2B(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_SUB_u16(rm, fdisp, true); break;
        case R_Bit32: ASM_SUB_u32(rm, fdisp, true); break;
        case R_Bit64: ASM_SUB_u64(rm, fdisp, true); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("SUB", rm, disp, v_Reg, true);
    } else {
        printf("SUB %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// SUB al, imm(8)
void ASM_2C(u8 imm) {
    ASM_incIP(2, NULL);
    
    u8 a = 0; u8 b = 0; u8 res = 0;
    a = regs[0].l; b = imm;
    regs[0].l = (a -= b);

    ASM_SUB_FLAGS(8, a, b, res);
    printf("SUB al, 0x%.2X", imm);

    ASM_rexPrint();
    ASM_end();
}

// SUB r/e/ax, imm(16/32)
void ASM_2D(u32 imm) {
    if (oper) { ASM_incIP(4, NULL);
        if (imm > UINT16_MAX) {
            printf("ERROR: Improper immediate size");
            exit(EXIT_FAILURE);
        }
        u16 a = 0; u16 b = 0; u16 res = 0;
        a = regs[0].x; b = imm;
        regs[0].x = (a -= b);

        ASM_SUB_FLAGS(16, a, b, res);
        printf("SUB ax, 0x%.4X", imm);
    } else if (rex.w) { ASM_incIP(6, NULL);
        u64 a = 0; u64 b = 0; u64 res = 0;
        a = regs[0].r; b = imm;
        regs[0].r = (a -= b);

        ASM_SUB_FLAGS(64, a, b, res);
        printf("SUB rax, 0x%.8X", imm);
    } else { ASM_incIP(5, NULL);
        u32 a = 0; u32 b = 0; u32 res = 0;
        a = regs[0].e; b = imm;
        regs[0].e = (a -= b);

        ASM_SUB_FLAGS(32, a, b, res);
        printf("SUB eax, 0x%.8X", imm);
    }

    ASM_rexPrint();
    ASM_end();
}

// NULL
void ASM_2E(void) {
    null = true;
}

// 0x2F invalid

// XOR r/m(8), r(8)
void ASM_30(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_XOR_u8(rm, fdisp, false);

    if (rm->isPtr) {
        ASM_rmPrint("XOR", rm, disp, v_Reg, false);
    } else {
        printf("XOR %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// XOR r/m(16-64), r(16-64)
void ASM_31(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_XOR_u16(rm, fdisp, false); break;
        case R_Bit32: ASM_XOR_u32(rm, fdisp, false); break;
        case R_Bit64: ASM_XOR_u64(rm, fdisp, false); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("XOR", rm, disp, v_Reg, false);
    } else {
        printf("XOR %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// XOR r(8), r/m(8)
void ASM_32(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_XOR_u8(rm, fdisp, true);

    if (rm->isPtr) {
        ASM_rmPrint("XOR", rm, disp, v_Reg, true);
    } else {
        printf("XOR %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// XOR r(16-64), r/m(16-64)
void ASM_33(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_XOR_u16(rm, fdisp, true); break;
        case R_Bit32: ASM_XOR_u32(rm, fdisp, true); break;
        case R_Bit64: ASM_XOR_u64(rm, fdisp, true); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("XOR", rm, disp, v_Reg, true);
    } else {
        printf("XOR %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// XOR al, imm(8)
void ASM_34(u8 imm) {
    ASM_incIP(2, NULL);
    
    u8 a = 0; u8 b = 0; u8 res = 0;
    a = regs[0].l; b = imm;
    regs[0].l = (a ^= b);

    ASM_BIT_FLAGS(8, a, b, res);
    printf("XOR al, 0x%.2X", imm);

    ASM_rexPrint();
    ASM_end();
}

// XOR r/e/ax, imm(16-32)
void ASM_35(u32 imm) {
    if (oper) { ASM_incIP(4, NULL);
        if (imm > UINT16_MAX) {
            printf("ERROR: Improper immediate size");
            exit(EXIT_FAILURE);
        }
        u16 a = 0; u16 b = 0; u16 res = 0;
        a = regs[0].x; b = imm;
        regs[0].x = res = (a ^ b);

        ASM_BIT_FLAGS(16, a, b, res);
        printf("XOR ax, 0x%.4X", imm);
    } else if (rex.w) { ASM_incIP(6, NULL);
        u64 a = 0; u64 b = 0; u64 res = 0;
        a = regs[0].r; b = imm;
        regs[0].r = res = (a ^ b);

        ASM_BIT_FLAGS(64, a, b, res);
        printf("XOR rax, 0x%.8X", imm);
    } else { ASM_incIP(5, NULL);
        u32 a = 0; u32 b = 0; u32 res = 0;
        a = regs[0].e; b = imm;
        regs[0].e = res = (a ^ b);

        ASM_BIT_FLAGS(32, a, b, res);
        printf("XOR eax, 0x%.8X", imm);
    }

    ASM_rexPrint();
    ASM_end();
}

// NULL
void ASM_36(void) {
    null = true;
}

// 0x37 invalid

// CMP r/m(8), r(8)
void ASM_38(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_CMP_u8(rm, fdisp, false);

    if (rm->isPtr) {
        ASM_rmPrint("CMP", rm, disp, v_Reg, false);
    } else {
        printf("CMP %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// CMP r/m(16-64), r(16-64)
void ASM_39(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_CMP_u16(rm, fdisp, false); break;
        case R_Bit32: ASM_CMP_u32(rm, fdisp, false); break;
        case R_Bit64: ASM_CMP_u64(rm, fdisp, false); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("CMP", rm, disp, v_Reg, false);
    } else {
        printf("CMP %s, %s", ASM_getRegName(rm->areg, rm->atype), ASM_getRegName(rm->oreg, rm->otype));
    }

    ASM_rexPrint();
    ASM_end();
}

// CMP r(8), r/m(8)
void ASM_3A(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit8);

    ASM_CMP_u8(rm, fdisp, true);

    if (rm->isPtr) {
        ASM_rmPrint("CMP", rm, disp, v_Reg, true);
    } else {
        printf("CMP %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// CMP r(16-64), r/m(16-64)
void ASM_3B(u8 rm_code, u8 sib, s32 disp) {
    RM *rm; u32 fdisp = ASM_setupOp(rm, rm_code, sib, disp, R_Bit32);

    switch (rm->otype) {
        default: break;
        case R_Bit16: ASM_CMP_u16(rm, fdisp, true); break;
        case R_Bit32: ASM_CMP_u32(rm, fdisp, true); break;
        case R_Bit64: ASM_CMP_u64(rm, fdisp, true); break;
    }

    if (rm->isPtr) {
        ASM_rmPrint("CMP", rm, disp, v_Reg, true);
    } else {
        printf("CMP %s, %s", ASM_getRegName(rm->oreg, rm->otype), ASM_getRegName(rm->areg, rm->atype));
    }

    ASM_rexPrint();
    ASM_end();
}

// CMP al, imm(8)
void ASM_3C(u8 imm) {
    ASM_incIP(2, NULL);
    
    u8 a = 0; u8 b = 0; u8 res = 0;
    a = regs[0].l; b = imm;
    regs[0].l = (a - b);

    ASM_BIT_FLAGS(8, a, b, res);
    printf("CMP al, 0x%.2X", imm);

    ASM_rexPrint();
    ASM_end();
}

// CMP r/e/ax, imm(16-32)
void ASM_3D(u32 imm) {
    if (oper) { ASM_incIP(4, NULL);
        if (imm > UINT16_MAX) {
            printf("ERROR: Improper immediate size");
            exit(EXIT_FAILURE);
        }
        u16 a = 0; u16 b = 0; u16 res = 0;
        a = regs[0].x; b = imm;
        res = (a - b);

        ASM_BIT_FLAGS(16, a, b, res);
        printf("CMP ax, 0x%.4X", imm);
    } else if (rex.w) { ASM_incIP(6, NULL);
        u64 a = 0; u64 b = 0; u64 res = 0;
        a = regs[0].r; b = imm;
        res = (a - b);

        ASM_BIT_FLAGS(64, a, b, res);
        printf("CMP rax, 0x%.8X", imm);
    } else { ASM_incIP(5, NULL);
        u32 a = 0; u32 b = 0; u32 res = 0;
        a = regs[0].e; b = imm;
        res = (a - b);

        ASM_BIT_FLAGS(32, a, b, res);
        printf("CMP eax, 0x%.8X", imm);
    }

    ASM_rexPrint();
    ASM_end();
}

// NULL
void ASM_3E(void) {
    null = true;
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
    STACK(u64, s, regs[4].e);

    *s = IS_B(IS_OP(regs[reg].r, regs[reg].x), IS_OP(regs[reg + 8].r, regs[reg + 8].x));

    printf("PUSH %s", ASM_getRegName(IS_B(reg, reg + 8), IS_OP(R_Bit64, R_Bit16)));
    ASM_rexPrint();
    ASM_end();
}

// POP r(16/64)
void ASM_5H(u8 in) {
    ASM_incIP(IS_B(1, 2), NULL);

    u8 reg = in % 8;
    reg = IS_B(reg, reg + 8);

    STACK(u64, s, regs[4].e);
    regs[4].r += IS_OP(8, 2);

    if (!oper) {
        regs[reg].r = *s;
    } else {
        regs[reg].x = *s;
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
        u32 fdisp = ASM_getDisp(&rm, disp);

        switch (rm.otype) {
            case R_Bit16:
            case R_Bit32: { STACK(u32, s, fdisp); regs[rm.oreg].e = *s; regs[rm.oreg].eh = 0; break; }
            case R_Bit64: { STACK(u32, s, fdisp); regs[rm.oreg].r = *s; break; }
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

// JS rel(8)
bool _ASM_78(s8 val) {
    ASM_incIP(2, NULL);
    
    printf("JS 0x%.2X",(u8) val);
    if (val > 0x7F) {
        printf(" (loop)");
    }
    if (!f.f.sf) { // if not sign, dont jump
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
    
    printf("JNS 0x%.2X",(u8) val);
    if (val > 0x7F) {
        printf(" (loop)");
    }
    if (f.f.sf) { // if sign, dont jump
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
    if (f.f.sf == f.f.of) { // if sign equals overflow (equal or greater), dont jump
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

// JLE rel(8)
bool _ASM_7E(s8 val) {
    ASM_incIP(2, NULL);
    
    printf("JLE 0x%.2X",(u8) val);
    if (val > 0x7F) {
        printf(" (loop)");
    }
    if (f.f.zf == 0 && f.f.sf == f.f.of) { // if zero and sign equals overflow (greater), dont jump
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
void ASM_80(u8 rm_code, u8 sib, s32 disp, u8 val) {
    RM ret = ASM_getRM(rm_code, sib, R_Bit8);
    ASM_incIP(3, &ret);

    if (ASM_80Funcs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: 80 /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_80Funcs[ret.reg](&ret, disp, val);

    ASM_end();
}

#include "subops/x86_81.c"
void ASM_81(u8 rm_code, u8 sib, s32 disp, u32 val) {
    RM ret = ASM_getRM(rm_code, sib, R_Bit32);
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
void ASM_83(u8 rm_code, u8 sib, s32 disp, u8 val) {
    RM ret = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(3, &ret);

    if (ASM_83Funcs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: 83 /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_83Funcs[ret.reg](&ret, disp, val);

    ASM_end();
}

// TEST r/m(8), r(8)
void ASM_84(u8 rm_code, u8 sib, s32 disp) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit8);
    ASM_incIP(2, &rm);
    
    Reg prev = { 0 };
    Reg res = { 0 };

    if (rm.isPtr) {
        u32 fdisp = ASM_getDisp(&rm, disp);

        STACK8(temp, fdisp);
        if (rm.otype == R_Bit8H) {
            prev.l = regs[rm.oreg].h;
        } else {
            prev.l = regs[rm.oreg].l;
        }
        res.l = *temp & prev.l;
        prev.l = *temp;

        ASM_rmPrint("TEST", &rm, disp, v_Reg, false);
    } else {
        if (rm.otype == R_Bit8H) {
            prev.l = regs[rm.oreg].h;
        } else {
            prev.l = regs[rm.oreg].l;
        }
        res.l = regs[rm.areg].l & prev.l;
        prev.l = regs[rm.areg].l;
        
        printf("TEST %s, %s", ASM_getRegName(rm.areg, rm.atype), ASM_getRegName(rm.oreg, rm.otype));
    }
    
    ASM_setFlags(&prev, &res, rm.otype, true);
    ASM_rexPrint();
    ASM_end();
}

// TEST r/m(16-64), r(16-64)
void ASM_85(u8 rm_code, u8 sib, s32 disp) {
    Reg prev = { 0 };
    Reg res = { 0 };

    RM rm = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(2, &rm);
    
    if (rm.isPtr) {
        u32 fdisp = ASM_getDisp(&rm, disp);

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
        u32 fdisp = ASM_getDisp(&rm, disp);

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
        u32 fdisp = ASM_getDisp(&rm, disp);

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
        u32 fdisp = ASM_getDisp(&rm, disp);

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
        u32 fdisp = ASM_getDisp(&rm, disp);

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
        u32 fdisp = ASM_getDisp(&rm, disp);

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

// TEST AL, imm(8)
void ASM_A8(u8 val) {
    ASM_incIP(2, NULL);

    Reg prev = { .l = regs[0].l    };
    Reg res =  { .l = prev.l & val };

    printf("TEST al, 0x%.2X", val);
    
    ASM_setFlags(&prev, &res, R_Bit8, false);
    f.f.af = 0;
    ASM_rexPrint();
    ASM_end();
}

void ASM_AA(void) {
    u64 cnt = regs[1].r;
    u64 *st = &regs[7].r;

    if (sing) {
        ASM_incIP(2, NULL);
        while (cnt-- > 0) {
            STACK8(temp, (*st)++);
            *temp = regs[0].l;
        }

        printf("REP STOSB");
    } else {
        ASM_incIP(1, NULL);
        STACK8(temp, *st++);
        *temp = regs[0].l;

        printf("STOSB");
    }

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
void ASM_C1(u8 rm_code, u8 sib, s32 disp, u8 val) {
    RM ret = ASM_getRM(rm_code, sib, R_Bit32);
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
    ASM_retPrint();
    ASM_end();
}

// 0xC4-C5 invalid

// MOV r/m(8), imm(8)
void ASM_C6(u8 rm_code, u8 sib, s32 disp, u8 val) {
    RM rm = ASM_getRM(rm_code, sib, R_Bit8);
    ASM_incIP(3, &rm);

    if (rm.isPtr) {
        u32 fdisp = ASM_getDisp(&rm, disp);

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
        u32 fdisp = ASM_getDisp(&rm, disp);

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
void ASM_D1(u8 rm_code, u8 sib, s32 disp) {
    RM ret = ASM_getRM(rm_code, sib, R_Bit32);
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
void ASM_F6(u8 rm_code, u8 sib, s32 disp, u8 val) {
    RM ret = ASM_getRM(rm_code, sib, R_Bit8);
    ASM_incIP(3, &ret);

    if (ASM_F6Funcs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: F6 /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_F6Funcs[ret.reg](&ret, disp, val);

    ASM_end();
}

#include "subops/x86_F7.c"
void ASM_F7(u8 rm_code, u8 sib, s32 disp, u32 val) {
    RM ret = ASM_getRM(rm_code, sib, R_Bit32);
    ASM_incIP(3, &ret);

    if (ASM_F7Funcs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: F7 /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_F7Funcs[ret.reg](&ret, disp, val);

    ASM_end();
}

#include "subops/x86_FF.c"
void ASM_FF(u8 rm_code, u8 sib, s32 disp) {
    RM ret = ASM_getRM(rm_code, sib, R_Bit32);

    if (ASM_FFFuncs[ret.reg] == 0) {
        printf("UNIMPLEMENTED OPCODE: FF /%X", ret.reg);
        exit(EXIT_FAILURE);
    }
    ASM_FFFuncs[ret.reg](&ret, disp);

    ASM_end();
}