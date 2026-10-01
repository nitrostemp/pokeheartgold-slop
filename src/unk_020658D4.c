#include "global.h"

#include "field_system.h"
#include "follow_mon.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "player_avatar.h"
#include "unk_02054648.h"
#include "unk_0205FD20.h"

// Map object movement types for the walking partner Pokemon (following the
// player), for objects that follow another map object, and for the shadow
// effect objects. Does not include the frozen unk_020658D4.h.

typedef struct FollowPlayerMoveData {
    u8 state;      // 0x0
    u8 init;       // 0x1
    u8 stepCount;  // 0x2
    u8 frameCount; // 0x3
    s16 x;         // 0x4
    s16 z;         // 0x6
    u16 unk8;      // 0x8
    u16 inGrass : 1;
    u16 animSize : 2;
    u16 unkA_3 : 13;
} FollowPlayerMoveData; // size: 0xC

typedef struct FollowObjectMoveData {
    u8 state;               // 0x0
    u8 init;                // 0x1
    s16 x;                  // 0x2
    s16 z;                  // 0x4
    u16 unk6;               // 0x6
    LocalMapObject *target; // 0x8
} FollowObjectMoveData;     // size: 0xC

typedef struct EffectObjectMoveData {
    u8 state;           // 0x0
    u8 type;            // 0x1
    u8 noEffect;        // 0x2
    void *effect;       // 0x4
} EffectObjectMoveData; // size: 0x8

typedef struct MovementList {
    u32 movements[4];
} MovementList;

typedef BOOL (*FollowPlayerMoveFunc)(LocalMapObject *mapObject, FollowPlayerMoveData *data);
typedef BOOL (*FollowObjectMoveFunc)(LocalMapObject *mapObject, FollowObjectMoveData *data);
typedef BOOL (*EffectObjectMoveFunc)(LocalMapObject *mapObject, EffectObjectMoveData *data);

u32 sub_020623C8(u32 movement);
u32 sub_020623D8(u32 direction);
BOOL sub_02062428(LocalMapObject *mapObject);
u32 sub_0206234C(u32 direction, u32 movement);
u32 sub_02061200(int x, int z, int x2, int z2);
u32 sub_02064518(LocalMapObject *mapObject);
void MapObject_ForceSetHeldMovement(LocalMapObject *mapObject, u32 movement);
void ov01_021FF070(LocalMapObject *mapObject, int a1);
void ov01_021FF0E4(LocalMapObject *mapObject, int a1, int x, int z, int a4);
void ov01_021FF8F0(LocalMapObject *mapObject, int a1);
void ov01_021FF964(LocalMapObject *mapObject, int a1, int x, int z, int a4);
void *ov01_021FFF5C(LocalMapObject *mapObject, int type);
void ov01_0220329C(LocalMapObject *mapObject, int a1);
u32 ov01_0220542C(u32 a0, u32 movement);
u8 ov01_022055DC(LocalMapObject *mapObject);
void ov01_02205604(LocalMapObject *mapObject, int *x, int *z);

void sub_020658D4(LocalMapObject *mapObject);
void sub_02065900(LocalMapObject *mapObject);
void sub_02065938(LocalMapObject *mapObject);
void sub_02065968(LocalMapObject *mapObject);
void sub_02065998(LocalMapObject *mapObject);
u8 sub_0206599C(LocalMapObject *mapObject);
u16 sub_020659A8(LocalMapObject *mapObject);
void sub_020659B8(LocalMapObject *mapObject);
void sub_020659CC(LocalMapObject *mapObject);
static BOOL sub_02065A4C(LocalMapObject *mapObject, FollowPlayerMoveData *data);
static BOOL sub_02065B70(LocalMapObject *mapObject, FollowPlayerMoveData *data);
static BOOL sub_02065BE8(LocalMapObject *mapObject, FollowPlayerMoveData *data);
static BOOL sub_02065C2C(LocalMapObject *mapObject, FollowPlayerMoveData *data);
static BOOL sub_02065C48(LocalMapObject *mapObject, FollowPlayerMoveData *data);
static BOOL sub_02065C90(LocalMapObject *mapObject, FollowPlayerMoveData *data);
static BOOL sub_02065CD0(LocalMapObject *mapObject, FollowPlayerMoveData *data);
static void sub_02065CFC(LocalMapObject *mapObject, FollowPlayerMoveData *data);
static BOOL sub_02065D24(LocalMapObject *mapObject, FollowPlayerMoveData *data);
static void sub_02065D58(LocalMapObject *mapObject, FollowPlayerMoveData *data);
static u32 sub_02065D78(LocalMapObject *mapObject);
static u32 sub_02065DB4(LocalMapObject *mapObject);
static BOOL sub_02065DF4(LocalMapObject *mapObject, FollowPlayerMoveData *data);
static BOOL sub_02065F44(LocalMapObject *mapObject);
static BOOL sub_02065FBC(LocalMapObject *mapObject);
void sub_02065FFC(LocalMapObject *mapObject);
void sub_02066024(LocalMapObject *mapObject);
void sub_02066054(LocalMapObject *mapObject);
void sub_02066058(LocalMapObject *mapObject);
static BOOL sub_02066064(LocalMapObject *mapObject, FollowObjectMoveData *data);
static BOOL sub_020660A0(LocalMapObject *mapObject, FollowObjectMoveData *data);
LocalMapObject *sub_020660C0(LocalMapObject *mapObject);
static BOOL sub_02066150(LocalMapObject *mapObject, FollowObjectMoveData *data);
static void sub_020661CC(LocalMapObject *mapObject, FollowObjectMoveData *data, LocalMapObject *target);
static BOOL sub_020661F0(LocalMapObject *mapObject, FollowObjectMoveData *data);
static BOOL sub_0206623C(LocalMapObject *mapObject, FollowObjectMoveData *data);
static void sub_020662C4(LocalMapObject *mapObject, u8 type);
void sub_0206630C(LocalMapObject *mapObject);
void sub_02066318(LocalMapObject *mapObject);
void sub_02066324(LocalMapObject *mapObject);
void sub_02066330(LocalMapObject *mapObject);
void sub_0206633C(LocalMapObject *mapObject);
void sub_02066360(LocalMapObject *mapObject);
void sub_02066370(LocalMapObject *mapObject);
static BOOL sub_020663B4(LocalMapObject *mapObject, EffectObjectMoveData *data);
static BOOL sub_020663E4(LocalMapObject *mapObject, EffectObjectMoveData *data);
void sub_02066420(LocalMapObject *mapObject, void *effect);
void *sub_0206642C(LocalMapObject *mapObject);
void sub_02066438(LocalMapObject *mapObject);
static int sub_02066444(u32 movement);
static void sub_020664D8(LocalMapObject *mapObject);

static const FollowObjectMoveFunc _020FE3D4[] = {
    sub_02066064,
    sub_020660A0,
};

static const EffectObjectMoveFunc _020FE3CC[] = {
    sub_020663B4,
    sub_020663E4,
};

static const VecFx32 _020FE3E8 = { 0, -FX32_CONST(32), 0 };

static const VecFx32 _020FE3DC = { 0, -FX32_CONST(32), 0 };

static const MovementList _020FE3F4 = {
    { 12, 13, 14, 15 }
};

static const FollowPlayerMoveFunc _020FE414[] = {
    sub_02065B70,
    sub_02065C2C,
    sub_02065C48,
    sub_02065C90,
};

static const FollowPlayerMoveFunc _020FE424[] = {
    sub_02065BE8,
    sub_02065C2C,
    sub_02065C48,
    sub_02065C90,
};

static const FollowPlayerMoveFunc _020FE404[] = {
    sub_02065A4C,
    sub_02065C2C,
    sub_02065C48,
    sub_02065C90,
};

static const MovementList _020FE434 = {
    { 16, 17, 18, 19 }
};

static const MovementList _020FE444 = {
    { 20, 21, 22, 23 }
};

void sub_020658D4(LocalMapObject *mapObject) {
    FollowPlayerMoveData *data = (FollowPlayerMoveData *)sub_0205F370(mapObject, sizeof(FollowPlayerMoveData));

    sub_02065CD0(mapObject, data);
    sub_0205F328(mapObject, 0);
    MapObject_ClearSingleMovement(mapObject);
    MapObject_ClearFlag18(mapObject, FALSE);
}

void sub_02065900(LocalMapObject *mapObject) {
    FollowPlayerMoveData *data = (FollowPlayerMoveData *)sub_0205F394(mapObject);

    if (sub_02065CD0(mapObject, data)) {
        MapObject_ClearFlag18(mapObject, FALSE);
        while (_020FE404[data->state](mapObject, data) == TRUE) {}
    }
}

void sub_02065938(LocalMapObject *mapObject) {
    FollowPlayerMoveData *data = (FollowPlayerMoveData *)sub_0205F394(mapObject);

    if (sub_02065CD0(mapObject, data)) {
        while (_020FE414[data->state](mapObject, data) == TRUE) {}
    }
}

void sub_02065968(LocalMapObject *mapObject) {
    FollowPlayerMoveData *data = (FollowPlayerMoveData *)sub_0205F394(mapObject);

    if (sub_02065CD0(mapObject, data)) {
        while (_020FE424[data->state](mapObject, data) == TRUE) {}
    }
}

void sub_02065998(LocalMapObject *mapObject) {
}

u8 sub_0206599C(LocalMapObject *mapObject) {
    FollowPlayerMoveData *data = (FollowPlayerMoveData *)sub_0205F394(mapObject);
    return data->frameCount;
}

u16 sub_020659A8(LocalMapObject *mapObject) {
    FollowPlayerMoveData *data = (FollowPlayerMoveData *)sub_0205F394(mapObject);
    return data->animSize;
}

void sub_020659B8(LocalMapObject *mapObject) {
    FollowPlayerMoveData *data = (FollowPlayerMoveData *)sub_0205F394(mapObject);
    data->inGrass = TRUE;
}

void sub_020659CC(LocalMapObject *mapObject) {
    FieldSystem *fieldSystem = MapObject_GetFieldSystem(mapObject);
    FollowPlayerMoveData *data = (FollowPlayerMoveData *)sub_0205F394(mapObject);

    MapObject_ClearSingleMovement(mapObject);
    MapObject_ClearEndMovement(mapObject);
    MapObject_ClearFlagsBits(mapObject, MAPOBJECTFLAG_UNK5);
    MapObject_SetMovementCommand(mapObject, 0xFF);
    MapObject_SetMovementStep(mapObject, 0);
    data->state = 0;
    fieldSystem->followMon.unk4 = 0;
    fieldSystem->followMon.unk1C = 0;
    fieldSystem->followMon.unk8 = 0;
    fieldSystem->followMon.unkC = 0;
    MapObject_SetPositionFromXYZAndDirection(mapObject, MapObject_GetXCoord(mapObject), MapObject_GetYCoord(mapObject), MapObject_GetZCoord(mapObject), MapObject_GetFacingDirection(mapObject));
}

static BOOL sub_02065A4C(LocalMapObject *mapObject, FollowPlayerMoveData *data) {
    FieldSystem *fieldSystem = MapObject_GetFieldSystem(mapObject);

    MapObject_ClearSingleMovement(mapObject);
    MapObject_ClearEndMovement(mapObject);
    if (fieldSystem->followMon.unk1C == 1) {
        fieldSystem->followMon.unk1C = 2;
        return FALSE;
    }
    if (fieldSystem->followMon.unk1C == 2) {
        sub_02065D58(mapObject, data);
        if (fieldSystem->followMon.unk8 == MapObject_GetXCoord(mapObject) && fieldSystem->followMon.unkC == MapObject_GetZCoord(mapObject)) {
            fieldSystem->followMon.unk1C = 0;
            data->state = 3;
            if (sub_02069E14(mapObject) && !data->inGrass) {
                if (sub_02069EAC(mapObject)) {
                    ov01_0220329C(mapObject, 0);
                    sub_02069E84(mapObject, 0);
                } else {
                    sub_02069DC8(mapObject, FALSE);
                }
                sub_020664D8(mapObject);
            }
            if (sub_020623C8(sub_02065D78(mapObject))) {
                sub_02069E28(mapObject, (u8)PlayerAvatar_GetFacingDirection(FieldSystem_GetPlayerAvatar(fieldSystem)));
            }
            return TRUE;
        }
        if (sub_02065DF4(mapObject, data) == TRUE) {
            if (sub_02069E14(mapObject)) {
                if (sub_02069EAC(mapObject)) {
                    ov01_0220329C(mapObject, 0);
                    sub_02069E84(mapObject, 0);
                } else {
                    sub_02069DC8(mapObject, FALSE);
                }
                sub_020664D8(mapObject);
            }
            MapObject_SetSingleMovement(mapObject);
            fieldSystem->followMon.unk1C = 3;
            return TRUE;
        }
    } else if (fieldSystem->followMon.unk1C == 3) {
        fieldSystem->followMon.unk1C = 0;
    }
    return FALSE;
}

static BOOL sub_02065B70(LocalMapObject *mapObject, FollowPlayerMoveData *data) {
    MapObject_ClearSingleMovement(mapObject);
    MapObject_ClearEndMovement(mapObject);
    if (sub_02065D24(mapObject, data) == TRUE) {
        sub_02065D58(mapObject, data);
        if (sub_02069E14(mapObject)) {
            if (sub_02069EAC(mapObject)) {
                ov01_0220329C(mapObject, 0);
                sub_02069E84(mapObject, 0);
            } else {
                sub_02069DC8(mapObject, FALSE);
            }
            sub_020664D8(mapObject);
        }
        if (sub_02065F44(mapObject) == TRUE) {
            MapObject_SetSingleMovement(mapObject);
            data->state++;
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL sub_02065BE8(LocalMapObject *mapObject, FollowPlayerMoveData *data) {
    MapObject_ClearSingleMovement(mapObject);
    MapObject_ClearEndMovement(mapObject);
    if (sub_02065D24(mapObject, data) == TRUE) {
        sub_02065D58(mapObject, data);
        if (sub_02065FBC(mapObject) == TRUE) {
            MapObject_SetSingleMovement(mapObject);
            data->state++;
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL sub_02065C2C(LocalMapObject *mapObject, FollowPlayerMoveData *data) {
    if (sub_02062428(mapObject) == TRUE) {
        MapObject_ClearSingleMovement(mapObject);
        data->state = 0;
    }
    return FALSE;
}

static BOOL sub_02065C48(LocalMapObject *mapObject, FollowPlayerMoveData *data) {
    if (sub_02062428(mapObject) == TRUE) {
        data->stepCount++;
        if (data->stepCount >= 2) {
            MapObject_ClearSingleMovement(mapObject);
            data->state = 0;
            data->frameCount = 0;
            data->animSize = 0;
            return FALSE;
        }
        MapObject_ForceSetHeldMovement(mapObject, sub_02069ED4(mapObject));
    }
    data->frameCount++;
    return FALSE;
}

static BOOL sub_02065C90(LocalMapObject *mapObject, FollowPlayerMoveData *data) {
    PlayerAvatar *playerAvatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(mapObject));
    LocalMapObject *playerObj = PlayerAvatar_GetMapObject(playerAvatar);

    if (MapObject_TestFlagsBits(playerObj, MAPOBJECTFLAG_UNK4) == TRUE && MapObject_TestFlagsBits(playerObj, MAPOBJECTFLAG_UNK5) == TRUE) {
        data->state = 0;
    }
    if (PlayerAvatar_GetPlayerMoveState(playerAvatar) == 3) {
        data->state = 0;
    }
    return FALSE;
}

static BOOL sub_02065CD0(LocalMapObject *mapObject, FollowPlayerMoveData *data) {
    if (MapObjectManager_GetFirstActiveObjectWithMovement1(MapObject_GetManager(mapObject)) == NULL) {
        data->init = FALSE;
        return FALSE;
    }
    if (data->init == FALSE) {
        sub_02065CFC(mapObject, data);
    }
    return TRUE;
}

static void sub_02065CFC(LocalMapObject *mapObject, FollowPlayerMoveData *data) {
    PlayerAvatar *playerAvatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(mapObject));

    data->init = TRUE;
    data->x = PlayerAvatar_GetXCoord(playerAvatar);
    data->z = PlayerAvatar_GetZCoord(playerAvatar);
    data->unk8 = 0xFF;
}

static BOOL sub_02065D24(LocalMapObject *mapObject, FollowPlayerMoveData *data) {
    PlayerAvatar *playerAvatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(mapObject));

    if (playerAvatar != NULL) {
        int x = PlayerAvatar_GetXCoord(playerAvatar);
        int z = PlayerAvatar_GetZCoord(playerAvatar);

        if (x != data->x || z != data->z) {
            return TRUE;
        }
    }
    return FALSE;
}

static void sub_02065D58(LocalMapObject *mapObject, FollowPlayerMoveData *data) {
    PlayerAvatar *playerAvatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(mapObject));

    data->x = PlayerAvatar_GetXCoord(playerAvatar);
    data->z = PlayerAvatar_GetZCoord(playerAvatar);
}

static u32 sub_02065D78(LocalMapObject *mapObject) {
    FieldSystem *fieldSystem = MapObject_GetFieldSystem(mapObject);
    u32 movement;

    FieldSystem_GetPlayerAvatar(fieldSystem);
    movement = fieldSystem->followMon.unk4;
    switch (movement) {
    case 0x58:
        return 0x10;
    case 0x59:
        return 0x11;
    case 0x5A:
        return 0x12;
    case 0x5B:
        return 0x13;
    }
    return movement;
}

static u32 sub_02065DB4(LocalMapObject *mapObject) {
    u32 movement = MapObject_GetMovementCommand(PlayerAvatar_GetMapObject(FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(mapObject))));

    switch (movement) {
    case 0x58:
        return 0x10;
    case 0x59:
        return 0x11;
    case 0x5A:
        return 0x12;
    case 0x5B:
        return 0x13;
    }
    return movement;
}

static BOOL sub_02065DF4(LocalMapObject *mapObject, FollowPlayerMoveData *data) {
    u32 direction;
    u32 playerMovement;
    u8 state;
    int x;
    int z;
    int prevX;
    int prevZ;
    u32 movement;
    PlayerAvatar *playerAvatar;
    BOOL isJump;

    playerAvatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(mapObject));
    x = MapObject_GetXCoord(mapObject);
    z = MapObject_GetZCoord(mapObject);
    prevX = PlayerAvatar_GetPreviousXCoord(playerAvatar);
    prevZ = PlayerAvatar_GetPreviousZCoord(playerAvatar);

    if (x != prevX || z != prevZ) {
        playerMovement = sub_02065D78(mapObject);
        direction = sub_02061200(x, z, prevX, prevZ);
        movement = sub_02069EC0(mapObject);
        isJump = sub_020623C8(playerMovement);
        state = 1;
        if (movement != 0) {
            if (isJump) {
                movement = ov01_0220542C(movement, sub_020623D8(movement));
                data->animSize = (u16)sub_02066444(movement);
                sub_02069E50(mapObject, movement);
                state = 2;
                data->stepCount = 0;
                data->frameCount = 0;
                sub_02069E28(mapObject, (u8)PlayerAvatar_GetFacingDirection(playerAvatar));
            } else {
                if (!PlayerAvatar_CheckFlag6(playerAvatar)) {
                    return FALSE;
                }
                movement = ov01_0220542C(movement, playerMovement);
                data->animSize = (u16)sub_02066444(movement);
                sub_02069E50(mapObject, movement);
                state = 2;
                data->stepCount = 0;
                data->frameCount = 0;
                sub_02069E28(mapObject, 0);
            }
        } else if (isJump) {
            movement = sub_020623D8(direction);
            sub_02069E28(mapObject, (u8)PlayerAvatar_GetFacingDirection(playerAvatar));
        } else {
            movement = sub_0206234C(direction, playerMovement);
        }
        MapObject_ForceSetHeldMovement(mapObject, movement);
        data->state = state;
        return TRUE;
    }
    return FALSE;
}

static BOOL sub_02065F44(LocalMapObject *mapObject) {
    PlayerAvatar *playerAvatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(mapObject));
    int x = MapObject_GetXCoord(mapObject);
    int z = MapObject_GetZCoord(mapObject);
    int prevX = PlayerAvatar_GetPreviousXCoord(playerAvatar);
    int prevZ = PlayerAvatar_GetPreviousZCoord(playerAvatar);
    u32 playerMovement;
    u32 direction;

    if (x != prevX || z != prevZ) {
        playerMovement = sub_02065DB4(mapObject);
        direction = sub_02061200(x, z, prevX, prevZ);
        if (playerMovement == 0xFF) {
            GF_ASSERT(FALSE);
            return FALSE;
        }
        MapObject_ForceSetHeldMovement(mapObject, sub_0206234C(direction, playerMovement));
        return TRUE;
    }
    return FALSE;
}

static BOOL sub_02065FBC(LocalMapObject *mapObject) {
    PlayerAvatar *playerAvatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(mapObject));
    u32 playerMovement;

    MapObject_GetXCoord(mapObject);
    MapObject_GetZCoord(mapObject);
    PlayerAvatar_GetPreviousXCoord(playerAvatar);
    PlayerAvatar_GetPreviousZCoord(playerAvatar);
    playerMovement = sub_02065DB4(mapObject);
    if (playerMovement == 0xFF) {
        return FALSE;
    }
    MapObject_ForceSetHeldMovement(mapObject, playerMovement);
    return TRUE;
}

void sub_02065FFC(LocalMapObject *mapObject) {
    FollowObjectMoveData *data = (FollowObjectMoveData *)sub_0205F370(mapObject, sizeof(FollowObjectMoveData));

    sub_02066150(mapObject, data);
    sub_0205F328(mapObject, 0);
    MapObject_ClearSingleMovement(mapObject);
    data->init = FALSE;
}

void sub_02066024(LocalMapObject *mapObject) {
    FollowObjectMoveData *data = (FollowObjectMoveData *)sub_0205F394(mapObject);

    if (sub_02066150(mapObject, data)) {
        while (_020FE3D4[data->state](mapObject, data) == TRUE) {}
    }
}

void sub_02066054(LocalMapObject *mapObject) {
}

void sub_02066058(LocalMapObject *mapObject) {
    FollowObjectMoveData *data = (FollowObjectMoveData *)sub_0205F394(mapObject);
    data->init = FALSE;
}

static BOOL sub_02066064(LocalMapObject *mapObject, FollowObjectMoveData *data) {
    MapObject_ClearSingleMovement(mapObject);
    MapObject_ClearEndMovement(mapObject);
    if (sub_020661F0(mapObject, data) == TRUE && sub_0206623C(mapObject, data) == TRUE) {
        MapObject_SetSingleMovement(mapObject);
        data->state++;
        return TRUE;
    }
    return FALSE;
}

static BOOL sub_020660A0(LocalMapObject *mapObject, FollowObjectMoveData *data) {
    if (sub_02062428(mapObject) == FALSE) {
        return FALSE;
    }
    MapObject_ClearSingleMovement(mapObject);
    data->state = 0;
    return FALSE;
}

LocalMapObject *sub_020660C0(LocalMapObject *mapObject) {
    s32 index = 0;
    LocalMapObject *other;
    u32 type = MapObject_GetType(mapObject);
    u32 mapId = MapObject_GetMapID(mapObject);
    u32 group = sub_02064518(mapObject);
    MapObjectManager *manager = MapObject_GetManager(mapObject);

    switch (type) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &other, &index, MAPOBJECTFLAG_ACTIVE) == TRUE) {
            if (mapObject != other && mapId == MapObject_GetMapID(other) && group == sub_02064518(other)) {
                return other;
            }
        }
        break;
    }
    return NULL;
}

static BOOL sub_02066150(LocalMapObject *mapObject, FollowObjectMoveData *data) {
    s32 index;
    LocalMapObject *other;
    u32 mapId;
    u32 group;
    MapObjectManager *manager;

    manager = MapObject_GetManager(mapObject);
    index = 0;
    mapId = MapObject_GetMapID(mapObject);
    group = sub_02064518(mapObject);

    while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &other, &index, MAPOBJECTFLAG_ACTIVE) == TRUE) {
        if (mapObject != other && mapId == MapObject_GetMapID(other) && group == sub_02064518(other)) {
            if (data->init == FALSE) {
                sub_020661CC(mapObject, data, other);
            }
            return TRUE;
        }
    }
    data->init = FALSE;
    return FALSE;
}

static void sub_020661CC(LocalMapObject *mapObject, FollowObjectMoveData *data, LocalMapObject *target) {
    data->init = TRUE;
    data->x = MapObject_GetXCoord(target);
    data->z = MapObject_GetZCoord(target);
    data->unk6 = 0xFF;
    data->target = target;
}

static BOOL sub_020661F0(LocalMapObject *mapObject, FollowObjectMoveData *data) {
    LocalMapObject *target = data->target;
    int x = MapObject_GetXCoord(mapObject);
    int z = MapObject_GetZCoord(mapObject);
    int prevX = MapObject_GetPreviousXCoord(target);
    int prevZ = MapObject_GetPreviousZCoord(target);

    if (x != prevX || z != prevZ) {
        if (MapObject_CheckSingleMovement(target) == TRUE || MapObject_GetFlagsBitsMask(target, (MapObjectFlagBits)0x1840) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL sub_0206623C(LocalMapObject *mapObject, FollowObjectMoveData *data) {
    int x = MapObject_GetXCoord(mapObject);
    int z = MapObject_GetZCoord(mapObject);
    int targetX = MapObject_GetXCoord(data->target);
    int targetZ = MapObject_GetZCoord(data->target);
    int prevX = MapObject_GetPreviousXCoord(data->target);
    int prevZ = MapObject_GetPreviousZCoord(data->target);
    u32 direction;

    if (x == targetX && z == targetZ) {
        return FALSE;
    }
    direction = sub_02061200(x, z, prevX, prevZ);
    x += GetDeltaXByFacingDirection(direction);
    z += GetDeltaYByFacingDirection(direction);
    if (x != targetX || z != targetZ) {
        MapObject_ForceSetHeldMovement(mapObject, sub_0206234C(direction, 12));
        return TRUE;
    }
    return FALSE;
}

static void sub_020662C4(LocalMapObject *mapObject, u8 type) {
    EffectObjectMoveData *data = (EffectObjectMoveData *)sub_0205F370(mapObject, sizeof(EffectObjectMoveData));
    VecFx32 facing;

    data->type = type;
    sub_0205F328(mapObject, 0);
    MapObject_ClearSingleMovement(mapObject);
    MapObject_SetFlagsBits(mapObject, MAPOBJECTFLAG_UNK20);
    facing = _020FE3DC;
    MapObject_SetFacingVector(mapObject, &facing);
}

void sub_0206630C(LocalMapObject *mapObject) {
    sub_020662C4(mapObject, 0);
}

void sub_02066318(LocalMapObject *mapObject) {
    sub_020662C4(mapObject, 1);
}

void sub_02066324(LocalMapObject *mapObject) {
    sub_020662C4(mapObject, 2);
}

void sub_02066330(LocalMapObject *mapObject) {
    sub_020662C4(mapObject, 3);
}

void sub_0206633C(LocalMapObject *mapObject) {
    EffectObjectMoveData *data = (EffectObjectMoveData *)sub_0205F394(mapObject);

    while (_020FE3CC[data->state](mapObject, data) == TRUE) {}
}

void sub_02066360(LocalMapObject *mapObject) {
    void *effect = sub_0206642C(mapObject);

    if (effect != NULL) {
        ov01_021F1640((int)effect);
    }
}

void sub_02066370(LocalMapObject *mapObject) {
    EffectObjectMoveData *data = (EffectObjectMoveData *)sub_0205F394(mapObject);
    VecFx32 facing;

    data->state = 0;
    sub_02066420(mapObject, NULL);
    if (data->noEffect == FALSE) {
        facing = _020FE3E8;
        MapObject_SetFacingVector(mapObject, &facing);
        MapObject_SetFlagsBits(mapObject, MAPOBJECTFLAG_UNK20);
    }
}

static BOOL sub_020663B4(LocalMapObject *mapObject, EffectObjectMoveData *data) {
    if (data->noEffect == FALSE) {
        sub_02066420(mapObject, ov01_021FFF5C(mapObject, data->type));
    }
    MapObject_ClearSingleMovement(mapObject);
    MapObject_ClearEndMovement(mapObject);
    data->state++;
    return FALSE;
}

static BOOL sub_020663E4(LocalMapObject *mapObject, EffectObjectMoveData *data) {
    if (data->noEffect == FALSE) {
        if (sub_0206642C(mapObject) == NULL && sub_0205F73C(mapObject) == TRUE) {
            sub_02066420(mapObject, ov01_021FFF5C(mapObject, data->type));
        }
        MapObject_SetFlagsBits(mapObject, MAPOBJECTFLAG_UNK20);
    }
    return FALSE;
}

void sub_02066420(LocalMapObject *mapObject, void *effect) {
    EffectObjectMoveData *data = (EffectObjectMoveData *)sub_0205F394(mapObject);
    data->effect = effect;
}

void *sub_0206642C(LocalMapObject *mapObject) {
    EffectObjectMoveData *data = (EffectObjectMoveData *)sub_0205F394(mapObject);
    return data->effect;
}

void sub_02066438(LocalMapObject *mapObject) {
    EffectObjectMoveData *data = (EffectObjectMoveData *)sub_0205F394(mapObject);
    data->noEffect = TRUE;
}

static int sub_02066444(u32 movement) {
    MovementList large = _020FE3F4;
    MovementList medium = _020FE434;
    MovementList small = _020FE444;
    u8 i;

    for (i = 0; i < 4; i++) {
        if (movement == large.movements[i]) {
            return 3;
        }
    }
    for (i = 0; i < 4; i++) {
        if (movement == medium.movements[i]) {
            return 2;
        }
    }
    for (i = 0; i < 4; i++) {
        if (movement == small.movements[i]) {
            return 1;
        }
    }
    GF_ASSERT(FALSE);
    return 0;
}

static void sub_020664D8(LocalMapObject *mapObject) {
    FieldSystem *fieldSystem = MapObject_GetFieldSystem(mapObject);
    int x = MapObject_GetXCoord(mapObject);
    int z = MapObject_GetZCoord(mapObject);
    u32 behavior = GetMetatileBehavior(fieldSystem, x, z);
    u8 direction;

    if (MetatileBehavior_IsTallGrass(behavior) == TRUE) {
        ov01_021FF070(mapObject, 0);
    } else if (MetatileBehavior_IsVeryTallGrass(behavior) == TRUE) {
        ov01_021FF8F0(mapObject, 0);
    }
    if (ov01_022055DC(mapObject)) {
        direction = MapObject_GetFacingDirection(mapObject);
        if (direction == DIR_WEST || direction == DIR_EAST) {
            ov01_02205604(mapObject, &x, &z);
            behavior = GetMetatileBehavior(fieldSystem, x, z);
            if (MetatileBehavior_IsTallGrass(behavior) == TRUE) {
                ov01_021FF0E4(mapObject, 1, x, z, 1);
            } else if (MetatileBehavior_IsVeryTallGrass(behavior) == TRUE) {
                ov01_021FF964(mapObject, 1, x, z, 1);
            }
        }
    }
}
