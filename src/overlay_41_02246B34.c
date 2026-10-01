#include "global.h"

#include "constants/game_stats.h"

#include "bg_window.h"
#include "brightness.h"
#include "fashion_case.h"
#include "game_stats.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "menu_input_state.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_41_02248400.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pokemon.h"
#include "pokepic.h"
#include "render_text.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_020210A0.h"
#include "yes_no_prompt.h"

typedef struct FashionAppArgs {
    Pokemon *mon;                         // 0x00
    SaveFashionDataSub *portrait;         // 0x04
    FashionCase *fashionCase;             // 0x08
    Options *options;                     // 0x0C
    GameStats *gameStats;                 // 0x10
    PlayerProfile *profile;               // 0x14
    int *result;                          // 0x18
    int showIntro;                        // 0x1C
    MenuInputStateMgr *menuInputStateMgr; // 0x20
} FashionAppArgs;

typedef struct UnkOv41CharEntry {
    void *unk0;     // 0x0
    void *charData; // 0x4
} UnkOv41CharEntry;

typedef struct UnkOv41PlttEntry {
    void *unk0;              // 0x0
    NNSG2dPaletteData *pltt; // 0x4
    int unk8;                // 0x8
} UnkOv41PlttEntry;

typedef struct UnkOv41GfxTables {
    UnkOv41CharEntry *chars;    // 0x00
    int numChars;               // 0x04
    UnkOv41PlttEntry *pltts;    // 0x08
    int numPltts;               // 0x0C
    GF_2DGfxRawResMan *charMan; // 0x10
    GF_2DGfxRawResMan *plttMan; // 0x14
} UnkOv41GfxTables;

typedef struct UnkOv41Pool {
    void *nodes; // 0x0
    int count;   // 0x4
} UnkOv41Pool;

typedef struct UnkOv41CanvasTemplate {
    void *unk00;                    // 0x00
    void *unk04;                    // 0x04
    void *unk08;                    // 0x08
    void *unk0C;                    // 0x0C
    PokepicManager *pokepicManager; // 0x10
    BgConfig *bgConfig;             // 0x14
    void *unk18;                    // 0x18
    UnkOv41Pool *pool;              // 0x1C
    int maxObjs;                    // 0x20
} UnkOv41CanvasTemplate;

typedef struct UnkOv41BgScrollTemplate {
    BgConfig *bgConfig; // 0x00
    NarcId narcId;      // 0x04
    int charFile;       // 0x08
    int plttFile;       // 0x0C
    int scrnFile;       // 0x10
    int x;              // 0x14
    int y;              // 0x18
    int bgId;           // 0x1C
    int plttCount;      // 0x20
    int plttSlot;       // 0x24
    int tileOffset;     // 0x28
    enum HeapID heapID; // 0x2C
} UnkOv41BgScrollTemplate;

typedef struct UnkOv41PanelArgs {
    BgConfig *bgConfig;       // 0x00
    SpriteList *spriteList;   // 0x04
    GF_2DGfxResMan **resMans; // 0x08
    Options *options;         // 0x0C
    int count;                // 0x10
    int msgFile;              // 0x14
    int msgId;                // 0x18
    int unk1C;                // 0x1C
    int *unk20;               // 0x20
    NARC *narc;               // 0x24
} UnkOv41PanelArgs;

typedef struct FashionApp {
    void *unk000;                     // 0x000
    void *unk004;                     // 0x004
    u8 unk008[0x8];                   // 0x008
    void *unk010;                     // 0x010
    u8 unk014[0x8];                   // 0x014
    int unk01C;                       // 0x01C
    PokepicManager *pokepicManager;   // 0x020
    NNSGfdTexKey texKey;              // 0x024
    NNSGfdPlttKey plttKey;            // 0x028
    BOOL pokepicActive;               // 0x02C
    void *unk030;                     // 0x030
    void *unk034;                     // 0x034
    void *unk038;                     // 0x038
    u8 unk03C[0x4];                   // 0x03C
    BgConfig *bgConfig;               // 0x040
    SpriteList *spriteList;           // 0x044
    GF_2DGfxResMan *resMans[4];       // 0x048
    u8 unk058[0x128];                 // 0x058
    NARC *narc;                       // 0x180
    UnkOv41FashionTable fashionTable; // 0x184
    UnkOv41Pool pool;                 // 0x35C
    void *unk364;                     // 0x364
    UnkOv41Board board;               // 0x368
    u8 canvas[0x88];                  // 0x3F4
    u8 unk47C[0x1C];                  // 0x47C
    UnkOv41TouchCtx touchCtx;         // 0x498
    u8 scroll[0x2C];                  // 0x4B4
    u8 buttonBar[0x88];               // 0x4E0
    u8 panel[0x148];                  // 0x568
    int mode;                         // 0x6B0
    BOOL taskDone;                    // 0x6B4
    YesNoPrompt *yesNoPrompt;         // 0x6B8
    Window *window;                   // 0x6BC
    BOOL confirmed;                   // 0x6C0
    u8 unk6C4[0x18];                  // 0x6C4
    Options *options;                 // 0x6DC
    int printerId;                    // 0x6E0
    u8 unk6E4[0x8];                   // 0x6E4
    int menuInputState;               // 0x6EC
} FashionApp;                         // size: 0x6F0

typedef struct UnkOv41FadeTask {
    FashionApp *app; // 0x0
    int *done;       // 0x4
    int counter;     // 0x8
    int state;       // 0xC
} UnkOv41FadeTask;

void *ov41_02245EA0(int count, enum HeapID heapID);
void ov41_02245ECC(void *obj);
void ov41_02246130(FashionApp *app);
void ov41_02246150(void);
void ov41_02246170(FashionApp *app);
void ov41_022461D0(FashionApp *app);
void ov41_0224621C(FashionApp *app);
void ov41_02246250(FashionApp *app, UnkOv41GfxTables *tables);
void ov41_0224626C(FashionApp *app);
void ov41_022463B0(FashionApp *app, UnkOv41GfxTables *tables);
void ov41_022463D4(UnkOv41GfxTables *tables);
void *ov41_022463DC(FashionApp *app, void *file, int idx);
void ov41_02246494(void *app);
void ov41_02247828(FashionApp *app, int *done);
void ov41_022479A8(SaveFashionDataSub *portrait, void *canvas, PlayerProfile *profile);
BOOL ov41_02247A48(UnkOv41FadeTask *task, int dx, int dy, int frames);
void ov41_02247AB4(FashionApp *app);
int ov41_02247B7C(FashionApp *app);
void ov41_02247D44(FashionApp *app);
void ov41_02247D64(FashionApp *app);
int ov41_02247DF8(FashionApp *app);
void ov41_02247F3C(void *canvas, const UnkOv41CanvasTemplate *tmpl);
void ov41_02247F90(void *canvas);
void ov41_02247FE0(void *canvas, Pokemon *mon, PokepicTemplate *tmpl, enum HeapID heapID);
void ov41_02247FFC(void *canvas, Pokemon *mon, PokepicTemplate *tmpl, enum HeapID heapID);
void ov41_022480E0(void *canvas);
void ov41_02248158(void *canvas);
void ov41_0224825C(void *canvas, int background, enum HeapID heapID);
void ov41_022482A8(void *canvas);
int ov41_02248ED4(UnkOv41FashionTable *tbl, int id);
int ov41_02248EE8(UnkOv41FashionTable *tbl, int slot);
void ov41_02248F18(UnkOv41TouchCtx *ctx, void *canvas, UnkOv41Board *board, FashionApp *app, void *panel, BOOL a5);
void ov41_02248F6C(UnkOv41TouchCtx *ctx);
void ov41_022495C8(FashionApp *app, PokepicTemplate *tmpl);
void ov41_022499B4(UnkOv41Pool *pool, int count, enum HeapID heapID);
void ov41_022499DC(UnkOv41Pool *pool);
void ov41_02249C7C(void *scroll, UnkOv41BgScrollTemplate *tmpl);
void ov41_02249CC4(void *scroll);
void ov41_0224A27C(void *bar, void *gfx, int *inputMode);
void ov41_0224A3E4(void *bar, void *gfx);
void ov41_0224A54C(void *bar);
void ov41_0224A580(void *bar);
void ov41_0224A5A4(void *bar, int dx, int dy);
void ov41_0224A5D4(void *bar, int id, void (*callback)(void *button, void *arg), void *arg, int buttonId);
void ov41_0224AA08(void *panel, const UnkOv41PanelArgs *args, u32 flags);
void ov41_0224AB40(void *panel);
void ov41_0224ABF0(void *panel);
void ov41_0224AC08(void *panel, NarcId narcId, int fileId, int msgId);
u8 ov41_0224AC40(void *panel, NarcId narcId, int fileId, int msgId);
void ov41_0224AC80(void *panel);
void ov41_0224B4E8(void *a0, void *canvas, enum HeapID heapID);
void ov41_0224B50C(void *a0);
void ov41_0224B518(void *a0);

void ov41_02246B34(FashionApp *app);
void ov41_02246B5C(FashionApp *app);
void ov41_02246B68(FashionApp *app, UnkOv41GfxTables *tables);
void ov41_02246BEC(FashionApp *app, UnkOv41GfxTables *tables);
void ov41_02246C90(FashionApp *app, enum HeapID heapID);
void ov41_02246CB0(FashionApp *app);
void ov41_02246CC0(FashionApp *app, enum HeapID heapID, u32 texSize, u32 plttSize);
void ov41_02246D2C(FashionApp *app);
void ov41_02246D54(UnkOv41GfxTables *tables, int numChars, int numPltts, enum HeapID heapID);
void ov41_02246DA8(UnkOv41GfxTables *tables);
BOOL ov41_02246DE0(OverlayManager *man, int *state);
BOOL ov41_02246F08(OverlayManager *man, int *state);
BOOL ov41_02247150(OverlayManager *man, int *state);
static void ov41_02247240(FashionApp *app);
static void ov41_0224726C(FashionApp *app);
static void ov41_02247274(FashionApp *app);
static void ov41_02247288(FashionApp *app, Pokemon *mon, int maxObjs, int a3);
static void ov41_02247310(FashionApp *app);
static void ov41_02247334(FashionApp *app);
static void ov41_022473F0(FashionApp *app);
void ov41_02247414(FashionApp *app);
static void ov41_02247478(void *arg);
void ov41_02247480(FashionApp *app, int a1);
static void ov41_022474C4(FashionApp *app);
static void ov41_022474D4(FashionApp *app);
static void ov41_02247568(FashionApp *app);
static void ov41_02247578(FashionApp *app);
void ov41_02247588(FashionApp *app);
static void ov41_02247598(void *button, void *arg);
static void ov41_022475B4(void *button, void *arg);
static void ov41_022475D4(void *button, void *arg);
static void ov41_022475F4(void *button, void *arg);
static void ov41_02247628(void *button, void *arg);
static void ov41_0224765C(FashionApp *app, Options *options);
static void ov41_022476A8(FashionApp *app);
static void ov41_022476B8(FashionApp *app, int *done);
static void ov41_022476E0(SysTask *task, void *data);

void ov41_02246B34(FashionApp *app) {
    int i;

    SpriteList_Delete(app->spriteList);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(app->resMans[i]);
    }
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}

void ov41_02246B5C(FashionApp *app) {
    SpriteList_RenderAndAnimateSprites(app->spriteList);
}

void ov41_02246B68(FashionApp *app, UnkOv41GfxTables *tables) {
    int i;
    void *file;

    for (i = 0; i < 100; i++) {
        file = GfGfxLoader_LoadFromOpenNarc(app->narc, i + 1, FALSE, HEAP_ID_14, TRUE);
        GF_ASSERT(file != NULL);
        tables->chars[i].charData = ov41_022463DC(app, file, i);
        tables->chars[i].unk0 = app->unk000;
    }
    file = GfGfxLoader_LoadFromOpenNarc(app->narc, 0, FALSE, HEAP_ID_14, TRUE);
    GF2dGfxRawResMan_AllocObj(tables->plttMan, file, 0);
    NNS_G2dGetUnpackedPaletteData(file, &tables->pltts->pltt);
    tables->pltts->unk0 = app->unk000;
    tables->pltts->unk8 = 3;
}

void ov41_02246BEC(FashionApp *app, UnkOv41GfxTables *tables) {
    int i, idx;
    void *file;

    for (i = 0; i < 18; i++) {
        file = GfGfxLoader_LoadFromOpenNarc(app->narc, i + 0xCE, FALSE, HEAP_ID_14, TRUE);
        idx = i + 100;
        tables->chars[idx].charData = ov41_022463DC(app, file, idx);
        tables->chars[idx].unk0 = app->unk000;
        idx = i + 1;
        file = GfGfxLoader_LoadFromOpenNarc(app->narc, 0x87 + i * 4, FALSE, HEAP_ID_14, TRUE);
        GF2dGfxRawResMan_AllocObj(tables->plttMan, file, idx);
        NNS_G2dGetUnpackedPaletteData(file, &tables->pltts[idx].pltt);
        GF_ASSERT(tables->pltts[idx].pltt != NULL);
        tables->pltts[idx].unk0 = app->unk000;
        tables->pltts[idx].unk8 = 1;
    }
}

void ov41_02246C90(FashionApp *app, enum HeapID heapID) {
    app->unk030 = GfGfxLoader_LoadFromOpenNarc(app->narc, 0xEB, FALSE, heapID, FALSE);
}

void ov41_02246CB0(FashionApp *app) {
    Heap_Free(app->unk030);
    app->unk030 = NULL;
}

void ov41_02246CC0(FashionApp *app, enum HeapID heapID, u32 texSize, u32 plttSize) {
    app->pokepicManager = PokepicManager_Create(heapID);
    app->texKey = NNS_GfdAllocTexVram(texSize, FALSE, 0);
    app->plttKey = NNS_GfdAllocPlttVram(plttSize, FALSE, 1);
    PokepicManager_SetCharBaseAddrAndSize(app->pokepicManager, NNS_GfdGetTexKeyAddr(app->texKey), ((app->texKey & 0x7FFF0000) >> 16) << 4);
    PokepicManager_SetPlttBaseAddrAndSize(app->pokepicManager, NNS_GfdGetPlttKeyAddr(app->plttKey), ((app->plttKey & 0xFFFF0000) >> 16) << 3);
    app->pokepicActive = TRUE;
}

void ov41_02246D2C(FashionApp *app) {
    PokepicManager_Delete(app->pokepicManager);
    NNS_GfdFreeTexVram(app->texKey);
    NNS_GfdFreePlttVram(app->plttKey);
    app->pokepicActive = FALSE;
}

void ov41_02246D54(UnkOv41GfxTables *tables, int numChars, int numPltts, enum HeapID heapID) {
    u32 size = numChars * sizeof(UnkOv41CharEntry);

    tables->chars = Heap_Alloc(heapID, size);
    memset(tables->chars, 0, size);
    tables->charMan = GF2dGfxRawResMan_Create(numChars, heapID);
    tables->numChars = numChars;

    size = numPltts * sizeof(UnkOv41PlttEntry);
    tables->pltts = Heap_Alloc(heapID, size);
    memset(tables->pltts, 0, size);
    tables->plttMan = GF2dGfxRawResMan_Create(numPltts, heapID);
    tables->numPltts = numPltts;
}

void ov41_02246DA8(UnkOv41GfxTables *tables) {
    if (tables->charMan != NULL) {
        GF2dGfxRawResObj_Destroy(tables->charMan);
        tables->charMan = NULL;
    }
    if (tables->plttMan != NULL) {
        GF2dGfxRawResObj_Destroy(tables->plttMan);
        tables->plttMan = NULL;
    }
    Heap_Free(tables->chars);
    tables->chars = NULL;
    Heap_Free(tables->pltts);
    tables->pltts = NULL;
}

BOOL ov41_02246DE0(OverlayManager *man, int *state) {
    FashionApp *app;
    FashionAppArgs *args;

    Heap_Create(HEAP_ID_3, HEAP_ID_13, 0x20000);
    Heap_Create(HEAP_ID_3, HEAP_ID_14, 0x40000);
    app = OverlayManager_CreateAndGetData(man, sizeof(FashionApp), HEAP_ID_13);
    memset(app, 0, sizeof(FashionApp));
    Main_SetVBlankIntrCB(ov41_02247478, app);
    HBlankInterruptDisable();
    args = OverlayManager_GetArgs(man);
    app->options = args->options;
    if (args->menuInputStateMgr != NULL) {
        app->menuInputState = MenuInputStateMgr_GetState(args->menuInputStateMgr);
    } else {
        app->menuInputState = 0;
    }
    sub_020210BC();
    sub_02021148(4);
    ov41_02248E84(args->fashionCase, &app->fashionTable);
    ov41_02247240(app);
    ov41_022499B4(&app->pool, 0x2CF, HEAP_ID_13);
    app->unk364 = ov41_02245EA0(0x2BC, HEAP_ID_13);
    ov41_02247288(app, args->mon, 10, 0);
    ov41_02247334(app);
    ov41_02247480(app, 0);
    ov41_022474D4(app);
    ov41_0224765C(app, args->options);
    ov41_02248F18(&app->touchCtx, app->canvas, &app->board, app, app->panel, TRUE);
    app->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_13);
    app->window = AllocWindows(HEAP_ID_13, 1);
    app->mode = 0;
    Sound_SetSceneAndPlayBGM(0x35, 0, 0);
    return TRUE;
}

BOOL ov41_02246F08(OverlayManager *man, int *state) {
    FashionApp *app = OverlayManager_GetData(man);
    BOOL ret = FALSE;
    FashionAppArgs *args = OverlayManager_GetArgs(man);

    switch (*state) {
    case 0:
    case 1:
        BeginNormalPaletteFade(FADE_MAIN_THEN_SUB, FADE_TYPE_UNK_5, FADE_TYPE_UNK_5, RGB_BLACK, 6, 1, HEAP_ID_13);
        *state = 2;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            (*state)++;
        }
        break;
    case 3:
        if (args->showIntro == 1) {
            TextFlags_SetCanTouchSpeedUpPrint(TRUE);
            app->printerId = ov41_0224AC40(app->panel, NARC_msgdata_msg, 0xD7, 0x2F);
            (*state)++;
        } else {
            *state = 6;
        }
        break;
    case 4:
        if (!TextPrinterCheckActive(app->printerId)) {
            ov41_0224AC80(app->panel);
            (*state)++;
        }
        break;
    case 5:
        if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) | gSystem.touchNew) {
            ov41_0224AC08(app->panel, NARC_msgdata_msg, 0xD7, 0x30);
            TextFlags_SetCanTouchSpeedUpPrint(FALSE);
            (*state)++;
        }
        break;
    case 6:
        if (app->mode == 3) {
            ov41_022476B8(app, &app->taskDone);
            *state = 7;
        }
        ov41_02248E44(&app->touchCtx);
        ov41_02247D44(app);
        ov41_0224ABF0(app->panel);
        ov41_02247578(app);
        break;
    case 7:
        if (app->taskDone) {
            app->taskDone = FALSE;
            *state = 8;
            app->mode = 4;
            ov41_0224B4E8(app->unk47C, app->canvas, HEAP_ID_14);
        }
        break;
    case 8:
        if (app->mode == 9) {
            ov41_02247828(app, &app->taskDone);
            *state = 10;
        } else if (app->mode == 8) {
            *state = 9;
            app->mode = 5;
            ov41_02247D64(app);
        } else {
            app->mode = ov41_02247B7C(app);
            ov41_0224B50C(app->unk47C);
        }
        break;
    case 9:
        if (app->mode == 6) {
            app->confirmed = TRUE;
            *state = 11;
        } else if (app->mode == 7) {
            app->confirmed = FALSE;
            *state = 11;
        } else {
            app->mode = ov41_02247DF8(app);
            ov41_0224B50C(app->unk47C);
        }
        break;
    case 10:
        if (app->taskDone) {
            app->taskDone = FALSE;
            *state = 6;
            app->mode = 0;
            ov41_0224B518(app->unk47C);
        }
        break;
    case 11:
        BeginNormalPaletteFade(FADE_MAIN_THEN_SUB, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_13);
        (*state)++;
        break;
    case 12:
        if (IsPaletteFadeFinished()) {
            *state = 0;
            app->mode = 10;
            ret = TRUE;
            ov41_0224B518(app->unk47C);
        }
        break;
    }
    ov41_0224726C(app);
    return ret;
}

BOOL ov41_02247150(OverlayManager *man, int *state) {
    FashionApp *app = OverlayManager_GetData(man);
    FashionAppArgs *args = OverlayManager_GetArgs(man);

    if (app->confirmed == TRUE) {
        GameStats_AddScore(args->gameStats, SCORE_EVENT_POKEMON_DRESSED);
        ov41_022479A8(args->portrait, app->canvas, args->profile);
    }
    if (args->result != NULL) {
        if (app->confirmed == TRUE) {
            *args->result = TRUE;
        } else {
            *args->result = FALSE;
        }
    }
    if (args->menuInputStateMgr != NULL) {
        MenuInputStateMgr_SetState(args->menuInputStateMgr, (MenuInputState)app->menuInputState);
    }
    YesNoPrompt_Destroy(app->yesNoPrompt);
    WindowArray_Delete(app->window, 1);
    ov41_022476A8(app);
    ov41_02248F6C(&app->touchCtx);
    ov41_02247568(app);
    ov41_022474C4(app);
    ov41_02247310(app);
    ov41_022473F0(app);
    ov41_02245ECC(app->unk364);
    app->unk364 = NULL;
    ov41_022499DC(&app->pool);
    ov41_02247274(app);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GF_ASSERT(sub_02021238() == TRUE);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_13);
    Heap_Destroy(HEAP_ID_14);
    return TRUE;
}

static void ov41_02247240(FashionApp *app) {
    UnkOv41GfxTables tables;

    ov41_02246130(app);
    ov41_02246170(app);
    ov41_022463B0(app, &tables);
    ov41_02246250(app, &tables);
    ov41_022463D4(&tables);
}

static void ov41_0224726C(FashionApp *app) {
    ov41_0224621C(app);
}

static void ov41_02247274(FashionApp *app) {
    ov41_0224626C(app);
    ov41_022461D0(app);
    ov41_02246150();
}

static void ov41_02247288(FashionApp *app, Pokemon *mon, int maxObjs, int a3) {
    UnkOv41CanvasTemplate tmpl;
    PokepicTemplate pokepicTemplate;

    tmpl.unk00 = app->unk000;
    tmpl.unk04 = app->unk004;
    tmpl.unk08 = app->unk010;
    tmpl.unk0C = app->unk030;
    tmpl.pokepicManager = app->pokepicManager;
    tmpl.bgConfig = app->bgConfig;
    tmpl.maxObjs = maxObjs;
    tmpl.unk18 = app->unk364;
    tmpl.pool = &app->pool;
    ov41_02247F3C(app->canvas, &tmpl);
    if (a3 == 0) {
        ov41_02247FE0(app->canvas, mon, &pokepicTemplate, HEAP_ID_14);
    } else {
        ov41_02247FFC(app->canvas, mon, &pokepicTemplate, HEAP_ID_14);
    }
    ov41_022495C8(app, &pokepicTemplate);
    ov41_02248158(app->canvas);
    ov41_0224825C(app->canvas, 0, HEAP_ID_14);
}

static void ov41_02247310(FashionApp *app) {
    ov41_022482A8(app->canvas);
    ov41_022480E0(app->canvas);
    ov41_02247F90(app->canvas);
}

static void ov41_02247334(FashionApp *app) {
    UnkOv41BoardTemplate tmpl;
    int i, j, n;

    tmpl.unk00 = app->unk000;
    tmpl.unk04 = app->unk004;
    tmpl.unk08 = app->unk010;
    tmpl.unk0C = app->unk030;
    tmpl.bgConfig = app->bgConfig;
    tmpl.fashionTable = &app->fashionTable;
    tmpl.unk14 = app->unk364;
    tmpl.pool = &app->pool;
    tmpl.count0 = 14;
    tmpl.count1 = 2;
    tmpl.count2 = 1;
    ov41_02248488(&app->board, &tmpl);
    ov41_022487F8(&app->board, 0, 0);
    for (i = 0; i < 100; i++) {
        n = ov41_02248ED4(&app->fashionTable, i);
        for (j = 0; j < n; j++) {
            ov41_022485DC(&app->board, 0, i);
        }
    }
    for (i = 0; i < 18; i++) {
        n = ov41_02248EE8(&app->fashionTable, i);
        if (n < 18) {
            ov41_022485DC(&app->board, 1, n);
        }
    }
    ov41_02248724(&app->board);
}

static void ov41_022473F0(FashionApp *app) {
    ov41_02248940(&app->board);
    ov41_022486F8(&app->board);
    ov41_022484C0(&app->board);
}

void ov41_02247414(FashionApp *app) {
    G2_SetBG0Priority(1);
    G2_SetBG1Priority(0);
    G2_SetBG2Priority(2);
    G2_SetBG3Priority(3);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, -16);
    ov41_0224888C(&app->board, 0);
    ov41_022488D8(&app->board, 0, 2, 0, NULL);
}

static void ov41_02247478(void *arg) {
    ov41_02246494(arg);
}

void ov41_02247480(FashionApp *app, int a1) {
    UnkOv41BgScrollTemplate tmpl;

    tmpl.bgConfig = app->bgConfig;
    tmpl.narcId = NARC_a_0_2_6;
    tmpl.charFile = a1 * 2 + 0x79;
    tmpl.plttFile = 0x85;
    tmpl.scrnFile = a1 * 2 + 0x7A;
    tmpl.x = 0;
    tmpl.y = 0;
    tmpl.bgId = 1;
    tmpl.plttCount = 1;
    tmpl.plttSlot = 2;
    tmpl.tileOffset = 0;
    tmpl.heapID = HEAP_ID_14;
    ov41_02249C7C(app->scroll, &tmpl);
}

static void ov41_022474C4(FashionApp *app) {
    ov41_02249CC4(app->scroll);
}

static void ov41_022474D4(FashionApp *app) {
    ov41_0224A27C(app->buttonBar, app, &app->menuInputState);
    ov41_0224A5D4(app->buttonBar, 0, ov41_022475B4, app, 0);
    ov41_0224A5D4(app->buttonBar, 1, ov41_022475D4, app, 0);
    ov41_0224A5D4(app->buttonBar, 2, ov41_022475F4, app, 0);
    ov41_0224A5D4(app->buttonBar, 3, ov41_02247628, app, 0);
    ov41_0224A5D4(app->buttonBar, 4, ov41_02247598, app, 0);
}

static void ov41_02247568(FashionApp *app) {
    ov41_0224A3E4(app->buttonBar, app);
}

static void ov41_02247578(FashionApp *app) {
    ov41_0224A54C(app->buttonBar);
}

void ov41_02247588(FashionApp *app) {
    ov41_0224A580(app->buttonBar);
}

static void ov41_02247598(void *button, void *arg) {
    FashionApp *app = arg;

    if (ov41_02248998(&app->board)) {
        app->mode = 3;
    }
}

static void ov41_022475B4(void *button, void *arg) {
    FashionApp *app = arg;

    ov41_02248790(&app->board, ov41_0224894C(&app->board), 0);
}

static void ov41_022475D4(void *button, void *arg) {
    FashionApp *app = arg;

    ov41_02248790(&app->board, ov41_0224894C(&app->board), 1);
}

static void ov41_022475F4(void *button, void *arg) {
    FashionApp *app = arg;

    if (app->mode != 0) {
        ov41_022487F8(&app->board, 0, ov41_0224895C(&app->board, 0));
        app->mode = 0;
    }
}

static void ov41_02247628(void *button, void *arg) {
    FashionApp *app = arg;

    if (app->mode != 1) {
        ov41_022487F8(&app->board, 1, ov41_0224895C(&app->board, 1));
        app->mode = 1;
    }
}

static void ov41_0224765C(FashionApp *app, Options *options) {
    UnkOv41PanelArgs args = { 0 };

    args.bgConfig = app->bgConfig;
    args.spriteList = app->spriteList;
    args.resMans = app->resMans;
    args.options = options;
    args.count = 10;
    args.narc = app->narc;
    ov41_0224AA08(app->panel, &args, 0xF);
}

static void ov41_022476A8(FashionApp *app) {
    ov41_0224AB40(app->panel);
}

static void ov41_022476B8(FashionApp *app, int *done) {
    UnkOv41FadeTask *task = SysTask_GetData(CreateSysTaskAndEnvironment(ov41_022476E0, sizeof(UnkOv41FadeTask), 10, HEAP_ID_13));

    task->app = app;
    task->done = done;
    task->counter = 0;
    task->state = 0;
}

static void ov41_022476E0(SysTask *task, void *data) {
    UnkOv41FadeTask *fade = data;

    switch (fade->state) {
    case 0:
        GF_ASSERT(ov41_02248750(&fade->app->board, 3, 0));
        fade->state++;
        break;
    case 1:
        if (ov41_02248998(&fade->app->board)) {
            fade->state++;
        }
        break;
    case 2:
        StartBrightnessTransition(8, -16, 0, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3), 1);
        fade->state++;
        break;
    case 3:
        if (IsBrightnessTransitionActive(1)) {
            fade->state++;
        }
        break;
    case 4:
        ov41_0224A5A4(fade->app->buttonBar, 0, 8);
        if (++fade->counter >= 8) {
            fade->counter = 0;
            fade->state++;
        }
        break;
    case 5:
        if (ov41_02247A48(fade, -8, 5, 8)) {
            fade->counter = 0;
            fade->state++;
        }
        break;
    case 6:
        ov41_02247480(fade->app, 1);
        ScheduleSetBgPosText(fade->app->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
        ov41_02247AB4(fade->app);
        fade->state++;
        break;
    case 7:
        StartBrightnessTransition(8, 0, -16, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3), 1);
        fade->state++;
        break;
    case 8:
        if (IsBrightnessTransitionActive(1)) {
            fade->state++;
        }
        break;
    case 9:
        *fade->done = TRUE;
        DestroySysTaskAndEnvironment(task);
        break;
    }
}
