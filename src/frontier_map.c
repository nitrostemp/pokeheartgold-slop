#include "global.h"

#include "frontier/frontier.h"

#include "bg_window.h"
#include "field_bgm.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "options.h"
#include "overlay_42.h"
#include "overlay_80_0222ACA0.h"
#include "palette.h"
#include "player_data.h"
#include "render_text.h"
#include "render_window.h"
#include "sound_02004A44.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_020210A0.h"
#include "unk_02026E30.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"

typedef struct FrontierMapObjData {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    s16 unk06;
    s16 unk08;
    u8 unk0A;
    u8 unk0B;
    u16 unk0C[9];
} FrontierMapObjData; // size: 0x1E

typedef struct FrontierMapObj {
    UnkStruct_ov42_02228110 *obj;    // 0x00
    UnkStruct_ov42_0222903C *sprite; // 0x04
    FrontierMapObjData data;         // 0x08
    u8 unk26[0x12];                  // 0x26
    SysTask *movementTask;           // 0x38
} FrontierMapObj;                    // size: 0x3C

typedef struct FrontierMapGfxRes {
    u16 id;
    u8 unk2;
    u8 unk3;
} FrontierMapGfxRes;

typedef struct FrontierMapSavedSprite {
    s16 x;
    s16 y;
    u8 resId;
    u8 anim;
    u16 frame : 13;
    u16 ticking : 1;
    u16 drawFlag : 1;
    u16 active : 1;
} FrontierMapSavedSprite;

typedef struct FrontierMapSavedSprites {
    u16 resIds[8];
    FrontierMapSavedSprite sprites[8];
} FrontierMapSavedSprites;

typedef struct FrontierMapSprites {
    ManagedSprite *sprites[8]; // 0x00
    u16 spriteResIds[8];       // 0x20
    u32 tickFlags;             // 0x30
    u16 resIds[8];             // 0x34
} FrontierMapSprites;          // size: 0x44

typedef struct FrontierMap {
    BgConfig *bgConfig;             // 0x00
    PaletteData *paletteData;       // 0x04
    void *frontier;                 // 0x08
    GF3DVramMan *g3dVramMan;        // 0x0C
    void *particleSys;              // 0x10
    UnkStruct_ov42_022280A8 *unk14; // 0x14
    UnkStruct_ov42_02227F68 *unk18; // 0x18
    UnkStruct_ov44_02232914 unk1C;  // 0x1C
    UnkStruct_ov42_02228EDC *unk20; // 0x20
    UnkStruct_ov42_022293B8 *unk24; // 0x24
    UnkStruct_ov42_022293B8 *unk28; // 0x28
    UnkStruct_ov42_02229A40 *unk2C; // 0x2C
    UnkStruct_ov42_02229A40 *unk30; // 0x30
    SpriteSystem *spriteSystem;     // 0x34
    SpriteManager *spriteManager;   // 0x38
    FrontierMapSprites spr;         // 0x3C
    ManagedSprite *unk80[4];        // 0x80
    u32 unk90;                      // 0x90
    SysTask *task94;                // 0x94
    SysTask *task98;                // 0x98
    SysTask *task9C;                // 0x9C
    SysTask *taskA0;                // 0xA0
    u8 unkA4[4];                    // 0xA4
    s16 scrollX;                    // 0xA8
    s16 scrollY;                    // 0xAA
    u8 unkAC[0x15];                 // 0xAC
    u8 sceneId;                     // 0xC1
    u8 unkC2[2];                    // 0xC2
} FrontierMap;                      // size: 0xC4

void Sound_SetFieldBGM(u16 seqNo);
FrontierMapGfxRes *sub_02096864(void *frontier);
FrontierMapObj *sub_02096868(void *frontier);
FrontierMapObj *sub_0209686C(void *frontier, int index);
FrontierMapSavedSprites *sub_02096878(void *frontier);
void sub_02096884(void *frontier);
u32 ov80_0222A7EC(PlayerProfile *profile);
void *ov80_02239960(enum HeapID heapID);
void ov80_02239980(void *particleSys);
void ov80_02239A38(void);
void ov80_02239AF8(SpriteSystem *spriteSystem, SpriteManager *spriteManager, NARC *narc, PaletteData *plttData, u16 index);
void ov80_02239B7C(SpriteManager *spriteManager, u32 index);
ManagedSprite *ov80_02239BB8(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u32 index);
void ov80_02239BE8(ManagedSprite *managedSprite);

FrontierMap *FrontierMap_Init(void *frontier);
void FrontierMap_Free(FrontierMap *map);
void ov80_022389C4(FrontierMap *map);
void ov80_02238A18(FrontierMap *map);
static void FrontierMap_VBlank(void *arg);
static void ov80_02238AAC(SysTask *task, void *data);
static void ov80_02238AB0(SysTask *task, void *data);
static void ov80_02238ABC(SysTask *task, void *data);
static void FrontierMap_Update(SysTask *task, void *data);
static void FrontierMap_Scroll(FrontierMap *map);
static void ov80_02238B7C(FrontierMap *map);
static void ov80_02238C78(FrontierMap *map);
static void FrontierMap_SetVramBank(BgConfig *bgConfig, int sceneId);
static void FrontierMap_LoadPaletteData(FrontierMap *map);
static void ov80_02238FA0(FrontierMap *map);
static void ov80_02239004(FrontierMap *map, int sceneId, PlayerProfile *profile);
static void ov80_0223927C(FrontierMap *map);
static GF3DVramMan *ov80_022392DC(enum HeapID heapID);
static void ov80_022392F8(void);
static void ov80_0223937C(GF3DVramMan *vramMan);
static void ov80_02239384(FrontierMap *map);
static void ov80_022393E8(FrontierMap *map);
void ov80_0223947C(FrontierMap *map, const FrontierMapGfxRes *res);
void ov80_022394D8(FrontierMap *map, int id);
UnkStruct_ov42_02228110 *ov80_02239510(FrontierMap *map, const FrontierMapObjData *data, int index);
void ov80_02239590(FrontierMap *map, UnkStruct_ov42_02228110 *obj);
void ov80_022395E8(FrontierMap *map, u16 id, UnkStruct_ov42_02228110 **outObj, UnkStruct_ov42_0222903C **outSprite);
void ov80_0223962C(FrontierMap *map, u16 resId);
void ov80_0223965C(FrontierMap *map, u16 resId);
ManagedSprite *ov80_0223968C(FrontierMap *map, u16 index, u16 resId);
void ov80_022396D8(FrontierMap *map, u16 index);
ManagedSprite *ov80_02239700(FrontierMap *map, u16 index);
void ov80_02239708(FrontierMap *map, u16 index, int ticking);
u32 ov80_02239734(FrontierMap *map, u16 index);
static void ov80_02239740(FrontierMap *map);
static void ov80_02239828(FrontierMap *map);
void ov80_022398E4(FrontierMap *map, s16 *x, s16 *y);
static void ov80_02239900(FrontierMapObj *obj, FrontierMapObjData *out);
static void ov80_02239914(void *frontier, int index, UnkStruct_ov42_02228110 *obj, UnkStruct_ov42_0222903C *sprite, const FrontierMapObjData *data);
FrontierMapObj *ov80_02239938(void *frontier, int id);

static const OamManagerParam ov80_0223D5B8 = { 0, 128, 0, 32, 0, 128, 0, 32 };

static const OamCharTransferParam ov80_0223D570 = { 96, 0x10000, 0x4000, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K };

static const SpriteResourceCountsListUnion ov80_0223D584 = { 96, 32, 64, 64, 8, 8 };

static const u32 ov80_0223D654[256] = { 0 };

FrontierMap *FrontierMap_Init(void *frontier) {
    FrontierMap *map;
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(frontier);
    PlayerProfile *profile = Save_PlayerData_GetProfile(args->saveData);
    int sceneId = args->unk20;
    int i;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);

    Heap_Create(HEAP_ID_3, HEAP_ID_101, 0x90000);
    map = Heap_Alloc(HEAP_ID_101, sizeof(FrontierMap));
    MI_CpuFill8(map, 0, sizeof(FrontierMap));
    map->frontier = frontier;
    map->sceneId = sceneId;
    for (i = 0; i < 8; i++) {
        map->spr.resIds[i] = 0xFFFF;
    }

    map->g3dVramMan = ov80_022392DC(HEAP_ID_101);
    map->paletteData = PaletteData_Init(HEAP_ID_101);
    PaletteData_SetAutoTransparent(map->paletteData, TRUE);
    PaletteData_AllocBuffers(map->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_101);
    PaletteData_AllocBuffers(map->paletteData, PLTTBUF_SUB_BG, 0x200, HEAP_ID_101);
    PaletteData_AllocBuffers(map->paletteData, PLTTBUF_MAIN_OBJ, 0x1C0, HEAP_ID_101);
    PaletteData_AllocBuffers(map->paletteData, PLTTBUF_SUB_OBJ, 0x200, HEAP_ID_101);
    map->bgConfig = BgConfig_Alloc(HEAP_ID_101);
    GF_CreateVramTransferManager(64, HEAP_ID_101);
    SetKeyRepeatTimers(4, 8);

    FrontierMap_SetVramBank(map->bgConfig, sceneId);
    FrontierMap_LoadPaletteData(map);
    ov80_02238FA0(map);
    sub_020210BC();
    sub_02021148(4);
    ov80_02239384(map);
    map->particleSys = ov80_02239960(HEAP_ID_101);
    ov80_02239004(map, sceneId, profile);

    map->task94 = SysTask_CreateOnMainQueue(ov80_02238AB0, map, 60000);
    map->task98 = SysTask_CreateOnMainQueue(ov80_02238ABC, map, 61000);
    map->task9C = SysTask_CreateOnMainQueue(FrontierMap_Update, map, 80000);

    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    Sound_SetFieldBGM(ov80_0222ACA0(sceneId, 3));
    sub_02055198(NULL, ov80_0222ACA0(sceneId, 3));
    TextFlags_SetAutoScrollParam(1);
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    Main_SetVBlankIntrCB(FrontierMap_VBlank, map);
    map->taskA0 = SysTask_CreateOnVBlankQueue(ov80_02238AAC, map, 10);
    ov80_0222AD9C(map, &map->unk90, map->sceneId);
    sub_0203A880();
    return map;
}

void FrontierMap_Free(FrontierMap *map) {
    Frontier_GetLaunchArgs(map->frontier);
    ov80_0222ADB4(map, &map->unk90, map->sceneId);
    ov80_0223927C(map);

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(map->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(map->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(map->bgConfig, GF_BG_LYR_MAIN_3);
    ToggleBgLayer(GF_BG_LYR_SUB_0, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(map->bgConfig, GF_BG_LYR_SUB_0);

    ov80_022393E8(map);
    ov80_02239980(map->particleSys);
    GF_DestroyVramTransferManager();
    PaletteData_FreeBuffers(map->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(map->paletteData, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(map->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(map->paletteData, PLTTBUF_SUB_OBJ);
    PaletteData_Free(map->paletteData);
    Heap_Free(map->bgConfig);
    SysTask_Destroy(map->task94);
    SysTask_Destroy(map->task98);
    SysTask_Destroy(map->task9C);
    SysTask_Destroy(map->taskA0);
    ov80_0223937C(map->g3dVramMan);
    sub_02021238();
    Heap_Free(map);

    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    Heap_Destroy(HEAP_ID_101);
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    sub_0203A914();

    MI_CpuFill16((void *)HW_BG_PLTT, 0x7FFF, 0x200);
    MI_CpuFill16((void *)HW_OBJ_PLTT, 0x7FFF, 0x200);
    MI_CpuFill16((void *)HW_DB_BG_PLTT, 0x7FFF, 0x200);
    MI_CpuFill16((void *)HW_DB_OBJ_PLTT, 0x7FFF, 0x200);
    G2_BlendNone();
    G2S_BlendNone();
}

void ov80_022389C4(FrontierMap *map) {
    int i;
    FrontierMapObj *obj;

    for (i = 0; i < 32; i++) {
        obj = sub_0209686C(map->frontier, i);
        if (obj->obj != NULL) {
            obj->data.unk0A = ov42_02228188(obj->obj, 6);
            obj->data.unk02 = ov42_02228188(obj->obj, 5);
            obj->data.unk06 = ov42_02228188(obj->obj, 0);
            obj->data.unk08 = ov42_02228188(obj->obj, 1);
            obj->data.unk0B = ov42_022291F4(obj->sprite);
        }
    }

    ov80_02239740(map);
}

void ov80_02238A18(FrontierMap *map) {
    int i;
    FrontierMapGfxRes *res = sub_02096864(map->frontier);

    for (i = 0; i < 24; i++) {
        if (res[i].id != 0xFFFF) {
            ov42_02228FE0(map->unk20, res[i].id, res[i].unk2, HEAP_ID_101);
        }
    }

    {
        FrontierMapObj *obj;
        FrontierMapObjData data;

        for (i = 0; i < 32; i++) {
            obj = sub_0209686C(map->frontier, i);
            if (obj->data.unk04 != 0xFFFF) {
                ov80_02239900(obj, &data);
                ov80_02239510(map, &data, i);
            }
        }
    }

    ov80_02239828(map);
}

static void FrontierMap_VBlank(void *arg) {
    FrontierMap *map = arg;

    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(map->paletteData);
    DoScheduledBgGpuUpdates(map->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

static void ov80_02238AAC(SysTask *task, void *data) {
}

static void ov80_02238AB0(SysTask *task, void *data) {
    FrontierMap *map = data;
    ov42_0222807C(map->unk14);
}

static void ov80_02238ABC(SysTask *task, void *data) {
    FrontierMap *map = data;
    ov80_02238C78(map);
}

static void FrontierMap_Update(SysTask *task, void *data) {
    FrontierMap *map = data;

    {
        FrontierMapObj *obj = sub_0209686C(map->frontier, 31);
        if (obj->obj != NULL) {
            ov42_02229358(&map->unk1C, obj->obj);
        }
        FrontierMap_Scroll(map);
    }

    ov42_022290DC(map->unk20);

    {
        u32 flags = map->spr.tickFlags;
        int i;

        for (i = 0; i < 8; i++) {
            if (map->spr.sprites[i] != NULL && (flags & 1)) {
                ManagedSprite_TickFrame(map->spr.sprites[i]);
            }
            flags >>= 1;
        }
    }

    SpriteSystem_DrawSprites(map->spriteManager);
    SpriteSystem_UpdateTransfer();
    ov80_02239A38();
    RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

static void FrontierMap_Scroll(FrontierMap *map) {
    int mode;
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(map->frontier);

    mode = ov80_0222ACA0(args->unk20, 12);
    switch (mode) {
    case 0:
    default:
        if (map->unk24 != NULL) {
            ov42_02229420(map->unk24, &map->unk1C);
        }
        if (map->unk28 != NULL && ov80_0222ACA0(args->unk20, 13) == 1) {
            ov42_02229420(map->unk28, &map->unk1C);
        }
        break;
    case 1:
        ov80_02238B7C(map);
        break;
    }
}

static void ov80_02238B7C(FrontierMap *map) {
    s16 x;
    s16 y;
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(map->frontier);

    y = ov42_022293A8(&map->unk1C) + map->scrollY;
    x = ov42_022293B0(&map->unk1C) + map->scrollX;

    G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(map->spriteSystem), FX32_CONST(x), FX32_CONST(y));
    ScheduleSetBgPosText(map->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, x);
    ScheduleSetBgPosText(map->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, y);

    if (ov80_0222ACA0(args->unk20, 9) != 0xFFFF && ov80_0222ACA0(args->unk20, 13) == 1) {
        ScheduleSetBgPosText(map->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, x);
        ScheduleSetBgPosText(map->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, y);
    }
}

static void ov80_02238C78(FrontierMap *map) {
    UnkStruct_ov42_02228CDC unk0;
    UnkStruct_ov42_02228EB0 unk1;
    s32 result;

    while (ov42_02229A08((UnkStruct_ov42_022299C0 *)map->unk30, (UnkStruct_ov44_02232031 *)&unk1) == 1) {
        ov42_02228068(map->unk14, &unk1);
    }

    while (ov42_02229AC8(map->unk2C, &unk0) == 1) {
        result = ov42_02228C80(map->unk18, map->unk14, &unk0, &unk1);
        if (result == 1) {
            ov42_02228068(map->unk14, &unk1);
        }
    }
}

static void FrontierMap_SetVramBank(BgConfig *bgConfig, int sceneId) {
    GXBGMode bgMode = (GXBGMode)ov80_0222ACA0(sceneId, 0);
    GfGfx_DisableEngineAPlanes();

    GraphicsBanks banks = {
        GX_VRAM_BG_256_BC,
        GX_VRAM_BGEXTPLTT_23_G,
        GX_VRAM_SUB_BG_32_H,
        GX_VRAM_SUB_BGEXTPLTT_NONE,
        GX_VRAM_OBJ_64_E,
        GX_VRAM_OBJEXTPLTT_NONE,
        GX_VRAM_SUB_OBJ_16_I,
        GX_VRAM_SUB_OBJEXTPLTT_NONE,
        GX_VRAM_TEX_0_A,
        GX_VRAM_TEXPLTT_0_F,
    };
    GfGfx_SetBanks(&banks);

    MI_CpuClear32((void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
    MI_CpuClear32((void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
    MI_CpuClear32((void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
    MI_CpuClear32((void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);

    GraphicsModes graphicsModes = { GX_DISPMODE_GRAPHICS, GX_BGMODE_5, GX_BGMODE_0, GX_BG0_AS_3D };
    graphicsModes.bgMode = bgMode;
    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate bgTemplates[] = {
        { 0, 0, 0x800,  0, GF_BG_SCR_SIZE_256x256, GX_BG_COLORMODE_16,  GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01, 0, 0, 0, FALSE },
        { 0, 0, 0x2000, 0, GF_BG_SCR_SIZE_512x512, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x20000, GX_BG_EXTPLTT_23, 1, 0, 0, FALSE },
        { 0, 0, 0x2000, 0, GF_BG_SCR_SIZE_512x512, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x2800, GX_BG_CHARBASE_0x30000, GX_BG_EXTPLTT_23, 3, 0, 0, FALSE },
    };

    if (bgMode == GX_BGMODE_0) {
        bgTemplates[1].colorMode = GX_BG_COLORMODE_16;
        bgTemplates[2].colorMode = GX_BG_COLORMODE_16;
        bgTemplates[1].bgExtPltt = GX_BG_EXTPLTT_01;
        bgTemplates[2].bgExtPltt = GX_BG_EXTPLTT_01;
    }

    u16 screenSize = ov80_0222ACA0(sceneId, 4);
    bgTemplates[2].size = screenSize;
    if (ov80_0222ACA0(sceneId, 9) != 0xFFFF) {
        bgTemplates[1].size = screenSize;
    }

    if (bgMode == GX_BGMODE_0) {
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bgTemplates[0], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bgTemplates[1], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bgTemplates[2], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    } else {
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bgTemplates[0], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bgTemplates[1], GF_BG_TYPE_256x16PLTT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bgTemplates[2], GF_BG_TYPE_256x16PLTT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    }

    G2_SetBG0Priority(0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);

    BgTemplate subBgTemplate[] = {
        { 0, 0, 0x800, 0, GF_BG_SCR_SIZE_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01, 3, 0, 0, FALSE },
    };
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &subBgTemplate[0], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, 0);
}

static void FrontierMap_LoadPaletteData(FrontierMap *map) {
    PaletteData_LoadNarc(map->paletteData, NARC_graphic_font, 7, HEAP_ID_101, PLTTBUF_MAIN_BG, 0x20, 0xE0);
    PaletteData_LoadNarc(map->paletteData, NARC_graphic_font, 8, HEAP_ID_101, PLTTBUF_MAIN_BG, 0x20, 0xD0);

    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(map->frontier);

    LoadUserFrameGfx2(map->bgConfig, GF_BG_LYR_MAIN_1, 0x3E2, 11, Options_GetFrame(args->options), HEAP_ID_101);
    PaletteData_LoadPaletteSlotFromHardware(map->paletteData, PLTTBUF_MAIN_BG, 0xB0, 0x20);
    LoadUserFrameGfx1(map->bgConfig, GF_BG_LYR_MAIN_1, 0x3D9, 12, 0, HEAP_ID_101);
    PaletteData_LoadPaletteSlotFromHardware(map->paletteData, PLTTBUF_MAIN_BG, 0xC0, 0x20);
}

static void ov80_02238FA0(FrontierMap *map) {
    NARC *narc = NARC_New(NARC_a_1_8_3, HEAP_ID_101);

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x81, map->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, HEAP_ID_101);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x82, map->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, HEAP_ID_101);
    PaletteData_LoadNarc(map->paletteData, NARC_a_1_8_3, 0xBE, HEAP_ID_101, PLTTBUF_SUB_BG, 0x20, 0);
    NARC_Delete(narc);
}

static void ov80_02239004(FrontierMap *map, int sceneId, PlayerProfile *profile) {
    map->unk14 = ov42_02228010(32, HEAP_ID_101);
    map->unk18 = ov42_02227EE0(256 / 16, 256 / 16, HEAP_ID_101);
    ov42_02229394(&map->unk1C);
    map->unk20 = ov42_02228F24(SpriteManager_GetSpriteList(map->spriteManager), map->paletteData, 32, ov80_0222A7EC(profile), 0, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_101);

    {
        UnkTemplate_ov42_022293B8 template = { 0, 3, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2800, GX_BG_CHARBASE_0x30000, GX_BG_EXTPLTT_01, 3, 0, 0, 0, 1 };
        int mode;

        template.narcId = ov80_0222ACA0(sceneId, 5);
        template.fileId = ov80_0222ACA0(sceneId, 6);
        mode = ov80_0222ACA0(sceneId, 12);
        if (mode == 0) {
            map->unk24 = ov42_022293B8(SpriteSystem_GetRenderer(map->spriteSystem), map->bgConfig, &template, HEAP_ID_101);
        }

        if (ov80_0222ACA0(sceneId, 9) != 0xFFFF) {
            template.fileId = ov80_0222ACA0(sceneId, 9);
            template.unk_1 = 2;
            template.screenBase = GX_BG_SCRBASE_0x0800;
            template.charBase = GX_BG_CHARBASE_0x20000;
            template.priority = 1;
            if (mode == 0) {
                map->unk28 = ov42_022293B8(SpriteSystem_GetRenderer(map->spriteSystem), map->bgConfig, &template, HEAP_ID_101);
            }
        }
    }

    map->unk2C = ov42_02229A40(128, HEAP_ID_101);
    map->unk30 = ov42_02229974(128, HEAP_ID_101);
    ov42_02227F48(map->unk18, ov80_0223D654);

    {
        GXBGMode bgMode = (GXBGMode)ov80_0222ACA0(sceneId, 0);
        u32 narcId = ov80_0222ACA0(sceneId, 5);
        NARC *narc = NARC_New((NarcId)narcId, HEAP_ID_101);

        GfGfxLoader_LoadCharDataFromOpenNarc(narc, ov80_0222ACA0(sceneId, 7), map->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_101);

        if (bgMode == GX_BGMODE_0) {
            PaletteData_LoadNarc(map->paletteData, (NarcId)narcId, ov80_0222ACA0(sceneId, 8), HEAP_ID_101, PLTTBUF_MAIN_BG, 0x160, 0);
        } else {
            NNSG2dPaletteData *plttData;
            void *pltt = GfGfxLoader_GetPlttDataFromOpenNarc(narc, ov80_0222ACA0(sceneId, 8), &plttData, HEAP_ID_101);

            DC_FlushRange(plttData->pRawData, plttData->szByte);
            GX_BeginLoadBGExtPltt();
            GX_LoadBGExtPltt(plttData->pRawData, 0x6000, 0x2000);
            GX_EndLoadBGExtPltt();
            Heap_Free(pltt);
        }

        PaletteData_FillPaletteInBuffer(map->paletteData, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, 0, 0, 1);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, ov80_0222ACA0(sceneId, 6), map->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_101);

        if (ov80_0222ACA0(sceneId, 9) != 0xFFFF) {
            GfGfxLoader_LoadCharDataFromOpenNarc(narc, ov80_0222ACA0(sceneId, 10), map->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_101);
            GfGfxLoader_LoadScrnDataFromOpenNarc(narc, ov80_0222ACA0(sceneId, 9), map->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_101);

            if (bgMode != GX_BGMODE_0) {
                NNSG2dPaletteData *plttData;
                void *pltt = GfGfxLoader_GetPlttDataFromOpenNarc(narc, ov80_0222ACA0(sceneId, 11), &plttData, HEAP_ID_101);

                DC_FlushRange(plttData->pRawData, plttData->szByte);
                GX_BeginLoadBGExtPltt();
                GX_LoadBGExtPltt(plttData->pRawData, 0x4000, 0x2000);
                GX_EndLoadBGExtPltt();
                Heap_Free(pltt);
            }
        }

        ScheduleBgTilemapBufferTransfer(map->bgConfig, GF_BG_LYR_MAIN_3);
        NARC_Delete(narc);
    }
}

static void ov80_0223927C(FrontierMap *map) {
    int i;
    FrontierMapObj *objs = sub_02096868(map->frontier);

    for (i = 0; i < 32; i++) {
        if (objs[i].obj != NULL) {
            ov42_02228100(objs[i].obj);
            GF_ASSERT(objs[i].movementTask == NULL);
        }
    }

    ov42_02228050(map->unk14);
    ov42_02227F28(map->unk18);
    ov42_02228F94(map->unk20);
    if (map->unk24 != NULL) {
        ov42_0222940C(map->unk24);
    }
    if (map->unk28 != NULL) {
        ov42_0222940C(map->unk28);
    }
    ov42_02229A78(map->unk2C);
    ov42_022299AC((u32 *)map->unk30);
}

static GF3DVramMan *ov80_022392DC(enum HeapID heapID) {
    return GF_3DVramMan_Create(heapID, GF_3D_TEXALLOC_LNK, 1, GF_3D_PLTTALLOC_LNK, 1, ov80_022392F8);
}

static void ov80_022392F8(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(RGB_BLACK, 0, 0x7FFF, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
}

static void ov80_0223937C(GF3DVramMan *vramMan) {
    GF_3DVramMan_Delete(vramMan);
}

static void ov80_02239384(FrontierMap *map) {
    map->spriteSystem = SpriteSystem_Alloc(HEAP_ID_101);
    SpriteSystem_Init(map->spriteSystem, &ov80_0223D5B8, &ov80_0223D570, 32);
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    map->spriteManager = SpriteManager_New(map->spriteSystem);
    SpriteSystem_InitSprites(map->spriteSystem, map->spriteManager, 128);
    SpriteSystem_InitManagerWithCapacities(map->spriteSystem, map->spriteManager, (SpriteResourceCountsListUnion *)&ov80_0223D584);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(map->spriteSystem), 0, FX32_CONST(512));
}

static void ov80_022393E8(FrontierMap *map) {
    int i;

    for (i = 0; i < 8; i++) {
        if (map->spr.sprites[i] != NULL) {
            ov80_02239BE8(map->spr.sprites[i]);
        }
    }

    for (i = 0; i < 8; i++) {
        if (map->spr.resIds[i] != 0xFFFF) {
            ov80_02239B7C(map->spriteManager, map->spr.resIds[i]);
        }
    }

    for (i = 0; i < 4; i++) {
        if (map->unk80[i] != NULL) {
            Sprite_DeleteAndFreeResources(map->unk80[i]);
            // The four ids must not share one constant (each is rematerialized
            // except the last), hence the distinct literal types.
            SpriteManager_UnloadCharObjById(map->spriteManager, 50000 + i);
            SpriteManager_UnloadPlttObjById(map->spriteManager, 50000u + i);
            SpriteManager_UnloadCellObjById(map->spriteManager, 50000L + i);
            SpriteManager_UnloadAnimObjById(map->spriteManager, 50000uL + i);
        }
    }

    SpriteSystem_FreeResourcesAndManager(map->spriteSystem, map->spriteManager);
    SpriteSystem_Free(map->spriteSystem);
}

void ov80_0223947C(FrontierMap *map, const FrontierMapGfxRes *res) {
    FrontierMapGfxRes *list;
    int i, slot;

    list = sub_02096864(map->frontier);

    for (i = 0; i < 24; i++) {
        if (list[i].id == res->id) {
            return;
        }
    }

    for (i = 0; i < 24; i++) {
        if (list[i].id == 0xFFFF) {
            break;
        }
    }

    GF_ASSERT(i != 24);

    slot = i;
    list[slot] = *res;
    ov42_02228FE0(map->unk20, res->id, res->unk2, HEAP_ID_101);
}

void ov80_022394D8(FrontierMap *map, int id) {
    int i;
    FrontierMapGfxRes *list = sub_02096864(map->frontier);

    for (i = 0; i < 24; i++) {
        if (list[i].id == id) {
            ov42_02229004(map->unk20, id);
            list[i].id = 0xFFFF;
            return;
        }
    }
}

UnkStruct_ov42_02228110 *ov80_02239510(FrontierMap *map, const FrontierMapObjData *data, int index) {
    FrontierMapObj *objs;
    int i, slot;
    UnkStruct_ov42_02122667 params;
    UnkStruct_ov42_02228110 *obj;
    UnkStruct_ov42_0222903C *sprite;

    objs = sub_02096868(map->frontier);

    if (index == -1) {
        for (i = 0; i < 32; i++) {
            if (objs[i].obj == NULL) {
                break;
            }
        }
        GF_ASSERT(i != 32);
        slot = i;
    } else {
        slot = index;
    }

    params.unk0 = data->unk06;
    params.unk2 = data->unk08;
    params.unk4 = data->unk04;
    params.unk6 = data->unk02;
    params.unk8 = data->unk0A;
    params.unkA = data->unk00;

    obj = ov42_022280B8(map->unk14, &params);
    sprite = ov42_0222903C(map->unk20, obj, 0, HEAP_ID_101);
    ov42_02229200(sprite, data->unk0B);
    ov80_02239914(map->frontier, slot, obj, sprite, data);
    return obj;
}

void ov80_02239590(FrontierMap *map, UnkStruct_ov42_02228110 *obj) {
    int i;
    FrontierMapObj *objs = sub_02096868(map->frontier);

    for (i = 0; i < 32; i++) {
        if (objs[i].obj == obj) {
            ov42_02228100(objs[i].obj);
            ov42_022290C4(objs[i].sprite);
            GF_ASSERT(objs[i].movementTask == NULL);
            MI_CpuFill8(&objs[i], 0, sizeof(FrontierMapObj));
            objs[i].data.unk04 = 0xFFFF;
            return;
        }
    }
}

void ov80_022395E8(FrontierMap *map, u16 id, UnkStruct_ov42_02228110 **outObj, UnkStruct_ov42_0222903C **outSprite) {
    int i;
    FrontierMapObj *objs = sub_02096868(map->frontier);

    for (i = 0; i < 32; i++) {
        if (objs[i].data.unk04 == id) {
            if (outObj != NULL) {
                *outObj = objs[i].obj;
            }
            if (outSprite != NULL) {
                *outSprite = objs[i].sprite;
            }
            return;
        }
    }

    GF_ASSERT(FALSE);
}

void ov80_0223962C(FrontierMap *map, u16 resId) {
    int i;

    for (i = 0; i < 8; i++) {
        if (map->spr.resIds[i] == 0xFFFF) {
            map->spr.resIds[i] = resId;
            return;
        }
    }

    GF_ASSERT(FALSE);
}

void ov80_0223965C(FrontierMap *map, u16 resId) {
    int i;

    for (i = 0; i < 8; i++) {
        if (map->spr.resIds[i] == resId) {
            map->spr.resIds[i] = 0xFFFF;
            return;
        }
    }
}

ManagedSprite *ov80_0223968C(FrontierMap *map, u16 index, u16 resId) {
    ManagedSprite *sprite;

    GF_ASSERT(index < 8);
    GF_ASSERT(map->spr.sprites[index] == NULL);

    sprite = ov80_02239BB8(map->spriteSystem, map->spriteManager, resId);
    map->spr.sprites[index] = sprite;
    map->spr.spriteResIds[index] = resId;
    ov80_02239708(map, index, 0);
    return sprite;
}

void ov80_022396D8(FrontierMap *map, u16 index) {
    GF_ASSERT(index < 8);
    GF_ASSERT(map->spr.sprites[index] != NULL);

    ov80_02239BE8(map->spr.sprites[index]);
    map->spr.sprites[index] = NULL;
}

ManagedSprite *ov80_02239700(FrontierMap *map, u16 index) {
    return map->spr.sprites[index];
}

void ov80_02239708(FrontierMap *map, u16 index, int ticking) {
    if (ticking == 1) {
        map->spr.tickFlags |= 1 << index;
    } else {
        map->spr.tickFlags &= 0xFFFFFFFF ^ (1 << index);
    }
}

u32 ov80_02239734(FrontierMap *map, u16 index) {
    return (map->spr.tickFlags >> index) & 1;
}

static void ov80_02239740(FrontierMap *map) {
    int i;
    FrontierMapSavedSprites *saved = sub_02096878(map->frontier);
    FrontierMapSprites *sprites = &map->spr;

    for (i = 0; i < 8; i++) {
        if (sprites->resIds[i] != 0xFFFF) {
            saved->resIds[i] = sprites->resIds[i];
            i++;
        }
    }

    i = 0;

    for (i = 0; i < 8; i++) {
        if (sprites->sprites[i] != NULL) {
            saved->sprites[i].anim = ManagedSprite_GetActiveAnim(sprites->sprites[i]);
            saved->sprites[i].frame = ManagedSprite_GetAnimationFrame(sprites->sprites[i]);
            saved->sprites[i].ticking = ov80_02239734(map, i);
            saved->sprites[i].drawFlag = ManagedSprite_GetDrawFlag(sprites->sprites[i]);
            saved->sprites[i].resId = sprites->spriteResIds[i];
            ManagedSprite_GetPositionXY(sprites->sprites[i], &saved->sprites[i].x, &saved->sprites[i].y);
            saved->sprites[i].active = TRUE;
        }
    }
}

static void ov80_02239828(FrontierMap *map) {
    int i;
    NARC *narc;
    FrontierMapSavedSprites *saved;
    ManagedSprite *sprite;

    saved = sub_02096878(map->frontier);
    narc = NARC_New(NARC_a_1_8_4, HEAP_ID_101);

    for (i = 0; i < 8; i++) {
        if (saved->resIds[i] != 0xFFFF) {
            ov80_02239AF8(map->spriteSystem, map->spriteManager, narc, map->paletteData, saved->resIds[i]);
            ov80_0223962C(map, saved->resIds[i]);
        }
    }

    for (i = 0; i < 8; i++) {
        if (saved->sprites[i].active == TRUE) {
            sprite = ov80_0223968C(map, i, saved->sprites[i].resId);
            ManagedSprite_SetPositionXY(sprite, saved->sprites[i].x, saved->sprites[i].y);
            ManagedSprite_SetDrawFlag(sprite, saved->sprites[i].drawFlag);
            ov80_02239708(map, i, saved->sprites[i].ticking);
            ManagedSprite_SetAnim(sprite, saved->sprites[i].anim);
            ManagedSprite_SetAnimationFrame(sprite, saved->sprites[i].frame);
        }
    }

    NARC_Delete(narc);
    sub_02096884(map->frontier);
}

void ov80_022398E4(FrontierMap *map, s16 *x, s16 *y) {
    *y = ov42_022293A8(&map->unk1C);
    *x = ov42_022293B0(&map->unk1C);
}

static void ov80_02239900(FrontierMapObj *obj, FrontierMapObjData *out) {
    *out = obj->data;
}

static void ov80_02239914(void *frontier, int index, UnkStruct_ov42_02228110 *obj, UnkStruct_ov42_0222903C *sprite, const FrontierMapObjData *data) {
    FrontierMapObj *entry = sub_0209686C(frontier, index);

    entry->obj = obj;
    entry->sprite = sprite;
    entry->data = *data;
}

FrontierMapObj *ov80_02239938(void *frontier, int id) {
    int i;
    FrontierMapObj *objs = sub_02096868(frontier);

    for (i = 0; i < 32; i++) {
        if (objs->obj != NULL && objs->data.unk04 == id) {
            return objs;
        }
        objs++;
    }

    GF_ASSERT(FALSE);
    return NULL;
}
