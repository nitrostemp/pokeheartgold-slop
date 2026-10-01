#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "overlay_41_02248400.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "system.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02013534.h"

typedef struct UnkOv41MonPic {
    Pokepic *pic;             // 0x0
    TouchscreenHitbox hitbox; // 0x4
    u8 rect[4];               // 0x8
    Pokemon *mon;             // 0xC
} UnkOv41MonPic;              // size: 0x10

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

typedef struct UnkOv41BgScrollTask {
    UnkOv41BgScroll *scroll;      // 0x00
    UnkOv41BgScrollTemplate tmpl; // 0x04
    int *doneFlag;                // 0x34
    int frames;                   // 0x38
    int stepX;                    // 0x3C
    int stepY;                    // 0x40
    int origX;                    // 0x44
    int origY;                    // 0x48
} UnkOv41BgScrollTask;            // size: 0x4C

typedef struct UnkOv41CharTransfer {
    BgConfig *bgConfig;            // 0x00
    NNSG2dCharacterData *charData; // 0x04
    void *buffer;                  // 0x08
    int bgId;                      // 0x0C
    int offset;                    // 0x10
} UnkOv41CharTransfer;             // size: 0x14

typedef struct UnkOv41PlttTransfer {
    NNSG2dPaletteData *plttData; // 0x00
    void *buffer;                // 0x04
    int location;                // 0x08
    int offset;                  // 0x0C
    int size;                    // 0x10
} UnkOv41PlttTransfer;           // size: 0x14

typedef struct UnkOv41Button UnkOv41Button;
typedef void (*UnkOv41ButtonCallback)(UnkOv41Button *button, void *arg);

struct UnkOv41Button {
    Sprite *sprite;                 // 0x00
    int id;                         // 0x04
    void *arg;                      // 0x08
    UnkOv41ButtonCallback callback; // 0x0C
}; // size: 0x10

typedef struct UnkOv41TextButton {
    UnkOv41Button button;        // 0x00
    TextOBJ *textObj;            // 0x10
    UnkStruct_02021AC8 charVram; // 0x14
} UnkOv41TextButton;             // size: 0x20

typedef struct UnkOv41ButtonTemplate {
    SpriteTemplate *spriteTemplate;     // 0x00
    UnkOv41ButtonCallback callback;     // 0x04
    void *arg;                          // 0x08
    int id;                             // 0x0C
    Window *window;                     // 0x10
    UnkStruct_02013534 *fontSystem;     // 0x14
    NNSG2dImagePaletteProxy *plttProxy; // 0x18
    int x;                              // 0x1C
    int y;                              // 0x20
    u32 offset;                         // 0x24
} UnkOv41ButtonTemplate;

typedef struct UnkOv41ButtonBar {
    UnkOv41Button buttons[4];                // 0x00
    UnkOv41TextButton textButton;            // 0x40
    int unk60;                               // 0x60
    UnkStruct_02013534 *fontSystem;          // 0x64
    TouchHitboxController *hitboxController; // 0x68
    TouchscreenHitbox hitboxes[5];           // 0x6C
    int *inputMode;                          // 0x80
    u16 timer;                               // 0x84
    u16 pressedButton;                       // 0x86
} UnkOv41ButtonBar;                          // size: 0x88

BOOL ov41_02245F14(void *obj, int a1, int a2, void *a3);
void ov41_02245F9C(void *obj, s16 x, s16 y);
void ov41_02245FA8(void *obj, int *x, int *y);
void ov41_02245FD8(void *obj, int *w, int *h);
void ov41_02246014(void *obj, int priority);
void ov41_02246020(void *obj, int *a1, int *a2);
void ov41_02246388(void *gfx, int index);
void ov41_0224639C(void *gfx, int index);
void ov41_02249700(UnkOv41MonPic *pic, int val);
void ov41_0224971C(UnkOv41MonPic *pic, int x, int y);
void ov41_02249780(UnkOv41MonPic *pic, int *x, int *y);
void ov41_022497A0(UnkOv41MonPic *pic, int *w, int *h);
BOOL ov41_022497A8(UnkOv41MonPic *pic, int x, int y, void *a3);
void ov41_02249888(UnkOv41MonPic *pic, u8 *out);
void ov41_0224A60C(int button, int state, void *arg);
void ov41_0224A6C4(UnkOv41Button *button, int id, void *gfx, int x, int y, int w, int h);
void ov41_0224A734(UnkOv41TextButton *button, int id, void *gfx, UnkStruct_02013534 *fontSystem, String *string, int x, int y, int w, int h);
void ov41_0224A7E0(TouchscreenHitbox *hitboxes, int index, int x, int y, int w, int h);
void ov41_0224A7F8(void *gfx);
void ov41_0224A888(void *gfx);
void ov41_0224A8B0(UnkOv41TextButton *button, int state);
void ov41_0224A8D4(UnkOv41TextButton *button, int state);
String *ov41_0224A928(void *gfx, NarcId narcId, int fileId, int msgId, int a4, int a5);
void ov41_0224A9B0(String *string);
extern s32 _s32_div_f(s32 num, s32 den);

static void ov41_02249A40(UnkOv41Node *node);
void ov41_02249A50(UnkOv41Node *node, UnkOv41Node *where);
void ov41_02249A60(UnkOv41Node *node);
void ov41_02249A70(UnkOv41Node *head);
void ov41_02249A90(UnkOv41Node *node, int priority);
BOOL ov41_02249AA8(UnkOv41Node *node, int a1, int a2, void **a3);
void ov41_02249AF4(UnkOv41Node *node, int x, int y);
void ov41_02249B44(UnkOv41Node *node, int *x, int *y);
void ov41_02249B94(UnkOv41Node *node, int *w, int *h);
void ov41_02249BAC(UnkOv41Node *node, int *marginLeft, int *marginRight, int *marginTop, int *marginBottom);
void ov41_02249BE8(UnkOv41Node *head, int dx, int dy);
void ov41_02249C20(UnkOv41Node *node, int *top, int *bottom, int *left, int *right);
void ov41_02249C7C(UnkOv41BgScroll *scroll, UnkOv41BgScrollTemplate *tmpl);
void ov41_02249CC4(UnkOv41BgScroll *scroll);
void ov41_02249CE0(UnkOv41BgScrollAnim *anim, const UnkOv41BgScrollAnimTemplate *tmpl);
void ov41_02249CF8(UnkOv41BgScrollAnim *anim, int step);
void ov41_02249D60(UnkOv41BgScrollAnim *anim);
void ov41_02249DB4(UnkOv41BgScroll *scroll, UnkOv41BgScrollTemplate *tmpl, int dx, int dy, int frames, int *doneFlag);
static void ov41_02249E40(NNSG2dScreenData *scrnData, int tileOffset);
static void ov41_02249E60(UnkOv41BgScrollTemplate *tmpl, int *outWidth, int *outHeight);
static void ov41_02249F0C(SysTask *task, void *data);
static void ov41_02249F7C(BgConfig *bgConfig, int bgId, NNSG2dScreenData *scrnData, int width, int height, int x, int y, int tileOffset, int plttSlot);
static void ov41_02249FFC(NarcId narcId, int fileId, BgConfig *bgConfig, int bgId, int offset, enum HeapID heapID);
static void ov41_0224A04C(NarcId narcId, int fileId, int location, int offset, int size, enum HeapID heapID);
static void ov41_0224A094(SysTask *task, void *data);
static void ov41_0224A0D0(SysTask *task, void *data);
void ov41_0224A118(UnkOv41Button *button, const UnkOv41ButtonTemplate *tmpl);
void ov41_0224A15C(UnkOv41TextButton *button, const UnkOv41ButtonTemplate *tmpl);
static void ov41_0224A1A8(UnkOv41Button *button);
static void ov41_0224A1C0(UnkOv41TextButton *button);
void ov41_0224A1DC(UnkOv41Button *button, int id);
void ov41_0224A1EC(UnkOv41ButtonBar *bar, int selected, int state);
void ov41_0224A238(UnkOv41Button *button, UnkOv41ButtonCallback callback, void *arg, int id);
static void ov41_0224A254(UnkOv41Button *button, void *arg);
void ov41_0224A258(UnkOv41Button *button);
void ov41_0224A264(UnkOv41Button *button);
void ov41_0224A270(UnkOv41Button *button);
void ov41_0224A27C(UnkOv41ButtonBar *bar, void *gfx, int *inputMode);
void ov41_0224A3E4(UnkOv41ButtonBar *bar, void *gfx);
static void ov41_0224A448(UnkOv41ButtonBar *bar);
static BOOL ov41_0224A4EC(UnkOv41ButtonBar *bar);
void ov41_0224A54C(UnkOv41ButtonBar *bar);
void ov41_0224A580(UnkOv41ButtonBar *bar);

static const u8 ov41_0224C094[] = { 2, 1, 0, 0 };

static void ov41_02249A40(UnkOv41Node *node) {
    memset(node, 0, sizeof(UnkOv41Node));
}

void ov41_02249A50(UnkOv41Node *node, UnkOv41Node *where) {
    node->next = where->next;
    where->next->prev = node;
    node->prev = where;
    where->next = node;
}

void ov41_02249A60(UnkOv41Node *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void ov41_02249A70(UnkOv41Node *head) {
    UnkOv41Node *node = head->next;
    UnkOv41Node *next;

    while (node != head) {
        next = node->next;
        ov41_02249A60(node);
        ov41_02249A40(node);
        node = next;
    }
}

void ov41_02249A90(UnkOv41Node *node, int priority) {
    if ((int)node->type < 3) {
        ov41_02246014(node->obj, priority);
    } else {
        ov41_02249700(node->obj, priority);
    }
}

BOOL ov41_02249AA8(UnkOv41Node *node, int a1, int a2, void **a3) {
    if (node->type == 0) {
        return ov41_02245F14(node->obj, a1, a2, a3[*(int *)node->obj]);
    } else if (node->type == 1) {
        return ov41_02245F14(node->obj, a1, a2, a3[*(int *)node->obj + 100]);
    } else if (node->type == 2) {
        return ov41_02245F14(node->obj, a1, a2, a3[*(int *)node->obj + 100]);
    } else {
        return ov41_022497A8(node->obj, a1, a2, a3[0x76]);
    }
}

void ov41_02249AF4(UnkOv41Node *node, int x, int y) {
    UnkOv41MonPic *pic;
    int w;
    int h;

    if ((int)node->type < 3) {
        ov41_02245F9C(node->obj, x, y);
    } else {
        pic = node->obj;
        ov41_022497A0(pic, &w, &h);
        w /= 2;
        h /= 2;
        ov41_0224971C(pic, x + w, y + h);
    }
}

void ov41_02249B44(UnkOv41Node *node, int *x, int *y) {
    UnkOv41MonPic *pic;
    int w;
    int h;

    if ((int)node->type < 3) {
        ov41_02245FA8(node->obj, x, y);
    } else {
        pic = node->obj;
        ov41_02249780(pic, x, y);
        ov41_022497A0(pic, &w, &h);
        w /= 2;
        h /= 2;
        *x -= w;
        *y -= h;
    }
}

void ov41_02249B94(UnkOv41Node *node, int *w, int *h) {
    if ((int)node->type < 3) {
        ov41_02245FD8(node->obj, w, h);
    } else {
        ov41_022497A0(node->obj, w, h);
    }
}

void ov41_02249BAC(UnkOv41Node *node, int *marginLeft, int *marginRight, int *marginTop, int *marginBottom) {
    u8 rect[4];

    if ((int)node->type < 3) {
        ov41_02246020(node->obj, marginLeft, marginTop);
        *marginRight = *marginLeft;
        *marginBottom = *marginTop;
    } else {
        ov41_02249888(node->obj, rect);
        *marginLeft = rect[0];
        *marginRight = rect[1];
        *marginTop = rect[2];
        *marginBottom = rect[3];
    }
}

void ov41_02249BE8(UnkOv41Node *head, int dx, int dy) {
    UnkOv41Node *node;
    int x;
    int y;

    for (node = head->next; node != head; node = node->next) {
        ov41_02249B44(node, &x, &y);
        x += dx;
        y += dy;
        ov41_02249AF4(node, x, y);
    }
}

void ov41_02249C20(UnkOv41Node *node, int *top, int *bottom, int *left, int *right) {
    int w;
    int h;
    int x;
    int y;
    int marginLeft;
    int marginTop;
    int marginRight;
    int marginBottom;

    ov41_02249B94(node, &w, &h);
    ov41_02249B44(node, &x, &y);
    ov41_02249BAC(node, &marginLeft, &marginRight, &marginTop, &marginBottom);
    *top = y + marginTop;
    *bottom = y + h - marginBottom;
    *left = x + marginLeft;
    *right = x + w - marginRight;
}

void ov41_02249C7C(UnkOv41BgScroll *scroll, UnkOv41BgScrollTemplate *tmpl) {
    scroll->bgConfig = tmpl->bgConfig;
    scroll->x = tmpl->x / 8;
    scroll->y = tmpl->y / 8;
    scroll->bgId = tmpl->bgId;
    scroll->plttSlot = tmpl->plttSlot;
    scroll->plttCount = tmpl->plttCount;
    scroll->tileOffset = tmpl->tileOffset;
    scroll->narcId = tmpl->narcId;
    scroll->scrnFile = tmpl->scrnFile;
    ov41_02249E60(tmpl, &scroll->width, &scroll->height);
}

void ov41_02249CC4(UnkOv41BgScroll *scroll) {
    BgClearTilemapBufferAndCommit(scroll->bgConfig, scroll->bgId);
    memset(scroll, 0, sizeof(UnkOv41BgScroll));
}

void ov41_02249CE0(UnkOv41BgScrollAnim *anim, const UnkOv41BgScrollAnimTemplate *tmpl) {
    anim->scroll = tmpl->scroll;
    anim->altScrnFile = tmpl->altScrnFile;
    anim->interval = tmpl->interval;
    anim->counter = 0;
    anim->showAlt = FALSE;
    anim->heapID = tmpl->heapID;
}

void ov41_02249CF8(UnkOv41BgScrollAnim *anim, int step) {
    int fileId;
    NNSG2dScreenData *scrnData;
    void *buffer;

    anim->counter += step;
    if (anim->counter > anim->interval) {
        anim->counter = 0;
        if (anim->showAlt == FALSE) {
            fileId = anim->altScrnFile;
            anim->showAlt = TRUE;
        } else {
            fileId = anim->scroll->scrnFile;
            anim->showAlt = FALSE;
        }
        buffer = GfGfxLoader_GetScrnData(anim->scroll->narcId, fileId, FALSE, &scrnData, anim->heapID);
        ov41_02249F7C(anim->scroll->bgConfig, anim->scroll->bgId, scrnData, anim->scroll->width, anim->scroll->height, anim->scroll->x, anim->scroll->y, anim->scroll->tileOffset, anim->scroll->plttSlot);
        Heap_Free(buffer);
    }
}

void ov41_02249D60(UnkOv41BgScrollAnim *anim) {
    NNSG2dScreenData *scrnData;
    void *buffer;

    buffer = GfGfxLoader_GetScrnData(anim->scroll->narcId, anim->scroll->scrnFile, FALSE, &scrnData, anim->heapID);
    ov41_02249F7C(anim->scroll->bgConfig, anim->scroll->bgId, scrnData, anim->scroll->width, anim->scroll->height, anim->scroll->x, anim->scroll->y, anim->scroll->tileOffset, anim->scroll->plttSlot);
    Heap_Free(buffer);

    memset(anim, 0, sizeof(UnkOv41BgScrollAnim));
}

void ov41_02249DB4(UnkOv41BgScroll *scroll, UnkOv41BgScrollTemplate *tmpl, int dx, int dy, int frames, int *doneFlag) {
    UnkOv41BgScrollTask *task = SysTask_GetData(CreateSysTaskAndEnvironment(ov41_02249F0C, sizeof(UnkOv41BgScrollTask), 0, HEAP_ID_13));

    task->scroll = scroll;
    task->tmpl = *tmpl;
    task->doneFlag = doneFlag;
    task->frames = frames;
    task->stepX = dx / frames;
    task->stepY = dy / frames;
    task->origX = Bg_GetXpos(scroll->bgConfig, (GFBgLayer)scroll->bgId);
    task->origY = Bg_GetYpos(scroll->bgConfig, (GFBgLayer)scroll->bgId);

    tmpl->tileOffset = 0x80;
    tmpl->plttSlot = 5;
    tmpl->x -= dx;
    tmpl->y -= dy;
    tmpl->heapID = HEAP_ID_14;
    ov41_02249E60(tmpl, NULL, NULL);
}

static void ov41_02249E40(NNSG2dScreenData *scrnData, int tileOffset) {
    int i;
    int n = scrnData->szByte / 2;
    u16 *data = (u16 *)scrnData->rawData;

    for (i = 0; i < n; i++) {
        data[i] += tileOffset;
    }
}

static void ov41_02249E60(UnkOv41BgScrollTemplate *tmpl, int *outWidth, int *outHeight) {
    NNSG2dScreenData *scrnData;
    void *buffer;
    int width;
    int height;
    int x = tmpl->x / 8;
    int y = tmpl->y / 8;

    ov41_02249FFC(tmpl->narcId, tmpl->charFile, tmpl->bgConfig, tmpl->bgId, tmpl->tileOffset, tmpl->heapID);
    ov41_0224A04C(tmpl->narcId, tmpl->plttFile, tmpl->bgId < GF_BG_LYR_SUB_0 ? 0 : 4, tmpl->plttSlot * 32, tmpl->plttCount * 32, tmpl->heapID);

    buffer = GfGfxLoader_GetScrnData(tmpl->narcId, tmpl->scrnFile, FALSE, &scrnData, tmpl->heapID);
    width = scrnData->screenWidth / 8;
    height = scrnData->screenHeight / 8;
    ov41_02249F7C(tmpl->bgConfig, tmpl->bgId, scrnData, width, height, x, y, tmpl->tileOffset, tmpl->plttSlot);
    Heap_Free(buffer);

    if (outWidth != NULL) {
        *outWidth = width;
    }
    if (outHeight != NULL) {
        *outHeight = height;
    }
}

static void ov41_02249F0C(SysTask *task, void *data) {
    UnkOv41BgScrollTask *env = data;

    if (--env->frames >= 0) {
        ScheduleSetBgPosText(env->scroll->bgConfig, env->scroll->bgId, BG_POS_OP_SUB_X, env->stepX);
        ScheduleSetBgPosText(env->scroll->bgConfig, env->scroll->bgId, BG_POS_OP_SUB_Y, env->stepY);
    } else {
        ScheduleSetBgPosText(env->scroll->bgConfig, env->scroll->bgId, BG_POS_OP_SET_X, env->origX);
        ScheduleSetBgPosText(env->scroll->bgConfig, env->scroll->bgId, BG_POS_OP_SET_Y, env->origY);
        ov41_02249C7C(env->scroll, &env->tmpl);
        if (env->doneFlag != NULL) {
            *env->doneFlag = TRUE;
        }
        DestroySysTaskAndEnvironment(task);
    }
}

static void ov41_02249F7C(BgConfig *bgConfig, int bgId, NNSG2dScreenData *scrnData, int width, int height, int x, int y, int tileOffset, int plttSlot) {
    ov41_02249E40(scrnData, tileOffset);
    CopyToBgTilemapRect(bgConfig, bgId, x, y, width, height, scrnData->rawData, 0, 0, width, height);
    BgTilemapRectChangePalette(bgConfig, bgId, x, y, width, height, plttSlot);
    ScheduleBgTilemapBufferTransfer(bgConfig, bgId);
}

static void ov41_02249FFC(NarcId narcId, int fileId, BgConfig *bgConfig, int bgId, int offset, enum HeapID heapID) {
    UnkOv41CharTransfer *transfer = Heap_Alloc(heapID, sizeof(UnkOv41CharTransfer));
    memset(transfer, 0, sizeof(UnkOv41CharTransfer));

    transfer->buffer = GfGfxLoader_GetCharData(narcId, fileId, FALSE, &transfer->charData, heapID);
    transfer->bgConfig = bgConfig;
    transfer->bgId = bgId;
    transfer->offset = offset;
    SysTask_CreateOnVWaitQueue(ov41_0224A094, transfer, 0x80);
}

static void ov41_0224A04C(NarcId narcId, int fileId, int location, int offset, int size, enum HeapID heapID) {
    UnkOv41PlttTransfer *transfer = Heap_Alloc(heapID, sizeof(UnkOv41PlttTransfer));
    memset(transfer, 0, sizeof(UnkOv41PlttTransfer));

    transfer->buffer = GfGfxLoader_GetPlttData(narcId, fileId, &transfer->plttData, heapID);
    transfer->location = location;
    transfer->offset = offset;
    transfer->size = size;
    SysTask_CreateOnVWaitQueue(ov41_0224A0D0, transfer, 0x80);
}

static void ov41_0224A094(SysTask *task, void *data) {
    UnkOv41CharTransfer *transfer = data;

    DC_FlushRange(transfer->charData->pRawData, transfer->charData->szByte);
    BG_LoadCharTilesData(transfer->bgConfig, transfer->bgId, transfer->charData->pRawData, transfer->charData->szByte, transfer->offset);
    SysTask_Destroy(task);
    Heap_Free(transfer->buffer);
    Heap_Free(transfer);
}

static void ov41_0224A0D0(SysTask *task, void *data) {
    UnkOv41PlttTransfer *transfer = data;

    DC_FlushRange(transfer->plttData->pRawData, transfer->size);
    if (transfer->location == 0) {
        GX_LoadBGPltt(transfer->plttData->pRawData, transfer->offset, transfer->size);
    } else if (transfer->location == 4) {
        GXS_LoadBGPltt(transfer->plttData->pRawData, transfer->offset, transfer->size);
    }
    SysTask_Destroy(task);
    Heap_Free(transfer->buffer);
    Heap_Free(transfer);
}

void ov41_0224A118(UnkOv41Button *button, const UnkOv41ButtonTemplate *tmpl) {
    GF_ASSERT(button != NULL);
    GF_ASSERT(tmpl != NULL);
    button->sprite = Sprite_CreateAffine(tmpl->spriteTemplate);
    GF_ASSERT(button->sprite != NULL);
    button->id = tmpl->id;
    button->arg = tmpl->arg;
    if (tmpl->callback != NULL) {
        button->callback = tmpl->callback;
    } else {
        button->callback = ov41_0224A254;
    }
}

void ov41_0224A15C(UnkOv41TextButton *button, const UnkOv41ButtonTemplate *tmpl) {
    TextOBJTemplate textObjTemplate;

    ov41_0224A118(&button->button, tmpl);
    textObjTemplate.fontSystem = tmpl->fontSystem;
    textObjTemplate.window = tmpl->window;
    textObjTemplate.spriteList = tmpl->spriteTemplate->spriteList;
    textObjTemplate.plttResourceProxy = tmpl->plttProxy;
    textObjTemplate.sprite = button->button.sprite;
    textObjTemplate.offset = tmpl->offset;
    textObjTemplate.x = tmpl->x;
    textObjTemplate.y = tmpl->y;
    textObjTemplate.unk_20 = 0;
    textObjTemplate.unk_24 = 0;
    textObjTemplate.vram = tmpl->spriteTemplate->whichScreen;
    textObjTemplate.heapID = tmpl->spriteTemplate->heapID;
    button->textObj = sub_020135D8(&textObjTemplate);
}

static void ov41_0224A1A8(UnkOv41Button *button) {
    Sprite_Delete(button->sprite);

    memset(button, 0, sizeof(UnkOv41Button));
}

static void ov41_0224A1C0(UnkOv41TextButton *button) {
    ov41_0224A1A8(&button->button);
    FontOAM_Delete(button->textObj);
    memset(button, 0, sizeof(UnkOv41TextButton));
}

void ov41_0224A1DC(UnkOv41Button *button, int id) {
    if (id == button->id) {
        button->callback(button, button->arg);
    }
}

void ov41_0224A1EC(UnkOv41ButtonBar *bar, int selected, int state) {
    int i;
    UnkOv41Button *button;

    for (i = 2, button = &bar->buttons[2]; i <= 3; i++, button++) {
        if (i != selected) {
            ov41_0224A264(button);
        } else if (state == 0) {
            ov41_0224A270(button);
            PlaySE(SEQ_SE_DP_PASO);
        } else if (state == 2) {
            ov41_0224A258(button);
        } else {
            ov41_0224A270(button);
        }
    }
}

void ov41_0224A238(UnkOv41Button *button, UnkOv41ButtonCallback callback, void *arg, int id) {
    GF_ASSERT(button != NULL);
    button->id = id;
    button->arg = arg;
    button->callback = callback;
}

static void ov41_0224A254(UnkOv41Button *button, void *arg) {
}

void ov41_0224A258(UnkOv41Button *button) {
    Sprite_SetAnimationFrame(button->sprite, 2);
}

void ov41_0224A264(UnkOv41Button *button) {
    Sprite_SetAnimationFrame(button->sprite, 0);
}

void ov41_0224A270(UnkOv41Button *button) {
    Sprite_SetAnimationFrame(button->sprite, 1);
}

void ov41_0224A27C(UnkOv41ButtonBar *bar, void *gfx, int *inputMode) {
    String *string;

    bar->inputMode = inputMode;
    bar->fontSystem = FontSystem_NewInit(1, HEAP_ID_13);
    ov41_0224A7F8(gfx);

    ov41_0224A6C4(&bar->buttons[0], 0, gfx, 0x30, 0x90, 0x28, 0x20);
    ov41_0224A7E0(bar->hitboxes, 0, 0x30, 0x98, 0x28, 0x18);
    ov41_0224A6C4(&bar->buttons[1], 1, gfx, 0x08, 0x90, 0x28, 0x20);
    ov41_0224A7E0(bar->hitboxes, 1, 0x08, 0x98, 0x28, 0x18);
    ov41_0224A6C4(&bar->buttons[2], 2, gfx, 0x60, 0x90, 0x28, 0x2A);
    ov41_0224A7E0(bar->hitboxes, 2, 0x60, 0x9C, 0x28, 0x22);
    ov41_0224A6C4(&bar->buttons[3], 3, gfx, 0x88, 0x90, 0x28, 0x2A);
    ov41_0224A7E0(bar->hitboxes, 3, 0x88, 0x9C, 0x28, 0x22);

    FontID_Alloc(2, HEAP_ID_14);
    string = ov41_0224A928(gfx, NARC_msgdata_msg, 0xD7, 0, 9, 5);
    ov41_0224A734(&bar->textButton, 4, gfx, bar->fontSystem, string, 0xB8, 0x90, 0x48, 0x2A);
    ov41_0224A7E0(bar->hitboxes, 4, 0xB8, 0x9C, 0x48, 0x22);
    ov41_0224A9B0(string);
    FontID_Release(2);

    bar->unk60 = 1;
    ov41_0224A888(gfx);
    ov41_0224A258(&bar->buttons[2]);
    bar->hitboxController = TouchHitboxController_Create(bar->hitboxes, 5, (TouchHitboxControllerCallback)ov41_0224A60C, bar, HEAP_ID_13);
}

void ov41_0224A3E4(UnkOv41ButtonBar *bar, void *gfx) {
    int i;

    for (i = 0; i < 5; i++) {
        ov41_02246388(gfx, i);
        ov41_0224639C(gfx, i);
        ObjCharTransfer_ResetTransferTasksByResID(i);
    }
    ObjPlttTransfer_FreeTaskByID(0);
    ObjPlttTransfer_FreeTaskByID(1);

    for (i = 0; i < 4; i++) {
        ov41_0224A1A8(&bar->buttons[i]);
    }
    sub_02021B5C(&bar->textButton.charVram);
    ov41_0224A1C0(&bar->textButton);
    sub_020135AC(bar->fontSystem);
    TouchHitboxController_Destroy(bar->hitboxController);
    bar->hitboxController = NULL;
}

static void ov41_0224A448(UnkOv41ButtonBar *bar) {
    int pressed = 0xFF;

    GF_ASSERT(bar->hitboxController != NULL);

    if (bar->timer != 0) {
        bar->timer--;
        ov41_0224A60C(bar->pressedButton, ov41_0224C094[bar->timer ^ 1], bar);
        return;
    }

    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        pressed = 4;
    } else if (gSystem.newKeys & PAD_KEY_UP) {
        pressed = 1;
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        pressed = 0;
    } else if (gSystem.newKeys & PAD_KEY_LEFT) {
        pressed = 2;
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        pressed = 3;
    }

    if (pressed != 0xFF) {
        bar->pressedButton = pressed;
        bar->timer = 2;
        ov41_0224A60C(bar->pressedButton, 0, bar);
    }
}

static BOOL ov41_0224A4EC(UnkOv41ButtonBar *bar) {
    if (bar->timer != 0) {
        return FALSE;
    }

    if (*bar->inputMode == 1) {
        if (System_GetTouchHeld()) {
            return FALSE;
        }
        if (gSystem.heldKeys) {
            *bar->inputMode = 0;
        }
    } else {
        if (gSystem.heldKeys) {
            return FALSE;
        }
        if (System_GetTouchHeld()) {
            *bar->inputMode = 1;
        }
    }
    return FALSE;
}

void ov41_0224A54C(UnkOv41ButtonBar *bar) {
    GF_ASSERT(bar->hitboxController != NULL);

    if (!ov41_0224A4EC(bar)) {
        if (*bar->inputMode == 0) {
            ov41_0224A448(bar);
        } else {
            TouchHitboxController_IsTriggered(bar->hitboxController);
        }
    }
}

void ov41_0224A580(UnkOv41ButtonBar *bar) {
    ov41_0224A8B0(&bar->textButton, 3);
    ov41_0224A8D4(&bar->textButton, 3);
    ov41_0224A1EC(bar, 2, 3);
}
