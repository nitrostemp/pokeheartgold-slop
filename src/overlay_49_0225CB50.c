#include "global.h"

#include "camera.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "math_util.h"
#include "unk_02018000.h"

typedef struct UnkOv49Pos {
    u16 x;
    u16 y;
} UnkOv49Pos;

typedef struct UnkOv49State {
    u8 unk0[3];
    u8 unk3;
} UnkOv49State;

typedef struct UnkOv49Camera {
    Camera *camera; // 0x00
    void *follow;   // 0x04
    VecFx32 target; // 0x08
} UnkOv49Camera;    // size: 0x14

typedef struct UnkOv49ObjA {
    u8 active;                 // 0x00
    u8 unk1;                   // 0x01
    u8 type;                   // 0x02
    UnkStruct_020181B0 render; // 0x04
    int unk7C;                 // 0x7C
    int unk80;                 // 0x80
    int unk84;                 // 0x84
    int unk88;                 // 0x88
    u8 unk8C[0x10];            // 0x8C
    VecFx32 pos;               // 0x9C
    VecFx32 offset;            // 0xA8
} UnkOv49ObjA;                 // size: 0xB4

typedef struct UnkOv49ObjB {
    u16 active;                  // 0x00
    u16 modelIdx;                // 0x02
    UnkStruct_020181B0 render;   // 0x04
    UnkStruct_020180BC anims[3]; // 0x7C
    u8 animActive[3];            // 0xB8
    u8 unkBB;                    // 0xBB
    u8 animMode[3];              // 0xBC
    fx32 frames[3];              // 0xC0
    u8 randMax;                  // 0xCC
    u8 counters[3];              // 0xCD
    int unkD0[3];                // 0xD0
    fx32 unkDC;                  // 0xDC
    u8 unkE0;                    // 0xE0
    u8 unkE1;                    // 0xE1
    u8 unkE2;                    // 0xE2
} UnkOv49ObjB;                   // size: 0xE4

typedef struct UnkOv49MapRes {
    UnkStruct_02018030 models[2]; // 0x00
    UnkStruct_020180BC anims[5];  // 0x20
    int animLoaded[5];            // 0x84
} UnkOv49MapRes;                  // size: 0x98

typedef struct UnkOv49MapData {
    u8 unk0[0x180];
    int modelFiles[3]; // 0x180
    int animFiles[5];  // 0x18C
} UnkOv49MapData;

typedef struct UnkOv49Manager {
    u8 unk0[4];                     // 0x000
    UnkStruct_020181B0 render;      // 0x004
    u8 unk7C[0x88];                 // 0x07C
    int mapAnimActive;              // 0x104
    u8 unk108[0x14];                // 0x108
    UnkOv49ObjA *objsA;             // 0x11C
    UnkOv49ObjB *objsB;             // 0x120
    u8 numObjsA;                    // 0x124
    u8 numObjsB;                    // 0x125
    u8 mapA;                        // 0x126
    u8 mapB;                        // 0x127
    int loaded;                     // 0x128
    UnkOv49MapRes mapRes;           // 0x12C
    UnkStruct_02018030 modelsA[3];  // 0x1C4
    u8 unk1F4[0xA8];                // 0x1F4
    UnkStruct_02018030 modelsB[18]; // 0x29C
    void *animRes[18][3];           // 0x3BC
    NNSFndAllocator allocator;      // 0x494
} UnkOv49Manager;                   // size: 0x4A4

void NNS_G3dMdlSetMdlLightEnableFlagAll(NNSG3dResMdl *pMdl, u32 flag);
void ov45_0222D740(NNSG3dResFileHeader *resFile);
void ov49_02258800(UnkOv49Pos *pos, VecFx32 *out);
void ov49_02258814(VecFx32 *vec, UnkOv49Pos *out);
void ov49_02258830(UnkStruct_02018030 *res, NARC *narc, int fileId, enum HeapID heapID);
void ov49_02259154(void *follow, VecFx32 *out);
void ov49_0225D6AC(UnkOv49MapRes *mapRes, NNSFndAllocator *allocator);
void ov49_0225D6F0(UnkOv49Manager *manager, UnkOv49MapRes *mapRes);
void ov49_0225D76C(UnkOv49Manager *manager, UnkOv49MapRes *mapRes);
void ov49_0225D7B8(UnkOv49Manager *manager, UnkOv49MapRes *mapRes);
void ov49_0225D804(UnkOv49Manager *manager, UnkOv49MapRes *mapRes);
UnkOv49ObjA *ov49_0225D820(UnkOv49Manager *manager);
void ov49_0225D854(UnkStruct_02018030 *models, NARC *narc, NNSFndAllocator *allocator, UnkOv49MapData *data, enum HeapID heapID);
void ov49_0225D9D0(UnkStruct_02018030 *models, NNSFndAllocator *allocator);
void ov49_0225DA70(UnkOv49ObjA *obj, UnkStruct_02018030 *models);
void ov49_0225DAFC(UnkOv49ObjA *obj, UnkStruct_02018030 *models);
UnkOv49ObjB *ov49_0225DBF8(UnkOv49Manager *manager);
void ov49_0225DC2C(UnkStruct_02018030 *models, NARC *narc, NNSFndAllocator *allocator, UnkOv49MapData *data, enum HeapID heapID);
void ov49_0225DCBC(UnkStruct_02018030 *models, NNSFndAllocator *allocator);
void ov49_0225DD0C(UnkStruct_02018030 *models, UnkOv49ObjB *obj);
void ov49_0225DD68(UnkOv49Manager *manager, UnkOv49ObjB *obj);

void ov49_0225CB50(void *unused, u8 value, UnkOv49State *state);
void ov49_0225CB68(u16 *value);
NNSG3dResFileHeader *ov49_0225CB70(UnkOv49Manager *manager);
UnkOv49Camera *ov49_0225CB78(enum HeapID heapID);
void ov49_0225CBDC(UnkOv49Camera *camera);
void ov49_0225CBF4(UnkOv49Camera *camera);
void ov49_0225CC20(UnkOv49Camera *camera, fx32 x, fx32 y, fx32 z);
void ov49_0225CC28(UnkOv49Camera *camera, fx32 x, fx32 y, fx32 z);
void ov49_0225CC40(UnkOv49Camera *camera, void *follow);
void ov49_0225CC44(UnkOv49Camera *camera);
UnkOv49Manager *ov49_0225CC4C(u8 numObjsA, u8 numObjsB, enum HeapID heapID);
void ov49_0225CCC0(UnkOv49Manager *manager);
void ov49_0225CCF0(UnkOv49Manager *manager);
void ov49_0225CD58(UnkOv49Manager *manager);
void ov49_0225CDE8(UnkOv49Manager *manager);
void ov49_0225CDEC(UnkOv49Manager *manager, u8 mapA, u8 mapB, enum HeapID heapID, enum HeapID allocHeapID);
void ov49_0225CE88(UnkOv49Manager *manager);
void ov49_0225CED0(UnkOv49Manager *manager);
void ov49_0225CEFC(UnkOv49Manager *manager);
UnkOv49ObjA *ov49_0225CF28(UnkOv49Manager *manager, int type, int param, VecFx32 *pos);
void ov49_0225CF94(UnkOv49ObjA *obj);
void ov49_0225CFA8(UnkOv49ObjA *obj, VecFx32 *pos);
void ov49_0225CFEC(UnkOv49ObjA *obj, VecFx32 *offset);
void ov49_0225D030(UnkOv49ObjA *obj, VecFx32 *out);
void ov49_0225D040(UnkOv49ObjA *obj, int visible);
BOOL ov49_0225D04C(UnkOv49ObjA *obj);
BOOL ov49_0225D064(UnkOv49ObjA *obj);
void ov49_0225D07C(UnkOv49ObjA *obj, u16 rotation);
int ov49_0225D088(UnkOv49ObjA *obj);
int ov49_0225D090(UnkOv49ObjA *obj);
UnkOv49ObjB *ov49_0225D098(UnkOv49Manager *manager, int modelIdx, int x, int y);
void ov49_0225D160(UnkOv49Manager *manager, UnkOv49ObjB *obj);
u16 ov49_0225D1C0(UnkOv49ObjB *obj);
void ov49_0225D1C4(UnkOv49ObjB *obj, UnkOv49Pos pos);
UnkOv49Pos ov49_0225D1EC(UnkOv49ObjB *obj);
void ov49_0225D214(UnkOv49Manager *manager, UnkOv49ObjB *obj, int idx, int mode);
void ov49_0225D224(UnkOv49Manager *manager, UnkOv49ObjB *obj, int idx, int mode, int param);
void ov49_0225D328(UnkOv49Manager *manager, UnkOv49ObjB *obj, int idx);
void ov49_0225D394(UnkOv49Manager *manager, UnkOv49ObjB *obj);
u8 ov49_0225D3BC(UnkOv49Manager *manager, UnkOv49ObjB *obj, int idx);
void ov49_0225D3F8(UnkOv49Manager *manager, UnkOv49ObjB *obj, int idx, fx32 frame);
u8 ov49_0225D450(UnkOv49ObjB *obj, int idx);
fx32 ov49_0225D470(UnkOv49ObjB *obj, int idx);
void ov49_0225D494(UnkOv49ObjB *obj, int visible);
void ov49_0225D4A0(UnkOv49Manager *manager, UnkOv49ObjB *obj, BOOL enable);
void ov49_0225D4C8(UnkOv49ObjB *obj, fx32 value);
void ov49_0225D4D0(UnkOv49ObjB *obj, u8 a1, u8 a2);
void ov49_0225D4E8(UnkOv49ObjB *obj);
void ov49_0225D4F0(UnkOv49ObjB *obj, fx32 x, fx32 y, fx32 z);
static UnkOv49MapData *ov49_0225D4FC(int mapA, int mapB, enum HeapID heapID);
static void ov49_0225D520(UnkOv49MapData *data);
void ov49_0225D528(UnkStruct_02018030 *res, NARC *narc, int fileId, enum HeapID heapID);
void ov49_0225D574(UnkStruct_02018030 *res);
void ov49_0225D57C(fx32 *frame, UnkStruct_020180BC *anim, fx32 speed);
BOOL ov49_0225D5A0(fx32 *frame, UnkStruct_020180BC *anim, fx32 speed);
void ov49_0225D5C8(fx32 *frame, UnkStruct_020180BC *anim, fx32 speed);
BOOL ov49_0225D5E4(fx32 *frame, UnkStruct_020180BC *anim, fx32 speed);
static void ov49_0225D5FC(UnkOv49MapRes *mapRes, NARC *narc, UnkOv49MapData *data, enum HeapID heapID, NNSFndAllocator *allocator);

static const CameraAngle ov49_02269A6C = { 0xD602, 0, 0 };

void ov49_0225CB50(void *unused, u8 value, UnkOv49State *state) {
    state->unk3 = value;
    if (state->unk3 == 0) {
        state->unk3 = 2;
    } else if (state->unk3 == 3) {
        state->unk3 = 1;
    }
}

void ov49_0225CB68(u16 *value) {
    *value = 0;
}

NNSG3dResFileHeader *ov49_0225CB70(UnkOv49Manager *manager) {
    return manager->modelsB[7].resFile;
}

UnkOv49Camera *ov49_0225CB78(enum HeapID heapID) {
    UnkOv49Camera *camera = Heap_Alloc(heapID, sizeof(UnkOv49Camera));

    memset(camera, 0, sizeof(UnkOv49Camera));
    camera->camera = Camera_New(heapID);
    Camera_Init_FromTargetDistanceAndAngle(&camera->target, 0x29AEC1, &ov49_02269A6C, 0x5C1, 0, TRUE, camera->camera);
    Camera_SetStaticPtr(camera->camera);
    Camera_SetPerspectiveClippingPlane(FX32_CONST(150), FX32_CONST(900), camera->camera);
    return camera;
}

void ov49_0225CBDC(UnkOv49Camera *camera) {
    Camera_UnsetStaticPtr();
    Camera_Delete(camera->camera);
    Heap_Free(camera);
}

void ov49_0225CBF4(UnkOv49Camera *camera) {
    if (camera->follow != NULL) {
        ov49_02259154(camera->follow, &camera->target);
        camera->target.x += FX32_CONST(8);
        camera->target.z -= FX32_CONST(32);
    }
    Camera_PushLookAtToNNSGlb();
}

void ov49_0225CC20(UnkOv49Camera *camera, fx32 x, fx32 y, fx32 z) {
    camera->target.x = x;
    camera->target.y = y;
    camera->target.z = z;
}

void ov49_0225CC28(UnkOv49Camera *camera, fx32 x, fx32 y, fx32 z) {
    camera->target.x = x + FX32_CONST(8);
    camera->target.y = y;
    camera->target.z = z - FX32_CONST(32);
}

void ov49_0225CC40(UnkOv49Camera *camera, void *follow) {
    camera->follow = follow;
}

void ov49_0225CC44(UnkOv49Camera *camera) {
    camera->follow = NULL;
}

UnkOv49Manager *ov49_0225CC4C(u8 numObjsA, u8 numObjsB, enum HeapID heapID) {
    UnkOv49Manager *manager = Heap_Alloc(heapID, sizeof(UnkOv49Manager));
    u32 sizeA;
    u32 sizeB;

    memset(manager, 0, sizeof(UnkOv49Manager));
    sizeA = numObjsA * sizeof(UnkOv49ObjA);
    manager->objsA = Heap_Alloc(heapID, sizeA);
    sizeB = numObjsB * sizeof(UnkOv49ObjB);
    manager->objsB = Heap_Alloc(heapID, sizeB);
    memset(manager->objsA, 0, sizeA);
    memset(manager->objsB, 0, sizeB);
    manager->numObjsA = numObjsA;
    manager->numObjsB = numObjsB;
    return manager;
}

void ov49_0225CCC0(UnkOv49Manager *manager) {
    if (manager->loaded) {
        ov49_0225CE88(manager);
    }
    Heap_Free(manager->objsA);
    Heap_Free(manager->objsB);
    Heap_Free(manager);
}

void ov49_0225CCF0(UnkOv49Manager *manager) {
    int i;

    ov49_0225D7B8(manager, &manager->mapRes);
    for (i = 0; i < manager->numObjsA; i++) {
        ov49_0225DA70(&manager->objsA[i], manager->modelsA);
    }
    for (i = 0; i < manager->numObjsB; i++) {
        ov49_0225DD68(manager, &manager->objsB[i]);
    }
}

void ov49_0225CD58(UnkOv49Manager *manager) {
    int i;

    GF_ASSERT(manager != NULL);
    GF_ASSERT(manager->objsB != NULL);
    GF_ASSERT(manager->objsA != NULL);

    ov49_0225D804(manager, &manager->mapRes);
    for (i = 0; i < manager->numObjsB; i++) {
        ov49_0225DD0C(manager->modelsB, &manager->objsB[i]);
    }
    for (i = 0; i < manager->numObjsA; i++) {
        ov49_0225DAFC(&manager->objsA[i], manager->modelsA);
    }
}

void ov49_0225CDE8(UnkOv49Manager *manager) {
}

void ov49_0225CDEC(UnkOv49Manager *manager, u8 mapA, u8 mapB, enum HeapID heapID, enum HeapID allocHeapID) {
    UnkOv49MapData *data;
    NARC *narc;

    manager->mapB = mapB;
    manager->mapA = mapA;
    data = ov49_0225D4FC(mapA, mapB, heapID);
    narc = NARC_New(NARC_a_2_0_1, heapID);
    HeapExp_FndInitAllocator(&manager->allocator, allocHeapID, 4);
    ov49_0225D5FC(&manager->mapRes, narc, data, allocHeapID, &manager->allocator);
    ov49_0225DC2C(manager->modelsB, narc, &manager->allocator, data, allocHeapID);
    ov49_0225D854(manager->modelsA, narc, &manager->allocator, data, allocHeapID);
    NARC_Delete(narc);
    ov49_0225D520(data);
    ov49_0225D6F0(manager, &manager->mapRes);
    manager->loaded = TRUE;
}

void ov49_0225CE88(UnkOv49Manager *manager) {
    ov49_0225D76C(manager, &manager->mapRes);
    ov49_0225D6AC(&manager->mapRes, &manager->allocator);
    ov49_0225DCBC(manager->modelsB, &manager->allocator);
    ov49_0225D9D0(manager->modelsA, &manager->allocator);
    manager->loaded = FALSE;
}

void ov49_0225CED0(UnkOv49Manager *manager) {
    if (manager->mapRes.animLoaded[4] == TRUE && manager->mapAnimActive == FALSE) {
        manager->mapAnimActive = TRUE;
        sub_020181D4(&manager->render, &manager->mapRes.anims[4]);
    }
}

void ov49_0225CEFC(UnkOv49Manager *manager) {
    if (manager->mapRes.animLoaded[4] == TRUE && manager->mapAnimActive == TRUE) {
        sub_020181E0(&manager->render, &manager->mapRes.anims[4]);
        manager->mapAnimActive = FALSE;
    }
}

UnkOv49ObjA *ov49_0225CF28(UnkOv49Manager *manager, int type, int param, VecFx32 *pos) {
    UnkOv49ObjA *obj;

    GF_ASSERT(type <= 2);
    GF_ASSERT(param <= 3);

    obj = ov49_0225D820(manager);
    sub_020181B0(&obj->render, &manager->modelsA[type]);
    sub_020182A0(&obj->render, TRUE);
    ov49_0225CFA8(obj, pos);
    VecFx32 offset = { 0, 0, 0 };
    ov49_0225CFEC(obj, &offset);
    obj->unk1 = param;
    obj->type = type;
    obj->active = TRUE;
    obj->unk7C = 1;
    obj->unk84 = 1;
    return obj;
}

void ov49_0225CF94(UnkOv49ObjA *obj) {
    sub_020182A0(&obj->render, FALSE);
    obj->active = FALSE;
}

void ov49_0225CFA8(UnkOv49ObjA *obj, VecFx32 *pos) {
    obj->pos = *pos;
    sub_020182A8(&obj->render, obj->pos.x + obj->offset.x, obj->pos.y + obj->offset.y, obj->pos.z + obj->offset.z);
}

void ov49_0225CFEC(UnkOv49ObjA *obj, VecFx32 *offset) {
    obj->offset = *offset;
    sub_020182A8(&obj->render, obj->pos.x + obj->offset.x, obj->pos.y + obj->offset.y, obj->pos.z + obj->offset.z);
}

void ov49_0225D030(UnkOv49ObjA *obj, VecFx32 *out) {
    sub_020182B0(&obj->render, &out->x, &out->y, &out->z);
}

void ov49_0225D040(UnkOv49ObjA *obj, int visible) {
    sub_020182A0(&obj->render, visible);
}

BOOL ov49_0225D04C(UnkOv49ObjA *obj) {
    if (obj->unk80 == 0) {
        obj->unk80 = 1;
        return TRUE;
    }
    return FALSE;
}

BOOL ov49_0225D064(UnkOv49ObjA *obj) {
    if (obj->unk88 == 0) {
        obj->unk88 = 1;
        return TRUE;
    }
    return FALSE;
}

void ov49_0225D07C(UnkOv49ObjA *obj, u16 rotation) {
    sub_020182E0(&obj->render, rotation, 0);
}

int ov49_0225D088(UnkOv49ObjA *obj) {
    return obj->unk80;
}

int ov49_0225D090(UnkOv49ObjA *obj) {
    return obj->unk88;
}

UnkOv49ObjB *ov49_0225D098(UnkOv49Manager *manager, int modelIdx, int x, int y) {
    int i;
    UnkOv49ObjB *obj = ov49_0225DBF8(manager);

    sub_020181B0(&obj->render, &manager->modelsB[modelIdx]);
    for (i = 0; i < 3; i++) {
        if (manager->animRes[modelIdx][i] != NULL) {
            sub_020180E8(&obj->anims[i], &manager->modelsB[modelIdx], manager->animRes[modelIdx][i], &manager->allocator);
        }
    }
    sub_020182A0(&obj->render, TRUE);

    {
        UnkOv49Pos pos;
        pos.x = x << 4;
        pos.y = y << 4;
        ov49_0225D1C4(obj, pos);
    }

    obj->active = TRUE;
    obj->modelIdx = modelIdx;
    obj->randMax = 20;
    obj->unkDC = FX32_ONE;
    obj->unkE0 = 0;
    obj->unkE1 = 31;
    obj->unkE2 = 31;
    return obj;
}

void ov49_0225D160(UnkOv49Manager *manager, UnkOv49ObjB *obj) {
    int i;

    sub_020182A0(&obj->render, FALSE);
    for (i = 0; i < 3; i++) {
        if (manager->animRes[obj->modelIdx][i] != NULL) {
            sub_020180E8(&obj->anims[i], &manager->modelsB[obj->modelIdx], manager->animRes[obj->modelIdx][i], &manager->allocator);
        }
    }
    obj->active = FALSE;
}

u16 ov49_0225D1C0(UnkOv49ObjB *obj) {
    return obj->modelIdx;
}

void ov49_0225D1C4(UnkOv49ObjB *obj, UnkOv49Pos pos) {
    VecFx32 vec;

    ov49_02258800(&pos, &vec);
    sub_020182A8(&obj->render, vec.x, vec.y, vec.z);
}

UnkOv49Pos ov49_0225D1EC(UnkOv49ObjB *obj) {
    UnkOv49Pos pos;
    VecFx32 vec;

    sub_020182B0(&obj->render, &vec.x, &vec.y, &vec.z);
    ov49_02258814(&vec, &pos);
    return pos;
}

void ov49_0225D214(UnkOv49Manager *manager, UnkOv49ObjB *obj, int idx, int mode) {
    ov49_0225D224(manager, obj, idx, mode, 0);
}

void ov49_0225D224(UnkOv49Manager *manager, UnkOv49ObjB *obj, int idx, int mode, int param) {
    GF_ASSERT(idx < 3);
    GF_ASSERT(mode < 7);
    GF_ASSERT(obj->modelIdx < 18);

    if (manager->animRes[obj->modelIdx][idx] != NULL) {
        if (obj->animActive[idx] == FALSE) {
            sub_020181D4(&obj->render, &obj->anims[idx]);
        }
        obj->animActive[idx] = TRUE;
        obj->animMode[idx] = mode;
        obj->unkD0[idx] = param;
        obj->counters[idx] = 0;

        switch (mode) {
        case 0:
        case 1:
        case 2:
            obj->frames[idx] = 0;
            break;
        case 3:
        case 4:
            obj->frames[idx] = sub_020181A4(&obj->anims[idx]);
            break;
        case 5:
            obj->frames[idx] = 0;
            obj->counters[idx] = MTRandom() % obj->randMax;
            break;
        case 6:
            obj->frames[idx] = 0;
            obj->counters[idx] = MTRandom() % obj->randMax;
            break;
        }

        sub_02018198(&obj->anims[idx], obj->frames[idx]);
    }
}

void ov49_0225D328(UnkOv49Manager *manager, UnkOv49ObjB *obj, int idx) {
    GF_ASSERT(idx < 3);
    GF_ASSERT(obj->modelIdx < 18);

    if (manager->animRes[obj->modelIdx][idx] != NULL && obj->animActive[idx] == TRUE) {
        sub_020181E0(&obj->render, &obj->anims[idx]);
        obj->animActive[idx] = FALSE;
        obj->frames[idx] = 0;
        obj->animMode[idx] = 0;
        obj->counters[idx] = 0;
        obj->unkD0[idx] = 0;
    }
}

void ov49_0225D394(UnkOv49Manager *manager, UnkOv49ObjB *obj) {
    int i;

    for (i = 0; i < 3; i++) {
        if (ov49_0225D450(obj, i) == TRUE) {
            ov49_0225D328(manager, obj, i);
        }
    }
}

u8 ov49_0225D3BC(UnkOv49Manager *manager, UnkOv49ObjB *obj, int idx) {
    GF_ASSERT(idx < 3);
    GF_ASSERT(obj->modelIdx < 18);

    if (manager->animRes[obj->modelIdx][idx] != NULL) {
        return obj->animActive[idx];
    }
    return FALSE;
}

void ov49_0225D3F8(UnkOv49Manager *manager, UnkOv49ObjB *obj, int idx, fx32 frame) {
    GF_ASSERT(idx < 3);
    GF_ASSERT(obj->modelIdx < 18);
    GF_ASSERT(manager->animRes[obj->modelIdx][idx] != NULL);

    if (obj->animMode[idx] == 2) {
        obj->frames[idx] = frame;
        sub_02018198(&obj->anims[idx], obj->frames[idx]);
    }
}

u8 ov49_0225D450(UnkOv49ObjB *obj, int idx) {
    GF_ASSERT(idx < 3);
    GF_ASSERT(obj->modelIdx < 18);
    return obj->animActive[idx];
}

fx32 ov49_0225D470(UnkOv49ObjB *obj, int idx) {
    GF_ASSERT(idx < 3);
    GF_ASSERT(obj->modelIdx < 18);
    return obj->frames[idx];
}

void ov49_0225D494(UnkOv49ObjB *obj, int visible) {
    sub_020182A0(&obj->render, visible);
}

void ov49_0225D4A0(UnkOv49Manager *manager, UnkOv49ObjB *obj, BOOL enable) {
    GF_ASSERT(obj->modelIdx < 18);
    NNS_G3dMdlSetMdlLightEnableFlagAll(manager->modelsB[obj->modelIdx].model, enable);
}

void ov49_0225D4C8(UnkOv49ObjB *obj, fx32 value) {
    obj->unkDC = value;
}

void ov49_0225D4D0(UnkOv49ObjB *obj, u8 a1, u8 a2) {
    obj->unkE0 = 1;
    obj->unkE1 = a1;
    obj->unkE2 = a2;
}

void ov49_0225D4E8(UnkOv49ObjB *obj) {
    obj->unkE0 = 0;
}

void ov49_0225D4F0(UnkOv49ObjB *obj, fx32 x, fx32 y, fx32 z) {
    sub_020182C4(&obj->render, x, y, z);
}

static UnkOv49MapData *ov49_0225D4FC(int mapA, int mapB, enum HeapID heapID) {
    u32 idx = mapA + mapB * 5;

    GF_ASSERT(idx < 25);
    return GfGfxLoader_LoadFromNarc(NARC_a_2_0_0, idx + 1, FALSE, heapID, TRUE);
}

static void ov49_0225D520(UnkOv49MapData *data) {
    Heap_Free(data);
}

void ov49_0225D528(UnkStruct_02018030 *res, NARC *narc, int fileId, enum HeapID heapID) {
    ov49_02258830(res, narc, fileId, heapID);
    res->mdlSet = NNS_G3dGetMdlSet(res->resFile);
    res->model = NNS_G3dGetMdlByIdx(res->mdlSet, 0);
    res->tex = NNS_G3dGetTex(res->resFile);
    GF3dRender_BindModelSet(res->resFile, res->tex);
}

void ov49_0225D574(UnkStruct_02018030 *res) {
    sub_02018068(res);
}

void ov49_0225D57C(fx32 *frame, UnkStruct_020180BC *anim, fx32 speed) {
    fx32 end = sub_020181A4(anim);

    if (*frame + speed < end) {
        *frame = *frame + speed;
    } else {
        *frame = (*frame + speed) % end;
    }
}

BOOL ov49_0225D5A0(fx32 *frame, UnkStruct_020180BC *anim, fx32 speed) {
    fx32 end = sub_020181A4(anim);

    if (*frame + speed < end) {
        *frame = *frame + speed;
        return FALSE;
    }
    *frame = end - FX32_HALF;
    return TRUE;
}

void ov49_0225D5C8(fx32 *frame, UnkStruct_020180BC *anim, fx32 speed) {
    fx32 end = sub_020181A4(anim);

    if (*frame - speed >= 0) {
        *frame = *frame - speed;
    } else {
        *frame = end + (*frame - speed);
    }
}

BOOL ov49_0225D5E4(fx32 *frame, UnkStruct_020180BC *anim, fx32 speed) {
    if (*frame - speed > 0) {
        *frame = *frame - speed;
        return FALSE;
    }
    *frame = 0;
    return TRUE;
}

static void ov49_0225D5FC(UnkOv49MapRes *mapRes, NARC *narc, UnkOv49MapData *data, enum HeapID heapID, NNSFndAllocator *allocator) {
    int i;

    for (i = 0; i < 2; i++) {
        ov49_0225D528(&mapRes->models[i], narc, data->modelFiles[i], heapID);
        ov45_0222D740(mapRes->models[i].resFile);
    }

    for (i = 0; i < 5; i++) {
        if (data->modelFiles[0] == data->animFiles[i]) {
            mapRes->animLoaded[i] = FALSE;
        } else {
            mapRes->animLoaded[i] = TRUE;
            if (i != 3) {
                sub_020180BC(&mapRes->anims[i], &mapRes->models[0], narc, data->animFiles[i], heapID, allocator);
            } else {
                sub_020180BC(&mapRes->anims[i], &mapRes->models[1], narc, data->animFiles[i], heapID, allocator);
            }
        }
    }
}
