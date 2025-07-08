#ifndef EBA6F15E_AF03_4213_BEF3_CE3839FA5FFA
#define EBA6F15E_AF03_4213_BEF3_CE3839FA5FFA

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN64
#include <windows.h>
#include <intrin.h>
#endif

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef struct s128 {
    u64 low;
    s64 high;
} s128;
typedef struct u128 {
    u64 low;
    u64 high;
} u128;
// typedef struct s256 {
//     u128 low;
//     s128 high;
// } s256;
// typedef struct u256 {
//     u128 low;
//     u128 high;
// } u256;
// typedef struct s512 {
//     u256 low;
//     s256 high;
// } s512;
// typedef struct u512 {
//     u256 low;
//     u256 high;
// } u512;


typedef float f32;
typedef double f64;
typedef long double f128;
// typedef struct f256 {
//     u8 sign : 1;
//     u32 expo : 19;
//     u256 mant;
// } f256;

typedef union _reg {
    u64 r;
    struct {
        u32 e;
        u32 eh;
    };
    u16 x;
    struct {
        u8 l;
        u8 h;
    };
} Reg;

typedef union _xmm_reg {
    u128 xi;
    f128 xd;
    f64 d[2];
    u64 ul[2];
    f32 f[4];
    u32 u[4];
} XMMReg;

// typedef union _ymm_reg {
//     u256 yi;
//     f256 yd;
//     u128 xi[2];
//     f128 xd[2];
//     f64 d[4];
//     u64 ul[4];
//     f32 f[8];
//     u32 u[8];
// } YMMReg;

typedef union _mmx_reg {
    u64 u;
} MMXReg;

typedef struct _flag {
// byte 0
    u32 sf : 1;
    u32 zf : 1;
    u32 _reserved2 : 1;
    u32 af : 1;
    u32 _reserved1 : 1;
    u32 pf : 1;
    u32 on : 1;
    u32 cf : 1;
// byte 1
    u32 md : 1;
    u32 nt : 1;
    u32 io : 2;
    u32 of : 1;
    u32 df : 1;
    u32 iF : 1;
    u32 tf : 1;
// byte 2
    u32 _reserved4 : 2;
    u32 id : 1;
    u32 vip : 1;
    u32 vif : 1;
    u32 ac : 1;
    u32 vm : 1;
    u32 rf : 1;
// byte 3
    u32 ai : 1;
    u32 ae : 1;
    u32 _reserved3 : 6;
} _Flags;

typedef union flag {
    _Flags f;
    u32 i;
} Flags;

typedef struct _rex_prefix {
    u8 b : 1;
    u8 x : 1;
    u8 r : 1;
    u8 w : 1;
    u8 enable : 4;
} REXPrefix;

typedef enum _reg_type {
    R_Bit8,
    R_Bit8H,
    R_Bit16,
    R_Bit32,
    R_Bit64,
    R_Float32,
    R_Float64,
    R_Float128,
    R_Float256,
    R_Float512,
    R_MMX,
    R_ST,
    R_Seg,
} RegType;

typedef enum opVal {
    v_None,
    v_Val,
    v_Reg
} opVal;

typedef struct _rm {
    u8 disp;
    u8 areg; // addr reg
    u8 oreg; // operand reg
    u8 breg; // sib reg
    RegType atype;
    RegType otype;
    RegType btype;
    RegType ptrtype; // otype unless sign ext

    u32 val; // if oreg isnt used
    RegType valtype;

    // data
    u8 mul;

    u8 mod;
    u8 reg;
    u8 rm;

    bool isPtr;
    bool isSib;
    u8 isVal;
} RM;

typedef struct _data {
    u8 reg;
    u8 rm_code;
    u8 sib;
    u32 val;
    s32 sdisp;
    u32 disp;
    RM rm;
    RegType regType;
    RegType valType;
    bool call;
} Data;

typedef void (*ASM_codeFunc)(void);
typedef void (*ASM_baseFunc)(RM *, s32);
typedef void (*ASM_valFunc)(RM *, s32, u8);
typedef void (*ASM_val32Func)(RM *, s32, u32);
typedef void (*ASM_dataFunc)(Data);

// order: a, c, d, b, sp, bp, si, di, 8-15, ip
extern Reg regs[17];

// order: ss, cs, ds, es, fs, gs
extern u16 sregs[6];
extern XMMReg xregs[16];
// extern YMMReg yregs[16];
extern MMXReg mregs[16];
extern u64 stregs[8];
extern Flags f;
extern bool oper; // operand override prefix
extern bool addr; // address override prefix
extern bool doub; // double  override prefix
extern bool sing; // float   override prefix
extern bool fs; // fs      override prefix
extern bool gs; // gs      override prefix
extern bool lock; // lock prefix
extern bool null; // lock prefix
extern REXPrefix rex;

extern u8 *stack;

#define REG_NULL 255

#define ALIGN_STACK_16 1
#define ALIGN_STACK_32 3
#define ALIGN_STACK_64 7
#define ALIGN_STACK_128 15
#define ALIGN_STACK_256 31
#define ALIGN_STACK_512 63

#define STACK(t, n, v) t* n = (t*)&stack[v]

#define IS_INT(type) (((type) == R_Bit8) || ((type) == R_Bit8H) || ((type) == R_Bit16) || ((type) == R_Bit32) || ((type) == R_Bit64))
#define IS_FLOAT(type) (((type) == R_Float32) || ((type) == R_Float64) || ((type) == R_Float128) || ((type) == R_Float256) || ((type) == R_Float512))

#define IS_R(n, y) ((rex.r == false) ? (n) : (y))
#define IS_B(n, y) ((rex.b == false) ? (n) : (y))
#define IS_X(n, y) ((rex.x == false) ? (n) : (y))
#define IS_W(n, y) ((rex.w == false) ? (n) : (y))
#define IS_OP(n, y) ((oper == false) ? (n) : (y))
#define IS_AD(n, y) ((addr == false) ? (n) : (y))
#define IS_DUP(n, y) ((doub == false && oper == false) ? (n) : (y))
#define IS_REX(n, y) ((rex.enable == 0) ? (n) : (y))
#define IS_FS(n, y) ((fs == false) ? (n) : (y))
#define IS_GS(n, y) ((gs == false) ? (n) : (y))

void u128_set(u128 *dest, u128 *src);
void u128_set64(u128 *dest, u64 high, u64 low);
// void u256_set(u256 *dest, u256 *src);
// void u256_set128(u256 *dest, u128 *high, u128 *low);
// void u256_set64(u256 *dest, u64 hh, u64 hl, u64 lh, u64 ll);

void ASM_init();
RM ASM_getRM(u8 rm, u8 sib, RegType type);
ASM_codeFunc ASM_getFunc(u64 ip);
ASM_codeFunc ASM_getCurrFunc(void);
u64 ASM_getReg(u8 index, RegType type);
u32 ASM_getDisp(RM *rm, s32 disp);
u32 ASM_setupOp(RM *rm, u8 rm_code, u8 sib, s32 disp, u32 ip, RegType type);
bool ASM_getParity(u8 num);
void ASM_incIP(u32 num, RM *rm);
void ASM_setFlags(Reg *prev, Reg *res, RegType type, bool borrow);
char *ASM_getRegName(u8 index, RegType type);
const char* ASM_ptrName(RegType bits);
void ASM_error(void);
void ASM_rmPrint(const char *name, RM *rm, s32 disp, opVal val, bool flip);
void ASM_rexPrint(void);
void ASM_regPrint(void);
void ASM_retPrint(void);
void ASM_end(void);
void ASM_exit(void);

#endif /* EBA6F15E_AF03_4213_BEF3_CE3839FA5FFA */
