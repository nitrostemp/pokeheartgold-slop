#include "unk_02061284.h"

#include "global.h"

#include "field_system.h"
#include "map_object.h"
#include "math_util.h"
#include "player_avatar.h"
#include "unk_0205CB48.h"
#include "unk_0205FD20.h"

typedef struct UnkStruct_020FD838 {
    int id;
    const int *list;
} UnkStruct_020FD838;

typedef struct UnkStruct_020614F4 {
    int minX;
    int minZ;
    int maxX;
    int maxZ;
} UnkStruct_020614F4;

typedef struct UnkStruct_02062064 {
    s8 unk_00;
    s8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_02062064;

typedef struct UnkStruct_02061284 {
    u16 unk_00;
    s16 unk_02;
    int unk_04;
} UnkStruct_02061284;

typedef struct UnkStruct_020613D0 {
    s16 unk_00;
    s16 unk_02;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_020613D0;

typedef struct UnkStruct_02061648 {
    int unk_00;
    int unk_04;
} UnkStruct_02061648;

typedef struct UnkStruct_020616C0 {
    s8 unk_00;
    s8 unk_01;
    s8 unk_02;
    s8 unk_03;
    int unk_04;
} UnkStruct_020616C0;

typedef struct UnkStruct_0206197C {
    s16 unk_00;
    s16 unk_02;
    UnkStruct_02062064 unk_04;
} UnkStruct_0206197C;

typedef struct UnkStruct_02061AEC {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    int unk_04;
    UnkStruct_02062064 unk_08;
} UnkStruct_02061AEC;

u32 sub_02060BB8(LocalMapObject *object, u32 direction);
int sub_02061200(int x, int z, int targetX, int targetZ);
u32 sub_0206234C(u32 direction, u32 movement);
BOOL sub_02062428(LocalMapObject *object);
void MapObject_ForceSetHeldMovement(LocalMapObject *object, u32 movement);

static void sub_02061284(LocalMapObject *object, int param1);
static void sub_020613D0(LocalMapObject *object, int param1, int param2, int param3);
static void sub_020614F4(LocalMapObject *object, UnkStruct_020614F4 *bounds);
static int sub_020615F0(LocalMapObject *object, int direction);
static void sub_02061648(LocalMapObject *object, int direction);
static void sub_020616C0(LocalMapObject *object, int param1);
static int sub_02061720(LocalMapObject *object, UnkStruct_020616C0 *data);
static int sub_02061754(LocalMapObject *object, UnkStruct_020616C0 *data);
static int sub_02061770(LocalMapObject *object, UnkStruct_020616C0 *data);
static int sub_020617AC(LocalMapObject *object, UnkStruct_020616C0 *data);
static int sub_02061874(LocalMapObject *object, UnkStruct_020616C0 *data);
static int sub_02061894(LocalMapObject *object, UnkStruct_020616C0 *data);
static int sub_020618B0(LocalMapObject *object, UnkStruct_020616C0 *data);
static int sub_020618C8(LocalMapObject *object, UnkStruct_020616C0 *data);
static int sub_020619C0(LocalMapObject *object, UnkStruct_0206197C *data);
static int sub_020619FC(LocalMapObject *object, UnkStruct_0206197C *data);
static int sub_02061ABC(LocalMapObject *object, UnkStruct_0206197C *data);
static void sub_02061AEC(LocalMapObject *object, int param1, int param2, int param3);
static int sub_02061C40(LocalMapObject *object, UnkStruct_02061AEC *data);
static int sub_02061D50(LocalMapObject *object, UnkStruct_02061AEC *data);
static int sub_02061E00(const int *list, int terminator);
static int sub_02061E20(const int *list, int terminator);
static int sub_02061E44(int id, int terminator);
static const int *sub_02061E6C(int id);
static int sub_02061E90(LocalMapObject *object);
static int sub_02061F5C(LocalMapObject *object, int id, int terminator);
static int sub_02062050(LocalMapObject *object);
static void sub_02062064(LocalMapObject *object, UnkStruct_02062064 *data);
static void sub_0206207C(LocalMapObject *object, UnkStruct_02062064 *data);
static void sub_020620F8(LocalMapObject *object, UnkStruct_02062064 *data);

static int (*const _020FD4EC[])(LocalMapObject *object, UnkStruct_02061AEC *data);
static int (*const _020FD548[])(LocalMapObject *object, UnkStruct_0206197C *data);
static int (*const _020FD5A0[])(LocalMapObject *object, UnkStruct_020616C0 *data);
static int (*const _020FD5D0[])(LocalMapObject *object, UnkStruct_020616C0 *data);
static const int _020FD7B8[];
static const int _020FD7E0[2][4];
static const int _020FD800[];
static const UnkStruct_020FD838 _020FD838[];

static void sub_02061284(LocalMapObject *object, int param1) {
    UnkStruct_02061284 *data = (UnkStruct_02061284 *)sub_0205F370(object, sizeof(UnkStruct_02061284));
    data->unk_02 = sub_02061E20(_020FD7B8, -1);
    data->unk_04 = param1;
    sub_0205F328(object, 0);
    MapObject_ClearSingleMovement(object);
}

void sub_020612B4(LocalMapObject *object) {
    sub_02061284(object, 0);
}

void sub_020612C0(LocalMapObject *object) {
    sub_02061284(object, 1);
}

void sub_020612CC(LocalMapObject *object) {
    sub_02061284(object, 2);
}

void sub_020612D8(LocalMapObject *object) {
    sub_02061284(object, 3);
}

void sub_020612E4(LocalMapObject *object) {
    sub_02061284(object, 4);
}

void sub_020612F0(LocalMapObject *object) {
    sub_02061284(object, 5);
}

void sub_020612FC(LocalMapObject *object) {
    sub_02061284(object, 6);
}

void sub_02061308(LocalMapObject *object) {
    sub_02061284(object, 7);
}

void sub_02061314(LocalMapObject *object) {
    sub_02061284(object, 8);
}

void sub_02061320(LocalMapObject *object) {
    sub_02061284(object, 9);
}

void sub_0206132C(LocalMapObject *object) {
    sub_02061284(object, 10);
}

void sub_02061338(LocalMapObject *object) {
    UnkStruct_02061284 *data = (UnkStruct_02061284 *)sub_0205F394(object);
    int direction = sub_02061F5C(object, data->unk_04, -1);

    if (direction != -1) {
        MapObject_SetFacingDirection(object, direction);
    } else {
        switch (data->unk_00) {
        case 0:
            data->unk_02--;
            if (data->unk_02 <= 0) {
                data->unk_02 = sub_02061E20(_020FD7B8, -1);
                MapObject_SetFacingDirection(object, sub_02061E44(data->unk_04, -1));
            }
        }
    }

    sub_02060F78(object);
}

void sub_0206139C(LocalMapObject *object) {
}

void sub_020613A0(LocalMapObject *object) {
    sub_020613D0(object, 12, 11, 0);
}

void sub_020613B0(LocalMapObject *object) {
    sub_020613D0(object, 12, 12, 0);
}

void sub_020613C0(LocalMapObject *object) {
    sub_020613D0(object, 12, 13, 0);
}

static void sub_020613D0(LocalMapObject *object, int param1, int param2, int param3) {
    UnkStruct_020613D0 *data = (UnkStruct_020613D0 *)sub_0205F370(object, sizeof(UnkStruct_020613D0));

    data->unk_04 = param3;
    data->unk_08 = param1;
    data->unk_0C = param2;
    sub_0205F328(object, 0);
    MapObject_ClearSingleMovement(object);
}

void sub_020613F8(LocalMapObject *object) {
    int direction;
    UnkStruct_020613D0 *data = (UnkStruct_020613D0 *)sub_0205F394(object);

    switch (data->unk_00) {
    case 0:
        MapObject_ClearSingleMovement(object);
        MapObject_ClearEndMovement(object);
        direction = MapObject_GetFacingDirection(object);
        direction = sub_0206234C(direction, 0);
        MapObject_ForceSetHeldMovement(object, direction);
        data->unk_00++;
        break;
    case 1:
        if (sub_02062428(object) == FALSE) {
            break;
        }
        data->unk_02 = sub_02061E20(_020FD7B8, -1);
        data->unk_00++;
    case 2:
        data->unk_02--;
        if (data->unk_02) {
            break;
        }
        data->unk_00++;
    case 3:
        direction = sub_02061E44(data->unk_0C, -1);
        MapObject_SetOrQueueFacing(object, direction);
        if (data->unk_04 == 1) {
            if (sub_020615F0(object, direction) == 0) {
                data->unk_00 = 0;
                break;
            }
        }
        if (sub_02060BB8(object, direction) != 0) {
            data->unk_00 = 0;
            break;
        }
        direction = sub_0206234C(direction, data->unk_08);
        MapObject_ForceSetHeldMovement(object, direction);
        MapObject_SetSingleMovement(object);
        data->unk_00++;
    case 4:
        if (sub_02062428(object) == FALSE) {
            break;
        }
        MapObject_ClearSingleMovement(object);
        data->unk_00 = 0;
    }
}

static void sub_020614F4(LocalMapObject *object, UnkStruct_020614F4 *bounds) {
    int movement, initialX, initialZ, rangeX, rangeZ;

    initialX = MapObject_GetInitialX(object);
    initialZ = MapObject_GetInitialZ(object);
    rangeX = MapObject_GetXRange(object);
    rangeZ = MapObject_GetYRange(object);
    movement = MapObject_GetMovement(object);

    switch (movement) {
    case 6:
        bounds->minX = initialX - rangeX;
        bounds->maxX = initialX;
        bounds->minZ = initialZ - rangeZ;
        bounds->maxZ = initialZ;
        break;
    case 7:
        bounds->minX = initialX;
        bounds->maxX = initialX + rangeX;
        bounds->minZ = initialZ - rangeZ;
        bounds->maxZ = initialZ;
        break;
    case 8:
        bounds->minX = initialX - rangeX;
        bounds->maxX = initialX;
        bounds->minZ = initialZ;
        bounds->maxZ = initialZ + rangeZ;
        break;
    case 9:
        bounds->minX = initialX;
        bounds->maxX = initialX + rangeX;
        bounds->minZ = initialZ;
        bounds->maxZ = initialZ + rangeZ;
        break;
    case 10:
        bounds->minX = initialX - rangeX;
        bounds->maxX = initialX;
        bounds->minZ = initialZ - rangeZ;
        bounds->maxZ = initialZ + rangeZ;
        break;
    case 11:
        bounds->minX = initialX;
        bounds->maxX = initialX + rangeX;
        bounds->minZ = initialZ - rangeZ;
        bounds->maxZ = initialZ + rangeZ;
        break;
    case 12:
        bounds->minX = initialX - rangeX;
        bounds->maxX = initialX + rangeX;
        bounds->minZ = initialZ - rangeZ;
        bounds->maxZ = initialZ;
        break;
    case 13:
        bounds->minX = initialX - rangeX;
        bounds->maxX = initialX + rangeX;
        bounds->minZ = initialZ;
        bounds->maxZ = initialZ + rangeZ;
        break;
    default:
        GF_ASSERT(FALSE);
    }
}

static int sub_020615F0(LocalMapObject *object, int direction) {
    int x, z;
    UnkStruct_020614F4 bounds;

    sub_020614F4(object, &bounds);

    x = MapObject_GetXCoord(object) + GetDeltaXByFacingDirection(direction);
    z = MapObject_GetZCoord(object) + GetDeltaYByFacingDirection(direction);

    if (bounds.minX > x || bounds.maxX < x) {
        return 0;
    }
    if (bounds.minZ > z || bounds.maxZ < z) {
        return 0;
    }
    return 1;
}

static void sub_02061648(LocalMapObject *object, int direction) {
    UnkStruct_02061648 *data = (UnkStruct_02061648 *)sub_0205F370(object, sizeof(UnkStruct_02061648));
    data->unk_00 = direction;
    sub_0205F328(object, 0);
    MapObject_ClearSingleMovement(object);
    sub_02060F78(object);
}

void sub_0206166C(LocalMapObject *object) {
    UnkStruct_02061648 *data = (UnkStruct_02061648 *)sub_0205F394(object);

    switch (data->unk_04) {
    case 0:
        MapObject_SetFacingDirection(object, data->unk_00);
        data->unk_04++;
        break;
    case 1:
        break;
    }
}

void sub_02061690(LocalMapObject *object) {
    sub_02061648(object, 0);
}

void sub_0206169C(LocalMapObject *object) {
    sub_02061648(object, 1);
}

void sub_020616A8(LocalMapObject *object) {
    sub_02061648(object, 2);
}

void sub_020616B4(LocalMapObject *object) {
    sub_02061648(object, 3);
}

static void sub_020616C0(LocalMapObject *object, int param1) {
    UnkStruct_020616C0 *data = (UnkStruct_020616C0 *)sub_0205F370(object, sizeof(UnkStruct_020616C0));
    data->unk_00 = param1;
    sub_0205F328(object, 0);
    MapObject_ClearSingleMovement(object);
    sub_02060F78(object);
}

void sub_020616E4(LocalMapObject *object) {
    sub_020616C0(object, 2);
}

void sub_020616F0(LocalMapObject *object) {
    sub_020616C0(object, 3);
}

void sub_020616FC(LocalMapObject *object) {
    UnkStruct_020616C0 *data = (UnkStruct_020616C0 *)sub_0205F394(object);

    while (_020FD5D0[data->unk_02](object, data) == 1) {
    }
}

static int sub_02061720(LocalMapObject *object, UnkStruct_020616C0 *data) {
    int direction = sub_02061F5C(object, 38, -1);

    if (direction == -1) {
        direction = MapObject_GetFacingDirection(object);
    }

    direction = sub_0206234C(direction, 0);
    MapObject_ForceSetHeldMovement(object, direction);
    data->unk_02 = 1;
    return 1;
}

static int sub_02061754(LocalMapObject *object, UnkStruct_020616C0 *data) {
    if (sub_02062428(object) == FALSE) {
        return 0;
    }

    data->unk_04 = 0;
    data->unk_02 = 2;
    return 1;
}

static int sub_02061770(LocalMapObject *object, UnkStruct_020616C0 *data) {
    if (data->unk_04) {
        if (sub_02061F5C(object, 38, -1) != -1) {
            data->unk_02 = 0;
            return 1;
        }
    }

    data->unk_04++;
    if (data->unk_04 < 24) {
        return 0;
    }

    data->unk_02 = 3;
    return 1;
}

static int sub_020617AC(LocalMapObject *object, UnkStruct_020616C0 *data) {
    int i, direction, *list;
    int list1[5] = { 0, 2, 1, 3, -1 };
    int list2[5] = { 0, 3, 1, 2, -1 };

    if (data->unk_00 == 2) {
        list = list1;
    } else {
        list = list2;
    }

    direction = MapObject_GetFacingDirection(object);

    for (i = 0; list[i] != -1; i++) {
        if (direction == list[i]) {
            break;
        }
    }

    GF_ASSERT(list[i] != -1);

    i++;
    if (list[i] == -1) {
        i = 0;
    }

    direction = list[i];
    MapObject_SetFacingDirection(object, direction);
    data->unk_02 = 0;
    return 1;
}

static int (*const _020FD5D0[])(LocalMapObject *object, UnkStruct_020616C0 *data) = {
    sub_02061720,
    sub_02061754,
    sub_02061770,
    sub_020617AC,
};

void sub_02061844(LocalMapObject *object) {
    sub_020616C0(object, 3);
}

void sub_02061850(LocalMapObject *object) {
    UnkStruct_020616C0 *data = (UnkStruct_020616C0 *)sub_0205F394(object);

    while (_020FD5A0[data->unk_02](object, data) == 1) {
    }
}

static int sub_02061874(LocalMapObject *object, UnkStruct_020616C0 *data) {
    int direction = MapObject_GetFacingDirection(object);

    direction = sub_0206234C(direction, 0);
    MapObject_ForceSetHeldMovement(object, direction);
    data->unk_02 = 1;
    return 1;
}

static int sub_02061894(LocalMapObject *object, UnkStruct_020616C0 *data) {
    if (sub_02062428(object) == FALSE) {
        return 0;
    }

    data->unk_04 = 0;
    data->unk_02 = 2;
    return 1;
}

static int sub_020618B0(LocalMapObject *object, UnkStruct_020616C0 *data) {
    data->unk_04++;
    if (data->unk_04 < 24) {
        return 0;
    }

    data->unk_02 = 3;
    return 1;
}

static int sub_020618C8(LocalMapObject *object, UnkStruct_020616C0 *data) {
    int i, direction, *list;
    int list1[5] = { 0, 2, 1, 3, -1 };
    int list2[5] = { 0, 3, 1, 2, -1 };

    if (data->unk_00 == 2) {
        list = list1;
    } else {
        list = list2;
    }

    direction = MapObject_GetFacingDirection(object);

    for (i = 0; list[i] != -1; i++) {
        if (direction == list[i]) {
            break;
        }
    }

    GF_ASSERT(list[i] != -1);

    i++;
    if (list[i] == -1) {
        i = 0;
    }

    direction = list[i];
    MapObject_SetFacingDirection(object, direction);

    {
        int facing = MapObject_GetFacingDirection(object);
        int initial = MapObject_GetInitialFacingDirection(object);

        if (facing == initial) {
            data->unk_00 = sub_020611F4(data->unk_00);
        }
    }

    data->unk_02 = 0;
    return 1;
}

static int (*const _020FD5A0[])(LocalMapObject *object, UnkStruct_020616C0 *data) = {
    sub_02061874,
    sub_02061894,
    sub_020618B0,
    sub_020618C8,
};

void sub_0206197C(LocalMapObject *object) {
    UnkStruct_0206197C *data = (UnkStruct_0206197C *)sub_0205F370(object, sizeof(UnkStruct_0206197C));

    if (sub_02062050(object) == 1) {
        sub_02062064(object, &data->unk_04);
    }
}

void sub_0206199C(LocalMapObject *object) {
    UnkStruct_0206197C *data = (UnkStruct_0206197C *)sub_0205F394(object);

    while (_020FD548[data->unk_00](object, data) == 1) {
    }
}

static int sub_020619C0(LocalMapObject *object, UnkStruct_0206197C *data) {
    int direction = MapObject_GetInitialFacingDirection(object);

    if (data->unk_02 == 1) {
        direction = sub_020611F4(direction);
    }

    MapObject_SetNextFacingDirection(object, direction);

    if (sub_02062050(object) == 0) {
        MapObject_SetFacingDirection(object, direction);
    }

    data->unk_00 = 1;
    return 1;
}

static int sub_020619FC(LocalMapObject *object, UnkStruct_0206197C *data) {
    if (data->unk_02) {
        int initialX, initialZ, x, z;

        initialX = MapObject_GetInitialX(object);
        initialZ = MapObject_GetInitialZ(object);
        x = MapObject_GetXCoord(object);
        z = MapObject_GetZCoord(object);

        if (initialX == x && initialZ == z) {
            int direction = sub_020611F4(MapObject_GetNextFacingDirection(object));

            MapObject_SetNextFacingDirection(object, direction);

            if (sub_02062050(object) == 0) {
                MapObject_SetFacingDirection(object, direction);
            }

            data->unk_02 = 0;
        }
    }

    {
        int direction, movement;
        u32 collision;

        direction = MapObject_GetNextFacingDirection(object);
        collision = sub_02060BB8(object, direction);

        if (collision & 1) {
            data->unk_02 = 1;
            direction = sub_020611F4(direction);
            collision = sub_02060BB8(object, direction);
        }

        movement = 12;
        if (collision != 0) {
            movement = 32;
        }

        movement = sub_0206234C(direction, movement);
        MapObject_ForceSetHeldMovement(object, movement);

        if (sub_02062050(object) == 1) {
            sub_0206207C(object, &data->unk_04);
        }
    }

    MapObject_SetSingleMovement(object);
    data->unk_00 = 2;
    return 1;
}

static int sub_02061ABC(LocalMapObject *object, UnkStruct_0206197C *data) {
    if (sub_02062428(object) == TRUE) {
        MapObject_ClearSingleMovement(object);

        if (sub_02062050(object) == 1) {
            sub_020620F8(object, &data->unk_04);
        }

        data->unk_00 = 0;
    }

    return 0;
}

static int (*const _020FD548[])(LocalMapObject *object, UnkStruct_0206197C *data) = {
    sub_020619C0,
    sub_020619FC,
    sub_02061ABC,
};

static void sub_02061AEC(LocalMapObject *object, int param1, int param2, int param3) {
    UnkStruct_02061AEC *data = (UnkStruct_02061AEC *)sub_0205F370(object, sizeof(UnkStruct_02061AEC));

    data->unk_02 = param1;
    data->unk_03 = param2;
    data->unk_04 = param3;

    if (sub_02062050(object) == 1) {
        sub_02062064(object, &data->unk_08);
    }
}

void sub_02061B1C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 14);
}

void sub_02061B2C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 15);
}

void sub_02061B3C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 16);
}

void sub_02061B4C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 17);
}

void sub_02061B5C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 18);
}

void sub_02061B6C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 19);
}

void sub_02061B7C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 20);
}

void sub_02061B8C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 21);
}

void sub_02061B9C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 22);
}

void sub_02061BAC(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 23);
}

void sub_02061BBC(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 24);
}

void sub_02061BCC(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 25);
}

void sub_02061BDC(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 26);
}

void sub_02061BEC(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 27);
}

void sub_02061BFC(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 28);
}

void sub_02061C0C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 29);
}

void sub_02061C1C(LocalMapObject *object) {
    UnkStruct_02061AEC *data = (UnkStruct_02061AEC *)sub_0205F394(object);

    while (_020FD4EC[data->unk_00](object, data) == 1) {
    }
}

static int sub_02061C40(LocalMapObject *object, UnkStruct_02061AEC *data) {
    if (data->unk_01 == data->unk_02) {
        if (data->unk_03 == 0) {
            int initialX = MapObject_GetInitialX(object);
            int x = MapObject_GetXCoord(object);

            if (initialX == x) {
                data->unk_01++;
            }
        } else {
            int initialZ = MapObject_GetInitialZ(object);
            int z = MapObject_GetZCoord(object);

            if (initialZ == z) {
                data->unk_01++;
            }
        }
    }

    if (data->unk_01 == 3) {
        int initialX = MapObject_GetInitialX(object);
        int initialZ = MapObject_GetInitialZ(object);
        int x = MapObject_GetXCoord(object);
        int z = MapObject_GetZCoord(object);

        if (initialX == x && initialZ == z) {
            data->unk_01 = 0;
        }
    }

    {
        const int *list;
        int direction, movement;
        u32 collision;

        list = sub_02061E6C(data->unk_04);
        direction = list[data->unk_01];

        MapObject_SetNextFacingDirection(object, direction);

        if (sub_02062050(object) == 0) {
            MapObject_SetFacingDirection(object, direction);
        }

        collision = sub_02060BB8(object, direction);

        if (collision & 1) {
            data->unk_01++;
            direction = list[data->unk_01];

            MapObject_SetNextFacingDirection(object, direction);

            if (sub_02062050(object) == 0) {
                MapObject_SetFacingDirection(object, direction);
            }

            collision = sub_02060BB8(object, direction);
        }

        movement = 12;
        if (collision != 0) {
            movement = 32;
        }

        movement = sub_0206234C(direction, movement);
        MapObject_ForceSetHeldMovement(object, movement);

        if (sub_02062050(object) == 1) {
            sub_0206207C(object, &data->unk_08);
        }
    }

    MapObject_SetSingleMovement(object);
    data->unk_00 = 1;
    return 1;
}

static int sub_02061D50(LocalMapObject *object, UnkStruct_02061AEC *data) {
    if (sub_02062428(object) == TRUE) {
        MapObject_ClearSingleMovement(object);

        if (sub_02062050(object) == 1) {
            sub_020620F8(object, &data->unk_08);
        }

        data->unk_00 = 0;
    }

    return 0;
}

static int (*const _020FD4EC[])(LocalMapObject *object, UnkStruct_02061AEC *data) = {
    sub_02061C40,
    sub_02061D50,
};

void sub_02061D80(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 30);
}

void sub_02061D90(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 31);
}

void sub_02061DA0(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 32);
}

void sub_02061DB0(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 33);
}

void sub_02061DC0(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 34);
}

void sub_02061DD0(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 35);
}

void sub_02061DE0(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 36);
}

void sub_02061DF0(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 37);
}

static int sub_02061E00(const int *list, int terminator) {
    int i = 0;

    while (list[i] != terminator) {
        i++;
    }

    GF_ASSERT(i);
    return i;
}

static int sub_02061E20(const int *list, int terminator) {
    return list[LCRandom() % sub_02061E00(list, terminator)];
}

static int sub_02061E44(int id, int terminator) {
    const int *list = sub_02061E6C(id);
    return list[LCRandom() % sub_02061E00(list, terminator)];
}

static const int *sub_02061E6C(int id) {
    const UnkStruct_020FD838 *entry = _020FD838;

    while (entry->id != 39) {
        if (entry->id == id) {
            return entry->list;
        }

        entry++;
    }

    GF_ASSERT(FALSE);
    return NULL;
}

static int sub_02061E90(LocalMapObject *object) {
    int type = MapObject_GetType(object);

    if (type != 1 && type != 2) {
        return -1;
    }

    {
        FieldSystem *fieldSystem = MapObject_GetFieldSystem(object);
        PlayerAvatar *playerAvatar = FieldSystem_GetPlayerAvatar(fieldSystem);

        if (sub_0205DE98(playerAvatar) == FALSE) {
            return -1;
        }

        {
            int movement, i = 0;

            type = MapObject_GetMovement(object);

            do {
                movement = _020FD800[i++];
                if (movement == type) {
                    break;
                }
            } while (movement != 0xFF);

            if (type != movement) {
                return -1;
            }
        }

        {
            LocalMapObject *playerObject = PlayerAvatar_GetMapObject(playerAvatar);
            int playerY = MapObject_GetPositionVectorYCoordUInt(playerObject);
            int y = MapObject_GetPositionVectorYCoordUInt(object);

            if (playerY != y) {
                return -1;
            }
        }

        {
            int playerX = PlayerAvatar_GetXCoord(playerAvatar);
            int playerZ = PlayerAvatar_GetZCoord(playerAvatar);
            int range = MapObject_GetParam(object, 0);
            int x = MapObject_GetXCoord(object);
            int z = MapObject_GetZCoord(object);
            int minX = x - range;
            int maxX = x + range;
            int minZ = z - range;
            int maxZ = z + range;

            if (minZ <= playerZ && maxZ >= playerZ) {
                if (minX <= playerX && maxX >= playerX) {
                    return sub_02061200(x, z, playerX, playerZ);
                }
            }
        }
    }

    return -1;
}

static int sub_02061F5C(LocalMapObject *object, int id, int terminator) {
    const int *list = sub_02061E6C(id);
    int count = sub_02061E00(list, terminator);

    if (count == 1) {
        return -1;
    }

    {
        int direction = sub_02061E90(object);

        if (direction == -1) {
            return direction;
        }

        {
            int i = 0;

            do {
                if (list[i] == direction) {
                    return direction;
                }
                i++;
            } while (i < count);

            {
                int dirX = -1, dirZ = -1;
                int x = MapObject_GetXCoord(object);
                int z = MapObject_GetZCoord(object);
                FieldSystem *fieldSystem = MapObject_GetFieldSystem(object);
                PlayerAvatar *playerAvatar = FieldSystem_GetPlayerAvatar(fieldSystem);
                int playerX = PlayerAvatar_GetXCoord(playerAvatar);
                int playerZ = PlayerAvatar_GetZCoord(playerAvatar);

                if (x > playerX) {
                    dirX = 2;
                } else if (x < playerX) {
                    dirX = 3;
                }

                if (z > playerZ) {
                    dirZ = 0;
                } else if (z < playerZ) {
                    dirZ = 1;
                }

                i = 0;

                if (dirX == -1) {
                    do {
                        if (list[i] == dirZ) {
                            return dirZ;
                        }
                        i++;
                    } while (i < count);
                } else if (dirZ == -1) {
                    do {
                        if (list[i] == dirX) {
                            return dirX;
                        }
                        i++;
                    } while (i < count);
                } else {
                    do {
                        if (list[i] == dirX) {
                            return dirX;
                        }
                        if (list[i] == dirZ) {
                            return dirZ;
                        }
                        i++;
                    } while (i < count);
                }
            }
        }
    }

    return -1;
}

static const int _020FD7E0[2][4] = {
    { 0, 2, 1, 3 },
    { 0, 3, 1, 2 },
};

static int sub_02062050(LocalMapObject *object) {
    int type = MapObject_GetType(object);

    if (type == 7 || type == 8) {
        return 1;
    }

    return 0;
}

static void sub_02062064(LocalMapObject *object, UnkStruct_02062064 *data) {
    if (MapObject_GetType(object) == 7) {
        data->unk_01 = 0;
    } else {
        data->unk_01 = 1;
    }
}

static void sub_0206207C(LocalMapObject *object, UnkStruct_02062064 *data) {
    int i, direction = MapObject_GetFacingDirection(object);

    for (i = 0; i < 4 && direction != _020FD7E0[data->unk_01][i]; i++) {
    }

    GF_ASSERT(i < 4);

    data->unk_00 = direction;

    i = (i + 1) % 4;
    direction = _020FD7E0[data->unk_01][i];

    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK7)) {
        data->unk_02 = 1;
    } else {
        data->unk_02 = 0;
    }

    MapObject_SetFacingDirection(object, direction);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK7);
}

static void sub_020620F8(LocalMapObject *object, UnkStruct_02062064 *data) {
    if (data->unk_02 == 0) {
        MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK7);
    }
}

static const int _020FD7B8[] = { 0x10, 0x20, 0x30, 0x40, -1 };

static const int _020FD7CC[] = { 0, 1, 2, 3, -1 };

static const int _020FD4F4[] = { 0, 2, -1 };

static const int _020FD518[] = { 0, 3, -1 };

static const int _020FD500[] = { 1, 2, -1 };

static const int _020FD524[] = { 1, 3, -1 };

static const int _020FD6E0[] = { 0, 1, 2, -1 };

static const int _020FD6A0[] = { 0, 1, 3, -1 };

static const int _020FD6B0[] = { 0, 2, 3, -1 };

static const int _020FD6D0[] = { 1, 2, 3, -1 };

static const int _020FD554[] = { 0, 1, -1 };

static const int _020FD53C[] = { 2, 3, -1 };

static const int _020FD740[] = { 0, 1, 2, 3, -1 };

static const int _020FD530[] = { 0, 1, -1 };

static const int _020FD50C[] = { 2, 3, -1 };

static const int _020FD650[] = { 0, 3, 2, 1 };

static const int _020FD640[] = { 3, 2, 1, 0 };

static const int _020FD620[] = { 1, 0, 3, 2 };

static const int _020FD630[] = { 2, 1, 0, 3 };

static const int _020FD720[] = { 2, 3, 1, 0 };

static const int _020FD600[] = { 2, 3, 1, 0 };

static const int _020FD560[] = { 1, 0, 2, 3 };

static const int _020FD730[] = { 3, 1, 0, 2 };

static const int _020FD660[] = { 2, 0, 1, 3 };

static const int _020FD670[] = { 0, 1, 3, 2 };

static const int _020FD6C0[] = { 3, 2, 0, 1 };

static const int _020FD6F0[] = { 1, 3, 2, 0 };

static const int _020FD680[] = { 3, 0, 1, 2 };

static const int _020FD580[] = { 0, 1, 2, 3 };

static const int _020FD570[] = { 2, 3, 0, 1 };

static const int _020FD5B0[] = { 1, 2, 3, 0 };

static const int _020FD610[] = { 0, 2, 1, 3 };

static const int _020FD5C0[] = { 1, 3, 0, 2 };

static const int _020FD690[] = { 2, 1, 3, 0 };

static const int _020FD700[] = { 3, 0, 2, 1 };

static const int _020FD590[] = { 0, 3, 1, 2 };

static const int _020FD5E0[] = { 1, 2, 0, 3 };

static const int _020FD5F0[] = { 2, 0, 3, 1 };

static const int _020FD710[] = { 3, 1, 2, 0 };

static const int _020FD790[] = { 0, 1, 2, 3, -1 };

static const UnkStruct_020FD838 _020FD838[] = {
    { 0x00, _020FD7CC },
    { 0x01, _020FD4F4 },
    { 0x02, _020FD518 },
    { 0x03, _020FD500 },
    { 0x04, _020FD524 },
    { 0x05, _020FD6E0 },
    { 0x06, _020FD6A0 },
    { 0x07, _020FD6B0 },
    { 0x08, _020FD6D0 },
    { 0x09, _020FD554 },
    { 0x0a, _020FD53C },
    { 0x0b, _020FD740 },
    { 0x0c, _020FD530 },
    { 0x0d, _020FD50C },
    { 0x0e, _020FD650 },
    { 0x0f, _020FD640 },
    { 0x10, _020FD620 },
    { 0x11, _020FD630 },
    { 0x12, _020FD720 },
    { 0x13, _020FD600 },
    { 0x14, _020FD560 },
    { 0x15, _020FD730 },
    { 0x16, _020FD660 },
    { 0x17, _020FD670 },
    { 0x18, _020FD6C0 },
    { 0x19, _020FD6F0 },
    { 0x1a, _020FD680 },
    { 0x1b, _020FD580 },
    { 0x1c, _020FD570 },
    { 0x1d, _020FD5B0 },
    { 0x1e, _020FD610 },
    { 0x1f, _020FD5C0 },
    { 0x20, _020FD690 },
    { 0x21, _020FD700 },
    { 0x22, _020FD590 },
    { 0x23, _020FD5E0 },
    { 0x24, _020FD5F0 },
    { 0x25, _020FD710 },
    { 0x26, _020FD790 },
    { 0x27, NULL      },
};

static const int _020FD800[] = { 2, 6, 7, 8, 9, 10, 11, 12, 13, 0x2D, 0x2E, 0x12, 0x13, 0xFF };
