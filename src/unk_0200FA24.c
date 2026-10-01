#include <nitro/gx/gx_load.h>

#include "global.h"

#include "heap.h"
#include "screen_fade.h"
#include "sys_task_api.h"
#include "system.h"

typedef struct FadeScreenSettings {
    int type;           // 0x00
    int steps;          // 0x04
    int framesPerStep;  // 0x08
    int unk0C;          // 0x0C
    PMLCDTarget screen; // 0x10
    int unk14;          // 0x14
    void *plttWork;     // 0x18
    void *hblank;       // 0x1C
    enum HeapID heapID; // 0x20
    u16 color;          // 0x24
    int unk28;          // 0x28
    int unk2C;          // 0x2C
} FadeScreenSettings;   // size: 0x30

typedef void (*FadeHBlankFunc)(void *arg);

typedef struct FadeHBlankTasks {
    void *args[2];           // 0x00
    FadeHBlankFunc funcs[2]; // 0x08
    BOOL active[2];          // 0x10
} FadeHBlankTasks;           // size: 0x18

typedef struct PaletteFadeData {
    int mode;                      // 0x000
    BOOL screenActive[2];          // 0x004
    BOOL screenEnabled[2];         // 0x00C
    FadeScreenSettings screens[2]; // 0x014
    FadeHBlankTasks hblank;        // 0x074
    u8 plttWork[0xC0];             // 0x08C
    u16 active;                    // 0x14C
    u8 busy[2];                    // 0x14E
    u16 lastColor;                 // 0x150
} PaletteFadeData;                 // size: 0x154

typedef struct FadeHBlankStartArgs {
    FadeHBlankTasks *hblank;
    void *arg;
    FadeHBlankFunc func;
    int idx;
} FadeHBlankStartArgs;

typedef struct FadeHBlankStopArgs {
    FadeHBlankTasks *hblank;
    int idx;
} FadeHBlankStopArgs;

typedef BOOL (*FadeFunc)(FadeScreenSettings *settings);

BOOL FadeFunc_00(FadeScreenSettings *settings);
BOOL FadeFunc_01(FadeScreenSettings *settings);
BOOL FadeFunc_02(FadeScreenSettings *settings);
BOOL FadeFunc_03(FadeScreenSettings *settings);
BOOL FadeFunc_04(FadeScreenSettings *settings);
BOOL FadeFunc_05(FadeScreenSettings *settings);
BOOL FadeFunc_06(FadeScreenSettings *settings);
BOOL FadeFunc_07(FadeScreenSettings *settings);
BOOL FadeFunc_08(FadeScreenSettings *settings);
BOOL FadeFunc_09(FadeScreenSettings *settings);
BOOL FadeFunc_10(FadeScreenSettings *settings);
BOOL FadeFunc_11(FadeScreenSettings *settings);
BOOL FadeFunc_12(FadeScreenSettings *settings);
BOOL FadeFunc_13(FadeScreenSettings *settings);
BOOL FadeFunc_14(FadeScreenSettings *settings);
BOOL FadeFunc_15(FadeScreenSettings *settings);
BOOL FadeFunc_16(FadeScreenSettings *settings);
BOOL FadeFunc_17(FadeScreenSettings *settings);
BOOL FadeFunc_18(FadeScreenSettings *settings);
BOOL FadeFunc_19(FadeScreenSettings *settings);
BOOL FadeFunc_20(FadeScreenSettings *settings);
BOOL FadeFunc_21(FadeScreenSettings *settings);
BOOL FadeFunc_22(FadeScreenSettings *settings);
BOOL FadeFunc_23(FadeScreenSettings *settings);
BOOL FadeFunc_24(FadeScreenSettings *settings);
BOOL FadeFunc_25(FadeScreenSettings *settings);
BOOL FadeFunc_26(FadeScreenSettings *settings);
BOOL FadeFunc_27(FadeScreenSettings *settings);
BOOL FadeFunc_28(FadeScreenSettings *settings);
BOOL FadeFunc_29(FadeScreenSettings *settings);
BOOL FadeFunc_30(FadeScreenSettings *settings);
BOOL FadeFunc_31(FadeScreenSettings *settings);
BOOL FadeFunc_32(FadeScreenSettings *settings);
BOOL FadeFunc_33(FadeScreenSettings *settings);
BOOL FadeFunc_34(FadeScreenSettings *settings);
BOOL FadeFunc_35(FadeScreenSettings *settings);
BOOL FadeFunc_36(FadeScreenSettings *settings);
BOOL FadeFunc_37(FadeScreenSettings *settings);
BOOL FadeFunc_38(FadeScreenSettings *settings);
BOOL FadeFunc_39(FadeScreenSettings *settings);
BOOL FadeFunc_40(FadeScreenSettings *settings);
BOOL FadeFunc_41(FadeScreenSettings *settings);
BOOL FadeFunc_42(FadeScreenSettings *settings);

void sub_020131F4(int a0, PMLCDTarget screen);
void sub_02013424(void *work, int a1, PMLCDTarget screen);
void sub_02013440(void *work, int a1, int a2, int a3, PMLCDTarget screen);
void sub_02013468(void *work, int a1, int a2, PMLCDTarget screen);
void sub_02013488(void *work, int a1, int a2, int a3, int a4, int a5, PMLCDTarget screen);
void GXx_SetMasterBrightness_(vu16 *reg, int brightness);

void sub_0200FC20(u16 color);
void sub_0200FC60(PMLCDTarget screen, u16 color);
void sub_0200FCDC(u16 color);
void SetMasterBrightness(PMLCDTarget screen, int brightness);
void sub_0200FF88(FadeHBlankTasks *hblank, void *arg, FadeHBlankFunc func, int idx, enum HeapID heapID);
void sub_0200FFB4(FadeHBlankTasks *hblank, int idx, enum HeapID heapID);

static void HandleEndFade(PaletteFadeData *fade);
static BOOL DoFadeUpdateFrame(PaletteFadeData *fade, FadeScreenSettings *main, FadeScreenSettings *sub);
static void FadeWork_UpdateFrame(BOOL *active, FadeScreenSettings *settings);
static BOOL CallFadeFunc(FadeScreenSettings *settings);
static void sub_0200FE14(int mode, PaletteFadeData *fade);
static void sub_0200FE78(PaletteFadeData *fade, int mode, BOOL mainEnabled, BOOL subEnabled);
static void sub_0200FE84(FadeScreenSettings *settings, int type, int steps, int framesPerStep, int unk0C, int unk14, PMLCDTarget screen, void *plttWork, FadeHBlankTasks *hblank, enum HeapID heapID, u16 color);
static void sub_0200FEB0(FadeHBlankTasks *hblank);
static void sub_0200FECC(void *data);
static void sub_0200FEE4(FadeHBlankTasks *hblank, void *arg, FadeHBlankFunc func, int idx);
static void sub_0200FF5C(FadeHBlankTasks *hblank, int idx);
static void sub_0200FFD8(SysTask *task, void *data);
static void sub_0200FFF8(SysTask *task, void *data);
static void sub_02010014(void *arg);
static u16 sub_02010018(PaletteFadeData *fade, u16 color);
static u16 sub_0201002C(PaletteFadeData *fade);
static void sub_02010050(SysTask *task, void *data);
static void sub_02010064(FadeScreenSettings *settings);
static void sub_02010094(FadeScreenSettings *settings);
static void sub_020100C4(PaletteFadeData *fade);

static const FadeFunc sFadeFuncPtrs[] = {
    FadeFunc_00,
    FadeFunc_01,
    FadeFunc_02,
    FadeFunc_03,
    FadeFunc_04,
    FadeFunc_05,
    FadeFunc_06,
    FadeFunc_07,
    FadeFunc_08,
    FadeFunc_09,
    FadeFunc_10,
    FadeFunc_11,
    FadeFunc_12,
    FadeFunc_13,
    FadeFunc_14,
    FadeFunc_15,
    FadeFunc_16,
    FadeFunc_17,
    FadeFunc_18,
    FadeFunc_19,
    FadeFunc_20,
    FadeFunc_21,
    FadeFunc_22,
    FadeFunc_23,
    FadeFunc_24,
    FadeFunc_25,
    FadeFunc_26,
    FadeFunc_27,
    FadeFunc_28,
    FadeFunc_29,
    FadeFunc_30,
    FadeFunc_31,
    FadeFunc_32,
    FadeFunc_33,
    FadeFunc_34,
    FadeFunc_35,
    FadeFunc_36,
    FadeFunc_37,
    FadeFunc_38,
    FadeFunc_39,
    FadeFunc_40,
    FadeFunc_41,
    FadeFunc_42,
};

static PaletteFadeData sPaletteFade;

// NONMATCHING: retail has two literal-pool entries for &sPaletteFade (address form and member-access
// base form); the second screenEnabled check must load via the address entry, which MWCC only
// emits when it stops propagating the pointer local and keeps it in r4 instead. MWCC inline asm
// dedupes `ldr =` literals by value, so the asm version spells out the pool with dcd.
#ifdef NONMATCHING
void BeginNormalPaletteFade(enum FadeMode fadeMode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID) {
    PaletteFadeData *fade = &sPaletteFade;

    GF_ASSERT(steps);
    GF_ASSERT(framesPerStep);
    GF_ASSERT(sPaletteFade.active == 0);

    sub_020100C4(&sPaletteFade);
    sub_0200FE14(fadeMode, &sPaletteFade);
    sub_0200FEB0(&sPaletteFade.hblank);
    color = sub_02010018(&sPaletteFade, color);
    sub_0200FE84(&sPaletteFade.screens[0], typeMain, steps, framesPerStep, 0, 0, PM_LCD_TOP, sPaletteFade.plttWork, &sPaletteFade.hblank, heapID, color);
    sub_0200FE84(&sPaletteFade.screens[1], typeSub, steps, framesPerStep, 0, 0, PM_LCD_BOTTOM, sPaletteFade.plttWork, &sPaletteFade.hblank, heapID, color);
    sPaletteFade.active = 1;
    FadeWork_UpdateFrame(&sPaletteFade.screenActive[0], &sPaletteFade.screens[0]);
    FadeWork_UpdateFrame(&sPaletteFade.screenActive[1], &sPaletteFade.screens[1]);
    if (sPaletteFade.screenEnabled[0]) {
        sub_02010064(&fade->screens[0]);
        fade->busy[0] = TRUE;
    }
    if (sPaletteFade.screenEnabled[1]) {
        sub_02010064(&fade->screens[1]);
        fade->busy[1] = TRUE;
    }
}
#else
// clang-format off
asm void BeginNormalPaletteFade(enum FadeMode fadeMode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID) {
    push {r4, r5, r6, r7, lr}
    sub sp, #0x1c
    add r5, r0, #0
    ldr r0, [sp, #0x30]
    add r4, r1, #0
    add r7, r2, #0
    add r6, r3, #0
    cmp r0, #0
    bne _0200FA3A
    bl GF_AssertFail
_0200FA3A:
    ldr r0, [sp, #0x34]
    cmp r0, #0
    bne _0200FA44
    bl GF_AssertFail
_0200FA44:
    ldr r0, [pc, #0xb8]
    ldrh r0, [r0, #0xc]
    cmp r0, #0
    beq _0200FA50
    bl GF_AssertFail
_0200FA50:
    ldr r0, [pc, #0xb0]
    bl sub_020100C4
    ldr r1, [pc, #0xac]
    add r0, r5, #0
    bl sub_0200FE14
    ldr r0, [pc, #0xa8]
    bl sub_0200FEB0
    ldr r0, [pc, #0x9c]
    add r1, r6, #0
    bl sub_02010018
    add r5, r0, #0
    mov r0, #0
    str r0, [sp]
    str r0, [sp, #4]
    str r0, [sp, #8]
    ldr r0, [pc, #0x94]
    ldr r2, [sp, #0x30]
    str r0, [sp, #0xc]
    ldr r0, [pc, #0x88]
    ldr r3, [sp, #0x34]
    str r0, [sp, #0x10]
    ldr r0, [sp, #0x38]
    add r1, r4, #0
    str r0, [sp, #0x14]
    ldr r0, [pc, #0x84]
    str r5, [sp, #0x18]
    bl sub_0200FE84
    mov r0, #0
    str r0, [sp]
    str r0, [sp, #4]
    mov r0, #1
    str r0, [sp, #8]
    ldr r0, [pc, #0x70]
    ldr r2, [sp, #0x30]
    str r0, [sp, #0xc]
    ldr r0, [pc, #0x64]
    ldr r3, [sp, #0x34]
    str r0, [sp, #0x10]
    ldr r0, [sp, #0x38]
    add r1, r7, #0
    str r0, [sp, #0x14]
    ldr r0, [pc, #0x64]
    str r5, [sp, #0x18]
    bl sub_0200FE84
    ldr r0, [pc, #0x48]
    mov r1, #1
    strh r1, [r0, #0xc]
    ldr r0, [pc, #0x5c]
    ldr r1, [pc, #0x50]
    bl FadeWork_UpdateFrame
    ldr r0, [pc, #0x58]
    ldr r1, [pc, #0x4c]
    bl FadeWork_UpdateFrame
    ldr r0, [pc, #0x54]
    ldr r0, [r0, #0xc]
    cmp r0, #0
    beq _0200FAE2
    ldr r0, [pc, #0x30]
    add r0, #0x14
    bl sub_02010064
    ldr r1, [pc, #0x48]
    ldr r0, [pc, #0x24]
    mov r2, #1
    strb r2, [r0, r1]
_0200FAE2:
    ldr r0, [pc, #0x20]
    ldr r0, [r0, #0x10]
    cmp r0, #0
    beq _0200FAFA
    ldr r0, [pc, #0x18]
    add r0, #0x44
    bl sub_02010064
    ldr r1, [pc, #0x34]
    ldr r0, [pc, #0xc]
    mov r2, #1
    strb r2, [r0, r1]
_0200FAFA:
    add sp, #0x1c
    pop {r4, r5, r6, r7, pc}
    nop
    dcd sPaletteFade+0x140
    dcd sPaletteFade
    dcd sPaletteFade+0x74
    dcd sPaletteFade+0x8c
    dcd sPaletteFade+0x14
    dcd sPaletteFade+0x44
    dcd sPaletteFade+0x4
    dcd sPaletteFade+0x8
    dcd sPaletteFade
    dcd 0x0000014E
    dcd 0x0000014F
}
// clang-format on
#endif // NONMATCHING

void HandleFadeUpdateFrame(void) {
    PaletteFadeData *fade = &sPaletteFade;

    if (sPaletteFade.active) {
        if (DoFadeUpdateFrame(fade, &fade->screens[0], &fade->screens[1]) == TRUE) {
            HandleEndFade(fade);
        }
    }
}

BOOL IsPaletteFadeFinished(void) {
    if (sPaletteFade.active == 0) {
        return TRUE;
    }
    return FALSE;
}

void sub_0200FB70(void) {
    sub_0200FF5C(&sPaletteFade.hblank, 0);
    sub_0200FF5C(&sPaletteFade.hblank, 1);
    if (sPaletteFade.screenActive[0]) {
        sPaletteFade.screens[0].unk0C = 2;
    }
    if (sPaletteFade.screenActive[1]) {
        sPaletteFade.screens[1].unk0C = 2;
    }
    FadeWork_UpdateFrame(&sPaletteFade.screenActive[0], &sPaletteFade.screens[0]);
    FadeWork_UpdateFrame(&sPaletteFade.screenActive[1], &sPaletteFade.screens[1]);
    sPaletteFade.active = 0;
    sPaletteFade.busy[0] = FALSE;
    sPaletteFade.busy[1] = FALSE;
    sub_020100C4(&sPaletteFade);
}

void ResetVisibleHardwareWindows(PMLCDTarget screen) {
    sub_020131F4(0, screen);
}

void SetMasterBrightnessNeutral(PMLCDTarget screen) {
    SetMasterBrightness(screen, 0);
}

void sub_0200FBF4(PMLCDTarget screen, u16 color) {
    int brightness;

    if (color == 0xFFFF) {
        color = sPaletteFade.lastColor;
    }
    if (color == 0x7FFF) {
        brightness = 16;
    } else {
        brightness = -16;
    }
    SetMasterBrightness(screen, brightness);
}

void sub_0200FC20(u16 color) {
    int brightness;

    if (color == 0xFFFF) {
        color = sPaletteFade.lastColor;
    }
    if (color == 0x7FFF) {
        brightness = 16;
    } else {
        brightness = -16;
    }
    SetMasterBrightness(PM_LCD_TOP, brightness);
    SetMasterBrightness(PM_LCD_BOTTOM, brightness);
    sPaletteFade.lastColor = color;
}

void sub_0200FC60(PMLCDTarget screen, u16 color) {
    if (color == 0xFFFF) {
        color = sPaletteFade.lastColor;
    }
    if (screen == PM_LCD_TOP) {
        GX_LoadBGPltt(&color, 0, sizeof(u16));
    } else {
        GXS_LoadBGPltt(&color, 0, sizeof(u16));
    }
    sub_02013424(sPaletteFade.plttWork, 1, screen);
    sub_02013440(sPaletteFade.plttWork, 0x3F, 0, 0, screen);
    sub_02013488(sPaletteFade.plttWork, 0, 0, 0, 0, 0, screen);
    sub_02013468(sPaletteFade.plttWork, 0x20, 0, screen);
}

void sub_0200FCDC(u16 color) {
    GX_LoadBGPltt(&color, 0, sizeof(u16));
    GXS_LoadBGPltt(&color, 0, sizeof(u16));
}

void SetMasterBrightness(PMLCDTarget screen, int brightness) {
    if (screen == PM_LCD_TOP) {
        GXx_SetMasterBrightness_(&reg_GX_MASTER_BRIGHT, brightness);
    } else {
        GXx_SetMasterBrightness_(&reg_GXS_DB_MASTER_BRIGHT, brightness);
    }
}

static void HandleEndFade(PaletteFadeData *fade) {
    fade->active = 0;
    fade->lastColor = sub_0201002C(fade);
    if (fade->screenEnabled[0]) {
        sub_02010094(&fade->screens[0]);
        if (fade->screens[0].unk28 == 0) {
            sPaletteFade.busy[0] = FALSE;
        }
    }
    if (fade->screenEnabled[1]) {
        sub_02010094(&fade->screens[1]);
        if (fade->screens[0].unk28 == 0) {
            sPaletteFade.busy[1] = FALSE;
        }
    }
    sub_020100C4(fade);
}

static BOOL DoFadeUpdateFrame(PaletteFadeData *fade, FadeScreenSettings *main, FadeScreenSettings *sub) {
    switch (fade->mode) {
    case FADE_BOTH_SCREENS:
        FadeWork_UpdateFrame(&fade->screenActive[0], main);
        FadeWork_UpdateFrame(&fade->screenActive[1], sub);
        break;
    case FADE_MAIN_THEN_SUB:
        if (fade->screenActive[0]) {
            FadeWork_UpdateFrame(&fade->screenActive[0], main);
        } else {
            FadeWork_UpdateFrame(&fade->screenActive[1], sub);
        }
        break;
    case FADE_SUB_THEN_MAIN:
        if (fade->screenActive[1]) {
            FadeWork_UpdateFrame(&fade->screenActive[1], sub);
        } else {
            FadeWork_UpdateFrame(&fade->screenActive[0], main);
        }
        break;
    }
    if (fade->screenActive[0] == FALSE && fade->screenActive[1] == FALSE) {
        return TRUE;
    }
    return FALSE;
}

static void FadeWork_UpdateFrame(BOOL *active, FadeScreenSettings *settings) {
    if (*active) {
        if (CallFadeFunc(settings) == TRUE) {
            *active = FALSE;
        }
    }
}

static BOOL CallFadeFunc(FadeScreenSettings *settings) {
    return sFadeFuncPtrs[settings->type](settings);
}

static void sub_0200FE14(int mode, PaletteFadeData *fade) {
    switch (mode) {
    case FADE_BOTH_SCREENS:
        sub_0200FE78(fade, FADE_BOTH_SCREENS, TRUE, TRUE);
        break;
    case FADE_MAIN_THEN_SUB:
        sub_0200FE78(fade, FADE_MAIN_THEN_SUB, TRUE, TRUE);
        break;
    case FADE_SUB_THEN_MAIN:
        sub_0200FE78(fade, FADE_SUB_THEN_MAIN, TRUE, TRUE);
        break;
    case FADE_MAIN_ONLY:
        sub_0200FE78(fade, FADE_MAIN_THEN_SUB, TRUE, FALSE);
        break;
    case FADE_SUB_ONLY:
        sub_0200FE78(fade, FADE_SUB_THEN_MAIN, FALSE, TRUE);
        break;
    }
}

static void sub_0200FE78(PaletteFadeData *fade, int mode, BOOL mainEnabled, BOOL subEnabled) {
    fade->mode = mode;
    fade->screenActive[0] = mainEnabled;
    fade->screenActive[1] = subEnabled;
    fade->screenEnabled[0] = mainEnabled;
    fade->screenEnabled[1] = subEnabled;
}

static void sub_0200FE84(FadeScreenSettings *settings, int type, int steps, int framesPerStep, int unk0C, int unk14, PMLCDTarget screen, void *plttWork, FadeHBlankTasks *hblank, enum HeapID heapID, u16 color) {
    settings->type = type;
    settings->steps = steps;
    settings->framesPerStep = framesPerStep;
    settings->unk0C = unk0C;
    settings->unk14 = unk14;
    settings->screen = screen;
    settings->plttWork = plttWork;
    settings->hblank = hblank;
    settings->heapID = heapID;
    settings->color = color;
}

static void sub_0200FEB0(FadeHBlankTasks *hblank) {
    int i;

    for (i = 0; i < 2; i++) {
        hblank->args[i] = NULL;
        hblank->funcs[i] = sub_02010014;
        hblank->active[i] = FALSE;
    }
}

static void sub_0200FECC(void *data) {
    FadeHBlankTasks *hblank = data;
    int i;

    for (i = 0; i < 2; i++) {
        hblank->funcs[i](hblank->args[i]);
    }
}

static void sub_0200FEE4(FadeHBlankTasks *hblank, void *arg, FadeHBlankFunc func, int idx) {
    BOOL ok = TRUE;

    GF_ASSERT(hblank->active[idx] == FALSE);
    GF_ASSERT(hblank->funcs[idx] != NULL);
    if (hblank->active[0] == FALSE && hblank->active[1] == FALSE) {
        ok = (u8)Main_SetHBlankIntrCB(sub_0200FECC, hblank);
    }
    GF_ASSERT(ok == TRUE);
    hblank->args[idx] = arg;
    if (func != NULL) {
        hblank->funcs[idx] = func;
    } else {
        hblank->funcs[idx] = sub_02010014;
    }
    hblank->active[idx] = TRUE;
}

static void sub_0200FF5C(FadeHBlankTasks *hblank, int idx) {
    hblank->active[idx] = FALSE;
    if (hblank->active[0] == FALSE && hblank->active[1] == FALSE) {
        HBlankInterruptDisable();
    }
    hblank->funcs[idx] = sub_02010014;
    hblank->args[idx] = NULL;
}

void sub_0200FF88(FadeHBlankTasks *hblank, void *arg, FadeHBlankFunc func, int idx, enum HeapID heapID) {
    FadeHBlankStartArgs *args = Heap_AllocAtEnd(heapID, sizeof(FadeHBlankStartArgs));

    args->hblank = hblank;
    args->arg = arg;
    args->func = func;
    args->idx = idx;
    SysTask_CreateOnVWaitQueue(sub_0200FFD8, args, 1024);
}

void sub_0200FFB4(FadeHBlankTasks *hblank, int idx, enum HeapID heapID) {
    FadeHBlankStopArgs *args = Heap_AllocAtEnd(heapID, sizeof(FadeHBlankStopArgs));

    args->hblank = hblank;
    args->idx = idx;
    SysTask_CreateOnVWaitQueue(sub_0200FFF8, args, 1024);
}

static void sub_0200FFD8(SysTask *task, void *data) {
    FadeHBlankStartArgs *args = data;

    sub_0200FEE4(args->hblank, args->arg, args->func, args->idx);
    SysTask_Destroy(task);
    Heap_Free(data);
}

static void sub_0200FFF8(SysTask *task, void *data) {
    FadeHBlankStopArgs *args = data;

    sub_0200FF5C(args->hblank, args->idx);
    SysTask_Destroy(task);
    Heap_Free(data);
}

static void sub_02010014(void *arg) {
}

static u16 sub_02010018(PaletteFadeData *fade, u16 color) {
    if (color == 0xFFFF) {
        color = fade->lastColor;
    }
    return color;
}

static u16 sub_0201002C(PaletteFadeData *fade) {
    FadeScreenSettings *settings;

    if (fade->screenEnabled[0] == TRUE) {
        settings = &fade->screens[0];
    } else {
        settings = &fade->screens[1];
    }
    if (settings->unk28 == TRUE) {
        return settings->color;
    }
    return fade->lastColor;
}

static void sub_02010050(SysTask *task, void *data) {
    FadeScreenSettings *settings = data;

    SetMasterBrightness(settings->screen, 0);
    SysTask_Destroy(task);
}

static void sub_02010064(FadeScreenSettings *settings) {
    if (settings->unk28 == 0) {
        if ((settings->color == 0x7FFF || settings->color == 0) && settings->unk2C == 0) {
            SysTask_CreateOnVWaitQueue(sub_02010050, settings, 1024);
        }
    }
}

static void sub_02010094(FadeScreenSettings *settings) {
    if (settings->unk28 == 1) {
        if ((settings->color == 0x7FFF || settings->color == 0) && settings->unk2C == 0) {
            sub_0200FBF4(settings->screen, settings->color);
            ResetVisibleHardwareWindows(settings->screen);
        }
    }
}

static void sub_020100C4(PaletteFadeData *fade) {
    memset(fade, 0, 0x14);
    memset(&fade->screens[0], 0, sizeof(FadeScreenSettings));
    memset(&fade->screens[1], 0, sizeof(FadeScreenSettings));
    memset(&fade->hblank, 0, sizeof(FadeHBlankTasks));
    memset(fade->plttWork, 0, sizeof(fade->plttWork));
}
