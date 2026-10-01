#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

int FadeFunc_00(void *data);
int FadeFunc_01(void *data);
int FadeFunc_02(void *data);
int FadeFunc_03(void *data);
int FadeFunc_04(void *data);
int FadeFunc_05(void *data);
int FadeFunc_06(void *data);
int FadeFunc_07(void *data);
int FadeFunc_08(void *data);
int FadeFunc_09(void *data);
int FadeFunc_10(void *data);
int FadeFunc_11(void *data);
int FadeFunc_12(void *data);
int FadeFunc_13(void *data);
int FadeFunc_14(void *data);
int FadeFunc_15(void *data);
int FadeFunc_16(void *data);
int FadeFunc_17(void *data);
int FadeFunc_18(void *data);
int FadeFunc_19(void *data);
int FadeFunc_20(void *data);
int FadeFunc_21(void *data);
int FadeFunc_22(void *data);
int FadeFunc_23(void *data);
int FadeFunc_24(void *data);
int FadeFunc_25(void *data);
int FadeFunc_26(void *data);
int FadeFunc_27(void *data);
int FadeFunc_28(void *data);
int FadeFunc_29(void *data);
int FadeFunc_30(void *data);
int FadeFunc_31(void *data);
int FadeFunc_32(void *data);
int FadeFunc_33(void *data);
int FadeFunc_34(void *data);
int FadeFunc_35(void *data);
int FadeFunc_36(void *data);
int FadeFunc_37(void *data);
int FadeFunc_38(void *data);
int FadeFunc_39(void *data);
int FadeFunc_40(void *data);
int FadeFunc_41(void *data);
int FadeFunc_42(void *data);
void sub_02010C38(void *arg);
void sub_02010E64(void *buf, int mode, int screen, int heapID);
void sub_02010EC8(void *buf);
void *sub_02010EE0(void *buf, int index);
void sub_02010F00(SysTask *task, void *data);
void sub_02010F34(int screen, void *ptr, int a2);
void sub_02010F84(void *a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);
void sub_02010FEC(void *a0, int a1, int a2, int a3);
void sub_02011068(void *a0, int a1, int a2, int a3);

extern void sub_0200FCDC(u16 color);
extern void SetMasterBrightness(int screen, int brightness);
extern void sub_0200FF88(void *arg0, void *arg1, int a2, int a3, int heapID);
extern void sub_0200FFB4(void *arg0, int idx, int heapID);
extern s32 FX_Div(s32 numer, s32 denom);
extern u32 FX_Sqrt(s32 val);
extern s32 _s32_div_f(s32 numer, s32 denom);
extern const s16 FX_SinCosTable_[];
extern void sub_02012DD8(void *a0, const void *a1);
extern int sub_02012E10(void *a0);
extern int sub_020131F4(int a0, int a1);
extern void sub_02013220(int a0, int a1, int a2, int a3);
extern void sub_020132A8(int a0, int a1, int a2);
extern GXWndPlane sub_020132E8(int a0, int a1);
extern GXWndPlane sub_0201333C(int a0);
extern void sub_02013364(int a0, int a1, int a2, int a3, int a4, int a5);
extern void sub_02013424(void *a0, int a1, int a2);
extern void sub_02013440(void *a0, int a1, int a2, int a3, int a4);
extern void sub_02013468(void *a0, int a1, int a2, int a3);
extern void sub_02013488(void *a0, int a1, int a2, int a3, int a4, int a5, int a6);

typedef struct {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u32 state;
    u32 field_10;
    void *work;
    void *field_18;
    void *field_1C;
    u32 heapID;
    u16 field_24;
    u16 pad_26;
    u32 field_28;
    u32 field_2C;
} FadeData;

typedef struct FadeScanlineBuffer {
    s16 cur[2][192];  // 0x000
    s16 next[2][192]; // 0x300
    int index;        // 0x600
} FadeScanlineBuffer; // size: 0x604

typedef struct FadeScanTable {
    s16 start;    // 0x0
    s16 end;      // 0x2
    s16 unk4;     // 0x4
    s16 unk6;     // 0x6
    u8 mode;      // 0x8
    u8 unk9;      // 0x9
    u8 unkA;      // 0xA
    u8 planeMask; // 0xB
} FadeScanTable;

typedef struct FadeRectBand {
    s32 cur[4];   // 0x00
    s32 delta[4]; // 0x10
    s32 end[4];   // 0x20
} FadeRectBand;   // size: 0x30

typedef struct FadeRectWork {
    s32 cur[4];        // 0x00
    s32 delta[4];      // 0x10
    s32 end[4];        // 0x20
    int screen;        // 0x30
    int mode;          // 0x34
    int steps;         // 0x38
    int framesPerStep; // 0x3C
    int counter;       // 0x40
    int planeMask;     // 0x44
    void *plttWork;    // 0x48
} FadeRectWork;        // size: 0x4C

typedef struct FadeWndLine {
    u8 next[192]; // 0x000
    u8 cur[192];  // 0x0C0
    int wnd;      // 0x180
} FadeWndLine;    // size: 0x184

typedef struct FadeWndCtrl {
    FadeWndLine lines[2]; // 0x000
    u8 count;             // 0x308
    u8 screen;            // 0x309
} FadeWndCtrl;            // size: 0x30C

typedef struct FadeScanlineCtrl {
    FadeScanlineBuffer *bufs; // 0x0
    int count;                // 0x4
    int screen;               // 0x8
} FadeScanlineCtrl;

typedef struct {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0C;
    s32 field_10;
    s32 field_14;
    u32 field_18;
} BrightWork;

// .rodata / .data: one aggregate each so MWCC keeps address order
typedef struct FadeParams {
    u16 v[4];
} FadeParams;

typedef struct UnkRodata_0201010C {
    u8 f020F5D58[4];
    u8 f020F5D5C[4];
    u8 f020F5D60[4];
    u8 f020F5D64[4];
    u8 f020F5D68[8];
    FadeParams f020F5D70;
    FadeParams f020F5D78;
    u8 f020F5D80[8];
    FadeParams f020F5D88;
    u8 f020F5D90[8];
    u8 f020F5D98[8];
    u8 f020F5DA0[8];
    u8 f020F5DA8[8];
    u8 f020F5DB0[8];
    u8 f020F5DB8[8];
    FadeParams f020F5DC0;
    u8 f020F5DC8[8];
    u8 f020F5DD0[8];
    u8 f020F5DD8[12];
    u8 f020F5DE4[12];
    u8 f020F5DF0[12];
    u8 f020F5DFC[12];
    u8 f020F5E08[12];
    u8 f020F5E14[12];
    u8 f020F5E20[12];
    u8 f020F5E2C[12];
    u8 f020F5E38[12];
    u8 f020F5E44[12];
    u8 f020F5E50[12];
    u8 f020F5E5C[12];
    u8 f020F5E68[12];
    u8 f020F5E74[12];
    u8 f020F5E80[12];
    u8 f020F5E8C[12];
    u8 f020F5E98[12];
    u8 f020F5EA4[12];
    u8 f020F5EB0[12];
    u8 f020F5EBC[12];
    u8 f020F5EC8[12];
    u8 f020F5ED4[12];
    u8 f020F5EE0[12];
    u8 f020F5EEC[16];
    u8 f020F5EFC[16];
    u8 f020F5F0C[16];
    u8 f020F5F1C[16];
} UnkRodata_0201010C;

static const UnkRodata_0201010C sRodata = {
    { 0xC0, 0x00, 0x00, 0x00 },
    { 0x00, 0xC0, 0x00, 0x00 },
    { 0x00, 0xC0, 0x01, 0x00 },
    { 0xC0, 0x00, 0x01, 0x00 },
    { 0x60, 0x00, 0x00, 0x00, 0x60, 0xC0, 0x00, 0x00 },
    { { 0x0000, 0x1FFF, 0x3F20, 0x0000 } },
    { { 0x0000, 0x1FFF, 0x203F, 0x0001 } },
    { 0x60, 0x00, 0x01, 0x00, 0x60, 0xC0, 0x01, 0x00 },
    { { 0x0000, 0x7F49, 0x3F20, 0x0001 } },
    { 0x00, 0x60, 0x01, 0x00, 0xC0, 0x60, 0x01, 0x00 },
    { 0x00, 0x5E, 0x01, 0x00, 0xC0, 0x62, 0x01, 0x00 },
    { 0x00, 0x00, 0xFF, 0x3F, 0x00, 0x3F, 0x20, 0x00 },
    { 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x3F, 0x20, 0x01 },
    { 0x5E, 0x00, 0x00, 0x00, 0x62, 0xC0, 0x00, 0x00 },
    { 0x00, 0x60, 0x00, 0x00, 0xC0, 0x60, 0x00, 0x00 },
    { { 0x0000, 0x7F49, 0x203F, 0x0000 } },
    { 0x00, 0x00, 0xFF, 0x3F, 0x00, 0x3F, 0x20, 0x00 },
    { 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x3F, 0x20, 0x01 },
    { 0x00, 0x02, 0x00, 0x00, 0x80, 0x00, 0x20, 0x01, 0x00, 0x3F, 0x20, 0x01 },
    { 0x00, 0x00, 0x00, 0x02, 0x80, 0x00, 0x20, 0x01, 0x00, 0x3F, 0x20, 0x00 },
    { 0x40, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0xC0, 0x80, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x20, 0x3F, 0x01 },
    { 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x60, 0x80, 0x60, 0x00, 0x3F, 0x20, 0x01 },
    { 0x80, 0x60, 0x80, 0x60, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x3F, 0x20, 0x00 },
    { 0x80, 0x60, 0x80, 0x60, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x20, 0x3F, 0x01 },
    { 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x3F, 0x20, 0x01 },
    { 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x3F, 0x20, 0x00 },
    { 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x60, 0x80, 0x60, 0x00, 0x20, 0x3F, 0x00 },
    { 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x20, 0x3F, 0x00 },
    { 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x80, 0xC0, 0x00, 0x3F, 0x20, 0x01 },
    { 0x80, 0x00, 0x80, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x3F, 0x20, 0x00 },
    { 0x80, 0x00, 0x60, 0x00, 0x38, 0x8E, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x01 },
    { 0x80, 0x00, 0x80, 0xC0, 0x00, 0x00, 0x80, 0xC0, 0x00, 0x20, 0x3F, 0x01 },
    { 0x80, 0x00, 0x80, 0xC0, 0x80, 0x00, 0xFF, 0xC0, 0x01, 0x20, 0x3F, 0x01 },
    { 0x00, 0x02, 0x00, 0x00, 0x80, 0x00, 0xB0, 0xFF, 0x00, 0x3F, 0x20, 0x01 },
    { 0x00, 0x00, 0x80, 0xC0, 0x80, 0x00, 0x80, 0xC0, 0x00, 0x20, 0x3F, 0x00 },
    { 0x80, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x80, 0xC0, 0x01, 0x20, 0x3F, 0x00 },
    { 0x00, 0x40, 0x01, 0x00, 0x40, 0x80, 0x01, 0x00, 0x80, 0xC0, 0x01, 0x00 },
    { 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x60, 0x00, 0x00, 0x3F, 0x20, 0x01 },
    { 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x60, 0x00, 0x00, 0x3F, 0x20, 0x00 },
    { 0x00, 0x00, 0x00, 0x02, 0x80, 0x00, 0xB0, 0xFF, 0x00, 0x3F, 0x20, 0x00 },
    { 0x00, 0x00, 0xFF, 0x30, 0x00, 0x2F, 0xFF, 0x60, 0x00, 0x60, 0xFF, 0x90, 0x00, 0x90, 0xFF, 0xC0 },
    { 0x00, 0x00, 0x00, 0x30, 0xFF, 0x2F, 0xFF, 0x60, 0x00, 0x60, 0x00, 0x90, 0xFF, 0x90, 0xFF, 0xC0 },
    { 0xFF, 0x00, 0xFF, 0x30, 0x00, 0x2F, 0x00, 0x60, 0xFF, 0x60, 0xFF, 0x90, 0x00, 0x90, 0x00, 0xC0 },
    { 0x00, 0x00, 0xFF, 0x30, 0x00, 0x2F, 0xFF, 0x60, 0x00, 0x60, 0xFF, 0x90, 0x00, 0x90, 0xFF, 0xC0 },
};

#define _020F5D58 (sRodata.f020F5D58)
#define _020F5D5C (sRodata.f020F5D5C)
#define _020F5D60 (sRodata.f020F5D60)
#define _020F5D64 (sRodata.f020F5D64)
#define _020F5D68 (sRodata.f020F5D68)
#define _020F5D70 (sRodata.f020F5D70)
#define _020F5D78 (sRodata.f020F5D78)
#define _020F5D80 (sRodata.f020F5D80)
#define _020F5D88 (sRodata.f020F5D88)
#define _020F5D90 (sRodata.f020F5D90)
#define _020F5D98 (sRodata.f020F5D98)
#define _020F5DA0 (sRodata.f020F5DA0)
#define _020F5DA8 (sRodata.f020F5DA8)
#define _020F5DB0 (sRodata.f020F5DB0)
#define _020F5DB8 (sRodata.f020F5DB8)
#define _020F5DC0 (sRodata.f020F5DC0)
#define _020F5DC8 (sRodata.f020F5DC8)
#define _020F5DD0 (sRodata.f020F5DD0)
#define _020F5DD8 (sRodata.f020F5DD8)
#define _020F5DE4 (sRodata.f020F5DE4)
#define _020F5DF0 (sRodata.f020F5DF0)
#define _020F5DFC (sRodata.f020F5DFC)
#define _020F5E08 (sRodata.f020F5E08)
#define _020F5E14 (sRodata.f020F5E14)
#define _020F5E20 (sRodata.f020F5E20)
#define _020F5E2C (sRodata.f020F5E2C)
#define _020F5E38 (sRodata.f020F5E38)
#define _020F5E44 (sRodata.f020F5E44)
#define _020F5E50 (sRodata.f020F5E50)
#define _020F5E5C (sRodata.f020F5E5C)
#define _020F5E68 (sRodata.f020F5E68)
#define _020F5E74 (sRodata.f020F5E74)
#define _020F5E80 (sRodata.f020F5E80)
#define _020F5E8C (sRodata.f020F5E8C)
#define _020F5E98 (sRodata.f020F5E98)
#define _020F5EA4 (sRodata.f020F5EA4)
#define _020F5EB0 (sRodata.f020F5EB0)
#define _020F5EBC (sRodata.f020F5EBC)
#define _020F5EC8 (sRodata.f020F5EC8)
#define _020F5ED4 (sRodata.f020F5ED4)
#define _020F5EE0 (sRodata.f020F5EE0)
#define _020F5EEC (sRodata.f020F5EEC)
#define _020F5EFC (sRodata.f020F5EFC)
#define _020F5F0C (sRodata.f020F5F0C)
#define _020F5F1C (sRodata.f020F5F1C)

typedef struct UnkData_0201010C {
    u8 f0210F64C[8];
    u8 f0210F654[8];
    u8 f0210F65C[8];
    u8 f0210F664[8];
    u8 f0210F66C[8];
    u8 f0210F674[8];
    u8 f0210F67C[8];
    u8 f0210F684[8];
    u8 f0210F68C[8];
    u8 f0210F694[8];
    u8 f0210F69C[24];
    u8 f0210F6B4[24];
} UnkData_0201010C;

static UnkData_0201010C sData = {
    { 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00 },
    { 0x00, 0x5E, 0xFF, 0x62, 0x80, 0x60, 0x80, 0x60, 0x00, 0x3F, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x33, 0x0B, 0x00, 0x00 },
    { 0x80, 0x60, 0x80, 0x60, 0x00, 0x5E, 0xFF, 0x62, 0x00, 0x3F, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x33, 0x0B, 0x00, 0x00 },
};

#define _0210F64C (sData.f0210F64C)
#define _0210F654 (sData.f0210F654)
#define _0210F65C (sData.f0210F65C)
#define _0210F664 (sData.f0210F664)
#define _0210F66C (sData.f0210F66C)
#define _0210F674 (sData.f0210F674)
#define _0210F67C (sData.f0210F67C)
#define _0210F684 (sData.f0210F684)
#define _0210F68C (sData.f0210F68C)
#define _0210F694 (sData.f0210F694)
#define _0210F69C (sData.f0210F69C)
#define _0210F6B4 (sData.f0210F6B4)

// Forward declarations
static void sub_02010B14(FadeData *data, u32 type);
static int sub_02010BB4(FadeData *data);
static int sub_02010BF4(void *work);
static void sub_0201289C(FadeData *data, const void *table);
static int sub_020128E0(FadeData *data);
static void sub_0201164C(FadeData *data, const void *table);
static int sub_0201169C(FadeData *data);
static void sub_020116EC(FadeData *data, const void *table1, const void *table2);
static int sub_02011744(FadeData *data);
static void sub_02011884(FadeData *data, const void *table);
static int sub_020118BC(FadeData *data);
static void sub_02011B5C(FadeData *data, const void *table);
static int sub_02011B94(FadeData *data);
static void sub_02011D60(FadeData *data, const void *table);
static int sub_02011D98(FadeData *data);
static void sub_02011FF8(FadeData *data, const void *table);
static int sub_02012030(FadeData *data);
static void sub_020122B8(FadeData *data, const void *table);
static int sub_020122F8(FadeData *data);
static void sub_020125EC(FadeData *data, const void *table);
static int sub_0201262C(FadeData *data);
static void sub_02012B1C(FadeData *data, const void *table);
static int sub_02012B80(FadeData *data);
static s32 sub_020109BC(s32 angle);
static s32 sub_020109D8(s32 angle, s32 radius);
static void sub_02010A00(s32 angle, s32 *buf, int count, int start);
static s32 sub_02010A54(s32 angle, s32 halfSize);
static s32 sub_02010A6C(int start, int end, int steps);
static s32 sub_02010A7C(s32 base, s32 offset);
static void sub_02010A8C(s32 *dst, const s32 *delta);
static void sub_02010AB0(s32 *startPos, s32 *endPos, s32 *deltaPos, const u8 *startVals, const u8 *endVals, int steps);
void sub_02010C38(void *arg);
void sub_02010E64(void *buf, int mode, int screen, int heapID);
void sub_02010EC8(void *buf);
static void sub_02010ED0(void *buf);
void *sub_02010EE0(void *buf, int index);
void sub_02010F00(SysTask *task, void *data);
void sub_02010F34(int screen, void *ptr, int a2);
void sub_02010F84(void *a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);
void sub_02010FEC(void *a0, int a1, int a2, int a3);
void sub_02011068(void *a0, int a1, int a2, int a3);
static void sub_02011080(void *buf, u32 a1, int a2, int a3, int a4);
static SysTask *sub_020110C4(void *data);
static void sub_02011130(void *arg);

static void sub_020110DC(void *a0, void *a1, int a2);
static void sub_020110F4(void *a0, void *a1, int a2);
static void sub_02011104(SysTask *task, void *data);
static void sub_020117A0(void *work, const void *table, int duration, int screen, int a4, int a5);
static int sub_020117FC(void *work);
static void sub_02011918(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7);
static int sub_020119F4(void *work);
static void sub_02011A44(s32 radius, int a1, int center, int target, s32 *outLeft, s32 *outRight);
static void sub_02011AD8(void *work);
static void sub_02011BF0(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7);
static int sub_02011CB8(void *work);
static void sub_02011D08(void *work);
static void sub_02011DEC(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7);
static int sub_02011EC0(void *work);
static void sub_02011F10(void *work);
static void sub_02012090(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7);
static int sub_020121A4(void *work);
static void sub_020121F4(void *work);
static void sub_02012204(void *work);
static void sub_02012238(void *work, void *entry);
static void sub_02012290(void *work);
static void sub_02012358(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7);
static int sub_02012454(void *work);
static void sub_020124AC(void *work);
static void sub_020124B0(void *work);
static void sub_020125D4(void *a0, int a1, int a2);
static void sub_0201268C(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7);
static int sub_0201275C(void *work);
static void sub_020127B4(void *work);
static void sub_020127B8(void *work);
static void sub_02012884(void *a0, int a1, int a2);
static void sub_02012940(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7);
static int sub_02012A2C(void *work);
static void sub_02012A8C(void *work);
static void sub_02012A90(void *work);
static void sub_02012ACC(const void *param, void *scanBuf, int progress, int total);
static void sub_02012BE8(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7);
static int sub_02012C68(void *work, FadeData *data);
static void sub_02012CDC(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7);
static int sub_02012D4C(void *work, FadeData *data);

// ============================================================
// FadeFunc_00 - FadeFunc_42
// ============================================================

int FadeFunc_00(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        data->field_28 = 1;
        data->field_2C = 1;
        sub_02010B14(data, 1);
        return 0;
    }
    return sub_02010BB4(data);
}

int FadeFunc_01(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        data->field_28 = 0;
        data->field_2C = 1;
        sub_02010B14(data, 0);
        return 0;
    }
    return sub_02010BB4(data);
}

int FadeFunc_02(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        *(const u8 **)&_0210F64C[0] = _020F5D60;
        sub_0200FCDC(data->field_24);
        sub_0201289C(data, _0210F64C);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_020128E0(data);
}

int FadeFunc_03(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        *(const u8 **)&_0210F64C[0x10] = _020F5D5C;
        sub_0200FCDC(data->field_24);
        sub_0201289C(data, _0210F65C);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_020128E0(data);
}

int FadeFunc_04(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        *(const u8 **)&_0210F64C[0x20] = _020F5D64;
        sub_0200FCDC(data->field_24);
        sub_0201289C(data, _0210F66C);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_020128E0(data);
}

int FadeFunc_05(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        *(const u8 **)&_0210F64C[0x28] = _020F5D58;
        sub_0200FCDC(data->field_24);
        sub_0201289C(data, _0210F674);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_020128E0(data);
}

int FadeFunc_06(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_0201164C(data, _020F5E2C);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_0201169C(data);
}

int FadeFunc_07(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_0201164C(data, _020F5E38);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_0201169C(data);
}

int FadeFunc_08(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        *(const u8 **)&_0210F64C[0x08] = _020F5D90;
        sub_0200FCDC(data->field_24);
        sub_0201289C(data, _0210F654);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_020128E0(data);
}

int FadeFunc_09(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        *(const u8 **)&_0210F64C[0x38] = _020F5D68;
        sub_0200FCDC(data->field_24);
        sub_0201289C(data, _0210F684);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_020128E0(data);
}

int FadeFunc_10(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        *(const u8 **)&_0210F64C[0x48] = _020F5D80;
        sub_0200FCDC(data->field_24);
        sub_0201289C(data, _0210F694);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_020128E0(data);
}

int FadeFunc_11(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        *(const u8 **)&_0210F64C[0x40] = _020F5DB8;
        sub_0200FCDC(data->field_24);
        sub_0201289C(data, _0210F68C);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_020128E0(data);
}

int FadeFunc_12(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_0201164C(data, _020F5E5C);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_0201169C(data);
}

int FadeFunc_13(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_0201164C(data, _020F5E68);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_0201169C(data);
}

int FadeFunc_14(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_020116EC(data, _020F5E80, _020F5E8C);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_02011744(data);
}

int FadeFunc_15(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_020116EC(data, _020F5EA4, _020F5EB0);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_02011744(data);
}

int FadeFunc_16(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_02011884(data, _020F5EC8);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_020118BC(data);
}

int FadeFunc_17(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_02011884(data, _020F5ED4);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_020118BC(data);
}

int FadeFunc_18(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_02011884(data, _020F5DD8);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_020118BC(data);
}

int FadeFunc_19(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_02011884(data, _020F5DE4);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_020118BC(data);
}

int FadeFunc_20(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_02011B5C(data, _020F5DD0);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_02011B94(data);
}

int FadeFunc_21(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_02011B5C(data, _020F5DC8);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_02011B94(data);
}

int FadeFunc_22(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_0201164C(data, _020F5E08);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_0201169C(data);
}

int FadeFunc_23(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_0201164C(data, _020F5E14);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_0201169C(data);
}

int FadeFunc_24(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_0201164C(data, _020F5E20);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_0201169C(data);
}

int FadeFunc_25(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_0201164C(data, _020F5E44);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_0201169C(data);
}

int FadeFunc_26(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_02011D60(data, _020F5DA8);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_02011D98(data);
}

int FadeFunc_27(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_02011D60(data, _020F5DA0);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_02011D98(data);
}

int FadeFunc_28(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        struct {
            const u8 *ptr0;
            const u8 *ptr1;
            u16 field_08;
            u16 field_0A;
            u8 field_0C;
            u8 field_0D;
            u16 field_0E;
        } params;
        params.ptr0 = _020F5EEC;
        params.ptr1 = _020F5EFC;
        params.field_08 = 4;
        params.field_0A = 0;
        params.field_0C = 0x3f;
        params.field_0D = 0x20;
        params.field_0E = 1;
        sub_0200FCDC(data->field_24);
        sub_02011FF8(data, &params);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_02012030(data);
}

int FadeFunc_29(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        struct {
            const u8 *ptr0;
            const u8 *ptr1;
            u16 field_08;
            u16 field_0A;
            u8 field_0C;
            u8 field_0D;
            u16 field_0E;
        } params;
        params.ptr0 = _020F5F0C;
        params.ptr1 = _020F5F1C;
        params.field_08 = 4;
        params.field_0A = 0;
        params.field_0C = 0x3f;
        params.field_0D = 0x20;
        params.field_0E = 0;
        sub_0200FCDC(data->field_24);
        sub_02011FF8(data, &params);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_02012030(data);
}

int FadeFunc_30(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        *(const u8 **)&_0210F64C[0x30] = _020F5EBC;
        sub_0200FCDC(data->field_24);
        sub_0201289C(data, _0210F67C);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_020128E0(data);
}

int FadeFunc_31(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        *(const u8 **)&_0210F64C[0x18] = _020F5DF0;
        sub_0200FCDC(data->field_24);
        sub_0201289C(data, _0210F664);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_020128E0(data);
}

int FadeFunc_32(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        FadeParams params = _020F5D88;

        sub_0200FCDC(data->field_24);
        sub_020122B8(data, &params);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_020122F8(data);
}

int FadeFunc_33(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        FadeParams params = _020F5DC0;

        sub_0200FCDC(data->field_24);
        sub_020122B8(data, &params);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_020122F8(data);
}

int FadeFunc_34(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        FadeParams params = _020F5D78;

        sub_0200FCDC(data->field_24);
        sub_020125EC(data, &params);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_0201262C(data);
}

int FadeFunc_35(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        FadeParams params = _020F5D70;

        sub_0200FCDC(data->field_24);
        sub_020125EC(data, &params);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_0201262C(data);
}

int FadeFunc_36(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_02011884(data, _020F5E98);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_020118BC(data);
}

int FadeFunc_37(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_02011884(data, _020F5EE0);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_020118BC(data);
}

int FadeFunc_38(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_0201164C(data, _020F5DFC);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_0201169C(data);
}

int FadeFunc_39(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_0201164C(data, _020F5E50);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_0201169C(data);
}

int FadeFunc_40(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        *(const u8 **)&_0210F64C[0x5c] = _020F5D98;
        sub_0200FCDC(data->field_24);
        sub_02012B1C(data, _0210F69C);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_02012B80(data);
}

int FadeFunc_41(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        *(const u8 **)&_0210F64C[0x74] = _020F5DB0;
        sub_0200FCDC(data->field_24);
        sub_02012B1C(data, _0210F6B4);
        data->field_28 = 0;
        data->field_2C = 0;
        return 0;
    }
    return sub_02012B80(data);
}

int FadeFunc_42(void *arg) {
    FadeData *data = arg;
    if (data->state == 0) {
        sub_0200FCDC(data->field_24);
        sub_02012DD8(data, _020F5E74);
        data->field_28 = 1;
        data->field_2C = 0;
        return 0;
    }
    return sub_02012E10(data);
}

// ============================================================
// Math utility functions
// ============================================================

static s32 sub_020109BC(s32 angle) {
    int idx = angle >> 4;
    s16 sinVal = FX_SinCosTable_[idx * 2];
    s16 cosVal = FX_SinCosTable_[idx * 2 + 1];
    return FX_Div(sinVal, cosVal);
}

static s32 sub_020109D8(s32 angle, s32 radius) {
    return FX_Whole(FX_Mul(sub_020109BC(angle), radius << FX32_SHIFT));
}

static void sub_02010A00(s32 angle, s32 *buf, int count, int start) {
    fx32 tanVal = sub_020109BC(angle);
    int i;

    for (i = start; i < count; i++) {
        buf[i] = FX_Whole(FX_Mul(tanVal, i << FX32_SHIFT));
    }
}

static s32 sub_02010A54(s32 angle, s32 size) {
    return FX_Div((size / 2) << FX32_SHIFT, sub_020109BC(angle));
}

static s32 sub_02010A6C(int start, int end, int steps) {
    return _s32_div_f((end - start) << 7, steps);
}

static s32 sub_02010A7C(s32 base, s32 offset) {
    s32 result = base + offset;
    if (result < 0) {
        result = 0;
    }
    if (result > 255) {
        result = 255;
    }
    return result;
}

static void sub_02010A8C(s32 *dst, const s32 *delta) {
    dst[0] += delta[0];
    dst[1] += delta[1];
    dst[2] += delta[2];
    dst[3] += delta[3];
}

static void sub_02010AB0(s32 *startPos, s32 *endPos, s32 *deltaPos, const u8 *startVals, const u8 *endVals, int steps) {
    startPos[0] = startVals[0] << 7;
    startPos[1] = startVals[1] << 7;
    startPos[2] = startVals[2] << 7;
    startPos[3] = startVals[3] << 7;
    endPos[0] = endVals[0];
    endPos[1] = endVals[1];
    endPos[2] = endVals[2];
    endPos[3] = endVals[3];
    deltaPos[0] = sub_02010A6C(startVals[0], endVals[0], steps);
    deltaPos[1] = sub_02010A6C(startVals[1], endVals[1], steps);
    deltaPos[2] = sub_02010A6C(startVals[2], endVals[2], steps);
    deltaPos[3] = sub_02010A6C(startVals[3], endVals[3], steps);
}

// ============================================================
// Brightness fade functions
// ============================================================

static void sub_02010B14(FadeData *data, u32 type) {
    BrightWork *work;
    s32 startBright, endBright;

    data->work = Heap_Alloc((enum HeapID)data->heapID, sizeof(BrightWork));
    memset(data->work, 0, sizeof(BrightWork));
    work = data->work;
    if (type == 0) {
        if (data->field_24 == 0x7FFF) {
            startBright = 16;
            endBright = 0;
        } else if (data->field_24 == 0) {
            startBright = -16;
            endBright = 0;
        } else {
            startBright = -16;
            endBright = 0;
            GF_AssertFail();
        }
    } else {
        if (data->field_24 == 0x7FFF) {
            startBright = 0;
            endBright = 16;
        } else if (data->field_24 == 0) {
            startBright = 0;
            endBright = -16;
        } else {
            startBright = 0;
            endBright = -16;
            GF_AssertFail();
        }
    }
    SetMasterBrightness(data->field_10, startBright);
    work->field_00 = data->field_04;
    work->field_04 = data->field_08;
    work->field_08 = 0;
    work->field_0C = startBright << 7;
    work->field_10 = endBright << 7;
    work->field_14 = sub_02010A6C(startBright, endBright, data->field_04);
    work->field_18 = data->field_10;
    data->state++;
}

static int sub_02010BB4(FadeData *data) {
    int ret = 0;
    BrightWork *work = data->work;

    switch (data->state) {
    case 1:
        if (sub_02010BF4(work) == 1) {
            data->state++;
        }
        break;
    case 2:
        Heap_Free(work);
        data->work = 0;
        ret = 1;
        data->state++;
        break;
    case 3:
        ret = 1;
        break;
    }
    return ret;
}

static int sub_02010BF4(void *arg) {
    BrightWork *work = arg;
    int done = FALSE;
    s32 steps;

    if (++work->field_08 >= work->field_04) {
        work->field_08 = 0;
        steps = work->field_00 - 1;
        if (steps > 0) {
            work->field_00 = steps;
            work->field_0C += work->field_14;
        } else {
            work->field_0C = work->field_10;
            done = TRUE;
        }
        SetMasterBrightness(work->field_18, work->field_0C / 128);
    }
    return done;
}

// ============================================================
// sub_02010C38 - HBlank handler: per-line window X positions
// ============================================================

static inline void FadeScanline_SetWindow(FadeScanlineCtrl *ctrl, int idx, int line) {
    FadeScanlineBuffer *scanBuf = sub_02010EE0(ctrl, idx);
    int screen = ctrl->screen;
    s16 right = scanBuf->cur[1][line];
    s16 left = scanBuf->cur[0][line];

    if (scanBuf->index == 0) {
        if (screen == 0) {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                G2_SetWnd0Position(left, 0, right, 192);
            }
        } else {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                G2S_SetWnd0Position(left, 0, right, 192);
            }
        }
    } else {
        if (screen == 0) {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                G2_SetWnd1Position(left, 0, right, 192);
            }
        } else {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                G2S_SetWnd1Position(left, 0, right, 192);
            }
        }
    }
}

void sub_02010C38(void *arg) {
    FadeScanlineCtrl *ctrl = arg;
    int vcount;
    int line;

    GF_ASSERT(ctrl != NULL);
    vcount = GX_GetVCount();
    if (vcount < 192) {
        line = vcount + 1;
        if (line > 191) {
            line -= 192;
        }
        if (ctrl->count == 1) {
            FadeScanline_SetWindow(ctrl, 0, line);
        } else {
            FadeScanline_SetWindow(ctrl, 0, line);
            FadeScanline_SetWindow(ctrl, 1, line);
        }
    }
}

// ============================================================
// Buffer management
// ============================================================

void sub_02010E64(void *buf, int mode, int screen, int heapID) {
    FadeScanlineCtrl *ctrl = buf;
    int i;

    switch (mode) {
    case 0:
    case 1:
        ctrl->bufs = Heap_Alloc((enum HeapID)heapID, sizeof(FadeScanlineBuffer));
        ctrl->count = 1;
        ctrl->screen = screen;
        ctrl->bufs->index = mode;
        break;
    case 2:
        ctrl->bufs = Heap_Alloc((enum HeapID)heapID, sizeof(FadeScanlineBuffer) * 2);
        ctrl->count = 2;
        ctrl->screen = screen;
        for (i = 0; i < 2; i++) {
            ctrl->bufs[i].index = i;
        }
        break;
    }
}

void sub_02010EC8(void *buf) {
    sub_02010ED0(buf);
}

static void sub_02010ED0(void *buf) {
    u32 *p = buf;
    Heap_Free((void *)p[0]);
    p[0] = 0;
}

void *sub_02010EE0(void *buf, int index) {
    u32 *p = buf;
    if ((int)p[1] <= index) {
        GF_AssertFail();
    }
    return (void *)((u8 *)(void *)p[0] + index * 0x604);
}

void sub_02010F00(SysTask *task, void *data) {
    FadeScanlineCtrl *ctrl = data;
    FadeScanlineBuffer *scanBuf;
    int i;

    for (i = 0; i < ctrl->count; i++) {
        scanBuf = sub_02010EE0(ctrl, i);
        memcpy(scanBuf->cur, scanBuf->next, sizeof(scanBuf->cur));
    }
    SysTask_Destroy(task);
}

void sub_02010F34(int screen, void *ptr, int a2) {
    if (screen == 0) {
        sub_02013424(ptr, 0, a2);
        return;
    }
    sub_02013424(ptr, 1, a2);
    sub_02013440(ptr, 0x3f, 0, 0, a2);
    sub_02013488(ptr, 0, 0, 0, 0, 0, a2);
    sub_02013468(ptr, 0x20, 0, a2);
}

void sub_02010F84(void *a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9) {
    if (a9 == 0) {
        sub_02013220(a1, 0, a3, a4);
        sub_020132A8(a2, 0, a4);
        sub_02013364(a5, a6, a7, a8, a3, a4);
    } else {
        sub_02013440(a0, a1, 0, a3, a4);
        sub_02013468(a0, a2, 0, a4);
        sub_02013488(a0, a5, a6, a7, a8, a3, a4);
    }
}

void sub_02010FEC(void *a0, int a1, int a2, int a3) {
    GXWndPlane outside;
    GXWndPlane inside;

    outside = sub_020132E8(a1, a2);
    inside = sub_0201333C(a2);
    if (a3 == 0) {
        sub_02013220(inside.planeMask, 0, a1, a2);
        sub_020132A8(outside.planeMask, 0, a2);
    } else {
        sub_02013440(a0, inside.planeMask, 0, a1, a2);
        sub_02013468(a0, outside.planeMask, 0, a2);
    }
}

void sub_02011068(void *a0, int a1, int a2, int a3) {
    if (a3 == 0) {
        sub_020131F4(a1, a2);
        return;
    }
    sub_02013424(a0, a1, a2);
}

// ============================================================
// Internal functions
// ============================================================

static void sub_02011080(void *buf, u32 a1, int a2, int a3, int a4) {
    u8 *p = buf;
    memset(p, 0, 0xC3 * 4);
    if (a2 == 1) {
        *(u32 *)(p + 0x180) = a3;
        p[0x308] = (u8)a2;
        p[0x309] = (u8)a1;
    } else {
        *(u32 *)(p + 0x180) = a3;
        *(u32 *)(p + 0x304) = a4;
        p[0x308] = (u8)a2;
        p[0x309] = (u8)a1;
    }
}

static SysTask *sub_020110C4(void *data) {
    return SysTask_CreateOnVWaitQueue(sub_02011104, data, 0x3FF);
}

static void sub_020110DC(void *a0, void *a1, int a2) {
    u8 *p = a1;
    sub_0200FF88(a0, a1, (int)sub_02011130, (int)p[0x309], a2);
}

static void sub_020110F4(void *a0, void *a1, int a2) {
    u8 *p = (u8 *)a1;
    sub_0200FFB4(a0, p[0x309], a2);
}

static void sub_02011104(SysTask *task, void *data) {
    u8 *p = data;
    int i;
    for (i = 0; i < 2; i++) {
        memcpy(p + 0xC0, p, 0xC0);
        p += 0xC0 + 0xC4;
    }
    SysTask_Destroy(task);
}

// ============================================================
// sub_02011130 - HBlank callback: per-line window plane masks
// ============================================================

static inline void FadeWnd_Apply(FadeWndCtrl *ctrl, int idx, int line) {
    FadeWndLine *wndLine = &ctrl->lines[idx];
    int screen;

    if (wndLine->cur[line] == 0) {
        if (ctrl->screen == 0) {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                G2_SetWndOutsidePlane(0x3F, TRUE);
            }
        } else {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                G2S_SetWndOutsidePlane(0x3F, TRUE);
            }
        }
        screen = ctrl->screen;
        if (wndLine->wnd == 0) {
            if (screen == 0) {
                if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                    G2_SetWnd0InsidePlane(0x20, TRUE);
                }
            } else {
                if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                    G2S_SetWnd0InsidePlane(0x20, TRUE);
                }
            }
        } else {
            if (screen == 0) {
                if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                    G2_SetWnd1InsidePlane(0x20, TRUE);
                }
            } else {
                if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                    G2S_SetWnd1InsidePlane(0x20, TRUE);
                }
            }
        }
    } else {
        if (ctrl->screen == 0) {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                G2_SetWndOutsidePlane(0x20, TRUE);
            }
        } else {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                G2S_SetWndOutsidePlane(0x20, TRUE);
            }
        }
        screen = ctrl->screen;
        if (wndLine->wnd == 0) {
            if (screen == 0) {
                if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                    G2_SetWnd0InsidePlane(0x3F, TRUE);
                }
            } else {
                if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                    G2S_SetWnd0InsidePlane(0x3F, TRUE);
                }
            }
        } else {
            if (screen == 0) {
                if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                    G2_SetWnd1InsidePlane(0x3F, TRUE);
                }
            } else {
                if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                    G2S_SetWnd1InsidePlane(0x3F, TRUE);
                }
            }
        }
    }
}

void sub_02011130(void *arg) {
    FadeWndCtrl *ctrl = arg;
    int line;

    GF_ASSERT(ctrl != NULL);
    line = GX_GetVCount();
    if (line < 192) {
        line++;
        if (line > 191) {
            line -= 192;
        }
        if (ctrl->count == 1) {
            FadeWnd_Apply(ctrl, 0, line);
        } else {
            FadeWnd_Apply(ctrl, 0, line);
            FadeWnd_Apply(ctrl, 1, line);
        }
    }
}

// ============================================================
// Window/wipe fade init + update functions
// ============================================================

static void sub_0201164C(FadeData *data, const void *table) {
    void *work;
    u8 *w;

    data->work = Heap_Alloc((enum HeapID)data->heapID, 0x4C);
    work = data->work;
    w = work;
    sub_020117A0(work, table, data->field_04, data->field_08, data->field_10, (int)data->field_18);
    if (((const u8 *)table)[8] == 0) {
        sub_02011068((void *)(int)data->field_18, 1, *(u32 *)(w + 0x30), *(u32 *)(w + 0x44));
    } else {
        sub_02011068((void *)(int)data->field_18, 2, *(u32 *)(w + 0x30), *(u32 *)(w + 0x44));
    }
    data->state++;
}

static int sub_0201169C(FadeData *data) {
    int ret = 0;
    u8 *work = data->work;

    switch (data->state) {
    case 1:
        if (sub_020117FC(work) == 1) {
            sub_02010F34(*(u32 *)(work + 0x44), data->field_18, data->field_10);
            data->state++;
        }
        break;
    case 2:
        Heap_Free(work);
        data->work = 0;
        ret = 1;
        data->state++;
        break;
    case 3:
        ret = 1;
        break;
    }
    return ret;
}

static void sub_020116EC(FadeData *data, const void *table1, const void *table2) {
    void *work;
    u8 *w;

    data->work = Heap_Alloc((enum HeapID)data->heapID, 0x98);
    work = data->work;
    w = work;
    sub_020117A0(work, table1, data->field_04, data->field_08, data->field_10, (int)data->field_18);
    sub_020117A0((u8 *)work + 0x4C, table2, data->field_04, data->field_08, data->field_10, (int)data->field_18);
    sub_02011068((void *)(int)data->field_18, 3, data->field_10, *(u32 *)(w + 0x44));
    data->state++;
}

static int sub_02011744(FadeData *data) {
    int ret = 0;
    u8 *work = data->work;

    switch (data->state) {
    case 1: {
        int r0 = sub_020117FC(work);
        int r1 = sub_020117FC((u8 *)work + 0x4C);
        if (r0 + r1 == 2) {
            sub_02010F34(*(u32 *)(work + 0x44), data->field_18, data->field_10);
            data->state++;
        }
        break;
    }
    case 2:
        Heap_Free(work);
        data->work = 0;
        ret = 1;
        data->state++;
        break;
    case 3:
        ret = 1;
        break;
    }
    return ret;
}

static void sub_020117A0(void *work, const void *table, int duration, int screen, int a4, int a5) {
    u8 *w = work;
    const u8 *t = table;

    sub_02010AB0((s32 *)w, (s32 *)(w + 0x20), (s32 *)(w + 0x10), t, t + 4, duration);
    *(u32 *)(w + 0x30) = a4;
    *(u32 *)(w + 0x34) = t[8];
    *(u32 *)(w + 0x38) = duration;
    *(u32 *)(w + 0x3C) = screen;
    *(u32 *)(w + 0x40) = 0;
    *(u32 *)(w + 0x48) = a5;
    *(u32 *)(w + 0x44) = t[11];
    sub_02010F84((void *)a5, t[9], t[10], t[8], a4, t[0], t[1], t[2], t[3], *(u32 *)(w + 0x44));
}

static int sub_020117FC(void *arg) {
    FadeRectWork *work = arg;
    s32 steps;

    if (++work->counter >= work->framesPerStep) {
        work->counter = 0;
        steps = work->steps - 1;
        if (steps > 0) {
            work->steps = steps;
            sub_02010A8C(work->cur, work->delta);
        } else {
            sub_02013488(work->plttWork, work->end[0], work->end[1], work->end[2], work->end[3], work->mode, work->screen);
            return TRUE;
        }
        sub_02013488(work->plttWork, work->cur[0] / 128, work->cur[1] / 128, work->cur[2] / 128, work->cur[3] / 128, work->mode, work->screen);
    }
    return FALSE;
}

// ============================================================
// Circle/iris fade functions
// ============================================================

static void sub_02011884(FadeData *data, const void *table) {
    data->work = Heap_Alloc((enum HeapID)data->heapID, 0x38);
    sub_02011918(data->work, table, data->field_04, data->field_08, data->field_10, (int)data->field_18, (int)data->field_1C, data->heapID);
    data->state++;
}

static int sub_020118BC(FadeData *data) {
    int ret = 0;
    u8 *work = data->work;

    switch (data->state) {
    case 1:
        if (sub_020119F4(work) == 1) {
            sub_02010F34(*(u32 *)(work + 0x2C), *(void **)(work + 0x30), data->field_10);
            data->state++;
        }
        break;
    case 2:
        sub_02010EC8(work);
        Heap_Free(data->work);
        data->work = 0;
        ret = 1;
        data->state++;
        break;
    case 3:
        ret = 1;
        break;
    default:
        GF_AssertFail();
        break;
    }
    return ret;
}

static void sub_02011918(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7) {
    u8 *w = work;
    const FadeScanTable *t = table;
    s32 delta;
    FadeScanlineBuffer *scanBuf;

    delta = sub_02010A6C(t->start, t->end, duration);
    sub_02010E64(w, t->mode, a4, a7);
    *(s32 *)(w + 0x0C) = t->start << 7;
    *(s32 *)(w + 0x10) = t->unk4;
    *(s32 *)(w + 0x14) = t->unk6;
    *(s32 *)(w + 0x18) = delta;
    *(s32 *)(w + 0x1C) = duration;
    *(s32 *)(w + 0x20) = screen;
    *(s32 *)(w + 0x24) = 0;
    *(void **)(w + 0x30) = (void *)a5;
    *(u32 *)(w + 0x34) = a6;
    *(u32 *)(w + 0x28) = a7;
    *(u32 *)(w + 0x2C) = t->planeMask;
    sub_02011AD8(w);

    SysTask_CreateOnVWaitQueue(sub_02010F00, w, 0x3FF);

    scanBuf = sub_02010EE0(w, 0);
    sub_02010F84((void *)a5, t->unk9, t->unkA, t->mode, a4, scanBuf->next[0][0], 0, scanBuf->next[1][0], 0xC0, *(u32 *)(w + 0x2C));

    if (t->mode == 0) {
        sub_02011068((void *)a5, 1, a4, *(u32 *)(w + 0x2C));
    } else {
        sub_02011068((void *)a5, 2, a4, *(u32 *)(w + 0x2C));
    }

    sub_0200FF88((void *)*(u32 *)(w + 0x34), w, (int)sub_02010C38, a4, a7);
}

static int sub_020119F4(void *work) {
    u8 *w = work;
    s32 tick = *(s32 *)(w + 0x24) + 1;
    *(s32 *)(w + 0x24) = tick;
    if (tick >= *(s32 *)(w + 0x20)) {
        *(s32 *)(w + 0x24) = 0;
        s32 count = *(s32 *)(w + 0x1C) - 1;
        if (count > 0) {
            *(s32 *)(w + 0x1C) = count;
            *(s32 *)(w + 0x0C) += *(s32 *)(w + 0x18);
            sub_02011AD8(w);
            SysTask_CreateOnVWaitQueue(sub_02010F00, w, 0x3FF);
        } else {
            sub_0200FFB4((void *)*(u32 *)(w + 0x34), *(u32 *)(w + 0x08), *(u32 *)(w + 0x28));
            return 1;
        }
    }
    return 0;
}

static void sub_02011A44(s32 radius, int a1, int center, int target, s32 *outLeft, s32 *outRight) {
    s32 r = radius / 128;
    s32 diff = target - center;
    s32 half;

    if (diff < 0) {
        diff = -diff;
    }
    if (diff >= r) {
        *outLeft = 0;
        *outRight = 0;
        return;
    }
    half = FX_Whole(FX_Sqrt(FX_Mul(r << FX32_SHIFT, r << FX32_SHIFT) - FX_Mul(diff << FX32_SHIFT, diff << FX32_SHIFT)));
    *outLeft = a1 - half;
    if (*outLeft < 0) {
        *outLeft = 0;
    }
    *outRight = *outLeft + half * 2;
    if (*outRight > 255) {
        *outRight = 255;
    }
}

static void sub_02011AD8(void *work) {
    u8 *w = work;
    FadeScanlineBuffer *scanBuf;
    s32 left, right;
    int i;

    scanBuf = sub_02010EE0(w, 0);
    for (i = 0; i < 192; i++) {
        if (i <= *(s32 *)(w + 0x14)) {
            sub_02011A44(*(s32 *)(w + 0x0C), *(s32 *)(w + 0x10), *(s32 *)(w + 0x14), i, &left, &right);
        } else if (i <= *(s32 *)(w + 0x14) * 2) {
            left = scanBuf->next[0][*(s32 *)(w + 0x14) * 2 - i];
            right = scanBuf->next[1][*(s32 *)(w + 0x14) * 2 - i];
        } else {
            sub_02011A44(*(s32 *)(w + 0x0C), *(s32 *)(w + 0x10), *(s32 *)(w + 0x14), i, &left, &right);
        }
        scanBuf->next[0][i] = left;
        scanBuf->next[1][i] = right;
    }
}

// ============================================================
// Shutter fade functions
// ============================================================

static void sub_02011B5C(FadeData *data, const void *table) {
    data->work = Heap_Alloc((enum HeapID)data->heapID, 0x30);
    sub_02011BF0(data->work, table, data->field_04, data->field_08, data->field_10, (int)data->field_18, (int)data->field_1C, data->heapID);
    data->state++;
}

static int sub_02011B94(FadeData *data) {
    int ret = 0;
    u8 *work = data->work;

    switch (data->state) {
    case 1:
        if (sub_02011CB8(work) == 1) {
            sub_02010F34(*(u32 *)(work + 0x20), *(void **)(work + 0x24), data->field_10);
            data->state++;
        }
        break;
    case 2:
        sub_02010EC8(work);
        Heap_Free(data->work);
        data->work = 0;
        ret = 1;
        data->state++;
        break;
    case 3:
        ret = 1;
        break;
    default:
        GF_AssertFail();
        break;
    }
    return ret;
}

static void sub_02011BF0(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7) {
    u8 *w = work;
    const u8 *t = table;

    *(s32 *)(w + 0x10) = sub_02010A6C(*(u16 *)t, *(u16 *)(t + 2), duration);
    sub_02010E64(w, t[4], a4, a7);
    *(s32 *)(w + 0x0C) = *(u16 *)t << 7;
    *(s32 *)(w + 0x14) = duration;
    *(s32 *)(w + 0x18) = screen;
    *(s32 *)(w + 0x1C) = 0;
    *(void **)(w + 0x24) = (void *)a5;
    *(u32 *)(w + 0x28) = a6;
    *(u32 *)(w + 0x2C) = a7;
    *(u32 *)(w + 0x20) = t[7];
    sub_02011D08(w);

    SysTask_CreateOnVWaitQueue(sub_02010F00, w, 0x3FF);

    {
        s16 *slot = sub_02010EE0(w, 0);
        sub_02010F84((void *)a5, t[5], t[6], t[4], a4, *(s16 *)((u8 *)slot + 0x300), 0, *(s16 *)((u8 *)slot + 0x480), 0xC0, *(u32 *)(w + 0x20));
    }

    if (t[4] == 0) {
        sub_02011068((void *)a5, 1, a4, *(u32 *)(w + 0x20));
    } else {
        sub_02011068((void *)a5, 2, a4, *(u32 *)(w + 0x20));
    }

    sub_0200FF88((void *)*(u32 *)(w + 0x28), w, (int)sub_02010C38, a4, a7);
}

static int sub_02011CB8(void *work) {
    u8 *w = work;
    s32 tick = *(s32 *)(w + 0x1C) + 1;
    *(s32 *)(w + 0x1C) = tick;
    if (tick >= *(s32 *)(w + 0x18)) {
        *(s32 *)(w + 0x1C) = 0;
        s32 count = *(s32 *)(w + 0x14) - 1;
        if (count > 0) {
            *(s32 *)(w + 0x14) = count;
            *(s32 *)(w + 0x0C) += *(s32 *)(w + 0x10);
            sub_02011D08(w);
            SysTask_CreateOnVWaitQueue(sub_02010F00, w, 0x3FF);
        } else {
            sub_0200FFB4((void *)*(u32 *)(w + 0x28), *(u32 *)(w + 0x08), *(u32 *)(w + 0x2C));
            return 1;
        }
    }
    return 0;
}

static void sub_02011D08(void *work) {
    u8 *w = work;
    s32 buf[192];
    FadeScanlineBuffer *scanBuf;
    int i;

    scanBuf = sub_02010EE0(w, 0);
    sub_02010A00(*(s32 *)(w + 0x0C) / 128, buf, 192, 0);
    for (i = 0; i < 192; i++) {
        scanBuf->next[0][i] = sub_02010A7C(0x80, -buf[i]);
        scanBuf->next[1][i] = sub_02010A7C(0x80, buf[i]);
    }
}

// ============================================================
// Rotation fade functions
// ============================================================

static void sub_02011D60(FadeData *data, const void *table) {
    data->work = Heap_Alloc((enum HeapID)data->heapID, 0x34);
    sub_02011DEC(data->work, table, data->field_04, data->field_08, data->field_10, (int)data->field_18, (int)data->field_1C, data->heapID);
    data->state++;
}

static int sub_02011D98(FadeData *data) {
    int ret = 0;
    u8 *work = data->work;

    switch (data->state) {
    case 1:
        if (sub_02011EC0(work) == 1) {
            sub_02010F34(*(u32 *)(work + 0x24), *(void **)(work + 0x28), data->field_10);
            data->state++;
        }
        break;
    case 2:
        sub_02010EC8(work);
        Heap_Free(data->work);
        data->work = 0;
        ret = 1;
        data->state++;
        break;
    case 3:
        ret = 1;
        break;
    }
    return ret;
}

static void sub_02011DEC(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7) {
    u8 *w = work;
    const u8 *t = table;
    s32 delta;

    delta = (*(u16 *)(t + 2) - *(u16 *)t) / duration;
    sub_02010E64(w, t[4], a4, a7);
    *(s32 *)(w + 0x0C) = 2 << 18;
    *(u32 *)(w + 0x10) = *(u16 *)t;
    *(s32 *)(w + 0x14) = delta;
    *(s32 *)(w + 0x18) = duration;
    *(s32 *)(w + 0x1C) = screen;
    *(s32 *)(w + 0x20) = 0;
    *(void **)(w + 0x28) = (void *)a5;
    *(u32 *)(w + 0x2C) = a6;
    *(u32 *)(w + 0x30) = a7;
    *(u32 *)(w + 0x24) = t[7];
    sub_02011F10(w);

    SysTask_CreateOnVWaitQueue(sub_02010F00, w, 0x3FF);

    {
        s16 *slot = sub_02010EE0(w, 0);
        sub_02010F84((void *)a5, t[5], t[6], t[4], a4, *(s16 *)((u8 *)slot + 0x3C0), 0, *(s16 *)((u8 *)slot + 0x540), 0xC0, *(u32 *)(w + 0x24));
    }

    if (t[4] == 0) {
        sub_02011068((void *)a5, 1, a4, *(u32 *)(w + 0x24));
    } else {
        sub_02011068((void *)a5, 2, a4, *(u32 *)(w + 0x24));
    }

    sub_0200FF88((void *)*(u32 *)(w + 0x2C), w, (int)sub_02010C38, a4, a7);
}

static int sub_02011EC0(void *work) {
    u8 *w = work;
    s32 tick = *(s32 *)(w + 0x20) + 1;
    *(s32 *)(w + 0x20) = tick;
    if (tick >= *(s32 *)(w + 0x1C)) {
        *(s32 *)(w + 0x20) = 0;
        s32 count = *(s32 *)(w + 0x18) - 1;
        if (count > 0) {
            *(s32 *)(w + 0x18) = count;
            *(u32 *)(w + 0x10) += *(s32 *)(w + 0x14);
            sub_02011F10(w);
            SysTask_CreateOnVWaitQueue(sub_02010F00, w, 0x3FF);
        } else {
            sub_0200FFB4((void *)*(u32 *)(w + 0x2C), *(u32 *)(w + 0x08), *(u32 *)(w + 0x30));
            return 1;
        }
    }
    return 0;
}

static void sub_02011F10(void *work) {
    u8 *w = work;
    s32 buf[192];
    int count;
    FadeScanlineBuffer *scanBuf;
    s32 ampl;
    s32 angle;
    s32 peak;
    s32 left, right;
    int i;
    int j;

    scanBuf = sub_02010EE0(w, 0);
    ampl = FX_Whole(FX_Mul(FX_SinCosTable_[(*(s32 *)(w + 0x10) >> 4) * 2], *(s32 *)(w + 0x0C)));
    angle = (0xFFFF * (180 - ((ampl * 2) / 21 + 1) * 2) / 360) / 2;
    count = FX_Whole(sub_02010A54(angle, 256));
    GF_ASSERT(count < 192);
    sub_02010A00(angle, buf, count, 0);
    for (i = 0; i < 96; i++) {
        j = count - (i + 1);
        peak = ampl;
        if (j > 0 && buf[j] > ampl) {
            peak = buf[j];
        }
        left = sub_02010A7C(0x80, -peak);
        right = sub_02010A7C(0x80, peak);
        scanBuf->next[0][i] = left;
        scanBuf->next[1][i] = right;
        scanBuf->next[0][191 - i] = left;
        scanBuf->next[1][191 - i] = right;
    }
}

// ============================================================
// Multi-band fade functions
// ============================================================

static void sub_02011FF8(FadeData *data, const void *table) {
    data->work = Heap_Alloc((enum HeapID)data->heapID, 0x30);
    sub_02012090(data->work, table, data->field_04, data->field_08, data->field_10, (int)data->field_18, (int)data->field_1C, data->heapID);
    data->state++;
}

static int sub_02012030(FadeData *data) {
    int ret = 0;
    u8 *work = data->work;

    switch (data->state) {
    case 1:
        if (sub_020121A4(work) == 1) {
            sub_02010F34(*(u32 *)(work + 0x20), *(void **)(work + 0x24), data->field_10);
            data->state++;
        }
        break;
    case 2:
        sub_020121F4(work);
        sub_02010EC8(work);
        Heap_Free(data->work);
        data->work = 0;
        ret = 1;
        data->state++;
        break;
    case 3:
        ret = 1;
        break;
    default:
        GF_AssertFail();
        break;
    }
    return ret;
}

static void sub_02012090(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7) {
    u8 *w = work;
    const u8 *t = table;
    int i;

    *(void **)(w + 0x0C) = Heap_Alloc((enum HeapID)a7, 0x30 * *(u16 *)(t + 8));
    GF_ASSERT(*(void **)(w + 0x0C) != NULL);
    *(u32 *)(w + 0x10) = *(u16 *)(t + 8);

    for (i = 0; i < *(u16 *)(t + 8); i++) {
        sub_02010AB0(((FadeRectBand *)*(void **)(w + 0x0C))[i].cur, ((FadeRectBand *)*(void **)(w + 0x0C))[i].end, ((FadeRectBand *)*(void **)(w + 0x0C))[i].delta, ((const u8 **)t)[0] + i * 4, ((const u8 **)t)[1] + i * 4, duration);
    }

    sub_02010E64(w, *(u16 *)(t + 0x0A), a4, a7);
    *(s32 *)(w + 0x14) = duration;
    *(s32 *)(w + 0x18) = screen;
    *(s32 *)(w + 0x1C) = 0;
    *(void **)(w + 0x24) = (void *)a5;
    *(u32 *)(w + 0x28) = a6;
    *(u32 *)(w + 0x2C) = a7;
    *(u32 *)(w + 0x20) = *(u16 *)(t + 0x0E);
    sub_02012204(w);

    SysTask_CreateOnVWaitQueue(sub_02010F00, w, 0x3FF);

    {
        s16 *slot = sub_02010EE0(w, 0);
        sub_02010F84((void *)a5, t[0x0C], t[0x0D], *(u16 *)(t + 0x0A), a4, *(s16 *)((u8 *)slot + 0x300), 0, *(s16 *)((u8 *)slot + 0x480), 0xC0, *(u32 *)(w + 0x20));
    }

    if (*(u16 *)(t + 0x0A) == 0) {
        sub_02011068((void *)*(u32 *)(w + 0x24), 1, a4, *(u32 *)(w + 0x20));
    } else {
        sub_02011068((void *)*(u32 *)(w + 0x24), 2, a4, *(u32 *)(w + 0x20));
    }

    sub_0200FF88((void *)*(u32 *)(w + 0x28), w, (int)sub_02010C38, a4, a7);
}

static int sub_020121A4(void *work) {
    u8 *w = work;
    s32 tick = *(s32 *)(w + 0x1C) + 1;
    *(s32 *)(w + 0x1C) = tick;
    if (tick >= *(s32 *)(w + 0x18)) {
        *(s32 *)(w + 0x1C) = 0;
        s32 count = *(s32 *)(w + 0x14) - 1;
        if (count > 0) {
            *(s32 *)(w + 0x14) = count;
            sub_02012290(w);
            sub_02012204(w);
            SysTask_CreateOnVWaitQueue(sub_02010F00, w, 0x3FF);
        } else {
            sub_0200FFB4((void *)*(u32 *)(w + 0x28), *(u32 *)(w + 0x08), *(u32 *)(w + 0x2C));
            return 1;
        }
    }
    return 0;
}

static void sub_020121F4(void *work) {
    u8 *w = work;
    Heap_Free(*(void **)(w + 0x0C));
    *(void **)(w + 0x0C) = 0;
}

static void sub_02012204(void *work) {
    u8 *w = work;
    s16 *slot;
    int i;

    slot = sub_02010EE0(w, 0);
    memset((u8 *)slot + 0x300, 0, 0x300);

    for (i = *(u32 *)(w + 0x10) - 1; i >= 0; i--) {
        sub_02012238(w, (u8 *)*(void **)(w + 0x0C) + i * 0x30);
    }
}

static void sub_02012238(void *work, void *entry) {
    FadeRectBand *band = entry;
    FadeScanlineBuffer *scanBuf = sub_02010EE0(work, 0);
    s32 y;
    s32 left = band->cur[0] / 128;
    s32 right = band->cur[2] / 128;
    s32 bottom = band->cur[3] / 128;

    for (y = band->cur[1] / 128; y < bottom; y++) {
        scanBuf->next[0][y] = left;
        scanBuf->next[1][y] = right;
    }
}

static void sub_02012290(void *work) {
    u8 *w = work;
    int i;

    for (i = 0; i < *(s32 *)(w + 0x10); i++) {
        sub_02010A8C(((FadeRectBand *)*(void **)(w + 0x0C))[i].cur, ((FadeRectBand *)*(void **)(w + 0x0C))[i].delta);
    }
}

// ============================================================
// Scan-line based symmetric fade
// ============================================================

static void sub_020122B8(FadeData *data, const void *table) {
    data->work = Heap_Alloc((enum HeapID)data->heapID, 0x38);
    memset(data->work, 0, 0x38);
    sub_02012358(data->work, table, data->field_04, data->field_08, data->field_10, (int)data->field_18, (int)data->field_1C, data->heapID);
    data->state++;
}

static int sub_020122F8(FadeData *data) {
    int ret = 0;
    u8 *work = data->work;

    switch (data->state) {
    case 1:
        if (sub_02012454(work) == 1) {
            sub_02010F34(*(u32 *)(work + 0x28), *(void **)(work + 0x30), data->field_10);
            data->state++;
        }
        break;
    case 2:
        sub_020124AC(work);
        sub_02010EC8(work);
        Heap_Free(data->work);
        data->work = 0;
        ret = 1;
        data->state++;
        break;
    case 3:
        ret = 1;
        break;
    default:
        GF_AssertFail();
        break;
    }
    return ret;
}

static void sub_02012358(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7) {
    u8 *w = work;
    const u8 *t = table;

    *(s32 *)(w + 0x0C) = 0;
    *(u32 *)(w + 0x10) = *(u16 *)t;
    *(s32 *)(w + 0x14) = *(u16 *)(t + 2) - *(u16 *)t;
    sub_02010E64(w, 2, a4, a7);
    *(s32 *)(w + 0x18) = duration;
    *(s32 *)(w + 0x1C) = 0;
    *(s32 *)(w + 0x20) = screen;
    *(s32 *)(w + 0x24) = 0;
    *(void **)(w + 0x30) = (void *)a5;
    *(u32 *)(w + 0x34) = a6;
    *(u32 *)(w + 0x2C) = a7;
    *(u32 *)(w + 0x28) = *(u16 *)(t + 6);

    sub_020125D4(w + 0x0C, *(s32 *)(w + 0x1C), *(s32 *)(w + 0x18));
    sub_020124B0(w);

    SysTask_CreateOnVWaitQueue(sub_02010F00, w, 0x3FF);

    {
        s16 *slot0 = sub_02010EE0(w, 0);
        s16 *slot1 = sub_02010EE0(w, 1);

        sub_02010F84((void *)a5, t[4], t[5], 0, a4, *(s16 *)((u8 *)slot0 + 0x300), 0, *(s16 *)((u8 *)slot0 + 0x480), 0xC0, *(u32 *)(w + 0x28));
        sub_02010F84((void *)a5, t[4], t[5], 1, a4, *(s16 *)((u8 *)slot1 + 0x300), 0, *(s16 *)((u8 *)slot1 + 0x480), 0xC0, *(u32 *)(w + 0x28));
    }

    sub_02011068((void *)a5, 3, a4, *(u32 *)(w + 0x28));
    sub_0200FF88((void *)*(u32 *)(w + 0x34), w, (int)sub_02010C38, a4, a7);
}

static int sub_02012454(void *work) {
    u8 *w = work;
    s32 tick = *(s32 *)(w + 0x24) + 1;
    *(s32 *)(w + 0x24) = tick;
    if (tick >= *(s32 *)(w + 0x20)) {
        *(s32 *)(w + 0x24) = 0;
        s32 nextStep = *(s32 *)(w + 0x1C) + 1;
        if (nextStep <= *(s32 *)(w + 0x18)) {
            *(s32 *)(w + 0x1C) = nextStep;
            sub_020125D4(w + 0x0C, *(s32 *)(w + 0x1C), *(s32 *)(w + 0x18));
            sub_020124B0(w);
            SysTask_CreateOnVWaitQueue(sub_02010F00, w, 0x3FF);
        } else {
            sub_0200FFB4((void *)*(u32 *)(w + 0x34), *(u32 *)(w + 0x08), *(u32 *)(w + 0x2C));
            return 1;
        }
    }
    return 0;
}

static void sub_020124AC(void *work) {
    (void)work;
}

static void sub_020124B0(void *work) {
    u8 *w = work;
    u16 angle;
    FadeScanlineBuffer *buf0;
    FadeScanlineBuffer *buf1;
    s32 val;
    int i;

    angle = *(s32 *)(w + 0x0C) % 0x3FFF;
    buf0 = sub_02010EE0(w, 0);
    buf1 = sub_02010EE0(w, 1);
    for (i = 0; i < 96; i++) {
        if (*(s32 *)(w + 0x0C) < 0x3FFF) {
            val = sub_020109D8(angle, 96 - i);
            if (val > 127) {
                val = 127;
            }
            buf0->next[0][191 - i] = 128 - val;
            buf0->next[1][191 - i] = 128;
            buf1->next[0][i] = 128;
            buf1->next[1][i] = val + 128;
        } else {
            buf0->next[0][191 - i] = 0;
            buf0->next[1][191 - i] = 128;
            buf1->next[0][i] = 128;
            buf1->next[1][i] = 255;
        }
    }
    for (i = 96; i < 192; i++) {
        if (*(s32 *)(w + 0x0C) < 0x3FFF) {
            buf0->next[0][191 - i] = 128;
            buf0->next[1][191 - i] = 128;
            buf1->next[0][i] = 128;
            buf1->next[1][i] = 128;
        } else {
            val = sub_020109D8(0x3FFF - angle, i - 96);
            if (val > 127) {
                val = 127;
            }
            buf0->next[0][191 - i] = 0;
            buf0->next[1][191 - i] = 128 - val;
            buf1->next[0][i] = val + 128;
            buf1->next[1][i] = 255;
        }
    }
}

static void sub_020125D4(void *a0, int a1, int a2) {
    s32 *p = a0;
    s32 v = p[2] * a1 / a2;
    p[0] = v + p[1];
}

// ============================================================
// Dual-screen symmetric fade
// ============================================================

static void sub_020125EC(FadeData *data, const void *table) {
    data->work = Heap_Alloc((enum HeapID)data->heapID, 0x38);
    memset(data->work, 0, 0x38);
    sub_0201268C(data->work, table, data->field_04, data->field_08, data->field_10, (int)data->field_18, (int)data->field_1C, data->heapID);
    data->state++;
}

static int sub_0201262C(FadeData *data) {
    int ret = 0;
    u8 *work = data->work;

    switch (data->state) {
    case 1:
        if (sub_0201275C(work) == 1) {
            sub_02010F34(*(u32 *)(work + 0x28), *(void **)(work + 0x30), data->field_10);
            data->state++;
        }
        break;
    case 2:
        sub_020127B4(work);
        sub_02010EC8(work);
        Heap_Free(data->work);
        data->work = 0;
        ret = 1;
        data->state++;
        break;
    case 3:
        ret = 1;
        break;
    default:
        GF_AssertFail();
        break;
    }
    return ret;
}

static void sub_0201268C(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7) {
    u8 *w = work;
    const u8 *t = table;

    *(u32 *)(w + 0x0C) = *(u16 *)t;
    *(u32 *)(w + 0x10) = *(u16 *)t;
    *(s32 *)(w + 0x14) = *(u16 *)(t + 2) - *(u16 *)t;
    sub_02010E64(w, 2, a4, a7);
    *(s32 *)(w + 0x18) = duration;
    *(s32 *)(w + 0x1C) = 0;
    *(s32 *)(w + 0x20) = screen;
    *(s32 *)(w + 0x24) = 0;
    *(void **)(w + 0x30) = (void *)a5;
    *(u32 *)(w + 0x34) = a6;
    *(u32 *)(w + 0x2C) = a7;
    *(u32 *)(w + 0x28) = *(u16 *)(t + 6);

    sub_020127B8(w);

    SysTask_CreateOnVWaitQueue(sub_02010F00, w, 0x3FF);

    sub_02010EE0(w, 0);
    sub_02010EE0(w, 1);

    sub_02010F84((void *)a5, t[4], t[5], 0, a4, 0, 0, 0xFF, 0xC0, *(u32 *)(w + 0x28));
    sub_02010F84((void *)a5, t[4], t[5], 1, a4, 0, 0, 0xFF, 0xC0, *(u32 *)(w + 0x28));

    sub_02011068((void *)a5, 3, a4, *(u32 *)(w + 0x28));
    sub_0200FF88((void *)*(u32 *)(w + 0x34), w, (int)sub_02010C38, a4, a7);
}

static int sub_0201275C(void *work) {
    u8 *w = work;
    s32 tick = *(s32 *)(w + 0x24) + 1;
    *(s32 *)(w + 0x24) = tick;
    if (tick >= *(s32 *)(w + 0x20)) {
        *(s32 *)(w + 0x24) = 0;
        s32 nextStep = *(s32 *)(w + 0x1C) + 1;
        if (nextStep <= *(s32 *)(w + 0x18)) {
            *(s32 *)(w + 0x1C) = nextStep;
            sub_02012884(w + 0x0C, *(s32 *)(w + 0x1C), *(s32 *)(w + 0x18));
            sub_020127B8(w);
            SysTask_CreateOnVWaitQueue(sub_02010F00, w, 0x3FF);
        } else {
            sub_0200FFB4((void *)*(u32 *)(w + 0x34), *(u32 *)(w + 0x08), *(u32 *)(w + 0x2C));
            return 1;
        }
    }
    return 0;
}

static void sub_020127B4(void *work) {
    (void)work;
}

static void sub_020127B8(void *work) {
    u8 *w = work;
    FadeScanlineBuffer *buf0;
    FadeScanlineBuffer *buf1;
    u16 angle;
    s32 a, b;
    int i;

    angle = *(s32 *)(w + 0x0C);
    buf0 = sub_02010EE0(w, 0);
    buf1 = sub_02010EE0(w, 1);
    for (i = 0; i < 96; i++) {
        a = sub_020109D8(angle, 96 - i);
        b = sub_020109D8(0x3FFF - angle, 96 - i);
        if (a > 127) {
            a = 127;
        }
        if (b > 127) {
            b = 127;
        }
        buf0->next[0][i] = (s16)(128 - b);
        buf0->next[1][i] = (s16)(128 - a);
        buf0->next[0][191 - i] = (s16)(128 - b);
        buf0->next[1][191 - i] = (s16)(128 - a);
        buf1->next[0][i] = (s16)(a + 128);
        buf1->next[1][i] = (s16)(b + 128);
        buf1->next[0][191 - i] = (s16)(a + 128);
        buf1->next[1][191 - i] = (s16)(b + 128);
    }
}

static void sub_02012884(void *a0, int a1, int a2) {
    s32 *p = a0;
    s32 v = p[2] * a1 / a2;
    p[0] = v + p[1];
}

// ============================================================
// Scan-line pattern fade
// ============================================================

static void sub_0201289C(FadeData *data, const void *table) {
    data->work = Heap_Alloc((enum HeapID)data->heapID, 0xCD * 4);
    memset(data->work, 0, 0xCD * 4);
    sub_02012940(data->work, table, data->field_04, data->field_08, data->field_10, (int)data->field_18, (int)data->field_1C, data->heapID);
    data->state++;
}

static int sub_020128E0(FadeData *data) {
    int ret = 0;
    u8 *work = data->work;

    switch (data->state) {
    case 1:
        if (sub_02012A2C(work) == 1) {
            sub_02010F34(*(u32 *)(work + 0xC9 * 4), *(void **)(work + 0xC9 * 4 + 8), data->field_10);
            data->state++;
        }
        break;
    case 2:
        sub_02012A8C(work);
        Heap_Free(data->work);
        data->work = 0;
        ret = 1;
        data->state++;
        break;
    case 3:
        ret = 1;
        break;
    default:
        GF_AssertFail();
        break;
    }
    return ret;
}

static void sub_02012940(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7) {
    u8 *w = work;
    const u8 *t = table;

    sub_02011080(w, a4, 1, 0, 0);

    if (*(u16 *)(t + 6) == 0) {
        memset(w, 1, 0xC0);
        memset(w + 0xC0, 1, 0xC0);
    } else {
        memset(w, 0, 0xC0);
        memset(w + 0xC0, 0, 0xC0);
    }

    *(u32 *)(w + 0xC3 * 4) = *(u32 *)t;
    *(u32 *)(w + 0xC3 * 4 + 4) = *(u16 *)(t + 4);
    *(u32 *)(w + 0xC3 * 4 + 0x18) = *(u16 *)(t + 6);
    *(u32 *)(w + 0xC3 * 4 + 0x1C) = a7;
    *(u32 *)(w + 0xC3 * 4 + 0x08) = duration;
    *(u32 *)(w + 0xC3 * 4 + 0x0C) = 0;
    *(u32 *)(w + 0xC3 * 4 + 0x10) = screen;
    *(u32 *)(w + 0xC3 * 4 + 0x14) = 0;
    *(u32 *)(w + 0xC3 * 4 + 0x20) = a5;
    *(u32 *)(w + 0xC3 * 4 + 0x24) = a6;

    sub_020110DC((void *)a6, w, a7);

    if (*(u16 *)(t + 6) == 1) {
        sub_02010F84((void *)a5, 0x20, 0x3f, 0, a4, 0, 0, 0, 0, *(u16 *)(t + 6));
    } else {
        sub_02010F84((void *)a5, 0x3f, 0x20, 0, a4, 0, 0, 0, 0, *(u16 *)(t + 6));
    }

    sub_02011068((void *)a5, 1, a4, *(u32 *)(w + 0xC9 * 4));
}

static int sub_02012A2C(void *work) {
    u8 *w = work;

    if (++*(s32 *)(w + 0x320) >= *(s32 *)(w + 0x31C)) {
        *(s32 *)(w + 0x320) = 0;
        if (*(s32 *)(w + 0x318) + 1 <= *(s32 *)(w + 0x314)) {
            (*(s32 *)(w + 0x318))++;
            sub_02012A90(w);
            sub_020110C4(w);
        } else {
            sub_020110F4((void *)*(u32 *)(w + 0x330), w, *(u32 *)(w + 0x328));
            return TRUE;
        }
    }
    return FALSE;
}

static void sub_02012A8C(void *work) {
    (void)work;
}

static void sub_02012A90(void *work) {
    u8 *w = work;
    int i;

    for (i = 0; i < *(s32 *)(w + 0x310); i++) {
        sub_02012ACC((const u8 *)*(u32 *)(w + 0x30C) + i * 4, w, *(u32 *)(w + 0x318), *(u32 *)(w + 0x314));
    }
}

static void sub_02012ACC(const void *param, void *scanBuf, int progress, int total) {
    const u8 *p = param;
    u8 *buf = scanBuf;
    u8 start = p[0];
    u8 end = p[1];
    int current;
    int lo, hi;
    int fill;

    current = (end - start) * progress / total;
    current += start;
    if (start <= end) {
        lo = start;
        hi = end;
        fill = *(u16 *)(p + 2);
    } else {
        lo = end;
        hi = start;
        if (*(u16 *)(p + 2) == 0) {
            fill = TRUE;
        } else {
            fill = FALSE;
        }
    }
    for (; lo < hi; lo++) {
        if (lo == current) {
            if (fill == FALSE) {
                fill = TRUE;
            } else {
                fill = FALSE;
            }
        }
        buf[lo] = fill;
    }
}

// ============================================================
// Two-pass scan-line pattern fade
// ============================================================

static void sub_02012B1C(FadeData *data, const void *table) {
    const u8 *t = table;
    void *work;

    data->work = Heap_Alloc((enum HeapID)data->heapID, 0xE2 * 4);
    memset(data->work, 0, 0xE2 * 4);
    work = data->work;
    if (t[0xB] == 0) {
        sub_02012BE8(work, t, data->field_04, data->field_08, data->field_10, (int)data->field_18, (int)data->field_1C, data->heapID);
    } else {
        sub_02012CDC(work, t, data->field_04, data->field_08, data->field_10, (int)data->field_18, (int)data->field_1C, data->heapID);
    }
    data->state++;
}

static int sub_02012B80(FadeData *data) {
    int ret = 0;
    u8 *work = data->work;
    int done;

    switch (data->state) {
    case 1:
        if (*(u8 *)(work + 0x386) == 0) {
            done = sub_02012C68(work, data);
        } else {
            done = sub_02012D4C(work, data);
        }
        if (done == TRUE) {
            sub_02010F34(data->field_28, data->field_18, data->field_10);
            data->state++;
        }
        break;
    case 2:
        Heap_Free(work);
        data->work = 0;
        ret = 1;
        data->state++;
        break;
    case 3:
        ret = 1;
        break;
    default:
        GF_AssertFail();
        break;
    }
    return ret;
}

static void sub_02012BE8(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7) {
    u8 *w = work;
    const u8 *t = table;
    s32 offset;

    offset = FX_Whole(FX_Mul(duration << FX32_SHIFT, *(s32 *)(t + 0x14)));
    w[0x384] = duration - offset;
    *(const u8 **)(w + 0x380) = t;
    w[0x386] = t[0xB];
    sub_020117A0(w, t, offset, screen, a4, a5);
    if (t[8] == 0) {
        sub_02011068((void *)a5, 1, a4, t[0xB]);
    } else {
        sub_02011068((void *)a5, 2, a4, t[0xB]);
    }
    w[0x385] = 0;
}

static int sub_02012C68(void *work, FadeData *data) {
    u8 *w = work;
    int ret = 0;
    const u8 *t;

    switch (w[0x385]) {
    case 0:
        if (sub_020117FC(w) == 1) {
            w[0x385]++;
            t = *(const u8 **)(w + 0x380);
            sub_02012940(w + 0x4C, t + 0x0C, w[0x384], data->field_08, data->field_10, (int)data->field_18, (int)data->field_1C, (int)data->heapID);
        }
        break;
    case 1:
        if (sub_02012A2C(w + 0x4C) == 1) {
            ret = 1;
            w[0x385]++;
        }
        break;
    case 2:
        ret = 1;
        break;
    }
    return ret;
}

static void sub_02012CDC(void *work, const void *table, int duration, int screen, int a4, int a5, int a6, int a7) {
    u8 *w = work;
    const u8 *t = table;
    int steps;

    w[0x384] = FX_Whole(FX_Mul(duration << FX32_SHIFT, *(s32 *)(t + 0x14)));
    steps = duration - w[0x384];
    *(const u8 **)(w + 0x380) = t;
    w[0x386] = t[0xB];
    sub_02012940(w + 0x4C, *(const u8 **)(w + 0x380) + 0x0C, steps, screen, a4, a5, a6, a7);
    w[0x385] = 0;
}

static int sub_02012D4C(void *work, FadeData *data) {
    u8 *w = work;
    int ret = 0;
    const u8 *t;

    switch (w[0x385]) {
    case 0:
        if (sub_02012A2C(w + 0x4C) == 1) {
            w[0x385]++;
            sub_020117A0(w, *(const u8 **)(w + 0x380), w[0x384], data->field_08, data->field_10, (int)data->field_18);
            t = *(const u8 **)(w + 0x380);
            if (t[8] == 0) {
                sub_02011068((void *)(int)data->field_18, 1, data->field_10, t[0xB]);
            } else {
                sub_02011068((void *)(int)data->field_18, 2, data->field_10, t[0xB]);
            }
        }
        break;
    case 1:
        if (sub_020117FC(w) == 1) {
            ret = 1;
            w[0x385]++;
        }
        break;
    case 2:
        ret = 1;
        break;
    }
    return ret;
}
