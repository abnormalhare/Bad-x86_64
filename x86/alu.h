#ifndef A50F76D8_CA93_4504_A5B7_A1A7DE0C0BB1
#define A50F76D8_CA93_4504_A5B7_A1A7DE0C0BB1

#include "data.h"

void ASM_ADD_u8 (RM *rm, u32 disp, bool flip);
void ASM_ADD_u16(RM *rm, u32 disp, bool flip);
void ASM_ADD_u32(RM *rm, u32 disp, bool flip);
void ASM_ADD_u64(RM *rm, u32 disp, bool flip);

void ASM_OR_u8 (RM *rm, u32 disp, bool flip);
void ASM_OR_u16(RM *rm, u32 disp, bool flip);
void ASM_OR_u32(RM *rm, u32 disp, bool flip);
void ASM_OR_u64(RM *rm, u32 disp, bool flip);

void ASM_ADC_u8 (RM *rm, u32 disp, bool flip);
void ASM_ADC_u16(RM *rm, u32 disp, bool flip);
void ASM_ADC_u32(RM *rm, u32 disp, bool flip);
void ASM_ADC_u64(RM *rm, u32 disp, bool flip);

void ASM_SBB_u8 (RM *rm, u32 disp, bool flip);
void ASM_SBB_u16(RM *rm, u32 disp, bool flip);
void ASM_SBB_u32(RM *rm, u32 disp, bool flip);
void ASM_SBB_u64(RM *rm, u32 disp, bool flip);

void ASM_AND_u8 (RM *rm, u32 disp, bool flip);
void ASM_AND_u16(RM *rm, u32 disp, bool flip);
void ASM_AND_u32(RM *rm, u32 disp, bool flip);
void ASM_AND_u64(RM *rm, u32 disp, bool flip);

#define ASM_ADD_FLAGS(t, a, b, res)         \
    f.f.cf = res < a;                       \
    f.f.pf = ASM_getParity(res);            \
    f.f.af = ((a ^ b ^ res) & 0x10) != 0;   \
    f.f.zf = (res == 0);                    \
    f.f.sf = (res & ((u##t)INT##t##_MAX + 1)) != 0; \
    f.f.of = ((~(a ^ b)) & (a ^ res) & ((u##t)INT##t##_MAX + 1)) != 0

#define ASM_ADC_FLAGS(t, a, b, res)         \
    f.f.cf = res < a + f.f.cf;              \
    f.f.pf = ASM_getParity(res);            \
    f.f.af = ((a ^ b ^ res) & 0x10) != 0;   \
    f.f.zf = (res == 0);                    \
    f.f.sf = (res & ((u##t)INT##t##_MAX + 1)) != 0; \
    f.f.of = ((~(a ^ b)) & (a ^ res) & ((u##t)INT##t##_MAX + 1)) != 0

#define ASM_BIT_FLAGS(t, a, b, res)         \
    f.f.cf = 0;                             \
    f.f.pf = ASM_getParity(res);            \
    f.f.af = rand() & 1;                    \
    f.f.zf = (res == 0);                    \
    f.f.sf = (res & ((u##t)INT##t##_MAX + 1)) != 0; \
    f.f.of = 0

#define ASM_SUB_FLAGS(t, a, b, res)         \
    f.f.cf = a < b;                         \
    f.f.pf = ASM_getParity(res);            \
    f.f.af = ((a ^ b ^ res) & 0x10) != 0;   \
    f.f.zf = (res == 0);                    \
    f.f.sf = (res & ((u##t)INT##t##_MAX + 1)) != 0; \
    f.f.of = ((a ^ b) & (a ^ res) & ((u##t)INT##t##_MAX + 1)) != 0

#define ASM_SBB_FLAGS(t, a, b, res)         \
    f.f.cf = a < b + f.f.cf;                \
    f.f.pf = ASM_getParity(res);            \
    f.f.af = ((a ^ b ^ res) & 0x10) != 0;   \
    f.f.zf = (res == 0);                    \
    f.f.sf = (res & ((u##t)INT##t##_MAX + 1)) != 0; \
    f.f.of = ((a ^ b) & (a ^ res) & ((u##t)INT##t##_MAX + 1)) != 0


#endif /* A50F76D8_CA93_4504_A5B7_A1A7DE0C0BB1 */
