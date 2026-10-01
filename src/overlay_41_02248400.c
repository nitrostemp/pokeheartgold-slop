#include "overlay_41_02248400.h"

#include "global.h"

#include "heap.h"
#include "math_util.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "system.h"
#include "touchscreen.h"

typedef struct UnkOv41ObjTemplate {
    void *unk00; // 0x00
    void *unk04; // 0x04
    void *unk08; // 0x08
    void *unk0C; // 0x0C
    int unk10;   // 0x10
    int unk14;   // 0x14
    int unk18;   // 0x18
    int unk1C;   // 0x1C
} UnkOv41ObjTemplate;

typedef struct UnkOv41BgScrollTemplate {
    BgConfig *bgConfig; // 0x00
    int unk04;          // 0x04
    int unk08;          // 0x08
    int unk0C;          // 0x0C
    int unk10;          // 0x10
    int unk14;          // 0x14
    int unk18;          // 0x18
    int unk1C;          // 0x1C
    int unk20;          // 0x20
    int unk24;          // 0x24
    int unk28;          // 0x28
    int unk2C;          // 0x2C
} UnkOv41BgScrollTemplate;

typedef struct UnkOv41Anim {
    UnkOv41Node *node; // 0x0
    int frames;        // 0x4
    int dy;            // 0x8
} UnkOv41Anim;

typedef struct UnkOv41SwapTask {
    UnkOv41Board *board; // 0x00
    int fromType;        // 0x04
    int fromSlot;        // 0x08
    int toType;          // 0x0C
    int toSlot;          // 0x10
    int doneFlag;        // 0x14
    int counter;         // 0x18
    int state;           // 0x1C
    int fromCount;       // 0x20
    int toCount;         // 0x24
    UnkOv41Anim *anims;  // 0x28
    int animCount;       // 0x2C
} UnkOv41SwapTask;

void *ov41_02245EE0(UnkOv41ObjTemplate *tmpl);
void ov41_02246008(void *obj, BOOL visible);
void ov41_02246014(void *obj, int priority);
int ov41_02248EF4(UnkOv41FashionTable *tbl, int id);
UnkOv41Node *ov41_022499F0(void *pool, void *obj, int type);
void ov41_02249A50(UnkOv41Node *node, UnkOv41Node *where);
void ov41_02249A60(UnkOv41Node *node);
void ov41_02249A70(UnkOv41Node *node);
BOOL ov41_02249AA8(UnkOv41Node *node, int a1, int a2, int a3);
void ov41_02249AF4(UnkOv41Node *node, int x, int y);
void ov41_02249B44(UnkOv41Node *node, int *x, int *y);
void ov41_02249B94(UnkOv41Node *node, int *w, int *h);
void ov41_02249BAC(UnkOv41Node *node, int *m0, int *m1, int *m2, int *m3);
void ov41_02249BE8(UnkOv41Node *head, int dx, int dy);
void ov41_02249C7C(void *scroll, UnkOv41BgScrollTemplate *tmpl);
void ov41_02249CC4(void *scroll);
void ov41_02249DB4(void *scroll, UnkOv41BgScrollTemplate *tmpl, int a2, int a3, int frames, int *doneFlag);
u8 sub_0202BAB0(FashionCase *fashionCase, int a1);
extern s32 _s32_div_f(s32 num, s32 den);

static void ov41_02248584(int type, int idx, int *outX, int *outY, int w, int h, UnkOv41FashionTable *tbl);
static void ov41_02248984(UnkOv41Board *board, int type, int slot, int dx, int dy);
static void ov41_022489A8(UnkOv41Board *board, const UnkOv41BoardTemplate *tmpl);
static void ov41_022489E4(UnkOv41Node *head, BOOL visible);
static void ov41_02248A08(UnkOv41Board *board, int type, int slot, BOOL visible);
static void ov41_02248A18(UnkOv41Board *board, BOOL visible);
static void ov41_02248A28(UnkOv41NodeList *list, int count);
static void ov41_02248A6C(UnkOv41NodeList *list);
static UnkOv41Node *ov41_02248A94(UnkOv41Board *board);
static UnkOv41Node *ov41_02248ABC(UnkOv41Board *board, int type, int slot);
static int ov41_02248AE0(UnkOv41Board *board, int type, int slot);
static UnkOv41Node *ov41_02248AFC(UnkOv41Board *board, int type, int slot, int index);
static void ov41_02248B20(UnkOv41Board *board, void *obj, int type, int slot);
static void ov41_02248B48(int pos, int *outX, int *outY);
static void ov41_02248B84(UnkOv41Board *board, int fromType, int fromSlot, int toType, int toSlot);
static void ov41_02248BFC(SysTask *task, void *taskData);
static void ov41_02248D64(UnkOv41Node *node, UnkOv41Anim *anims, int count);
static UnkOv41Anim *ov41_02248D7C(UnkOv41Anim *anims, int count);
static void ov41_02248DA4(UnkOv41Anim *anims, int count);
static void ov41_02248DC8(UnkOv41Anim *anim);
static int ov41_02248E10(int n, int d);
static void ov41_02248E80(UnkOv41TouchCtx *ctx);

void ov41_02248400(UnkOv41Node *node, int *outDx, int *outDy) {
    int w, h, x, y;
    int m0, m2, m1, m3;
    int dxL, dxR, dyT, dyB;

    ov41_02249B94(node, &w, &h);
    ov41_02249B44(node, &x, &y);
    ov41_02249BAC(node, &m0, &m1, &m2, &m3);

    dxL = 0x8A - (x + m0);
    dxR = (x + w) - m1 - 0xF6;
    dyT = 0x12 - (y + m2);
    dyB = (y + h) - m3 - 0x8F;

    if (dxL > 0) {
        *outDx = dxL;
    } else if (dxR > 0) {
        *outDx = -dxR;
    } else {
        *outDx = 0;
    }

    if (dyT > 0) {
        *outDy = dyT;
    } else if (dyB > 0) {
        *outDy = -dyB;
    } else {
        *outDy = 0;
    }
}

void ov41_02248488(UnkOv41Board *board, const UnkOv41BoardTemplate *tmpl) {
    board->unk44 = tmpl->unk00;
    board->unk48 = tmpl->unk04;
    board->unk4C = tmpl->unk08;
    board->unk50 = tmpl->unk0C;
    board->bgConfig = tmpl->bgConfig;
    board->unk58 = tmpl->unk14;
    board->pool = tmpl->pool;
    board->fashionTable = tmpl->fashionTable;
    ov41_0224888C(board, 0);
    ov41_022489A8(board, tmpl);
}

void ov41_022484C0(UnkOv41Board *board) {
    int i;

    ov41_022486F8(board);
    for (i = 0; i < 4; i++) {
        ov41_02248A6C(&board->lists[i]);
    }
    memset(board, 0, sizeof(UnkOv41Board));
}

int ov41_022484E8(int type, int idx, UnkOv41FashionTable *tbl) {
    switch (type) {
    case 0:
        if (idx <= 5) {
            return 0;
        } else if (idx <= 11) {
            return 1;
        } else if (idx <= 17) {
            return 2;
        } else if (idx <= 21) {
            return 3;
        } else if (idx <= 28) {
            return 4;
        } else if (idx <= 33) {
            return 5;
        } else if (idx <= 38) {
            return 6;
        } else if (idx <= 42) {
            return 7;
        } else if (idx <= 49) {
            return 8;
        } else if (idx <= 55) {
            return 9;
        } else if (idx <= 60) {
            return 10;
        } else if (idx <= 71) {
            return 11;
        } else if (idx <= 91) {
            return 12;
        } else if (idx <= 99) {
            return 13;
        }
        break;
    case 1:
        return ov41_02248EF4(tbl, idx) / 9;
    case 2:
        return ov41_02248EF4(tbl, idx) / 9;
    }
}

static void ov41_02248584(int type, int idx, int *outX, int *outY, int w, int h, UnkOv41FashionTable *tbl) {
    switch (type) {
    case 0:
        *outX = 10;
        *outY = 0x12;
        *outX += MTRandom() % (0x6C - w);
        *outY += MTRandom() % (0x7D - h);
        break;
    case 1:
    case 2:
        ov41_02248B48(ov41_02248EF4(tbl, idx), outX, outY);
        break;
    }
}

void ov41_022485DC(UnkOv41Board *board, int type, int idx) {
    int resIdx;
    void *obj;
    UnkOv41Node *node;
    UnkOv41ObjTemplate tmpl;
    int x, y;
    int w, h;
    int frameIdx;
    int slot;

    tmpl.unk00 = board->unk58;
    tmpl.unk18 = idx;
    tmpl.unk04 = board->unk44;
    tmpl.unk10 = 0;
    tmpl.unk14 = 0;

    switch (type) {
    case 0:
        resIdx = idx;
        frameIdx = 0;
        tmpl.unk1C = board->unk50[idx];
        break;
    case 1:
        resIdx = idx;
        resIdx += 100;
        frameIdx = idx + 1;
        tmpl.unk1C = 0;
        break;
    case 2:
        resIdx = idx;
        resIdx += 100;
        frameIdx = idx + 1;
        tmpl.unk1C = 0;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    tmpl.unk08 = board->unk48[resIdx];
    tmpl.unk0C = board->unk4C[frameIdx];
    GF_ASSERT(tmpl.unk08 != NULL);
    GF_ASSERT(tmpl.unk0C != NULL);

    slot = ov41_022484E8(type, idx, board->fashionTable);
    obj = ov41_02245EE0(&tmpl);
    node = ov41_022499F0(board->pool, obj, type);
    ov41_02249A50(node, board->lists[type].nodes[slot].prev);
    ov41_02249B94(node, &w, &h);
    ov41_02248584(type, idx, &x, &y, w, h, board->fashionTable);
    ov41_02249AF4(node, x, y);
    ov41_02248B20(board, obj, type, slot);
}

void ov41_022486C4(UnkOv41Board *board, int type, int slot, UnkOv41Node *node) {
    ov41_02249A50(node, &board->lists[type].nodes[slot]);
    ov41_02248B20(board, node->obj, type, slot);
}

void ov41_022486F0(UnkOv41Node *node) {
    ov41_02249A60(node);
}

void ov41_022486F8(UnkOv41Board *board) {
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < board->lists[i].count; j++) {
            ov41_02249A70(&board->lists[i].nodes[j]);
        }
    }
}

void ov41_02248724(UnkOv41Board *board) {
    UnkOv41Node *node;
    int priority = -1;
    UnkOv41Node *head = ov41_02248A94(board);

    for (node = head->next; node != head; node = node->next) {
        if (node->type <= 2) {
            ov41_02246014(node->obj, priority);
        }
        priority--;
    }
}

BOOL ov41_02248750(UnkOv41Board *board, int type, int slot) {
    if (board->busy == FALSE) {
        ov41_02248B84(board, board->curList, board->lists[board->curList].sel, type, slot);
        board->curList = type;
        board->lists[type].sel = slot;
        ov41_02248724(board);
        return TRUE;
    }
    return FALSE;
}

BOOL ov41_02248790(UnkOv41Board *board, int type, int dir) {
    int cur = ov41_0224895C(board, type);
    int i;
    int slot;
    UnkOv41Node *head;

    for (i = 1; i < board->lists[type].count; i++) {
        if (dir == 0) {
            slot = (i + cur) % board->lists[type].count;
        } else {
            slot = cur - i;
            if (slot < 0) {
                slot += board->lists[type].count;
            }
        }
        head = ov41_02248ABC(board, type, slot);
        if (head->next != head) {
            return ov41_02248750(board, type, slot);
        }
    }
    return FALSE;
}

void ov41_022487F8(UnkOv41Board *board, int type, int slot) {
    ov41_02248A18(board, FALSE);
    board->curList = type;
    board->lists[type].sel = slot;
    ov41_02248A18(board, TRUE);
    ov41_02248724(board);
}

BOOL ov41_02248820(UnkOv41Board *board) {
    TouchscreenHitbox hitbox;

    hitbox.rect.top = 0x12;
    hitbox.rect.bottom = 0x8F;
    hitbox.rect.left = 10;
    hitbox.rect.right = 0x76;
    return TouchscreenHitbox_TouchHeldIsIn(&hitbox);
}

BOOL ov41_0224883C(UnkOv41Board *board, u32 x, u32 y) {
    TouchscreenHitbox hitbox;

    hitbox.rect.top = 0x12;
    hitbox.rect.bottom = 0x8F;
    hitbox.rect.left = 10;
    hitbox.rect.right = 0x76;
    return TouchscreenHitbox_PointIsIn(&hitbox, x, y);
}

UnkOv41Node *ov41_02248858(UnkOv41Board *board, int a1, int a2, int a3) {
    UnkOv41Node *node;
    UnkOv41Node *head = ov41_02248A94(board);

    for (node = head->next; node != head; node = node->next) {
        if (ov41_02249AA8(node, a1, a2, a3) == TRUE) {
            return node;
        }
    }
    return NULL;
}

void ov41_0224888C(UnkOv41Board *board, int page) {
    UnkOv41BgScrollTemplate tmpl;

    tmpl.bgConfig = board->bgConfig;
    tmpl.unk04 = 0x1A;
    tmpl.unk08 = page * 2 + 0x81;
    tmpl.unk0C = 0x85;
    tmpl.unk10 = page * 2 + 0x82;
    tmpl.unk14 = 8;
    tmpl.unk18 = 0x81;
    tmpl.unk1C = 3;
    tmpl.unk20 = 1;
    tmpl.unk24 = 2;
    tmpl.unk28 = 0;
    tmpl.unk2C = 0xE;
    ov41_02249C7C(board->unk5C, &tmpl);
    board->page = page;
}

void ov41_022488D8(UnkOv41Board *board, int page, u32 flags, int frames, int *doneFlag) {
    UnkOv41BgScrollTemplate tmpl;
    int a2;
    int a3;

    tmpl.bgConfig = board->bgConfig;
    tmpl.unk04 = 0x1A;
    tmpl.unk08 = page * 2 + 0x81;
    tmpl.unk0C = 0x85;
    tmpl.unk10 = page * 2 + 0x82;
    tmpl.unk14 = 8;
    tmpl.unk18 = 0x81;
    tmpl.unk1C = 3;
    tmpl.unk20 = 1;
    tmpl.unk24 = 2;
    tmpl.unk28 = 0;
    tmpl.unk2C = 0xE;

    a2 = 0;
    if (flags & 1) {
        a2 = 0x70;
    }
    if (flags & 2) {
        a3 = 0x81;
    } else {
        a3 = 0;
    }
    ov41_02249DB4(board->unk5C, &tmpl, a2, a3, frames, doneFlag);
    board->page = page;
}

void ov41_02248940(UnkOv41Board *board) {
    ov41_02249CC4(board->unk5C);
}

int ov41_0224894C(UnkOv41Board *board) {
    GF_ASSERT(board != NULL);
    return board->curList;
}

int ov41_0224895C(UnkOv41Board *board, int type) {
    UnkOv41NodeList list;

    GF_ASSERT(board != NULL);
    list = board->lists[type];
    return list.sel;
}

static void ov41_02248984(UnkOv41Board *board, int type, int slot, int dx, int dy) {
    ov41_02249BE8(ov41_02248ABC(board, type, slot), dx, dy);
}

BOOL ov41_02248998(UnkOv41Board *board) {
    if (board->busy == FALSE) {
        return TRUE;
    }
    return FALSE;
}

static void ov41_022489A8(UnkOv41Board *board, const UnkOv41BoardTemplate *tmpl) {
    ov41_02248A28(&board->lists[0], tmpl->count0);
    board->curList = 0;
    ov41_022489E4(board->lists[0].nodes, TRUE);
    ov41_02248A28(&board->lists[1], tmpl->count1);
    ov41_02248A28(&board->lists[2], tmpl->count2);
    ov41_02248A28(&board->lists[3], 1);
}

static void ov41_022489E4(UnkOv41Node *head, BOOL visible) {
    UnkOv41Node *node;

    for (node = head->next; node != head; node = node->next) {
        if (node->type <= 2) {
            ov41_02246008(node->obj, visible);
        }
    }
}

static void ov41_02248A08(UnkOv41Board *board, int type, int slot, BOOL visible) {
    ov41_022489E4(ov41_02248ABC(board, type, slot), visible);
}

static void ov41_02248A18(UnkOv41Board *board, BOOL visible) {
    ov41_022489E4(ov41_02248A94(board), visible);
}

static void ov41_02248A28(UnkOv41NodeList *list, int count) {
    int i;

    list->nodes = Heap_Alloc(HEAP_ID_14, count * sizeof(UnkOv41Node));
    list->count = count;
    list->sel = 0;
    for (i = 0; i < list->count; i++) {
        list->nodes[i].next = &list->nodes[i];
        list->nodes[i].prev = &list->nodes[i];
        ov41_022489E4(&list->nodes[i], FALSE);
    }
}

static void ov41_02248A6C(UnkOv41NodeList *list) {
    Heap_Free(list->nodes);
    list->nodes = NULL;
    memset(list, 0, sizeof(UnkOv41NodeList));
}

static UnkOv41Node *ov41_02248A94(UnkOv41Board *board) {
    UnkOv41NodeList list = board->lists[board->curList];
    return &list.nodes[list.sel];
}

static UnkOv41Node *ov41_02248ABC(UnkOv41Board *board, int type, int slot) {
    UnkOv41NodeList list = board->lists[type];
    return &list.nodes[slot];
}

static int ov41_02248AE0(UnkOv41Board *board, int type, int slot) {
    int count = 0;
    UnkOv41Node *head = ov41_02248ABC(board, type, slot);
    UnkOv41Node *node;

    for (node = head->next; node != head; node = node->next) {
        count++;
    }
    return count;
}

static UnkOv41Node *ov41_02248AFC(UnkOv41Board *board, int type, int slot, int index) {
    UnkOv41Node *node;
    int i = 0;
    UnkOv41Node *head = ov41_02248ABC(board, type, slot);

    for (node = head->next; node != head; node = node->next) {
        if (i == index) {
            return node;
        }
        i++;
    }
    return NULL;
}

static void ov41_02248B20(UnkOv41Board *board, void *obj, int type, int slot) {
    if (board->curList != type || slot != board->lists[type].sel) {
        ov41_02246008(obj, FALSE);
    } else {
        ov41_02246008(obj, TRUE);
    }
}

// NONMATCHING: register tie-break; MWCC puts row in r4 and outY in r6 (retail: outY r4, row r6)
#ifdef NONMATCHING
static void ov41_02248B48(int pos, int *outX, int *outY) {
    int cell = pos % 9;
    int row = cell / 3;
    int col = cell % 3;

    *outY = (row + 1) * 8 + row * 32 + 0x10;
    *outX = (col + 1) * 8 + col * 24 + 8;
}
#else
// clang-format off
asm static void ov41_02248B48(int pos, int *outX, int *outY) {
    push {r3, r4, r5, r6, r7, lr}
    add r5, r1, #0
    mov r1, #9
    add r4, r2, #0
    bl _s32_div_f
    add r7, r1, #0
    add r0, r7, #0
    mov r1, #3
    bl _s32_div_f
    add r6, r0, #0
    add r0, r7, #0
    mov r1, #3
    bl _s32_div_f
    add r0, r6, #1
    lsl r2, r0, #3
    lsl r0, r6, #5
    add r0, r2, r0
    add r0, #0x10
    str r0, [r4, #0]
    add r0, r1, #1
    lsl r2, r0, #3
    mov r0, #0x18
    mul r0, r1
    add r0, r2, r0
    add r0, #8
    str r0, [r5, #0]
    pop {r3, r4, r5, r6, r7, pc}
}
// clang-format on
#endif // NONMATCHING

static void ov41_02248B84(UnkOv41Board *board, int fromType, int fromSlot, int toType, int toSlot) {
    UnkOv41SwapTask *data = SysTask_GetData(CreateSysTaskAndEnvironment(ov41_02248BFC, sizeof(UnkOv41SwapTask), 0, HEAP_ID_13));

    data->board = board;
    data->fromType = fromType;
    data->fromSlot = fromSlot;
    data->toType = toType;
    data->toSlot = toSlot;
    data->state = 0;
    data->fromCount = ov41_02248AE0(board, fromType, fromSlot);
    data->toCount = ov41_02248AE0(board, toType, toSlot);
    data->animCount = data->fromCount + data->toCount;
    data->anims = Heap_Alloc(HEAP_ID_13, data->animCount * sizeof(UnkOv41Anim));
    GF_ASSERT(data->anims != NULL);
    memset(data->anims, 0, data->animCount * sizeof(UnkOv41Anim));
    board->busy = TRUE;
}

static void ov41_02248BFC(SysTask *task, void *taskData) {
    UnkOv41SwapTask *data = taskData;
    int i;

    switch (data->state) {
    case 0:
        ov41_02248984(data->board, data->toType, data->toSlot, 0, -0x84);
        ov41_02248A08(data->board, data->toType, data->toSlot, TRUE);
        data->doneFlag = 0;
        ov41_022488D8(data->board, (data->board->page + 1) % 2, 2, 5, &data->doneFlag);
        data->counter = ov41_02248E10(data->fromCount, 1);
        data->state++;
        break;
    case 1:
        for (i = 0; i < data->counter; i++) {
            if (data->fromCount - 1 >= 0) {
                data->fromCount--;
                ov41_02248D64(ov41_02248AFC(data->board, data->fromType, data->fromSlot, data->fromCount), data->anims, data->animCount);
            }
        }
        if (data->fromCount == 0) {
            data->counter = ov41_02248E10(data->toCount, 2);
            data->state++;
        }
        break;
    case 2:
        for (i = 0; i < data->counter; i++) {
            if (data->toCount - 1 >= 0) {
                data->toCount--;
                ov41_02248D64(ov41_02248AFC(data->board, data->toType, data->toSlot, data->toCount), data->anims, data->animCount);
            }
        }
        if (data->toCount == 0) {
            data->state++;
            data->counter = 0;
        }
        break;
    case 3:
        if (++data->counter > 3 && data->doneFlag) {
            data->state++;
        }
        break;
    case 4:
        ov41_02248A08(data->board, data->fromType, data->fromSlot, FALSE);
        ov41_02248984(data->board, data->fromType, data->fromSlot, 0, -0x84);
        data->board->busy = FALSE;
        Heap_Free(data->anims);
        DestroySysTaskAndEnvironment(task);
        return;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    ov41_02248DA4(data->anims, data->animCount);
}

static void ov41_02248D64(UnkOv41Node *node, UnkOv41Anim *anims, int count) {
    UnkOv41Anim *anim = ov41_02248D7C(anims, count);

    anim->node = node;
    anim->frames = 3;
    anim->dy = 0x2C;
}

static UnkOv41Anim *ov41_02248D7C(UnkOv41Anim *anims, int count) {
    int i;

    for (i = 0; i < count; i++) {
        if (anims[i].node == NULL) {
            return &anims[i];
        }
    }
    return NULL;
}

static void ov41_02248DA4(UnkOv41Anim *anims, int count) {
    int i;

    for (i = 0; i < count; i++) {
        if (anims[i].node != NULL) {
            ov41_02248DC8(&anims[i]);
        }
    }
}

static void ov41_02248DC8(UnkOv41Anim *anim) {
    int x, y;

    ov41_02249B44(anim->node, &x, &y);
    y += anim->dy;
    ov41_02249AF4(anim->node, x, y);
    if (--anim->frames <= 0) {
        memset(anim, 0, sizeof(UnkOv41Anim));
    }
}

static int ov41_02248E10(int n, int d) {
    return (n + (d - n % d)) / d;
}

void ov41_02248E28(UnkOv41TouchCtx *ctx) {
    memset(ctx, 0, sizeof(UnkOv41TouchCtx));
    ctx->onNew = ov41_02248E80;
    ctx->onRelease = ov41_02248E80;
    ctx->onHeld = ov41_02248E80;
}

void ov41_02248E44(UnkOv41TouchCtx *ctx) {
    if (gSystem.touchNew) {
        ctx->onNew(ctx);
    } else if (gSystem.touchHeld) {
        ctx->onHeld(ctx);
    } else if (ctx->prevHeld) {
        ctx->onRelease(ctx);
    }
    ctx->x = gSystem.touchX;
    ctx->y = gSystem.touchY;
    ctx->prevHeld = gSystem.touchHeld;
}

static void ov41_02248E80(UnkOv41TouchCtx *ctx) {
}

void ov41_02248E84(FashionCase *fashionCase, UnkOv41FashionTable *tbl) {
    int i;
    u8 slot;

    for (i = 0; i < 100; i++) {
        tbl->counts[i] = sub_0202BA70(fashionCase, i);
    }
    for (i = 0; i < 18; i++) {
        tbl->slotToId[i] = 18;
    }
    for (i = 0; i < 18; i++) {
        slot = sub_0202BAB0(fashionCase, i);
        if (slot != 18) {
            tbl->slotToId[slot] = i;
        }
    }
}
