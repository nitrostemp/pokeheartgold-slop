#include "global.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "mail_message.h"
#include "menu_input_state.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_55.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_icon_idx.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

// Mail viewer / composer. HG version of pokeplatinum's mail_viewer.c with
// touch input and a YesNoPrompt instead of the list menu.

enum MailViewerMode {
    MAIL_VIEWER_MODE_READ = 0,
    MAIL_VIEWER_MODE_WRITE,
    MAIL_VIEWER_MODE_CONFIRM_EMPTY,
    MAIL_VIEWER_MODE_CANCEL,
};

enum MailViewerWindow {
    MAIL_VIEWER_WINDOW_SENTENCE_1 = 0,
    MAIL_VIEWER_WINDOW_SENTENCE_2,
    MAIL_VIEWER_WINDOW_SENTENCE_3,
    MAIL_VIEWER_WINDOW_CONFIRM,
    MAIL_VIEWER_WINDOW_CANCEL,
    MAIL_VIEWER_WINDOW_MESSAGE,
    MAIL_VIEWER_WINDOW_COUNT,
};

#define SELECTION_INDEX_CONFIRM_CANCEL 3
#define VERTICAL_SELECTION_INDEX_COUNT 4

#define MAIL_OPTION_PALETTE_SLOT_START 34

typedef struct MailViewerApp MailViewerApp;
typedef BOOL (*MailViewerModeFunc)(MailViewerApp *app);

struct MailViewerApp {
    enum HeapID heapID;                       // 0x00
    int state;                                // 0x04
    u16 subState;                             // 0x08
    u8 inputMode;                             // 0x0A
    u8 frame;                                 // 0x0B
    u8 mode;                                  // 0x0C
    u8 initialMode;                           // 0x0D
    u8 padding;                               // 0x0E
    u8 printerID;                             // 0x0F
    u8 textFrameDelay;                        // 0x10
    u8 verticalSelectionIndex;                // 0x11
    u8 horizontalSelectionIndex;              // 0x12
    u8 pauseGlowEffect;                       // 0x13
    u8 blendFraction;                         // 0x14
    u8 blendDirection;                        // 0x15
    u8 blendIndex;                            // 0x16
    u8 prevBlendIndex;                        // 0x17
    BgConfig *bgConfig;                       // 0x18
    UnkStruct_ov55_021E5B08 *args;            // 0x1C
    MsgData *msgData;                         // 0x20
    void *dummy[1];                           // 0x24
    u8 padding2[8];                           // 0x28
    PaletteData *paletteData;                 // 0x30
    MailViewerModeFunc onSwitchToKeys;        // 0x34
    MailViewerModeFunc onSwitchToTouch;       // 0x38
    void *backgroundNSCRBuffer;               // 0x3C
    void *interfaceNSCRBuffer;                // 0x40
    NNSG2dScreenData *backgroundScreenData;   // 0x44
    NNSG2dScreenData *interfaceScreenData;    // 0x48
    Window windows[MAIL_VIEWER_WINDOW_COUNT]; // 0x4C
    YesNoPrompt *yesNoPrompt;                 // 0xAC
    SpriteSystem *spriteSys;                  // 0xB0
    SpriteManager *spriteMan;                 // 0xB4
    ManagedSprite *managedSprites[3];         // 0xB8
}; // size: 0xC4

typedef union MailIconData {
    u16 asValue;
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
} MailIconData;

BOOL ov56_021E5C20(OverlayManager *man, int *state);
BOOL ov56_021E5C9C(OverlayManager *man, int *state);
BOOL ov56_021E5CB4(OverlayManager *man, int *state);
static BOOL ov56_021E5CE0(MailViewerApp *app);
static void ov56_021E5D08(MailViewerApp *app);
static BOOL ov56_021E5D34(MailViewerApp *app);
static BOOL ov56_021E5D40(MailViewerApp *app);
static BOOL ov56_021E5D44(MailViewerApp *app);
static BOOL ov56_021E5DA4(MailViewerApp *app);
static BOOL ov56_021E5DDC(MailViewerApp *app);
static BOOL ov56_021E5EFC(MailViewerApp *app);
static BOOL ov56_021E5FB4(MailViewerApp *app);
static BOOL ov56_021E5FDC(MailViewerApp *app);
static void ov56_021E609C(MailViewerApp *app);
static int ov56_021E60F4(MailViewerApp *app);
static BOOL ov56_021E614C(MailViewerApp *app);
static BOOL ov56_021E6228(MailViewerApp *app);
static void ov56_021E63C0(void *data);
static void ov56_021E63FC(SysTask *task, void *data);
static BOOL ov56_021E647C(MailViewerApp *app);
static BOOL ov56_021E64C8(MailViewerApp *app);
static void ov56_021E64F4(void);
static void ov56_021E6514(MailViewerApp *app);
static void ov56_021E660C(MailViewerApp *app);
static void ov56_021E6650(MailViewerApp *app);
static void ov56_021E692C(MailViewerApp *app);
static void ov56_021E696C(MailViewerApp *app);
static void ov56_021E6A7C(MailViewerApp *app);
static void ov56_021E6AA4(MailViewerApp *app);
static void ov56_021E6BB4(MailViewerApp *app);
static void ov56_021E6D90(MailViewerApp *app);

static const TouchscreenHitbox ov56_021E6E20[6];

BOOL ov56_021E5C20(OverlayManager *man, int *state) {
    MailViewerApp *app;

    Heap_Create(HEAP_ID_3, HEAP_ID_41, 0x20000);
    app = OverlayManager_CreateAndGetData(man, sizeof(MailViewerApp), HEAP_ID_41);
    memset(app, 0, sizeof(MailViewerApp));

    app->heapID = HEAP_ID_41;
    app->args = OverlayManager_GetArgs(man);
    app->initialMode = app->mode = app->args->unk0;
    app->verticalSelectionIndex = app->args->mailMessageIdx;
    app->horizontalSelectionIndex = ((u8 *)app->args)[3];
    app->prevBlendIndex = app->blendIndex = app->verticalSelectionIndex;
    app->args->unk0 = 0xFFFF;
    app->textFrameDelay = Options_GetTextFrameDelay(app->args->options);
    app->frame = Options_GetFrame(app->args->options);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    app->inputMode = MenuInputStateMgr_GetState(app->args->menuInputStateMgr);
    return TRUE;
}

BOOL ov56_021E5C9C(OverlayManager *man, int *state) {
    MailViewerApp *app = OverlayManager_GetData(man);

    return !!ov56_021E6228(app);
}

BOOL ov56_021E5CB4(OverlayManager *man, int *state) {
    MailViewerApp *app = OverlayManager_GetData(man);
    enum HeapID heapID;

    TextFlags_SetCanABSpeedUpPrint(FALSE);
    heapID = app->heapID;
    MenuInputStateMgr_SetState(app->args->menuInputStateMgr, (MenuInputState)app->inputMode);
    OverlayManager_FreeData(man);
    Heap_Destroy(heapID);
    return TRUE;
}

static BOOL ov56_021E5CE0(MailViewerApp *app) {
    int i;

    for (i = 0; i < 3; i++) {
        if (MailMsg_IsInit(&app->args->mailMessages[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

static void ov56_021E5D08(MailViewerApp *app) {
    PaletteData_BlendPalette(app->paletteData, PLTTBUF_MAIN_BG, MAIL_OPTION_PALETTE_SLOT_START + app->prevBlendIndex, 1, 0, RGB_WHITE);
    app->blendDirection = 0;
    app->blendFraction = 0;
}

static BOOL ov56_021E5D34(MailViewerApp *app) {
    ov56_021E5D08(app);
    return FALSE;
}

static BOOL ov56_021E5D40(MailViewerApp *app) {
    return FALSE;
}

static BOOL ov56_021E5D44(MailViewerApp *app) {
    if (app->inputMode == MENU_INPUT_STATE_TOUCH) {
        if (System_GetTouchHeld()) {
            return FALSE;
        }
        if (gSystem.heldKeys) {
            if (app->onSwitchToKeys != NULL) {
                app->onSwitchToKeys(app);
            }
            app->inputMode = MENU_INPUT_STATE_BUTTONS;
            return TRUE;
        }
    } else {
        if (gSystem.heldKeys) {
            return FALSE;
        }
        if (System_GetTouchHeld()) {
            if (app->onSwitchToTouch != NULL) {
                app->onSwitchToTouch(app);
            }
            app->inputMode = MENU_INPUT_STATE_TOUCH;
            return FALSE;
        }
    }
    return FALSE;
}

static BOOL ov56_021E5DA4(MailViewerApp *app) {
    if (gSystem.newKeys) {
        app->inputMode = MENU_INPUT_STATE_BUTTONS;
    } else if (System_GetTouchHeld()) {
        app->inputMode = MENU_INPUT_STATE_TOUCH;
    } else {
        return FALSE;
    }
    PlaySE(SEQ_SE_DP_PIRORIRO2);
    return TRUE;
}

static BOOL ov56_021E5DDC(MailViewerApp *app) {
    BOOL selectionChanged = FALSE;

    if (gSystem.newKeys & PAD_BUTTON_A) {
        if (app->verticalSelectionIndex == SELECTION_INDEX_CONFIRM_CANCEL) {
            if (app->horizontalSelectionIndex == 0) {
                if (ov56_021E5CE0(app)) {
                    PlaySE(SEQ_SE_DP_DECIDE);
                    app->mode = MAIL_VIEWER_MODE_CONFIRM_EMPTY;
                    return FALSE;
                } else {
                    app->args->unk0 = 3;
                    PlaySE(SEQ_SE_DP_PIRORIRO2);
                    app->args->mailMessageIdx = 0;
                    ((u8 *)app->args)[3] = 0;
                    return TRUE;
                }
            } else {
                PlaySE(SEQ_SE_DP_DECIDE);
                app->mode = MAIL_VIEWER_MODE_CANCEL;
                return FALSE;
            }
        } else {
            app->args->unk0 = app->args->mailMessageIdx = app->verticalSelectionIndex;
            ((u8 *)app->args)[3] = app->horizontalSelectionIndex;
            PlaySE(SEQ_SE_DP_DECIDE);
        }
        return TRUE;
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_DECIDE);
        app->mode = MAIL_VIEWER_MODE_CANCEL;
        return FALSE;
    }

    if (gSystem.newKeys & PAD_BUTTON_START) {
        app->verticalSelectionIndex = SELECTION_INDEX_CONFIRM_CANCEL;
        app->horizontalSelectionIndex = 0;
        selectionChanged = TRUE;
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        app->verticalSelectionIndex = (app->verticalSelectionIndex + 1) % VERTICAL_SELECTION_INDEX_COUNT;
        selectionChanged = TRUE;
    } else if (gSystem.newKeys & PAD_KEY_UP) {
        app->verticalSelectionIndex = (app->verticalSelectionIndex + VERTICAL_SELECTION_INDEX_COUNT - 1) % VERTICAL_SELECTION_INDEX_COUNT;
        selectionChanged = TRUE;
    } else if (gSystem.newKeys & (PAD_KEY_RIGHT | PAD_KEY_LEFT)) {
        if (app->verticalSelectionIndex == SELECTION_INDEX_CONFIRM_CANCEL) {
            app->horizontalSelectionIndex ^= 1;
            selectionChanged = TRUE;
        }
    } else {
        return FALSE;
    }

    if (!selectionChanged) {
        return FALSE;
    }

    PlaySE(SEQ_SE_DP_SELECT);

    if (app->verticalSelectionIndex == SELECTION_INDEX_CONFIRM_CANCEL) {
        app->blendIndex = app->verticalSelectionIndex + app->horizontalSelectionIndex;
    } else {
        app->blendIndex = app->verticalSelectionIndex;
    }
    return FALSE;
}

static BOOL ov56_021E5EFC(MailViewerApp *app) {
    int hit = TouchscreenHitbox_FindRectAtTouchNew(ov56_021E6E20);
    u16 pixel;

    if (hit == -1) {
        return FALSE;
    }

    pixel = 1;
    if (DoesPixelAtScreenXYMatchPtrVal(app->bgConfig, GF_BG_LYR_MAIN_2, gSystem.touchX, gSystem.touchY, &pixel) == TRUE) {
        return FALSE;
    }

    if (hit == 3) {
        PlaySE(SEQ_SE_DP_DECIDE);
        app->mode = MAIL_VIEWER_MODE_CANCEL;
        return FALSE;
    } else if (hit < 3) {
        app->args->unk0 = app->args->mailMessageIdx = hit;
        ((u8 *)app->args)[3] = app->horizontalSelectionIndex;
        PlaySE(SEQ_SE_DP_DECIDE);
        return TRUE;
    } else {
        if (ov56_021E5CE0(app)) {
            PlaySE(SEQ_SE_DP_DECIDE);
            app->mode = MAIL_VIEWER_MODE_CONFIRM_EMPTY;
            return FALSE;
        }
        app->args->unk0 = 3;
        PlaySE(SEQ_SE_DP_PIRORIRO2);
        app->args->mailMessageIdx = 0;
        ((u8 *)app->args)[3] = 0;
        return TRUE;
    }
}

static BOOL ov56_021E5FB4(MailViewerApp *app) {
    if (ov56_021E5D44(app)) {
        return FALSE;
    }
    if (app->inputMode == MENU_INPUT_STATE_BUTTONS) {
        return ov56_021E5DDC(app);
    }
    return ov56_021E5EFC(app);
}

static BOOL ov56_021E5FDC(MailViewerApp *app) {
    BOOL done = FALSE;
    String *string;

    switch (app->subState) {
    case 0:
        DrawFrameAndWindow2(&app->windows[MAIL_VIEWER_WINDOW_MESSAGE], TRUE, 10, 6);
        FillWindowPixelBuffer(&app->windows[MAIL_VIEWER_WINDOW_MESSAGE], 0xFF);
        string = String_New(38 * 2, app->heapID);
        ReadMsgDataIntoString(app->msgData, 2, string);
        AddTextPrinterParameterizedWithColor(&app->windows[MAIL_VIEWER_WINDOW_MESSAGE], 1, string, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 15), NULL);
        String_Delete(string);
        break;
    case 1:
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            done = TRUE;
            app->inputMode = MENU_INPUT_STATE_BUTTONS;
        } else if (System_GetTouchNew()) {
            done = TRUE;
            app->inputMode = MENU_INPUT_STATE_TOUCH;
            ov56_021E5D08(app);
        }
        if (!done) {
            return FALSE;
        }
        ClearFrameAndWindow2(&app->windows[MAIL_VIEWER_WINDOW_MESSAGE], TRUE);
        ClearWindowTilemapAndCopyToVram(&app->windows[MAIL_VIEWER_WINDOW_MESSAGE]);
        app->subState = 0;
        app->mode = app->initialMode;
        return FALSE;
    }

    app->subState++;
    return FALSE;
}

static void ov56_021E609C(MailViewerApp *app) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0, sizeof(YesNoPromptTemplate));
    template.bgConfig = app->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = 0x1BB;
    template.plttSlot = 3;
    template.x = 24;
    template.y = 10;
    template.ignoreTouchFlag = app->inputMode;
    template.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplateWithPalette(app->yesNoPrompt, &template, app->paletteData);
}

static int ov56_021E60F4(MailViewerApp *app) {
    int result;
    int touchMode;

    switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
    case YESNORESPONSE_YES:
        result = TRUE;
        break;
    case YESNORESPONSE_NO:
        result = FALSE;
        break;
    default:
        return -1;
    }

    touchMode = YesNoPrompt_IsInTouchMode(app->yesNoPrompt);
    if (touchMode != app->inputMode) {
        if (app->inputMode == MENU_INPUT_STATE_BUTTONS) {
            app->onSwitchToKeys(app);
        } else {
            app->onSwitchToTouch(app);
        }
        app->inputMode = touchMode;
    }
    YesNoPrompt_Reset(app->yesNoPrompt);
    return result;
}

static BOOL ov56_021E614C(MailViewerApp *app) {
    String *string;
    int result;

    switch (app->subState) {
    case 0:
        app->pauseGlowEffect = TRUE;
        DrawFrameAndWindow2(&app->windows[MAIL_VIEWER_WINDOW_MESSAGE], TRUE, 10, 6);
        FillWindowPixelBuffer(&app->windows[MAIL_VIEWER_WINDOW_MESSAGE], 0xFF);
        string = String_New(38 * 2, app->heapID);
        ReadMsgDataIntoString(app->msgData, 3, string);
        app->printerID = AddTextPrinterParameterizedWithColor(&app->windows[MAIL_VIEWER_WINDOW_MESSAGE], 1, string, 0, 0, app->textFrameDelay, MAKE_TEXT_COLOR(1, 2, 15), NULL);
        String_Delete(string);
        ov56_021E5D08(app);
        app->prevBlendIndex = app->blendIndex;
        break;
    case 1:
        if (TextPrinterCheckActive(app->printerID)) {
            return FALSE;
        }
        ov56_021E609C(app);
        break;
    case 2:
        result = ov56_021E60F4(app);
        if (result < 0) {
            return FALSE;
        }
        ClearFrameAndWindow2(&app->windows[MAIL_VIEWER_WINDOW_MESSAGE], TRUE);
        ClearWindowTilemapAndCopyToVram(&app->windows[MAIL_VIEWER_WINDOW_MESSAGE]);
        app->subState = 0;
        if (result) {
            app->args->unk0 = 0xFFFF;
            return TRUE;
        } else {
            app->mode = app->initialMode;
            app->pauseGlowEffect = FALSE;
            return FALSE;
        }
    }

    app->subState++;
    return FALSE;
}

static BOOL ov56_021E6228(MailViewerApp *app) {
    static const MailViewerModeFunc sModeFuncs[] = {
        ov56_021E5DA4,
        ov56_021E5FB4,
        ov56_021E5FDC,
        ov56_021E614C,
    };

    switch (app->state) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);
        sub_0200FBF4(PM_LCD_TOP, RGB_BLACK);
        sub_0200FBF4(PM_LCD_BOTTOM, RGB_BLACK);
        ResetVisibleHardwareWindows(PM_LCD_TOP);
        ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
        break;
    case 1:
        if (!ov56_021E647C(app)) {
            return FALSE;
        }
        Main_SetVBlankIntrCB(ov56_021E63C0, app);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        SetMasterBrightnessNeutral(PM_LCD_TOP);
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG3, 28, 4);
        PaletteData_BeginPaletteFade(app->paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_MAIN_OBJ), 0xFFFF, -1, 16, 0, RGB_BLACK);
        break;
    case 2:
        if (app->spriteMan != NULL) {
            SpriteSystem_DrawSprites(app->spriteMan);
        }
        if (PaletteData_GetSelectedBuffersBitmask(app->paletteData)) {
            return FALSE;
        }
        break;
    case 3:
        if (app->spriteMan != NULL) {
            SpriteSystem_DrawSprites(app->spriteMan);
        }
        if (!sModeFuncs[app->mode](app)) {
            return FALSE;
        }
        PaletteData_BeginPaletteFade(app->paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_MAIN_OBJ), 0xFFFF, -1, 0, 16, RGB_BLACK);
        break;
    case 4:
        if (PaletteData_GetSelectedBuffersBitmask(app->paletteData)) {
            if (app->spriteMan != NULL) {
                SpriteSystem_DrawSprites(app->spriteMan);
            }
            return FALSE;
        }
        sub_0200FBF4(PM_LCD_TOP, RGB_BLACK);
        sub_0200FBF4(PM_LCD_BOTTOM, RGB_BLACK);
        Main_SetVBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);
        break;
    case 5:
        if (ov56_021E64C8(app)) {
            return TRUE;
        }
        return FALSE;
    }

    app->state++;
    return FALSE;
}

static void ov56_021E63C0(void *data) {
    MailViewerApp *app = data;

    if (app->paletteData != NULL) {
        PaletteData_PushTransparentBuffers(app->paletteData);
    }
    if (app->spriteSys != NULL) {
        SpriteSystem_TransferOam();
    }
    NNS_GfdDoVramTransfer();
    DoScheduledBgGpuUpdates(app->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

static void ov56_021E63FC(SysTask *task, void *data) {
    MailViewerApp *app = data;

    if (app->paletteData == NULL) {
        SysTask_Destroy(task);
        return;
    }

    if (app->pauseGlowEffect || app->inputMode == MENU_INPUT_STATE_TOUCH) {
        return;
    }

    if (app->prevBlendIndex != app->blendIndex) {
        ov56_021E5D08(app);
        app->prevBlendIndex = app->blendIndex;
    }

    PaletteData_BlendPalette(app->paletteData, PLTTBUF_MAIN_BG, MAIL_OPTION_PALETTE_SLOT_START + app->blendIndex, 1, app->blendFraction, RGB_WHITE);

    if (app->blendDirection) {
        if (app->blendFraction-- == 1) {
            app->blendDirection ^= 1;
        }
    } else {
        if (app->blendFraction++ == 12) {
            app->blendDirection ^= 1;
        }
    }
}

static BOOL ov56_021E647C(MailViewerApp *app) {
    switch (app->subState) {
    case 0:
        ov56_021E6514(app);
        break;
    case 1:
        ov56_021E6650(app);
        break;
    case 2:
        ov56_021E696C(app);
        ov56_021E6AA4(app);
        break;
    case 3:
        ov56_021E6BB4(app);
        app->subState = 0;
        return TRUE;
    }

    app->subState++;
    return FALSE;
}

static BOOL ov56_021E64C8(MailViewerApp *app) {
    ov56_021E6D90(app);

    // bug: dummy is never initialized
    if (app->dummy != NULL) {
        DestroyMsgData(app->msgData);
    }

    ov56_021E6A7C(app);
    ov56_021E692C(app);
    ov56_021E660C(app);
    return TRUE;
}

static void ov56_021E64F4(void) {
    GraphicsBanks banks = {
        .bg = GX_VRAM_BG_128_A,
        .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
        .subbg = GX_VRAM_SUB_BG_128_C,
        .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
        .obj = GX_VRAM_OBJ_64_E,
        .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
        .subobj = GX_VRAM_SUB_OBJ_16_I,
        .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
        .tex = GX_VRAM_TEX_NONE,
        .texpltt = GX_VRAM_TEXPLTT_NONE,
    };

    GfGfx_SetBanks(&banks);
}

static void ov56_021E6514(MailViewerApp *app) {
    ov56_021E64F4();

    app->bgConfig = BgConfig_Alloc(app->heapID);

    GraphicsModes graphicsModes = {
        .dispMode = GX_DISPMODE_GRAPHICS,
        .bgMode = GX_BGMODE_0,
        .subMode = GX_BGMODE_0,
        ._2d3dMode = GX_BG0_AS_2D,
    };

    SetBothScreensModesAndDisable(&graphicsModes);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    BgTemplate bgTemplates[] = {
        {
         .x = 0,
         .y = 0,
         .bufferSize = 0x800,
         .baseTile = 0,
         .size = GF_BG_SCR_SIZE_256x256,
         .colorMode = GX_BG_COLORMODE_16,
         .screenBase = GX_BG_SCRBASE_0xf800,
         .charBase = GX_BG_CHARBASE_0x10000,
         .bgExtPltt = GX_BG_EXTPLTT_01,
         .priority = 0,
         .areaOver = 0,
         .mosaic = FALSE,
         },
        {
         .x = 0,
         .y = 0,
         .bufferSize = 0x800,
         .baseTile = 0,
         .size = GF_BG_SCR_SIZE_256x256,
         .colorMode = GX_BG_COLORMODE_16,
         .screenBase = GX_BG_SCRBASE_0xf000,
         .charBase = GX_BG_CHARBASE_0x00000,
         .bgExtPltt = GX_BG_EXTPLTT_01,
         .priority = 1,
         .areaOver = 0,
         .mosaic = FALSE,
         },
        {
         .x = 0,
         .y = 0,
         .bufferSize = 0x800,
         .baseTile = 0,
         .size = GF_BG_SCR_SIZE_256x256,
         .colorMode = GX_BG_COLORMODE_16,
         .screenBase = GX_BG_SCRBASE_0xe800,
         .charBase = GX_BG_CHARBASE_0x00000,
         .bgExtPltt = GX_BG_EXTPLTT_01,
         .priority = 2,
         .areaOver = 0,
         .mosaic = FALSE,
         },
        {
         .x = 0,
         .y = 0,
         .bufferSize = 0x800,
         .baseTile = 0,
         .size = GF_BG_SCR_SIZE_256x256,
         .colorMode = GX_BG_COLORMODE_16,
         .screenBase = GX_BG_SCRBASE_0xe000,
         .charBase = GX_BG_CHARBASE_0x00000,
         .bgExtPltt = GX_BG_EXTPLTT_01,
         .priority = 3,
         .areaOver = 0,
         .mosaic = FALSE,
         },
        {
         .x = 0,
         .y = 0,
         .bufferSize = 0x800,
         .baseTile = 0,
         .size = GF_BG_SCR_SIZE_256x256,
         .colorMode = GX_BG_COLORMODE_16,
         .screenBase = GX_BG_SCRBASE_0xf800,
         .charBase = GX_BG_CHARBASE_0x00000,
         .bgExtPltt = GX_BG_EXTPLTT_01,
         .priority = 0,
         .areaOver = 0,
         .mosaic = FALSE,
         },
    };

    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_0, &bgTemplates[0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_1, &bgTemplates[1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_2, &bgTemplates[2], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_3, &bgTemplates[3], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_0, &bgTemplates[4], GF_BG_TYPE_TEXT);

    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_0);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, app->heapID);
}

static void ov56_021E660C(MailViewerApp *app) {
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_0);
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    Heap_Free(app->bgConfig);
}

static void ov56_021E6650(MailViewerApp *app) {
    NNSG2dCharacterData *charData;
    NNSG2dPaletteData *plttData;
    int plttNarcIndex = app->args->mailType;
    int screenDataNarcIndex = plttNarcIndex + 0x18;
    NARC *narc = NARC_New(NARC_a_0_7_9, app->heapID);

    LoadUserFrameGfx1(app->bgConfig, GF_BG_LYR_MAIN_0, 1, 4, 0, app->heapID);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_MAIN_0, 10, 6, app->frame, app->heapID);

    int tilesNarcIndex = plttNarcIndex + 0xC;
    u32 size = NARC_GetMemberSize(narc, tilesNarcIndex);
    void *narcMemberBuffer = Heap_AllocAtEnd(app->heapID, size);
    NARC_ReadWholeMember(narc, tilesNarcIndex, narcMemberBuffer);

    NNS_G2dGetUnpackedCharacterData(narcMemberBuffer, &charData);
    BG_LoadCharTilesData(app->bgConfig, GF_BG_LYR_MAIN_1, charData->pRawData, charData->szByte, 0);
    BG_LoadCharTilesData(app->bgConfig, GF_BG_LYR_SUB_0, charData->pRawData, charData->szByte, 0);
    Heap_Free(narcMemberBuffer);

    size = NARC_GetMemberSize(narc, plttNarcIndex);
    narcMemberBuffer = Heap_AllocAtEnd(app->heapID, size);
    NARC_ReadWholeMember(narc, plttNarcIndex, narcMemberBuffer);

    NNS_G2dGetUnpackedPaletteData(narcMemberBuffer, &plttData);
    BG_LoadPlttData(GF_BG_LYR_SUB_0, plttData->pRawData, plttData->szByte, 0);

    app->paletteData = PaletteData_Init(app->heapID);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_BG, 0x20 * 7, app->heapID);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_OBJ, 0x20 * 3, app->heapID);
    PaletteData_LoadPalette(app->paletteData, plttData->pRawData, PLTTBUF_MAIN_BG, 0, 0x20 * 3);

    if (app->mode == MAIL_VIEWER_MODE_WRITE) {
        PaletteData_LoadPalette(app->paletteData, &((u16 *)plttData->pRawData)[0x30], PLTTBUF_MAIN_BG, 0x10, 0x20);
    }

    PaletteData_LoadNarc(app->paletteData, NARC_poketool_icongra_poke_icon, 0, app->heapID, PLTTBUF_MAIN_OBJ, 0x20 * 3, 0);
    PaletteData_LoadNarc(app->paletteData, NARC_graphic_font, 7, app->heapID, PLTTBUF_MAIN_BG, 0x20, 0x30);
    PaletteData_LoadNarc(app->paletteData, NARC_graphic_font, 8, app->heapID, PLTTBUF_MAIN_BG, 0x20, 0x50);
    PaletteData_LoadNarc(app->paletteData, NARC_a_0_3_8, 25, app->heapID, PLTTBUF_MAIN_BG, 0x20, 0x40);
    PaletteData_LoadNarc(app->paletteData, NARC_a_0_3_8, 26 + app->frame, app->heapID, PLTTBUF_MAIN_BG, 0x20, 0x60);
    PaletteData_BlendPalette(app->paletteData, PLTTBUF_MAIN_BG, 0, 16 * 7, 16, RGB_BLACK);
    PaletteData_BlendPalette(app->paletteData, PLTTBUF_MAIN_OBJ, 0, 16 * 3, 16, RGB_BLACK);
    PaletteData_SetAutoTransparent(app->paletteData, TRUE);
    PaletteData_PushTransparentBuffers(app->paletteData);
    Heap_Free(narcMemberBuffer);

    size = NARC_GetMemberSize(narc, screenDataNarcIndex);
    app->backgroundNSCRBuffer = Heap_Alloc(app->heapID, size);
    NARC_ReadWholeMember(narc, screenDataNarcIndex, app->backgroundNSCRBuffer);
    NNS_G2dGetUnpackedScreenData(app->backgroundNSCRBuffer, &app->backgroundScreenData);

    size = NARC_GetMemberSize(narc, 0x24);
    app->interfaceNSCRBuffer = Heap_Alloc(app->heapID, size);
    NARC_ReadWholeMember(narc, 0x24, app->interfaceNSCRBuffer);
    NNS_G2dGetUnpackedScreenData(app->interfaceNSCRBuffer, &app->interfaceScreenData);

    NARC_Delete(narc);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_SUB_0, 0x2001, 0, 0, 32, 32, TILEMAP_FILL_OVWT_PAL);
    CopyToBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 32, 24, app->backgroundScreenData->rawData, 0, 0, app->backgroundScreenData->screenWidth / 8, app->backgroundScreenData->screenHeight / 8);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_0);

    if (app->mode != MAIL_VIEWER_MODE_READ) {
        CopyToBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 32, 24, app->interfaceScreenData->rawData, 0, 0, app->interfaceScreenData->screenWidth / 8, app->interfaceScreenData->screenHeight / 8);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_2);
        app->blendFraction = 0;
        app->blendDirection = 0;
        SysTask_CreateOnMainQueue(ov56_021E63FC, app, 0);
        app->onSwitchToKeys = ov56_021E5D40;
        app->onSwitchToTouch = ov56_021E5D34;
    }
}

static void ov56_021E692C(MailViewerApp *app) {
    Heap_Free(app->interfaceNSCRBuffer);
    Heap_Free(app->backgroundNSCRBuffer);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(app->paletteData);
    app->paletteData = NULL;
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_NONE, 31, 0);
}

static void ov56_021E696C(MailViewerApp *app) {
    int i;

    AddWindowParameterized(app->bgConfig, &app->windows[MAIL_VIEWER_WINDOW_SENTENCE_1], GF_BG_LYR_MAIN_1, 3, 3, 26, 4, 1, 0x397);
    AddWindowParameterized(app->bgConfig, &app->windows[MAIL_VIEWER_WINDOW_SENTENCE_2], GF_BG_LYR_MAIN_1, 3, 8, 26, 4, 1, 0x32F);
    AddWindowParameterized(app->bgConfig, &app->windows[MAIL_VIEWER_WINDOW_SENTENCE_3], GF_BG_LYR_MAIN_1, 3, 13, 26, 4, 1, 0x2C7);
    AddWindowParameterized(app->bgConfig, &app->windows[MAIL_VIEWER_WINDOW_CONFIRM], GF_BG_LYR_MAIN_1, 3, 20, 8, 2, 1, 0x2B7);
    AddWindowParameterized(app->bgConfig, &app->windows[MAIL_VIEWER_WINDOW_CANCEL], GF_BG_LYR_MAIN_1, 21, 20, 8, 2, 1, 0x2A7);
    AddWindowParameterized(app->bgConfig, &app->windows[MAIL_VIEWER_WINDOW_MESSAGE], GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 5, 0x23B);

    for (i = 0; i < MAIL_VIEWER_WINDOW_COUNT; i++) {
        FillWindowPixelBuffer(&app->windows[i], 0);
        if (i < MAIL_VIEWER_WINDOW_MESSAGE) {
            CopyWindowToVram(&app->windows[i]);
        }
    }

    app->yesNoPrompt = YesNoPrompt_Create(app->heapID);
}

static void ov56_021E6A7C(MailViewerApp *app) {
    int i;

    YesNoPrompt_Destroy(app->yesNoPrompt);
    for (i = 0; i < MAIL_VIEWER_WINDOW_COUNT; i++) {
        ClearWindowTilemapAndCopyToVram(&app->windows[i]);
        RemoveWindow(&app->windows[i]);
    }
}

static void ov56_021E6AA4(MailViewerApp *app) {
    int i;

    for (i = 0; i < 3; i++) {
        if (!MailMsg_IsInit(&app->args->mailMessages[i])) {
            continue;
        }

        String *sentenceStr = MailMsg_GetExpandedString(&app->args->mailMessages[i], app->heapID);

        AddTextPrinterParameterizedWithColor(&app->windows[i], 1, sentenceStr, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(sentenceStr);
        CopyWindowToVram(&app->windows[i]);
    }

    if (app->mode == MAIL_VIEWER_MODE_WRITE) {
        app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0233_bin, app->heapID);

        String *string = String_New(8 * 2, app->heapID);

        for (i = 0; i < 2; i++) {
            String_SetEmpty(string);
            ReadMsgDataIntoString(app->msgData, i, string);

            int xOffset = 8 * 8 - FontID_String_GetWidth(1, string, 0);

            AddTextPrinterParameterizedWithColor(&app->windows[MAIL_VIEWER_WINDOW_CONFIRM + i], 1, string, xOffset / 2, 2, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
            CopyWindowToVram(&app->windows[MAIL_VIEWER_WINDOW_CONFIRM + i]);
        }

        String_Delete(string);
    } else {
        AddTextPrinterParameterizedWithColor(&app->windows[MAIL_VIEWER_WINDOW_CANCEL], 1, app->args->mailAuthorName, 0, 1, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        CopyWindowToVram(&app->windows[MAIL_VIEWER_WINDOW_CANCEL]);
    }
}

static void ov56_021E6BB4(MailViewerApp *app) {
    ManagedSpriteTemplate spriteTemplate;
    int i;

    sub_0203A964();

    if (app->mode == MAIL_VIEWER_MODE_WRITE) {
        return;
    }

    GF_CreateVramTransferManager(32, app->heapID);

    app->spriteSys = SpriteSystem_Alloc(app->heapID);
    app->spriteMan = SpriteManager_New(app->spriteSys);

    OamManagerParam oamManager = {
        .fromOBJmain = 0,
        .numOBJmain = 7,
        .fromAffineMain = 1,
        .numAffineMain = 1,
        .fromOBJsub = 0,
        .numOBJsub = 1,
        .fromAffineSub = 1,
        .numAffineSub = 1,
    };

    OamCharTransferParam transferParam = {
        .maxTasks = 3,
        .sizeMain = 1024,
        .sizeSub = 0,
        .charModeMain = GX_OBJVRAMMODE_CHAR_1D_32K,
        .charModeSub = GX_OBJVRAMMODE_CHAR_1D_32K,
    };

    const SpriteResourceCountsListUnion capacities = {
        .numChar = 3,
        .numPltt = 1,
        .numCell = 1,
        .numAnim = 1,
        .numMcel = 0,
        .numManm = 0,
    };

    SpriteSystem_Init(app->spriteSys, &oamManager, &transferParam, 32);
    SpriteSystem_InitSprites(app->spriteSys, app->spriteMan, 3);
    SpriteSystem_InitManagerWithCapacities(app->spriteSys, app->spriteMan, (SpriteResourceCountsListUnion *)&capacities);
    thunk_ClearMainOAM(app->heapID);

    SpriteSystem_LoadPlttResObj(app->spriteSys, app->spriteMan, NARC_poketool_icongra_poke_icon, sub_02074490(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObj(app->spriteSys, app->spriteMan, NARC_poketool_icongra_poke_icon, sub_02074494(), FALSE, 0);
    SpriteSystem_LoadAnimResObj(app->spriteSys, app->spriteMan, NARC_poketool_icongra_poke_icon, sub_020744A0(), FALSE, 0);

    for (i = 0; i < 3; i++) {
        if (((MailIconData *)app->args->unk18)[i].asValue == 0xFFFF) {
            break;
        }

        SpriteSystem_LoadCharResObjWithHardwareMappingType(app->spriteSys, app->spriteMan, NARC_poketool_icongra_poke_icon, ((MailIconData *)app->args->unk18)[i].asStruct.spriteIndex, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, i);
        MI_CpuFill8(&spriteTemplate, 0, sizeof(ManagedSpriteTemplate));

        spriteTemplate.x = 128 - 40 * i;
        spriteTemplate.y = 160;
        spriteTemplate.z = 0;
        spriteTemplate.animation = 0;
        spriteTemplate.bgPriority = 2;
        spriteTemplate.pal = ((MailIconData *)app->args->unk18)[i].asStruct.palIndex;
        spriteTemplate.vramTransfer = FALSE;
        spriteTemplate.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_CHAR] = i;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_PLTT] = 0;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_CELL] = 0;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_ANIM] = 0;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_MANM] = -1;

        app->managedSprites[i] = SpriteSystem_NewSprite(app->spriteSys, app->spriteMan, &spriteTemplate);

        if (((MailIconData *)app->args->unk18)[i].asStruct.spriteIndex == 7) {
            ManagedSprite_SetDrawFlag(app->managedSprites[i], FALSE);
        }
    }
}

static void ov56_021E6D90(MailViewerApp *app) {
    int i;

    if (app->mode == MAIL_VIEWER_MODE_READ) {
        for (i = 0; i < 3; i++) {
            if (app->managedSprites[i] != NULL) {
                Sprite_DeleteAndFreeResources(app->managedSprites[i]);
            }
        }
        SpriteSystem_FreeResourcesAndManager(app->spriteSys, app->spriteMan);
        SpriteSystem_Free(app->spriteSys);
        GF_DestroyVramTransferManager();
    }
}

static const TouchscreenHitbox ov56_021E6E20[] = {
    { .rect = { 0x18, 0x38, 0x18, 0xE8 } },
    { .rect = { 0x40, 0x60, 0x18, 0xE8 } },
    { .rect = { 0x68, 0x88, 0x18, 0xE8 } },
    { .rect = { 0x94, 0xAC, 0xA0, 0xE0 } },
    { .rect = { 0x94, 0xAC, 0x18, 0x58 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END, 0, 0, 0 } },
};
