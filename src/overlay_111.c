// The original TU saw sub_0200CE7C() with a full-width glyphId parameter
// (ov111_021E69A0 passes it without truncation), so hide the header prototype
// and re-declare it below.
#define sub_0200CE7C sub_0200CE7C_
#include "overlay_111.h"

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_0200CE7C.h"
#include "unk_0208805C.h"
#include "yes_no_prompt.h"

#undef sub_0200CE7C

void sub_0200CE7C(MessagePrinter *msgPrinter, u32 glyphId, u32 num, u32 ndigits, PrintingMode mode, Window *window, u32 x, u32 y);

typedef struct UnkOv111WindowTemplate {
    s16 dx;
    s16 dy;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile;
} UnkOv111WindowTemplate;

typedef struct UnkOv111MsgBox {
    BgConfig *bgConfig;       // 0x00
    MessagePrinter *printer;  // 0x04
    MessageFormat *msgFormat; // 0x08
    MsgData *msgData;         // 0x0C
    String *string;           // 0x10
    Options *options;         // 0x14
    enum HeapID heapID;       // 0x18
    Window window;            // 0x1C
    int printerId;            // 0x2C
    u32 delay : 16;           // 0x30
    u32 bgId : 3;
    u32 skipDelay : 1;
    u32 unused : 12;
} UnkOv111MsgBox; // size: 0x34

typedef struct UnkOv111MonPanel {
    Pokemon *mon;              // 0x00
    ManagedSprite *sprites[2]; // 0x04
    Window windows[6];         // 0x0C
    int moving;                // 0x6C
    s16 targetY;               // 0x70
    s16 x;                     // 0x72
    s16 y;                     // 0x74
} UnkOv111MonPanel;            // size: 0x78

typedef struct UnkOv111MonDisplay {
    enum HeapID heapID;           // 0x00
    BgConfig *bgConfig;           // 0x04
    UnkOv111MsgBox *msgBox;       // 0x08
    SpriteSystem *spriteSystem;   // 0x0C
    SpriteManager *spriteManager; // 0x10
    s8 iconPlttIdx;               // 0x14
    UnkOv111MonPanel *panels[2];  // 0x18
} UnkOv111MonDisplay;             // size: 0x20

typedef struct BugContestSwapMonData {
    enum HeapID heapID;             // 0x00
    BugContestSwapMonArgs *args;    // 0x04
    BgConfig *bgConfig;             // 0x08
    SpriteSystem *spriteSystem;     // 0x0C
    SpriteManager *spriteManager;   // 0x10
    ManagedSprite *arrowSprite;     // 0x14
    YesNoPrompt *yesNoPrompt;       // 0x18
    NARC *narc;                     // 0x1C
    UnkOv111MonDisplay *monDisplay; // 0x20
    UnkOv111MsgBox *msgBox;         // 0x24
    s8 iconPlttIdx;                 // 0x28
    int mode;                       // 0x2C
    int state;                      // 0x30
} BugContestSwapMonData;            // size: 0x34

static BOOL ov111_021E59E4(BugContestSwapMonData *data);
static BOOL ov111_021E5AA0(BugContestSwapMonData *data);
static void ov111_021E5BE4(BugContestSwapMonData *data);
static void ov111_021E5C54(BugContestSwapMonData *data);
static int ov111_021E5C94(u8 noPokemonCaught);
static void ov111_021E5CB4(void);
static void ov111_021E5CD4(void);
static void ov111_021E5D08(BugContestSwapMonData *data, int choice);
static void ov111_021E5D2C(Window *window, u16 hp, u16 maxHp);
static void ov111_021E5DF0(void *arg);
static void ov111_021E5E34(OverlayManager *man);
static void ov111_021E5F04(OverlayManager *man);
static void ov111_021E5F50(BugContestSwapMonData *data);
static void ov111_021E5FD4(BugContestSwapMonData *data);
static void ov111_021E6000(BugContestSwapMonData *data);
static void ov111_021E60D4(BugContestSwapMonData *data);
static void ov111_021E6170(SpriteSystem *spriteSystem, SpriteManager *spriteManager);
static void ov111_021E6180(BugContestSwapMonData *data);
static void ov111_021E6268(ManagedSprite *sprite, Pokemon *mon, s8 plttIdx, enum HeapID heapID);
static void ov111_021E62E0(ManagedSprite *sprite, void *data, u32 size);
static ManagedSprite *ov111_021E6330(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 animation, u8 drawPriority);
static ManagedSprite *ov111_021E6380(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 index);
static UnkOv111MonPanel *ov111_021E63D0(UnkOv111MonDisplay *display, u16 x, u16 y, int index, Pokemon *mon);
static void ov111_021E64C8(UnkOv111MonPanel *panel, UnkOv111MsgBox *msgBox);
static void ov111_021E65CC(UnkOv111MonPanel *panel, int moving, s16 targetY);
static void ov111_021E65D4(UnkOv111MonPanel *panel);
static BOOL ov111_021E6684(UnkOv111MonPanel *panel);
static void ov111_021E6694(UnkOv111MonPanel *panel);
static UnkOv111MonDisplay *ov111_021E66DC(enum HeapID heapID, BgConfig *bgConfig, UnkOv111MsgBox *msgBox, SpriteSystem *spriteSystem, SpriteManager *spriteManager, s8 iconPlttIdx);
static void ov111_021E6710(UnkOv111MonDisplay *display);
static void ov111_021E6738(UnkOv111MonDisplay *display, int index, u16 x, u16 y, Pokemon *mon);
static void ov111_021E6770(UnkOv111MonDisplay *display, int index, int moving, s16 targetY);
static void ov111_021E6784(UnkOv111MonDisplay *display);
static BOOL ov111_021E67A4(UnkOv111MonDisplay *display);
static UnkOv111MsgBox *ov111_021E67C4(enum HeapID heapID);
static void ov111_021E67EC(UnkOv111MsgBox *msgBox, BgConfig *bgConfig, u8 bgId, Options *options);
static void ov111_021E685C(UnkOv111MsgBox *msgBox);
static BOOL ov111_021E6888(UnkOv111MsgBox *msgBox);
static void ov111_021E68FC(UnkOv111MsgBox *msgBox, int msgId);
static void ov111_021E6934(UnkOv111MsgBox *msgBox, Window *window, int msgId, u8 x, u8 y);
static void ov111_021E696C(UnkOv111MsgBox *msgBox, Window *window, int msgId, u8 x, u8 y, u32 color);
static void ov111_021E69A0(UnkOv111MsgBox *msgBox, Window *window, u32 num, u32 ndigits, BOOL useGlyph, int glyphId);
static void ov111_021E69F4(UnkOv111MsgBox *msgBox);
static MessageFormat *ov111_021E6A2C(UnkOv111MsgBox *msgBox);
static void ov111_021E6A44(BgConfig *bgConfig, u8 bgId, enum HeapID heapID);
static u8 ov111_021E6A74(BgConfig *bgConfig, Window *window, String **outString, MessageFormat *msgFormat, MsgData *msgData, int msgId, u8 bgId, enum HeapID heapID, Options *options);
static void ov111_021E6B30(BgConfig *bgConfig, Window *window, MessageFormat *msgFormat, MsgData *msgData, int msgId, u8 bgId, u8 x, u8 y, u32 color, enum HeapID heapID);

static const u8 _021E6B74[] = { GF_BG_LYR_MAIN_0, GF_BG_LYR_MAIN_1, GF_BG_LYR_MAIN_2, GF_BG_LYR_MAIN_3, GF_BG_LYR_SUB_0 };

static const UnkOv111WindowTemplate ov111_021E6C68[] = {
    { -2, -2, 8, 2, 15, 0x1C },
    { 6,  -2, 1, 2, 15, 0x2C },
    { -8, 1,  5, 1, 15, 0x2E },
    { -2, 1,  3, 1, 15, 0x33 },
    { 1,  1,  5, 1, 15, 0x36 },
    { 0,  0,  6, 1, 15, 0x3B },
};

static BOOL (*_021E6D40[])(BugContestSwapMonData *data) = {
    ov111_021E59E4,
    ov111_021E5AA0,
};

BOOL BugContestSwapMon_Init(OverlayManager *man, int *state) {
    BugContestSwapMonData *data;

    switch (*state) {
    case 0:
        ov111_021E5E34(man);
        (*state)++;
        break;
    case 1:
        data = OverlayManager_GetData(man);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, data->heapID);
        (*state)++;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            *state = 0;
            return TRUE;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    return FALSE;
}

BOOL BugContestSwapMon_Exit(OverlayManager *man, int *state) {
    BugContestSwapMonData *data = OverlayManager_GetData(man);

    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, data->heapID);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            ov111_021E5F04(man);
            return TRUE;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    return FALSE;
}

BOOL BugContestSwapMon_Main(OverlayManager *man, int *state) {
    BugContestSwapMonData *data = OverlayManager_GetData(man);

    GF_ASSERT(data != NULL);

    if (_021E6D40[data->mode](data)) {
        return TRUE;
    }

    return FALSE;
}

static BOOL ov111_021E59E4(BugContestSwapMonData *data) {
    int msgId = -1;
    MessageFormat *msgFormat = ov111_021E6A2C(data->msgBox);

    if (ov111_021E6888(data->msgBox)) {
        return FALSE;
    }

    switch (data->state) {
    case 0: {
        Pokemon *mon = data->args->newlyCaughtMon;
        ov111_021E6738(data->monDisplay, 0, 0x80, 0x48, mon);
        BufferBoxMonSpeciesName(msgFormat, 0, Mon_GetBoxMon(mon));
        msgId = 0;
        data->state = 2;
        break;
    }
    case 1:
        if (!ov111_021E6888(data->msgBox)) {
            data->state = 2;
        }
        break;
    case 2:
        if (System_GetTouchNew() || (gSystem.newKeys & PAD_BUTTON_A)) {
            PlaySE(SEQ_SE_DP_SELECT);
            data->state = 3;
        }
        break;
    case 3:
        return TRUE;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    if (msgId != -1) {
        ov111_021E68FC(data->msgBox, msgId);
    }

    return FALSE;
}

static BOOL ov111_021E5AA0(BugContestSwapMonData *data) {
    int msgId = -1;

    ov111_021E6A2C(data->msgBox);

    if (ov111_021E6888(data->msgBox)) {
        return FALSE;
    }

    switch (data->state) {
    case 0:
        msgId = 1;
        ov111_021E5BE4(data);
        data->state = 1;
        break;
    case 1:
        ov111_021E5C54(data);
        data->state = 2;
        break;
    case 2:
        switch (YesNoPrompt_HandleInput(data->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(data->yesNoPrompt);
            ManagedSprite_SetDrawFlag(data->arrowSprite, FALSE);
            ov111_021E6770(data->monDisplay, 0, 1, 0x48);
            ov111_021E6770(data->monDisplay, 1, 1, 0xF2);
            ov111_021E5D08(data, 1);
            data->state = 3;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(data->yesNoPrompt);
            ManagedSprite_SetDrawFlag(data->arrowSprite, FALSE);
            ov111_021E6770(data->monDisplay, 0, 1, -0x32);
            ov111_021E6770(data->monDisplay, 1, 1, 0x48);
            ov111_021E5D08(data, 2);
            data->state = 4;
            break;
        }
        break;
    case 3:
        ov111_021E6784(data->monDisplay);
        if (ov111_021E67A4(data->monDisplay)) {
            msgId = 2;
            data->state = 5;
        }
        break;
    case 4:
        ov111_021E6784(data->monDisplay);
        if (ov111_021E67A4(data->monDisplay)) {
            msgId = 3;
            data->state = 5;
        }
        break;
    case 5:
        if (System_GetTouchNew() || (gSystem.newKeys & PAD_BUTTON_A)) {
            PlaySE(SEQ_SE_DP_SELECT);
            data->state = 6;
        }
        break;
    case 6:
        return TRUE;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    if (msgId != -1) {
        ov111_021E68FC(data->msgBox, msgId);
    }

    return FALSE;
}

static void ov111_021E5BE4(BugContestSwapMonData *data) {
    MessageFormat *msgFormat = ov111_021E6A2C(data->msgBox);
    Pokemon *newMon = data->args->newlyCaughtMon;
    Pokemon *curMon = data->args->currentMon;

    ov111_021E6738(data->monDisplay, 0, 0x80, 0x20, newMon);
    ov111_021E6738(data->monDisplay, 1, 0x80, 0x70, curMon);
    BufferBoxMonSpeciesName(msgFormat, 0, Mon_GetBoxMon(newMon));
    BufferBoxMonSpeciesName(msgFormat, 1, Mon_GetBoxMon(curMon));
    data->arrowSprite = ov111_021E6330(data->spriteSystem, data->spriteManager, 0x80, 0x48, 1, 0);
    ManagedSprite_SetPaletteOverride(data->arrowSprite, 1);
}

static void ov111_021E5C54(BugContestSwapMonData *data) {
    YesNoPromptTemplate template = { 0 };

    template.bgConfig = data->bgConfig;
    template.bgId = 1;
    template.tileStart = 0xC8;
    template.plttSlot = 6;
    template.x = 25;
    template.y = 10;
    template.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplate(data->yesNoPrompt, &template);
}

static int ov111_021E5C94(u8 noPokemonCaught) {
    switch (noPokemonCaught) {
    case FALSE:
        return 1;
    case TRUE:
        return 0;
    default:
        GF_ASSERT(FALSE);
        GF_ASSERT(FALSE);
        return 0;
    }
}

static void ov111_021E5CB4(void) {
    GraphicsBanks banks = {
        GX_VRAM_BG_128_B,
        GX_VRAM_BGEXTPLTT_NONE,
        GX_VRAM_SUB_BG_128_C,
        GX_VRAM_SUB_BGEXTPLTT_NONE,
        GX_VRAM_OBJ_128_A,
        GX_VRAM_OBJEXTPLTT_NONE,
        GX_VRAM_SUB_OBJ_16_I,
        GX_VRAM_SUB_OBJEXTPLTT_NONE,
        GX_VRAM_TEX_NONE,
        GX_VRAM_TEXPLTT_NONE,
    };

    GfGfx_SetBanks(&banks);
}

static void ov111_021E5CD4(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

static void ov111_021E5D08(BugContestSwapMonData *data, int choice) {
    switch (choice) {
    case 1:
        data->args->unk10 = data->args->newlyCaughtMon;
        break;
    case 2:
        data->args->unk10 = data->args->currentMon;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

static void ov111_021E5D2C(Window *window, u16 hp, u16 maxHp) {
    u8 i;
    u8 pixels;
    u16 tile;
    u16 fullTile;
    u16 baseTile;
    BgConfig *bgConfig;
    u8 bgId;
    u8 y;
    u8 x;

    bgConfig = GetWindowBgConfig(window);
    bgId = GetWindowBgId(window);
    x = GetWindowX(window);
    y = GetWindowY(window);

    pixels = CalculateHpBarPixelsLength(hp, maxHp, 48);
    switch (CalculateHpBarColor(hp, maxHp, 48)) {
    case 0:
    case 3:
    case 4:
        baseTile = 1;
        break;
    case 2:
        baseTile = 10;
        break;
    case 1:
        baseTile = 19;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    for (i = 0, fullTile = baseTile + 8; i < 6; i++) {
        if (pixels >= 8) {
            tile = fullTile;
        } else {
            tile = baseTile + pixels;
        }
        FillBgTilemapRect(bgConfig, bgId, tile, x + i, y, 1, 1, TILEMAP_FILL_OVWT_PAL);
        if (pixels < 8) {
            pixels = 0;
        } else {
            pixels -= 8;
        }
    }

    ScheduleBgTilemapBufferTransfer(bgConfig, bgId);
}

static void ov111_021E5DF0(void *arg) {
    BugContestSwapMonData *data = arg;

    GF_ASSERT(data != NULL);
    GF_ASSERT(data->spriteManager != NULL);
    GF_ASSERT(data->bgConfig != NULL);

    SpriteSystem_DrawSprites(data->spriteManager);
    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(data->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

static void ov111_021E5E34(OverlayManager *man) {
    BugContestSwapMonArgs *args = OverlayManager_GetArgs(man);
    BugContestSwapMonData *data;

    GF_ASSERT(args != NULL);

    Heap_Create(HEAP_ID_3, HEAP_ID_148, 0x30000);
    data = OverlayManager_CreateAndGetData(man, sizeof(BugContestSwapMonData), HEAP_ID_148);
    MI_CpuFill8(data, 0, sizeof(BugContestSwapMonData));
    data->heapID = HEAP_ID_148;
    data->args = args;
    data->mode = ov111_021E5C94(args->noPokemonCaught);

    ov111_021E5CD4();
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    ov111_021E5CB4();
    ov111_021E5F50(data);
    ov111_021E6000(data);
    ov111_021E60D4(data);
    ov111_021E6180(data);

    data->yesNoPrompt = YesNoPrompt_Create(data->heapID);
    data->msgBox = ov111_021E67C4(data->heapID);
    ov111_021E67EC(data->msgBox, data->bgConfig, 1, args->options);
    data->monDisplay = ov111_021E66DC(data->heapID, data->bgConfig, data->msgBox, data->spriteSystem, data->spriteManager, data->iconPlttIdx);

    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    Main_SetVBlankIntrCB(ov111_021E5DF0, data);
}

static void ov111_021E5F04(OverlayManager *man) {
    BugContestSwapMonData *data = OverlayManager_GetData(man);

    YesNoPrompt_Destroy(data->yesNoPrompt);
    ov111_021E6710(data->monDisplay);
    ov111_021E685C(data->msgBox);
    if (data->arrowSprite != NULL) {
        Sprite_DeleteAndFreeResources(data->arrowSprite);
        data->arrowSprite = NULL;
    }
    ov111_021E6170(data->spriteSystem, data->spriteManager);
    ov111_021E5FD4(data);
    ov111_021E5CD4();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_148);
}

static void ov111_021E5F50(BugContestSwapMonData *data) {
    int i;

    data->bgConfig = BgConfig_Alloc(data->heapID);

    {
        GraphicsModes modes = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };
        SetBothScreensModesAndDisable(&modes);
    }

    {
        BgTemplate templates[] = {
            { 0, 0, 0x800, 0, GF_BG_SCR_SIZE_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01, 3, 0, 0, FALSE },
            { 0, 0, 0x800, 0, GF_BG_SCR_SIZE_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01, 0, 0, 0, FALSE },
            { 0, 0, 0x800, 0, GF_BG_SCR_SIZE_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01, 1, 0, 0, FALSE },
            { 0, 0, 0x800, 0, GF_BG_SCR_SIZE_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01, 1, 0, 0, FALSE },
            { 0, 0, 0x800, 0, GF_BG_SCR_SIZE_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01, 2, 0, 0, FALSE },
        };

        for (i = 0; i < 5; i++) {
            InitBgFromTemplate(data->bgConfig, (GFBgLayer)_021E6B74[i], &templates[i], GF_BG_TYPE_TEXT);
            BgClearTilemapBufferAndCommit(data->bgConfig, (GFBgLayer)_021E6B74[i]);
            BG_FillCharDataRange(data->bgConfig, (GFBgLayer)_021E6B74[i], 0, 1, 0);
        }
    }

    data->narc = NARC_New(NARC_a_1_7_3, data->heapID);
}

static void ov111_021E5FD4(BugContestSwapMonData *data) {
    int i;

    NARC_Delete(data->narc);
    for (i = 0; i < 5; i++) {
        FreeBgTilemapBuffer(data->bgConfig, (GFBgLayer)_021E6B74[i]);
    }
    Heap_Free(data->bgConfig);
}

static void ov111_021E6000(BugContestSwapMonData *data) {
    GF_ASSERT(data->bgConfig != NULL);

    GfGfxLoader_GXLoadPalFromOpenNarc(data->narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x200, data->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(data->narc, 1, data->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, data->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(data->narc, 2, data->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, data->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(data->narc, 4, data->bgConfig, GF_BG_LYR_MAIN_2, 1, 0, TRUE, data->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(data->narc, 4, data->bgConfig, GF_BG_LYR_MAIN_3, 1, 0, TRUE, data->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(data->narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x200, data->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(data->narc, 1, data->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, data->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(data->narc, 3, data->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, data->heapID);
}

static void ov111_021E60D4(BugContestSwapMonData *data) {
    data->spriteSystem = SpriteSystem_Alloc(data->heapID);
    data->spriteManager = SpriteManager_New(data->spriteSystem);

    {
        OamManagerParam oamManagerParam = { 0, 128, 0, 32, 0, 128, 0, 32 };
        OamCharTransferParam oamTransferParam = { 0, 0x20000, 0x4000, GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_64K };

        oamTransferParam.maxTasks = 32;
        SpriteSystem_Init(data->spriteSystem, &oamManagerParam, &oamTransferParam, 32);
        SpriteSystem_InitSprites(data->spriteSystem, data->spriteManager, 32);
        SpriteResourceCountsListUnion counts = { 3, 2, 2, 2, 0, 0 };
        SpriteSystem_InitManagerWithCapacities(data->spriteSystem, data->spriteManager, &counts);
    }

    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(data->spriteSystem), 0, FX32_CONST(524));
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void ov111_021E6170(SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
    SpriteSystem_FreeResourcesAndManager(spriteSystem, spriteManager);
    SpriteSystem_Free(spriteSystem);
}

static void ov111_021E6180(BugContestSwapMonData *data) {
    int i;
    SpriteSystem *spriteSystem = data->spriteSystem;
    SpriteManager *spriteManager = data->spriteManager;
    NARC *narc;

    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, data->narc, 5, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, data->narc, 6, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, data->narc, 7, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, data->narc, 8, TRUE, 0);

    narc = NARC_New(NARC_poketool_icongra_poke_icon, data->heapID);
    for (i = 0; i < 2; i++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, data->narc, 9, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, i + 1);
    }
    data->iconPlttIdx = SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, narc, sub_02074490(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, sub_0207449C(), FALSE, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, sub_020744A8(), FALSE, 1);
    NARC_Delete(narc);
}

static void ov111_021E6268(ManagedSprite *sprite, Pokemon *mon, s8 plttIdx, enum HeapID heapID) {
    NNSG2dCharacterData *charData;
    NARC *narc = NARC_New(NARC_poketool_icongra_poke_icon, heapID);
    void *buffer = GfGfxLoader_GetCharDataFromOpenNarc(narc, Pokemon_GetIconNaix(mon), FALSE, &charData, heapID);
    u8 palette;

    ov111_021E62E0(sprite, charData->pRawData, 0x400);
    Heap_Free(buffer);
    palette = Pokemon_GetIconPalette(mon) + plttIdx;
    ManagedSprite_SetPaletteOverride(sprite, palette);
    ManagedSprite_SetAnim(sprite, 1);
    ManagedSprite_SetAnimSpeed(sprite, FX32_ONE);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    NARC_Delete(narc);
}

static void ov111_021E62E0(ManagedSprite *sprite, void *data, u32 size) {
    NNS_G2D_VRAM_TYPE vramType = Sprite_GetVramType(sprite->sprite);
    u32 location = NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprite->sprite), vramType);

    DC_FlushRange(data, size);
    switch (vramType) {
    case NNS_G2D_VRAM_TYPE_2DMAIN:
        GX_LoadOBJ(data, location, size);
        break;
    case NNS_G2D_VRAM_TYPE_2DSUB:
        GXS_LoadOBJ(data, location, size);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

static ManagedSprite *ov111_021E6330(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 animation, u8 drawPriority) {
    ManagedSprite *sprite;
    ManagedSpriteTemplate template = {
        0, 0, 0, 0, 1, 0, NNS_G2D_VRAM_TYPE_2DMAIN, { 0, 0, 0, 0, 0, 0 },
               2, 0
    };

    template.x = x;
    template.y = y;
    template.drawPriority = drawPriority;
    template.animation = animation;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, FX32_CONST(524));
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

static ManagedSprite *ov111_021E6380(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 index) {
    ManagedSpriteTemplate template = {
        0, 0, 0, 0, 0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, { 0, 1, 1, 1, 0, 0 },
               2, 0
    };

    GF_ASSERT(index < 2);
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = index + 1;
    template.x = x;
    template.y = y;
    return SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, FX32_CONST(524));
}

static UnkOv111MonPanel *ov111_021E63D0(UnkOv111MonDisplay *display, u16 x, u16 y, int index, Pokemon *mon) {
    int i;
    UnkOv111MonPanel *panel;
    u8 bgId = index == 0 ? GF_BG_LYR_MAIN_2 : GF_BG_LYR_MAIN_3;

    panel = Heap_Alloc(display->heapID, sizeof(UnkOv111MonPanel));
    const UnkOv111WindowTemplate *windowTemplate;
    u8 tileY;
    u8 tileX;

    MI_CpuFill8(panel, 0, sizeof(UnkOv111MonPanel));
    panel->mon = mon;
    panel->x = x;
    panel->y = y;
    panel->sprites[0] = ov111_021E6330(display->spriteSystem, display->spriteManager, panel->x, panel->y, 0, 1);
    panel->sprites[1] = ov111_021E6380(display->spriteSystem, display->spriteManager, panel->x - 0x2C, panel->y - 8, index);

    windowTemplate = ov111_021E6C68;
    tileY = y / 8;
    tileX = x / 8;
    for (i = 0; i < 6; i++) {
        AddWindowParameterized(display->bgConfig, &panel->windows[i], bgId, tileX + windowTemplate->dx, tileY + windowTemplate->dy, windowTemplate->width, windowTemplate->height, windowTemplate->palette, windowTemplate->baseTile);
        windowTemplate++;
    }

    return panel;
}

static void ov111_021E64C8(UnkOv111MonPanel *panel, UnkOv111MsgBox *msgBox) {
    u32 hp, maxHp;
    int msgId;
    MessageFormat *msgFormat = ov111_021E6A2C(msgBox);
    Pokemon *mon = panel->mon;
    u32 color;

    GF_ASSERT(panel != NULL);
    GF_ASSERT(msgBox != NULL);

    hp = GetMonData(mon, MON_DATA_HP, NULL);
    maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
    BufferBoxMonSpeciesName(msgFormat, 0, Mon_GetBoxMon(mon));
    ov111_021E6934(msgBox, &panel->windows[0], 4, 4, 1);

    color = MAKE_TEXT_COLOR(7, 8, 0);
    switch (GetMonGender(mon)) {
    case MON_MALE:
        msgId = 5;
        break;
    case MON_FEMALE:
        msgId = 6;
        color = MAKE_TEXT_COLOR(3, 4, 0);
        break;
    case MON_GENDERLESS:
        msgId = 7;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    ov111_021E696C(msgBox, &panel->windows[1], msgId, 0, 1, color);
    ov111_021E69A0(msgBox, &panel->windows[2], GetMonData(mon, MON_DATA_LEVEL, NULL), 3, TRUE, 1);
    ov111_021E69A0(msgBox, &panel->windows[3], hp, 3, FALSE, 0);
    ov111_021E69A0(msgBox, &panel->windows[4], maxHp, 3, TRUE, 0);
    ov111_021E5D2C(&panel->windows[5], hp, maxHp);
}

static void ov111_021E65CC(UnkOv111MonPanel *panel, int moving, s16 targetY) {
    panel->moving = moving;
    panel->targetY = targetY;
}

static void ov111_021E65D4(UnkOv111MonPanel *panel) {
    int i;
    s16 diff;
    s16 step;
    BgConfig *bgConfig;
    u8 bgId;
    s16 x, y;
    Window *window;

    GF_ASSERT(panel != NULL);

    window = &panel->windows[0];
    bgConfig = GetWindowBgConfig(window);
    bgId = GetWindowBgId(window);
    diff = (-panel->targetY + panel->y) < 0 ? -(-panel->targetY + panel->y) : (-panel->targetY + panel->y);
    step = diff >> 2;

    if (diff <= 1 || step == 0) {
        panel->moving = 0;
    } else {
        if (panel->y > panel->targetY) {
            step *= -1;
        }
        panel->y += step;
        if (panel->y < 0xE8) {
            ScheduleSetBgPosText(bgConfig, bgId, BG_POS_OP_SUB_Y, step);
            for (i = 0; i < 2; i++) {
                ManagedSprite_GetPositionXYWithSubscreenOffset(panel->sprites[i], &x, &y, FX32_CONST(524));
                y += step;
                ManagedSprite_SetPositionXYWithSubscreenOffset(panel->sprites[i], x, y, FX32_CONST(524));
            }
        }
    }

    BgCommitTilemapBufferToVram(bgConfig, bgId);
}

static BOOL ov111_021E6684(UnkOv111MonPanel *panel) {
    if (panel->moving == 0) {
        return TRUE;
    }
    return FALSE;
}

static void ov111_021E6694(UnkOv111MonPanel *panel) {
    int i;

    GF_ASSERT(panel != NULL);

    for (i = 0; i < 2; i++) {
        if (panel->sprites[i] != NULL) {
            Sprite_DeleteAndFreeResources(panel->sprites[i]);
            panel->sprites[i] = NULL;
        }
    }
    for (i = 0; i < 6; i++) {
        ClearWindowTilemapAndCopyToVram(&panel->windows[i]);
        RemoveWindow(&panel->windows[i]);
    }
    Heap_Free(panel);
}

static UnkOv111MonDisplay *ov111_021E66DC(enum HeapID heapID, BgConfig *bgConfig, UnkOv111MsgBox *msgBox, SpriteSystem *spriteSystem, SpriteManager *spriteManager, s8 iconPlttIdx) {
    UnkOv111MonDisplay *display = Heap_Alloc(heapID, sizeof(UnkOv111MonDisplay));

    MI_CpuFill8(display, 0, sizeof(UnkOv111MonDisplay));
    display->bgConfig = bgConfig;
    display->heapID = heapID;
    display->msgBox = msgBox;
    display->spriteSystem = spriteSystem;
    display->spriteManager = spriteManager;
    display->iconPlttIdx = iconPlttIdx;
    return display;
}

static void ov111_021E6710(UnkOv111MonDisplay *display) {
    int i;

    GF_ASSERT(display != NULL);

    for (i = 0; i < 2; i++) {
        if (display->panels[i] != NULL) {
            ov111_021E6694(display->panels[i]);
        }
    }
    Heap_Free(display);
}

static void ov111_021E6738(UnkOv111MonDisplay *display, int index, u16 x, u16 y, Pokemon *mon) {
    UnkOv111MonPanel *panel = ov111_021E63D0(display, x, y, index, mon);

    ov111_021E6268(panel->sprites[1], panel->mon, display->iconPlttIdx, display->heapID);
    ov111_021E64C8(panel, display->msgBox);
    display->panels[index] = panel;
}

static void ov111_021E6770(UnkOv111MonDisplay *display, int index, int moving, s16 targetY) {
    ov111_021E65CC(display->panels[index], moving, targetY);
}

static void ov111_021E6784(UnkOv111MonDisplay *display) {
    int i;

    for (i = 0; i < 2; i++) {
        if (!ov111_021E6684(display->panels[i])) {
            ov111_021E65D4(display->panels[i]);
        }
    }
}

static BOOL ov111_021E67A4(UnkOv111MonDisplay *display) {
    int i;

    for (i = 0; i < 2; i++) {
        if (!ov111_021E6684(display->panels[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

static UnkOv111MsgBox *ov111_021E67C4(enum HeapID heapID) {
    UnkOv111MsgBox *msgBox = Heap_Alloc(heapID, sizeof(UnkOv111MsgBox));

    MI_CpuFill8(msgBox, 0, sizeof(UnkOv111MsgBox));
    msgBox->heapID = heapID;
    msgBox->printer = MessagePrinter_New(1, 2, 0, heapID);
    return msgBox;
}

static void ov111_021E67EC(UnkOv111MsgBox *msgBox, BgConfig *bgConfig, u8 bgId, Options *options) {
    msgBox->bgConfig = bgConfig;
    msgBox->printerId = -1;
    msgBox->bgId = bgId;
    msgBox->options = options;
    msgBox->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1D, msgBox->heapID);
    msgBox->msgFormat = MessageFormat_New(msgBox->heapID);
    ResetAllTextPrinters();
    ov111_021E6A44(msgBox->bgConfig, msgBox->bgId, msgBox->heapID);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, msgBox->heapID);
    BG_FillCharDataRange(msgBox->bgConfig, (GFBgLayer)msgBox->bgId, 0, 1, 0);
}

static void ov111_021E685C(UnkOv111MsgBox *msgBox) {
    ov111_021E69F4(msgBox);
    DestroyMsgData(msgBox->msgData);
    MessageFormat_Delete(msgBox->msgFormat);
    if (msgBox->string != NULL) {
        String_Delete(msgBox->string);
    }
    MessagePrinter_Delete(msgBox->printer);
    Heap_Free(msgBox);
}

static BOOL ov111_021E6888(UnkOv111MsgBox *msgBox) {
    if (msgBox->printerId != -1) {
        if (!TextPrinterCheckActive(msgBox->printerId)) {
            msgBox->delay = 0;
            String_Delete(msgBox->string);
            msgBox->string = NULL;
            msgBox->printerId = -1;
        }
        return TRUE;
    }

    if (msgBox->delay != 0) {
        if (msgBox->skipDelay == TRUE) {
            msgBox->delay = 0;
            msgBox->skipDelay = FALSE;
        } else {
            msgBox->delay--;
            return TRUE;
        }
    }

    return FALSE;
}

static void ov111_021E68FC(UnkOv111MsgBox *msgBox, int msgId) {
    msgBox->printerId = ov111_021E6A74(msgBox->bgConfig, &msgBox->window, &msgBox->string, msgBox->msgFormat, msgBox->msgData, msgId, msgBox->bgId, msgBox->heapID, msgBox->options);
}

static void ov111_021E6934(UnkOv111MsgBox *msgBox, Window *window, int msgId, u8 x, u8 y) {
    ov111_021E6B30(msgBox->bgConfig, window, msgBox->msgFormat, msgBox->msgData, msgId, msgBox->bgId, x, y, MAKE_TEXT_COLOR(1, 2, 0), msgBox->heapID);
}

static void ov111_021E696C(UnkOv111MsgBox *msgBox, Window *window, int msgId, u8 x, u8 y, u32 color) {
    ov111_021E6B30(msgBox->bgConfig, window, msgBox->msgFormat, msgBox->msgData, msgId, msgBox->bgId, x, y, color, msgBox->heapID);
}

static void ov111_021E69A0(UnkOv111MsgBox *msgBox, Window *window, u32 num, u32 ndigits, BOOL useGlyph, int glyphId) {
    FillWindowPixelBuffer(window, 0);
    if (useGlyph) {
        sub_0200CE7C(msgBox->printer, glyphId, num, ndigits, PRINTING_MODE_RIGHT_ALIGN, window, 0, 0);
    } else {
        PrintUIntOnWindow(msgBox->printer, num, ndigits, PRINTING_MODE_RIGHT_ALIGN, window, 0, 0);
    }
    ScheduleWindowCopyToVram(window);
}

static void ov111_021E69F4(UnkOv111MsgBox *msgBox) {
    if (msgBox->printerId != -1) {
        RemoveTextPrinter(msgBox->printerId);
    }
    if (msgBox->window.bgConfig != NULL) {
        ClearFrameAndWindow2(&msgBox->window, FALSE);
        ClearWindowTilemapAndCopyToVram(&msgBox->window);
        RemoveWindow(&msgBox->window);
    }
}

static MessageFormat *ov111_021E6A2C(UnkOv111MsgBox *msgBox) {
    GF_ASSERT(msgBox != NULL);
    GF_ASSERT(msgBox->msgFormat != NULL);
    return msgBox->msgFormat;
}

static void ov111_021E6A44(BgConfig *bgConfig, u8 bgId, enum HeapID heapID) {
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, heapID);
    LoadUserFrameGfx2(bgConfig, (GFBgLayer)bgId, 0x3D2, 13, 0, heapID);
}

static u8 ov111_021E6A74(BgConfig *bgConfig, Window *window, String **outString, MessageFormat *msgFormat, MsgData *msgData, int msgId, u8 bgId, enum HeapID heapID, Options *options) {
    u8 printerId;
    int textSpeed;

    GF_ASSERT(bgConfig != NULL);
    GF_ASSERT(options != NULL);

    if (window->bgConfig == NULL) {
        AddWindowParameterized(bgConfig, window, bgId, 2, 19, 27, 4, 15, 1);
        LoadUserFrameGfx2(bgConfig, (GFBgLayer)GetWindowBgId(window), 0x3D2, 13, Options_GetFrame(options), heapID);
    }

    textSpeed = Options_GetTextFrameDelay(options);
    FillWindowPixelBuffer(window, 15);
    *outString = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msgId, heapID);
    printerId = AddTextPrinterParameterizedWithColor(window, 1, *outString, 0, 0, textSpeed, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    DrawFrameAndWindow2(window, FALSE, 0x3D2, 13);
    return printerId;
}

static void ov111_021E6B30(BgConfig *bgConfig, Window *window, MessageFormat *msgFormat, MsgData *msgData, int msgId, u8 bgId, u8 x, u8 y, u32 color, enum HeapID heapID) {
    String *string;

    FillWindowPixelBuffer(window, 0);
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msgId, heapID);
    AddTextPrinterParameterizedWithColor(window, 0, string, x, y, 0, color, NULL);
    String_Delete(string);
}
