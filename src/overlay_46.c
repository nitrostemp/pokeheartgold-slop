#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "game_stats.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gf_rtc.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "player_data.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"
#include "yes_no_prompt.h"

typedef struct Ov46MsgWindow {
    MessageFormat *msgFmt; // 0x00
    MsgData *msgData;      // 0x04
    Window window;         // 0x08
    String *buf;           // 0x18
    String *tmpBuf;        // 0x1C
    int frameType;         // 0x20
    WaitingIcon *icon;     // 0x24
    int textDelay;         // 0x28
    int printerId;         // 0x2C
} Ov46MsgWindow;           // size: 0x30

typedef struct Ov46PalAnim {
    SysTask *task;       // 0x000
    BOOL active;         // 0x004
    u16 base[4][16];     // 0x008
    u16 blended[21][16]; // 0x088
    s16 index;           // 0x328
    s8 dir;              // 0x32A
    u8 frameToggle;      // 0x32B
    u32 screen;          // 0x32C
} Ov46PalAnim;           // size: 0x330

typedef struct Ov46Work {
    SaveData *saveData;       // 0x000
    u8 unk004[4];             // 0x004
    int timer;                // 0x008
    BgConfig *bgConfig;       // 0x00C
    Ov46MsgWindow titleWin;   // 0x010
    Ov46MsgWindow msgWin;     // 0x040
    Ov46MsgWindow waitWin;    // 0x070
    Ov46MsgWindow errWin;     // 0x0A0
    YesNoPrompt *yesNoPrompt; // 0x0D0
    Ov46PalAnim palAnim;      // 0x0D4
} Ov46Work;                   // size: 0x404

typedef struct Ov46Args {
    SaveData *saveData; // 0x00
    void *lobby;        // 0x04
    BOOL skipPrompt;    // 0x08
    s64 *connectTime;   // 0x0C
    BOOL connected;     // 0x10
} Ov46Args;

s32 ov45_0222A5C0(void *lobby);
void ov45_0222B244(void *lobby);
void ov45_0222B270(void *lobby);
int ov45_0222D7CC(u32 a0, u32 a1);
BOOL ov45_0222D7FC(u32 a0, u32 a1);
int ov45_0222E7CC(void);
u32 ov45_0222E7FC(int a0);
void ov45_0222ED7C(void);
BOOL ov45_0222EDA8(void);

BOOL ov46_02258800(OverlayManager *man, int *state);
BOOL ov46_0225892C(OverlayManager *man, int *state);
BOOL ov46_02258C38(OverlayManager *man, int *state);
BOOL ov46_02258CB4(OverlayManager *man, int *state);
BOOL ov46_02258DA8(OverlayManager *man, int *state);
BOOL ov46_02258EFC(OverlayManager *man, int *state);
static void ov46_02258F70(void *data);
static void ov46_02258F78(Ov46Work *work, enum HeapID heapID);
static void ov46_02259210(SysTask *task, void *data);
static void ov46_022592B8(Ov46Work *work);
static void ov46_022592E0(Ov46Work *work);
static void ov46_022592EC(Ov46MsgWindow *win, BgConfig *bgConfig, int frameType, int msgBank, int x, int y, int width, int height, int baseTile, SaveData *saveData, enum HeapID heapID);
static void ov46_02259374(Ov46MsgWindow *win, int msgId);
static void ov46_022593F8(Ov46MsgWindow *win);
static void ov46_02259450(Ov46MsgWindow *win);
static void ov46_02259474(Ov46MsgWindow *win);
static void ov46_02259494(Ov46MsgWindow *win);
static void ov46_022594E0(Ov46MsgWindow *win, int msgId);
static void ov46_02259534(Ov46MsgWindow *win, int num);
static YesNoPrompt *ov46_02259550(BgConfig *bgConfig, int tileStart);

static const BgTemplate ov46_022595DC[3] = {
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xe800,
     .charBase = GX_BG_CHARBASE_0x00000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 1,
     .areaOver = GX_BG_AREAOVER_XLU,
     .dummy = 0,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xe000,
     .charBase = GX_BG_CHARBASE_0x10000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 0,
     .areaOver = GX_BG_AREAOVER_XLU,
     .dummy = 0,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xe800,
     .charBase = GX_BG_CHARBASE_0x00000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 0,
     .areaOver = GX_BG_AREAOVER_XLU,
     .dummy = 0,
     .mosaic = FALSE,
     },
};

static const GraphicsBanks ov46_022595B4 = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_128_B,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_NONE,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_NONE,
    .texpltt = GX_VRAM_TEXPLTT_NONE,
};

static const GraphicsModes ov46_022595A4 = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const u32 ov46_02259598[3] = { GF_BG_LYR_MAIN_0, GF_BG_LYR_MAIN_1, GF_BG_LYR_SUB_0 };

BOOL ov46_02258800(OverlayManager *man, int *state) {
    Ov46Work *work;
    Ov46Args *args;

    Heap_Create(HEAP_ID_3, HEAP_ID_119, 0x20000);
    work = OverlayManager_CreateAndGetData(man, sizeof(Ov46Work), HEAP_ID_119);
    memset(work, 0, sizeof(Ov46Work));
    args = OverlayManager_GetArgs(man);
    work->saveData = args->saveData;
    Sound_SetSceneAndPlayBGM(11, SEQ_GS_WIFI_ACCESS, 0);
    args->connected = FALSE;
    ov46_02258F78(work, HEAP_ID_119);
    ov46_022592EC(&work->msgWin, work->bgConfig, 1, 0x30A, 2, 0x13, 0x1B, 4, 0x28, work->saveData, HEAP_ID_119);
    ov46_022592EC(&work->waitWin, work->bgConfig, 1, 0x320, 2, 0x13, 0x1B, 4, 0x28, work->saveData, HEAP_ID_119);
    ov46_022592EC(&work->errWin, work->bgConfig, 0, 0x320, 4, 4, 0x17, 0x10, 0x94, work->saveData, HEAP_ID_119);
    ov46_022592EC(&work->titleWin, work->bgConfig, 1, 0x30A, 5, 1, 0x16, 2, 0x204, work->saveData, HEAP_ID_119);
    ov46_022594E0(&work->titleWin, 0x15);
    Main_SetVBlankIntrCB(ov46_02258F70, work);
    HBlankInterruptDisable();
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    return TRUE;
}

BOOL ov46_0225892C(OverlayManager *man, int *state) {
    int input;
    int msgId;
    u32 errCode;
    u32 *err;
    Ov46Work *work = OverlayManager_GetData(man);
    Ov46Args *args = OverlayManager_GetArgs(man);

    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0xFFFF, 6, 1, HEAP_ID_119);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            (*state)++;
        }
        break;
    case 2:
        if (args->skipPrompt == FALSE) {
            ov46_02259374(&work->waitWin, 0x11);
            work->yesNoPrompt = ov46_02259550(work->bgConfig, 0x230);
            (*state)++;
        } else {
            *state = 4;
        }
        break;
    case 3:
        input = YesNoPrompt_HandleInput(work->yesNoPrompt);
        if (input == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(work->yesNoPrompt);
            *state = 4;
        } else if (input == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(work->yesNoPrompt);
            ov46_022593F8(&work->msgWin);
            *state = 7;
        }
        break;
    case 4:
        sub_0203976C(work->saveData, ov45_0222A5C0(args->lobby));
        ov46_02259374(&work->waitWin, 0x17);
        ov46_02259450(&work->waitWin);
        (*state)++;
        break;
    case 5:
        if (sub_020393C8() || sub_020397FC()) {
            ov46_02259474(&work->waitWin);
            *state = 9;
        }
        if (sub_020397E4() == 1) {
            ov45_0222B244(args->lobby);
            (*state)++;
        }
        break;
    case 6:
        if (sub_020393C8() || sub_020397FC()) {
            ov46_02259474(&work->waitWin);
            *state = 9;
        }
        if (sub_02039274()) {
            ov46_02259474(&work->waitWin);
            GameStats_AddScore(Save_GameStats_Get(work->saveData), SCORE_EVENT_WIFI_PLAZA_ACCESSED);
            *args->connectTime = GF_RTC_DateTimeToSec();
            args->connected = TRUE;
            (*state)++;
        }
        break;
    case 9:
        if (sub_020393C8()) {
            err = sub_020392D8();
            msgId = ov45_0222D7CC(err[0], err[1]);
            errCode = err[0];
        } else {
            errCode = ov45_0222E7FC(ov45_0222E7CC());
            msgId = 0x20;
        }
        ov45_0222B270(args->lobby);
        ov46_022593F8(&work->msgWin);
        ov46_022593F8(&work->waitWin);
        ov46_02259534(&work->errWin, errCode);
        ov46_02259374(&work->errWin, msgId);
        (*state)++;
        break;
    case 10:
        if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || System_GetTouchNew() == TRUE) {
            if (sub_020393C8()) {
                err = sub_020392D8();
                if (ov45_0222D7FC(err[0], err[1]) == FALSE) {
                    *state = 11;
                } else {
                    *state = 14;
                }
            } else {
                *state = 11;
            }
        }
        break;
    case 11:
        ov46_022593F8(&work->errWin);
        ov46_02259374(&work->msgWin, 0x58);
        work->yesNoPrompt = ov46_02259550(work->bgConfig, 0x230);
        (*state)++;
        break;
    case 12:
        input = YesNoPrompt_HandleInput(work->yesNoPrompt);
        if (input == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(work->yesNoPrompt);
            sub_020397C8();
            *state = 13;
        } else if (input == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(work->yesNoPrompt);
            *state = 14;
        }
        break;
    case 13:
        if (!sub_02037D78()) {
            *state = 4;
        }
        break;
    case 14:
        ov46_022593F8(&work->msgWin);
        ov46_022593F8(&work->waitWin);
        ov46_022593F8(&work->errWin);
        sub_020397C8();
        (*state)++;
        break;
    case 15:
        if (!sub_02037D78()) {
            ov46_022593F8(&work->msgWin);
            ov46_022593F8(&work->waitWin);
            *state = 7;
        }
        break;
    case 7:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_119);
        (*state)++;
        break;
    case 8:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return FALSE;
}

BOOL ov46_02258C38(OverlayManager *man, int *state) {
    Ov46Work *work = OverlayManager_GetData(man);

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    if (work->palAnim.task != NULL) {
        SysTask_Destroy(work->palAnim.task);
        work->palAnim.task = NULL;
        work->palAnim.active = FALSE;
    }
    ov46_02259494(&work->msgWin);
    ov46_02259494(&work->waitWin);
    ov46_02259494(&work->errWin);
    ov46_02259494(&work->titleWin);
    ov46_022592B8(work);
    work->palAnim.active = FALSE;
    Heap_Free(work);
    Heap_Destroy(HEAP_ID_119);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    return TRUE;
}

BOOL ov46_02258CB4(OverlayManager *man, int *state) {
    Ov46Work *work;

    Heap_Create(HEAP_ID_3, HEAP_ID_119, 0x20000);
    work = OverlayManager_CreateAndGetData(man, sizeof(Ov46Work), HEAP_ID_119);
    memset(work, 0, sizeof(Ov46Work));
    work->saveData = ((Ov46Args *)OverlayManager_GetArgs(man))->saveData;
    ov46_02258F78(work, HEAP_ID_119);
    ov46_022592EC(&work->msgWin, work->bgConfig, 1, 0x320, 2, 0x13, 0x1B, 4, 0x28, work->saveData, HEAP_ID_119);
    ov46_022592EC(&work->errWin, work->bgConfig, 0, 0x320, 4, 4, 0x17, 0x10, 0x94, work->saveData, HEAP_ID_119);
    ov46_022592EC(&work->titleWin, work->bgConfig, 1, 0x30A, 5, 1, 0x16, 2, 0x204, work->saveData, HEAP_ID_119);
    ov46_022594E0(&work->titleWin, 0x15);
    sub_0203A880();
    Main_SetVBlankIntrCB(ov46_02258F70, work);
    HBlankInterruptDisable();
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    return TRUE;
}

BOOL ov46_02258DA8(OverlayManager *man, int *state) {
    Ov46Work *work = OverlayManager_GetData(man);
    Ov46Args *args = OverlayManager_GetArgs(man);

    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0xFFFF, 6, 1, HEAP_ID_119);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            *state = 2;
        }
        break;
    case 2:
        ov46_02259374(&work->msgWin, 0x1A);
        if (sub_020393C8() || sub_020397FC()) {
            *state = 5;
            ov46_02259450(&work->msgWin);
        } else {
            *state = 3;
            ov46_02259450(&work->msgWin);
        }
        break;
    case 3:
        ov45_0222ED7C();
        work->timer = 900;
        *state = 4;
        break;
    case 4:
        work->timer--;
        if (ov45_0222EDA8() || work->timer == 0) {
            *state = 5;
        }
        break;
    case 5:
        sub_020397C8();
        *state = 6;
        break;
    case 6:
        if (!sub_02037D78()) {
            ov46_02259474(&work->waitWin);
            ov45_0222B270(args->lobby);
            *state = 7;
        }
        break;
    case 7:
        ov46_02259374(&work->msgWin, 0x1B);
        work->timer = 90;
        *state = 8;
        break;
    case 8:
        if (--work->timer == 0) {
            *state = 9;
        }
        break;
    case 9:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_119);
        (*state)++;
        break;
    case 10:
        if (IsPaletteFadeFinished()) {
            ov46_02259474(&work->msgWin);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL ov46_02258EFC(OverlayManager *man, int *state) {
    Ov46Work *work = OverlayManager_GetData(man);

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    if (work->palAnim.task != NULL) {
        SysTask_Destroy(work->palAnim.task);
        work->palAnim.task = NULL;
        work->palAnim.active = FALSE;
    }
    ov46_02259494(&work->msgWin);
    ov46_02259494(&work->errWin);
    ov46_02259494(&work->titleWin);
    ov46_022592B8(work);
    work->palAnim.active = FALSE;
    Heap_Free(work);
    Heap_Destroy(HEAP_ID_119);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    return TRUE;
}

static void ov46_02258F70(void *data) {
    ov46_022592E0(data);
}

static void ov46_02258F78(Ov46Work *work, enum HeapID heapID) {
    NNSG2dPaletteData *plttData;
    NARC *narc;
    int j;
    int k;
    int step;
    BOOL done;
    int i;
    u8 frame;
    void *pltt;

    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    GfGfx_SetBanks(&ov46_022595B4);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, 0);
    SetBothScreensModesAndDisable(&ov46_022595A4);
    work->bgConfig = BgConfig_Alloc(heapID);
    for (i = 0; i < 3; i++) {
        InitBgFromTemplate(work->bgConfig, ov46_02259598[i], &ov46_022595DC[i], GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(ov46_02259598[i], 0x20, 0, heapID);
        BgClearTilemapBufferAndCommit(work->bgConfig, ov46_02259598[i]);
    }
    frame = Options_GetFrame(Save_PlayerData_GetOptionsAddr(work->saveData));
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_10_OFFSET, heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_9_OFFSET, heapID);
    LoadUserFrameGfx1(work->bgConfig, GF_BG_LYR_MAIN_1, 0x1F, 12, 0, heapID);
    LoadUserFrameGfx2(work->bgConfig, GF_BG_LYR_MAIN_1, 1, 11, frame, heapID);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_8, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x120, heapID);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_8, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x120, heapID);
    GfGfxLoader_LoadCharData(NARC_a_0_8_8, 2, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadCharData(NARC_a_0_8_8, 11, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_8, 6, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_8, 12, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    narc = NARC_New(NARC_a_0_8_8, heapID);
    MI_CpuFill8(&work->palAnim, 0, sizeof(Ov46PalAnim));
    pltt = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &plttData, heapID);
    MIi_CpuCopy16(plttData->pRawData, work->palAnim.base[0], sizeof(work->palAnim.base));
    MIi_CpuCopy16(plttData->pRawData, work->palAnim.blended[0], sizeof(work->palAnim.base));
    Heap_Free(pltt);
    k = 0;
    for (j = 0; j < 3; j++) {
        step = 0;
        done = FALSE;
        while (TRUE) {
            GF_ASSERT(k < 21);
            for (i = 1; i < 16; i++) {
                BlendPalette(&work->palAnim.base[j][i], &work->palAnim.blended[k][i], 1, step >> 8, work->palAnim.base[0][(j + 1) * 16 + i]);
            }
            k++;
            if (done == TRUE) {
                break;
            }
            step += 0x300;
            if (step >= 0x1000) {
                step = 0x1000;
                done = TRUE;
            }
        }
    }
    DC_FlushRange(work->palAnim.blended, sizeof(work->palAnim.blended));
    work->palAnim.active = TRUE;
    work->palAnim.screen = 0;
    work->palAnim.task = SysTask_CreateOnVBlankQueue(ov46_02259210, &work->palAnim, 20);
    NARC_Delete(narc);
}

static void ov46_02259210(SysTask *task, void *data) {
    Ov46PalAnim *palAnim = data;

    if (palAnim->active) {
        palAnim->frameToggle ^= 1;
        if (!(palAnim->frameToggle & 1)) {
            if (palAnim->screen <= 1) {
                GX_LoadBGPltt(palAnim->blended[palAnim->index], 0, 0x20);
            }
            if (palAnim->screen == 0 || palAnim->screen == 2) {
                GXS_LoadBGPltt(palAnim->blended[palAnim->index], 0, 0x20);
            }
            if (palAnim->dir == 0) {
                palAnim->index++;
                if (palAnim->index >= 21) {
                    palAnim->index = 19;
                    palAnim->dir ^= 1;
                }
            } else {
                palAnim->index--;
                if (palAnim->index < 0) {
                    palAnim->index = 1;
                    palAnim->dir ^= 1;
                }
            }
        }
    }
}

static void ov46_022592B8(Ov46Work *work) {
    int i;

    for (i = 0; i < 3; i++) {
        FreeBgTilemapBuffer(work->bgConfig, ov46_02259598[i]);
    }
    Heap_Free(work->bgConfig);
}

static void ov46_022592E0(Ov46Work *work) {
    DoScheduledBgGpuUpdates(work->bgConfig);
}

static void ov46_022592EC(Ov46MsgWindow *win, BgConfig *bgConfig, int frameType, int msgBank, int x, int y, int width, int height, int baseTile, SaveData *saveData, enum HeapID heapID) {
    win->msgFmt = MessageFormat_New(heapID);
    win->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, msgBank, heapID);
    win->buf = String_New(0x100, heapID);
    win->tmpBuf = String_New(0x100, heapID);
    win->frameType = frameType;
    win->textDelay = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(saveData));
    win->printerId = 0;
    AddWindowParameterized(bgConfig, &win->window, GF_BG_LYR_MAIN_1, x, y, width, height, 9, baseTile);
}

static void ov46_02259374(Ov46MsgWindow *win, int msgId) {
    if (TextPrinterCheckActive(win->printerId)) {
        RemoveTextPrinter(win->printerId);
    }
    FillWindowPixelBuffer(&win->window, 15);
    ReadMsgDataIntoString(win->msgData, msgId, win->tmpBuf);
    StringExpandPlaceholders(win->msgFmt, win->buf, win->tmpBuf);
    AddTextPrinterParameterized(&win->window, 1, win->buf, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    if (win->frameType == 0) {
        DrawFrameAndWindow1(&win->window, TRUE, 0x1F, 12);
    } else {
        DrawFrameAndWindow2(&win->window, TRUE, 1, 11);
    }
    ScheduleWindowCopyToVram(&win->window);
}

static void ov46_022593F8(Ov46MsgWindow *win) {
    if (TextPrinterCheckActive(win->printerId)) {
        RemoveTextPrinter(win->printerId);
    }
    if (win->frameType == 0) {
        sub_0200E5D4(&win->window, TRUE);
        ClearWindowTilemapAndScheduleTransfer(&win->window);
        return;
    }
    if (win->icon != NULL) {
        ov46_02259474(win);
    }
    ClearFrameAndWindow2(&win->window, TRUE);
    ClearWindowTilemapAndScheduleTransfer(&win->window);
}

static void ov46_02259450(Ov46MsgWindow *win) {
    if (win->frameType == 1) {
        GF_ASSERT(win->icon == NULL);
        win->icon = WaitingIcon_New(&win->window, 1);
    }
}

static void ov46_02259474(Ov46MsgWindow *win) {
    if (win->frameType == 1) {
        GF_ASSERT(win->icon != NULL);
        sub_0200F450(win->icon);
        win->icon = NULL;
    }
}

static void ov46_02259494(Ov46MsgWindow *win) {
    if (TextPrinterCheckActive(win->printerId)) {
        RemoveTextPrinter(win->printerId);
    }
    if (win->icon != NULL) {
        ov46_02259474(win);
    }
    RemoveWindow(&win->window);
    String_Delete(win->tmpBuf);
    String_Delete(win->buf);
    DestroyMsgData(win->msgData);
    MessageFormat_Delete(win->msgFmt);
}

static void ov46_022594E0(Ov46MsgWindow *win, int msgId) {
    u32 x;

    FillWindowPixelBuffer(&win->window, 0);
    ReadMsgDataIntoString(win->msgData, msgId, win->tmpBuf);
    StringExpandPlaceholders(win->msgFmt, win->buf, win->tmpBuf);
    x = FontID_String_GetCenterAlignmentX(0, win->buf, 0, 0xB0);
    AddTextPrinterParameterizedWithColor(&win->window, 1, win->buf, x, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(15, 14, 0), NULL);
}

static void ov46_02259534(Ov46MsgWindow *win, int num) {
    BufferIntegerAsString(win->msgFmt, 0, num, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
}

static YesNoPrompt *ov46_02259550(BgConfig *bgConfig, int tileStart) {
    YesNoPromptTemplate template;
    YesNoPrompt *prompt = YesNoPrompt_Create(HEAP_ID_119);

    template.bgConfig = bgConfig;
    template.bgId = GF_BG_LYR_MAIN_1;
    template.tileStart = tileStart;
    template.plttSlot = 13;
    template.x = 25;
    template.y = 10;
    template.ignoreTouchFlag = FALSE;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(prompt, &template);
    return prompt;
}
