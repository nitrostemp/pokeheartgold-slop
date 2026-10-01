#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "math_util.h"
#include "overlay_41_02248400.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

typedef struct UnkOv41TouchParent {
    u8 unk00[0x38];
    int unk38; // 0x38
} UnkOv41TouchParent;

typedef struct UnkOv41TouchWork {
    UnkOv41TouchParent *parent; // 0x00
    void *unk04;                // 0x04
    UnkOv41Board *board;        // 0x08
    void *unk0C;                // 0x0C
    UnkOv41Node *node;          // 0x10
    int grabX;                  // 0x14
    int grabY;                  // 0x18
    s16 origX;                  // 0x1C
    s16 origY;                  // 0x1E
    u8 fromObj;                 // 0x20
    u8 slot;                    // 0x21
    int lastX;                  // 0x24
    int lastY;                  // 0x28
    BOOL unk2C;                 // 0x2C
    BOOL active;                // 0x30
} UnkOv41TouchWork;             // size: 0x34

typedef struct UnkOv41SlideTask {
    UnkOv41Board *board; // 0x00
    UnkOv41Node *node;   // 0x04
    int dx;              // 0x08
    int dy;              // 0x0C
    int x;               // 0x10
    int y;               // 0x14
    int type;            // 0x18
    int slot;            // 0x1C
    int frames;          // 0x20
    BOOL *busy;          // 0x24
    BOOL *active;        // 0x28
} UnkOv41SlideTask;      // size: 0x2C

typedef struct UnkOv41MonPic {
    Pokepic *pic;             // 0x0
    TouchscreenHitbox hitbox; // 0x4
    u8 rect[4];               // 0x8
    Pokemon *mon;             // 0xC
} UnkOv41MonPic;              // size: 0x10

typedef struct UnkOv41Pool {
    UnkOv41Node *nodes; // 0x0
    int count;          // 0x4
} UnkOv41Pool;

NNSG2dCharacterData *ov41_022463DC(void *a0, void *file, int a2);
void ov41_022463FC(void);
void ov41_0224642C(void);
BOOL ov41_022464BC(void *a0, int x, int y, int a3);
void ov41_02248020(void *obj, UnkOv41Node *node);
void ov41_02248030(UnkOv41Node *node);
BOOL ov41_022480A4(void *obj, UnkOv41Node *node, int a2);
void ov41_022480C8(void *obj, UnkOv41Node *node);
void ov41_02248114(void *obj, int dx, int dy);
void ov41_02248158(void *obj);
BOOL ov41_022481BC(void *obj);
int ov41_022481D8(void *obj, int x, int y);
UnkOv41Node *ov41_022481F4(void *obj, int *x, int *y, int a3);
void ov41_0224825C(void *obj, int id, int a2);
void ov41_022482A8(void *obj);
void ov41_022482B8(void *obj, int *x, int *y);
void ov41_02249A90(UnkOv41Node *node, int a1);
void ov41_02249AF4(UnkOv41Node *node, int x, int y);
void ov41_02249B44(UnkOv41Node *node, int *x, int *y);
void ov41_02249B94(UnkOv41Node *node, int *w, int *h);
void ov41_02249BAC(UnkOv41Node *node, int *m0, int *m1, int *m2, int *m3);
void ov41_02249C20(UnkOv41Node *node, int *top, int *bottom, int *left, int *right);
void ov41_0224AC08(void *a0, int a1, int a2, int a3);

u32 ov41_02248ED4(UnkOv41FashionTable *tbl, u32 id);
int ov41_02248EE8(UnkOv41FashionTable *tbl, int slot);
int ov41_02248EF4(UnkOv41FashionTable *tbl, int id);
void ov41_02248F18(UnkOv41TouchCtx *ctx, void *obj, UnkOv41Board *board, UnkOv41TouchParent *parent, void *a4, BOOL a5);
void ov41_02248F6C(UnkOv41TouchCtx *ctx);
void ov41_022495C8(void *a0, PokepicTemplate *tmpl);
void ov41_022495F0(UnkOv41MonPic *pic, PokepicManager *mgr, Pokemon *mon, PokepicTemplate *tmpl, enum HeapID heapID);
void ov41_02249604(UnkOv41MonPic *pic, PokepicManager *mgr, Pokemon *mon, PokepicTemplate *tmpl, enum HeapID heapID, BOOL a5);
void ov41_022496E8(UnkOv41MonPic *pic);
void ov41_02249700(UnkOv41MonPic *pic, int val);
int ov41_02249710(UnkOv41MonPic *pic);
void ov41_0224971C(UnkOv41MonPic *pic, int x, int y);
void ov41_02249780(UnkOv41MonPic *pic, int *x, int *y);
void ov41_022497A0(UnkOv41MonPic *pic, int *w, int *h);
BOOL ov41_022497A8(UnkOv41MonPic *pic, int *outX, int *outY, void *a3);
BOOL ov41_02249820(UnkOv41MonPic *pic, u32 x, u32 y, void *a3);
void ov41_02249888(UnkOv41MonPic *pic, u8 *out);
void ov41_022499B4(UnkOv41Pool *pool, int count, enum HeapID heapID);
void ov41_022499DC(UnkOv41Pool *pool);
UnkOv41Node *ov41_022499F0(UnkOv41Pool *pool, void *obj, int type);

static void ov41_02248F80(UnkOv41TouchCtx *ctx);
static void ov41_022490AC(UnkOv41TouchCtx *ctx);
static void ov41_022490B0(UnkOv41TouchCtx *ctx);
static void ov41_022490F0(UnkOv41TouchCtx *ctx);
static void ov41_02249280(UnkOv41TouchCtx *ctx);
static void ov41_022492B0(UnkOv41TouchCtx *ctx);
static void ov41_022492E0(UnkOv41TouchCtx *ctx);
static void ov41_02249390(UnkOv41TouchCtx *ctx);
static void ov41_022493BC(UnkOv41TouchWork *work, UnkOv41Node *node, int fromObj, int x, int y, int a5);
static void ov41_02249418(UnkOv41TouchWork *work);
static void ov41_0224942C(UnkOv41TouchCtx *ctx, int *top, int *bottom, int *left, int *right);
static void ov41_0224946C(UnkOv41TouchCtx *ctx, int *top, int *bottom, int *left, int *right);
static void ov41_02249480(UnkOv41TouchWork *work, int frames, int x, int y, int type, int slot);
static void ov41_022494F4(SysTask *task, void *data);
static void ov41_02249574(UnkOv41TouchWork *work);
static void ov41_022495A4(UnkOv41TouchCtx *ctx, int x, int y);
static BOOL ov41_02249768(UnkOv41MonPic *pic);
static BOOL ov41_02249774(UnkOv41MonPic *pic, u32 x, u32 y);
static int ov41_0224989C(const s8 *raw, int stride);
static void ov41_022498E8(const s8 *raw, int stride, int height, u8 *out);
static void ov41_02249978(TouchscreenHitbox *hitbox, int x, int y, int halfW, int halfH);

u32 ov41_02248ED4(UnkOv41FashionTable *tbl, u32 id) {
    GF_ASSERT(id < 100);
    return tbl->counts[id];
}

int ov41_02248EE8(UnkOv41FashionTable *tbl, int slot) {
    return tbl->slotToId[slot];
}

int ov41_02248EF4(UnkOv41FashionTable *tbl, int id) {
    int i;

    for (i = 0; i < 18; i++) {
        if (id == tbl->slotToId[i]) {
            return i;
        }
    }
    return i;
}

void ov41_02248F18(UnkOv41TouchCtx *ctx, void *obj, UnkOv41Board *board, UnkOv41TouchParent *parent, void *a4, BOOL a5) {
    UnkOv41TouchWork *work;

    ov41_02248E28(ctx);
    ctx->unk00 = Heap_Alloc(HEAP_ID_13, sizeof(UnkOv41TouchWork));
    memset(ctx->unk00, 0, sizeof(UnkOv41TouchWork));
    work = ctx->unk00;
    work->unk04 = obj;
    work->board = board;
    work->parent = parent;
    work->unk0C = a4;
    work->unk2C = a5;
    ctx->onNew = ov41_02248F80;
    ctx->onRelease = ov41_022490F0;
    ctx->onHeld = ov41_02249280;
    ctx->unk10 = ov41_02248F6C;
}

void ov41_02248F6C(UnkOv41TouchCtx *ctx) {
    Heap_Free(ctx->unk00);
    ov41_02248E28(ctx);
}

static void ov41_02248F80(UnkOv41TouchCtx *ctx) {
    UnkOv41TouchWork *work = ctx->unk00;
    UnkOv41Node *node;
    int x, y;
    int z;
    int se;

    if (ov41_022481BC(work->unk04)) {
        node = ov41_022481F4(work->unk04, &x, &y, work->parent->unk38);
        if (node != NULL && node->type == 3 && work->unk2C == FALSE) {
            node = NULL;
        }
        if (node != NULL) {
            if (node->type == 0) {
                ov41_022480C8(work->unk04, node);
                z = 0;
            } else {
                ov41_02248030(node);
                z = ov41_02249710(node->obj);
                ov41_02249B44(node, &work->lastX, &work->lastY);
            }
            ov41_02248158(work->unk04);
            ov41_022493BC(work, node, 1, x, y, z);
            ov41_0224642C();
        }
    } else if (ov41_02248820(work->board)) {
        node = ov41_02248858(work->board, (int)&x, (int)&y, work->parent->unk38);
        if (node != NULL) {
            ov41_022486F0(node);
            ov41_02248724(work->board);
            ov41_022493BC(work, node, 0, x, y, 0);
            ov41_0224642C();
        }
    }

    if (work->node != NULL) {
        se = 0x5EB;
        switch (work->node->type) {
        case 0:
            ctx->onRelease = ov41_022490F0;
            ctx->onHeld = ov41_02249280;
            break;
        case 1:
            ctx->onRelease = ov41_022490B0;
            ctx->onHeld = ov41_022490AC;
            ov41_02249390(ctx);
            se = 0x67D;
            break;
        case 3:
            ctx->onRelease = ov41_022492B0;
            ctx->onHeld = ov41_022492E0;
            break;
        }
        PlaySE(se);
        ov41_02249574(work);
    }
}

static void ov41_022490AC(UnkOv41TouchCtx *ctx) {
}

static void ov41_022490B0(UnkOv41TouchCtx *ctx) {
    UnkOv41TouchWork *work = ctx->unk00;

    if (work->node != NULL) {
        GF_ASSERT(work->node->type == 1);
        ov41_02249480(work, 4, work->origX, work->origY, work->node->type, work->slot);
        ov41_02249418(work);
    }
}

static void ov41_022490F0(UnkOv41TouchCtx *ctx) {
    UnkOv41TouchWork *work = ctx->unk00;
    int top, bottom, left, right;
    int w, h;
    int destX, destY;
    int n;
    int *item;

    if (work->node == NULL) {
        return;
    }

    ov41_0224946C(ctx, &top, &bottom, &left, &right);
    n = ov41_022481D8(work->unk04, left, top);
    n += ov41_022481D8(work->unk04, right, top);
    n += ov41_022481D8(work->unk04, left, bottom);
    n += ov41_022481D8(work->unk04, right, bottom);
    if (n >= 4) {
        if (!ov41_022480A4(work->unk04, work->node, work->parent->unk38)) {
            destX = work->origX;
            destY = work->origY;
            PlaySE(0x682);
            ov41_0224AC08(work->unk0C, 0x1B, 0xD7, 3);
            ov41_02249480(work, 4, destX, destY, work->node->type, work->slot);
        } else {
            ov41_02248158(work->unk04);
            ov41_022463FC();
            work->active = FALSE;
            PlaySE(0x5EA);
        }
    } else {
        item = work->node->obj;
        ov41_0224942C(ctx, &top, &bottom, &left, &right);
        n = ov41_0224883C(work->board, left, top);
        n += ov41_0224883C(work->board, right, bottom);
        if (n < 2) {
            if (work->fromObj == 1) {
                ov41_02249B94(work->node, &w, &h);
                destX = MTRandom() % (0x6C - w) + 10;
                destY = MTRandom() % (0x7D - h) + 0x12;
            } else {
                destX = work->origX;
                destY = work->origY;
            }
            PlaySE(0x682);
        } else {
            ov41_02249B44(work->node, &destX, &destY);
            PlaySE(0x5EB);
        }
        if (work->fromObj == 1) {
            work->slot = ov41_022484E8(work->node->type, *item, work->board->fashionTable);
        }
        ov41_02249480(work, 4, destX, destY, work->node->type, work->slot);
    }
    ov41_02249418(work);
}

static void ov41_02249280(UnkOv41TouchCtx *ctx) {
    UnkOv41TouchWork *work = ctx->unk00;

    if (work->node != NULL && gSystem.touchX != 0xFFFF && gSystem.touchX != 0xFFFF) {
        ov41_02249AF4(work->node, gSystem.touchX - work->grabX, gSystem.touchY - work->grabY);
    }
}

static void ov41_022492B0(UnkOv41TouchCtx *ctx) {
    UnkOv41TouchWork *work = ctx->unk00;

    if (work->node != NULL) {
        ov41_02248020(work->unk04, work->node);
        ov41_02248158(work->unk04);
        ov41_022463FC();
        work->active = FALSE;
        PlaySE(0x5EB);
        ov41_02249418(work);
    }
}

static void ov41_022492E0(UnkOv41TouchCtx *ctx) {
    UnkOv41TouchWork *work = ctx->unk00;
    int w, h;
    int ml, mt, mr, mb;
    int x, y;
    int yt;

    if (work->node != NULL && gSystem.touchX != 0xFFFF && gSystem.touchX != 0xFFFF) {
        ov41_02249BAC(work->node, &ml, &mr, &mt, &mb);
        ov41_02249B94(work->node, &w, &h);
        x = gSystem.touchX - work->grabX;
        y = gSystem.touchY - work->grabY;
        yt = y + mt;
        w -= mr;
        h -= mb;
        if (x + ml <= 0x8A) {
            x = 0x8A - ml;
        } else if (x + w >= 0xF6) {
            x = 0xF6 - w;
        }
        if (yt <= 0x12) {
            y = 0x12 - mt;
        } else if (y + h >= 0x8F) {
            y = 0x8F - h;
        }
        ov41_022495A4(ctx, x, y);
        ov41_022482B8(work->unk04, &ml, &mt);
        ov41_022495A4(ctx, x + ml, y + mt);
    }
}

static void ov41_02249390(UnkOv41TouchCtx *ctx) {
    UnkOv41TouchWork *work = ctx->unk00;
    int *item;

    if (work->node != NULL) {
        GF_ASSERT(work->node->type == 1);
        item = work->node->obj;
        ov41_022482A8(work->unk04);
        ov41_0224825C(work->unk04, *item, 14);
    }
}

static void ov41_022493BC(UnkOv41TouchWork *work, UnkOv41Node *node, int fromObj, int x, int y, int a5) {
    int nx, ny;

    ov41_02249B44(node, &nx, &ny);
    work->node = node;
    work->origX = nx;
    work->origY = ny;
    work->fromObj = fromObj;
    work->grabX = x;
    work->grabY = y;
    if (fromObj == 0) {
        work->slot = ov41_0224895C(work->board, work->node->type);
    } else {
        work->slot = 0;
    }
    work->active = TRUE;
    ov41_02249A90(node, a5);
}

static void ov41_02249418(UnkOv41TouchWork *work) {
    work->node = NULL;
    work->origX = 0;
    work->origY = 0;
    work->fromObj = 0;
    work->slot = 0;
}

static void ov41_0224942C(UnkOv41TouchCtx *ctx, int *top, int *bottom, int *left, int *right) {
    UnkOv41TouchWork *work = ctx->unk00;
    int w, h;
    int x, y;

    ov41_02249B94(work->node, &w, &h);
    ov41_02249B44(work->node, &x, &y);
    *top = y;
    *bottom = y + h;
    *left = x;
    *right = x + w;
}

static void ov41_0224946C(UnkOv41TouchCtx *ctx, int *top, int *bottom, int *left, int *right) {
    UnkOv41TouchWork *work = ctx->unk00;

    ov41_02249C20(work->node, top, bottom, left, right);
}

static void ov41_02249480(UnkOv41TouchWork *work, int frames, int x, int y, int type, int slot) {
    int curX, curY;
    UnkOv41SlideTask *task = SysTask_GetData(CreateSysTaskAndEnvironment(ov41_022494F4, sizeof(UnkOv41SlideTask), 0, HEAP_ID_13));

    task->board = work->board;
    task->node = work->node;
    task->frames = frames;
    task->x = x;
    task->y = y;
    task->type = type;
    task->slot = slot;
    task->busy = &work->board->busy;
    task->active = &work->active;
    ov41_02249B44(work->node, &curX, &curY);
    task->dx = (task->x - curX) / frames;
    task->dy = (task->y - curY) / frames;
    work->active = FALSE;
}

static void ov41_022494F4(SysTask *task, void *data) {
    UnkOv41SlideTask *slide = data;
    int x, y;

    if (*slide->busy == TRUE) {
        return;
    }
    ov41_02249B44(slide->node, &x, &y);
    x += slide->dx;
    y += slide->dy;
    slide->frames--;
    if (slide->frames < 0 || (slide->dx == 0 && slide->dy == 0)) {
        ov41_02249AF4(slide->node, slide->x, slide->y);
        ov41_022486C4(slide->board, slide->type, slide->slot, slide->node);
        ov41_02248724(slide->board);
        if (*slide->active == FALSE) {
            ov41_022463FC();
        }
        DestroySysTaskAndEnvironment(task);
        return;
    }
    ov41_02249AF4(slide->node, x, y);
}

static void ov41_02249574(UnkOv41TouchWork *work) {
    if (work->node->type == 0) {
        ov41_0224AC08(work->unk0C, 0x1B, 0xD8, *(int *)work->node->obj);
    } else if (work->node->type == 1) {
        ov41_0224AC08(work->unk0C, 0x1B, 0xDA, *(int *)work->node->obj);
    }
}

static void ov41_022495A4(UnkOv41TouchCtx *ctx, int x, int y) {
    UnkOv41TouchWork *work = ctx->unk00;

    ov41_02249AF4(work->node, x, y);
    ov41_02248114(work->unk04, x - work->lastX, y - work->lastY);
    work->lastX = x;
    work->lastY = y;
}

void ov41_022495C8(void *a0, PokepicTemplate *tmpl) {
    NNSG2dCharacterData *charData = ov41_022463DC(a0, GfGfxLoader_LoadFromNarc((NarcId)tmpl->narcID, tmpl->charDataID, FALSE, HEAP_ID_14, FALSE), 0x76);
    UnscanPokepic(charData->pRawData, (NarcId)tmpl->narcID);
}

void ov41_022495F0(UnkOv41MonPic *pic, PokepicManager *mgr, Pokemon *mon, PokepicTemplate *tmpl, enum HeapID heapID) {
    ov41_02249604(pic, mgr, mon, tmpl, heapID, FALSE);
}

void ov41_02249604(UnkOv41MonPic *pic, PokepicManager *mgr, Pokemon *mon, PokepicTemplate *tmpl, enum HeapID heapID, BOOL a5) {
    int w, h;
    NNSG2dCharacterData *charData;
    void *file;
    u8 yOffset;

    GetMonData(mon, MON_DATA_SPECIES, NULL);
    sub_02070130(tmpl, (BoxPokemon *)mon, 2);
    pic->pic = PokepicManager_CreatePokepic(mgr, tmpl, 0xC0, 0x38, 0, 0, NULL, NULL);
    ov41_022497A0(pic, &w, &h);
    w /= 2;
    h /= 2;
    pic->hitbox.rect.top = 0x38 - h;
    pic->hitbox.rect.bottom = h + 0x38;
    pic->hitbox.rect.left = 0xC0 - w;
    pic->hitbox.rect.right = w + 0xC0;
    ov41_02249978(&pic->hitbox, 0xC0, 0x38, w, h);
    yOffset = sub_02070848((BoxPokemon *)mon, 2);
    file = GfGfxLoader_LoadFromNarc((NarcId)tmpl->narcID, tmpl->charDataID, FALSE, heapID, FALSE);
    NNS_G2dGetUnpackedCharacterData(file, &charData);
    UnscanPokepic(charData->pRawData, (NarcId)tmpl->narcID);
    if (a5 == FALSE) {
        ov41_022498E8(charData->pRawData, charData->W * 8, charData->H * 8, pic->rect);
    } else {
        pic->rect[0] = ov41_0224989C(charData->pRawData, charData->W * 8);
        pic->rect[1] = pic->rect[0];
        pic->rect[3] = yOffset;
        pic->rect[2] = yOffset;
    }
    Heap_Free(file);
    pic->mon = mon;
}

void ov41_022496E8(UnkOv41MonPic *pic) {
    u32 i;
    u8 *p;

    Pokepic_Delete(pic->pic);
    p = (u8 *)pic;
    i = sizeof(UnkOv41MonPic);
    do {
        *p = 0;
        p++;
        i--;
    } while (i != 0);
}

void ov41_02249700(UnkOv41MonPic *pic, int val) {
    Pokepic_SetAttr(pic->pic, 2, val);
}

int ov41_02249710(UnkOv41MonPic *pic) {
    return Pokepic_GetAttr(pic->pic, 2);
}

void ov41_0224971C(UnkOv41MonPic *pic, int x, int y) {
    int w, h;

    ov41_022497A0(pic, &w, &h);
    Pokepic_SetAttr(pic->pic, 0, x);
    Pokepic_SetAttr(pic->pic, 1, y);
    h /= 2;
    w /= 2;
    ov41_02249978(&pic->hitbox, x, y, w, h);
}

static BOOL ov41_02249768(UnkOv41MonPic *pic) {
    return TouchscreenHitbox_TouchHeldIsIn(&pic->hitbox);
}

static BOOL ov41_02249774(UnkOv41MonPic *pic, u32 x, u32 y) {
    return TouchscreenHitbox_PointIsIn(&pic->hitbox, x, y);
}

void ov41_02249780(UnkOv41MonPic *pic, int *x, int *y) {
    *x = Pokepic_GetAttr(pic->pic, 0);
    *y = Pokepic_GetAttr(pic->pic, 1);
}

void ov41_022497A0(UnkOv41MonPic *pic, int *w, int *h) {
    *w = 0x50;
    *h = 0x50;
}

BOOL ov41_022497A8(UnkOv41MonPic *pic, int *outX, int *outY, void *a3) {
    int w, h;
    int x, y;

    if (!ov41_02249768(pic)) {
        return FALSE;
    }
    ov41_02249780(pic, &x, &y);
    ov41_022497A0(pic, &w, &h);
    x -= w / 2;
    y -= h / 2;
    *outX = gSystem.touchX - x;
    *outY = gSystem.touchY - y;
    if (ov41_022464BC(a3, *outX, *outY, 0) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

BOOL ov41_02249820(UnkOv41MonPic *pic, u32 px, u32 py, void *a3) {
    int w, h;
    int x, y;

    if (!ov41_02249774(pic, px, py)) {
        return FALSE;
    }
    ov41_02249780(pic, &x, &y);
    ov41_022497A0(pic, &w, &h);
    x -= w / 2;
    y -= h / 2;
    px -= x;
    py -= y;
    if (ov41_022464BC(a3, px, py, 0) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

void ov41_02249888(UnkOv41MonPic *pic, u8 *out) {
    out[0] = pic->rect[0];
    out[1] = pic->rect[1];
    out[2] = pic->rect[2];
    out[3] = pic->rect[3];
}

static int ov41_0224989C(const s8 *raw, int stride) {
    int i, j;

    for (i = 0; i < 80; i++) {
        for (j = 0; j < 80; j++) {
            int idx = i + j * stride;
            if ((u8)(0xF << ((idx % 2) * 4)) & raw[idx / 2]) {
                return i;
            }
        }
    }
    return 80;
}

static void ov41_022498E8(const s8 *raw, int stride, int height, u8 *out) {
    int i, j;

    out[0] = 40;
    out[1] = 40;
    out[2] = 40;
    out[3] = 40;
    for (i = 0; i < 80; i++) {
        for (j = 0; j < 80; j++) {
            int idx = i + j * stride;
            if ((u8)(0xF << ((idx % 2) * 4)) & raw[idx / 2]) {
                if (out[0] > i) {
                    out[0] = i;
                }
                if (out[1] > 80 - i) {
                    out[1] = 80 - i;
                }
                if (out[2] > j) {
                    out[2] = j;
                }
                if (out[3] > 80 - j) {
                    out[3] = 80 - j;
                }
            }
        }
    }
}

static void ov41_02249978(TouchscreenHitbox *hitbox, int x, int y, int halfW, int halfH) {
    if (y - halfH >= 0) {
        hitbox->rect.top = y - halfH;
    } else {
        hitbox->rect.top = 0;
    }
    if (y + halfH <= 0xBF) {
        hitbox->rect.bottom = y + halfH;
    } else {
        hitbox->rect.bottom = 0xBF;
    }
    if (x - halfW >= 0) {
        hitbox->rect.left = x - halfW;
    } else {
        hitbox->rect.left = 0;
    }
    if (x + halfW <= 0xFF) {
        hitbox->rect.right = x + halfW;
    } else {
        hitbox->rect.right = 0xFF;
    }
}

void ov41_022499B4(UnkOv41Pool *pool, int count, enum HeapID heapID) {
    pool->nodes = Heap_Alloc(heapID, count * sizeof(UnkOv41Node));
    GF_ASSERT(pool->nodes != NULL);
    memset(pool->nodes, 0, count * sizeof(UnkOv41Node));
    pool->count = count;
}

void ov41_022499DC(UnkOv41Pool *pool) {
    Heap_Free(pool->nodes);
    pool->nodes = NULL;
    pool->count = 0;
}

UnkOv41Node *ov41_022499F0(UnkOv41Pool *pool, void *obj, int type) {
    int i;

    GF_ASSERT(pool->nodes != NULL);
    GF_ASSERT(pool->count != 0);
    for (i = 0; i < pool->count; i++) {
        if (pool->nodes[i].obj == NULL) {
            break;
        }
    }
    GF_ASSERT(pool->count > i);
    pool->nodes[i].obj = obj;
    pool->nodes[i].type = type;
    return &pool->nodes[i];
}
