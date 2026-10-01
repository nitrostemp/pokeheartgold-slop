#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_02026E30.h"

typedef struct UnkOv41Point {
    s16 x;
    s16 y;
} UnkOv41Point;

typedef struct UnkOv41SpriteTemplate {
    void *unk00;  // 0x00
    void *unk04;  // 0x04
    void *unk08;  // 0x08
    s16 x;        // 0x0C
    s16 y;        // 0x0E
    s16 z;        // 0x10
    int unk14;    // 0x14
    int unk18;    // 0x18
    s16 priority; // 0x1C
} UnkOv41SpriteTemplate;

typedef struct UnkOv41SystemTemplate {
    int maxSprites;     // 0x0
    int maxChars;       // 0x4
    int maxPltts;       // 0x8
    enum HeapID heapID; // 0xC
} UnkOv41SystemTemplate;

typedef struct UnkOv41Obj {
    int unk0;     // 0x0
    void *sprite; // 0x4
} UnkOv41Obj;

typedef struct UnkOv41ObjPool {
    UnkOv41Obj *objs; // 0x0
    int count;        // 0x4
} UnkOv41ObjPool;

typedef struct UnkOv41ObjTemplate {
    UnkOv41ObjPool *pool; // 0x00
    void *unk04;          // 0x04
    void *unk08;          // 0x08
    void *unk0C;          // 0x0C
    int x;                // 0x10
    int y;                // 0x14
    int unk18;            // 0x18
    int priority;         // 0x1C
} UnkOv41ObjTemplate;

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

typedef struct UnkOv41Gfx {
    void *system;                   // 0x000
    void **charHandles;             // 0x004
    int maxCharHandles;             // 0x008
    int numCharHandles;             // 0x00C
    void **plttHandles;             // 0x010
    int maxPlttHandles;             // 0x014
    int numPlttHandles;             // 0x018
    BOOL systemActive;              // 0x01C
    PokepicManager *pokepicManager; // 0x020
    NNSGfdTexKey texKey;            // 0x024
    NNSGfdPlttKey plttKey;          // 0x028
    BOOL pokepicActive;             // 0x02C
    void *unk030;                   // 0x030
    GF_2DGfxRawResMan *rawResMan;   // 0x034
    NNSG2dCharacterData **charData; // 0x038
    int numCharData;                // 0x03C
    BgConfig *bgConfig;             // 0x040
    SpriteList *spriteList;         // 0x044
    GF_2DGfxResMan *resMans[4];     // 0x048
    G2dRenderer renderer;           // 0x058
    NARC *narc;                     // 0x180
} UnkOv41Gfx;

void NNS_GfdResetFrmTexVramState(void);
void NNS_GfdResetFrmPlttVramState(void);

void *sub_02015DDC(const UnkOv41SystemTemplate *tmpl);
void sub_02015E20(void *system);
void sub_02015E64(void *system);
void *sub_02015EA0(UnkOv41CharEntry *entry);
void sub_02015EF4(void *system);
void *sub_02015F1C(UnkOv41PlttEntry *entry);
void sub_02015F64(void *system);
void *sub_02015F8C(UnkOv41SpriteTemplate *tmpl);
void sub_02015FB0(void *sprite, int a1);
void sub_02015FC4(void *sprite, int x, int y);
UnkOv41Point sub_02015FCC(void *sprite);
UnkOv41Point sub_02015FE8(void *sprite);
void sub_02015FF4(void *sprite, int priority);

void ov41_02246B34(UnkOv41Gfx *gfx);
void ov41_02246B5C(UnkOv41Gfx *gfx);
void ov41_02246B68(UnkOv41Gfx *gfx, UnkOv41GfxTables *tables);
void ov41_02246BEC(UnkOv41Gfx *gfx, UnkOv41GfxTables *tables);
void ov41_02246C90(UnkOv41Gfx *gfx, enum HeapID heapID);
void ov41_02246CB0(UnkOv41Gfx *gfx);
void ov41_02246CC0(UnkOv41Gfx *gfx, enum HeapID heapID, u32 texSize, u32 plttSize);
void ov41_02246D2C(UnkOv41Gfx *gfx);
void ov41_02246D54(UnkOv41GfxTables *tables, int numChars, int numPltts, enum HeapID heapID);
void ov41_02246DA8(UnkOv41GfxTables *tables);

UnkOv41ObjPool *ov41_02245EA0(int count, enum HeapID heapID);
void ov41_02245ECC(UnkOv41ObjPool *pool);
UnkOv41Obj *ov41_02245EE0(UnkOv41ObjTemplate *tmpl);
BOOL ov41_02245F04(UnkOv41Obj *obj);
BOOL ov41_02245F14(UnkOv41Obj *obj, int *dx, int *dy, NNSG2dCharacterData *charData);
void ov41_02245F9C(UnkOv41Obj *obj, int x, int y);
void ov41_02245FA8(UnkOv41Obj *obj, int *x, int *y);
void ov41_02245FD8(UnkOv41Obj *obj, int *w, int *h);
void ov41_02246008(UnkOv41Obj *obj, int a1);
void ov41_02246014(UnkOv41Obj *obj, int priority);
void ov41_02246020(UnkOv41Obj *obj, int *outW, int *outH);
static UnkOv41Obj *ov41_0224607C(UnkOv41ObjPool *pool);
static void *ov41_022460A8(UnkOv41ObjTemplate *tmpl);
static void ov41_022460DC(UnkOv41Obj *obj, TouchscreenHitbox *hitbox);
void ov41_02246130(UnkOv41Gfx *gfx);
void ov41_02246150(void);
void ov41_02246170(UnkOv41Gfx *gfx);
void ov41_022461D0(UnkOv41Gfx *gfx);
void ov41_0224621C(UnkOv41Gfx *gfx);
void ov41_02246250(UnkOv41Gfx *gfx, UnkOv41GfxTables *tables);
void ov41_0224626C(UnkOv41Gfx *gfx);
Sprite *ov41_02246280(UnkOv41Gfx *gfx, int id, int x, int y, int priority, NNS_G2D_VRAM_TYPE whichScreen);
void ov41_022462E4(UnkOv41Gfx *gfx, NARC *narc, int fileId, BOOL compressed, int vram, int id);
void ov41_02246304(UnkOv41Gfx *gfx, NARC *narc, int fileId, BOOL compressed, int vram, int count, int id);
void ov41_02246328(UnkOv41Gfx *gfx, NARC *narc, int fileId, BOOL compressed, int id);
void ov41_02246344(UnkOv41Gfx *gfx, NARC *narc, int fileId, BOOL compressed, int id);
void ov41_02246360(UnkOv41Gfx *gfx, int id);
void ov41_02246374(UnkOv41Gfx *gfx, int id);
void ov41_02246388(UnkOv41Gfx *gfx, int id);
void ov41_0224639C(UnkOv41Gfx *gfx, int id);
void ov41_022463B0(UnkOv41Gfx *gfx, UnkOv41GfxTables *tables);
void ov41_022463D4(UnkOv41GfxTables *tables);
NNSG2dCharacterData *ov41_022463DC(UnkOv41Gfx *gfx, void *file, int idx);
void ov41_022463FC(void);
void ov41_0224642C(void);
void ov41_02246494(UnkOv41Gfx *gfx);
void ov41_022464AC(UnkOv41GfxTables *tables, enum HeapID heapID);
int ov41_022464BC(NNSG2dCharacterData *charData, int x, int y, int color);
void ov41_02246518(UnkOv41Gfx *gfx, const UnkOv41SystemTemplate *tmpl, enum HeapID heapID);
void ov41_02246544(UnkOv41Gfx *gfx, BgConfig *bgConfig, enum HeapID heapID);
void ov41_02246594(UnkOv41Gfx *gfx);
void ov41_022465C0(UnkOv41Gfx *gfx);
void ov41_022465CC(UnkOv41Gfx *gfx);
void ov41_022465D8(UnkOv41Gfx *gfx, int x, int y, u16 angle, const VecFx32 *scale);
void ov41_02246670(UnkOv41Gfx *gfx);
void ov41_02246698(UnkOv41Gfx *gfx);
void ov41_022466B8(UnkOv41Gfx *gfx);
void ov41_022466C8(UnkOv41Gfx *gfx);
static void ov41_022466D0(void);
static void ov41_022466F0(void);
static void ov41_02246778(void);
static void ov41_022467C8(void);
static void ov41_022467D4(void);
static void ov41_022467E4(UnkOv41Gfx *gfx, const UnkOv41SystemTemplate *tmpl);
static void ov41_02246820(UnkOv41Gfx *gfx);
static void ov41_02246830(UnkOv41Gfx *gfx);
static void ov41_0224683C(UnkOv41Gfx *gfx, UnkOv41CharEntry *entries, int count);
static void ov41_0224689C(UnkOv41Gfx *gfx, UnkOv41PlttEntry *entries, int count);
static void ov41_022468FC(UnkOv41Gfx *gfx);
static void ov41_02246A20(UnkOv41Gfx *gfx);
static void ov41_02246A50(UnkOv41Gfx *gfx);
static void ov41_02246A7C(UnkOv41Gfx *gfx);
static void ov41_02246A94(UnkOv41Gfx *gfx);

static inline void G3_Scale(fx32 x, fx32 y, fx32 z) {
    reg_G3_MTX_SCALE = (u32)x;
    reg_G3_MTX_SCALE = (u32)y;
    reg_G3_MTX_SCALE = (u32)z;
}

UnkOv41ObjPool *ov41_02245EA0(int count, enum HeapID heapID) {
    UnkOv41ObjPool *pool = Heap_Alloc(heapID, sizeof(UnkOv41ObjPool));

    pool->objs = Heap_Alloc(heapID, count * sizeof(UnkOv41Obj));
    memset(pool->objs, 0, count * sizeof(UnkOv41Obj));
    pool->count = count;
    return pool;
}

void ov41_02245ECC(UnkOv41ObjPool *pool) {
    Heap_Free(pool->objs);
    Heap_Free(pool);
}

UnkOv41Obj *ov41_02245EE0(UnkOv41ObjTemplate *tmpl) {
    UnkOv41Obj *obj = ov41_0224607C(tmpl->pool);

    GF_ASSERT(obj != NULL);
    obj->unk0 = tmpl->unk18;
    obj->sprite = ov41_022460A8(tmpl);
    return obj;
}

BOOL ov41_02245F04(UnkOv41Obj *obj) {
    TouchscreenHitbox hitbox;

    ov41_022460DC(obj, &hitbox);
    return TouchscreenHitbox_TouchHeldIsIn(&hitbox);
}

BOOL ov41_02245F14(UnkOv41Obj *obj, int *dx, int *dy, NNSG2dCharacterData *charData) {
    int x, y;
    int i, j;

    if (!ov41_02245F04(obj)) {
        return FALSE;
    }
    ov41_02245FA8(obj, &x, &y);
    *dx = gSystem.touchX - x;
    *dy = gSystem.touchY - y;
    for (i = *dy - 4; i < *dy + 4; i++) {
        if (i < 0) {
            continue;
        }
        for (j = *dx - 4; j < *dx + 4; j++) {
            if (j < 0) {
                continue;
            }
            if (ov41_022464BC(charData, j, i, 0) == 0) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void ov41_02245F9C(UnkOv41Obj *obj, int x, int y) {
    sub_02015FC4(obj->sprite, x, y);
}

void ov41_02245FA8(UnkOv41Obj *obj, int *x, int *y) {
    UnkOv41Point pos = sub_02015FCC(obj->sprite);

    *x = pos.x;
    *y = pos.y;
}

void ov41_02245FD8(UnkOv41Obj *obj, int *w, int *h) {
    UnkOv41Point size = sub_02015FE8(obj->sprite);

    *w = size.x;
    *h = size.y;
}

void ov41_02246008(UnkOv41Obj *obj, int a1) {
    sub_02015FB0(obj->sprite, a1);
}

void ov41_02246014(UnkOv41Obj *obj, int priority) {
    sub_02015FF4(obj->sprite, priority);
}

void ov41_02246020(UnkOv41Obj *obj, int *outW, int *outH) {
    int w, h;

    ov41_02245FD8(obj, &w, &h);
    switch (w) {
    case 16:
        *outW = 0;
        break;
    case 32:
        *outW = 10;
        break;
    case 64:
        *outW = 20;
        break;
    }
    switch (h) {
    case 16:
        *outH = 0;
        break;
    case 32:
        *outH = 10;
        break;
    case 64:
        *outH = 20;
        break;
    }
}

static UnkOv41Obj *ov41_0224607C(UnkOv41ObjPool *pool) {
    int i;

    for (i = 0; i < pool->count; i++) {
        if (pool->objs[i].sprite == NULL) {
            return &pool->objs[i];
        }
    }
    return NULL;
}

static void *ov41_022460A8(UnkOv41ObjTemplate *tmpl) {
    UnkOv41SpriteTemplate spriteTmpl;

    spriteTmpl.unk00 = tmpl->unk04;
    spriteTmpl.unk04 = tmpl->unk08;
    spriteTmpl.unk08 = tmpl->unk0C;
    spriteTmpl.x = tmpl->x;
    spriteTmpl.y = tmpl->y;
    spriteTmpl.z = 0;
    spriteTmpl.unk14 = 31;
    spriteTmpl.unk18 = 0;
    spriteTmpl.priority = tmpl->priority;
    return sub_02015F8C(&spriteTmpl);
}

static void ov41_022460DC(UnkOv41Obj *obj, TouchscreenHitbox *hitbox) {
    UnkOv41Point pos = sub_02015FCC(obj->sprite);
    UnkOv41Point size = sub_02015FE8(obj->sprite);

    hitbox->rect.top = pos.y;
    hitbox->rect.bottom = pos.y + size.y;
    hitbox->rect.left = pos.x;
    hitbox->rect.right = pos.x + size.x;
}

void ov41_02246130(UnkOv41Gfx *gfx) {
    ov41_022466D0();
    ov41_022466F0();
    ov41_02246778();
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
}

void ov41_02246150(void) {
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    ov41_022467D4();
    ov41_022467C8();
    GX_ResetBankForTex();
}

void ov41_02246170(UnkOv41Gfx *gfx) {
    UnkOv41SystemTemplate tmpl = { 718, 118, 19, HEAP_ID_14 };

    gfx->narc = NARC_New(NARC_a_0_2_6, HEAP_ID_14);
    ov41_022467E4(gfx, &tmpl);
    ov41_02246CC0(gfx, HEAP_ID_14, 0x2800, 0x20);
    ov41_02246A50(gfx);
    ov41_02246C90(gfx, HEAP_ID_13);
    ov41_02246A94(gfx);
    gfx->bgConfig = BgConfig_Alloc(HEAP_ID_14);
    ov41_022468FC(gfx);
}

void ov41_022461D0(UnkOv41Gfx *gfx) {
    ov41_02246CB0(gfx);
    ov41_02246820(gfx);
    ov41_02246A20(gfx);
    Heap_Free(gfx->bgConfig);
    ov41_02246D2C(gfx);
    ov41_02246B34(gfx);
    ov41_02246A7C(gfx);
    NARC_Delete(gfx->narc);
    Heap_Free(gfx->charHandles);
    gfx->charHandles = NULL;
    Heap_Free(gfx->plttHandles);
    gfx->plttHandles = NULL;
}

void ov41_0224621C(UnkOv41Gfx *gfx) {
    Thunk_G3X_Reset();
    NNS_G2dSetupSoftwareSpriteCamera();
    if (gfx->systemActive) {
        ov41_02246830(gfx);
    }
    if (gfx->pokepicActive) {
        PokepicManager_DrawAll(gfx->pokepicManager);
    }
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    ov41_02246B5C(gfx);
}

void ov41_02246250(UnkOv41Gfx *gfx, UnkOv41GfxTables *tables) {
    ov41_0224683C(gfx, tables->chars, tables->numChars);
    ov41_0224689C(gfx, tables->pltts, tables->numPltts);
}

void ov41_0224626C(UnkOv41Gfx *gfx) {
    sub_02015EF4(gfx->system);
    sub_02015F64(gfx->system);
}

Sprite *ov41_02246280(UnkOv41Gfx *gfx, int id, int x, int y, int priority, NNS_G2D_VRAM_TYPE whichScreen) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate tmpl;

    CreateSpriteResourcesHeader(&header, id, id, id, id, -1, -1, 0, 0, gfx->resMans[0], gfx->resMans[1], gfx->resMans[2], gfx->resMans[3], NULL, NULL);
    tmpl.spriteList = gfx->spriteList;
    tmpl.header = &header;
    tmpl.position.x = x << FX32_SHIFT;
    tmpl.position.y = y << FX32_SHIFT;
    tmpl.position.z = 0;
    tmpl.priority = priority;
    tmpl.whichScreen = whichScreen;
    tmpl.heapID = HEAP_ID_14;
    return Sprite_Create(&tmpl);
}

void ov41_022462E4(UnkOv41Gfx *gfx, NARC *narc, int fileId, BOOL compressed, int vram, int id) {
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(AddCharResObjFromOpenNarc(gfx->resMans[0], narc, fileId, compressed, id, vram, HEAP_ID_14));
}

void ov41_02246304(UnkOv41Gfx *gfx, NARC *narc, int fileId, BOOL compressed, int vram, int count, int id) {
    SpriteTransfer_CreatePlttTransferTask(AddPlttResObjFromOpenNarc(gfx->resMans[1], narc, fileId, compressed, id, vram, count, HEAP_ID_14));
}

void ov41_02246328(UnkOv41Gfx *gfx, NARC *narc, int fileId, BOOL compressed, int id) {
    AddCellOrAnimResObjFromOpenNarc(gfx->resMans[2], narc, fileId, compressed, id, GF_GFX_RES_TYPE_CELL, HEAP_ID_14);
}

void ov41_02246344(UnkOv41Gfx *gfx, NARC *narc, int fileId, BOOL compressed, int id) {
    AddCellOrAnimResObjFromOpenNarc(gfx->resMans[3], narc, fileId, compressed, id, GF_GFX_RES_TYPE_ANIM, HEAP_ID_14);
}

void ov41_02246360(UnkOv41Gfx *gfx, int id) {
    DestroySingle2DGfxResObj(gfx->resMans[0], SpriteResourceCollection_Find(gfx->resMans[0], id));
}

void ov41_02246374(UnkOv41Gfx *gfx, int id) {
    DestroySingle2DGfxResObj(gfx->resMans[1], SpriteResourceCollection_Find(gfx->resMans[1], id));
}

void ov41_02246388(UnkOv41Gfx *gfx, int id) {
    DestroySingle2DGfxResObj(gfx->resMans[2], SpriteResourceCollection_Find(gfx->resMans[2], id));
}

void ov41_0224639C(UnkOv41Gfx *gfx, int id) {
    DestroySingle2DGfxResObj(gfx->resMans[3], SpriteResourceCollection_Find(gfx->resMans[3], id));
}

void ov41_022463B0(UnkOv41Gfx *gfx, UnkOv41GfxTables *tables) {
    ov41_02246D54(tables, 118, 19, HEAP_ID_14);
    ov41_02246B68(gfx, tables);
    ov41_02246BEC(gfx, tables);
}

void ov41_022463D4(UnkOv41GfxTables *tables) {
    ov41_02246DA8(tables);
}

NNSG2dCharacterData *ov41_022463DC(UnkOv41Gfx *gfx, void *file, int idx) {
    GF2dGfxRawResMan_AllocObj(gfx->rawResMan, file, idx);
    NNS_G2dGetUnpackedCharacterData(file, &gfx->charData[idx]);
    return gfx->charData[idx];
}

void ov41_022463FC(void) {
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    G2_SetBG0Priority(1);
    G2_SetBG1Priority(0);
}

void ov41_0224642C(void) {
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_OBJ, FALSE);
    G2_SetWnd0Position(10, 18, 246, 143);
    G2_SetBG0Priority(0);
    G2_SetBG1Priority(1);
}

void ov41_02246494(UnkOv41Gfx *gfx) {
    DoScheduledBgGpuUpdates(gfx->bgConfig);
    PokepicManager_HandleLoadImgAndOrPltt(gfx->pokepicManager);
    OamManager_ApplyAndResetBuffers();
}

void ov41_022464AC(UnkOv41GfxTables *tables, enum HeapID heapID) {
    ov41_02246D54(tables, 118, 19, heapID);
}

int ov41_022464BC(NNSG2dCharacterData *charData, int x, int y, int color) {
    int width = charData->W;
    int height = charData->H;

    width *= 8;
    int idx;
    int shift;
    u32 pixels;

    if (x < 0 || y < 0 || x >= width || y >= height * 8) {
        return 2;
    }
    idx = x + y * width;
    shift = (idx % 8) * 4;
    pixels = ((u32 *)charData->pRawData)[idx / 8];
    if ((color << shift) == (pixels & (0xF << shift))) {
        return 1;
    }
    return 0;
}

void ov41_02246518(UnkOv41Gfx *gfx, const UnkOv41SystemTemplate *tmpl, enum HeapID heapID) {
    ov41_022467E4(gfx, tmpl);
    ov41_02246CC0(gfx, heapID, 0x2800, 0x20);
    PokepicManager_SetNeedG3IdentityFlag(gfx->pokepicManager, TRUE);
    ov41_02246C90(gfx, heapID);
}

void ov41_02246544(UnkOv41Gfx *gfx, BgConfig *bgConfig, enum HeapID heapID) {
    gfx->bgConfig = bgConfig;
    BgTemplate bgTemplate = {
        0,
        0,
        GF_BG_BUF_SIZE_256x256_4BPP,
        0,
        GF_BG_SCR_SIZE_256x256,
        GX_BG_COLORMODE_16,
        GX_BG_SCRBASE_0xf000,
        GX_BG_CHARBASE_0x04000,
        GX_BG_EXTPLTT_01,
        2,
        GX_BG_AREAOVER_XLU,
        0,
        FALSE,
    };
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    InitBgFromTemplate(gfx->bgConfig, GF_BG_LYR_MAIN_2, &bgTemplate, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, heapID);
    BgClearTilemapBufferAndCommit(gfx->bgConfig, GF_BG_LYR_MAIN_2);
}

void ov41_02246594(UnkOv41Gfx *gfx) {
    ov41_02246CB0(gfx);
    ov41_02246820(gfx);
    ov41_02246D2C(gfx);
    Heap_Free(gfx->charHandles);
    gfx->charHandles = NULL;
    Heap_Free(gfx->plttHandles);
    gfx->plttHandles = NULL;
}

void ov41_022465C0(UnkOv41Gfx *gfx) {
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
}

void ov41_022465CC(UnkOv41Gfx *gfx) {
    PokepicManager_HandleLoadImgAndOrPltt(gfx->pokepicManager);
}

void ov41_022465D8(UnkOv41Gfx *gfx, int x, int y, u16 angle, const VecFx32 *scale) {
    G3_Identity();
    G3_PushMtx();
    NNS_G2dSetupSoftwareSpriteCamera();
    G3_Translate(x << FX32_SHIFT, y << FX32_SHIFT, 0);
    G3_RotZ(FX_SinIdx(angle), FX_CosIdx(angle));
    G3_Scale(scale->x, scale->y, scale->z);
    G3_Translate(-x << FX32_SHIFT, -y << FX32_SHIFT, 0);
    G3_PushMtx();
    if (gfx->systemActive) {
        ov41_02246830(gfx);
    }
    if (gfx->pokepicActive) {
        PokepicManager_DrawAll(gfx->pokepicManager);
    }
    G3_PopMtx(1);
    G3_PopMtx(1);
}

void ov41_02246670(UnkOv41Gfx *gfx) {
    gfx->narc = NARC_New(NARC_a_0_2_6, HEAP_ID_14);
    ov41_02246A94(gfx);
    gfx->bgConfig = BgConfig_Alloc(HEAP_ID_14);
    ov41_022468FC(gfx);
}

void ov41_02246698(UnkOv41Gfx *gfx) {
    ov41_02246A20(gfx);
    Heap_Free(gfx->bgConfig);
    NARC_Delete(gfx->narc);
    ov41_02246B34(gfx);
}

void ov41_022466B8(UnkOv41Gfx *gfx) {
    DoScheduledBgGpuUpdates(gfx->bgConfig);
    OamManager_ApplyAndResetBuffers();
}

void ov41_022466C8(UnkOv41Gfx *gfx) {
    ov41_02246B5C(gfx);
}

static void ov41_022466D0(void) {
    GraphicsBanks banks = {
        GX_VRAM_BG_128_C,
        GX_VRAM_BGEXTPLTT_NONE,
        GX_VRAM_SUB_BG_32_H,
        GX_VRAM_SUB_BGEXTPLTT_NONE,
        GX_VRAM_OBJ_32_FG,
        GX_VRAM_OBJEXTPLTT_NONE,
        GX_VRAM_SUB_OBJ_16_I,
        GX_VRAM_SUB_OBJEXTPLTT_NONE,
        GX_VRAM_TEX_01_AB,
        GX_VRAM_TEXPLTT_0123_E,
    };

    GfGfx_SetBanks(&banks);
}

static void ov41_022466F0(void) {
    NNS_G3dInit();
    G3X_InitMtxStack();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_SetClearColor(RGB_BLACK, 0, 0x7FFF, 0x3F, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(2, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
}

static void ov41_02246778(void) {
    GraphicsModes modes = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };

    SetBothScreensModesAndDisable(&modes);
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    NNS_G2dInitOamManagerModule();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void ov41_022467C8(void) {
    NNS_GfdResetFrmTexVramState();
    NNS_GfdResetFrmPlttVramState();
}

static void ov41_022467D4(void) {
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    NNS_G2dInitOamManagerModule();
}

static void ov41_022467E4(UnkOv41Gfx *gfx, const UnkOv41SystemTemplate *tmpl) {
    gfx->system = sub_02015DDC(tmpl);
    gfx->charHandles = Heap_Alloc(tmpl->heapID, 118 * sizeof(void *));
    gfx->maxCharHandles = 118;
    gfx->numCharHandles = 0;
    gfx->plttHandles = Heap_Alloc(tmpl->heapID, 19 * sizeof(void *));
    gfx->maxPlttHandles = 19;
    gfx->numPlttHandles = 0;
    gfx->systemActive = TRUE;
}

static void ov41_02246820(UnkOv41Gfx *gfx) {
    sub_02015E20(gfx->system);
    gfx->system = NULL;
}

static void ov41_02246830(UnkOv41Gfx *gfx) {
    sub_02015E64(gfx->system);
}

static void ov41_0224683C(UnkOv41Gfx *gfx, UnkOv41CharEntry *entries, int count) {
    int i;
    UnkOv41CharEntry *entry = entries;

    for (i = 0; i < count; i++) {
        GF_ASSERT(gfx->numCharHandles < gfx->maxCharHandles);
        if (entry->charData != NULL) {
            gfx->charHandles[gfx->numCharHandles] = sub_02015EA0(&entries[i]);
        } else {
            gfx->charHandles[gfx->numCharHandles] = NULL;
        }
        gfx->numCharHandles++;
        entry++;
    }
}

static void ov41_0224689C(UnkOv41Gfx *gfx, UnkOv41PlttEntry *entries, int count) {
    int i;
    UnkOv41PlttEntry *entry = entries;

    for (i = 0; i < count; i++) {
        GF_ASSERT(gfx->numPlttHandles < gfx->maxPlttHandles);
        if (entry->pltt != NULL) {
            gfx->plttHandles[gfx->numPlttHandles] = sub_02015F1C(&entries[i]);
        } else {
            gfx->plttHandles[gfx->numPlttHandles] = NULL;
        }
        gfx->numPlttHandles++;
        entry++;
    }
}

static void ov41_022468FC(UnkOv41Gfx *gfx) {
    BgTemplate bgTemplate1 = {
        0,
        0,
        GF_BG_BUF_SIZE_256x256_4BPP,
        0,
        GF_BG_SCR_SIZE_256x256,
        GX_BG_COLORMODE_16,
        GX_BG_SCRBASE_0xf800,
        GX_BG_CHARBASE_0x00000,
        GX_BG_EXTPLTT_01,
        0,
        GX_BG_AREAOVER_XLU,
        0,
        FALSE,
    };
    InitBgFromTemplate(gfx->bgConfig, GF_BG_LYR_MAIN_1, &bgTemplate1, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_14);
    BgClearTilemapBufferAndCommit(gfx->bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate bgTemplate2 = {
        0,
        0,
        GF_BG_BUF_SIZE_256x256_4BPP,
        0,
        GF_BG_SCR_SIZE_256x256,
        GX_BG_COLORMODE_16,
        GX_BG_SCRBASE_0xf000,
        GX_BG_CHARBASE_0x04000,
        GX_BG_EXTPLTT_01,
        2,
        GX_BG_AREAOVER_XLU,
        0,
        FALSE,
    };
    InitBgFromTemplate(gfx->bgConfig, GF_BG_LYR_MAIN_2, &bgTemplate2, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, HEAP_ID_14);
    BgClearTilemapBufferAndCommit(gfx->bgConfig, GF_BG_LYR_MAIN_2);

    BgTemplate bgTemplate3 = {
        0,
        -145,
        GF_BG_BUF_SIZE_256x256_4BPP,
        0,
        GF_BG_SCR_SIZE_256x256,
        GX_BG_COLORMODE_16,
        GX_BG_SCRBASE_0xe800,
        GX_BG_CHARBASE_0x08000,
        GX_BG_EXTPLTT_01,
        3,
        GX_BG_AREAOVER_XLU,
        0,
        FALSE,
    };
    InitBgFromTemplate(gfx->bgConfig, GF_BG_LYR_MAIN_3, &bgTemplate3, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, HEAP_ID_14);
    BgClearTilemapBufferAndCommit(gfx->bgConfig, GF_BG_LYR_MAIN_3);

    BgTemplate bgTemplate4 = {
        0,
        0,
        GF_BG_BUF_SIZE_256x256_4BPP,
        0,
        GF_BG_SCR_SIZE_256x256,
        GX_BG_COLORMODE_16,
        GX_BG_SCRBASE_0x7800,
        GX_BG_CHARBASE_0x00000,
        GX_BG_EXTPLTT_01,
        1,
        GX_BG_AREAOVER_XLU,
        0,
        FALSE,
    };
    InitBgFromTemplate(gfx->bgConfig, GF_BG_LYR_SUB_0, &bgTemplate4, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_14);
    BgClearTilemapBufferAndCommit(gfx->bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate bgTemplate5 = {
        0,
        0,
        GF_BG_BUF_SIZE_256x256_4BPP,
        0,
        GF_BG_SCR_SIZE_256x256,
        GX_BG_COLORMODE_16,
        GX_BG_SCRBASE_0x7000,
        GX_BG_CHARBASE_0x04000,
        GX_BG_EXTPLTT_01,
        0,
        GX_BG_AREAOVER_XLU,
        0,
        FALSE,
    };
    InitBgFromTemplate(gfx->bgConfig, GF_BG_LYR_SUB_1, &bgTemplate5, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_14);
    BgClearTilemapBufferAndCommit(gfx->bgConfig, GF_BG_LYR_SUB_1);
}

static void ov41_02246A20(UnkOv41Gfx *gfx) {
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_SUB_1);
}

static void ov41_02246A50(UnkOv41Gfx *gfx) {
    gfx->rawResMan = GF2dGfxRawResMan_Create(119, HEAP_ID_14);
    gfx->charData = Heap_Alloc(HEAP_ID_14, 119 * sizeof(NNSG2dCharacterData *));
    memset(gfx->charData, 0, 119 * sizeof(NNSG2dCharacterData *));
    gfx->numCharData = 119;
}

static void ov41_02246A7C(UnkOv41Gfx *gfx) {
    Heap_Free(gfx->charData);
    GF2dGfxRawResObj_Destroy(gfx->rawResMan);
    gfx->numCharData = 0;
}

static void ov41_02246A94(UnkOv41Gfx *gfx) {
    ObjCharTransferTemplate tmpl = { 8, 0x8000, 0x4000, HEAP_ID_14 };

    ObjCharTransfer_InitEx(&tmpl, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(5, HEAP_ID_14);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 124, 0, 31, 0, 124, 0, 31, HEAP_ID_14);
    gfx->spriteList = G2dRenderer_Init(48, &gfx->renderer, HEAP_ID_14);
    G2dRenderer_SetSubSurfaceCoords(&gfx->renderer, 0, FX32_CONST(512));
    gfx->resMans[0] = Create2DGfxResObjMan(8, GF_GFX_RES_TYPE_CHAR, HEAP_ID_14);
    gfx->resMans[1] = Create2DGfxResObjMan(5, GF_GFX_RES_TYPE_PLTT, HEAP_ID_14);
    gfx->resMans[2] = Create2DGfxResObjMan(48, GF_GFX_RES_TYPE_CELL, HEAP_ID_14);
    gfx->resMans[3] = Create2DGfxResObjMan(48, GF_GFX_RES_TYPE_ANIM, HEAP_ID_14);
}
