#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "fashion_case.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_41.h"
#include "overlay_manager.h"
#include "pokemon.h"
#include "pokepic.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_02026E30.h"

typedef struct UnkOv41SystemTemplate {
    int maxSprites;     // 0x0
    int maxChars;       // 0x4
    int maxPltts;       // 0x8
    enum HeapID heapID; // 0xC
} UnkOv41SystemTemplate;

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
    u8 *unk030;                     // 0x030
    GF_2DGfxRawResMan *rawResMan;   // 0x034
    NNSG2dCharacterData **charData; // 0x038
    int numCharData;                // 0x03C
    BgConfig *bgConfig;             // 0x040
    SpriteList *spriteList;         // 0x044
    GF_2DGfxResMan *resMans[4];     // 0x048
    G2dRenderer renderer;           // 0x058
    NARC *narc;                     // 0x180
} UnkOv41Gfx;                       // size: 0x184

typedef struct UnkOv41ObjPool UnkOv41ObjPool;

typedef struct UnkOv41Pool {
    void *nodes; // 0x0
    int count;   // 0x4
} UnkOv41Pool;

typedef struct UnkOv41SpriteRes {
    SpriteResource *res[4]; // 0x00
} UnkOv41SpriteRes;

typedef struct UnkOv41Tween {
    fx32 cur;     // 0x00
    fx32 start;   // 0x04
    fx32 delta;   // 0x08
    int frame;    // 0x0C
    int duration; // 0x10
} UnkOv41Tween;   // size: 0x14

typedef struct UnkOv41DigitAnim {
    BOOL active;          // 0x00
    Sprite *sprites[2];   // 0x04
    VecFx32 pos[2];       // 0x0C
    UnkOv41Tween scale;   // 0x24
    UnkOv41Tween offsetX; // 0x38
    UnkOv41Tween offsetY; // 0x4C
} UnkOv41DigitAnim;       // size: 0x60

typedef struct UnkOv41PanelSub {
    UnkOv41SpriteRes res;  // 0x00
    Sprite *sprites[2];    // 0x10
    Window *window;        // 0x18
    int value;             // 0x1C
    int frames;            // 0x20
    SysTask *task0;        // 0x24
    SysTask *task1;        // 0x28
    int *valuePtr;         // 0x2C
    UnkOv41DigitAnim anim; // 0x30
    BOOL highlighted;      // 0x90
} UnkOv41PanelSub;         // size: 0x94

typedef struct UnkOv41BgScroll {
    BgConfig *bgConfig; // 0x00
    NarcId narcId;      // 0x04
    int scrnFile;       // 0x08
    int x;              // 0x0C
    int y;              // 0x10
    int width;          // 0x14
    int height;         // 0x18
    int bgId;           // 0x1C
    int plttSlot;       // 0x20
    int plttCount;      // 0x24
    int tileOffset;     // 0x28
} UnkOv41BgScroll;      // size: 0x2C

typedef struct UnkOv41BgScrollAnimTemplate {
    UnkOv41BgScroll *scroll; // 0x00
    int altScrnFile;         // 0x04
    int interval;            // 0x08
    enum HeapID heapID;      // 0x0C
} UnkOv41BgScrollAnimTemplate;

typedef struct UnkOv41BgScrollAnim {
    UnkOv41BgScroll *scroll; // 0x00
    int altScrnFile;         // 0x04
    int interval;            // 0x08
    int counter;             // 0x0C
    BOOL showAlt;            // 0x10
    enum HeapID heapID;      // 0x14
} UnkOv41BgScrollAnim;       // size: 0x18

typedef struct UnkOv41CanvasTemplate {
    void *unk00;                    // 0x00
    void **unk04;                   // 0x04
    void **unk08;                   // 0x08
    u8 *unk0C;                      // 0x0C
    PokepicManager *pokepicManager; // 0x10
    BgConfig *bgConfig;             // 0x14
    UnkOv41ObjPool *objPool;        // 0x18
    UnkOv41Pool *pool;              // 0x1C
    int maxObjs;                    // 0x20
} UnkOv41CanvasTemplate;

typedef struct UnkOv41Canvas {
    u8 unk00[0x48];
    UnkOv41BgScroll scroll; // 0x48
    int background;         // 0x74
    u8 unk78[0x10];
} UnkOv41Canvas; // size: 0x88

typedef struct UnkOv41CanvasAnim {
    UnkOv41Canvas *canvas;    // 0x00
    UnkOv41BgScrollAnim anim; // 0x04
} UnkOv41CanvasAnim;          // size: 0x1C

typedef struct UnkOv41Portrait {
    UnkOv41Gfx gfx;               // 0x000
    UnkOv41Pool pool;             // 0x184
    UnkOv41ObjPool *objPool;      // 0x18C
    UnkOv41Canvas canvas;         // 0x190
    UnkOv41CanvasAnim canvasAnim; // 0x218
    Pokemon *mon;                 // 0x234
    enum HeapID heapID;           // 0x238
    int x;                        // 0x23C
    int y;                        // 0x240
    int centerX;                  // 0x244
    int centerY;                  // 0x248
    u16 angle;                    // 0x24C
    VecFx32 scale;                // 0x250
    u16 unk25C;                   // 0x25C
    BOOL bgActive;                // 0x260
    BOOL drawActive;              // 0x264
} UnkOv41Portrait;                // size: 0x268

typedef struct UnkOv41PortraitTemplate {
    BgConfig *bgConfig; // 0x0
    int x;              // 0x4
    int y;              // 0x8
    enum HeapID heapID; // 0xC
} UnkOv41PortraitTemplate;

typedef struct UnkOv41PortraitArgs {
    void *mon;          // 0x00
    void *markers[20];  // 0x04
    int numMarkers;     // 0x54
    BgConfig *bgConfig; // 0x58
    int background;     // 0x5C
    int x;              // 0x60
    int y;              // 0x64
    enum HeapID heapID; // 0x68
} UnkOv41PortraitArgs;  // size: 0x6C

typedef struct AccessoryPortraitApp {
    SaveFashionDataSub *fashionSub; // 0x000
    int unk004;
    int unk008;                // 0x008
    int unk00C;                // 0x00C
    UnkOv41Portrait *portrait; // 0x010
    UnkOv41Gfx gfx;            // 0x014
    Sprite *sprite;            // 0x198
    Window *window;            // 0x19C
} AccessoryPortraitApp;        // size: 0x1A0

BOOL sub_0202BDEC(SaveFashionDataSub *sub, int idx);
void *sub_0202BE14(SaveFashionDataSub *sub);
void *sub_0202BE2C(SaveFashionDataSub *sub, int idx);
u8 sub_0202BE80(SaveFashionDataSub *sub);
s8 sub_0202BEDC(void *mon);
u8 sub_0202BEE4(void *mon);
u8 sub_0202BEEC(void *mon);
void sub_0202BEF4(void *mon, Pokemon *pokemon);
u8 sub_0202BEFC(void *marker);
u8 sub_0202BF00(void *marker);
u8 sub_0202BF04(void *marker);
s8 sub_0202BF08(void *marker);

UnkOv41ObjPool *ov41_02245EA0(int count, enum HeapID heapID);
void ov41_02245ECC(UnkOv41ObjPool *pool);
void ov41_02246130(void);
void ov41_02246150(void);
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
void ov41_022463D4(UnkOv41GfxTables *tables);
void ov41_022464AC(UnkOv41GfxTables *tables, enum HeapID heapID);
void ov41_02246518(UnkOv41Gfx *gfx, const UnkOv41SystemTemplate *tmpl, enum HeapID heapID);
void ov41_02246544(UnkOv41Gfx *gfx, BgConfig *bgConfig, enum HeapID heapID);
void ov41_02246594(UnkOv41Gfx *gfx);
void ov41_022465C0(UnkOv41Gfx *gfx);
void ov41_022465CC(UnkOv41Gfx *gfx);
void ov41_022465D8(UnkOv41Gfx *gfx, int x, int y, u16 angle, const VecFx32 *scale);
void ov41_02246670(UnkOv41Gfx *gfx, enum HeapID heapID);
void ov41_02246698(UnkOv41Gfx *gfx);
void ov41_022466B8(UnkOv41Gfx *gfx);
void ov41_022466C8(UnkOv41Gfx *gfx);
void ov41_02247F3C(UnkOv41Canvas *canvas, const UnkOv41CanvasTemplate *tmpl);
void ov41_02247F90(UnkOv41Canvas *canvas);
void ov41_02247FAC(UnkOv41Canvas *canvas, Pokemon *mon, PokepicTemplate *tmpl, int x, int y, int priority, enum HeapID heapID);
BOOL ov41_02248044(UnkOv41Canvas *canvas, int id, int x, int y, int priority);
void ov41_022480E0(UnkOv41Canvas *canvas);
void ov41_02248120(UnkOv41Canvas *canvas, int x0, int y0, int x1, int y1);
void ov41_0224825C(UnkOv41Canvas *canvas, int background, enum HeapID heapID);
void ov41_022482A8(UnkOv41Canvas *canvas);
void ov41_022499B4(UnkOv41Pool *pool, int count, enum HeapID heapID);
void ov41_022499DC(UnkOv41Pool *pool);
void ov41_02249CE0(UnkOv41BgScrollAnim *anim, const UnkOv41BgScrollAnimTemplate *tmpl);
void ov41_02249CF8(UnkOv41BgScrollAnim *anim, int step);
void ov41_02249D60(UnkOv41BgScrollAnim *anim);
void ov41_0224AD84(Window *window);
void ov41_0224B084(UnkOv41SpriteRes *res, GF_2DGfxResMan **resMans);
void ov41_0224BE34(AccessoryPortraitApp *app);
void ov41_0224BE5C(AccessoryPortraitApp *app);

void ov41_0224B21C(UnkOv41PanelSub *sub, GF_2DGfxResMan **resMans);
void ov41_0224B250(UnkOv41PanelSub *sub);
static void ov41_0224B270(UnkOv41PanelSub *sub);
void ov41_0224B298(UnkOv41PanelSub *sub);
static void ov41_0224B310(UnkOv41PanelSub *sub);
static void ov41_0224B31C(UnkOv41Tween *tween, fx32 start, fx32 end, int duration);
static BOOL ov41_0224B32C(UnkOv41Tween *tween);
static void ov41_0224B374(UnkOv41PanelSub *sub, UnkOv41DigitAnim *anim);
static void ov41_0224B450(UnkOv41DigitAnim *anim);
void ov41_0224B4E8(UnkOv41CanvasAnim *canvasAnim, UnkOv41Canvas *canvas, enum HeapID heapID);
void ov41_0224B50C(UnkOv41CanvasAnim *canvasAnim);
void ov41_0224B518(UnkOv41CanvasAnim *canvasAnim);
UnkOv41Portrait *ov41_0224B530(const UnkOv41PortraitTemplate *tmpl, SaveFashionDataSub *fashionSub);
void ov41_0224B554(UnkOv41Portrait *portrait);
void ov41_0224B57C(UnkOv41Portrait *portrait);
void ov41_0224B5C8(UnkOv41Portrait *portrait);
void ov41_0224B5D0(UnkOv41Portrait *portrait, BOOL drawActive);
static void ov41_0224B5D8(UnkOv41Portrait *portrait, int x, int y);
static UnkOv41Portrait *ov41_0224B630(UnkOv41PortraitArgs *args);
static void ov41_0224B6CC(UnkOv41Portrait *portrait, UnkOv41PortraitArgs *args);
static void ov41_0224B720(UnkOv41Portrait *portrait);
static void ov41_0224B754(UnkOv41Portrait *portrait);
static void ov41_0224B780(UnkOv41Portrait *portrait, UnkOv41PortraitArgs *args);
static void ov41_0224B848(UnkOv41Portrait *portrait, UnkOv41PortraitArgs *args);
static void ov41_0224B85C(UnkOv41Portrait *portrait);
static void ov41_0224B878(UnkOv41Portrait *portrait);
static void ov41_0224B888(UnkOv41Portrait *portrait, UnkOv41PortraitArgs *args);
static void ov41_0224B8DC(UnkOv41PortraitArgs *args, const UnkOv41PortraitTemplate *tmpl);
static void ov41_0224B8F0(UnkOv41PortraitArgs *args, SaveFashionDataSub *fashionSub);
static void ov41_0224B938(UnkOv41Gfx *gfx, UnkOv41GfxTables *tables, UnkOv41PortraitArgs *args, enum HeapID heapID);
static void ov41_0224B958(UnkOv41Gfx *gfx, UnkOv41GfxTables *tables, UnkOv41PortraitArgs *args, enum HeapID heapID);
static void ov41_0224BBF0(void *data);
static void ov41_0224BC04(AccessoryPortraitApp *app);
static void ov41_0224BCA4(AccessoryPortraitApp *app);
static void ov41_0224BCF0(AccessoryPortraitApp *app);
static void ov41_0224BD8C(AccessoryPortraitApp *app);
static void ov41_0224BDCC(AccessoryPortraitApp *app);

void ov41_0224B21C(UnkOv41PanelSub *sub, GF_2DGfxResMan **resMans) {
    if (sub->task0 != NULL) {
        SysTask_Destroy(sub->task0);
    }
    if (sub->task1 != NULL) {
        SysTask_Destroy(sub->task1);
    }
    ov41_0224B084(&sub->res, resMans);
    ov41_0224AD84(sub->window);
    memset(sub, 0, sizeof(UnkOv41PanelSub));
}

void ov41_0224B250(UnkOv41PanelSub *sub) {
    ov41_0224B310(sub);
    ov41_0224B270(sub);
    ov41_0224B298(sub);
    ov41_0224B450(&sub->anim);
}

static void ov41_0224B270(UnkOv41PanelSub *sub) {
    int value = *sub->valuePtr;

    if (sub->value != value) {
        sub->value = value;
        if (value <= 10) {
            ov41_0224B374(sub, &sub->anim);
            PlaySE(SEQ_SE_DP_HYUN2);
        }
    }
}

void ov41_0224B298(UnkOv41PanelSub *sub) {
    int digit;
    int value;
    int divisor;
    int i;

    value = sub->value;
    divisor = 1;
    divisor *= 10;
    for (i = 0; i < 2; i++) {
        digit = value / divisor;
        GF_ASSERT(digit <= 10);
        Sprite_SetAnimCtrlSeq(sub->sprites[i], digit);
        value -= digit * divisor;
        divisor /= 10;
        if (sub->highlighted == FALSE && sub->value <= 10) {
            Sprite_SetPalIndexRespectVramOffset(sub->sprites[i], 1);
            if (i == 1) {
                sub->highlighted = TRUE;
            }
        }
    }
}

static void ov41_0224B310(UnkOv41PanelSub *sub) {
    if (sub->frames - 1 >= 0) {
        sub->frames--;
    }
}

static void ov41_0224B31C(UnkOv41Tween *tween, fx32 start, fx32 end, int duration) {
    tween->cur = start;
    tween->start = start;
    tween->delta = end - start;
    tween->duration = duration;
    tween->frame = 0;
}

static BOOL ov41_0224B32C(UnkOv41Tween *tween) {
    fx32 amount = FX_Div(FX_Mul(tween->delta, tween->frame << FX32_SHIFT), tween->duration << FX32_SHIFT);
    tween->cur = amount + tween->start;
    if (tween->frame + 1 <= tween->duration) {
        tween->frame++;
        return FALSE;
    }
    tween->frame = tween->duration;
    return TRUE;
}

static void ov41_0224B374(UnkOv41PanelSub *sub, UnkOv41DigitAnim *anim) {
    int i;
    VecFx32 pos;
    fx32 scale;
    fx32 offset;
    int n;

    pos.y = 58 << FX32_SHIFT;
    pos.y += 2 << 20;
    for (i = 0; i < 2; i++) {
        anim->sprites[i] = sub->sprites[i];
        Sprite_SetAffineOverwriteMode(anim->sprites[i], 2);
        pos.x = i * 0x18 + 0x67;
        pos.x <<= FX32_SHIFT;
        anim->pos[i] = pos;
    }

    n = 10 - sub->value;
    if (n > 0) {
        scale = FX_Mul(n << FX32_SHIFT, 0x266) + FX32_ONE;
    } else {
        scale = FX32_ONE;
    }
    ov41_0224B31C(&anim->scale, scale, FX32_ONE, 16);
    offset = FX_Mul(scale, FX32_CONST(24)) - FX32_CONST(24);
    ov41_0224B31C(&anim->offsetX, offset, 0, 16);
    ov41_0224B31C(&anim->offsetY, offset, 0, 16);
    anim->active = TRUE;
}

static void ov41_0224B450(UnkOv41DigitAnim *anim) {
    BOOL done;
    VecFx32 scale;
    VecFx32 pos;

    if (anim->active) {
        done = ov41_0224B32C(&anim->scale);
        ov41_0224B32C(&anim->offsetX);
        ov41_0224B32C(&anim->offsetY);
        scale.x = anim->scale.cur;
        scale.y = anim->scale.cur;
        scale.z = anim->scale.cur;
        Sprite_SetAffineScale(anim->sprites[0], &scale);
        Sprite_SetAffineScale(anim->sprites[1], &scale);
        pos = anim->pos[0];
        pos.x -= anim->offsetX.cur;
        pos.y -= anim->offsetY.cur;
        Sprite_SetMatrix(anim->sprites[0], &pos);
        pos = anim->pos[1];
        pos.y -= anim->offsetY.cur;
        Sprite_SetMatrix(anim->sprites[1], &pos);
        if (done) {
            anim->active = FALSE;
        }
    }
}

void ov41_0224B4E8(UnkOv41CanvasAnim *canvasAnim, UnkOv41Canvas *canvas, enum HeapID heapID) {
    UnkOv41BgScrollAnimTemplate tmpl;

    canvasAnim->canvas = canvas;
    tmpl.scroll = &canvas->scroll;
    tmpl.altScrnFile = canvas->background * 4 + 0x89;
    tmpl.interval = 16;
    tmpl.heapID = heapID;
    ov41_02249CE0(&canvasAnim->anim, &tmpl);
}

void ov41_0224B50C(UnkOv41CanvasAnim *canvasAnim) {
    ov41_02249CF8(&canvasAnim->anim, 1);
}

void ov41_0224B518(UnkOv41CanvasAnim *canvasAnim) {
    ov41_02249D60(&canvasAnim->anim);
    memset(canvasAnim, 0, sizeof(UnkOv41CanvasAnim));
}

UnkOv41Portrait *ov41_0224B530(const UnkOv41PortraitTemplate *tmpl, SaveFashionDataSub *fashionSub) {
    UnkOv41PortraitArgs args;

    ov41_0224B8DC(&args, tmpl);
    ov41_0224B8F0(&args, fashionSub);
    return ov41_0224B630(&args);
}

void ov41_0224B554(UnkOv41Portrait *portrait) {
    if (portrait->bgActive == TRUE) {
        ov41_0224B50C(&portrait->canvasAnim);
    }
    if (portrait->drawActive == TRUE) {
        ov41_0224B720(portrait);
    }
}

void ov41_0224B57C(UnkOv41Portrait *portrait) {
    if (portrait->bgActive) {
        ov41_0224B878(portrait);
    }
    ov41_0224B85C(portrait);
    ov41_02245ECC(portrait->objPool);
    portrait->objPool = NULL;
    ov41_022499DC(&portrait->pool);
    ov41_0224B754(portrait);
    Heap_Free(portrait->mon);
    Heap_Free(portrait);
}

void ov41_0224B5C8(UnkOv41Portrait *portrait) {
    ov41_022465CC(&portrait->gfx);
}

void ov41_0224B5D0(UnkOv41Portrait *portrait, BOOL drawActive) {
    portrait->drawActive = drawActive;
}

static void ov41_0224B5D8(UnkOv41Portrait *portrait, int x, int y) {
    ov41_02248120(&portrait->canvas, portrait->x, portrait->y, x, y);
    if (portrait->bgActive) {
        BgSetPosTextAndCommit(portrait->gfx.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0x88 - x);
        BgSetPosTextAndCommit(portrait->gfx.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0x10 - y);
    }
    portrait->x = x;
    portrait->y = y;
}

static UnkOv41Portrait *ov41_0224B630(UnkOv41PortraitArgs *args) {
    UnkOv41Portrait *portrait = Heap_Alloc(args->heapID, sizeof(UnkOv41Portrait));
    memset(portrait, 0, sizeof(UnkOv41Portrait));
    portrait->heapID = args->heapID;
    portrait->mon = AllocMonZeroed(args->heapID);
    sub_0202BEF4(args->mon, portrait->mon);
    ov41_0224B6CC(portrait, args);
    ov41_02246544(&portrait->gfx, args->bgConfig, args->heapID);
    ov41_022499B4(&portrait->pool, 0x15, args->heapID);
    portrait->objPool = ov41_02245EA0(0x14, args->heapID);
    ov41_0224B780(portrait, args);
    ov41_0224B848(portrait, args);
    ov41_0224B4E8(&portrait->canvasAnim, &portrait->canvas, args->heapID);
    portrait->bgActive = TRUE;
    portrait->drawActive = TRUE;
    ov41_0224B888(portrait, args);
    return portrait;
}

static void ov41_0224B6CC(UnkOv41Portrait *portrait, UnkOv41PortraitArgs *args) {
    UnkOv41GfxTables tables;
    UnkOv41SystemTemplate sysTmpl;

    portrait->gfx.narc = NARC_New(NARC_a_0_2_6, args->heapID);
    sysTmpl.maxSprites = 0x2CE;
    sysTmpl.maxChars = 0x76;
    sysTmpl.maxPltts = 0x13;
    sysTmpl.heapID = args->heapID;
    ov41_02246518(&portrait->gfx, &sysTmpl, args->heapID);
    ov41_0224B938(&portrait->gfx, &tables, args, args->heapID);
    ov41_02246250(&portrait->gfx, &tables);
    ov41_022463D4(&tables);
}

static void ov41_0224B720(UnkOv41Portrait *portrait) {
    ov41_022465D8(&portrait->gfx, portrait->x + portrait->centerX, portrait->y + portrait->centerY, portrait->angle, &portrait->scale);
}

static void ov41_0224B754(UnkOv41Portrait *portrait) {
    ov41_0224626C(&portrait->gfx);
    ov41_02246594(&portrait->gfx);
    if (portrait->bgActive) {
        ov41_022465C0(&portrait->gfx);
    }
    NARC_Delete(portrait->gfx.narc);
}

static void ov41_0224B780(UnkOv41Portrait *portrait, UnkOv41PortraitArgs *args) {
    UnkOv41CanvasTemplate canvasTmpl = { 0 };
    PokepicTemplate pokepicTmpl;
    int i;

    canvasTmpl.unk00 = portrait->gfx.system;
    canvasTmpl.unk04 = portrait->gfx.charHandles;
    canvasTmpl.unk08 = portrait->gfx.plttHandles;
    canvasTmpl.unk0C = portrait->gfx.unk030;
    canvasTmpl.pokepicManager = portrait->gfx.pokepicManager;
    canvasTmpl.bgConfig = portrait->gfx.bgConfig;
    canvasTmpl.objPool = portrait->objPool;
    canvasTmpl.pool = &portrait->pool;
    canvasTmpl.maxObjs = 0x15;
    ov41_02247F3C(&portrait->canvas, &canvasTmpl);
    ov41_02247FAC(&portrait->canvas, portrait->mon, &pokepicTmpl, sub_0202BEE4(args->mon), sub_0202BEEC(args->mon), sub_0202BEDC(args->mon), args->heapID);
    for (i = 0; i < args->numMarkers; i++) {
        ov41_02248044(&portrait->canvas, sub_0202BEFC(args->markers[i]), sub_0202BF00(args->markers[i]), sub_0202BF04(args->markers[i]), sub_0202BF08(args->markers[i]));
    }
}

static void ov41_0224B848(UnkOv41Portrait *portrait, UnkOv41PortraitArgs *args) {
    ov41_0224825C(&portrait->canvas, args->background, args->heapID);
}

static void ov41_0224B85C(UnkOv41Portrait *portrait) {
    ov41_022480E0(&portrait->canvas);
    ov41_02247F90(&portrait->canvas);
}

static void ov41_0224B878(UnkOv41Portrait *portrait) {
    ov41_022482A8(&portrait->canvas);
}

static void ov41_0224B888(UnkOv41Portrait *portrait, UnkOv41PortraitArgs *args) {
    portrait->x = 0x48;
    portrait->y = 0x38;
    portrait->centerX = 0x38;
    portrait->centerY = 0x40;
    portrait->scale.x = FX32_ONE;
    portrait->scale.y = FX32_ONE;
    portrait->scale.z = FX32_ONE;
    portrait->angle = 0;
    ov41_0224B5D8(portrait, args->x, args->y);
    portrait->unk25C = 0x7FFF;
}

static void ov41_0224B8DC(UnkOv41PortraitArgs *args, const UnkOv41PortraitTemplate *tmpl) {
    args->bgConfig = tmpl->bgConfig;
    args->x = tmpl->x;
    args->y = tmpl->y;
    args->heapID = tmpl->heapID;
}

static void ov41_0224B8F0(UnkOv41PortraitArgs *args, SaveFashionDataSub *fashionSub) {
    int i;

    args->mon = sub_0202BE14(fashionSub);
    args->numMarkers = 0;
    for (i = 0; i < 10; i++) {
        if (sub_0202BDEC(fashionSub, i)) {
            args->markers[args->numMarkers] = sub_0202BE2C(fashionSub, i);
            args->numMarkers++;
        }
    }
    args->background = sub_0202BE80(fashionSub);
}

static void ov41_0224B938(UnkOv41Gfx *gfx, UnkOv41GfxTables *tables, UnkOv41PortraitArgs *args, enum HeapID heapID) {
    ov41_022464AC(tables, heapID);
    ov41_0224B958(gfx, tables, args, heapID);
}

static void ov41_0224B958(UnkOv41Gfx *gfx, UnkOv41GfxTables *tables, UnkOv41PortraitArgs *args, enum HeapID heapID) {
    int i;
    int id;
    void *file;

    for (i = 0; i < args->numMarkers; i++) {
        id = sub_0202BEFC(args->markers[i]);
        if (GF2dGfxRawResMan_DoesNotHaveObjWithId(tables->charMan, id) == TRUE) {
            file = GfGfxLoader_LoadFromOpenNarc(gfx->narc, id + 1, FALSE, heapID, TRUE);
            GF2dGfxRawResMan_AllocObj(tables->charMan, file, id);
            NNS_G2dGetUnpackedCharacterData(file, (NNSG2dCharacterData **)&tables->chars[id].charData);
            tables->chars[id].unk0 = gfx->system;
        }
    }
    file = GfGfxLoader_LoadFromOpenNarc(gfx->narc, 0, FALSE, heapID, TRUE);
    GF2dGfxRawResMan_AllocObj(tables->plttMan, file, 0);
    NNS_G2dGetUnpackedPaletteData(file, &tables->pltts[0].pltt);
    tables->pltts[0].unk0 = gfx->system;
    tables->pltts[0].unk8 = 3;
}

BOOL AccessoryPortrait_Init(OverlayManager *man, int *state) {
    AccessoryPortraitApp *app;
    FashionAppData *args;
    UnkOv41PortraitTemplate tmpl;

    Heap_Create(HEAP_ID_3, HEAP_ID_13, 0x20000);
    Heap_Create(HEAP_ID_3, HEAP_ID_14, 0x40000);
    app = OverlayManager_CreateAndGetData(man, sizeof(AccessoryPortraitApp), HEAP_ID_13);
    memset(app, 0, sizeof(AccessoryPortraitApp));
    Main_SetVBlankIntrCB(ov41_0224BBF0, app);
    HBlankInterruptDisable();
    args = OverlayManager_GetArgs(man);
    app->fashionSub = sub_0202B9B8(args->saveFashionData, args->unk_4);
    app->unk008 = args->unk_4;
    app->unk00C = args->unk_8;
    ov41_02246130();
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    ov41_02246670(&app->gfx, HEAP_ID_14);
    tmpl.bgConfig = app->gfx.bgConfig;
    tmpl.x = 0x48;
    tmpl.y = 0x10;
    tmpl.heapID = HEAP_ID_14;
    app->portrait = ov41_0224B530(&tmpl, app->fashionSub);
    ov41_0224BC04(app);
    ov41_0224BCA4(app);
    ov41_0224BCF0(app);
    ov41_0224BDCC(app);
    ov41_0224BE5C(app);
    return TRUE;
}

BOOL AccessoryPortrait_Main(OverlayManager *man, int *state) {
    AccessoryPortraitApp *app = OverlayManager_GetData(man);

    Thunk_G3X_Reset();
    NNS_G2dSetupSoftwareSpriteCamera();
    ov41_0224B554(app->portrait);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    ov41_022466C8(&app->gfx);

    switch (*state) {
    case 0:
        (*state)++;
        break;
    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_UNK_5, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_13);
        (*state)++;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            (*state)++;
        }
        break;
    case 3:
        if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || System_GetTouchNew()) {
            PlaySE(SEQ_SE_DP_DECIDE);
            (*state)++;
        }
        break;
    case 4:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_DOWNWARD_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_13);
        (*state)++;
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL AccessoryPortrait_Exit(OverlayManager *man, int *state) {
    AccessoryPortraitApp *app = OverlayManager_GetData(man);

    ov41_0224B57C(app->portrait);
    ov41_0224BD8C(app);
    ov41_0224BE34(app);
    ov41_02246698(&app->gfx);
    ov41_02246150();
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_13);
    Heap_Destroy(HEAP_ID_14);
    return TRUE;
}

static void ov41_0224BBF0(void *data) {
    AccessoryPortraitApp *app = data;

    ov41_0224B5C8(app->portrait);
    ov41_022466B8(&app->gfx);
}

static void ov41_0224BC04(AccessoryPortraitApp *app) {
    NNSG2dScreenData *scrnData;
    void *buffer;

    GfGfxLoader_GXLoadPalFromOpenNarc(app->gfx.narc, 0x7E, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x60, 0x40, HEAP_ID_14);
    GfGfxLoader_LoadCharDataFromOpenNarc(app->gfx.narc, 0x7D, app->gfx.bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_14);
    buffer = GfGfxLoader_GetScrnData(NARC_a_0_2_6, 0x80, FALSE, &scrnData, HEAP_ID_14);
    LoadRectToBgTilemapRect(app->gfx.bgConfig, GF_BG_LYR_MAIN_1, scrnData->rawData, 0, 0, scrnData->screenWidth / 8, scrnData->screenHeight / 8);
    BgTilemapRectChangePalette(app->gfx.bgConfig, GF_BG_LYR_MAIN_1, 0, 0, scrnData->screenWidth / 8, scrnData->screenHeight / 8, 4);
    Heap_Free(buffer);
    ScheduleBgTilemapBufferTransfer(app->gfx.bgConfig, GF_BG_LYR_MAIN_1);
}

static void ov41_0224BCA4(AccessoryPortraitApp *app) {
    GfGfxLoader_GXLoadPal(NARC_a_2_3_7, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_14);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_7, 9, app->gfx.bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, HEAP_ID_14);
    GfGfxLoader_LoadCharData(NARC_a_2_3_7, 1, app->gfx.bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, HEAP_ID_14);
}

static void ov41_0224BCF0(AccessoryPortraitApp *app) {
    ov41_022462E4(&app->gfx, app->gfx.narc, 0xE9, FALSE, 1, 1000);
    ov41_02246304(&app->gfx, app->gfx.narc, 0xEA, FALSE, 1, 6, 1000);
    ov41_02246328(&app->gfx, app->gfx.narc, 0xE8, FALSE, 1000);
    ov41_02246344(&app->gfx, app->gfx.narc, 0xE7, FALSE, 1000);
    app->sprite = ov41_02246280(&app->gfx, 1000, 0, 0x90, 100, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetPriority(app->sprite, 1);
}

static void ov41_0224BD8C(AccessoryPortraitApp *app) {
    ov41_02246360(&app->gfx, 1000);
    ov41_02246374(&app->gfx, 1000);
    ov41_02246388(&app->gfx, 1000);
    ov41_0224639C(&app->gfx, 1000);
    Sprite_Delete(app->sprite);
}

static void ov41_0224BDCC(AccessoryPortraitApp *app) {
    app->window = AllocWindows(HEAP_ID_14, 1);
    AddWindowParameterized(app->gfx.bgConfig, app->window, GF_BG_LYR_MAIN_3, 0, 0x12, 0x20, 6, 5, 1);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0xA0, HEAP_ID_14);
    SetBgPriority(GF_BG_LYR_MAIN_3, 0);
    SetBgPriority(GF_BG_LYR_MAIN_0, 2);
    SetBgPriority(GF_BG_LYR_MAIN_1, 1);
    BgSetPosTextAndCommit(app->gfx.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
}
