#include "data.h"

// order: a, c, d, b, sp, bp, si, di, 8-15, ip
Reg regs[17] = { 0 };
// order: ss, cs, ds, es, fs, gs
u16 sregs[6] = { 0 };
FloatReg fregs[16] = { 0 };
MMXReg xregs[16] = { 0 };
u64 stregs[8] = { 0 };
Flags f = {0, .f.on = 1 };
bool oper = false; // operand override prefix
bool addr = false; // address override prefix
bool doub = false; // double  override prefix
bool sing = false; // float   override prefix
bool fs   = false; // fs      override prefix
bool gs   = false; // gs      override prefix
bool lock = false; // lock prefix
REXPrefix rex;

u8 *stack;

void ASM_init() {
    stack = (u8*)malloc(0x100000000 * sizeof(u8));

    for (int i = 0; i < 16; i++) {
        if (i == 4 || i == 5) {
            regs[i].r = INT32_MIN;
        } else {
            regs[i].r = 0;
        }
    }
    for (int i = 0; i < 16; i++) {
        fregs[i].yd = _mm256_set1_pd(0.0);
    }
}

void ASM_getSIB(RM *rm, u8 sib) {
    u8 ss = (sib >> 6) % 4;
    u8 ix = (sib >> 3) % 8;
    u8 bs = (sib >> 0) % 8;

    rm->isSib = true;

    rm->breg = IS_B(bs, bs + 8);
    if (bs == 5 && rm->mod == 0) {
        rm->breg = REG_NULL;
        rm->disp = 4;
    }

    rm->btype = IS_AD(R_Bit64, R_Bit32);
    rm->bmul = 1 << ss;

    rm->areg = IS_X(ix, ix + 8);
    if (ix == 4 && rex.x == 0) rm->areg = REG_NULL;

    rm->atype = IS_AD(R_Bit64, R_Bit32);

    if (rm->areg == REG_NULL && rm->breg == REG_NULL) {
        rm->areg = IS_FS(IS_GS(1, 5), 4);
        rm->atype = R_Seg;
    }
}

RM ASM_getRM(u8 rm, u8 sib, RegType type) {
    RM ret = {0};
    u8 reg = REG_NULL;

    ret.mod = (rm >> 6) % 4;
    ret.reg = (rm >> 3) % 8;
    ret.rm  = (rm >> 0) % 8;
    ret.isPtr = true;

    ret.breg = REG_NULL;

    ret.oreg = IS_R(ret.reg, ret.reg + 8);
    switch (type) {
        default:
            ret.otype = type;
            break;

        case R_Bit8:
            if (ret.reg >= 4 && ret.reg <= 8) {
                ret.otype = IS_REX(R_Bit8H, R_Bit8);
            }
            break;
        
        case R_Bit16: case R_Bit32: case R_Bit64:
            ret.otype = IS_OP(IS_W(R_Bit32, R_Bit64), R_Bit16);
            break;

        case R_MMX: case R_Float128:
            ret.otype = IS_DUP(R_MMX, R_Float128);
            break;
    }

    ret.areg = IS_B(ret.rm, ret.rm + 8);
    ret.atype = IS_AD(R_Bit64, R_Bit32);
    if (ret.rm == 4 && ret.mod != 3) {
        ASM_getSIB(&ret, sib);
    } else if (ret.rm == 5 && ret.mod == 0) {
        ret.areg = 16;
        ret.disp = 4;
    }
    
    switch (ret.mod) {
        case 0: if (ret.rm == 5) ret.disp = 4; break;
        case 1: ret.disp = 1; break;
        case 2: ret.disp = 4; break;
        case 3:
            ret.atype = ret.otype;
            ret.isPtr = false;
            if (ret.areg >= 4 && ret.areg < 8 && ret.otype == R_Bit8) {
                ret.areg -= 4;
                ret.atype = R_Bit8H;
            }
            break;
    }

    ret.ptrtype = ret.otype;
    ret.valType = REG_NULL;

    return ret;
}

ASM_codeFunc ASM_getFunc(u64 ip) {
    STACK64(temp, ip);
    ASM_codeFunc func = NULL;
    
    func = (ASM_codeFunc)*temp;
    if ((u64)func < 0x100000) {
        STACK64(temp2, *temp);
        func = (ASM_codeFunc)*temp2;
    }

    if (func == NULL) {
        printf("Function not found: %.8llX\n", ip);
        // ASM_regPrint();
        exit(EXIT_FAILURE);
    } else {
        printf("\n%.8llX:\n", ip);
    }

    return func;
}

ASM_codeFunc ASM_getCurrFunc(void) {
    return ASM_getFunc(regs[16].e);
}

u64 ASM_getReg(u8 index, RegType type) {
    u64 reg = 0;

    switch (type) {
        case R_Bit8:     reg =  regs[index].l; break;
        case R_Bit8H:    reg =  regs[index].h; break;
        case R_Bit16:    reg =  regs[index].x; break;
        case R_Bit32:    reg =  regs[index].e; break;
        case R_Bit64:    reg =  regs[index].e; break;
        case R_Seg:      reg =  regs[16].e;    break;
        default: break;
    }

    return reg;
}

void ASM_incIP(u32 num, RM *rm) {
    if (rm == NULL) {
        regs[16].e += num;
        return;
    }
    regs[16].e += num + rm->disp;
    if (rm->isSib)       regs[16].e += 1;
    if (rex.enable != 0) regs[16].e += 1;
    if (addr != 0)       regs[16].e += 1;
    if (oper != 0)       regs[16].e += 1;
    if (fs != 0)         regs[16].e += 1;
    if (gs != 0)         regs[16].e += 1;
    if (lock != 0)       regs[16].e += 1;
}

bool ASM_getParity(u8 num) {
    num ^= num >> 4;
    num ^= num >> 2;
    num ^= num >> 1;
    return ~(num & 1) == 1;
}

void ASM_setFlags(Reg *prev, Reg *res, RegType type, bool borrow) {
    f.f.pf = ASM_getParity(res->l);
    f.f.af = (((prev->l & 0x18) ^ (res->l & 0x18)) == 0x18);
    switch (type) {
        case R_Bit8: case R_Bit8H:
            if (borrow) f.f.cf = (res->l > prev->l);
            else        f.f.cf = (res->l < prev->l);
            f.f.zf = (res->l == 0);
            f.f.sf = ((s8)res->l < 0);
            f.f.of = ((res->l & 0x8000) != (prev->l & 0x8000));
            break;
        case R_Bit16:
            if (borrow) f.f.cf = (res->x > prev->x);
            else        f.f.cf = (res->x < prev->x);
            f.f.zf = (res->x == 0);
            f.f.sf = ((s16)res->x < 0);
            f.f.of = ((res->x & 0x8000) != (prev->x & 0x8000));
            break;
        case R_Bit32:
        if (borrow) f.f.cf = (res->e > prev->e);
        else        f.f.cf = (res->e < prev->e);
            f.f.zf = (res->e == 0);
            f.f.sf = ((s32)res->e < 0);
            f.f.of = ((res->e & 0x80000000) != (prev->e & 0x80000000));
            break;
        case R_Bit64:
            if (borrow) f.f.cf = (res->r > prev->r);
            else        f.f.cf = (res->r < prev->r);
            f.f.zf = (res->r == 0);
            f.f.sf = ((s64)res->r < 0);
            f.f.of = ((res->e & 0x8000000000000000) != (prev->e & 0x8000000000000000));
            break;
        default: break;
    }
}

char *ASM_getRegName(u8 index, RegType type) {
    char *buf = (char *)malloc(6 * sizeof(char));
    if (buf == NULL) {
        printf("ERROR: getRegName buffer is NULL");
        exit(EXIT_FAILURE);
        return NULL;
    }

    memset(buf, 0, 6);

    if (IS_INT(type)) {
        u8 i = 0;
        switch (index) {
            case 0: buf[i++] = 'a';               break;
            case 1: buf[i++] = 'c';               break;
            case 2: buf[i++] = 'd';               break;
            case 3: buf[i++] = 'b';               break;
            case 4: buf[i++] = 's'; buf[i++] = 'p'; break;
            case 5: buf[i++] = 'b'; buf[i++] = 'p'; break;
            case 6: buf[i++] = 's'; buf[i++] = 'i'; break;
            case 7: buf[i++] = 'd'; buf[i++] = 'i'; break;
            case 16: buf[i++] = 'i'; buf[i++] = 'p'; break;

            default:
                buf[i++] = 'r';
                if (index >= 10) {
                    buf[i++] = '1';
                    buf[i++] = (index % 10) + '0';
                } else {
                    buf[i++] = index + '0';
                }
        }

        if (type != R_Bit8 && type != R_Bit8H && type != R_Bit16 && (index < 8 || index == 16)) {
            buf[3] = buf[2];
            buf[2] = buf[1];
            buf[1] = buf[0];
            i++;
        } else if (type == R_Bit8 && index < 8) {
            buf[i] = 'l';
        } else if (type == R_Bit8) {
            buf[i] = 'b';
        } else if (type == R_Bit8H) {
            buf[i] = 'h';
        }

        switch (type) {
            default: break;
            case R_Bit16:
                if (index < 4) buf[i] = 'x';
                else if (index < 8 || index == 16);
                else           buf[i] = 'w';
                break;
            
            case R_Bit32:
                if (index < 4) { buf[0] = 'e'; buf[i] = 'x'; }
                else if (index < 8 || index == 16) buf[0] = 'e';
                else             buf[i] = 'd';
                break;
            
            case R_Bit64:
                if (index < 4) { buf[0] = 'r'; buf[i] = 'x'; }
                else if (index < 8 || index == 16) buf[0] = 'r';
                break;
        }
    } else if (IS_FLOAT(type)) {
        buf[0] = 'x'; buf[1] = 'm'; buf[2] = 'm';
        if (index >= 10) {
            buf[3] = '1';
            buf[4] = (index % 10) + '0';
        } else {
            buf[3] = index + '0';
        }
    } else if (type == R_MMX) {
        buf[0] = 'm'; buf[1] = 'm';
        if (index >= 10) {
            buf[2] = '1';
            buf[3] = (index % 10) + '0';
        } else {
            buf[2] = index + '0';
        }
    } else if (type == R_ST) {
        buf[0] = 's'; buf[1] = 't'; buf[2] = '(';
        if (index >= 10) {
            buf[3] = '1';
            buf[4] = (index % 10) + '0';
        } else {
            buf[3] = index + '0';
        }
        buf[5] = ')';
    } else if (type == R_Seg) {
        switch (index) {
            case 0: buf[0] = 's'; break;
            case 1: buf[0] = 'c'; break;
            case 2: buf[0] = 'd'; break;
            case 3: buf[0] = 'e'; break;
            case 4: buf[0] = 'f'; break;
            case 5: buf[0] = 'g'; break;
        }
        buf[1] = 's';
    } else {
        buf[0] = 'E'; buf[1] = 'R'; buf[2] = 'R';
    }

    return buf;
}

const char *ASM_ptrName(RegType bits) {
    switch (bits) {
        case R_Bit8:
        case R_Bit8H: return "BYTE PTR";
        case R_Bit16: return "WORD PTR";
        case R_Bit32: return "DWORD PTR";
        case R_Bit64: return "QWORD PTR";
        case R_MMX: return "MMXWORD PTR";
        case R_Float128: return "XMMWORD PTR";
        default: return "UNIMPL PTR";
    }
}

void ASM_rexPrint(void) {
    printf("\n  |IP:%.8X|A:%.16llX C:%.16llX  D:%.16llX  B:%.16llX|XMM0L:%.8X%.8X|SP:%.8X :", regs[16].e, regs[0].r, regs[1].r, regs[2].r, regs[3].r, fregs[0].u[0], fregs[0].u[1], regs[4].e);
    
    int sp = regs[4].e;
    if (sp < 0x80000000)
        printf(" %.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X\n",
                stack[sp], stack[sp + 1], stack[sp + 2], stack[sp + 3], stack[sp + 4], stack[sp + 5], stack[sp + 6], stack[sp + 7]);
    
    printf("  |BP:%.8X|8:%.16llX 9:%.16llX 10:%.16llX 11:%.16llX|XMM0H:%.8X%.8X| F:%d%d%d%d%d%d%d%d :", regs[5].e, regs[8].r, regs[9].r, regs[10].r, regs[11].r, fregs[1].u[0], fregs[1].u[1], f.f.of, f.f.df, f.f.iF, f.f.sf, f.f.zf, f.f.af, f.f.pf, f.f.cf);

    sp = regs[4].e + 8;
    if (sp < 0x80000000)
        printf(" %.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X\n",
                stack[sp], stack[sp + 1], stack[sp + 2], stack[sp + 3], stack[sp + 4], stack[sp + 5], stack[sp + 6], stack[sp + 7]);
}

bool ASM_sign = false;
char ASM_S(void) {
    if (ASM_sign) {
        return '-';
    } else {
        return '+';
    }
}

void ASM_rmPrint(const char *name, RM *rm, s32 disp, opVal val, bool flip) {
    char buf[256] = {0};
    s8 sDisp = disp;
    if (disp < 0 && rm->disp == 4) {
        disp = -disp;
        ASM_sign = true;
    } else if (sDisp < 0 && rm->disp == 1) {
        sDisp = -sDisp;
        ASM_sign = true;
    }


    if (lock) {
        sprintf_s(buf, 256, "LOCK ");
    }
    sprintf_s(buf, 256, "%s%s ", buf, name);

    if (flip) {
        sprintf_s(buf, 256, "%s%s, ", buf, ASM_getRegName(rm->oreg, rm->otype));
    }

    if (rm->atype == R_Seg) {
        sprintf_s(buf, 256, "%s%s:[rip", buf, ASM_getRegName(rm->areg, rm->atype));
        
        if      (rm->disp == 1) sprintf_s(buf, 256, "%s %c %#.2X]", buf, ASM_S(), sDisp);
        else if (rm->disp == 4) sprintf_s(buf, 256, "%s %c %#.8X]", buf, ASM_S(), disp);
        
        printf_s(buf);
        return;
    } else if (rm->isSib && rm->breg != REG_NULL && rm->areg != REG_NULL) {
        sprintf_s(buf, 256, "%s%s [%s+%s*%d", buf, ASM_ptrName(rm->ptrtype), ASM_getRegName(rm->breg, rm->btype), ASM_getRegName(rm->areg, rm->atype), rm->bmul);

        if      (rm->disp == 1) sprintf_s(buf, 256, "%s %c %#.2X", buf, ASM_S(), sDisp);
        else if (rm->disp == 4) sprintf_s(buf, 256, "%s %c %#.8X", buf, ASM_S(), disp);
    } else if (rm->isSib && rm->breg == REG_NULL && rm->areg == REG_NULL) {
        sprintf_s(buf, 256, "%s%s [", buf, ASM_ptrName(rm->ptrtype));

        if      (rm->disp == 1) sprintf_s(buf, 256, "%s%c%#.2X", buf, ASM_S(), sDisp);
        else if (rm->disp == 4) sprintf_s(buf, 256, "%s%c%#.8X", buf, ASM_S(), disp);
    } else if (rm->isSib && rm->breg == REG_NULL) {
        sprintf_s(buf, 256, "%s%s [%s*%d", buf, ASM_ptrName(rm->ptrtype), ASM_getRegName(rm->areg, rm->atype), rm->bmul);

        if      (rm->disp == 1) sprintf_s(buf, 256, "%s %c %#.2X", buf, ASM_S(), sDisp);
        else if (rm->disp == 4) sprintf_s(buf, 256, "%s %c %#.8X", buf, ASM_S(), disp);
    } else if (rm->isSib && rm->areg == REG_NULL) {
        sprintf_s(buf, 256, "%s%s [%s", buf, ASM_ptrName(rm->ptrtype), ASM_getRegName(rm->breg, rm->btype));
        if      (rm->disp == 1) sprintf_s(buf, 256, "%s %c %#.2X", buf, ASM_S(), sDisp);
        else if (rm->disp == 4) sprintf_s(buf, 256, "%s %c %#.8X", buf, ASM_S(), disp);
    } else if (rm->areg == 16) {
        sprintf_s(buf, 256, "%s%s [", buf, ASM_ptrName(rm->ptrtype), ASM_getRegName(rm->areg, rm->atype));

        if (ASM_sign) { sDisp = -sDisp; disp = -disp; }
        if      (rm->disp == 1) sprintf_s(buf, 256, "%s%#.8X", buf, sDisp + regs[16].e);
        else if (rm->disp == 4) sprintf_s(buf, 256, "%s%#.8X", buf, disp + regs[16].e);
    } else {
        sprintf_s(buf, 256, "%s%s [%s", buf, ASM_ptrName(rm->ptrtype), ASM_getRegName(rm->areg, rm->atype));
        if      (rm->disp == 1) sprintf_s(buf, 256, "%s %c %#.2X", buf, ASM_S(), sDisp);
        else if (rm->disp == 4) sprintf_s(buf, 256, "%s %c %#.8X", buf, ASM_S(), disp);
    }
    if (val == v_Reg && !flip) {
        sprintf_s(buf, 256, "%s], %s", buf, ASM_getRegName(rm->oreg, rm->otype));
    } else if (val == v_Val && !flip) {
        switch (rm->valType) {
            case R_Bit8:  sprintf_s(buf, 256, "%s], 0x%.2X",  buf, rm->val); break;
            case R_Bit16: sprintf_s(buf, 256, "%s], 0x%.4X",  buf, rm->val); break;
            case R_Bit32: sprintf_s(buf, 256, "%s], 0x%.8X",  buf, rm->val); break;
            case R_Bit64: sprintf_s(buf, 256, "%s], 0x%.16llX", buf, rm->val); break;
            default: sprintf_s(buf, 256, "%s], %X", buf, rm->val); break;
        }
    } else {
        sprintf_s(buf, 256, "%s]", buf);
    }
    printf_s(buf);
    ASM_sign = false;
}

void ASM_regPrint(void) {
    char buf[0x1000] = {0};

    for (int i = 0; i < 16; i++) {
        if (i == 8 || i == 9)
            sprintf(buf, "%s%s:  %.16llX  ", buf, ASM_getRegName(i, R_Bit64), regs[i].r);
        else
            sprintf(buf, "%s%s: %.16llX  ", buf, ASM_getRegName(i, R_Bit64), regs[i].r);
        if (i % 4 == 3) {
            sprintf(buf, "%s\n", buf);
        }
    }
    sprintf(buf, "%s\nrip: %.16llX  eflags: %.8X\n", buf, regs[16].r, f.i);
    for (int i = 0; i < 16; i++) {
        if (i < 10)
            sprintf(buf, "%s%s:  %.16llX%.16llX  ", buf, ASM_getRegName(i, R_Float128), fregs[i].ul[0], fregs[i].ul[1]);
        else
            sprintf(buf, "%s%s: %.16llX%.16llX  ", buf, ASM_getRegName(i, R_Float128), fregs[i].ul[0], fregs[i].ul[1]);
        if (i % 4 == 3) {
            sprintf(buf, "%s\n", buf);
        }
    }

    printf("-----\n%s", buf);
}

void ASM_end(void) {
    rex.enable = 0;
    rex.w = 0;
    rex.r = 0;
    rex.x = 0;
    rex.b = 0;

    oper = 0;
    addr = 0;
    doub = 0;
    sing = 0;
    fs = 0;
    gs = 0;
    lock = 0;
}

void ASM_exit(void) {
    ASM_regPrint();
}