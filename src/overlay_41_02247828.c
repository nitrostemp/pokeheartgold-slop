#include "global.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "brightness.h"
#include "fashion_case.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_41_02248400.h"
#include "player_data.h"
#include "pokemon.h"
#include "pokepic.h"
#include "render_window.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "text.h"
#include "touchscreen.h"
#include "yes_no_prompt.h"

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

typedef struct UnkOv41CanvasTemplate {
    void *unk00;                    // 0x00
    void **unk04;                   // 0x04
    void **unk08;                   // 0x08
    u8 *unk0C;                      // 0x0C
    PokepicManager *pokepicManager; // 0x10
    BgConfig *bgConfig;             // 0x14
    void *unk18;                    // 0x18
    UnkOv41Pool *pool;              // 0x1C
    int maxObjs;                    // 0x20
} UnkOv41CanvasTemplate;

typedef struct UnkOv41Canvas {
    UnkOv41Pool *pool;              // 0x00
    UnkOv41Node backList;           // 0x04
    UnkOv41Node frontList;          // 0x14
    int objCount;                   // 0x24
    int maxObjs;                    // 0x28
    void *unk2C;                    // 0x2C
    void **unk30;                   // 0x30
    void **unk34;                   // 0x34
    u8 *unk38;                      // 0x38
    PokepicManager *pokepicManager; // 0x3C
    BgConfig *bgConfig;             // 0x40
    void *unk44;                    // 0x44
    u8 scroll[0x2C];                // 0x48
    int background;                 // 0x74
    UnkOv41MonPic monPic;           // 0x78
} UnkOv41Canvas;                    // size: 0x88

typedef struct UnkOv41App {
    u8 unk000[0x40];
    BgConfig *bgConfig; // 0x040
    u8 unk044[0x324];
    UnkOv41Board board;   // 0x368
    UnkOv41Canvas canvas; // 0x3F4
    u8 unk47C[0x64];
    u8 unk4E0[0x88];          // 0x4E0
    u8 unk568[0x150];         // 0x568
    YesNoPrompt *yesNoPrompt; // 0x6B8
    Window *window;           // 0x6BC
    u8 unk6C0[0x1C];
    Options *options; // 0x6DC
    u8 unk6E0[0x8];
    MessageFormat *msgFormat; // 0x6E8
    int menuInputState;       // 0x6EC
} UnkOv41App;

typedef struct UnkOv41FadeInTask {
    UnkOv41App *app; // 0x0
    int *done;       // 0x4
    int counter;     // 0x8
    int state;       // 0xC
} UnkOv41FadeInTask;

void *ov41_02245EE0(UnkOv41ObjTemplate *tmpl);
void ov41_02246014(void *obj, int priority);
void ov41_02247414(UnkOv41App *app);
void ov41_02247480(UnkOv41App *app, int a1);
void ov41_02247588(UnkOv41App *app);
void ov41_022495F0(UnkOv41MonPic *pic, PokepicManager *mgr, Pokemon *mon, PokepicTemplate *tmpl, enum HeapID heapID);
void ov41_02249604(UnkOv41MonPic *pic, PokepicManager *mgr, Pokemon *mon, PokepicTemplate *tmpl, enum HeapID heapID, BOOL a5);
void ov41_022496E8(UnkOv41MonPic *pic);
void ov41_02249700(UnkOv41MonPic *pic, int val);
void ov41_0224971C(UnkOv41MonPic *pic, int x, int y);
BOOL ov41_02249820(UnkOv41MonPic *pic, u32 x, u32 y, void *a3);
UnkOv41Node *ov41_022499F0(UnkOv41Pool *pool, void *obj, int type);
void ov41_02249A50(UnkOv41Node *node, UnkOv41Node *where);
void ov41_02249A60(UnkOv41Node *node);
void ov41_02249A70(UnkOv41Node *node);
BOOL ov41_02249AA8(UnkOv41Node *node, int a1, int a2, int a3);
void ov41_02249B44(UnkOv41Node *node, int *x, int *y);
void ov41_02249B94(UnkOv41Node *node, int *w, int *h);
void ov41_02249BAC(UnkOv41Node *node, int *m0, int *m1, int *m2, int *m3);
void ov41_02249BE8(UnkOv41Node *head, int dx, int dy);
void ov41_02249C7C(void *scroll, UnkOv41BgScrollTemplate *tmpl);
void ov41_02249CC4(void *scroll);
void ov41_0224A5A4(void *a0, int a1, int a2);
void ov41_0224AC98(void *a0, int a1);
void sub_0202BC38(SaveFashionDataSub *sub);
void sub_0202BC60(SaveFashionDataSub *sub);
void sub_0202BC88(SaveFashionDataSub *sub, Pokemon *pokemon, UnkOv41MonPic *pic);
void sub_0202BCAC(SaveFashionDataSub *sub, void *a1, int a2);
void sub_0202BD60(SaveFashionDataSub *sub, int a1);
void sub_0202BDC8(SaveFashionDataSub *sub, const String *str, u32 gender);

void ov41_02247828(UnkOv41App *app, int *done);
static void ov41_02247850(SysTask *task, void *data);
void ov41_022479A8(SaveFashionDataSub *sub, UnkOv41Canvas *canvas, PlayerProfile *profile);
BOOL ov41_02247A48(UnkOv41FadeInTask *task, int dx, int dy, int frames);
void ov41_02247AB4(UnkOv41App *app);
static void ov41_02247B5C(UnkOv41App *app);
int ov41_02247B7C(UnkOv41App *app);
static void ov41_02247BB8(UnkOv41App *app, int msgId, u8 x, u8 y, u8 width, u8 height);
static void ov41_02247C7C(UnkOv41App *app, int msgId);
static void ov41_02247D00(UnkOv41App *app);
static void ov41_02247D1C(UnkOv41App *app, int msgId);
static void ov41_02247D34(UnkOv41App *app, int msgId);
static void ov41_02247D3C(UnkOv41App *app);
void ov41_02247D44(UnkOv41App *app);
void ov41_02247D64(UnkOv41App *app);
int ov41_02247DF8(UnkOv41App *app);
static void ov41_02247E34(u32 top, u32 bottom, u32 left, u32 right, int idx, int *outX, int *outY);
void ov41_02247F3C(UnkOv41Canvas *canvas, const UnkOv41CanvasTemplate *tmpl);
void ov41_02247F90(UnkOv41Canvas *canvas);
void ov41_02247FAC(UnkOv41Canvas *canvas, Pokemon *mon, PokepicTemplate *tmpl, int x, int y, int priority, enum HeapID heapID);
void ov41_02247FE0(UnkOv41Canvas *canvas, Pokemon *mon, PokepicTemplate *tmpl, enum HeapID heapID);
void ov41_02247FFC(UnkOv41Canvas *canvas, Pokemon *mon, PokepicTemplate *tmpl, enum HeapID heapID);
void ov41_02248020(UnkOv41Canvas *canvas, UnkOv41Node *node);
void ov41_02248030(UnkOv41Node *node);
static void ov41_02248038(UnkOv41Canvas *canvas);
BOOL ov41_02248044(UnkOv41Canvas *canvas, int id, int x, int y, int priority);
BOOL ov41_022480A4(UnkOv41Canvas *canvas, UnkOv41Node *node, void *a2);
void ov41_022480C8(UnkOv41Canvas *canvas, UnkOv41Node *node);
void ov41_022480E0(UnkOv41Canvas *canvas);
static void ov41_022480F8(UnkOv41Canvas *canvas, int dx, int dy);
void ov41_02248114(UnkOv41Canvas *canvas, int dx, int dy);
void ov41_02248120(UnkOv41Canvas *canvas, int x0, int y0, int x1, int y1);
void ov41_02248158(UnkOv41Canvas *canvas);
static void ov41_02248164(UnkOv41Canvas *canvas, int priority);
BOOL ov41_022481BC(UnkOv41Canvas *canvas);
BOOL ov41_022481D8(UnkOv41Canvas *canvas, int x, int y);
UnkOv41Node *ov41_022481F4(UnkOv41Canvas *canvas, int a1, int a2, int a3);
void ov41_0224825C(UnkOv41Canvas *canvas, int background, int a2);
void ov41_022482A8(UnkOv41Canvas *canvas);
static int ov41_022482B4(UnkOv41Canvas *canvas);
void ov41_022482B8(UnkOv41Canvas *canvas, int *outX, int *outY);
static void ov41_02248324(UnkOv41Canvas *canvas, UnkOv41Node *node, BOOL toFront, void *a3);

void ov41_02247828(UnkOv41App *app, int *done) {
    UnkOv41FadeInTask *task = SysTask_GetData(CreateSysTaskAndEnvironment(ov41_02247850, sizeof(UnkOv41FadeInTask), 10, HEAP_ID_13));

    task->app = app;
    task->done = done;
    task->counter = 0;
    task->state = 0;
}

static void ov41_02247850(SysTask *task, void *data) {
    UnkOv41FadeInTask *env = data;

    switch (env->state) {
    case 0:
        StartBrightnessTransition(8, -16, 0, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
        env->state++;
        break;
    case 1:
        if (IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
            env->state++;
        }
        break;
    case 2:
        ov41_02247B5C(env->app);
        ov41_02247414(env->app);
        ov41_02247588(env->app);
        ov41_02247480(env->app, 0);
        ScheduleSetBgPosText(env->app->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, -40);
        env->state++;
        break;
    case 3:
        if (ov41_02247A48(env, 8, -5, 8)) {
            env->counter = 0;
            env->state++;
        }
        break;
    case 4:
        ov41_0224A5A4(env->app->unk4E0, 0, -8);
        env->counter++;
        if (env->counter >= 8) {
            env->counter = 0;
            env->state++;
        }
        break;
    case 5:
        StartBrightnessTransition(8, 0, -16, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
        env->state++;
        break;
    case 6:
        if (IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
            env->state++;
        }
        break;
    case 7:
        GF_ASSERT(ov41_02248750(&env->app->board, 0, 0));
        env->state++;
        break;
    case 8:
        if (ov41_02248998(&env->app->board)) {
            env->state++;
        }
        break;
    case 9:
        *env->done = TRUE;
        DestroySysTaskAndEnvironment(task);
        break;
    }
}

void ov41_022479A8(SaveFashionDataSub *sub, UnkOv41Canvas *canvas, PlayerProfile *profile) {
    UnkOv41Node *node;
    int i;

    sub_0202BC60(sub);
    sub_0202BC88(sub, canvas->monPic.mon, &canvas->monPic);
    if (profile != NULL) {
        String *name = PlayerProfile_GetPlayerName_NewString(profile, HEAP_ID_13);
        sub_0202BDC8(sub, name, PlayerProfile_GetTrainerGender(profile));
        String_Delete(name);
    }
    i = 0;
    for (node = canvas->frontList.next; node != &canvas->frontList; node = node->next) {
        if (node->type == 0) {
            sub_0202BCAC(sub, node->obj, i);
            i++;
        }
    }
    for (node = canvas->backList.next; node != &canvas->backList; node = node->next) {
        if (node->type == 0) {
            sub_0202BCAC(sub, node->obj, i);
            i++;
        }
    }
    sub_0202BD60(sub, (u8)canvas->background);
    sub_0202BC38(sub);
}

BOOL ov41_02247A48(UnkOv41FadeInTask *task, int dx, int dy, int frames) {
    if (task->counter < 8) {
        ov41_022480F8(&task->app->canvas, dx, dy);
    }
    if (task->counter >= 1) {
        ScheduleSetBgPosText(task->app->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SUB_X, dx);
        ScheduleSetBgPosText(task->app->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SUB_X, dx);
        ScheduleSetBgPosText(task->app->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SUB_Y, dy);
        ScheduleSetBgPosText(task->app->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SUB_Y, dy);
    }
    task->counter++;
    if (task->counter > frames) {
        return TRUE;
    }
    return FALSE;
}

void ov41_02247AB4(UnkOv41App *app) {
    YesNoPromptTemplate template;

    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    MI_CpuFill8(&template, 0, sizeof(YesNoPromptTemplate));
    template.bgConfig = app->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_3;
    template.tileStart = 0;
    template.plttSlot = 5;
    template.x = 25;
    template.y = 4;
    template.ignoreTouchFlag = app->menuInputState;
    YesNoPrompt_InitFromTemplate(app->yesNoPrompt, &template);
    ov41_02247D1C(app, 1);
    G2_SetBG0Priority(2);
    G2_SetBG1Priority(1);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(0);
}

static void ov41_02247B5C(UnkOv41App *app) {
    YesNoPrompt_Reset(app->yesNoPrompt);
    ov41_02247D3C(app);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3);
}

int ov41_02247B7C(UnkOv41App *app) {
    int ret = YesNoPrompt_HandleInput(app->yesNoPrompt);

    switch (ret) {
    case YESNORESPONSE_WAIT:
        return 4;
    case YESNORESPONSE_YES:
        ret = 8;
        break;
    case YESNORESPONSE_NO:
        ret = 9;
        break;
    }
    app->menuInputState = YesNoPrompt_IsInTouchMode(app->yesNoPrompt);
    return ret;
}

static void ov41_02247BB8(UnkOv41App *app, int msgId, u8 x, u8 y, u8 width, u8 height) {
    u32 frame = Options_GetFrame(app->options);
    MsgData *msgData;
    String *string;

    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_7_OFFSET, HEAP_ID_14);
    AddWindowParameterized(app->bgConfig, app->window, GF_BG_LYR_MAIN_3, x, y, width, height, 7, 0x5A);
    FillWindowPixelBuffer(app->window, 15);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_MAIN_3, 0x3C, 8, frame, HEAP_ID_14);
    DrawFrameAndWindow2(app->window, FALSE, 0x3C, 8);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0215_bin, HEAP_ID_13);
    string = NewString_ReadMsgData(msgData, msgId);
    AddTextPrinterParameterizedWithColor(app->window, 1, string, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(app->window);
}

static void ov41_02247C7C(UnkOv41App *app, int msgId) {
    MsgData *msgData;
    String *src;
    String *dest;

    FillWindowPixelBuffer(app->window, 15);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0215_bin, HEAP_ID_13);
    src = NewString_ReadMsgData(msgData, msgId);
    dest = String_New(0x100, HEAP_ID_13);
    StringExpandPlaceholders(app->msgFormat, dest, src);
    AddTextPrinterParameterizedWithColor(app->window, 1, dest, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    String_Delete(dest);
    String_Delete(src);
    DestroyMsgData(msgData);
    CopyWindowToVram(app->window);
}

static void ov41_02247D00(UnkOv41App *app) {
    ClearWindowTilemapAndCopyToVram(app->window);
    RemoveWindow(app->window);
}

static void ov41_02247D1C(UnkOv41App *app, int msgId) {
    ov41_02247BB8(app, msgId, 2, 1, 27, 2);
}

static void ov41_02247D34(UnkOv41App *app, int msgId) {
    ov41_02247C7C(app, msgId);
}

static void ov41_02247D3C(UnkOv41App *app) {
    ov41_02247D00(app);
}

void ov41_02247D44(UnkOv41App *app) {
    ov41_0224AC98(app->unk568, ov41_022482B4(&app->canvas));
}

void ov41_02247D64(UnkOv41App *app) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0, sizeof(YesNoPromptTemplate));
    template.bgConfig = app->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_3;
    template.tileStart = 0;
    template.plttSlot = 5;
    template.x = 25;
    template.y = 4;
    template.ignoreTouchFlag = app->menuInputState;
    YesNoPrompt_Reset(app->yesNoPrompt);
    YesNoPrompt_InitFromTemplate(app->yesNoPrompt, &template);
    ov41_02247D34(app, 2);
    G2_SetBG0Priority(2);
    G2_SetBG1Priority(1);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(0);
}

int ov41_02247DF8(UnkOv41App *app) {
    int ret = YesNoPrompt_HandleInput(app->yesNoPrompt);

    switch (ret) {
    case YESNORESPONSE_WAIT:
        return 5;
    case YESNORESPONSE_YES:
        ret = 6;
        break;
    case YESNORESPONSE_NO:
        ret = 7;
        break;
    }
    app->menuInputState = YesNoPrompt_IsInTouchMode(app->yesNoPrompt);
    return ret;
}

static void ov41_02247E34(u32 top, u32 bottom, u32 left, u32 right, int idx, int *outX, int *outY) {
    u32 xStep = (right - left) / 3;
    u32 x1 = left + xStep;
    u32 x2 = left + xStep * 2;
    u32 yStep = (bottom - top) / 3;
    u32 y1 = top + yStep;
    u32 y2 = top + yStep * 2;

    switch (idx) {
    case 0:
        *outX = left;
        *outY = top;
        break;
    case 1:
        *outX = left;
        *outY = y1;
        break;
    case 2:
        *outX = left;
        *outY = y2;
        break;
    case 3:
        *outX = left;
        *outY = bottom;
        break;
    case 4:
        *outX = x1;
        *outY = top;
        break;
    case 5:
        *outX = x1;
        *outY = y1;
        break;
    case 6:
        *outX = x1;
        *outY = y2;
        break;
    case 7:
        *outX = x1;
        *outY = bottom;
        break;
    case 8:
        *outX = x2;
        *outY = top;
        break;
    case 9:
        *outX = x2;
        *outY = y1;
        break;
    case 10:
        *outX = x2;
        *outY = y2;
        break;
    case 11:
        *outX = x2;
        *outY = bottom;
        break;
    case 12:
        *outX = right;
        *outY = top;
        break;
    case 13:
        *outX = right;
        *outY = y1;
        break;
    case 14:
        *outX = right;
        *outY = y2;
        break;
    case 15:
        *outX = right;
        *outY = bottom;
        break;
    }
}

void ov41_02247F3C(UnkOv41Canvas *canvas, const UnkOv41CanvasTemplate *tmpl) {
    canvas->unk2C = tmpl->unk00;
    canvas->unk30 = tmpl->unk04;
    canvas->unk34 = tmpl->unk08;
    canvas->unk38 = tmpl->unk0C;
    canvas->pokepicManager = tmpl->pokepicManager;
    canvas->bgConfig = tmpl->bgConfig;
    canvas->unk44 = tmpl->unk18;
    canvas->background = 0;
    canvas->pool = tmpl->pool;
    canvas->backList.next = &canvas->backList;
    canvas->backList.prev = &canvas->backList;
    canvas->frontList.next = &canvas->frontList;
    canvas->frontList.prev = &canvas->frontList;
    canvas->objCount = 0;
    canvas->maxObjs = tmpl->maxObjs;
    ov41_02249A50(ov41_022499F0(canvas->pool, &canvas->monPic, 3), &canvas->frontList);
}

void ov41_02247F90(UnkOv41Canvas *canvas) {
    ov41_02248038(canvas);
    ov41_022480E0(canvas);
    memset(canvas, 0, sizeof(UnkOv41Canvas));
}

void ov41_02247FAC(UnkOv41Canvas *canvas, Pokemon *mon, PokepicTemplate *tmpl, int x, int y, int priority, enum HeapID heapID) {
    ov41_022495F0(&canvas->monPic, canvas->pokepicManager, mon, tmpl, heapID);
    ov41_02249700(&canvas->monPic, priority);
    ov41_0224971C(&canvas->monPic, x, y);
}

void ov41_02247FE0(UnkOv41Canvas *canvas, Pokemon *mon, PokepicTemplate *tmpl, enum HeapID heapID) {
    ov41_022495F0(&canvas->monPic, canvas->pokepicManager, mon, tmpl, heapID);
}

void ov41_02247FFC(UnkOv41Canvas *canvas, Pokemon *mon, PokepicTemplate *tmpl, enum HeapID heapID) {
    ov41_02249604(&canvas->monPic, canvas->pokepicManager, mon, tmpl, heapID, TRUE);
}

void ov41_02248020(UnkOv41Canvas *canvas, UnkOv41Node *node) {
    ov41_02249A50(node, canvas->frontList.prev);
}

void ov41_02248030(UnkOv41Node *node) {
    ov41_02249A60(node);
}

static void ov41_02248038(UnkOv41Canvas *canvas) {
    ov41_022496E8(&canvas->monPic);
}

BOOL ov41_02248044(UnkOv41Canvas *canvas, int id, int x, int y, int priority) {
    UnkOv41ObjTemplate template;
    void *obj;

    if (canvas->objCount < canvas->maxObjs) {
        template.unk00 = canvas->unk44;
        template.unk04 = canvas->unk2C;
        template.unk08 = canvas->unk30[id];
        template.unk0C = *canvas->unk34;
        template.unk10 = x;
        template.unk14 = y;
        template.unk18 = id;
        template.unk1C = canvas->unk38[id];
        obj = ov41_02245EE0(&template);
        ov41_02246014(obj, priority);
        ov41_02249A50(ov41_022499F0(canvas->pool, obj, 0), &canvas->backList);
        canvas->objCount++;
        return TRUE;
    }
    return FALSE;
}

BOOL ov41_022480A4(UnkOv41Canvas *canvas, UnkOv41Node *node, void *a2) {
    if (canvas->objCount < canvas->maxObjs) {
        ov41_02248324(canvas, node, TRUE, a2);
        canvas->objCount++;
        return TRUE;
    }
    return FALSE;
}

void ov41_022480C8(UnkOv41Canvas *canvas, UnkOv41Node *node) {
    ov41_02249A60(node);
    canvas->objCount--;
    GF_ASSERT(canvas->objCount >= 0);
}

void ov41_022480E0(UnkOv41Canvas *canvas) {
    ov41_02249A70(&canvas->backList);
    ov41_02249A70(&canvas->frontList);
    canvas->objCount = 0;
}

static void ov41_022480F8(UnkOv41Canvas *canvas, int dx, int dy) {
    ov41_02249BE8(&canvas->backList, dx, dy);
    ov41_02249BE8(&canvas->frontList, dx, dy);
}

void ov41_02248114(UnkOv41Canvas *canvas, int dx, int dy) {
    ov41_02249BE8(&canvas->frontList, dx, dy);
}

void ov41_02248120(UnkOv41Canvas *canvas, int x0, int y0, int x1, int y1) {
    ov41_02249BE8(&canvas->backList, -x0, -y0);
    ov41_02249BE8(&canvas->backList, x1, y1);
    ov41_02249BE8(&canvas->frontList, -x0, -y0);
    ov41_02249BE8(&canvas->frontList, x1, y1);
}

void ov41_02248158(UnkOv41Canvas *canvas) {
    ov41_02248164(canvas, -1);
}

static void ov41_02248164(UnkOv41Canvas *canvas, int priority) {
    UnkOv41Node *node;

    for (node = canvas->frontList.next; node != &canvas->frontList; node = node->next) {
        if (node->type == 0) {
            ov41_02246014(node->obj, priority);
        } else {
            ov41_02249700(node->obj, priority);
        }
        priority--;
    }
    priority -= 8;
    for (node = canvas->backList.next; node != &canvas->backList; node = node->next) {
        if (node->type == 0) {
            ov41_02246014(node->obj, priority);
        } else {
            ov41_02249700(node->obj, priority);
        }
        priority--;
    }
}

BOOL ov41_022481BC(UnkOv41Canvas *canvas) {
    TouchscreenHitbox hitbox;

    hitbox.rect.top = 18;
    hitbox.rect.bottom = 143;
    hitbox.rect.left = 138;
    hitbox.rect.right = 246;
    return TouchscreenHitbox_TouchHeldIsIn(&hitbox);
}

BOOL ov41_022481D8(UnkOv41Canvas *canvas, int x, int y) {
    TouchscreenHitbox hitbox;

    hitbox.rect.top = 18;
    hitbox.rect.bottom = 143;
    hitbox.rect.left = 138;
    hitbox.rect.right = 246;
    return TouchscreenHitbox_PointIsIn(&hitbox, x, y);
}

UnkOv41Node *ov41_022481F4(UnkOv41Canvas *canvas, int a1, int a2, int a3) {
    UnkOv41Node *node;

    for (node = canvas->frontList.next; node != &canvas->frontList; node = node->next) {
        if (ov41_02249AA8(node, a1, a2, a3) == TRUE) {
            return node;
        }
    }
    for (node = canvas->backList.next; node != &canvas->backList; node = node->next) {
        if (ov41_02249AA8(node, a1, a2, a3) == TRUE) {
            return node;
        }
    }
    return NULL;
}

void ov41_0224825C(UnkOv41Canvas *canvas, int background, int a2) {
    UnkOv41BgScrollTemplate template;

    template.bgConfig = canvas->bgConfig;
    template.unk04 = 26;
    template.unk08 = background * 4 + 0x86;
    template.unk0C = background * 4 + 0x87;
    template.unk10 = background * 4 + 0x88;
    template.unk14 = 0x88;
    template.unk18 = 16;
    template.unk1C = 2;
    template.unk20 = 1;
    template.unk24 = HEAP_ID_13;
    template.unk28 = 0;
    template.unk2C = a2;
    ov41_02249C7C(canvas->scroll, &template);
    canvas->background = background;
}

void ov41_022482A8(UnkOv41Canvas *canvas) {
    ov41_02249CC4(canvas->scroll);
}

static int ov41_022482B4(UnkOv41Canvas *canvas) {
    return canvas->objCount;
}

void ov41_022482B8(UnkOv41Canvas *canvas, int *outX, int *outY) {
    UnkOv41Node *node;
    int maxX = 0;
    int maxY = 0;
    int dx;
    int dy;

    for (node = canvas->frontList.next; node != &canvas->frontList; node = node->next) {
        ov41_02248400(node, &dx, &dy);
        if ((dx < 0 ? -dx : dx) > (maxX < 0 ? -maxX : maxX)) {
            maxX = dx;
        }
        if ((dy < 0 ? -dy : dy) > (maxY < 0 ? -maxY : maxY)) {
            maxY = dy;
        }
    }
    *outX = maxX;
    *outY = maxY;
}

static void ov41_02248324(UnkOv41Canvas *canvas, UnkOv41Node *node, BOOL toFront, void *a3) {
    int x;
    int y;
    int w;
    int h;
    int marginLeft;
    int marginTop;
    int px;
    int py;
    int marginRight;
    int marginBottom;
    int left;
    int top;
    int right;
    int bottom;
    void *unk;
    int i;
    BOOL hit;

    ov41_02249B44(node, &x, &y);
    ov41_02249B94(node, &w, &h);
    ov41_02249BAC(node, &marginLeft, &marginRight, &marginTop, &marginBottom);
    unk = *(void **)((u8 *)a3 + 0x1D8);
    hit = FALSE;
    left = x + marginLeft;
    top = y + marginTop;
    right = x + w - marginRight;
    bottom = y + h - marginBottom;
    for (i = 0; i < 16; i++) {
        ov41_02247E34(top, bottom, left, right, i, &px, &py);
        hit |= ov41_02249820(&canvas->monPic, px, py, unk);
    }
    if (hit) {
        if (toFront) {
            ov41_02249A50(node, &canvas->frontList);
        } else {
            ov41_02249A50(node, canvas->frontList.prev);
        }
    } else {
        if (toFront) {
            ov41_02249A50(node, &canvas->backList);
        } else {
            ov41_02249A50(node, canvas->backList.prev);
        }
    }
}
