#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "pm_string.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013534.h"

typedef struct UnkOv41Gfx {
    u8 unk0[0x40];
    BgConfig *bgConfig;         // 0x40
    SpriteList *spriteList;     // 0x44
    GF_2DGfxResMan *resMans[4]; // 0x48
    u8 unk58[0x128];
    NARC *narc; // 0x180
} UnkOv41Gfx;

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
    SpriteTemplate *spriteTemplate; // 0x00
    UnkOv41ButtonCallback callback; // 0x04
    void *arg;                      // 0x08
    int id;                         // 0x0C
} UnkOv41ButtonTemplate;            // size: 0x10

typedef struct UnkOv41TextButtonTemplate {
    UnkOv41ButtonTemplate base;         // 0x00
    Window *window;                     // 0x10
    UnkStruct_02013534 *fontSystem;     // 0x14
    NNSG2dImagePaletteProxy *plttProxy; // 0x18
    int x;                              // 0x1C
    int y;                              // 0x20
    u32 offset;                         // 0x24
} UnkOv41TextButtonTemplate;            // size: 0x28

typedef struct UnkOv41ButtonBar {
    UnkOv41Button buttons[4];     // 0x00
    UnkOv41TextButton textButton; // 0x40
    int unk60;                    // 0x60
} UnkOv41ButtonBar;

typedef struct UnkOv41SpriteRes {
    SpriteResource *res[4]; // 0x00
} UnkOv41SpriteRes;

typedef struct UnkOv41SpriteGrid {
    UnkOv41SpriteRes res; // 0x00
    Sprite *sprites[20];  // 0x10
    int count;            // 0x60
    int unk64;            // 0x64
} UnkOv41SpriteGrid;      // size: 0x68

typedef struct UnkOv41PanelSub {
    UnkOv41SpriteRes res; // 0x00
    Sprite *sprites[2];   // 0x10
    Window *window;       // 0x18
    int unk1C;            // 0x1C
    int unk20;            // 0x20
    u8 unk24[8];          // 0x24
    int *unk2C;           // 0x2C
    u8 unk30[0x60];       // 0x30
    int unk90;            // 0x90
} UnkOv41PanelSub;        // size: 0x94

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

typedef struct UnkOv41Panel {
    UnkOv41BgScroll scroll;   // 0x000
    Window *window;           // 0x02C
    SpriteList *spriteList;   // 0x030
    GF_2DGfxResMan **resMans; // 0x034
    UnkOv41SpriteGrid grid;   // 0x038
    UnkOv41PanelSub sub;      // 0x0A0
    Window *titleWindow;      // 0x134
    u32 flags;                // 0x138
    int frame;                // 0x13C
    int textFrameDelay;       // 0x140
    String *string;           // 0x144
} UnkOv41Panel;               // size: 0x148

void ov41_022462E4(UnkOv41Gfx *gfx, NARC *narc, int fileId, BOOL compressed, int vram, int id);
void ov41_02246304(UnkOv41Gfx *gfx, NARC *narc, int fileId, BOOL compressed, int vram, int count, int id);
void ov41_02246328(UnkOv41Gfx *gfx, NARC *narc, int fileId, BOOL compressed, int id);
void ov41_02246344(UnkOv41Gfx *gfx, NARC *narc, int fileId, BOOL compressed, int id);
void ov41_02246360(UnkOv41Gfx *gfx, int id);
void ov41_02246374(UnkOv41Gfx *gfx, int id);
void ov41_02249C7C(UnkOv41BgScroll *scroll, UnkOv41BgScrollTemplate *tmpl);
void ov41_02249CC4(UnkOv41BgScroll *scroll);
void ov41_0224A118(UnkOv41Button *button, const UnkOv41ButtonTemplate *tmpl);
void ov41_0224A15C(UnkOv41TextButton *button, const UnkOv41TextButtonTemplate *tmpl);
void ov41_0224A1DC(UnkOv41Button *button, int id);
void ov41_0224A1EC(UnkOv41ButtonBar *bar, int selected, int state);
void ov41_0224A238(UnkOv41Button *button, UnkOv41ButtonCallback callback, void *arg, int id);
void ov41_0224A258(UnkOv41Button *button);
void ov41_0224A264(UnkOv41Button *button);
void ov41_0224A270(UnkOv41Button *button);
void ov41_0224B21C(UnkOv41PanelSub *sub, GF_2DGfxResMan **resMans);
void ov41_0224B250(UnkOv41PanelSub *sub);
void ov41_0224B298(UnkOv41PanelSub *sub);
void sub_02013728(TextOBJ *textOBJ);

void ov41_0224A5A4(UnkOv41ButtonBar *bar, int dx, int dy);
void ov41_0224A5D4(UnkOv41ButtonBar *bar, int id, UnkOv41ButtonCallback callback, void *arg, int buttonId);
void ov41_0224A60C(int button, int state, UnkOv41ButtonBar *bar);
void ov41_0224A6C4(UnkOv41Button *button, int id, UnkOv41Gfx *gfx, int x, int y, int w, int h);
void ov41_0224A734(UnkOv41TextButton *button, int id, UnkOv41Gfx *gfx, UnkStruct_02013534 *fontSystem, Window *window, int x, int y, int w, int h);
void ov41_0224A7E0(TouchscreenHitbox *hitboxes, int index, int x, int y, int w, int h);
void ov41_0224A7F8(UnkOv41Gfx *gfx);
void ov41_0224A888(UnkOv41Gfx *gfx);
void ov41_0224A8B0(UnkOv41Button *button, int state);
void ov41_0224A8D4(UnkOv41TextButton *button, int state);
static void ov41_0224A918(UnkOv41Button *button, int se, int trigger, int state);
Window *ov41_0224A928(UnkOv41Gfx *gfx, NarcId narcId, int fileId, int msgId, int width, int height);
void ov41_0224A9B0(Window *window);
static void ov41_0224A9BC(UnkOv41Button *button, int dx, int dy);
static void ov41_0224A9F8(UnkOv41TextButton *button, int dx, int dy);
void ov41_0224AA08(UnkOv41Panel *panel, const UnkOv41PanelArgs *args, u32 flags);
void ov41_0224AB40(UnkOv41Panel *panel);
void ov41_0224ABF0(UnkOv41Panel *panel);
void ov41_0224AC08(UnkOv41Panel *panel, NarcId narcId, int fileId, int msgId);
void ov41_0224AC40(UnkOv41Panel *panel, NarcId narcId, int fileId, int msgId);
void ov41_0224AC80(UnkOv41Panel *panel);
void ov41_0224AC98(UnkOv41Panel *panel, int count);
static void ov41_0224ACA4(UnkOv41Panel *panel, BgConfig *bgConfig);
static void ov41_0224ACDC(BgConfig *bgConfig, Options *options);
static void ov41_0224AD0C(Window **out, BgConfig *bgConfig, int x, int y, int width, int height, int baseTile, BOOL drawFrame);
static void ov41_0224AD7C(UnkOv41BgScroll *scroll);
void ov41_0224AD84(Window *window);
static u8 ov41_0224AD90(Window *window, NarcId narcId, int fileId, int msgId, int x, int y, int textSpeed);
static u8 ov41_0224ADD8(Window *window, NarcId narcId, int fileId, int msgId, int x, int y, int textSpeed, String **out);
static u8 ov41_0224AE24(Window *window, NarcId narcId, int fileId, int msgId, int x, int y, u32 color, int textSpeed);
static u8 ov41_0224AE78(Window *window, NarcId narcId, int fileId, int msgId, int x, int y, u32 color, int textSpeed, String **out);
static void ov41_0224AED8(UnkOv41SpriteGrid *grid, SpriteList *spriteList, GF_2DGfxResMan **resMans, int count, NARC *narc);
static void ov41_0224AF8C(UnkOv41SpriteGrid *grid, int count);
static void ov41_0224AFD4(UnkOv41SpriteGrid *grid, GF_2DGfxResMan **resMans);
static void ov41_0224AFF8(UnkOv41SpriteRes *res, GF_2DGfxResMan **resMans, enum HeapID heapID, NARC *narc, int charFile, int plttFile, int cellFile, int animFile, int plttCount, int idBase);
void ov41_0224B084(UnkOv41SpriteRes *res, GF_2DGfxResMan **resMans);
static void ov41_0224B0B8(UnkOv41SpriteRes *res, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int priority);
static void ov41_0224B118(UnkOv41PanelSub *sub, SpriteList *spriteList, GF_2DGfxResMan **resMans, int a3, BgConfig *bgConfig, int *a5, NARC *narc);

void ov41_0224A5A4(UnkOv41ButtonBar *bar, int dx, int dy) {
    int i;

    for (i = 0; i < 4; i++) {
        ov41_0224A9BC(&bar->buttons[i], dx, dy);
    }
    ov41_0224A9F8(&bar->textButton, dx, dy);
}

void ov41_0224A5D4(UnkOv41ButtonBar *bar, int id, UnkOv41ButtonCallback callback, void *arg, int buttonId) {
    GF_ASSERT(bar != NULL);
    if (id < 4) {
        ov41_0224A238(&bar->buttons[id], callback, arg, buttonId);
    } else {
        ov41_0224A238(&bar->textButton.button, callback, arg, buttonId);
    }
}

void ov41_0224A60C(int button, int state, UnkOv41ButtonBar *bar) {
    switch (button) {
    case 0:
        ov41_0224A8B0(&bar->buttons[0], state);
        ov41_0224A918(&bar->buttons[0], SEQ_SE_DP_MAZYO3, 0, state);
        ov41_0224A1DC(&bar->buttons[0], state);
        break;
    case 1:
        ov41_0224A8B0(&bar->buttons[1], state);
        ov41_0224A918(&bar->buttons[1], SEQ_SE_DP_MAZYO3, 0, state);
        ov41_0224A1DC(&bar->buttons[1], state);
        break;
    case 2:
    case 3:
        ov41_0224A1EC(bar, button, state);
        ov41_0224A1DC(&bar->buttons[button], state);
        break;
    case 4:
        if (bar->unk60 == 1) {
            ov41_0224A8B0(&bar->textButton.button, state);
            ov41_0224A8D4(&bar->textButton, state);
            ov41_0224A918(&bar->textButton.button, SEQ_SE_DP_PIRORIRO, 0, state);
            ov41_0224A1DC(&bar->textButton.button, state);
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

void ov41_0224A6C4(UnkOv41Button *button, int id, UnkOv41Gfx *gfx, int x, int y, int w, int h) {
    UnkOv41ButtonTemplate buttonTemplate;
    SpriteResourcesHeader header;
    SpriteTemplate spriteTemplate;

    CreateSpriteResourcesHeader(&header, id, 0, id, id, -1, -1, 0, 0, gfx->resMans[0], gfx->resMans[1], gfx->resMans[2], gfx->resMans[3], NULL, NULL);
    spriteTemplate.spriteList = gfx->spriteList;
    spriteTemplate.header = &header;
    spriteTemplate.position.x = x << FX32_SHIFT;
    spriteTemplate.position.y = y << FX32_SHIFT;
    spriteTemplate.position.z = 0;
    spriteTemplate.drawPriority = 2;
    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTemplate.heapID = HEAP_ID_14;
    buttonTemplate.spriteTemplate = &spriteTemplate;
    buttonTemplate.callback = NULL;
    buttonTemplate.arg = NULL;
    buttonTemplate.id = 1;
    ov41_0224A118(button, &buttonTemplate);
}

void ov41_0224A734(UnkOv41TextButton *button, int id, UnkOv41Gfx *gfx, UnkStruct_02013534 *fontSystem, Window *window, int x, int y, int w, int h) {
    UnkOv41TextButtonTemplate buttonTemplate;
    SpriteResourcesHeader header;
    SpriteTemplate spriteTemplate;

    CreateSpriteResourcesHeader(&header, id, 0, id, id, -1, -1, 0, 0, gfx->resMans[0], gfx->resMans[1], gfx->resMans[2], gfx->resMans[3], NULL, NULL);
    spriteTemplate.spriteList = gfx->spriteList;
    spriteTemplate.header = &header;
    spriteTemplate.position.x = x << FX32_SHIFT;
    spriteTemplate.position.y = y << FX32_SHIFT;
    spriteTemplate.position.z = 0;
    spriteTemplate.drawPriority = 2;
    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTemplate.heapID = HEAP_ID_14;
    buttonTemplate.base.spriteTemplate = &spriteTemplate;
    buttonTemplate.base.callback = NULL;
    buttonTemplate.base.arg = NULL;
    buttonTemplate.base.id = 1;
    buttonTemplate.fontSystem = fontSystem;
    buttonTemplate.window = window;
    buttonTemplate.x = 0;
    buttonTemplate.y = 0x13;
    buttonTemplate.plttProxy = SpriteTransfer_GetPaletteProxy(SpriteResourceCollection_Find(gfx->resMans[1], 1), NULL);
    GF_ASSERT(sub_02021AC8(sub_02013688(window, NNS_G2D_VRAM_TYPE_2DMAIN, 0xD), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &button->charVram));
    buttonTemplate.offset = button->charVram.offset;
    ov41_0224A15C(button, &buttonTemplate);
}

void ov41_0224A7E0(TouchscreenHitbox *hitboxes, int index, int x, int y, int w, int h) {
    hitboxes[index].rect.top = y;
    hitboxes[index].rect.left = x;
    hitboxes[index].rect.bottom = y + h;
    hitboxes[index].rect.right = x + w;
}

void ov41_0224A7F8(UnkOv41Gfx *gfx) {
    int i;

    for (i = 0; i < 5; i++) {
        ov41_022462E4(gfx, gfx->narc, i * 3 + 0x6B, FALSE, 1, i);
        ov41_02246328(gfx, gfx->narc, i * 3 + 0x6A, FALSE, i);
        ov41_02246344(gfx, gfx->narc, i * 3 + 0x69, FALSE, i);
    }
    ov41_02246304(gfx, gfx->narc, 0x68, FALSE, 1, 3, 0);
    ov41_02246304(gfx, gfx->narc, 0x78, FALSE, 1, 2, 1);
}

void ov41_0224A888(UnkOv41Gfx *gfx) {
    int i;

    for (i = 0; i < 5; i++) {
        ov41_02246360(gfx, i);
    }
    ov41_02246374(gfx, 0);
    ov41_02246374(gfx, 1);
}

void ov41_0224A8B0(UnkOv41Button *button, int state) {
    if (state == 0) {
        ov41_0224A270(button);
    } else if (state == 2) {
        ov41_0224A258(button);
    } else if (state == 1 || state == 3) {
        ov41_0224A264(button);
    }
}

void ov41_0224A8D4(UnkOv41TextButton *button, int state) {
    if (state == 0) {
        sub_020136B4(button->textObj, 0, 0xF);
        TextOBJ_SetPaletteNum(button->textObj, 4);
        return;
    }
    if (state == 1) {
        sub_020136B4(button->textObj, 0, 0x13);
    }
    if (state == 3) {
        sub_020136B4(button->textObj, 0, 0x13);
        TextOBJ_SetPaletteNum(button->textObj, 3);
    }
}

static void ov41_0224A918(UnkOv41Button *button, int se, int trigger, int state) {
    if (state == trigger) {
        PlaySE(se);
    }
}

Window *ov41_0224A928(UnkOv41Gfx *gfx, NarcId narcId, int fileId, int msgId, int width, int height) {
    MsgData *msgData;
    String *string;
    Window *window;
    u32 x;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, narcId, fileId, HEAP_ID_13);
    GF_ASSERT(msgData != NULL);
    string = NewString_ReadMsgData(msgData, msgId);
    window = AllocWindows(HEAP_ID_14, 1);
    InitWindow(window);
    AddTextWindowTopLeftCorner(gfx->bgConfig, window, width, height, 0, 0);
    x = FontID_String_GetCenterAlignmentX(2, string, 0, width * 8);
    AddTextPrinterParameterizedWithColor(window, 2, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 3), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    return window;
}

void ov41_0224A9B0(Window *window) {
    WindowArray_Delete(window, 1);
}

static void ov41_0224A9BC(UnkOv41Button *button, int dx, int dy) {
    VecFx32 pos = *Sprite_GetMatrixPtr(button->sprite);

    pos.x += dx << FX32_SHIFT;
    pos.y += dy << FX32_SHIFT;
    Sprite_SetMatrix(button->sprite, &pos);
}

static void ov41_0224A9F8(UnkOv41TextButton *button, int dx, int dy) {
    ov41_0224A9BC(&button->button, dx, dy);
    sub_02013728(button->textObj);
}

void ov41_0224AA08(UnkOv41Panel *panel, const UnkOv41PanelArgs *args, u32 flags) {
    if (flags & 1) {
        memset(panel, 0, sizeof(UnkOv41Panel));
    }
    if (flags & 2) {
        ov41_0224ACA4(panel, args->bgConfig);
    }
    if (flags & 4) {
        ov41_0224AD0C(&panel->window, args->bgConfig, 2, 0x13, 0x1B, 4, 0x1F, TRUE);
    }
    if (flags & 8) {
        ov41_0224AED8(&panel->grid, args->spriteList, args->resMans, args->count, args->narc);
    }
    if (flags & 0x10) {
        ov41_0224B118(&panel->sub, args->spriteList, args->resMans, args->unk1C, args->bgConfig, args->unk20, args->narc);
    }
    if (flags & 0x20) {
        ov41_0224AD0C(&panel->titleWindow, args->bgConfig, 2, 1, 0x1B, 2, 0x8B, TRUE);
        FillWindowPixelBuffer(panel->titleWindow, 0xF);
        ov41_0224AE24(panel->titleWindow, NARC_msgdata_msg, args->msgFile, 6, 0, 0, MAKE_TEXT_COLOR(1, 2, 15), TEXT_SPEED_NOTRANSFER);
        ov41_0224AE24(panel->titleWindow, NARC_msgdata_msg, args->msgFile, args->msgId, 0x48, 0, MAKE_TEXT_COLOR(1, 2, 15), TEXT_SPEED_NOTRANSFER);
        DrawFrameAndWindow2(panel->titleWindow, FALSE, 1, 1);
    }
    panel->spriteList = args->spriteList;
    panel->resMans = args->resMans;
    ov41_0224ACDC(args->bgConfig, args->options);
    panel->frame = Options_GetFrame(args->options);
    panel->textFrameDelay = Options_GetTextFrameDelay(args->options);
    panel->flags |= flags;
}

void ov41_0224AB40(UnkOv41Panel *panel) {
    if (panel->flags & 2) {
        ov41_0224AD7C(&panel->scroll);
        panel->flags &= ~2;
    }
    if (panel->flags & 4) {
        ov41_0224AD84(panel->window);
        panel->flags &= ~4;
    }
    if (panel->flags & 8) {
        ov41_0224AFD4(&panel->grid, panel->resMans);
        panel->flags &= ~8;
    }
    if (panel->flags & 0x10) {
        ov41_0224B21C(&panel->sub, panel->resMans);
        panel->flags &= ~0x10;
    }
    if (panel->flags & 0x20) {
        ov41_0224AD84(panel->titleWindow);
        panel->flags &= ~0x20;
    }
    memset(panel, 0, sizeof(UnkOv41Panel));
}

void ov41_0224ABF0(UnkOv41Panel *panel) {
    if (panel->flags & 0x10) {
        ov41_0224B250(&panel->sub);
    }
}

void ov41_0224AC08(UnkOv41Panel *panel, NarcId narcId, int fileId, int msgId) {
    GF_ASSERT(panel->flags & 4);
    ov41_0224AD90(panel->window, narcId, fileId, msgId, 0, 0, TEXT_SPEED_NOTRANSFER);
}

void ov41_0224AC40(UnkOv41Panel *panel, NarcId narcId, int fileId, int msgId) {
    GF_ASSERT(panel->flags & 4);
    ov41_0224ADD8(panel->window, narcId, fileId, msgId, 0, 0, panel->textFrameDelay, &panel->string);
}

void ov41_0224AC80(UnkOv41Panel *panel) {
    String_Delete(panel->string);
    panel->string = NULL;
}

void ov41_0224AC98(UnkOv41Panel *panel, int count) {
    ov41_0224AF8C(&panel->grid, count);
}

static void ov41_0224ACA4(UnkOv41Panel *panel, BgConfig *bgConfig) {
    UnkOv41BgScrollTemplate tmpl;

    tmpl.bgConfig = bgConfig;
    tmpl.narcId = NARC_a_0_2_6;
    tmpl.charFile = 0xE0;
    tmpl.plttFile = 0xE1;
    tmpl.scrnFile = 0xE2;
    tmpl.x = 0;
    tmpl.y = 0;
    tmpl.bgId = 4;
    tmpl.plttCount = 1;
    tmpl.plttSlot = 0;
    tmpl.tileOffset = 0;
    tmpl.heapID = HEAP_ID_14;
    ov41_02249C7C(&panel->scroll, &tmpl);
}

static void ov41_0224ACDC(BgConfig *bgConfig, Options *options) {
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_SUB_1, 1, 1, Options_GetFrame(options), HEAP_ID_14);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_2_OFFSET, HEAP_ID_14);
}

static void ov41_0224AD0C(Window **out, BgConfig *bgConfig, int x, int y, int width, int height, int baseTile, BOOL drawFrame) {
    *out = AllocWindows(HEAP_ID_14, 1);
    InitWindow(*out);
    AddWindowParameterized(bgConfig, *out, GF_BG_LYR_SUB_1, x, y, width, height, 2, baseTile);
    FillWindowPixelBuffer(*out, 0xF);
    if (drawFrame) {
        DrawFrameAndWindow2(*out, FALSE, 1, 1);
    }
    CopyWindowToVram(*out);
}

static void ov41_0224AD7C(UnkOv41BgScroll *scroll) {
    ov41_02249CC4(scroll);
}

void ov41_0224AD84(Window *window) {
    WindowArray_Delete(window, 1);
}

static u8 ov41_0224AD90(Window *window, NarcId narcId, int fileId, int msgId, int x, int y, int textSpeed) {
    u8 printerId;

    FillWindowPixelBuffer(window, 0xF);
    printerId = ov41_0224AE24(window, narcId, fileId, msgId, x, y, MAKE_TEXT_COLOR(1, 2, 15), textSpeed);
    DrawFrameAndWindow2(window, FALSE, 1, 1);
    return printerId;
}

static u8 ov41_0224ADD8(Window *window, NarcId narcId, int fileId, int msgId, int x, int y, int textSpeed, String **out) {
    u8 printerId;

    FillWindowPixelBuffer(window, 0xF);
    printerId = ov41_0224AE78(window, narcId, fileId, msgId, x, y, MAKE_TEXT_COLOR(1, 2, 15), textSpeed, out);
    DrawFrameAndWindow2(window, FALSE, 1, 1);
    return printerId;
}

static u8 ov41_0224AE24(Window *window, NarcId narcId, int fileId, int msgId, int x, int y, u32 color, int textSpeed) {
    MsgData *msgData;
    String *string;
    u8 printerId;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, narcId, fileId, HEAP_ID_13);
    GF_ASSERT(msgData != NULL);
    string = NewString_ReadMsgData(msgData, msgId);
    printerId = AddTextPrinterParameterizedWithColor(window, 1, string, x, y, textSpeed, color, NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    return printerId;
}

static u8 ov41_0224AE78(Window *window, NarcId narcId, int fileId, int msgId, int x, int y, u32 color, int textSpeed, String **out) {
    MsgData *msgData;
    u8 printerId;

    GF_ASSERT(*out == NULL);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, narcId, fileId, HEAP_ID_13);
    GF_ASSERT(msgData != NULL);
    *out = NewString_ReadMsgData(msgData, msgId);
    printerId = AddTextPrinterParameterizedWithColor(window, 1, *out, x, y, textSpeed, color, NULL);
    DestroyMsgData(msgData);
    return printerId;
}

static void ov41_0224AED8(UnkOv41SpriteGrid *grid, SpriteList *spriteList, GF_2DGfxResMan **resMans, int count, NARC *narc) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate tmpl;
    int i, j;

    ov41_0224AFF8(&grid->res, resMans, HEAP_ID_14, narc, 0x67, 0xE1, 0x66, 0x65, 2, 2000);
    ov41_0224B0B8(&grid->res, resMans, &header, 0);
    tmpl.spriteList = spriteList;
    tmpl.header = &header;
    tmpl.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    tmpl.priority = 0;
    tmpl.heapID = HEAP_ID_14;

    for (i = 0; i < 2; i++) {
        tmpl.position.y = i * 0x12 + 0x68;
        tmpl.position.y <<= FX32_SHIFT;
        tmpl.position.y += 512 << FX32_SHIFT;
        for (j = 0; j < 10; j++) {
            tmpl.position.x = j * 0x12 + 0x26;
            tmpl.position.x <<= FX32_SHIFT;
            grid->sprites[i * 10 + j] = Sprite_Create(&tmpl);
            Sprite_SetAnimCtrlSeq(grid->sprites[i * 10 + j], 1);
            if (i * 10 + j >= count) {
                Sprite_SetDrawFlag(grid->sprites[i * 10 + j], FALSE);
            }
        }
    }
}

static void ov41_0224AF8C(UnkOv41SpriteGrid *grid, int count) {
    int i;

    if (grid->count < count) {
        for (i = grid->count; i < count; i++) {
            Sprite_SetAnimCtrlSeq(grid->sprites[i], 0);
        }
    } else if (grid->count > count) {
        for (i = grid->count - 1; i >= count; i--) {
            Sprite_SetAnimCtrlSeq(grid->sprites[i], 1);
        }
    }
    grid->count = count;
}

static void ov41_0224AFD4(UnkOv41SpriteGrid *grid, GF_2DGfxResMan **resMans) {
    int i;

    for (i = 0; i < 20; i++) {
        Sprite_Delete(grid->sprites[i]);
    }
    ov41_0224B084(&grid->res, resMans);
}

static void ov41_0224AFF8(UnkOv41SpriteRes *res, GF_2DGfxResMan **resMans, enum HeapID heapID, NARC *narc, int charFile, int plttFile, int cellFile, int animFile, int plttCount, int idBase) {
    res->res[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_CHAR], narc, charFile, FALSE, idBase + charFile, NNS_G2D_VRAM_TYPE_2DSUB, heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->res[GF_GFX_RES_TYPE_CHAR]);
    sub_0200A740(res->res[GF_GFX_RES_TYPE_CHAR]);
    res->res[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_PLTT], narc, plttFile, FALSE, idBase + plttFile, NNS_G2D_VRAM_TYPE_2DSUB, plttCount, heapID);
    SpriteTransfer_CreatePlttTransferTask(res->res[GF_GFX_RES_TYPE_PLTT]);
    sub_0200A740(res->res[GF_GFX_RES_TYPE_PLTT]);
    res->res[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_CELL], narc, cellFile, FALSE, idBase + cellFile, GF_GFX_RES_TYPE_CELL, heapID);
    res->res[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_ANIM], narc, animFile, FALSE, idBase + animFile, GF_GFX_RES_TYPE_ANIM, heapID);
}

void ov41_0224B084(UnkOv41SpriteRes *res, GF_2DGfxResMan **resMans) {
    SpriteTransfer_DeleteCharTransferTask(res->res[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(res->res[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(resMans[GF_GFX_RES_TYPE_CHAR], res->res[GF_GFX_RES_TYPE_CHAR]);
    DestroySingle2DGfxResObj(resMans[GF_GFX_RES_TYPE_PLTT], res->res[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(resMans[GF_GFX_RES_TYPE_CELL], res->res[GF_GFX_RES_TYPE_CELL]);
    DestroySingle2DGfxResObj(resMans[GF_GFX_RES_TYPE_ANIM], res->res[GF_GFX_RES_TYPE_ANIM]);
}

static void ov41_0224B0B8(UnkOv41SpriteRes *res, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int priority) {
    int charId = GF2DGfxResObj_GetResID(res->res[GF_GFX_RES_TYPE_CHAR]);
    int plttId = GF2DGfxResObj_GetResID(res->res[GF_GFX_RES_TYPE_PLTT]);
    int cellId = GF2DGfxResObj_GetResID(res->res[GF_GFX_RES_TYPE_CELL]);
    int animId = GF2DGfxResObj_GetResID(res->res[GF_GFX_RES_TYPE_ANIM]);

    CreateSpriteResourcesHeader(header, charId, plttId, cellId, animId, -1, -1, 0, priority, resMans[GF_GFX_RES_TYPE_CHAR], resMans[GF_GFX_RES_TYPE_PLTT], resMans[GF_GFX_RES_TYPE_CELL], resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

static void ov41_0224B118(UnkOv41PanelSub *sub, SpriteList *spriteList, GF_2DGfxResMan **resMans, int a3, BgConfig *bgConfig, int *a5, NARC *narc) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate tmpl;
    int i;
    NNS_G2D_VRAM_TYPE screen;

    ov41_0224AFF8(&sub->res, resMans, HEAP_ID_14, narc, 0xE5, 0xE6, 0xE4, 0xE3, 2, 3000);
    ov41_0224B0B8(&sub->res, resMans, &header, 0);
    tmpl.header = &header;
    screen = NNS_G2D_VRAM_TYPE_2DSUB;
    tmpl.heapID = HEAP_ID_14;
    tmpl.spriteList = spriteList;
    tmpl.position.y = 58 << FX32_SHIFT;
    tmpl.whichScreen = screen;
    tmpl.priority = 0;
    tmpl.position.y += screen << 20;

    for (i = 0; i < 2; i++) {
        tmpl.position.x = i * 0x18 + 0x67;
        tmpl.position.x <<= FX32_SHIFT;
        sub->sprites[i] = Sprite_Create(&tmpl);
    }
    sub->unk1C = a3;
    sub->unk20 = a3 * 30;
    sub->unk2C = a5;
    sub->unk2C[0] = a3;
    sub->unk2C[2] = a3;
    sub->unk90 = 0;
    ov41_0224B298(sub);
    ov41_0224AD0C(&sub->window, bgConfig, 0xA, 8, 0xE, 4, 0xC1, FALSE);
    FillWindowPixelBuffer(sub->window, 0);
    ov41_0224AE24(sub->window, NARC_msgdata_msg, 0xD7, 4, 0, 4, MAKE_TEXT_COLOR(1, 2, 0), TEXT_SPEED_NOTRANSFER);
    ov41_0224AE24(sub->window, NARC_msgdata_msg, 0xD7, 5, 0x48, 4, MAKE_TEXT_COLOR(1, 2, 0), TEXT_SPEED_NOTRANSFER);
    CopyWindowToVram(sub->window);
}
