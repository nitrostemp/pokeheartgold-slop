#include "unk_02085604.h"

#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "system.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02013534.h"

void *sub_0203A4AC(enum HeapID heapID);

typedef struct UnkStruct_020850F4_Entry {
    int unk0;
    int unk4;
    int unk8;
    ManagedSprite *sprite;
    TouchscreenHitbox *hitbox;
    s16 dx;
    s16 dy;
    u8 counter;
    u8 scaleIdx;
    u8 unk1A[2];
} UnkStruct_020850F4_Entry;

typedef struct UnkStruct_020850F4_Range {
    u16 lo;
    u16 hi;
} UnkStruct_020850F4_Range;

struct UnkStruct_020850F4 {
    UnkStruct_020850F4_Entry digits[16];
    UnkStruct_020850F4_Entry seps[3];
    UnkStruct_020850F4_Entry cursors[3];
    UnkStruct_020850F4_Entry buttons[2];
    s16 groupX[5];
    UnkStruct_020850F4_Range groupRanges[5];
    int state;
    int unk2C4;
    int subState;
    int timer;
    int numDigitSlots;
    int curGroup;
    int prevGroup;
    int rangeLo;
    int rangeHi;
    int prevRangeLo;
    int prevRangeHi;
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    PaletteData *pltt;
    TouchHitboxController *touchCtrl;
    TouchscreenHitbox hitboxes[28];
    int unk374;
    UnkStruct_02013534 *fontSystem;
    TextOBJ *textObjs[2];
    UnkStruct_02021AC8 unk384[2];
    Window window;
    int action;
    int actionArg;
    int actionFlag;
    u8 unk3B8[4];
    int groupLen[5];
    u8 unk3D0[4];
    String *outStr;
    Options *options;
    int unk3DC;
    u32 number;
    int msgId;
    int unk3E8;
    int numSeps;
    int numDigits;
};

typedef struct UnkStruct_020850F4 UnkStruct_020850F4;

void sub_02086490(UnkStruct_020850F4 *ctx);
void sub_02086758(UnkStruct_020850F4 *ctx);
void sub_020868A0(UnkStruct_020850F4 *ctx);
void sub_020869BC(UnkStruct_020850F4 *ctx);
void sub_02086AB4(UnkStruct_020850F4 *ctx, int idx, int flag);
void sub_02086AE4(UnkStruct_020850F4 *ctx, int idx);
void sub_02086B2C(UnkStruct_020850F4 *ctx, int idx);
void sub_02086B6C(UnkStruct_020850F4 *ctx, int hitboxIdx, int spriteIdx);
void sub_02086BB4(UnkStruct_020850F4 *ctx);
void sub_02086C8C(UnkStruct_020850F4 *ctx);
int sub_02086D98(int anim, int flag);
void sub_02086DA4(UnkStruct_020850F4 *ctx);
void sub_02086DE4(UnkStruct_020850F4 *ctx, BOOL animate);
void sub_02086F44(UnkStruct_020850F4 *ctx);
void sub_02086FCC(UnkStruct_020850F4 *ctx);
void sub_02087064(UnkStruct_020850F4 *ctx);
void sub_020871C4(BgConfig *bgConfig, Window *window, int bgId, int x, int y, int width, int height, int baseTile, int msgId);

static void sub_02085604(UnkStruct_020850F4 *ctx, int group);
static void sub_02085808(UnkStruct_020850F4 *ctx, int state);
static BOOL sub_02085820(UnkStruct_020850F4 *ctx);
static BOOL sub_020858DC(UnkStruct_020850F4 *ctx);
static BOOL sub_02085938(UnkStruct_020850F4 *ctx);
static BOOL sub_02085974(UnkStruct_020850F4 *ctx);
static void sub_02085C20(UnkStruct_020850F4 *ctx);
static void sub_02085F80(UnkStruct_020850F4 *ctx);
static void sub_02085FFC(UnkStruct_020850F4 *ctx);
static void sub_02086180(u32 index, u32 event, void *arg);
static void sub_02086328(UnkStruct_020850F4 *ctx);
static void sub_02086384(UnkStruct_020850F4 *ctx);
static int sub_02086398(UnkStruct_020850F4 *ctx, int group);
static int sub_020863C0(UnkStruct_020850F4 *ctx, int group);

static f32 sScaleTableGrow[7] = { 0.5f, 0.2f, 0.5f, 1.0f, 1.2f, 1.0f, 1.0f };
static f32 sScaleTableShrink[7] = { 0.8f, 0.6f, 0.4f, 0.2f, 0.8f, 1.0f, 1.0f };

typedef struct UnkStruct_02085604_Rect {
    s16 x;
    s16 y;
    s16 halfWidth;
    s16 halfHeight;
} UnkStruct_02085604_Rect;

typedef struct UnkStruct_02085604_RectList {
    UnkStruct_02085604_Rect rects[12];
} UnkStruct_02085604_RectList;

static BOOL (*const sStateFuncs[])(UnkStruct_020850F4 *ctx) = {
    sub_02085820,
    sub_02085938,
    sub_02085974,
    sub_020858DC,
};

static const UnkStruct_02085604_RectList sButtonRects = {
    {
     { 0x20, 0x50, 0x14, 0x14 },
     { 0x50, 0x50, 0x14, 0x14 },
     { 0x80, 0x50, 0x14, 0x14 },
     { 0xB0, 0x50, 0x14, 0x14 },
     { 0xE0, 0x50, 0x14, 0x14 },
     { 0x20, 0x80, 0x14, 0x14 },
     { 0x50, 0x80, 0x14, 0x14 },
     { 0x80, 0x80, 0x14, 0x14 },
     { 0xB0, 0x80, 0x14, 0x14 },
     { 0xE0, 0x80, 0x14, 0x14 },
     { 0x40, 0xB0, 0x3C, 0x0C },
     { 0xC0, 0xB0, 0x3C, 0x0C },
     },
};

static void sub_02085604(UnkStruct_020850F4 *ctx, int group) {
    ctx->prevGroup = ctx->curGroup;
    ctx->curGroup = group;
    ctx->rangeLo = 0;
    ctx->rangeHi = 0;
    ctx->prevRangeLo = 0;
    ctx->prevRangeHi = 0;
    if (ctx->curGroup != 0) {
        ctx->rangeLo = ctx->groupRanges[ctx->curGroup - 1].lo;
        ctx->rangeHi = ctx->groupRanges[ctx->curGroup - 1].hi;
    }
    if (ctx->prevGroup != 0) {
        ctx->prevRangeLo = ctx->groupRanges[ctx->prevGroup - 1].lo;
        ctx->prevRangeHi = ctx->groupRanges[ctx->prevGroup - 1].hi;
    }
}

void sub_02085688(UnkStruct_020850F4 *ctx) {
    int i;
    int j;
    int group;
    int total;
    u16 offset;

    ctx->unk374 = 1;
    offset = 0;
    for (i = 0; i < 5; i++) {
        ctx->groupRanges[i].lo = offset;
        offset += ctx->groupLen[i];
        ctx->groupRanges[i].hi = offset;
    }
    sub_02085604(ctx, ctx->unk3DC + 1);
    for (i = 0; i < 4; i++) {
        if (ctx->groupLen[i] == 0) {
            break;
        }
        ctx->numDigitSlots += ctx->groupLen[i];
        ctx->numSeps++;
    }
    ctx->numSeps--;
    ctx->groupX[0] = 0x70 - ((ctx->numDigitSlots + ctx->numSeps) * 8) / 2;
    for (i = 0; i < 4; i++) {
        ctx->groupX[i + 1] = 0x70 - (ctx->numSeps * 8 + ((ctx->numDigitSlots - ctx->groupLen[i]) * 8 + ctx->groupLen[i] * 32)) / 2;
    }
    ctx->groupX[1] += 12;
    total = 0;
    for (i = 0; i < ctx->numSeps; i++) {
        total += ctx->groupLen[i];
        ctx->seps[i].unk0 = total - 1;
    }
    i = 0;
    group = 0;
    do {
        for (j = 0; j < ctx->groupLen[group]; j++) {
            ctx->digits[i].unk4 = group + 1;
            i++;
        }
        group++;
    } while (i < ctx->numDigitSlots);
    for (i = 0; i < ctx->unk3DC; i++) {
        ctx->numDigits += ctx->groupLen[i];
    }
}

static void sub_02085808(UnkStruct_020850F4 *ctx, int state) {
    ctx->state = state;
    ctx->unk2C4 = 0;
    ctx->subState = 0;
    ctx->timer = 0;
}

static BOOL sub_02085820(UnkStruct_020850F4 *ctx) {
    void *nclr;
    NNSG2dPaletteData *plttData;

    sub_02086490(ctx);
    sub_02086DA4(ctx);
    sub_02086758(ctx);
    sub_02086DE4(ctx, FALSE);
    sub_020868A0(ctx);
    sub_020869BC(ctx);
    sub_02086F44(ctx);
    sub_02086FCC(ctx);
    sub_02087064(ctx);
    sub_020871C4(ctx->bgConfig, &ctx->window, 4, 2, 0x15, 0x1B, 2, 0x64, ctx->msgId);
    if (ctx->unk3E8 != 0) {
        nclr = sub_0203A4AC(HEAP_ID_108);
        NNS_G2dGetUnpackedPaletteData(nclr, &plttData);
        PaletteData_LoadPalette(ctx->pltt, plttData->pRawData, PLTTBUF_SUB_OBJ, 0xE0, 0x20);
        Heap_Free(nclr);
    }
    sub_02085808(ctx, 1);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_108);
    return FALSE;
}

static BOOL sub_020858DC(UnkStruct_020850F4 *ctx) {
    switch (ctx->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_108);
        ctx->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            ctx->subState++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

static BOOL sub_02085938(UnkStruct_020850F4 *ctx) {
    if (ctx->subState == 0) {
        if (IsPaletteFadeFinished() == TRUE) {
            ctx->subState++;
        }
    } else {
        sub_02086328(ctx);
        TouchHitboxController_IsTriggered(ctx->touchCtrl);
        sub_02085C20(ctx);
    }
    return FALSE;
}

static BOOL sub_02085974(UnkStruct_020850F4 *ctx) {
    int i;

    switch (ctx->subState) {
    case 0:
        sub_02086AB4(ctx, 0, 0);
        for (i = 0; i < ctx->numDigitSlots; i++) {
            if (ctx->digits[i].counter != 0) {
                ManagedSprite_OffsetPositionXY(ctx->digits[i].sprite, ctx->digits[i].dx, ctx->digits[i].dy);
                ctx->digits[i].counter--;
                if (i >= ctx->rangeLo && i < ctx->rangeHi) {
                    ManagedSprite_SetAffineScale(ctx->digits[i].sprite, sScaleTableGrow[ctx->digits[i].scaleIdx], sScaleTableGrow[ctx->digits[i].scaleIdx]);
                    ctx->digits[i].scaleIdx++;
                }
                if (i >= ctx->prevRangeLo && i < ctx->prevRangeHi) {
                    ManagedSprite_SetAffineScale(ctx->digits[i].sprite, sScaleTableShrink[ctx->digits[i].scaleIdx], sScaleTableShrink[ctx->digits[i].scaleIdx]);
                    ctx->digits[i].scaleIdx++;
                }
            }
        }
        for (i = 0; i < ctx->numSeps; i++) {
            if (ctx->seps[i].counter != 0) {
                ManagedSprite_OffsetPositionXY(ctx->seps[i].sprite, ctx->seps[i].dx, ctx->seps[i].dy);
                ctx->seps[i].counter--;
            }
        }
        if (ctx->digits[0].counter == 0) {
            for (i = ctx->rangeLo; i < ctx->rangeHi; i++) {
                ManagedSprite_SetAnim(ctx->digits[i].sprite, sub_02086D98(ctx->digits[i].unk0, ctx->digits[i].unk8));
                ManagedSprite_TickFrame(ctx->digits[i].sprite);
            }
            for (i = ctx->prevRangeLo; i < ctx->prevRangeHi; i++) {
                ManagedSprite_SetAnim(ctx->digits[i].sprite, sub_02086D98(ctx->digits[i].unk0, ctx->digits[i].unk8));
                ManagedSprite_TickFrame(ctx->digits[i].sprite);
            }
            ctx->subState++;
        }
        ctx->timer++;
        break;
    case 1:
        for (i = ctx->rangeLo; i < ctx->rangeHi; i++) {
            if (ctx->digits[i].scaleIdx != 6) {
                ManagedSprite_SetAffineScale(ctx->digits[i].sprite, sScaleTableGrow[ctx->digits[i].scaleIdx], sScaleTableGrow[ctx->digits[i].scaleIdx]);
                ctx->digits[i].scaleIdx++;
            }
        }
        for (i = ctx->prevRangeLo; i < ctx->prevRangeHi; i++) {
            if (ctx->digits[i].scaleIdx != 6) {
                ManagedSprite_SetAffineScale(ctx->digits[i].sprite, sScaleTableShrink[ctx->digits[i].scaleIdx], sScaleTableShrink[ctx->digits[i].scaleIdx]);
                ctx->digits[i].scaleIdx++;
            }
        }
        ctx->timer++;
        if (ctx->timer == 6) {
            ctx->subState++;
        }
        break;
    default:
        sub_02086F44(ctx);
        if (ctx->actionFlag == 0) {
            sub_02086AE4(ctx, sub_02086398(ctx, ctx->actionArg));
        } else {
            sub_02086AE4(ctx, sub_020863C0(ctx, ctx->actionArg));
        }
        if (ctx->curGroup != 0) {
            sub_02086AB4(ctx, 0, 1);
        }
        sub_02086384(ctx);
        sub_02085808(ctx, 1);
        break;
    }
    return FALSE;
}

BOOL sub_02085BEC(UnkStruct_020850F4 *ctx) {
    BOOL ret = sStateFuncs[ctx->state](ctx);
    sub_02086BB4(ctx);
    sub_02086C8C(ctx);
    SpriteSystem_DrawSprites(ctx->spriteManager);
    return ret;
}

static void sub_02085C20(UnkStruct_020850F4 *ctx) {
    BOOL moved;
    int pos;
    int cell;
    int group;
    int nextGroup;
    int grid[3][5] = {
        { 0,  1,  2,  3,  4  },
        { 5,  6,  7,  8,  9  },
        { 10, 10, 10, 11, 11 },
    };

    moved = FALSE;
    cell = grid[ctx->cursors[1].dy][ctx->cursors[1].dx];
    if (ctx->state != 1 || ctx->action == 1) {
        return;
    }
    if (ctx->unk374 == 1) {
        if (gSystem.newKeys == 0 || System_GetTouchHeld()) {
            return;
        }
        ctx->unk374 = 0;
        sub_02086B2C(ctx, cell);
        if (cell == 10 || cell == 11) {
            if (ctx->cursors[1].unk0 != 2) {
                ctx->cursors[1].unk0 = 2;
                return;
            }
        } else {
            if (ctx->cursors[1].unk0 != 1) {
                ctx->cursors[1].unk0 = 1;
                return;
            }
        }
        return;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (ctx->cursors[1].dy > 0) {
            ctx->cursors[1].dy--;
        } else {
            ctx->cursors[1].dy = 2;
        }
        moved = TRUE;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        ctx->cursors[1].dy++;
        ctx->cursors[1].dy %= 3;
        moved = TRUE;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        if (cell == 10) {
            ctx->cursors[1].dx = 3;
        } else if (cell == 11) {
            ctx->cursors[1].dx = 0;
        } else {
            ctx->cursors[1].dx++;
            ctx->cursors[1].dx %= 5;
        }
        moved = TRUE;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        if (cell == 10) {
            ctx->cursors[1].dx = 3;
        } else if (cell == 11) {
            ctx->cursors[1].dx = 0;
        } else if (ctx->cursors[1].dx > 0) {
            ctx->cursors[1].dx--;
        } else {
            ctx->cursors[1].dx = 4;
        }
        moved = TRUE;
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        if (cell == 10) {
            sub_02085FFC(ctx);
            PlaySE(SEQ_SE_DP_BUTTON3);
        } else if (cell == 11) {
            sub_02085F80(ctx);
            PlaySE(SEQ_SE_DP_PIRORIRO);
        } else {
            if (ctx->curGroup == 0) {
                return;
            }
            pos = ctx->cursors[0].unk0;
            ctx->digits[pos].unk0 = cell + 1;
            sub_02086AB4(ctx, 1, 0);
            sub_02086AB4(ctx, 2, 1);
            sub_02086B6C(ctx, cell, 2);
            ManagedSprite_SetAnim(ctx->digits[pos].sprite, sub_02086D98(ctx->digits[pos].unk0, ctx->digits[pos].unk8));
            ManagedSprite_SetAnim(ctx->cursors[2].sprite, 3);
            group = ctx->digits[pos].unk4;
            if (pos + 1 == ctx->numDigitSlots) {
                moved = TRUE;
                ctx->action = 1;
                ctx->actionArg = 0;
                ctx->cursors[1].dx = 3;
                ctx->cursors[1].dy = 2;
            } else {
                nextGroup = ctx->digits[pos + 1].unk4;
                if (group != nextGroup) {
                    ctx->action = 1;
                    ctx->actionArg = nextGroup;
                } else {
                    ctx->action = 2;
                    ctx->actionArg = pos + 1;
                }
                PlaySE(SEQ_SE_DP_BUTTON3);
            }
        }
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        sub_02085FFC(ctx);
        PlaySE(SEQ_SE_DP_BUTTON3);
    } else if (gSystem.newAndRepeatedKeys & PAD_BUTTON_L) {
        if (ctx->cursors[0].unk0 == ctx->numDigits) {
            ctx->cursors[0].unk0 = ctx->numDigitSlots - 1;
        } else {
            ctx->cursors[0].unk0--;
        }
        pos = ctx->cursors[0].unk0;
        if (ctx->digits[pos].unk8 == 1) {
            ctx->action = 2;
            ctx->actionArg = pos;
        } else {
            ctx->action = 1;
            ctx->actionArg = ctx->digits[pos].unk4;
            ctx->actionFlag = 1;
        }
        PlaySE(SEQ_SE_DP_SELECT78);
    } else if (gSystem.newAndRepeatedKeys & PAD_BUTTON_R) {
        if (ctx->cursors[0].unk0 == ctx->numDigitSlots - 1) {
            ctx->cursors[0].unk0 = ctx->numDigits;
        } else {
            ctx->cursors[0].unk0++;
        }
        pos = ctx->cursors[0].unk0;
        if (ctx->digits[pos].unk8 == 1) {
            ctx->action = 2;
            ctx->actionArg = pos;
        } else {
            ctx->action = 1;
            ctx->actionArg = ctx->digits[pos].unk4;
        }
        PlaySE(SEQ_SE_DP_SELECT78);
    }
    if (moved == TRUE) {
        PlaySE(SEQ_SE_DP_SELECT78);
        cell = grid[ctx->cursors[1].dy][ctx->cursors[1].dx];
        sub_02086B2C(ctx, cell);
        if (cell == 10 || cell == 11) {
            if (ctx->cursors[1].unk0 != 2) {
                ctx->cursors[1].unk0 = 2;
                return;
            }
        } else {
            if (ctx->cursors[1].unk0 != 1) {
                ctx->cursors[1].unk0 = 1;
            }
        }
    }
}

static void sub_02085F80(UnkStruct_020850F4 *ctx) {
    int i;
    String *str;

    str = String_New(100, HEAP_ID_108);
    ctx->buttons[1].unk0 = 1;
    ctx->buttons[1].counter = 0;
    for (i = 0; i < ctx->numDigitSlots; i++) {
        if (ctx->digits[i].unk0 == 0) {
            ctx->digits[i].unk0 = 1;
            ManagedSprite_SetAnim(ctx->digits[i].sprite, sub_02086D98(ctx->digits[i].unk0, ctx->digits[i].unk8));
        }
        String16_FormatInteger(str, ctx->digits[i].unk0 - 1, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        String_Cat(ctx->outStr, str);
    }
    String_Delete(str);
    sub_02085808(ctx, 3);
}

static void sub_02085FFC(UnkStruct_020850F4 *ctx) {
    int pos;
    int group;
    int prevGroup;

    ctx->buttons[0].unk0 = 1;
    ctx->buttons[0].counter = 0;
    if (ctx->curGroup == 0) {
        ctx->cursors[0].unk0 = ctx->numDigitSlots - 1;
        group = ctx->digits[ctx->cursors[0].unk0].unk4;
        ctx->action = 1;
        ctx->actionArg = group;
        ctx->actionFlag = 1;
        return;
    }
    pos = ctx->cursors[0].unk0;
    ctx->digits[pos].unk0 = 0;
    ManagedSprite_SetAnim(ctx->digits[pos].sprite, sub_02086D98(ctx->digits[pos].unk0, ctx->digits[pos].unk8));
    group = ctx->digits[pos].unk4;
    if (pos > ctx->numDigits) {
        ManagedSprite_SetAnim(ctx->digits[pos - 1].sprite, sub_02086D98(ctx->digits[pos - 1].unk0, ctx->digits[pos - 1].unk8));
        prevGroup = ctx->digits[pos - 1].unk4;
        if (group != prevGroup) {
            ctx->action = 1;
            ctx->actionArg = prevGroup;
            ctx->actionFlag = 1;
            return;
        }
        ctx->action = 2;
        ctx->actionArg = pos - 1;
    }
}

void sub_020860B8(UnkStruct_020850F4 *ctx) {
    int i;
    UnkStruct_02085604_RectList rects;

    for (i = 0; i < 16; i++) {
        ctx->digits[i].hitbox = &ctx->hitboxes[i];
    }
    rects = sButtonRects;
    for (; i < 28; i++) {
        ctx->hitboxes[i].rect.top = rects.rects[i - 16].y - rects.rects[i - 16].halfHeight;
        ctx->hitboxes[i].rect.left = rects.rects[i - 16].x - rects.rects[i - 16].halfWidth;
        ctx->hitboxes[i].rect.bottom = rects.rects[i - 16].y + rects.rects[i - 16].halfHeight;
        ctx->hitboxes[i].rect.right = rects.rects[i - 16].x + rects.rects[i - 16].halfWidth;
    }
    ctx->touchCtrl = TouchHitboxController_Create(ctx->hitboxes, 28, sub_02086180, ctx, HEAP_ID_108);
}

static void sub_02086180(u32 index, u32 event, void *arg) {
    UnkStruct_020850F4 *ctx = arg;
    int pos;
    int group;
    int nextGroup;

    if (ctx->state != 1) {
        return;
    }
    if (ctx->unk374 != 1) {
        ctx->unk374 = 1;
    }
    if (event == 0) {
        if (index < 16) {
            if (index >= ctx->numDigits) {
                if (ctx->digits[index].unk8 == 1) {
                    ctx->action = 2;
                    ctx->actionArg = index;
                } else {
                    ctx->action = 1;
                    ctx->actionArg = ctx->digits[index].unk4;
                }
                PlaySE(SEQ_SE_DP_BUTTON3);
            }
            return;
        }
        if (index == 26) {
            ctx->cursors[1].dx = 0;
            ctx->cursors[1].dy = 2;
            PlaySE(SEQ_SE_DP_BUTTON3);
        } else if (index == 27) {
            ctx->cursors[1].dx = 3;
            ctx->cursors[1].dy = 2;
            PlaySE(SEQ_SE_DP_PIRORIRO);
        } else {
            ctx->cursors[1].dx = (index - 16) % 5;
            ctx->cursors[1].dy = (index - 16) / 5;
            PlaySE(SEQ_SE_DP_BUTTON3);
        }
        if (index >= 16 && index <= 25) {
            if (ctx->curGroup == 0) {
                return;
            }
            pos = ctx->cursors[0].unk0;
            ctx->digits[pos].unk0 = index - 15;
            ManagedSprite_SetAnim(ctx->digits[pos].sprite, sub_02086D98(ctx->digits[pos].unk0, ctx->digits[pos].unk8));
            sub_02086AB4(ctx, 1, 1);
            sub_02086B2C(ctx, index - 16);
            sub_02086AB4(ctx, 1, 0);
            sub_02086AB4(ctx, 2, 1);
            sub_02086B6C(ctx, index - 16, 2);
            ManagedSprite_SetAnim(ctx->cursors[2].sprite, 3);
            group = ctx->digits[pos].unk4;
            if (pos + 1 == ctx->numDigitSlots) {
                ctx->action = 1;
                ctx->actionArg = 0;
                ctx->actionFlag = 0;
                return;
            }
            nextGroup = ctx->digits[pos + 1].unk4;
            if (group != nextGroup) {
                ctx->action = 1;
                ctx->actionArg = nextGroup;
                ctx->actionFlag = 0;
                return;
            }
            ctx->action = 2;
            ctx->actionArg = pos + 1;
        } else if (index == 26) {
            sub_02085FFC(ctx);
        } else {
            sub_02085F80(ctx);
        }
    }
}

static void sub_02086328(UnkStruct_020850F4 *ctx) {
    switch (ctx->action) {
    case 0:
        break;
    case 1:
        sub_02085604(ctx, ctx->actionArg);
        sub_02086DA4(ctx);
        sub_02086DE4(ctx, TRUE);
        sub_02085808(ctx, 2);
        ctx->action = 0xFF;
        break;
    case 2:
        sub_02086AE4(ctx, ctx->actionArg);
        sub_02086384(ctx);
        break;
    case 0xFF:
        break;
    }
}

static void sub_02086384(UnkStruct_020850F4 *ctx) {
    ctx->action = 0;
    ctx->actionArg = 0;
    ctx->actionFlag = 0;
}

static int sub_02086398(UnkStruct_020850F4 *ctx, int group) {
    int i;

    for (i = 0; i < ctx->numDigitSlots; i++) {
        if (group == ctx->digits[i].unk4) {
            return i;
        }
    }
    return 0;
}

static int sub_020863C0(UnkStruct_020850F4 *ctx, int group) {
    int i;
    int found = 0;

    for (i = 0; i < ctx->numDigitSlots; i++) {
        if (group == ctx->digits[i].unk4) {
            found = 1;
        } else if (found == 1) {
            return i - 1;
        }
    }
    return ctx->numDigitSlots - 1;
}
