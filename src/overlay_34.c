// The original TU saw Sprite_GetAnimationNumber() returning a wider type than u16
// (ov34_0225E5EC truncates its result), so hide the header prototype and
// re-declare it below.
#define Sprite_GetAnimationNumber Sprite_GetAnimationNumber_
#include "global.h"

#include "bg_window.h"
#include "dialog_box.h"
#include "field_system.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "mail_message.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_01.h"
#include "player_data.h"
#include "pm_string.h"
#include "save_palpad.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "system.h"
#include "task.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

#undef Sprite_GetAnimationNumber

typedef struct UnkOv34LogEntry {
    String *name;      // 0x00
    String *message;   // 0x04
    String *palPadMsg; // 0x08
    u32 trainerId;     // 0x0C
    u32 gender;        // 0x10
    MailMessage mail;  // 0x14
} UnkOv34LogEntry;     // size: 0x1C

typedef struct UnkOv34Log {
    UnkOv34LogEntry entries[30]; // 0x000
    int count;                   // 0x348
    int head;                    // 0x34C
} UnkOv34Log;

typedef struct UnkOv34Member {
    u8 unk0[0xD];
    u8 unkD;
    u8 unkE;
    u8 unkF;
    u8 unk10[8];
} UnkOv34Member; // size: 0x18

typedef struct UnkOv34UnionRoom {
    UnkOv34Member members[50]; // 0x000
    u8 unk4B0[0xF];
    u8 unk4BF;
    u8 unk4C0[0x18];
    SavePalPad *palPad; // 0x4D8
    UnkOv34Log *log;    // 0x4DC
} UnkOv34UnionRoom;

typedef struct UnkOv34UserGameInfo {
    u32 trainerId;
    u32 unk4;
    MailMessage mail;
} UnkOv34UserGameInfo;

typedef struct UnkOv34BssDesc {
    u8 unk0[0x50];
    UnkOv34UserGameInfo info; // 0x50
} UnkOv34BssDesc;

typedef struct UnkOv34Row {
    Window nameWindow;    // 0x00
    Window messageWindow; // 0x10
    Window palPadWindow;  // 0x20
    int gender;           // 0x30
    int highlight;        // 0x34
} UnkOv34Row;             // size: 0x38

typedef struct UnkOv34 {
    u32 state;                    // 0x000
    UnkOv34UnionRoom *unionRoom;  // 0x004
    void *unk8;                   // 0x008
    FieldSystem *fieldSystem;     // 0x00C
    PlayerProfile *profile;       // 0x010
    BgConfig *bgConfig;           // 0x014
    MessageFormat *msgFormat;     // 0x018
    MsgData *msgData;             // 0x01C
    void *scrnFile;               // 0x020
    NNSG2dScreenData *scrnData;   // 0x024
    SpriteList *spriteList;       // 0x028
    G2dRenderer renderer;         // 0x02C
    GF_2DGfxResMan *resMans[4];   // 0x154
    SpriteResource *resObjs[4];   // 0x164
    SpriteResourcesHeader header; // 0x174
    Sprite *sprites[3];           // 0x198
    u8 unk1A4[0x20];              // 0x1A4
    int touchedRow;               // 0x1C4
    UnkOv34Row rows[3];           // 0x1C8
    UnkOv34Log *log;              // 0x270
    Window titleWindow;           // 0x274
    u16 count;                    // 0x284
    u16 prevCount;                // 0x286
    u16 scrollPos;                // 0x288
    u16 scrollBarVisible;         // 0x28A
    int lastScrollPos;            // 0x28C
    int highlightTotal;           // 0x290
    int needsRedraw;              // 0x294
    u8 touchActive;               // 0x298
    s8 touchRepeatTimer;          // 0x299
    u8 touchRepeatStart;          // 0x29A
    u8 touchRepeatContinue;       // 0x29B
    int scrollTouchEnabled;       // 0x29C
    u16 buttonPressed[2];         // 0x2A0
    SysTask *task;                // 0x2A4
} UnkOv34;                        // size: 0x2A8

UnkOv34BssDesc *sub_02035754(int index);
PlayerProfile *sub_02035784(void);
PlayerProfile *sub_02035798(int index);
MailMessage *sub_0205AA84(void *a0);
int Sprite_GetAnimationNumber(Sprite *sprite);

static void ov34_0225D520(UnkOv34 *data);
static void ov34_0225D558(UnkOv34 *data, BgConfig *bgConfig);
static void ov34_0225D5A0(SysTask *task, void *taskData);
static void ov34_0225D5F8(UnkOv34 *data);
static void ov34_0225D650(BgConfig *bgConfig, UnkOv34Row *rows, Window *titleWindow);
static void ov34_0225D77C(UnkOv34Row *rows, Window *titleWindow);
UnkOv34 *ov34_0225D7A8(FieldSystem *fieldSystem);
void ov34_0225D87C(UnkOv34 *data);
static void ov34_0225D900(BgConfig *bgConfig);
static void ov34_0225D924(BgConfig *bgConfig);
static void ov34_0225DA50(UnkOv34 *data);
static void ov34_0225DB20(UnkOv34 *data);
static int ov34_0225DC00(UnkOv34Log *log, int index);
static int ov34_0225DC0C(int index, int offset);
static void ov34_0225DC18(UnkOv34 *data, int row, UnkOv34LogEntry *entry);
static void ov34_0225DD04(UnkOv34 *data);
static void ov34_0225DDB8(Sprite *sprite, int y);
static void ov34_0225DE04(UnkOv34 *data);
static int ov34_0225DE94(UnkOv34 *data);
static int ov34_0225E020(UnkOv34 *data);
static void ov34_0225E0E4(UnkOv34 *data);
static void ov34_0225E164(UnkOv34 *data);
static void ov34_0225E1C4(BgConfig *bgConfig, NNSG2dScreenData *scrnData, UnkOv34Row *rows, int skipRow, int count, int *highlightTotal);
static String *ov34_0225E2BC(SavePalPad *palPad, u32 trainerId, MessageFormat *msgFormat, MsgData *msgData, PlayerProfile *profile);
static void ov34_0225E348(UnkOv34 *data, u32 trainerId, MailMessage *mail, PlayerProfile *profile);
static BOOL ov34_0225E428(UnkOv34 *data, MailMessage *mail, u32 trainerId);
static void ov34_0225E4A8(UnkOv34 *data, PlayerProfile *profile, MailMessage *mail, u32 trainerId);
static void ov34_0225E4F8(UnkOv34 *data);
static void ov34_0225E560(UnkOv34 *data);
static void ov34_0225E56C(UnkOv34 *data);
static void ov34_0225E58C(UnkOv34 *data);
static u8 ov34_0225E5D4(UnkOv34 *data);
static void ov34_0225E5DC(UnkOv34 *data, int enabled);
static int ov34_0225E5E4(UnkOv34 *data);
static void ov34_0225E5EC(UnkOv34 *data, int button);
static void ov34_0225E630(UnkOv34 *data);

static void ov34_0225D520(UnkOv34 *data) {
    String *string = NewString_ReadMsgData(data->msgData, 0xDB);
    AddTextPrinterParameterizedWithColor(&data->titleWindow, 0, string, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    String_Delete(string);
}

static void ov34_0225D558(UnkOv34 *data, BgConfig *bgConfig) {
    if (gSystem.newKeys & PAD_BUTTON_X) {
        if (ov01_021F6B10(data->fieldSystem) == TRUE) {
            data->state = 3;
        }
    }
    ov34_0225E58C(data);
    ov34_0225E4F8(data);
    ov34_0225DE04(data);
    ov34_0225E164(data);
    ov34_0225DD04(data);
    ov34_0225E630(data);
}

static void ov34_0225D5A0(SysTask *task, void *taskData) {
    UnkOv34 *data = taskData;
    BgConfig *bgConfig = data->bgConfig;

    if (data->fieldSystem->unk84 == NULL) {
        return;
    }

    switch (data->state) {
    case 0:
    case 1:
        break;
    case 2:
        if (FieldSystem_TaskIsRunning(data->fieldSystem) == FALSE) {
            ov34_0225D558(data, bgConfig);
        }
        SpriteList_RenderAndAnimateSprites(data->spriteList);
        break;
    case 3:
        ov01_021F6A9C(data->fieldSystem, 0, NULL);
        data->state = 4;
        break;
    case 4:
    case 5:
        break;
    }
}

static void ov34_0225D5F8(UnkOv34 *data) {
    data->needsRedraw = 0;
    data->count = 0;
    data->prevCount = 0;
    data->scrollPos = 0;
    data->scrollBarVisible = 0;
    data->msgFormat = MessageFormat_New(HEAP_ID_FIELD1);
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x2E2, HEAP_ID_FIELD1);
    data->scrnFile = GfGfxLoader_GetScrnData(NARC_a_0_7_3, 3, TRUE, &data->scrnData, HEAP_ID_FIELD1);
}

static void ov34_0225D650(BgConfig *bgConfig, UnkOv34Row *rows, Window *titleWindow) {
    int i;

    for (i = 0; i < 3; i++) {
        AddWindowParameterized(bgConfig, &rows[i].nameWindow, i + 4, 1, i * 7 + 3, 8, 2, 12, i * 0x10 + 0xA0);
        FillWindowPixelBuffer(&rows[i].nameWindow, 0);
        CopyWindowToVram(&rows[i].nameWindow);
        AddWindowParameterized(bgConfig, &rows[i].messageWindow, i + 4, 2, i * 7 + 5, 27, 5, 12, i * 0x87 + 0xD0);
        FillWindowPixelBuffer(&rows[i].messageWindow, 0);
        CopyWindowToVram(&rows[i].messageWindow);
        AddWindowParameterized(bgConfig, &rows[i].palPadWindow, i + 4, 12, i * 7 + 3, 15, 2, 12, i * 0x1E + 0x265);
        FillWindowPixelBuffer(&rows[i].palPadWindow, 0);
        CopyWindowToVram(&rows[i].palPadWindow);
    }

    AddWindowParameterized(bgConfig, titleWindow, 4, 8, 0, 7, 2, 12, 0x2BF);
    FillWindowPixelBuffer(titleWindow, 0);
}

static void ov34_0225D77C(UnkOv34Row *rows, Window *titleWindow) {
    int i;

    RemoveWindow(titleWindow);
    for (i = 0; i < 3; i++) {
        RemoveWindow(&rows[i].messageWindow);
        RemoveWindow(&rows[i].nameWindow);
        RemoveWindow(&rows[i].palPadWindow);
    }
}

UnkOv34 *ov34_0225D7A8(FieldSystem *fieldSystem) {
    SysTask *task = CreateSysTaskAndEnvironment(ov34_0225D5A0, sizeof(UnkOv34), 4, HEAP_ID_FIELD1);
    UnkOv34 *data = SysTask_GetData(task);

    data->fieldSystem = fieldSystem;
    data->bgConfig = fieldSystem->bgConfig;
    data->unionRoom = (UnkOv34UnionRoom *)fieldSystem->unk84;
    data->unk8 = fieldSystem->unk80;
    data->profile = Save_PlayerData_GetProfile(fieldSystem->saveData);
    data->state = 2;
    data->task = task;
    data->log = ((UnkOv34UnionRoom *)fieldSystem->unk84)->log;

    ov34_0225D924(data->bgConfig);
    ov34_0225D5F8(data);
    SetKeyRepeatTimers(4, 8);
    ov34_0225E56C(data);
    ov34_0225E5DC(data, 1);
    ov34_0225DA50(data);
    ov34_0225DB20(data);
    FontID_SetAccessDirect(1, HEAP_ID_FIELD1);
    ov34_0225D650(data->bgConfig, data->rows, &data->titleWindow);
    ov34_0225D520(data);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    ov34_0225E560(data);
    return data;
}

void ov34_0225D87C(UnkOv34 *data) {
    int i;
    BgConfig *bgConfig;

    if (data->state - 2 <= 2) {
        bgConfig = data->bgConfig;
        data->state = 5;
        FontID_SetAccessLazy(1);
        SpriteTransfer_DeleteCharTransferTask(data->resObjs[GF_GFX_RES_TYPE_CHAR]);
        SpriteTransfer_DeletePlttTransferTask(data->resObjs[GF_GFX_RES_TYPE_PLTT]);
        for (i = 0; i < 4; i++) {
            Destroy2DGfxResObjMan(data->resMans[i]);
        }
        SpriteList_Delete(data->spriteList);
        DestroyMsgData(data->msgData);
        MessageFormat_Delete(data->msgFormat);
        ov34_0225D77C(data->rows, &data->titleWindow);
        ov34_0225D900(bgConfig);
        Heap_Free(data->scrnFile);
        DestroySysTaskAndEnvironment(data->task);
    } else {
        GF_ASSERT(FALSE);
    }
}

static void ov34_0225D900(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
}

static void ov34_0225D924(BgConfig *bgConfig) {
    ov34_0225D900(bgConfig);

    {
        BgTemplate template = { 0, 0, 0x800, 0, GF_BG_SCR_SIZE_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x6000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01, 1, 0, 0, FALSE };
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    }
    {
        BgTemplate template = { 0, 0, 0x800, 0, GF_BG_SCR_SIZE_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x6800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01, 2, 0, 0, FALSE };
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    }
    {
        BgTemplate template = { 0, 0, 0x800, 0, GF_BG_SCR_SIZE_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01, 2, 0, 0, FALSE };
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    }
    {
        BgTemplate template = { 0, 0, 0x800, 0, GF_BG_SCR_SIZE_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01, 2, 0, 0, FALSE };
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &template, GF_BG_TYPE_TEXT);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    }

    GfGfxLoader_GXLoadPal(NARC_a_0_7_3, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_FIELD1);
    GfGfxLoader_LoadCharData(NARC_a_0_7_3, 2, bgConfig, GF_BG_LYR_SUB_3, 0, 0x1400, TRUE, HEAP_ID_FIELD1);
    GfGfxLoader_LoadScrnData(NARC_a_0_7_3, 4, bgConfig, GF_BG_LYR_SUB_3, 0, 0x600, TRUE, HEAP_ID_FIELD1);
    FieldMessage_LoadTextPalettes(GF_PAL_LOCATION_SUB_BG, FALSE);
}

static void ov34_0225DA50(UnkOv34 *data) {
    int i;

    data->spriteList = G2dRenderer_Init(10, &data->renderer, HEAP_ID_FIELD1);
    for (i = 0; i < 4; i++) {
        data->resMans[i] = Create2DGfxResObjMan(1, (GfGfxResType)i, HEAP_ID_FIELD1);
    }
    data->resObjs[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_0_7_3, 5, TRUE, 999, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_FIELD1);
    data->resObjs[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_PLTT], NARC_a_0_7_3, 1, FALSE, 999, NNS_G2D_VRAM_TYPE_2DSUB, 1, HEAP_ID_FIELD1);
    data->resObjs[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_0_7_3, 6, TRUE, 999, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
    data->resObjs[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_0_7_3, 7, TRUE, 999, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(data->resObjs[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreatePlttTransferTask(data->resObjs[GF_GFX_RES_TYPE_PLTT]);
}

static const int ov34_0225E6A0[3] = { 0, 0xA0, 0x60 };

static void ov34_0225DB20(UnkOv34 *data) {
    int i;
    SpriteTemplate template;

    CreateSpriteResourcesHeader(&data->header, 999, 999, 999, 999, -1, -1, 0, 0, data->resMans[GF_GFX_RES_TYPE_CHAR], data->resMans[GF_GFX_RES_TYPE_PLTT], data->resMans[GF_GFX_RES_TYPE_CELL], data->resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);

    template.spriteList = data->spriteList;
    template.header = &data->header;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = HEAP_ID_FIELD1;

    for (i = 0; i < 3; i++) {
        template.position.x = FX32_CONST(232);
        template.position.y = FX32_CONST(ov34_0225E6A0[i]) + FX32_CONST(192);
        data->sprites[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(data->sprites[i], TRUE);
        Sprite_SetAnimCtrlSeq(data->sprites[i], i);
    }
}

static int ov34_0225DC00(UnkOv34Log *log, int index) {
    index++;
    if (index == 30) {
        index = 0;
    }
    return index;
}

static int ov34_0225DC0C(int index, int offset) {
    index += offset;
    if (index >= 30) {
        index -= 30;
    }
    return index;
}

static void ov34_0225DC18(UnkOv34 *data, int row, UnkOv34LogEntry *entry) {
    data->rows[row].gender = entry->gender;
    CopyToBgTilemapRect(data->bgConfig, GF_BG_LYR_SUB_3, 0, row * 7 + 2, 32, 7, data->scrnData->rawData, 0, data->rows[row].gender * 24, 32, 48);
    FillWindowPixelBuffer(&data->rows[row].nameWindow, 0);
    FillWindowPixelBuffer(&data->rows[row].messageWindow, 0);
    FillWindowPixelBuffer(&data->rows[row].palPadWindow, 0);
    AddTextPrinterParameterizedWithColor(&data->rows[row].nameWindow, 1, entry->name, 0, 1, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    AddTextPrinterParameterizedWithColor(&data->rows[row].messageWindow, 1, entry->message, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(&data->rows[row].nameWindow);
    ScheduleWindowCopyToVram(&data->rows[row].messageWindow);
    if (entry->palPadMsg != NULL) {
        AddTextPrinterParameterizedWithColor(&data->rows[row].palPadWindow, 1, entry->palPadMsg, 0, 1, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    }
    ScheduleWindowCopyToVram(&data->rows[row].palPadWindow);
}

static void ov34_0225DD04(UnkOv34 *data) {
    int i;
    int index = ov34_0225DC0C(data->log->head, data->scrollPos);
    int count = data->log->count;

    if (count > 3) {
        count = 3;
    }

    if (data->scrollPos != data->lastScrollPos) {
        ov34_0225E560(data);
        data->lastScrollPos = data->scrollPos;
    }

    if (data->needsRedraw == 1) {
        for (i = 0; i < count; i++) {
            ov34_0225DC18(data, i, &data->log->entries[index]);
            index = ov34_0225DC00(data->log, index);
        }
        DC_FlushRange(GetBgTilemapBuffer(data->bgConfig, GF_BG_LYR_SUB_3), 0x600);
        BgCopyOrUncompressTilemapBufferRangeToVram(data->bgConfig, GF_BG_LYR_SUB_3, GetBgTilemapBuffer(data->bgConfig, GF_BG_LYR_SUB_3), 0x600, 0);
        data->needsRedraw = 0;
    }
}

static void ov34_0225DDB8(Sprite *sprite, int y) {
    VecFx32 pos;

    pos.x = FX32_CONST(232);
    pos.y = FX32_CONST(y) + FX32_CONST(192);
    pos.z = 0;
    Sprite_SetMatrix(sprite, &pos);
}

static void ov34_0225DE04(UnkOv34 *data) {
    data->count = data->log->count;
    if (data->count > 3 && data->prevCount <= 3) {
        data->scrollBarVisible = 1;
        data->scrollPos = data->count - 3;
    }
    Sprite_SetDrawFlag(data->sprites[2], data->scrollBarVisible);
    if (data->scrollBarVisible != 0 && ov34_0225E5E4(data) == 1) {
        ov34_0225DDB8(data->sprites[2], data->scrollPos * 0x60 / (data->count - 3) + 0x30);
    }
    data->prevCount = data->count;
}

static const TouchscreenHitbox ov34_0225E730[] = {
    { .rect = { 0x00, 0x20, 0xE8, 0xF8 } },
    { .rect = { 0xA0, 0x20, 0xE8, 0xF8 } },
    { .rect = { 0x10, 0x48, 0x00, 0xE8 } },
    { .rect = { 0x48, 0x80, 0x00, 0xE8 } },
    { .rect = { 0x80, 0xB8, 0x00, 0xE8 } },
    { .rect = { 0x30, 0x90, 0xE8, 0x00 } },
    { .rect = { 0x00, 0x0F, 0x00, 0xE8 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static int ov34_0225DE94(UnkOv34 *data) {
    int i;
    int hit = TouchscreenHitbox_FindRectAtTouchHeld(ov34_0225E730);
    u8 touchActive = ov34_0225E5D4(data);

    if (hit != -1) {
        switch (hit) {
        case 0:
            ov34_0225E5EC(data, hit);
            if (touchActive == 1) {
                if (data->scrollPos != 0) {
                    PlaySE(SEQ_SE_DP_BUTTON3);
                    data->scrollPos--;
                }
                data->touchedRow = hit - 2;
            }
            break;
        case 1:
            ov34_0225E5EC(data, hit);
            if (touchActive == 1) {
                if (data->scrollPos < data->count - 3) {
                    PlaySE(SEQ_SE_DP_BUTTON3);
                    data->scrollPos++;
                }
                data->touchedRow = hit - 2;
            }
            break;
        case 5:
            data->touchedRow = hit - 2;
            break;
        case 6:
            if (gSystem.touchNew != 0 && ov01_021F6B10(data->fieldSystem) == TRUE) {
                PlaySE(SEQ_SE_DP_WIN_OPEN);
                data->state = 3;
            }
            break;
        default:
            if (gSystem.touchNew != 0) {
                if (data->log->count >= hit - 1) {
                    int index = ov34_0225DC0C(data->log->head, data->scrollPos + hit - 2);

                    for (i = 0; i < 10; i++) {
                        UnkOv34BssDesc *bssDesc = sub_02035754(i);
                        if (bssDesc != NULL) {
                            UnkOv34UserGameInfo *info = &bssDesc->info;
                            if (data->unionRoom->members[i].unkD == 2 && info->trainerId == data->log->entries[index].trainerId) {
                                PlaySE(SEQ_SE_DP_BUTTON3);
                                data->unionRoom->members[i].unkF = 1;
                                break;
                            }
                        }
                    }
                    if (data->log->entries[index].trainerId == PlayerProfile_GetTrainerID(sub_02035784())) {
                        PlaySE(SEQ_SE_DP_BUTTON3);
                        data->unionRoom->unk4BF = 1;
                    }
                }
                data->touchedRow = hit - 2;
            }
            break;
        }
    }

    return hit;
}

static const TouchscreenHitbox ov34_0225E6AC[] = {
    { .rect = { 0x30, 0x90, 0xE8, 0x00 } },
    { .rect = { 0x10, 0x48, 0x00, 0xE8 } },
    { .rect = { 0x48, 0x80, 0x00, 0xE8 } },
    { .rect = { 0x80, 0xB8, 0x00, 0xE8 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static int ov34_0225E020(UnkOv34 *data) {
    u32 x, y;
    int hit = TouchscreenHitbox_FindRectAtTouchHeld(ov34_0225E6AC);

    if (hit != -1) {
        if (hit == 0) {
            ov34_0225E5DC(data, 0);
            System_GetTouchHeldCoords(&x, &y);
            ov34_0225DDB8(data->sprites[2], y);
            if (data->count > 3) {
                int numPositions = data->count - 2;
                int step = 0x60 / numPositions;
                int i;

                for (i = 0; i < numPositions; i++) {
                    if (y >= step * i + 0x30 && y < step * i + step + 0x30) {
                        data->scrollPos = i;
                        break;
                    }
                }
            }
        } else if (data->log->count >= hit && data->touchedRow == hit - 1 && data->rows[hit - 1].highlight < 5) {
            data->rows[hit - 1].highlight++;
        }
    } else {
        ov34_0225E5DC(data, 1);
    }

    return hit;
}

static void ov34_0225E0E4(UnkOv34 *data) {
    if (gSystem.heldKeys & PAD_BUTTON_L) {
        ov34_0225E5EC(data, 0);
        if ((gSystem.newAndRepeatedKeys & PAD_BUTTON_L) && data->scrollPos != 0) {
            data->scrollPos--;
            PlaySE(SEQ_SE_DP_BUTTON3);
        }
    } else if (gSystem.heldKeys & PAD_BUTTON_R) {
        ov34_0225E5EC(data, 1);
        if ((gSystem.newAndRepeatedKeys & PAD_BUTTON_R) && data->scrollPos < data->count - 3) {
            data->scrollPos++;
            PlaySE(SEQ_SE_DP_BUTTON3);
        }
    }
}

static void ov34_0225E164(UnkOv34 *data) {
    int buttonHit;
    int listHit = -1;

    if (FieldSystem_TaskIsRunning(data->fieldSystem) == FALSE) {
        buttonHit = ov34_0225DE94(data);
        listHit = ov34_0225E020(data);
        if (buttonHit == -1 && listHit == -1) {
            ov34_0225E0E4(data);
        }
    }

    ov34_0225E1C4(data->bgConfig, data->scrnData, data->rows, listHit - 1, data->log->count, &data->highlightTotal);
}

static const int _0225E694[3] = { 0, 0, 0 };

static void ov34_0225E1C4(BgConfig *bgConfig, NNSG2dScreenData *scrnData, UnkOv34Row *rows, int skipRow, int count, int *highlightTotal) {
    int i;
    int total = 0;

    if (count > 3) {
        count = 3;
    }

    for (i = 0; i < count; i++) {
        if (skipRow != i && rows[i].highlight != 0) {
            rows[i].highlight--;
        }
        total += rows[i].highlight;
    }

    if (total == 0 && *highlightTotal == 0) {
        *highlightTotal = total;
        return;
    }

    *highlightTotal = total;

    for (i = 0; i < count; i++) {
        int frame = rows[i].highlight / 2;

        CopyToBgTilemapRect(bgConfig, GF_BG_LYR_SUB_3, 0, i * 7 + 2, 32, 7, scrnData->rawData, 0, frame * 8 + rows[i].gender * 24, 32, 48);
        BgSetPosTextAndCommit(bgConfig, i + 4, BG_POS_OP_SET_Y, _0225E694[frame]);
    }

    DC_FlushRange(GetBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3), 0x600);
    BgCopyOrUncompressTilemapBufferRangeToVram(bgConfig, GF_BG_LYR_SUB_3, GetBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3), 0x600, 0);
}

static String *ov34_0225E2BC(SavePalPad *palPad, u32 trainerId, MessageFormat *msgFormat, MsgData *msgData, PlayerProfile *profile) {
    int friendType = 0;
    String *ret = NULL;

    if (trainerId != PlayerProfile_GetTrainerID(profile)) {
        friendType = PalPad_PlayerIdIsFriendOrMutual(palPad, trainerId);
    }

    if (friendType > 0) {
        if (friendType == 1) {
            BufferPlayersName(msgFormat, 0, profile);
        } else if (friendType >= 2) {
            int n = friendType - 2;
            String *string = String_New(10, HEAP_ID_87);

            CopyU16ArrayToString(string, (const u16 *)PalPad_GetNthEntry(palPad, n));
            BufferString(msgFormat, 0, string, 0, 0, PalPadEntry_GetFromUnk68Array(palPad, n));
            String_Delete(string);
        }
        ret = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, 0xD0, HEAP_ID_87);
    }

    return ret;
}

static void ov34_0225E348(UnkOv34 *data, u32 trainerId, MailMessage *mail, PlayerProfile *profile) {
    UnkOv34Log *log = data->log;
    SavePalPad *palPad = data->unionRoom->palPad;
    int *index = (log->count == 30) ? &log->head : &log->count;

    if (log->entries[*index].message != NULL) {
        String_Delete(log->entries[*index].message);
    }
    if (log->entries[*index].palPadMsg != NULL) {
        String_Delete(log->entries[*index].palPadMsg);
    }
    CopyU16ArrayToString(log->entries[*index].name, PlayerProfile_GetNamePtr(profile));
    log->entries[*index].mail = *mail;
    log->entries[*index].trainerId = trainerId;
    log->entries[*index].gender = PlayerProfile_GetTrainerGender(profile);
    log->entries[*index].message = MailMsg_GetExpandedString(mail, HEAP_ID_87);
    log->entries[*index].palPadMsg = ov34_0225E2BC(palPad, trainerId, data->msgFormat, data->msgData, data->profile);
    (*index)++;
    if (log->head == 30) {
        log->head = 0;
    }
}

static BOOL ov34_0225E428(UnkOv34 *data, MailMessage *mail, u32 trainerId) {
    int i;

    if (MailMsg_IsInit(mail) == FALSE) {
        return FALSE;
    }

    for (i = 0; i < data->log->count; i++) {
        if (trainerId == data->log->entries[i].trainerId && MailMsg_Compare(mail, &data->log->entries[i].mail)) {
            break;
        }
    }

    if (i != data->log->count && data->log->count != 0) {
        return FALSE;
    }

    if (trainerId == data->log->entries[i].trainerId || MailMsg_Compare(mail, &data->log->entries[i].mail)) {
    }

    return TRUE;
}

static void ov34_0225E4A8(UnkOv34 *data, PlayerProfile *profile, MailMessage *mail, u32 trainerId) {
    BOOL atBottom = FALSE;

    if (data->scrollPos == data->count - 3) {
        atBottom = TRUE;
    }

    ov34_0225E348(data, trainerId, mail, profile);

    if (data->scrollBarVisible != 0 && atBottom) {
        data->scrollPos = data->log->count - 3;
    }

    ov34_0225E560(data);
}

static void ov34_0225E4F8(UnkOv34 *data) {
    int i;
    MailMessage *mail;

    if (FieldSystem_TaskIsRunning(data->fieldSystem) != FALSE) {
        return;
    }

    for (i = 0; i < 16; i++) {
        UnkOv34BssDesc *bssDesc = sub_02035754(i);
        if (bssDesc != NULL) {
            UnkOv34UserGameInfo *info = &bssDesc->info;
            MailMessage *mail = &info->mail;
            if (ov34_0225E428(data, mail, bssDesc->info.trainerId)) {
                ov34_0225E4A8(data, sub_02035798(i), mail, info->trainerId);
            }
        }
    }

    mail = sub_0205AA84(data->unk8);
    if (mail != NULL) {
        ov34_0225E4A8(data, data->profile, mail, PlayerProfile_GetTrainerID(data->profile));
    }
}

static void ov34_0225E560(UnkOv34 *data) {
    data->needsRedraw = 1;
}

static void ov34_0225E56C(UnkOv34 *data) {
    data->touchActive = 0;
    data->touchRepeatStart = 8;
    data->touchRepeatContinue = 4;
    data->touchRepeatTimer = data->touchRepeatStart;
}

static void ov34_0225E58C(UnkOv34 *data) {
    data->touchActive = 0;
    if (gSystem.touchNew) {
        data->touchActive = 1;
        return;
    }
    if (gSystem.touchHeld) {
        data->touchRepeatTimer--;
        if (data->touchRepeatTimer < 0) {
            data->touchActive = 1;
            data->touchRepeatTimer = data->touchRepeatContinue;
        }
    } else {
        data->touchRepeatTimer = data->touchRepeatStart;
    }
}

static u8 ov34_0225E5D4(UnkOv34 *data) {
    return data->touchActive;
}

static void ov34_0225E5DC(UnkOv34 *data, int enabled) {
    data->scrollTouchEnabled = enabled;
}

static int ov34_0225E5E4(UnkOv34 *data) {
    return data->scrollTouchEnabled;
}

static void ov34_0225E5EC(UnkOv34 *data, int button) {
    int frame = Sprite_GetAnimationFrame(data->sprites[button]);
    u16 anim = Sprite_GetAnimationNumber(data->sprites[button]);

    if (frame != 0 || anim != button + 4) {
        Sprite_SetAnimCtrlSeq(data->sprites[button], button + 4);
    }
    data->buttonPressed[button] = 1;
}

static void ov34_0225E630(UnkOv34 *data) {
    int i;

    for (i = 0; i < 2; i++) {
        Sprite_GetAnimationFrame(data->sprites[i]);
        if (data->buttonPressed[i] == 1) {
            Sprite_SetAnimActiveFlag(data->sprites[i], FALSE);
            data->buttonPressed[i] = 0;
        } else if (Sprite_GetAnimActiveFlag(data->sprites[i]) == FALSE) {
            Sprite_SetAnimActiveFlag(data->sprites[i], TRUE);
            Sprite_SetAnimationFrame(data->sprites[i], 1);
        }
    }
}
