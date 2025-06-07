#include "alu.h"

#define NIBBLE(t, r) (t == R_Bit8) ? r.l : r.h

void ASM_ADD_u8(RM *rm, u32 disp, bool flip) {
    u8 *a = NULL;
    u8 b = 0;
    u8 res = 0;

    if (rm->isPtr) {
        STACK(u8, s, disp);
        if (!flip) { a = s;  b = NIBBLE(rm->otype, regs[rm->oreg]);  }
        else       { b = *s; a = NIBBLE(rm->otype, &regs[rm->oreg]); }
        res = (*a += b);
    } else {
        if (!flip) { a = NIBBLE(rm->atype, &regs[rm->areg]); b = NIBBLE(rm->otype, regs[rm->oreg]); }
        else       { b = NIBBLE(rm->atype, regs[rm->areg]); a = NIBBLE(rm->otype, &regs[rm->oreg]); }
        res = (*a += b);
    }

    ASM_ADD_FLAGS(8, *a, b, res);
}

void ASM_ADD_u16(RM *rm, u32 disp, bool flip) {
    u16 *a = NULL;
    u16 b = 0;
    u16 res = 0;

    if (rm->isPtr) {
        STACK(u16, s, disp);
        if (!flip) { a = s;  b = regs[rm->oreg].x;  }
        else       { b = *s; a = &regs[rm->oreg].x; }
        res = (*a += b);
    } else {
        if (!flip) { a = &regs[rm->areg].x; b = regs[rm->oreg].x; }
        else       { b = regs[rm->areg].x; a = &regs[rm->oreg].x; }
        res = (*a += b);
    }

    ASM_ADD_FLAGS(16, *a, b, res);
}

void ASM_ADD_u32(RM *rm, u32 disp, bool flip) {
    u32 *a = NULL;
    u32 b = 0;
    u32 res = 0;

    if (rm->isPtr) {
        STACK(u32, s, disp);
        if (!flip) { a = s;  b = regs[rm->oreg].e;  }
        else       { b = *s; a = &regs[rm->oreg].e; }
        res = (*a += b);
    } else {
        if (!flip) { a = &regs[rm->areg].e; b = regs[rm->oreg].e; }
        else       { b = regs[rm->areg].e; a = &regs[rm->oreg].e; }
        res = (*a += b);
    }

    ASM_ADD_FLAGS(32, *a, b, res);
}

void ASM_ADD_u64(RM *rm, u32 disp, bool flip) {
    u64 *a = NULL;
    u64 b = 0;
    u64 res = 0;

    if (rm->isPtr) {
        STACK(u64, s, disp);
        if (!flip) { a = s;  b = regs[rm->oreg].r;  }
        else       { b = *s; a = &regs[rm->oreg].r; }
        res = (*a += b);
    } else {
        if (!flip) { a = &regs[rm->areg].r; b = regs[rm->oreg].r; }
        else       { b = regs[rm->areg].r; a = &regs[rm->oreg].r; }
        res = (*a += b);
    }

    ASM_ADD_FLAGS(64, *a, b, res);
}

void ASM_OR_u8(RM *rm, u32 disp, bool flip) {
    u8 *a = NULL;
    u8 b = 0;
    u8 res = 0;

    if (rm->isPtr) {
        STACK(u8, s, disp);
        if (!flip) { a = s;  b = NIBBLE(rm->otype, regs[rm->oreg]);  }
        else       { b = *s; a = NIBBLE(rm->otype, &regs[rm->oreg]); }
        res = (*a |= b);
    } else {
        if (!flip) { a = NIBBLE(rm->atype, &regs[rm->areg]); b = NIBBLE(rm->otype, regs[rm->oreg]); }
        else       { b = NIBBLE(rm->atype, regs[rm->areg]); a = NIBBLE(rm->otype, &regs[rm->oreg]); }
        res = (*a |= b);
    }

    ASM_BIT_FLAGS(8, *a, b, res);
}

void ASM_OR_u16(RM *rm, u32 disp, bool flip) {
    u16 *a = NULL;
    u16 b = 0;
    u16 res = 0;

    if (rm->isPtr) {
        STACK(u16, s, disp);
        if (!flip) { a = s;  b = regs[rm->oreg].x;  }
        else       { b = *s; a = &regs[rm->oreg].x; }
        res = (*a |= b);
    } else {
        if (!flip) { a = &regs[rm->areg].x; b = regs[rm->oreg].x; }
        else       { b = regs[rm->areg].x; a = &regs[rm->oreg].x; }
        res = (*a |= b);
    }

    ASM_BIT_FLAGS(16, *a, b, res);
}

void ASM_OR_u32(RM *rm, u32 disp, bool flip) {
    u32 *a = NULL;
    u32 b = 0;
    u32 res = 0;

    if (rm->isPtr) {
        STACK(u32, s, disp);
        if (!flip) { a = s;  b = regs[rm->oreg].e;  }
        else       { b = *s; a = &regs[rm->oreg].e; }
        res = (*a |= b);
    } else {
        if (!flip) { a = &regs[rm->areg].e; b = regs[rm->oreg].e; }
        else       { b = regs[rm->areg].e; a = &regs[rm->oreg].e; }
        res = (*a |= b);
    }

    ASM_BIT_FLAGS(32, *a, b, res);
}

void ASM_OR_u64(RM *rm, u32 disp, bool flip) {
    u64 *a = NULL;
    u64 b = 0;
    u64 res = 0;

    if (rm->isPtr) {
        STACK(u64, s, disp);
        if (!flip) { a = s;  b = regs[rm->oreg].r;  }
        else       { b = *s; a = &regs[rm->oreg].r; }
        res = (*a |= b);
    } else {
        if (!flip) { a = &regs[rm->areg].r; b = regs[rm->oreg].r; }
        else       { b = regs[rm->areg].r; a = &regs[rm->oreg].r; }
        res = (*a |= b);
    }

    ASM_BIT_FLAGS(64, *a, b, res);
}

void ASM_ADC_u8(RM *rm, u32 disp, bool flip) {
    u8 *a = NULL;
    u8 b = 0;
    u8 res = 0;

    if (rm->isPtr) {
        STACK(u8, s, disp);
        if (!flip) { a = s;  b = NIBBLE(rm->otype, regs[rm->oreg]);  }
        else       { b = *s; a = NIBBLE(rm->otype, &regs[rm->oreg]); }
        res = (*a += b + f.f.cf);
    } else {
        if (!flip) { a = NIBBLE(rm->atype, &regs[rm->areg]); b = NIBBLE(rm->otype, regs[rm->oreg]); }
        else       { b = NIBBLE(rm->atype, regs[rm->areg]); a = NIBBLE(rm->otype, &regs[rm->oreg]); }
        res = (*a += b + f.f.cf);
    }

    ASM_ADD_FLAGS(8, *a, b, res);
}

void ASM_ADC_u16(RM *rm, u32 disp, bool flip) {
    u16 *a = NULL;
    u16 b = 0;
    u16 res = 0;

    if (rm->isPtr) {
        STACK(u16, s, disp);
        if (!flip) { a = s;  b =  regs[rm->oreg].x; }
        else       { b = *s; a = &regs[rm->oreg].x; }
        res = (*a += b + f.f.cf);
    } else {
        if (!flip) { a = &regs[rm->areg].x; b = regs[rm->oreg].x; }
        else       { b = regs[rm->areg].x; a = &regs[rm->oreg].x; }
        res = (*a += b + f.f.cf);
    }

    ASM_ADD_FLAGS(16, *a, b, res);
}

void ASM_ADC_u32(RM *rm, u32 disp, bool flip) {
    u32 *a = NULL;
    u32 b = 0;
    u32 res = 0;

    if (rm->isPtr) {
        STACK(u32, s, disp);
        if (!flip) { a = s;  b =  regs[rm->oreg].e; }
        else       { b = *s; a = &regs[rm->oreg].e; }
        res = (*a += b + f.f.cf);
    } else {
        if (!flip) { a = &regs[rm->areg].e; b = regs[rm->oreg].e; }
        else       { b = regs[rm->areg].e; a = &regs[rm->oreg].e; }
        res = (*a += b + f.f.cf);
    }

    ASM_ADD_FLAGS(32, *a, b, res);
}

void ASM_ADC_u64(RM *rm, u32 disp, bool flip) {
    u64 *a = NULL;
    u64 b = 0;
    u64 res = 0;

    if (rm->isPtr) {
        STACK(u64, s, disp);
        if (!flip) { a = s;  b =  regs[rm->oreg].r; }
        else       { b = *s; a = &regs[rm->oreg].r; }
        res = (*a += b + f.f.cf);
    } else {
        if (!flip) { a = &regs[rm->areg].r; b = regs[rm->oreg].r; }
        else       { b = regs[rm->areg].r; a = &regs[rm->oreg].r; }
        res = (*a += b + f.f.cf);
    }

    ASM_ADD_FLAGS(64, *a, b, res);
}

void ASM_SBB_u8(RM *rm, u32 disp, bool flip) {
    u8 *a = NULL;
    u8 b = 0;
    u8 res = 0;

    if (rm->isPtr) {
        STACK(u8, s, disp);
        if (!flip) { a = s;  b = NIBBLE(rm->otype,  regs[rm->oreg]); }
        else       { b = *s; a = NIBBLE(rm->otype, &regs[rm->oreg]); }
        res = (*a -= b + f.f.cf);
    } else {
        if (!flip) { a = NIBBLE(rm->atype, &regs[rm->areg]); b = NIBBLE(rm->otype, regs[rm->oreg]); }
        else       { b = NIBBLE(rm->atype, regs[rm->areg]); a = NIBBLE(rm->otype, &regs[rm->oreg]); }
        res = (*a -= b + f.f.cf);
    }

    ASM_SUB_FLAGS(8, *a, b, res);
}

void ASM_SBB_u16(RM *rm, u32 disp, bool flip) {
    u16 *a = NULL;
    u16 b = 0;
    u16 res = 0;

    if (rm->isPtr) {
        STACK(u16, s, disp);
        if (!flip) { a = s;  b =  regs[rm->oreg].x; }
        else       { b = *s; a = &regs[rm->oreg].x; }
        res = (*a -= b + f.f.cf);
    } else {
        if (!flip) { a = &regs[rm->areg].x; b = regs[rm->oreg].x; }
        else       { b = regs[rm->areg].x; a = &regs[rm->oreg].x; }
        res = (*a -= b + f.f.cf);
    }

    ASM_SUB_FLAGS(16, *a, b, res);
}

void ASM_SBB_u32(RM *rm, u32 disp, bool flip) {
    u32 *a = NULL;
    u32 b = 0;
    u32 res = 0;

    if (rm->isPtr) {
        STACK(u32, s, disp);
        if (!flip) { a = s;  b =  regs[rm->oreg].e; }
        else       { b = *s; a = &regs[rm->oreg].e; }
        res = (*a -= b + f.f.cf);
    } else {
        if (!flip) { a = &regs[rm->areg].e; b = regs[rm->oreg].e; }
        else       { b = regs[rm->areg].e; a = &regs[rm->oreg].e; }
        res = (*a -= b + f.f.cf);
    }

    ASM_SUB_FLAGS(32, *a, b, res);
}

void ASM_SBB_u64(RM *rm, u32 disp, bool flip) {
    u64 *a = NULL;
    u64 b = 0;
    u64 res = 0;

    if (rm->isPtr) {
        STACK(u64, s, disp);
        if (!flip) { a = s;  b =  regs[rm->oreg].r; }
        else       { b = *s; a = &regs[rm->oreg].r; }
        res = (*a -= b + f.f.cf);
    } else {
        if (!flip) { a = &regs[rm->areg].r; b = regs[rm->oreg].r; }
        else       { b = regs[rm->areg].r; a = &regs[rm->oreg].r; }
        res = (*a -= b + f.f.cf);
    }

    ASM_SUB_FLAGS(64, *a, b, res);
}

void ASM_AND_u8(RM *rm, u32 disp, bool flip) {
    u8 *a = NULL;
    u8 b = 0;
    u8 res = 0;

    if (rm->isPtr) {
        STACK(u8, s, disp);
        if (!flip) { a = s;  b = NIBBLE(rm->otype, regs[rm->oreg]);  }
        else       { b = *s; a = NIBBLE(rm->otype, &regs[rm->oreg]); }
        res = (*a &= b);
    } else {
        if (!flip) { a = NIBBLE(rm->atype, &regs[rm->areg]); b = NIBBLE(rm->otype, regs[rm->oreg]); }
        else       { b = NIBBLE(rm->atype, regs[rm->areg]); a = NIBBLE(rm->otype, &regs[rm->oreg]); }
        res = (*a &= b);
    }

    ASM_BIT_FLAGS(8, *a, b, res);
}

void ASM_AND_u16(RM *rm, u32 disp, bool flip) {
    u16 *a = NULL;
    u16 b = 0;
    u16 res = 0;

    if (rm->isPtr) {
        STACK(u16, s, disp);
        if (!flip) { a = s;  b = regs[rm->oreg].x;  }
        else       { b = *s; a = &regs[rm->oreg].x; }
        res = (*a &= b);
    } else {
        if (!flip) { a = &regs[rm->areg].x; b = regs[rm->oreg].x; }
        else       { b = regs[rm->areg].x; a = &regs[rm->oreg].x; }
        res = (*a &= b);
    }

    ASM_BIT_FLAGS(16, *a, b, res);
}

void ASM_AND_u32(RM *rm, u32 disp, bool flip) {
    u32 *a = NULL;
    u32 b = 0;
    u32 res = 0;

    if (rm->isPtr) {
        STACK(u32, s, disp);
        if (!flip) { a = s;  b = regs[rm->oreg].e;  }
        else       { b = *s; a = &regs[rm->oreg].e; }
        res = (*a &= b);
    } else {
        if (!flip) { a = &regs[rm->areg].e; b = regs[rm->oreg].e; }
        else       { b = regs[rm->areg].e; a = &regs[rm->oreg].e; }
        res = (*a &= b);
    }

    ASM_BIT_FLAGS(32, *a, b, res);
}

void ASM_AND_u64(RM *rm, u32 disp, bool flip) {
    u64 *a = NULL;
    u64 b = 0;
    u64 res = 0;

    if (rm->isPtr) {
        STACK(u64, s, disp);
        if (!flip) { a = s;  b = regs[rm->oreg].r;  }
        else       { b = *s; a = &regs[rm->oreg].r; }
        res = (*a &= b);
    } else {
        if (!flip) { a = &regs[rm->areg].r; b = regs[rm->oreg].r; }
        else       { b = regs[rm->areg].r; a = &regs[rm->oreg].r; }
        res = (*a &= b);
    }

    ASM_BIT_FLAGS(64, *a, b, res);
}