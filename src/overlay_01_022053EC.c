#include "overlay_01_022053EC.h"

#include "global.h"

#include "field_system.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_25.h"
#include "palette.h"
#include "party.h"
#include "player_avatar.h"
#include "poke_overlay.h"
#include "scrcmd.h"
#include "script_pokemon_util.h"
#include "sys_task_api.h"
#include "task.h"
#include "unk_0202068C.h"
#include "unk_02026DE0.h"
#include "unk_02054648.h"
#include "unk_02062108.h"
#include "vram_transfer_manager.h"

FS_EXTERN_OVERLAY(OVY_25);

typedef struct FollowMonBillboardResources {
    void *modelRes;
    const NNSG3dResTex *texture;
    const void *anims;
    BillboardGfxSequence gfxSequence;
    NNSGfdTexKey texKey;
    NNSGfdTexKey tex4x4Key;
    NNSGfdPlttKey plttKey;
} FollowMonBillboardResources;

typedef struct FollowMonGrayscaleWork {
    void *nsbtx;
    void *texData;
    u16 palette[16];
} FollowMonGrayscaleWork;

typedef struct FollowMonHopWork {
    u8 state;
    u8 step;
    u8 timer;
    u8 movementIdx;
    u16 unk4[0x20];
    int movingRight;
} FollowMonHopWork;

typedef struct FollowMonPaletteWork {
    int timer;
    u16 palette[0x20];
} FollowMonPaletteWork;

typedef struct FollowMonFadeWork {
    s8 fade;
    u8 step;
    u8 timer;
    u8 delay;
    u16 srcPalette[0x20];
    u16 dstPalette[0x20];
} FollowMonFadeWork;

typedef struct MovementQuad {
    u32 v[4];
} MovementQuad;

typedef struct FollowMonHeightTable {
    fx32 v[3][16];
} FollowMonHeightTable;

extern NNSGfdPlttKey sub_02023FB0(void *billboard);
extern NNSGfdTexKey sub_02023FA0(void *billboard);
extern void sub_02023E78(void *billboard, const VecFx32 *scale);
extern void sub_02023EC8(void *billboard, const FollowMonBillboardResources *resources);
extern void sub_02023EE0(void *billboard, int animNum);
extern void sub_02023F40(void *billboard, fx32 animFrameNum);
extern void *ov01_021FC5A4(void *manager, u32 id);
extern BOOL ov01_021F9744(MapObjectManager *manager, u32 spriteId, FollowMonBillboardResources *out);
extern int ov01_021FA44C(u32 dir);
extern void *ov01_021F771C(MapObjectManager *mapObjectManager);
extern void ov01_021F9048(LocalMapObject *mapObject);
extern u8 sub_0206599C(LocalMapObject *mapObject);
extern u16 sub_020659A8(LocalMapObject *mapObject);
extern void sub_020659B8(LocalMapObject *mapObject);
extern u32 sub_0206234C(u32 direction, u32 movement);
extern s32 GetMoveModelNoBySpriteId(u32 spriteId);

u32 ov01_0220542C(u32 a0, u32 movement);
fx32 ov01_022054E0(LocalMapObject *mapObject);
int ov01_02205564(LocalMapObject *mapObject);
static u8 ov01_02205584(LocalMapObject *mapObject);
u8 ov01_022055B0(LocalMapObject *mapObject);
void ov01_02205664(LocalMapObject *mapObject, int *x, int *z);
u8 ov01_022056C4(LocalMapObject *mapObject, u32 direction);
void ov01_02205808(int a0, LocalMapObject *mapObject, void *billboard);
void ov01_02205870(BOOL a0, LocalMapObject *mapObject, void *billboard, FollowMonBillboardResources *resources);
void ov01_0220589C(int a0, LocalMapObject *mapObject, void *billboard);
static void ov01_0220596C(SysTask *task, void *data);
static BOOL ov01_02205B14(TaskManager *taskMan);
static u8 ov01_02205CF0(FieldSystem *fieldSystem, FollowMonHopWork *work);
static BOOL ov01_02205DB4(TaskManager *taskMan);
void ov01_02205EE0(TaskManager *taskMan);
static BOOL ov01_02205F00(TaskManager *taskMan);
static void ov01_02206028(LocalMapObject *playerObj, LocalMapObject *followObj);
static u32 ov01_02206088(u32 spriteId);
void ov01_0220609C(FieldSystem *fieldSystem, u32 direction);
static BOOL ov01_0220610C(TaskManager *taskMan);
u8 ov01_022062CC(FieldSystem *fieldSystem);

BOOL ScrCmd_808(ScriptContext *ctx) {
    u16 trainerNum = ScriptGetVar(ctx);

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_25), OVY_LOAD_ASYNC);
    TrainerHouse_StartBattle(ctx->fieldSystem, trainerNum);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_25));
    return TRUE;
}

void ov01_02205424(FieldSystem *fieldSystem) {
    fieldSystem->followMon.unk15 = TRUE;
}

u32 ov01_0220542C(u32 a0, u32 movement) {
    MovementQuad list0 = {
        { 8, 9, 10, 11 }
    };
    MovementQuad list1 = {
        { 12, 13, 14, 15 }
    };
    MovementQuad list2 = {
        { 16, 17, 18, 19 }
    };
    MovementQuad list3 = {
        { 20, 21, 22, 23 }
    };
    u8 i;

    for (i = 0; i < 4; i++) {
        if (movement == list0.v[i]) {
            return list1.v[a0];
        }
    }
    for (i = 0; i < 4; i++) {
        if (movement == list1.v[i]) {
            return list2.v[a0];
        }
    }
    for (i = 0; i < 4; i++) {
        if (movement == list2.v[i]) {
            return list3.v[a0];
        }
    }
    GF_ASSERT(FALSE);
    return 0;
}

fx32 ov01_022054E0(LocalMapObject *mapObject) {
    FollowMonHeightTable table = {
        {
         { 0, FX32_CONST(8), FX32_CONST(12), FX32_CONST(8), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
         { 0, FX32_CONST(6), FX32_CONST(8), FX32_CONST(10), FX32_CONST(12), FX32_CONST(10), FX32_CONST(8), FX32_CONST(6), 0, 0, 0, 0, 0, 0, 0, 0 },
         { 0, FX32_CONST(6), FX32_CONST(8), FX32_CONST(8), FX32_CONST(10), FX32_CONST(10), FX32_CONST(12), FX32_CONST(12), FX32_CONST(12), FX32_CONST(12), FX32_CONST(12), FX32_CONST(10), FX32_CONST(10), FX32_CONST(8), FX32_CONST(8), FX32_CONST(6) },
         }
    };
    int frame;
    u8 type;

    if (ov01_02205584(mapObject)) {
        return 0;
    }
    type = sub_020659A8(mapObject);
    if (type == 0) {
        return 0;
    }
    frame = sub_0206599C(mapObject);
    GF_ASSERT(frame < 16);
    return table.v[(u8)(type - 1)][frame];
}

BOOL ov01_0220553C(LocalMapObject *mapObject) {
    u32 id = MapObject_GetID(mapObject);

    if (id != 0xFD && id != 0xFA && id != 0xFB) {
        return FALSE;
    }
    return MapObject_GetParam(mapObject, 2) & 1;
}

int ov01_02205564(LocalMapObject *mapObject) {
    int spriteId = MapObject_GetSpriteID(mapObject);

    if (spriteId >= 0x19F && spriteId <= 0x1A4) {
        return TRUE;
    }
    return FALSE;
}

static u8 ov01_02205584(LocalMapObject *mapObject) {
    u16 param = MapObject_GetParam(mapObject, 1);

    if (MapObject_GetID(mapObject) != 0xFD) {
        return 0;
    }
    return ((u8)param >> 4) & 0xF;
}

u8 ov01_022055B0(LocalMapObject *mapObject) {
    u16 param = MapObject_GetParam(mapObject, 1);

    if (MapObject_GetID(mapObject) != 0xFD) {
        return 0;
    }
    return (u8)param & 0xF;
}

u8 ov01_022055DC(LocalMapObject *mapObject) {
    u16 param = MapObject_GetParam(mapObject, 1);

    if (MapObject_GetID(mapObject) != 0xFD) {
        return 0;
    }
    return (param >> 8) & 0xF;
}

void ov01_02205604(LocalMapObject *mapObject, int *x, int *z) {
    u8 direction = MapObject_GetFacingDirection(mapObject);

    *x = MapObject_GetXCoord(mapObject);
    *z = MapObject_GetZCoord(mapObject);
    switch (direction) {
    case 0:
        (*z)++;
        break;
    case 1:
        (*z)--;
        break;
    case 2:
        (*x)++;
        break;
    case 3:
        (*x)--;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

void ov01_02205664(LocalMapObject *mapObject, int *x, int *z) {
    u8 direction = MapObject_GetFacingDirection(mapObject);

    *x = MapObject_GetPreviousXCoord(mapObject);
    *z = MapObject_GetPreviousZCoord(mapObject);
    switch (direction) {
    case 0:
        (*z)++;
        break;
    case 1:
        (*z)--;
        break;
    case 2:
        (*x)++;
        break;
    case 3:
        (*x)--;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

u8 ov01_022056C4(LocalMapObject *mapObject, u32 direction) {
    FieldSystem *fieldSystem = MapObject_GetFieldSystem(mapObject);
    int x = MapObject_GetXCoord(mapObject);
    int z = MapObject_GetZCoord(mapObject);

    switch (direction) {
    case 0:
        z--;
        break;
    case 1:
        z++;
        break;
    case 3:
        x--;
        break;
    case 2:
        x++;
        break;
    case 4:
        x++;
        z++;
        break;
    case 5:
        x--;
        z++;
        break;
    }
    return GetMetatileBehavior(fieldSystem, x, z);
}

void ov01_02205720(LocalMapObject *playerObj, LocalMapObject *tsurePokeObj, int a2, int a3) {
    VecFx32 pos;

    MapObject_CopyPositionVector(playerObj, &pos);
    switch (a2) {
    case 0:
        pos.z -= FX32_CONST(16);
        break;
    case 1:
        pos.z += FX32_CONST(16);
        break;
    case 3:
        pos.x += FX32_CONST(16);
        break;
    case 2:
        pos.x -= FX32_CONST(16);
        break;
    }
    MapObject_SetPositionFromVectorAndDirection(tsurePokeObj, &pos, a3);
}

void ov01_02205784(LocalMapObject *object) {
    ov01_0220329C(object, 0);
}

void ov01_02205790(FieldSystem *fieldSystem, u8 a1) {
    VecFx32 pos;
    LocalMapObject *followObj;

    if (FollowMon_IsActive(fieldSystem)) {
        followObj = FollowMon_GetMapObject(fieldSystem);
        MapObject_CopyPositionVector(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), &pos);
        MapObject_SetPositionFromVectorAndDirection(followObj, &pos, a1);
    }
}

BOOL ov01_022057C4(FieldSystem *fieldSystem) {
    return MapObject_CheckVisible(FollowMon_GetMapObject(fieldSystem));
}

void ov01_022057D0(FieldSystem *fieldSystem) {
    sub_020659B8(FollowMon_GetMapObject(fieldSystem));
}

void ov01_022057DC(MapObjectManager *mapObjectManager) {
    void *unk = sub_0205F1A0(mapObjectManager);
    int i;
    s32 *data = ov01_021FC5A4(*(void **)((u8 *)unk + 0xFC), 0x16);
    int count = data[0];
    u8 *plttIdx = (u8 *)((u16 *)&data[1] + count) + count;

    for (i = 0; i < count; i++) {
        plttIdx[i] = 1;
    }
}

void ov01_02205808(int a0, LocalMapObject *mapObject, void *billboard) {
    FollowMonBillboardResources resources;
    MapObjectManager *manager = MapObject_GetManager(mapObject);
    u32 id;

    ov01_021F9744(manager, MapObject_GetSpriteID(mapObject), &resources);
    id = MapObject_GetID(mapObject);
    if (id != 0xFD && id != 0xFA && id != 0xFB) {
        return;
    }
    ov01_02205870(a0, mapObject, billboard, &resources);
    sub_02023EC8(billboard, &resources);
    sub_02023EE0(billboard, ov01_021FA44C(MapObject_GetFacingDirection(mapObject)));
    sub_02023F40(billboard, 0);
}

void ov01_02205870(BOOL a0, LocalMapObject *mapObject, void *billboard, FollowMonBillboardResources *resources) {
    void *unk = sub_0205F1A0(MapObject_GetManager(mapObject));

    sub_02026E18(ov01_021FC5A4(*(void **)((u8 *)unk + 0xFC), a0 ? 0x16 : 0x15), &resources->gfxSequence);
}

void ov01_0220589C(int a0, LocalMapObject *mapObject, void *billboard) {
    FollowMonGrayscaleWork *work = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(FollowMonGrayscaleWork));
    u32 modelNo;
    NNSG3dResTex *tex;
    void *palette;
    NNSGfdTexKey sizeKey;
    NNSGfdTexKey addrKey;

    MI_CpuFill8(work, 0, 4);
    modelNo = ov01_02206088((u16)((u32)MapObject_GetParam(mapObject, 2) >> 1));
    work->nsbtx = NARC_AllocAndReadWholeMember(MapObjectManager_GetMapModelNarc(MapObject_GetManager(mapObject)), modelNo, HEAP_ID_FIELD2);
    tex = NNS_G3dGetTex(work->nsbtx);
    palette = sub_02020888(tex, 0);
    work->texData = sub_02020838(tex, 0);
    MIi_CpuCopy16(palette, work->palette, 0x20);
    TintPalette_GrayScale(work->palette, 16);
    TintPalette_CustomTone(work->palette, 16, 0x100, 0xB4, 0);
    GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, NNS_GfdGetPlttKeyAddr(sub_02023FB0(billboard)), work->palette, 0x20);
    sizeKey = sub_02023FA0(billboard);
    addrKey = sub_02023FA0(billboard);
    GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_VRAM, NNS_GfdGetTexKeyAddr(addrKey), work->texData, ((sizeKey & 0x7FFF0000) >> 16) << 4);
    SysTask_CreateOnVWaitQueue(ov01_0220596C, work, 0);
}

static void ov01_0220596C(SysTask *task, void *data) {
    FollowMonGrayscaleWork *work = data;

    if (GF_GetNumPendingVramTransferTasks() == 0) {
        SysTask_Destroy(task);
        Heap_Free(work->nsbtx);
        Heap_Free(work);
    }
}

void ov01_02205990(int a0, int x, int z, FollowMon *mon) {
    mon->unk4 = a0;
    mon->unk8 = x;
    mon->unkC = z;
    if (mon->unk1C == 3) {
        mon->unk1C = 2;
    } else if (mon->unk1C == 0) {
        mon->unk1C = 1;
    }
}

void ov01_022059AC(FieldSystem *fieldSystem) {
    LocalMapObject *mapObject;

    if (fieldSystem->followMon.unk15) {
        if (!fieldSystem->followMon.active) {
            int x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
            int z = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
            int direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);

            FollowMon_InitMapObject(fieldSystem->mapObjectManager, x, z, direction, fieldSystem->location->mapId);
            mapObject = fieldSystem->followMon.mapObject;
            if (fieldSystem->followMon.active == TRUE) {
                ov01_021F9048(mapObject);
            }
        } else if (!FollowMon_GetPermissionBySpeciesAndMap(FollowMon_GetSpecies(fieldSystem->followMon.mapObject), fieldSystem->location->mapId)) {
            MapObject_Remove(fieldSystem->followMon.mapObject);
            fieldSystem->followMon.active = FALSE;
        }
    }
    fieldSystem->followMon.unk15 = FALSE;
}

void ov01_02205A34(FieldSystem *fieldSystem, fx32 y) {
    VecFx32 pos;
    LocalMapObject *followObj;

    if (FollowMon_IsActive(fieldSystem)) {
        followObj = FollowMon_GetMapObject(fieldSystem);
        MapObject_CopyPositionVector(followObj, &pos);
        pos.y = y;
        MapObject_SetPositionVector(followObj, &pos);
    }
}

BOOL ov01_02205A60(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    u32 *state = TaskManager_GetStatePtr(taskMan);
    LocalMapObject *followObj;

    if (!FollowMon_IsActive(fieldSystem)) {
        return TRUE;
    }
    followObj = fieldSystem->followMon.mapObject;
    switch (*state) {
    case 0:
        if (MapObject_CheckMovementPaused(followObj)) {
            MapObject_UnpauseMovement(followObj);
            *state = 1;
        } else {
            *state = 2;
        }
        break;
    case 1:
        if (MapObject_IsMovementPaused(followObj) && MapObject_AreBitsSetForMovementScriptInit(followObj)) {
            MapObject_PauseMovement(followObj);
            return TRUE;
        }
        break;
    case 2:
        if (MapObject_IsMovementPaused(followObj) && MapObject_AreBitsSetForMovementScriptInit(followObj)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void ov01_02205AEC(FieldSystem *fieldSystem) {
    FollowMonHopWork *work = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(FollowMonHopWork));

    work->state = 0;
    work->step = 0;
    work->timer = 0;
    work->movementIdx = 0;
    TaskManager_Call(fieldSystem->taskman, ov01_02205B14, work);
}

static BOOL ov01_02205B14(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    FollowMonHopWork *work = TaskManager_GetEnvironment(taskMan);

    switch (work->state) {
    case 0:
        if (!FollowMon_IsActive(fieldSystem)) {
            Heap_Free(work);
            return TRUE;
        }
        if (ov01_022057C4(fieldSystem)) {
            Heap_Free(work);
            return TRUE;
        }
        MapObject_UnpauseMovement(FollowMon_GetMapObject(fieldSystem));
        work->state++;
        // fallthrough
    case 1:
        if (MapObject_IsMovementPaused(FollowMon_GetMapObject(fieldSystem))) {
            work->state = ov01_02205CF0(fieldSystem, work);
        }
        break;
    case 2: {
        u32 movements[2] = { 14, 12 };
        LocalMapObject *followObj = FollowMon_GetMapObject(fieldSystem);

        if (MapObject_IsMovementPaused(followObj)) {
            MapObject_SetHeldMovement(followObj, movements[work->movementIdx++]);
            if (work->movementIdx >= 2) {
                work->state++;
            }
        }
        break;
    }
    case 3: {
        LocalMapObject *followObj = FollowMon_GetMapObject(fieldSystem);

        if (MapObject_IsMovementPaused(followObj)) {
            MapObject_SetHeldMovement(followObj, 0);
            work->state++;
        }
        break;
    }
    case 4: {
        s8 zOffsets[8] = { 4, 4, 4, 2, 2, 2, 0, 0 };
        s8 yOffsets[8] = { 1, 2, 2, 3, 3, 2, 2, 0 };
        LocalMapObject *followObj = FollowMon_GetMapObject(fieldSystem);
        fx32 dx = FX32_CONST(2);
        VecFx32 pos;

        if (work->movingRight == 0) {
            dx *= -1;
        }
        MapObject_CopyPositionVector(followObj, &pos);
        pos.z -= zOffsets[work->step] << FX32_SHIFT;
        pos.x += dx;
        pos.y += yOffsets[work->step] << FX32_SHIFT;
        MapObject_SetPositionVector(followObj, &pos);
        work->step++;
        if (work->step >= 8) {
            work->state++;
        }
        break;
    }
    case 5:
        ov01_0220329C(FollowMon_GetMapObject(fieldSystem), 3);
        work->state++;
        break;
    case 6:
        work->timer++;
        if (work->timer >= 20) {
            ov01_02205790(fieldSystem, 0);
            VecFx32 scale = { FX32_ONE, FX32_ONE, FX32_ONE };
            sub_02023E78(ov01_021F771C(fieldSystem->mapObjectManager), &scale);
            sub_02069E84(fieldSystem->followMon.mapObject, 1);
            work->state++;
        }
        break;
    case 7:
        Heap_Free(work);
        return TRUE;
    }
    return FALSE;
}

static u8 ov01_02205CF0(FieldSystem *fieldSystem, FollowMonHopWork *work) {
    int playerX = MapObject_GetXCoord(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar));
    int playerZ = MapObject_GetZCoord(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar));
    int followX = MapObject_GetXCoord(FollowMon_GetMapObject(fieldSystem));
    int followZ = MapObject_GetZCoord(FollowMon_GetMapObject(fieldSystem));

    if (followX == playerX && followZ == playerZ + 1) {
        work->movingRight = TRUE;
        return 2;
    }
    if (followX == playerX + 1 && followZ == playerZ) {
        work->movingRight = FALSE;
        return 3;
    }
    if (followX + 1 == playerX && followZ == playerZ) {
        work->movingRight = TRUE;
        return 3;
    }
    GF_ASSERT(FALSE);
    return 2;
}

BOOL ov01_02205D68(FieldSystem *fieldSystem) {
    FollowMonPaletteWork *work;

    if (!FollowMon_IsActive(fieldSystem)) {
        return FALSE;
    }
    if (ov01_022057C4(fieldSystem)) {
        sub_0206A054(fieldSystem);
        ov01_02205790(fieldSystem, 0);
        return FALSE;
    }
    work = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(FollowMonPaletteWork));
    work->timer = 0;
    TaskManager_Call(fieldSystem->taskman, ov01_02205DB4, work);
    return TRUE;
}

static BOOL ov01_02205DB4(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    FollowMonPaletteWork *work = TaskManager_GetEnvironment(taskMan);
    u32 *state = TaskManager_GetStatePtr(taskMan);

    switch (*state) {
    case 0:
        MapObject_UnpauseMovement(FollowMon_GetMapObject(fieldSystem));
        (*state)++;
        // fallthrough
    case 1:
        if (MapObject_IsMovementPaused(FollowMon_GetMapObject(fieldSystem))) {
            (*state)++;
        }
        break;
    case 2: {
        LocalMapObject *followObj = FollowMon_GetMapObject(fieldSystem);
        u32 modelNo = ov01_02206088(MapObject_GetSpriteID(followObj));
        void *nsbtx = NARC_AllocAndReadWholeMember(MapObjectManager_GetMapModelNarc(fieldSystem->mapObjectManager), modelNo, HEAP_ID_FIELD2);
        NNSG3dResTex *tex = NNS_G3dGetTex(nsbtx);

        MIi_CpuCopy16((void *)tex + tex->plttInfo.ofsPlttData, work->palette, 0x40);
        Heap_Free(nsbtx);
        ov01_0220329C(followObj, 1);
        (*state)++;
        break;
    }
    case 3:
        if (++work->timer >= 20) {
            void *billboard;
            NNSGfdPlttKey sizeKey;
            NNSGfdPlttKey addrKey;

            ov01_02205790(fieldSystem, 0);
            VecFx32 scale = { FX32_ONE, FX32_ONE, FX32_ONE };
            billboard = ov01_021F771C(fieldSystem->mapObjectManager);
            sub_02023E78(billboard, &scale);
            sizeKey = sub_02023FB0(billboard);
            addrKey = sub_02023FB0(billboard);
            GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, NNS_GfdGetPlttKeyAddr(addrKey), work->palette, ((sizeKey & 0xFFFF0000) >> 16) << 3);
            sub_0206A054(fieldSystem);
            sub_02069E28(FollowMon_GetMapObject(fieldSystem), 0);
            (*state)++;
        }
        break;
    case 4:
        Heap_Free(work);
        return TRUE;
    }
    return FALSE;
}

void ov01_02205EE0(TaskManager *taskMan) {
    int *timer = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(int));

    *timer = 0;
    TaskManager_Call(taskMan, ov01_02205F00, timer);
}

static BOOL ov01_02205F00(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    int *timer = TaskManager_GetEnvironment(taskMan);
    u32 *state = TaskManager_GetStatePtr(taskMan);

    switch (*state) {
    case 0:
        sub_020659CC(fieldSystem->followMon.mapObject);
        sub_0205F484(fieldSystem->followMon.mapObject);
        (*state)++;
        break;
    case 1:
        if (MapObject_AreBitsSetForMovementScriptInit(fieldSystem->followMon.mapObject) == TRUE) {
            ov01_02206028(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), fieldSystem->followMon.mapObject);
            (*state)++;
        }
        break;
    case 2:
        if (MapObject_AreBitsSetForMovementScriptInit(fieldSystem->followMon.mapObject) == TRUE) {
            MapObject_SetFacingDirection(fieldSystem->followMon.mapObject, MapObject_GetFacingDirection(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar)));
            (*state)++;
        }
        break;
    case 3:
        if (++(*timer) > 10) {
            (*state)++;
        }
        break;
    case 4:
        if (MapObject_AreBitsSetForMovementScriptInit(fieldSystem->followMon.mapObject) == TRUE) {
            sub_0206A040(fieldSystem->followMon.mapObject, FALSE);
            MapObject_SetHeldMovement(fieldSystem->followMon.mapObject, sub_0206234C(MapObject_GetFacingDirection(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar)), 0x34));
            (*state)++;
        }
        break;
    case 5:
        if (MapObject_AreBitsSetForMovementScriptInit(fieldSystem->followMon.mapObject) == TRUE) {
            ov01_0220329C(fieldSystem->followMon.mapObject, 2);
            sub_0206A054(fieldSystem);
            (*state)++;
        }
        break;
    case 6:
        Heap_Free(timer);
        return TRUE;
    }
    return FALSE;
}

static void ov01_02206028(LocalMapObject *playerObj, LocalMapObject *followObj) {
    int playerX = MapObject_GetXCoord(playerObj);
    int playerZ = MapObject_GetZCoord(playerObj);
    int followX = MapObject_GetXCoord(followObj);
    int followZ = MapObject_GetZCoord(followObj);
    int dx = playerX - followX;
    int dz = playerZ - followZ;

    MapObject_GetFacingDirection(playerObj);
    if (dx < 0) {
        MapObject_SetHeldMovement(followObj, 10);
    } else if (dx > 0) {
        MapObject_SetHeldMovement(followObj, 11);
    } else if (dz < 0) {
        MapObject_SetHeldMovement(followObj, 8);
    } else if (dz > 0) {
        MapObject_SetHeldMovement(followObj, 9);
    }
}

static u32 ov01_02206088(u32 spriteId) {
    s32 modelNo = GetMoveModelNoBySpriteId(spriteId);

    if (modelNo < 0) {
        GF_ASSERT(FALSE);
        modelNo = 0;
    }
    return modelNo;
}

void ov01_0220609C(FieldSystem *fieldSystem, u32 direction) {
    if (FollowMon_IsActive(fieldSystem)) {
        MapObject_SetFacingDirection(fieldSystem->followMon.mapObject, direction);
    }
}

BOOL ov01_022060B8(FieldSystem *fieldSystem, u8 a1, u8 a2) {
    FollowMonFadeWork *work;

    if (!FollowMon_IsActive(fieldSystem)) {
        return FALSE;
    }
    if (ov01_022057C4(fieldSystem)) {
        return FALSE;
    }
    if (a1 == 0) {
        return FALSE;
    }
    work = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(FollowMonFadeWork));
    MI_CpuFill8(work, 0, sizeof(FollowMonFadeWork));
    work->step = a1;
    work->delay = a2;
    TaskManager_Call(fieldSystem->taskman, ov01_0220610C, work);
    return TRUE;
}

static BOOL ov01_0220610C(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    FollowMonFadeWork *work = TaskManager_GetEnvironment(taskMan);
    u32 *state = TaskManager_GetStatePtr(taskMan);

    switch (*state) {
    case 0: {
        u32 modelNo = ov01_02206088(MapObject_GetSpriteID(FollowMon_GetMapObject(fieldSystem)));
        void *nsbtx = NARC_AllocAndReadWholeMember(MapObjectManager_GetMapModelNarc(fieldSystem->mapObjectManager), modelNo, HEAP_ID_FIELD2);
        NNSG3dResTex *tex = NNS_G3dGetTex(nsbtx);

        MIi_CpuCopy16((void *)tex + tex->plttInfo.ofsPlttData, work->srcPalette, 0x40);
        Heap_Free(nsbtx);
        (*state)++;
        break;
    }
    case 1:
        if (work->timer != 0) {
            work->timer--;
        } else {
            void *billboard;
            NNSGfdPlttKey sizeKey;
            NNSGfdPlttKey addrKey;

            work->fade += work->step;
            if (work->fade >= 16) {
                work->fade = 16;
                (*state)++;
            }
            BlendPalette(work->srcPalette, work->dstPalette, 0x20, work->fade, 0xFFFF);
            billboard = ov01_021F771C(fieldSystem->mapObjectManager);
            sizeKey = sub_02023FB0(billboard);
            addrKey = sub_02023FB0(billboard);
            GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, NNS_GfdGetPlttKeyAddr(addrKey), work->dstPalette, ((sizeKey & 0xFFFF0000) >> 16) << 3);
            work->timer = work->delay;
        }
        break;
    case 2:
        if (work->timer != 0) {
            work->timer--;
        } else {
            void *billboard;
            NNSGfdPlttKey sizeKey;
            NNSGfdPlttKey addrKey;

            work->fade -= work->step;
            if (work->fade <= 0) {
                (*state)++;
            }
            BlendPalette(work->srcPalette, work->dstPalette, 0x20, work->fade, 0xFFFF);
            billboard = ov01_021F771C(fieldSystem->mapObjectManager);
            sizeKey = sub_02023FB0(billboard);
            addrKey = sub_02023FB0(billboard);
            GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, NNS_GfdGetPlttKeyAddr(addrKey), work->dstPalette, ((sizeKey & 0xFFFF0000) >> 16) << 3);
            work->timer = work->delay;
        }
        break;
    case 3:
        Heap_Free(work);
        return TRUE;
    }
    return FALSE;
}

BOOL ov01_02206268(FieldSystem *fieldSystem) {
    LocalMapObject *playerObj;
    LocalMapObject *followObj;
    int playerX;
    int playerZ;
    int followX;
    int followZ;

    if (!FollowMon_IsVisible(fieldSystem)) {
        return FALSE;
    }
    playerObj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    followObj = fieldSystem->followMon.mapObject;
    playerX = MapObject_GetXCoord(playerObj);
    playerZ = MapObject_GetZCoord(playerObj);
    followX = MapObject_GetXCoord(followObj);
    followZ = MapObject_GetZCoord(followObj);
    if (playerX == followX) {
        if (playerZ + 1 == followZ || playerZ - 1 == followZ) {
            return TRUE;
        }
    } else if (playerZ == followZ) {
        if (playerX + 1 == followX || playerX - 1 == followX) {
            return TRUE;
        }
    }
    return FALSE;
}

u8 ov01_022062CC(FieldSystem *fieldSystem) {
    return GetIdxOfFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
}
