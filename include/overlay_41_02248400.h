#ifndef POKEHEARTGOLD_OVERLAY_41_02248400_H
#define POKEHEARTGOLD_OVERLAY_41_02248400_H

#include "bg_window.h"
#include "fashion_case.h"

typedef struct UnkOv41Node {
    void *obj;                // 0x0
    u32 type;                 // 0x4
    struct UnkOv41Node *next; // 0x8
    struct UnkOv41Node *prev; // 0xC
} UnkOv41Node;

typedef struct UnkOv41NodeList {
    UnkOv41Node *nodes; // 0x0
    int count;          // 0x4
    int sel;            // 0x8
} UnkOv41NodeList;

typedef struct UnkOv41FashionTable {
    u32 counts[100];  // 0x000
    int slotToId[18]; // 0x190
} UnkOv41FashionTable;

typedef struct UnkOv41Board {
    int curList;                       // 0x00
    UnkOv41FashionTable *fashionTable; // 0x04
    void *pool;                        // 0x08
    UnkOv41NodeList lists[4];          // 0x0C
    BOOL busy;                         // 0x3C
    int page;                          // 0x40
    void *unk44;                       // 0x44
    void **unk48;                      // 0x48
    void **unk4C;                      // 0x4C
    u8 *unk50;                         // 0x50
    BgConfig *bgConfig;                // 0x54
    void *unk58;                       // 0x58
    u8 unk5C[0x2C];                    // 0x5C
    u8 unk88[4];                       // 0x88
} UnkOv41Board;                        // size: 0x8C

typedef struct UnkOv41BoardTemplate {
    void *unk00;                       // 0x00
    void **unk04;                      // 0x04
    void **unk08;                      // 0x08
    u8 *unk0C;                         // 0x0C
    BgConfig *bgConfig;                // 0x10
    void *unk14;                       // 0x14
    void *pool;                        // 0x18
    int count0;                        // 0x1C
    int count1;                        // 0x20
    int count2;                        // 0x24
    UnkOv41FashionTable *fashionTable; // 0x28
} UnkOv41BoardTemplate;

typedef struct UnkOv41TouchCtx UnkOv41TouchCtx;
typedef void (*UnkOv41TouchCallback)(UnkOv41TouchCtx *ctx);

struct UnkOv41TouchCtx {
    void *unk00;                    // 0x00
    UnkOv41TouchCallback onNew;     // 0x04
    UnkOv41TouchCallback onRelease; // 0x08
    UnkOv41TouchCallback onHeld;    // 0x0C
    void *unk10;                    // 0x10
    u16 x;                          // 0x14
    u16 y;                          // 0x16
    u8 prevHeld;                    // 0x18
}; // size: 0x1C

void ov41_02248400(UnkOv41Node *node, int *outDx, int *outDy);
void ov41_02248488(UnkOv41Board *board, const UnkOv41BoardTemplate *tmpl);
void ov41_022484C0(UnkOv41Board *board);
int ov41_022484E8(int type, int idx, UnkOv41FashionTable *tbl);
void ov41_022485DC(UnkOv41Board *board, int type, int idx);
void ov41_022486C4(UnkOv41Board *board, int type, int slot, UnkOv41Node *node);
void ov41_022486F0(UnkOv41Node *node);
void ov41_022486F8(UnkOv41Board *board);
void ov41_02248724(UnkOv41Board *board);
BOOL ov41_02248750(UnkOv41Board *board, int type, int slot);
BOOL ov41_02248790(UnkOv41Board *board, int type, int dir);
void ov41_022487F8(UnkOv41Board *board, int type, int slot);
BOOL ov41_02248820(UnkOv41Board *board);
BOOL ov41_0224883C(UnkOv41Board *board, u32 x, u32 y);
UnkOv41Node *ov41_02248858(UnkOv41Board *board, int a1, int a2, int a3);
void ov41_0224888C(UnkOv41Board *board, int page);
void ov41_022488D8(UnkOv41Board *board, int page, u32 flags, int frames, int *doneFlag);
void ov41_02248940(UnkOv41Board *board);
int ov41_0224894C(UnkOv41Board *board);
int ov41_0224895C(UnkOv41Board *board, int type);
BOOL ov41_02248998(UnkOv41Board *board);
void ov41_02248E28(UnkOv41TouchCtx *ctx);
void ov41_02248E44(UnkOv41TouchCtx *ctx);
void ov41_02248E84(FashionCase *fashionCase, UnkOv41FashionTable *tbl);

#endif // POKEHEARTGOLD_OVERLAY_41_02248400_H
