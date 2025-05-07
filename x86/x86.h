#ifndef BD6C694D_1681_47C6_8F9C_5C15CFF88DFD
#define BD6C694D_1681_47C6_8F9C_5C15CFF88DFD

#include "data.h"

void ASM_01(u8 rm_code, u8 sib, s32 disp);
void ASM_03(u8 rm_code, u8 sib, s32 disp);

void ASM_0F(u8 index, Data* data);
bool ASM_0F_85(Data *data);
#define ASM_0F_I(ix, data, t) if (ASM_0F_## ix (data)) goto JMP_##t; ASM_end()

void ASM_23(u8 rm_code, u8 sib, s32 disp);
void ASM_31(u8 rm_code, u8 sib, s32 disp);
void ASM_33(u8 rm_code, u8 sib, s32 disp);
void ASM_39(u8 rm_code, u8 sib, s32 disp);
void ASM_3B(u8 rm_code, u8 sib, s32 disp);
void ASM_4X(u8 in_rex);
void ASM_5L(u8 in);
void ASM_5H(u8 in);
void ASM_63(u8 rm_code, u8 sib, s32 disp);
void ASM_64(void);
void ASM_65(void);
void ASM_66(void);
void ASM_67(void);
bool _ASM_73(s8 val);
#define ASM_73(val, t) if (_ASM_73(val)) goto JMP_##t
bool _ASM_74(s8 val);
#define ASM_74(val, t) if (_ASM_74(val)) goto JMP_##t
bool _ASM_75(s8 val);
#define ASM_75(val, t) if (_ASM_75(val)) goto JMP_##t
bool _ASM_76(s8 val);
#define ASM_76(val, t) if (_ASM_76(val)) goto JMP_##t
bool _ASM_77(s8 val);
#define ASM_77(val, t) if (_ASM_77(val)) goto JMP_##t
bool _ASM_7C(s8 val);
#define ASM_7C(val, t) if (_ASM_7C(val)) goto JMP_##t
void ASM_81(u8 rm, u8 sib, s32 disp, u32 val);
void ASM_83(u8 rm, u8 sib, s32 disp, u8 val);
void ASM_85(u8 rm_code, u8 sib, s32 disp);
void ASM_89(u8 rm_code, u8 sib, s32 disp);
void ASM_8B(u8 rm_code, u8 sib, s32 disp);
void ASM_8D(u8 rm_code, u8 sib, s32 disp);
void ASM_90(void);
void ASM_BH(u8 in, u64 val);
void ASM_C1(u8 rm, u8 sib, s32 disp, u8 val);
void ASM_C3(void);
void ASM_C7(u8 rm_code, u8 sib, s32 disp, u32 val);
void ASM_E8(u32 val);
void ASM_E9(u32 val);
void _ASM_EB(u8 val);
#define ASM_EB(val, t) _ASM_EB(val); goto JMP_##t
void ASM_F0(void);
void ASM_F7(u8 rm, u8 sib, s32 disp, u8 val);
void ASM_FF(u8 rm, u8 sib, s32 disp);

#endif /* BD6C694D_1681_47C6_8F9C_5C15CFF88DFD */
