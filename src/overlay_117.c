#include "global.h"

#include "bg_window.h"
#include "brightness.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "screen_fade.h"
#include "sprite.h"
#include "string.h"
#include "sys_task.h"
#include "text.h"

typedef struct Ov117Unk10Sub {
    u8 unk00[0x1C];
    void *hblankSystem; // 0x1C
} Ov117Unk10Sub;

typedef struct Ov117Unk10 {
    u8 unk0[4];
    Ov117Unk10Sub *unk4; // 0x4
    BgConfig *bgConfig;  // 0x8
} Ov117Unk10;

typedef struct Ov117Work {
    int state;         // 0x00
    int unk04;         // 0x04
    u8 unk08[4];       // 0x08
    void *data;        // 0x0C
    Ov117Unk10 *unk10; // 0x10
    int *doneFlag;     // 0x14
    u8 unk18[8];       // 0x18
    NARC *narc;        // 0x20
} Ov117Work;

typedef struct Ov117SpriteMgr {
    SpriteList *spriteList; // 0x000
    u8 unk004[0x138];
} Ov117SpriteMgr; // size: 0x13C

typedef struct Ov117SpriteRes {
    u8 unk00[0x34];
} Ov117SpriteRes;

typedef struct Ov117AccelAnim {
    fx32 cur; // 0x00
    u8 unk04[0x14];
} Ov117AccelAnim; // size: 0x18

typedef struct Ov117LinearAnim {
    int cur; // 0x00
    u8 unk04[0x10];
} Ov117LinearAnim; // size: 0x14

typedef struct Ov117FlyData {
    Ov117SpriteMgr mgr;      // 0x000
    Ov117SpriteRes res;      // 0x13C
    Sprite *sprites[6];      // 0x170
    Ov117AccelAnim scale[6]; // 0x188
    Ov117AccelAnim x[6];     // 0x218
    Ov117AccelAnim y[6];     // 0x2A8
    Ov117LinearAnim rot[6];  // 0x338
    BOOL active[6];          // 0x3B0
    int index;               // 0x3C8
    int timer;               // 0x3CC
} Ov117FlyData;              // size: 0x3D0

typedef struct Ov117Unk18C {
    u8 unk0[0xC];
} Ov117Unk18C;

typedef struct Ov117BannerData {
    Sprite *sprite;      // 0x000
    Ov117SpriteMgr mgr;  // 0x004
    Ov117SpriteRes res;  // 0x140
    Ov117AccelAnim anim; // 0x174
    Ov117Unk18C unk18C;  // 0x18C
    Window window;       // 0x198
    VecFx32 pos;         // 0x1A8
    int counter;         // 0x1B4
    BOOL unk1B8;         // 0x1B8
} Ov117BannerData;       // size: 0x1BC

typedef struct Ov117BannerTemplate {
    u8 res[4];     // 0x0
    u32 trainerId; // 0x4
} Ov117BannerTemplate;

typedef struct Ov117FlyParam {
    fx32 x0;   // 0x00
    fx32 x1;   // 0x04
    fx32 xv;   // 0x08
    fx32 y0;   // 0x0C
    fx32 y1;   // 0x10
    fx32 yv;   // 0x14
    int delay; // 0x18
    int rot;   // 0x1C
} Ov117FlyParam;

void ov01_021EFCDC(Ov117Work *work, SysTask *task);
void ov01_021EFCF8(int a0, int a1, int a2, int *a3, int a4);
void ov01_021EFE34(Ov117LinearAnim *anim, int start, int end, int frames);
BOOL ov01_021EFE44(Ov117LinearAnim *anim);
void ov01_021EFEC8(Ov117AccelAnim *anim, fx32 start, fx32 end, fx32 v0, int frames);
BOOL ov01_021EFF28(Ov117AccelAnim *anim);
void ov01_021F0454(NARC *narc, int a1, int a2, int a3, int a4, int a5, BgConfig *bgConfig, int bgId);
void ov01_021F05C4(Ov117SpriteMgr *mgr, int count, enum HeapID heapID);
void ov01_021F05F4(Ov117SpriteMgr *mgr);
void ov01_021F0614(NARC *narc, Ov117SpriteMgr *mgr, Ov117SpriteRes *res, int a3, int a4, int a5, int a6, int a7, int a8);
void ov01_021F06EC(Ov117SpriteMgr *mgr, Ov117SpriteRes *res);
Sprite *ov01_021F0718(Ov117SpriteMgr *mgr, Ov117SpriteRes *res, fx32 x, fx32 y, fx32 z, int a5);
void ov01_021F074C(VecFx32 *vec, fx32 x, fx32 y, fx32 z);
void ov01_021F12B4(Ov117Unk18C *a0, enum HeapID heapID);
void ov01_021F12D0(Ov117Unk18C *a0);
void ov01_021F1310(Ov117Unk18C *a0, int a1);
void HBlankSystem_Start(void *hblankSystem);
void GfGfxLoader_GXLoadPalFromOpenNarc(NARC *narc, s32 memberNo, int location, int palSlotOffset, u32 szByte, u32 heapID);
void *GfGfxLoader_GetScrnDataFromOpenNarc(NARC *narc, s32 memberNo, BOOL isCompressed, NNSG2dScreenData **ppScrnData, enum HeapID heapID);
void HBlankSystem_Stop(void *hblankSystem);

void ov117_0225F020(SysTask *task, Ov117Work *work);
void ov117_0225F4D4(SysTask *task, Ov117Work *work);
void ov117_0225F4E4(SysTask *task, Ov117Work *work);
void ov117_0225F4F4(SysTask *task, Ov117Work *work);
void ov117_0225F504(SysTask *task, Ov117Work *work);
void ov117_0225F514(SysTask *task, Ov117Work *work);

static void ov117_0225F420(Ov117Work *work, u8 bgId, int fileId);
static String *ov117_0225F470(u32 trainerId, enum HeapID heapID);
static void ov117_0225F524(SysTask *task, Ov117Work *work, const Ov117BannerTemplate *tmpl);

static const Ov117BannerTemplate ov117_0225FAEC = {
    { 0xEE, 0xEF, 0xF0, 0xF1 },
    0x2BC
};

static const Ov117BannerTemplate ov117_0225FAE4 = {
    { 0xEA, 0xEB, 0xEC, 0xED },
    0x1DF
};

static const Ov117BannerTemplate ov117_0225FADC = {
    { 0xE6, 0xE7, 0xE8, 0xE9 },
    0x1E5
};

static const Ov117BannerTemplate ov117_0225FAD4 = {
    { 0xE2, 0xE3, 0xE4, 0xE5 },
    0x1E6
};

static const Ov117BannerTemplate ov117_0225FACC = {
    { 0xDE, 0xDF, 0xE0, 0xE1 },
    0x1E8
};

static const Ov117FlyParam ov117_0225FAF4[6] = {
    { 0x104000,   0x80000, -0x1E000, 0,          0x64000, 0x14000,  4, 0x1FFFE    },
    { 0xFFFF0000, 0x80000, 0x1E000,  0xA0000,    0x64000, -0x14000, 3, 0xFFFF     },
    { 0,          0x80000, 0x1E000,  0xFFFF0000, 0x64000, 0x14000,  4, 0xFFFD0003 },
    { 0x8C000,    0x80000, -0xA000,  0xA0000,    0x64000, -0x14000, 2, 0xFFFE0002 },
    { 0x104000,   0x80000, -0x1E000, 0x50000,    0x64000, 0x1000,   3, 0xFFFD0003 },
    { 0,          0x80000, 0x1E000,  0xA0000,    0x64000, -0x14000, 3, 0xFFFF     },
};

void ov117_0225F020(SysTask *task, Ov117Work *work) {
    VecFx32 pos;
    VecFx32 scale;
    BOOL done;
    int i;
    Ov117FlyData *data = work->data;

    switch (work->state) {
    case 0:
        work->data = Heap_Alloc(HEAP_ID_FIELD1, sizeof(Ov117FlyData));
        memset(work->data, 0, sizeof(Ov117FlyData));
        data = work->data;
        ov01_021F05C4(&data->mgr, 6, HEAP_ID_1);
        ov01_021F0614(work->narc, &data->mgr, &data->res, 3, 1, 0x9C, 0x9E, 0x9D, 0x927C0);
        for (i = 0; i < 6; i++) {
            data->sprites[i] = ov01_021F0718(&data->mgr, &data->res, 0, 0, 0, 0);
            Sprite_SetDrawFlag(data->sprites[i], FALSE);
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        work->state++;
        break;
    case 1:
        ov01_021EFCF8(1, -0x10, -0x10, &work->unk04, 2);
        work->state++;
        break;
    case 2:
        if (work->unk04 != 0) {
            work->state++;
            data->index = 0;
            data->timer = ov117_0225FAF4[data->index].delay;
        }
        break;
    case 3:
        data->timer--;
        if (data->timer < 0) {
            ov01_021EFEC8(&data->x[data->index], ov117_0225FAF4[data->index].x0, ov117_0225FAF4[data->index].x1, ov117_0225FAF4[data->index].xv, 8);
            ov01_021EFEC8(&data->y[data->index], ov117_0225FAF4[data->index].y0, ov117_0225FAF4[data->index].y1, ov117_0225FAF4[data->index].yv, 8);
            ov01_021EFEC8(&data->scale[data->index], 0x2000, 0x29, -0x666, 8);
            ov01_021EFE34(&data->rot[data->index], 0, ov117_0225FAF4[data->index].rot, 8);
            Sprite_SetDrawFlag(data->sprites[data->index], TRUE);
            ov01_021F074C(&pos, ov117_0225FAF4[data->index].x0, ov117_0225FAF4[data->index].y0, 0);
            Sprite_SetMatrix(data->sprites[data->index], &pos);
            ov01_021F074C(&scale, 0x2000, 0x2000, 0);
            Sprite_SetScaleAndAffineType(data->sprites[data->index], &scale, 2);
            data->active[data->index] = TRUE;
            data->index++;
            if (data->index >= 6) {
                work->state++;
            } else {
                data->timer = ov117_0225FAF4[data->index].delay;
            }
        }
        break;
    case 4:
        if (data->active[5] == FALSE) {
            work->state++;
        }
        break;
    case 5:
        HBlankSystem_Stop(work->unk10->unk4->hblankSystem);
        BeginNormalPaletteFade(FADE_MAIN_ONLY, (enum FadeType)0x22, FADE_TYPE_BRIGHTNESS_OUT, 0, 12, 1, HEAP_ID_FIELD1);
        work->state++;
        break;
    case 6:
        if (IsPaletteFadeFinished()) {
            work->state++;
        }
        break;
    case 7:
        sub_0200FBF4(PM_LCD_BOTTOM, 0);
        HBlankSystem_Start(work->unk10->unk4->hblankSystem);
        if (work->doneFlag != NULL) {
            *work->doneFlag = TRUE;
        }
        for (i = 0; i < 6; i++) {
            Sprite_Delete(data->sprites[i]);
        }
        ov01_021F06EC(&data->mgr, &data->res);
        ov01_021F05F4(&data->mgr);
        ov01_021EFCDC(work, task);
        break;
    }

    for (i = 0; i < 6; i++) {
        if (data->active[i] == TRUE) {
            done = ov01_021EFF28(&data->x[i]);
            ov01_021EFF28(&data->y[i]);
            ov01_021EFF28(&data->scale[i]);
            ov01_021EFE44(&data->rot[i]);
            if (done) {
                data->active[i] = FALSE;
                Sprite_SetDrawFlag(data->sprites[i], FALSE);
            }
            ov01_021F074C(&pos, data->x[i].cur, data->y[i].cur, 0);
            Sprite_SetMatrix(data->sprites[i], &pos);
            ov01_021F074C(&scale, data->scale[i].cur, data->scale[i].cur, 0);
            Sprite_SetAffineScale(data->sprites[i], &scale);
            Sprite_SetAffineZRotation(data->sprites[i], (u16)data->rot[i].cur);
        }
    }

    if (work->state != 7) {
        SpriteList_RenderAndAnimateSprites(data->mgr.spriteList);
    }
}

static void ov117_0225F420(Ov117Work *work, u8 bgId, int fileId) {
    NNSG2dScreenData *scrnData;
    void *buffer = GfGfxLoader_GetScrnDataFromOpenNarc(work->narc, fileId, FALSE, &scrnData, HEAP_ID_FIELD1);

    LoadRectToBgTilemapRect(work->unk10->bgConfig, bgId, scrnData->rawData, 0, 0, scrnData->screenWidth / 8, scrnData->screenHeight / 8);
    Heap_Free(buffer);
    BgCommitTilemapBufferToVram(work->unk10->bgConfig, bgId);
}

static String *ov117_0225F470(u32 trainerId, enum HeapID heapID) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xBD, heapID);
    MessageFormat *msgFmt = MessageFormat_New(heapID);
    String *str = String_New(0x80, heapID);
    String *tmpl = String_New(0x80, heapID);

    ReadMsgDataIntoString(msgData, 0, tmpl);
    BufferTrainerName(msgFmt, 0, trainerId);
    StringExpandPlaceholders(msgFmt, str, tmpl);
    DestroyMsgData(msgData);
    MessageFormat_Delete(msgFmt);
    String_Delete(tmpl);
    return str;
}

void ov117_0225F4D4(SysTask *task, Ov117Work *work) {
    ov117_0225F524(task, work, &ov117_0225FACC);
}

void ov117_0225F4E4(SysTask *task, Ov117Work *work) {
    ov117_0225F524(task, work, &ov117_0225FAD4);
}

void ov117_0225F4F4(SysTask *task, Ov117Work *work) {
    ov117_0225F524(task, work, &ov117_0225FADC);
}

void ov117_0225F504(SysTask *task, Ov117Work *work) {
    ov117_0225F524(task, work, &ov117_0225FAE4);
}

void ov117_0225F514(SysTask *task, Ov117Work *work) {
    ov117_0225F524(task, work, &ov117_0225FAEC);
}

static void ov117_0225F524(SysTask *task, Ov117Work *work, const Ov117BannerTemplate *tmpl) {
    String *str;
    BOOL done;
    int pos;
    enum HeapID heapID = HEAP_ID_FIELD1;
    Ov117BannerData *data = work->data;

    switch (work->state) {
    case 0:
        work->data = Heap_Alloc(heapID, sizeof(Ov117BannerData));
        memset(work->data, 0, sizeof(Ov117BannerData));
        data = work->data;
        ov01_021F05C4(&data->mgr, 1, HEAP_ID_1);
        ov01_021F0614(work->narc, &data->mgr, &data->res, tmpl->res[0], 1, tmpl->res[1], tmpl->res[2], tmpl->res[3], 0x927C0);
        data->sprite = ov01_021F0718(&data->mgr, &data->res, 0x140000, 0x80000, 0, 0);
        Sprite_SetDrawFlag(data->sprite, FALSE);
        Sprite_SetOamMode(data->sprite, GX_OAM_MODE_XLU);
        Sprite_SetPriority(data->sprite, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        data->pos.x = 0x140000;
        data->pos.y = 0x80000;
        data->pos.z = 0;
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 12, 4);
        SetBgPriority(GF_BG_LYR_MAIN_2, 0);
        SetBgPriority(GF_BG_LYR_MAIN_3, 1);
        SetBgPriority(GF_BG_LYR_MAIN_1, 2);
        SetBgPriority(GF_BG_LYR_MAIN_0, 3);
        ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_OFF);
        ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_OFF);
        ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_OFF);
        ov01_021F0454(work->narc, 0xD9, 0xD8, 0xD7, 0, 2, work->unk10->bgConfig, 1);
        GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 0x10, 0, 0x40, 0x20, heapID);
        AddWindowParameterized(work->unk10->bgConfig, &data->window, GF_BG_LYR_MAIN_2, 0, 0x14, 0x10, 2, 2, 1);
        FillWindowPixelBuffer(&data->window, 0);
        str = ov117_0225F470(tmpl->trainerId, heapID);
        AddTextPrinterParameterizedWithColor(&data->window, 0, str, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(str);
        ov01_021F0454(work->narc, 0xDA, 0xD8, 0xD7, 0, 2, work->unk10->bgConfig, 3);
        G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG3, TRUE);
        G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_OBJ, TRUE);
        G2_SetWnd0Position(0, 0x60, 0xFF, 0x60);
        ov01_021F12B4(&data->unk18C, heapID);
        ov01_021EFCF8(1, 0x10, -0x10, &work->unk04, 1);
        work->state++;
        break;
    case 1:
        ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_ON);
        data->counter = 12;
        work->state++;
    case 2:
        BgSetPosTextAndCommit(work->unk10->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_ADD_X, 0x18);
        if (--data->counter <= 0) {
            work->state++;
        }
        break;
    case 3:
        GX_SetVisibleWnd(GX_WNDMASK_W0);
        ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_ON);
        ov01_021EFEC8(&data->anim, 0, 0x60000, 0x4000, 4);
        work->state++;
    case 4:
        BgSetPosTextAndCommit(work->unk10->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_ADD_X, 0x18);
        if (!ov01_021EFF28(&data->anim)) {
            pos = data->anim.cur >> FX32_SHIFT;
            G2_SetWnd0Position(0, 0x60 - pos, 0xFF, 0x60 + pos);
        } else {
            GX_SetVisibleWnd(GX_WNDMASK_NONE);
            BgSetPosTextAndCommit(work->unk10->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
            data->counter = 13;
            work->state++;
        }
        break;
    case 5:
        if (--data->counter <= 0) {
            work->state++;
        }
        break;
    case 6:
        ov117_0225F420(work, GF_BG_LYR_MAIN_1, 0xDD);
        SetBgPriority(GF_BG_LYR_MAIN_1, 1);
        SetBgPriority(GF_BG_LYR_MAIN_3, 2);
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 0, 16);
        Sprite_SetDrawFlag(data->sprite, TRUE);
        data->counter = 0;
        work->state++;
    case 7:
        done = FALSE;
        if (data->counter <= 16) {
            data->counter += 4;
            if (data->counter > 16) {
                data->counter = 16;
                done = TRUE;
            }
            G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, data->counter, 16 - data->counter);
        }
        data->pos.x += (0x80000 - data->pos.x) * 2 / 3;
        if (data->pos.x >> FX32_SHIFT <= 0x82) {
            data->pos.x = 0x80000;
            if (done == TRUE) {
                StartBrightnessTransition(8, 0, 16, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ), SCREEN_MASK_MAIN);
                BG_SetMaskColor(GF_BG_LYR_MAIN_0, 0x14A5);
                ToggleBgLayer(GF_BG_LYR_MAIN_0, GF_PLANE_TOGGLE_OFF);
                work->state++;
            }
        }
        if (data->counter > 4) {
            ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_ON);
        }
        ScheduleSetBgPosText(work->unk10->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, -((data->pos.x >> FX32_SHIFT) + 8));
        Sprite_SetMatrix(data->sprite, &data->pos);
        break;
    case 8:
        if (IsBrightnessTransitionActive(1)) {
            data->counter = 16;
            work->state++;
        }
        break;
    case 9:
        if (--data->counter <= 0) {
            ov01_021EFEC8(&data->anim, data->pos.x, -0x60000, 0x2000, 8);
            data->counter = 8;
            ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_OFF);
            work->state++;
        }
        break;
    case 10:
        data->pos.x = data->anim.cur;
        pos = -((data->pos.x >> FX32_SHIFT) + 8);
        if (pos < 0) {
            ScheduleSetBgPosText(work->unk10->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, pos);
        } else {
            ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_OFF);
        }
        Sprite_SetMatrix(data->sprite, &data->pos);
        if (ov01_021EFF28(&data->anim)) {
            GXS_DispOff();
            BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 4, 1, HEAP_ID_FIELD1);
            work->state++;
        }
        break;
    case 11:
        if (IsPaletteFadeFinished()) {
            work->state++;
        }
        break;
    case 12:
        if (work->doneFlag != NULL) {
            *work->doneFlag = TRUE;
        }
        ov01_021F12D0(&data->unk18C);
        data->unk1B8 = FALSE;
        Sprite_Delete(data->sprite);
        ov01_021F06EC(&data->mgr, &data->res);
        ov01_021F05F4(&data->mgr);
        RemoveWindow(&data->window);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, HEAP_ID_FIELD1);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_FIELD1);
        BgClearTilemapBufferAndCommit(work->unk10->bgConfig, GF_BG_LYR_MAIN_3);
        BgClearTilemapBufferAndCommit(work->unk10->bgConfig, GF_BG_LYR_MAIN_1);
        ov01_021EFCDC(work, task);
        break;
    }

    if (data->unk1B8 == TRUE) {
        ov01_021F1310(&data->unk18C, 2);
    }
    if (work->state != 12) {
        SpriteList_RenderAndAnimateSprites(data->mgr.spriteList);
    }
}
