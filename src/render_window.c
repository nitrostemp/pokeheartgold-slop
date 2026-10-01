#include "global.h"

#include "field/field_sprite_manager.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "pokemon.h"
#include "pokepic.h"
#include "render_text.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013FDC.h"

// render_window.h is frozen at its callers' matching-time state (sub_0200F478 is
// declared there without its parameter), so the defining TU declares its own API.
typedef struct WaitingIcon WaitingIcon;
struct PokepicManager;

void sub_0200E398(BgConfig *bgConfig, u32 layer, u32 a2, u32 a3, enum HeapID heapID);
u32 sub_0200E3D8(void);
void LoadUserFrameGfx1(BgConfig *bg_config, GFBgLayer layer, u16 baseTile, u8 palette_num, u8 frame, enum HeapID heapID);
void DrawFrameAndWindow1(Window *window, BOOL dont_copy_to_vram, u16 baseTile, u8 palette_num);
void sub_0200E5D4(Window *window, BOOL dont_copy_to_vram);
void LoadUserFrameGfx2(BgConfig *bgConfig, GFBgLayer layer, u16 baseTile, u8 paletteNum, u8 frame, enum HeapID heapID);
void DrawFrameAndWindow2(Window *window, BOOL dont_copy_to_vram, u16 baseTile, u8 palette_num);
void ClearFrameAndWindow2(Window *window, BOOL dont_copy_to_vram);
void sub_0200EB68(Window *window, int a1);
void LoadMapSignpostFrameAndGraphic(BgConfig *bgConfig, u8 bgId, u16 baseTile, u8 plttNum, u8 type, u16 map, enum HeapID heapId);
void DrawFrameAndWindow3(Window *window, BOOL dont_copy_to_vram, u16 baseTile, u8 palette_num, u8 type);
WaitingIcon *WaitingIcon_New(Window *window, u32 tileNum);
void sub_0200F450(WaitingIcon *waitingIcon);
void sub_0200F478(WaitingIcon *waitingIcon);
struct PokepicManager *DrawPokemonPicFromSpecies(BgConfig *bgConfig, u8 layer, u8 x, u8 y, u8 paletteNum, u16 baseTile, u16 species, u8 gender, enum HeapID heapID);
struct PokepicManager *DrawPokemonPicFromMon(BgConfig *bgConfig, u8 layer, u8 x, u8 y, u8 paletteNum, u16 baseTile, Pokemon *mon, enum HeapID heapID);

#define POKEMON_PREVIEW_RES_ID 89301

static const UnkStruct_02014E30 sPokemonPreviewFrame1Region = { 10, 0, 10, 10 };
static const UnkStruct_02014E30 sPokemonPreviewFrame0Region = { 0, 0, 10, 10 };
static const SpriteResourceCountsListUnion sPokemonPreviewResCounts = {
    { 1, 1, 1, 1, 0, 0 }
};
static const ManagedSpriteTemplate sPokemonPreviewSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 0,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = { POKEMON_PREVIEW_RES_ID, POKEMON_PREVIEW_RES_ID, POKEMON_PREVIEW_RES_ID, POKEMON_PREVIEW_RES_ID, 0, 0 },
    .bgPriority = 0,
    .vramTransfer = FALSE,
};

typedef struct PokemonPreview {
    FieldSpriteManager spriteManager; // 0x000
    ManagedSprite *managedSprite;     // 0x164
    BgConfig *bgConfig;               // 0x168
    u8 bgLayer;                       // 0x16C
    u8 x;                             // 0x16D
    u8 y;                             // 0x16E
    u8 state;                         // 0x16F
} PokemonPreview;                     // size: 0x170

struct WaitingIcon {
    Window *window;            // 0x000
    u8 pixels[8 * 0x80];       // 0x004
    u8 messageBoxPixels[0x80]; // 0x404
    u16 messageBoxTile;        // 0x484
    u8 counter;                // 0x486
    u8 curFrame : 7;           // 0x487
    u8 : 1;
    u8 deleteMode : 2; // 0x488
    u8 : 6;
}; // size: 0x48C

// Non-static functions missing from the frozen render_window.h
u32 sub_0200E63C(u32 a0);
u32 sub_0200E640(u32 a0);
void sub_0200E948(Window *window, u32 baseTile, u32 palette);
void sub_0200EB80(BgConfig *bgConfig, u8 bgId, u16 baseTile, u8 withTile, u8 frame, enum HeapID heapID);

// Static forward declarations
static void sub_0200E448(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile);
static void sub_0200E6B4(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile);
static void sub_0200EA24(void *srcPixels, u16 srcX, u16 srcY, u16 srcWidth, u16 srcHeight, void *destPixels, u16 destWidth, u16 destHeight, u16 destX, u16 destY, u16 width, u16 height);
static void sub_0200EA68(Window *window, u32 memberNo, u32 baseTile, int numFrames, u8 srcX, u8 srcY);
static void sub_0200EC84(BgConfig *bgConfig, u8 bgId, u16 offset, u8 type, u16 map, enum HeapID heapID);
static void sub_0200ECBC(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile);
static void sub_0200EF84(Window *window, u16 tile, u8 palette);
static void sub_0200F1D4(WaitingIcon *icon, int mode);
static void sub_0200F3D0(SysTask *task, void *data);
static void sub_0200F43C(SysTask *task, void *data);
static void sub_0200F54C(SysTask *task, void *data);
static PokemonPreview *sub_0200F5C4(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, enum HeapID heapID);
static void sub_0200F600(PokemonPreview *preview, enum HeapID heapID);
static void sub_0200F62C(PokemonPreview *preview);
static void sub_0200F684(PokemonPreview *preview, u8 x, u8 y);
static void sub_0200F6D4(PokemonPreview *preview, u16 species, u8 gender);
static void sub_0200F714(PokemonPreview *preview, Pokemon *mon);
static void sub_0200F748(PokemonPreview *preview, PokepicTemplate *pokepicTemplate);
static void sub_0200F82C(PokemonPreview *preview, u8 palette, u16 tile);
static void sub_0200F9DC(PokemonPreview *preview);

void sub_0200E398(BgConfig *bgConfig, u32 layer, u32 a2, u32 a3, enum HeapID heapID) {
    if (a3 == 0) {
        GfGfxLoader_LoadCharData(NARC_a_0_3_8, 0, bgConfig, (GFBgLayer)layer, a2, 0, FALSE, heapID);
    } else {
        GfGfxLoader_LoadCharData(NARC_a_0_3_8, 1, bgConfig, (GFBgLayer)layer, a2, 0, FALSE, heapID);
    }
}

u32 sub_0200E3D8(void) {
    return 0x19;
}

void LoadUserFrameGfx1(BgConfig *bg_config, GFBgLayer layer, u16 baseTile, u8 palette_num, u8 frame, enum HeapID heapID) {
    u32 memberNo;

    if (frame != 0) {
        memberNo = 1;
    } else {
        memberNo = 0;
    }
    GfGfxLoader_LoadCharData(NARC_a_0_3_8, memberNo, bg_config, layer, baseTile, 0, FALSE, heapID);
    if (frame == 2) {
        memberNo = 46;
    } else {
        memberNo = 25;
    }
    if ((u32)layer < GF_BG_LYR_SUB_0) {
        GfGfxLoader_GXLoadPal(NARC_a_0_3_8, memberNo, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(palette_num * 32), 32, heapID);
    } else {
        GfGfxLoader_GXLoadPal(NARC_a_0_3_8, memberNo, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)(palette_num * 32), 32, heapID);
    }
}

static void sub_0200E448(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile) {
    FillBgTilemapRect(bgConfig, bgId, tile, x - 1, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 1, x, y - 1, width, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 2, x + width, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 3, x - 1, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 5, x + width, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 6, x - 1, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 7, x, y + height, width, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 8, x + width, y + height, 1, 1, palette);
}

void DrawFrameAndWindow1(Window *window, BOOL dont_copy_to_vram, u16 baseTile, u8 palette_num) {
    sub_0200E448(window->bgConfig, GetWindowBgId(window), GetWindowX(window), GetWindowY(window), GetWindowWidth(window), GetWindowHeight(window), palette_num, baseTile);
    if (dont_copy_to_vram == FALSE) {
        CopyWindowToVram(window);
    }
}

void sub_0200E5D4(Window *window, BOOL dont_copy_to_vram) {
    FillBgTilemapRect(window->bgConfig, GetWindowBgId(window), 0, GetWindowX(window) - 1, GetWindowY(window) - 1, GetWindowWidth(window) + 2, GetWindowHeight(window) + 2, 0);
    if (dont_copy_to_vram == FALSE) {
        ClearWindowTilemapAndCopyToVram(window);
    }
}

u32 sub_0200E63C(u32 a0) {
    return a0 + 2;
}

u32 sub_0200E640(u32 a0) {
    return a0 + 0x1a;
}

void LoadUserFrameGfx2(BgConfig *bgConfig, GFBgLayer layer, u16 baseTile, u8 paletteNum, u8 frame, enum HeapID heapID) {
    GfGfxLoader_LoadCharData(NARC_a_0_3_8, sub_0200E63C(frame), bgConfig, layer, baseTile, 0, FALSE, heapID);
    if ((u32)layer < GF_BG_LYR_SUB_0) {
        GfGfxLoader_GXLoadPal(NARC_a_0_3_8, sub_0200E640(frame), GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(paletteNum * 32), 32, heapID);
    } else {
        GfGfxLoader_GXLoadPal(NARC_a_0_3_8, sub_0200E640(frame), GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)(paletteNum * 32), 32, heapID);
    }
}

static void sub_0200E6B4(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile) {
    FillBgTilemapRect(bgConfig, bgId, tile, x - 2, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 1, x - 1, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 2, x, y - 1, width, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 3, x + width, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 4, x + width + 1, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 5, x + width + 2, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 6, x - 2, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 7, x - 1, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 9, x + width, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 10, x + width + 1, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 11, x + width + 2, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 12, x - 2, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 13, x - 1, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 14, x, y + height, width, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 15, x + width, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 16, x + width + 1, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 17, x + width + 2, y + height, 1, 1, palette);
}

void sub_0200E948(Window *window, u32 baseTile, u32 palette) {
    sub_0200E6B4(window->bgConfig, GetWindowBgId(window), GetWindowX(window), GetWindowY(window), GetWindowWidth(window), GetWindowHeight(window), palette, baseTile);
}

void DrawFrameAndWindow2(Window *window, BOOL dont_copy_to_vram, u16 baseTile, u8 palette_num) {
    sub_0200E948(window, baseTile, palette_num);
    if (dont_copy_to_vram == FALSE) {
        CopyWindowToVram(window);
    }
    TextPrinter_SetDownArrowBaseTile(baseTile);
}

void ClearFrameAndWindow2(Window *window, BOOL dont_copy_to_vram) {
    FillBgTilemapRect(window->bgConfig, GetWindowBgId(window), 0, GetWindowX(window) - 2, GetWindowY(window) - 1, GetWindowWidth(window) + 5, GetWindowHeight(window) + 2, 0);
    if (dont_copy_to_vram == FALSE) {
        ClearWindowTilemapAndCopyToVram(window);
    }
}

static void sub_0200EA24(void *srcPixels, u16 srcX, u16 srcY, u16 srcWidth, u16 srcHeight, void *destPixels, u16 destWidth, u16 destHeight, u16 destX, u16 destY, u16 width, u16 height) {
    Bitmap src, dest;

    src.pixels = srcPixels;
    src.width = srcWidth;
    src.height = srcHeight;
    dest.pixels = destPixels;
    dest.width = destWidth;
    dest.height = destHeight;
    BlitBitmapRect4Bit(&src, &dest, srcX, srcY, destX, destY, width, height, 0);
}

// NONMATCHING: retail spills baseTile/numFrames before window and stores the bumped baseTile home before the outgoing arg; scheduling only
#ifdef NONMATCHING
static void sub_0200EA68(Window *window, u32 memberNo, u32 baseTile, int numFrames, u8 srcX, u8 srcY) {
    NNSG2dCharacterData *pCharData;
    u8 *srcTiles;
    void *charData;
    u32 size;
    u8 bgId;
    enum HeapID heapID;
    u8 *blit;
    u8 *bgChars;
    u8 i;

    heapID = BgConfig_GetHeapId(window->bgConfig);
    bgId = GetWindowBgId(window);
    size = numFrames * 0x80;
    blit = Heap_Alloc(heapID, size);
    bgChars = BgGetCharPtr(bgId);

    charData = GfGfxLoader_GetCharData(NARC_a_0_3_8, memberNo, FALSE, &pCharData, heapID);
    srcTiles = pCharData->pRawData;
    for (i = 0; i < numFrames; i++) {
        u32 frameOffset = i * 0x80;
        memcpy(&blit[frameOffset + 0x00], &bgChars[(baseTile + 10) * 32], 32);
        memcpy(&blit[frameOffset + 0x20], &bgChars[(baseTile + 11) * 32], 32);
        memcpy(&blit[frameOffset + 0x40], &bgChars[(baseTile + 10) * 32], 32);
        memcpy(&blit[frameOffset + 0x60], &bgChars[(baseTile + 11) * 32], 32);
    }
    sub_0200EA24(srcTiles, srcX, srcY, (u8)(16 - srcX), (u8)((16 - srcY) * numFrames), blit, (u8)(16 - srcX), (u8)((16 - srcY) * numFrames), 0, 0, (u8)(16 - srcX), (u8)((16 - srcY) * numFrames));
    baseTile += 18;
    BG_LoadCharTilesData(window->bgConfig, bgId, blit, size, baseTile);
    Heap_Free(charData);
    Heap_Free(blit);
}
#else
// clang-format off
asm static void sub_0200EA68(Window *window, u32 memberNo, u32 baseTile, int numFrames, u8 srcX, u8 srcY) {
    push {r3, r4, r5, r6, r7, lr}
    sub sp, #0x48
    str r2, [sp, #0x24]
    str r3, [sp, #0x28]
    str r0, [sp, #0x20]
    ldr r0, [r0, #0]
    add r7, r1, #0
    bl BgConfig_GetHeapId
    add r6, r0, #0
    ldr r0, [sp, #0x20]
    bl GetWindowBgId
    str r0, [sp, #0x2c]
    ldr r0, [sp, #0x28]
    lsl r0, r0, #7
    str r0, [sp, #0x30]
    ldr r1, [sp, #0x30]
    add r0, r6, #0
    bl Heap_Alloc
    add r5, r0, #0
    ldr r0, [sp, #0x2c]
    bl BgGetCharPtr
    add r4, r0, #0
    str r6, [sp]
    mov r0, #0x26
    add r1, r7, #0
    mov r2, #0
    add r3, sp, #0x44
    bl GfGfxLoader_GetCharData
    str r0, [sp, #0x34]
    ldr r0, [sp, #0x44]
    mov r7, #0
    ldr r0, [r0, #0x14]
    str r0, [sp, #0x38]
    ldr r0, [sp, #0x28]
    cmp r0, #0
    ble _0200EB12
    ldr r0, [sp, #0x24]
    add r0, #0xa
    lsl r0, r0, #5
    str r0, [sp, #0x3c]
    ldr r0, [sp, #0x24]
    add r0, #0xb
    lsl r0, r0, #5
    str r0, [sp, #0x40]
_0200EACA:
    ldr r1, [sp, #0x3c]
    lsl r6, r7, #7
    add r0, r5, r6
    add r1, r4, r1
    mov r2, #0x20
    bl memcpy
    add r0, r6, #0
    ldr r1, [sp, #0x40]
    add r0, #0x20
    add r0, r5, r0
    add r1, r4, r1
    mov r2, #0x20
    bl memcpy
    add r0, r6, #0
    ldr r1, [sp, #0x3c]
    add r0, #0x40
    add r0, r5, r0
    add r1, r4, r1
    mov r2, #0x20
    bl memcpy
    ldr r1, [sp, #0x40]
    add r6, #0x60
    add r0, r5, r6
    add r1, r4, r1
    mov r2, #0x20
    bl memcpy
    add r0, r7, #1
    lsl r0, r0, #0x18
    lsr r7, r0, #0x18
    ldr r0, [sp, #0x28]
    cmp r7, r0
    blt _0200EACA
_0200EB12:
    add r1, sp, #0x50
    ldrb r2, [r1, #0x14]
    mov r3, #0x10
    ldr r0, [sp, #0x28]
    sub r4, r3, r2
    mul r0, r4
    ldrb r1, [r1, #0x10]
    lsl r0, r0, #0x18
    lsr r0, r0, #0x18
    sub r3, r3, r1
    lsl r3, r3, #0x18
    str r0, [sp]
    lsr r3, r3, #0x18
    str r5, [sp, #4]
    str r3, [sp, #8]
    str r0, [sp, #0xc]
    mov r4, #0
    str r4, [sp, #0x10]
    str r4, [sp, #0x14]
    str r3, [sp, #0x18]
    str r0, [sp, #0x1c]
    ldr r0, [sp, #0x38]
    bl sub_0200EA24
    ldr r0, [sp, #0x24]
    ldr r3, [sp, #0x30]
    add r0, #0x12
    str r0, [sp, #0x24]
    str r0, [sp]
    ldr r0, [sp, #0x20]
    ldr r1, [sp, #0x2c]
    ldr r0, [r0, #0]
    add r2, r5, #0
    bl BG_LoadCharTilesData
    ldr r0, [sp, #0x34]
    bl Heap_Free
    add r0, r5, #0
    bl Heap_Free
    add sp, #0x48
    pop {r3, r4, r5, r6, r7, pc}
}
// clang-format on
#endif // NONMATCHING

void sub_0200EB68(Window *window, int a1) {
    sub_0200EA68(window, 22, a1, 3, 3, 0);
}

void sub_0200EB80(BgConfig *bgConfig, u8 bgId, u16 baseTile, u8 withTile, u8 frame, enum HeapID heapID) {
    void *charData;
    NNSG2dCharacterData *pCharData;
    u8 *src;
    u32 i;
    u8 hi, lo;

    charData = GfGfxLoader_GetCharData(NARC_a_0_3_8, sub_0200E63C(frame), FALSE, &pCharData, heapID);
    src = Heap_Alloc(heapID, 18 * 32);
    memcpy(src, pCharData->pRawData, 18 * 32);
    for (i = 0; i < 18 * 32; i++) {
        hi = src[i] >> 4;
        lo = src[i] & 0xF;
        if (hi == 0) {
            hi = withTile;
        }
        if (lo == 0) {
            lo = withTile;
        }
        src[i] = (hi << 4) | lo;
    }
    BG_LoadCharTilesData(bgConfig, bgId, src, 18 * 32, baseTile);
    Heap_Free(charData);
    Heap_Free(src);
}

void LoadMapSignpostFrameAndGraphic(BgConfig *bgConfig, u8 bgId, u16 baseTile, u8 plttNum, u8 type, u16 map, enum HeapID heapId) {
    void *nclr;
    NNSG2dPaletteData *plttData;

    GfGfxLoader_LoadCharData(NARC_a_0_3_6, 0, bgConfig, (GFBgLayer)bgId, baseTile, 30 * 32, FALSE, heapId);
    nclr = AllocAndReadWholeNarcMemberByIdPair(NARC_a_0_3_6, 1, heapId);
    NNS_G2dGetUnpackedPaletteData(nclr, &plttData);
    BG_LoadPlttData(bgId, (u16 *)plttData->pRawData + type * 16, 32, plttNum * 32);
    Heap_FreeExplicit(heapId, nclr);
    if (type == 0 || type == 1) {
        sub_0200EC84(bgConfig, bgId, baseTile + 30, type, map, heapId);
    }
}

static void sub_0200EC84(BgConfig *bgConfig, u8 bgId, u16 offset, u8 type, u16 map, enum HeapID heapID) {
    if (type == 0) {
        map += 33;
    } else {
        map += 2;
    }
    GfGfxLoader_LoadCharData(NARC_a_0_3_6, map, bgConfig, (GFBgLayer)bgId, offset, 24 * 32, FALSE, heapID);
}

static void sub_0200ECBC(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile) {
    FillBgTilemapRect(bgConfig, bgId, tile, x - 9, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 1, x - 8, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 2, x - 7, y - 1, width + 7, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 3, x + width, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 4, x + width + 1, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 5, x + width + 2, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 6, x - 9, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 7, x - 8, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 8, x - 1, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 9, x + width, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 10, x + width + 1, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 11, x + width + 2, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 12, x - 9, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 13, x - 8, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 14, x - 7, y + height, width + 7, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 15, x + width, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 16, x + width + 1, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 17, x + width + 2, y + height, 1, 1, palette);
}

static void sub_0200EF84(Window *window, u16 tile, u8 palette) {
    u16 dy, dx;
    u8 bgId = GetWindowBgId(window);
    u16 x = GetWindowX(window) - 7;
    u16 y = GetWindowY(window);

    for (dy = 0; dy < 4; dy++) {
        for (dx = 0; dx < 6; dx++) {
            FillBgTilemapRect(window->bgConfig, bgId, tile + dy * 6 + dx, x + dx, y + dy, 1, 1, palette);
        }
    }
}

void DrawFrameAndWindow3(Window *window, BOOL dont_copy_to_vram, u16 baseTile, u8 palette_num, u8 type) {
    u8 bgId = GetWindowBgId(window);

    if (type == 0 || type == 1) {
        sub_0200ECBC(window->bgConfig, bgId, GetWindowX(window), GetWindowY(window), GetWindowWidth(window), GetWindowHeight(window), palette_num, baseTile);
        sub_0200EF84(window, baseTile + 30, palette_num);
    } else {
        sub_0200E6B4(window->bgConfig, bgId, GetWindowX(window), GetWindowY(window), GetWindowWidth(window), GetWindowHeight(window), palette_num, baseTile);
    }
    if (dont_copy_to_vram == FALSE) {
        CopyWindowToVram(window);
    }
    TextPrinter_SetDownArrowBaseTile(baseTile);
}

WaitingIcon *WaitingIcon_New(Window *window, u32 tileNum) {
    WaitingIcon *icon;
    enum HeapID heapID;
    u8 *bgChars;
    u8 *dialTiles;
    u8 *tmp;
    void *dialTilesRaw;
    u8 i;
    NNSG2dCharacterData *pCharData;

    heapID = BgConfig_GetHeapId(window->bgConfig);
    bgChars = BgGetCharPtr(GetWindowBgId(window));
    icon = Heap_Alloc(heapID, sizeof(WaitingIcon));
    memcpy(icon->messageBoxPixels, &bgChars[(tileNum + 18) * 32], 0x80);
    tmp = Heap_Alloc(heapID, 0x80);
    memcpy(&tmp[0x00], &bgChars[(tileNum + 10) * 32], 32);
    memcpy(&tmp[0x20], &bgChars[(tileNum + 11) * 32], 32);
    memcpy(&tmp[0x40], &bgChars[(tileNum + 10) * 32], 32);
    memcpy(&tmp[0x60], &bgChars[(tileNum + 11) * 32], 32);
    for (i = 0; i < 8; i++) {
        memcpy(icon->pixels + i * 0x80, tmp, 0x80);
    }
    Heap_Free(tmp);
    dialTilesRaw = GfGfxLoader_GetCharData(NARC_a_0_3_8, 23, FALSE, &pCharData, heapID);
    dialTiles = pCharData->pRawData;
    sub_0200EA24(dialTiles, 0, 0, 16, 0x80, icon->pixels, 16, 0x80, 0, 0, 16, 0x80);
    Heap_Free(dialTilesRaw);
    icon->window = window;
    icon->messageBoxTile = tileNum;
    icon->counter = 0;
    icon->curFrame = 0;
    icon->deleteMode = 0;
    SysTask_CreateOnVBlankQueue(sub_0200F3D0, icon, 0);
    sub_0200F1D4(icon, 1);
    return icon;
}

static void sub_0200F1D4(WaitingIcon *icon, int mode) {
    u8 bgId = GetWindowBgId(icon->window);
    u8 x = GetWindowX(icon->window);
    u8 y = GetWindowY(icon->window);
    u8 width = GetWindowWidth(icon->window);

    if (mode == 2) {
        BG_LoadCharTilesData(icon->window->bgConfig, bgId, icon->messageBoxPixels, 0x80, icon->messageBoxTile + 18);
        FillBgTilemapRect(icon->window->bgConfig, bgId, icon->messageBoxTile + 10, x + width + 1, y + 2, 1, 1, 16);
        FillBgTilemapRect(icon->window->bgConfig, bgId, icon->messageBoxTile + 11, x + width + 2, y + 2, 1, 1, 16);
        FillBgTilemapRect(icon->window->bgConfig, bgId, icon->messageBoxTile + 10, x + width + 1, y + 3, 1, 1, 16);
        FillBgTilemapRect(icon->window->bgConfig, bgId, icon->messageBoxTile + 11, x + width + 2, y + 3, 1, 1, 16);
        BgCommitTilemapBufferToVram(icon->window->bgConfig, bgId);
        return;
    }
    BG_LoadCharTilesData(icon->window->bgConfig, bgId, &icon->pixels[0x80 * icon->curFrame], 0x80, icon->messageBoxTile + 18);
    if (mode == 0) {
        return;
    }
    FillBgTilemapRect(icon->window->bgConfig, bgId, icon->messageBoxTile + 18, x + width + 1, y + 2, 1, 1, 16);
    FillBgTilemapRect(icon->window->bgConfig, bgId, icon->messageBoxTile + 19, x + width + 2, y + 2, 1, 1, 16);
    FillBgTilemapRect(icon->window->bgConfig, bgId, icon->messageBoxTile + 20, x + width + 1, y + 3, 1, 1, 16);
    FillBgTilemapRect(icon->window->bgConfig, bgId, icon->messageBoxTile + 21, x + width + 2, y + 3, 1, 1, 16);
    BgCommitTilemapBufferToVram(icon->window->bgConfig, bgId);
}

static void sub_0200F3D0(SysTask *task, void *data) {
    WaitingIcon *icon = data;

    if (icon->deleteMode != 0) {
        if (icon->deleteMode == 1) {
            sub_0200F1D4(icon, 2);
        }
        SysTask_Destroy(task);
        return;
    }
    icon->counter++;
    if (icon->counter == 16) {
        icon->counter = 0;
        icon->curFrame = (icon->curFrame + 1) & 7;
        sub_0200F1D4(icon, 0);
    }
}

static void sub_0200F43C(SysTask *task, void *data) {
    Heap_Free(data);
    SysTask_Destroy(task);
}

void sub_0200F450(WaitingIcon *waitingIcon) {
    SysTask_CreateOnVWaitQueue(sub_0200F43C, waitingIcon, 0);
    waitingIcon->deleteMode = 1;
}

void sub_0200F478(WaitingIcon *waitingIcon);

void sub_0200F478(WaitingIcon *waitingIcon) {
    SysTask_CreateOnVWaitQueue(sub_0200F43C, waitingIcon, 0);
    waitingIcon->deleteMode = 2;
}

// NONMATCHING: retail keeps heapID in r5 and spills x/y; MWCC keeps x in r7 and reloads heapID
#ifdef NONMATCHING
struct PokepicManager *DrawPokemonPicFromSpecies(BgConfig *bgConfig, u8 layer, u8 x, u8 y, u8 paletteNum, u16 baseTile, u16 species, u8 gender, enum HeapID heapID) {
    PokemonPreview *preview = sub_0200F5C4(bgConfig, layer, x, y, heapID);

    sub_0200F600(preview, heapID);
    sub_0200F62C(preview);
    sub_0200F684(preview, x, y);
    sub_0200F6D4(preview, species, gender);
    sub_0200F82C(preview, paletteNum, baseTile);
    BgCommitTilemapBufferToVram(bgConfig, layer);
    return (struct PokepicManager *)&preview->state;
}
#else
// clang-format off
asm struct PokepicManager *DrawPokemonPicFromSpecies(BgConfig *bgConfig, u8 layer, u8 x, u8 y, u8 paletteNum, u16 baseTile, u16 species, u8 gender, enum HeapID heapID) {
    push {r4, r5, r6, r7, lr}
    sub sp, #0xc
    str r2, [sp, #4]
    ldr r5, [sp, #0x30]
    str r3, [sp, #8]
    add r6, r0, #0
    add r7, r1, #0
    str r5, [sp]
    bl sub_0200F5C4
    add r4, r0, #0
    add r1, r5, #0
    bl sub_0200F600
    add r0, r4, #0
    bl sub_0200F62C
    ldr r1, [sp, #4]
    ldr r2, [sp, #8]
    add r0, r4, #0
    bl sub_0200F684
    add r2, sp, #0x10
    ldrh r1, [r2, #0x18]
    ldrb r2, [r2, #0x1c]
    add r0, r4, #0
    bl sub_0200F6D4
    add r2, sp, #0x10
    ldrb r1, [r2, #0x10]
    ldrh r2, [r2, #0x14]
    add r0, r4, #0
    bl sub_0200F82C
    add r0, r6, #0
    add r1, r7, #0
    bl BgCommitTilemapBufferToVram
    ldr r0, [pc, #0x4]
    add r0, r4, r0
    add sp, #0xc
    pop {r4, r5, r6, r7, pc}
    dcd 0x0000016F
}
// clang-format on
#endif // NONMATCHING

// NONMATCHING: retail keeps heapID in r5 and spills x/y; MWCC keeps x in r7 and reloads heapID
#ifdef NONMATCHING
struct PokepicManager *DrawPokemonPicFromMon(BgConfig *bgConfig, u8 layer, u8 x, u8 y, u8 paletteNum, u16 baseTile, Pokemon *mon, enum HeapID heapID) {
    PokemonPreview *preview = sub_0200F5C4(bgConfig, layer, x, y, heapID);

    sub_0200F600(preview, heapID);
    sub_0200F62C(preview);
    sub_0200F684(preview, x, y);
    sub_0200F714(preview, mon);
    sub_0200F82C(preview, paletteNum, baseTile);
    BgCommitTilemapBufferToVram(bgConfig, layer);
    return (struct PokepicManager *)&preview->state;
}
#else
// clang-format off
asm struct PokepicManager *DrawPokemonPicFromMon(BgConfig *bgConfig, u8 layer, u8 x, u8 y, u8 paletteNum, u16 baseTile, Pokemon *mon, enum HeapID heapID) {
    push {r4, r5, r6, r7, lr}
    sub sp, #0xc
    str r2, [sp, #4]
    ldr r5, [sp, #0x2c]
    str r3, [sp, #8]
    add r6, r0, #0
    add r7, r1, #0
    str r5, [sp]
    bl sub_0200F5C4
    add r4, r0, #0
    add r1, r5, #0
    bl sub_0200F600
    add r0, r4, #0
    bl sub_0200F62C
    ldr r1, [sp, #4]
    ldr r2, [sp, #8]
    add r0, r4, #0
    bl sub_0200F684
    ldr r1, [sp, #0x28]
    add r0, r4, #0
    bl sub_0200F714
    add r2, sp, #0x10
    ldrb r1, [r2, #0x10]
    ldrh r2, [r2, #0x14]
    add r0, r4, #0
    bl sub_0200F82C
    add r0, r6, #0
    add r1, r7, #0
    bl BgCommitTilemapBufferToVram
    ldr r0, [pc, #0x4]
    add r0, r4, r0
    add sp, #0xc
    pop {r4, r5, r6, r7, pc}
    dcd 0x0000016F
}
// clang-format on
#endif // NONMATCHING

static void sub_0200F54C(SysTask *task, void *data) {
    PokemonPreview *preview = data;

    switch (preview->state) {
    case 1:
        sub_0200F9DC(preview);
        Sprite_DeleteAndFreeResources(preview->managedSprite);
        FieldSpriteManager_ReleaseWithoutResDat(&preview->spriteManager);
        DestroySysTaskAndEnvironment(task);
        return;
    case 2:
        preview->state = 3;
        Sprite_SetAnimCtrlSeq(preview->managedSprite->sprite, 1);
        break;
    case 3:
        if (Sprite_GetAnimationFrame(preview->managedSprite->sprite) == 6) {
            preview->state = 0;
        }
        break;
    }
    Sprite_UpdateAnim(preview->managedSprite->sprite, FX32_ONE);
    SpriteList_RenderAndAnimateSprites(preview->spriteManager.spriteList);
}

static PokemonPreview *sub_0200F5C4(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, enum HeapID heapID) {
    PokemonPreview *preview = SysTask_GetData(CreateSysTaskAndEnvironment(sub_0200F54C, sizeof(PokemonPreview), 0, heapID));

    preview->state = 0;
    preview->bgConfig = bgConfig;
    preview->bgLayer = bgLayer;
    preview->x = x;
    preview->y = y;
    return preview;
}

static void sub_0200F600(PokemonPreview *preview, enum HeapID heapID) {
    SpriteResourceCountsListUnion counts = sPokemonPreviewResCounts;

    FieldSpriteManager_InitEmptyResLists(&preview->spriteManager, &counts, 1, heapID);
}

static void sub_0200F62C(PokemonPreview *preview) {
    FieldSpriteManager_AddPlttRes(&preview->spriteManager, NARC_a_0_3_8, 50, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, POKEMON_PREVIEW_RES_ID);
    FieldSpriteManager_AddCellRes(&preview->spriteManager, NARC_a_0_3_8, 48, FALSE, POKEMON_PREVIEW_RES_ID);
    FieldSpriteManager_AddAnimRes(&preview->spriteManager, NARC_a_0_3_8, 47, FALSE, POKEMON_PREVIEW_RES_ID);
    FieldSpriteManager_AddCharRes(&preview->spriteManager, NARC_a_0_3_8, 49, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, POKEMON_PREVIEW_RES_ID);
}

static void sub_0200F684(PokemonPreview *preview, u8 x, u8 y) {
    ManagedSpriteTemplate template = sPokemonPreviewSpriteTemplate;

    template.x = (x + 5) * 8;
    template.y = (y + 5) * 8;
    preview->managedSprite = FieldSpriteManager_CreateManagedSprite(&preview->spriteManager, &template);
    SpriteList_RenderAndAnimateSprites(preview->spriteManager.spriteList);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void sub_0200F6D4(PokemonPreview *preview, u16 species, u8 gender) {
    PokepicManager *pokepicManager = PokepicManager_Create((enum HeapID)preview->spriteManager.heapID);
    PokepicTemplate template;

    GetMonSpriteCharAndPlttNarcIdsEx(&template, species, gender, 2, FALSE, 0, 0);
    sub_0200F748(preview, &template);
    PokepicManager_Delete(pokepicManager);
}

static void sub_0200F714(PokemonPreview *preview, Pokemon *mon) {
    PokepicManager *pokepicManager = PokepicManager_Create((enum HeapID)preview->spriteManager.heapID);
    PokepicTemplate template;

    GetPokemonSpriteCharAndPlttNarcIds(&template, mon, 2);
    sub_0200F748(preview, &template);
    PokepicManager_Delete(pokepicManager);
}

static void sub_0200F748(PokemonPreview *preview, PokepicTemplate *pokepicTemplate) {
    u8 *buf;
    u32 offset;
    NNSG2dImageProxy *imageProxy;
    const NNSG2dImagePaletteProxy *paletteProxy;

    buf = Heap_Alloc((enum HeapID)preview->spriteManager.heapID, 0x1900);
    {
        UnkStruct_02014E30 frame0 = sPokemonPreviewFrame0Region;
        sub_020143E0((NarcId)pokepicTemplate->narcID, pokepicTemplate->charDataID, (enum HeapID)preview->spriteManager.heapID, &frame0, buf);
    }
    {
        UnkStruct_02014E30 frame1 = sPokemonPreviewFrame1Region;
        sub_020143E0((NarcId)pokepicTemplate->narcID, pokepicTemplate->charDataID, (enum HeapID)preview->spriteManager.heapID, &frame1, buf + 0xC80);
    }
    imageProxy = SpriteTransfer_GetCharProxy(SpriteResourceCollection_Find(preview->spriteManager.spriteResManagers[GF_GFX_RES_TYPE_CHAR], POKEMON_PREVIEW_RES_ID));
    offset = NNS_G2dGetImageLocation(imageProxy, NNS_G2D_VRAM_TYPE_2DMAIN);
    DC_FlushRange(buf, 0x1900);
    GX_LoadOBJ(buf, offset, 0x1900);
    Heap_Free(buf);
    buf = sub_02014450((NarcId)pokepicTemplate->narcID, pokepicTemplate->palDataID, (enum HeapID)preview->spriteManager.heapID);
    paletteProxy = SpriteTransfer_GetPaletteProxy(SpriteResourceCollection_Find(preview->spriteManager.spriteResManagers[GF_GFX_RES_TYPE_PLTT], POKEMON_PREVIEW_RES_ID), imageProxy);
    offset = NNS_G2dGetImagePaletteLocation(paletteProxy, NNS_G2D_VRAM_TYPE_2DMAIN);
    DC_FlushRange(buf, 32);
    GX_LoadOBJPltt(buf, offset, 32);
    Heap_Free(buf);
}

static void sub_0200F82C(PokemonPreview *preview, u8 palette, u16 tile) {
    FillBgTilemapRect(preview->bgConfig, preview->bgLayer, tile, preview->x - 1, preview->y - 1, 1, 1, palette);
    FillBgTilemapRect(preview->bgConfig, preview->bgLayer, tile + 1, preview->x, preview->y - 1, 10, 1, palette);
    FillBgTilemapRect(preview->bgConfig, preview->bgLayer, tile + 2, preview->x + 10, preview->y - 1, 1, 1, palette);
    FillBgTilemapRect(preview->bgConfig, preview->bgLayer, tile + 4, preview->x, preview->y, 10, 10, palette);
    FillBgTilemapRect(preview->bgConfig, preview->bgLayer, tile + 3, preview->x - 1, preview->y, 1, 10, palette);
    FillBgTilemapRect(preview->bgConfig, preview->bgLayer, tile + 5, preview->x + 10, preview->y, 1, 10, palette);
    FillBgTilemapRect(preview->bgConfig, preview->bgLayer, tile + 6, preview->x - 1, preview->y + 10, 1, 1, palette);
    FillBgTilemapRect(preview->bgConfig, preview->bgLayer, tile + 7, preview->x, preview->y + 10, 10, 1, palette);
    FillBgTilemapRect(preview->bgConfig, preview->bgLayer, tile + 8, preview->x + 10, preview->y + 10, 1, 1, palette);
    ScheduleBgTilemapBufferTransfer(preview->bgConfig, preview->bgLayer);
}

static void sub_0200F9DC(PokemonPreview *preview) {
    FillBgTilemapRect(preview->bgConfig, preview->bgLayer, 0, preview->x - 1, preview->y - 1, 12, 12, 0);
    ScheduleBgTilemapBufferTransfer(preview->bgConfig, preview->bgLayer);
}
