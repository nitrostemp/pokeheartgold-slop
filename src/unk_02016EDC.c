#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "pokepic.h"
#include "sys_task.h"
#include "sys_task_api.h"

// Pokemon sprite animation script interpreter (ported from
// pret/pokeplatinum src/pokemon_anim.c). Does not include the frozen
// unk_02016EDC.h, whose callers were matched against void * prototypes.

#define NUM_POKEMON_ANIMS         143
#define NUM_POKEMON_ANIM_COMMANDS 34

#define MAX_ANIM_SCRIPT_COMMANDS 256
#define MAX_ANIM_RADIANS         0x10000

#define ANIM_TRANSLATE_X 8
#define ANIM_TRANSLATE_Y 9
#define ANIM_OFFSET_X    10
#define ANIM_OFFSET_Y    11
#define ANIM_SCALE_X     12
#define ANIM_SCALE_Y     13
#define ANIM_ROTATION_Z  14

#define COMPARISON_RESULT_LESS_THAN    15
#define COMPARISON_RESULT_GREATER_THAN 16
#define COMPARISON_RESULT_EQUAL        17

#define ANIM_READ_TYPE_VALUE  18
#define ANIM_READ_TYPE_VAR    19
#define ANIM_READ_TYPE_VALUE2 20
#define ANIM_READ_TYPE_VAR2   21

#define ANIM_ATTRIBUTE_SET 22
#define ANIM_ATTRIBUTE_ADD 23

#define TRANSFORM_CALC_SET       24
#define TRANSFORM_CALC_ADD       25
#define TRANSFORM_CALC_INCREMENT 26

#define Y_NORMALIZATION_NEGATIVE_SCALE 27
#define Y_NORMALIZATION_OFF            28
#define Y_NORMALIZATION_ON             29

#define TRANSFORM_TYPE_OFFSET_X   35
#define TRANSFORM_TYPE_OFFSET_Y   36
#define TRANSFORM_TYPE_SCALE_X    37
#define TRANSFORM_TYPE_SCALE_Y    38
#define TRANSFORM_TYPE_ROTATION_Z 39

#define TRANSFORM_CURVE_SIN          30
#define TRANSFORM_CURVE_COS          31
#define TRANSFORM_CURVE_NEGATIVE_SIN 32
#define TRANSFORM_CURVE_NEGATIVE_COS 33

#define TRANSFORM_FUNC_CURVE          0
#define TRANSFORM_FUNC_CURVE_EVEN     1
#define TRANSFORM_FUNC_LINEAR         2
#define TRANSFORM_FUNC_LINEAR_EVEN    3
#define TRANSFORM_FUNC_LINEAR_BOUNDED 4

#define MAX_ANIM_TRANSFORMS     4
#define MAX_TRANSFORM_DATA_VARS 8
#define MAX_POKEMON_ANIM_VARS   8

typedef struct TransformData TransformData;
typedef struct PokemonAnim PokemonAnim;

typedef void (*TransformFunc)(TransformData *, PokemonAnim *);

typedef struct PokemonAnimTemplate {
    u16 animation;
    u16 startDelay;
    u8 flipSprite;
} PokemonAnimTemplate;

struct TransformData {
    BOOL active;
    int vars[MAX_TRANSFORM_DATA_VARS];
    int *transformMemberPtr;
    int *animMemberPtr;
    u8 calcType;
    u8 startDelay;
    int originalValue;
    int dummy_34;
    int dummy_38;
    int offsetX;
    int offsetY;
    int scaleX;
    int scaleY;
    int rotationZ;
    TransformFunc func;
};

struct PokemonAnim {
    Pokepic *sprite;
    SysTask *task;
    void *scriptData;
    u32 *scriptPtr;
    BOOL active;
    int animNum;
    int waitFrame;
    int endAnim;
    BOOL completed;
    int vars[MAX_POKEMON_ANIM_VARS];
    int commandCount;
    int loopMax;
    int loopCounter;
    u32 *loopStart;
    int startDelay;
    int originalX;
    int originalY;
    int translateX;
    int translateY;
    int offsetX;
    int offsetY;
    int scaleX;
    int scaleY;
    int rotationZ;
    TransformData transforms[MAX_ANIM_TRANSFORMS];
    u8 flipSprite;
    u8 waitForTransform;
    u8 yNormalization;
    u8 fadeActive;
};

typedef struct PokemonAnimManager {
    PokemonAnim *anims;
    enum HeapID heapID;
    u8 flipSprite;
    u8 animCount;
} PokemonAnimManager;

PokemonAnimManager *sub_02016EDC(enum HeapID heapID, int animCount, u8 flipSprite);
void sub_02016F2C(PokemonAnimManager *monAnimMan);
void sub_02016F40(PokemonAnimManager *monAnimMan, Pokepic *monSprite, const PokemonAnimTemplate *animTemplate, u8 index);
BOOL sub_02017068(PokemonAnimManager *monAnimMan, u8 index);
void sub_02017088(PokemonAnimManager *monAnimMan, u8 index);

typedef void (*PokemonAnimCmd)(PokemonAnim *);

typedef struct {
    TransformFunc func;
    int paramCount;
    int transformTypeIndex;
} TransformFuncParameters;

static int sub_020171F4(u32 *scriptPtr, u8 index, u8 one);
static int sub_02017208(u32 *scriptPtr, u8 one);
static int sub_02017214(u32 *scriptPtr);
static void sub_02017C78(PokemonAnim *monAnim, int funcType);
static void sub_020170C4(SysTask *task, void *monAnim);
static void sub_020170FC(PokemonAnim *monAnim);
static void sub_020174A4(PokemonAnim *monAnim);
static void sub_020174B4(PokemonAnim *monAnim);
static void sub_020174BC(PokemonAnim *monAnim);
static void sub_020175EC(PokemonAnim *monAnim);
static void sub_020176CC(PokemonAnim *monAnim);
static void sub_02017504(PokemonAnim *monAnim);
static void sub_0201752C(PokemonAnim *monAnim);
static void sub_02017550(PokemonAnim *monAnim);
static void sub_02017578(PokemonAnim *monAnim);
static void sub_0201759C(PokemonAnim *monAnim);
static void sub_020175C4(PokemonAnim *monAnim);
static void sub_020176F0(PokemonAnim *monAnim);
static void sub_02017714(PokemonAnim *monAnim);
static void sub_02017730(PokemonAnim *monAnim);
static void sub_0201775C(PokemonAnim *monAnim);
static void sub_02017788(PokemonAnim *monAnim);
static void sub_02017808(PokemonAnim *monAnim);
static void sub_0201783C(PokemonAnim *monAnim);
static void sub_02017874(PokemonAnim *monAnim);
static void sub_020178BC(PokemonAnim *monAnim);
static void sub_0201790C(PokemonAnim *monAnim);
static void sub_020179D4(PokemonAnim *monAnim);
static void sub_02017A1C(PokemonAnim *monAnim);
static void sub_02017A84(PokemonAnim *monAnim);
static void sub_02017AD8(PokemonAnim *monAnim);
static void sub_02017AEC(PokemonAnim *monAnim);
static void sub_02017B2C(PokemonAnim *monAnim);
static void sub_02017B48(PokemonAnim *monAnim);
static void sub_02017B54(PokemonAnim *monAnim);
static void sub_02017B8C(PokemonAnim *monAnim);
static void sub_02017B98(PokemonAnim *monAnim);
static void sub_02017BA4(PokemonAnim *monAnim);
static void sub_02017BB0(PokemonAnim *monAnim);
static void sub_02017BBC(PokemonAnim *monAnim);
static void sub_02017D20(TransformData *transform, PokemonAnim *monAnim);
static void sub_02017DD8(TransformData *transform, PokemonAnim *monAnim);
static void sub_02017E98(TransformData *transform, PokemonAnim *monAnim);
static void sub_02017ED4(TransformData *transform, PokemonAnim *monAnim);
static void sub_02017F10(TransformData *transform, PokemonAnim *monAnim);

static const TransformFuncParameters sTransformFuncToParams[] = {
    [TRANSFORM_FUNC_CURVE] = { sub_02017D20, 6, 1 },
    [TRANSFORM_FUNC_CURVE_EVEN] = { sub_02017DD8, 6, 1 },
    [TRANSFORM_FUNC_LINEAR] = { sub_02017E98, 4, 0 },
    [TRANSFORM_FUNC_LINEAR_EVEN] = { sub_02017ED4, 3, 0 },
    [TRANSFORM_FUNC_LINEAR_BOUNDED] = { sub_02017F10, 4, 0 },
};

static const PokemonAnimCmd sPokemonAnimCmds[NUM_POKEMON_ANIM_COMMANDS] = {
    sub_020174A4,
    sub_020174B4,
    sub_020174BC,
    sub_020175EC,
    sub_020176CC,
    sub_02017504,
    sub_0201752C,
    sub_02017550,
    sub_02017578,
    sub_0201759C,
    sub_020175C4,
    sub_020176F0,
    sub_02017714,
    sub_02017730,
    sub_0201775C,
    sub_02017788,
    sub_02017808,
    sub_0201783C,
    sub_02017874,
    sub_020178BC,
    sub_0201790C,
    sub_020179D4,
    sub_02017A1C,
    sub_02017A84,
    sub_02017B48,
    sub_02017B54,
    sub_02017B8C,
    sub_02017B98,
    sub_02017BA4,
    sub_02017BB0,
    sub_02017BBC,
    sub_02017AD8,
    sub_02017AEC,
    sub_02017B2C,
};

PokemonAnimManager *sub_02016EDC(enum HeapID heapID, int animCount, u8 flipSprite) {
    PokemonAnimManager *monAnimMan = Heap_Alloc(heapID, sizeof(PokemonAnimManager));
    monAnimMan->flipSprite = flipSprite;
    monAnimMan->animCount = animCount;
    monAnimMan->heapID = heapID;
    monAnimMan->anims = Heap_Alloc(heapID, sizeof(PokemonAnim) * animCount);

    MI_CpuClear8(monAnimMan->anims, sizeof(PokemonAnim) * animCount);

    for (int i = 0; i < animCount; i++) {
        monAnimMan->anims[i].completed = TRUE;
    }

    return monAnimMan;
}

void sub_02016F2C(PokemonAnimManager *monAnimMan) {
    Heap_Free(monAnimMan->anims);
    Heap_Free(monAnimMan);
}

void sub_02016F40(PokemonAnimManager *monAnimMan, Pokepic *monSprite, const PokemonAnimTemplate *animTemplate, u8 index) {
    int animNum = animTemplate->animation;
    int startDelay = animTemplate->startDelay;

    GF_ASSERT(index < monAnimMan->animCount);
    GF_ASSERT(monAnimMan->anims[index].active == FALSE);

    MI_CpuClear8(&monAnimMan->anims[index], sizeof(PokemonAnim));

    monAnimMan->anims[index].active = TRUE;
    monAnimMan->anims[index].sprite = monSprite;

    if (animNum >= NUM_POKEMON_ANIMS) {
        animNum = 0;
        startDelay = 0;
    }

    monAnimMan->anims[index].animNum = animNum;

    if (monAnimMan->flipSprite) {
        monAnimMan->anims[index].flipSprite = animTemplate->flipSprite;
    } else {
        monAnimMan->anims[index].flipSprite = FALSE;
    }

    monAnimMan->anims[index].scriptData = AllocAtEndAndReadWholeNarcMemberByIdPair(NARC_a_0_9_0, monAnimMan->anims[index].animNum, monAnimMan->heapID);
    monAnimMan->anims[index].scriptPtr = (u32 *)monAnimMan->anims[index].scriptData;
    monAnimMan->anims[index].endAnim = FALSE;
    monAnimMan->anims[index].completed = FALSE;
    monAnimMan->anims[index].waitForTransform = FALSE;
    monAnimMan->anims[index].yNormalization = Y_NORMALIZATION_OFF;
    monAnimMan->anims[index].fadeActive = FALSE;
    monAnimMan->anims[index].task = SysTask_CreateOnMainQueue(sub_020170C4, &monAnimMan->anims[index], 0);
    monAnimMan->anims[index].startDelay = startDelay;
    monAnimMan->anims[index].originalX = Pokepic_GetAttr(monSprite, POKEPIC_X);
    monAnimMan->anims[index].originalY = Pokepic_GetAttr(monSprite, POKEPIC_Y);
    monAnimMan->anims[index].translateX = 0;
    monAnimMan->anims[index].translateY = 0;
    monAnimMan->anims[index].offsetX = 0;
    monAnimMan->anims[index].offsetY = 0;
    monAnimMan->anims[index].scaleX = 0;
    monAnimMan->anims[index].scaleY = 0;
    monAnimMan->anims[index].rotationZ = 0;
}

BOOL sub_02017068(PokemonAnimManager *monAnimMan, u8 index) {
    GF_ASSERT(index < monAnimMan->animCount);
    return monAnimMan->anims[index].completed;
}

void sub_02017088(PokemonAnimManager *monAnimMan, u8 index) {
    if (monAnimMan->anims[index].task != NULL) {
        SysTask_Destroy(monAnimMan->anims[index].task);

        monAnimMan->anims[index].task = NULL;
        monAnimMan->anims[index].completed = TRUE;
        monAnimMan->anims[index].active = FALSE;

        Heap_Free(monAnimMan->anims[index].scriptData);
    }
}

static void sub_020170C4(SysTask *task, void *monAnim) {
    PokemonAnim *anim = (PokemonAnim *)(monAnim);

    if (anim->startDelay == 0) {
        sub_020170FC(anim);
    } else {
        anim->startDelay--;
    }

    if (anim->endAnim) {
        anim->completed = TRUE;
        anim->active = FALSE;

        SysTask_Destroy(task);
        anim->task = NULL;
        Heap_Free(anim->scriptData);
    }
}

static void sub_020170FC(PokemonAnim *monAnim) {
    monAnim->waitFrame = FALSE;
    monAnim->commandCount = 0;
    u8 inactiveTransforms = 0;

    for (u8 i = 0; i < MAX_ANIM_TRANSFORMS; i++) {
        TransformData *transform = &(monAnim->transforms[i]);

        if (transform->active) {
            if (transform->startDelay == 0) {
                transform->func(transform, monAnim);
            } else {
                transform->startDelay--;
            }
        } else {
            inactiveTransforms++;
        }
    }

    if (inactiveTransforms == MAX_ANIM_TRANSFORMS) {
        monAnim->waitForTransform = FALSE;
    }

    if (monAnim->waitForTransform) {
        sub_020179D4(monAnim);
        sub_02017A1C(monAnim);
        return;
    }

    if (monAnim->fadeActive) {
        if (!Pokepic_ResumePaletteFade(monAnim->sprite)) {
            monAnim->fadeActive = FALSE;
        } else {
            return;
        }
    }

    while (TRUE) {
        monAnim->commandCount++;

        GF_ASSERT(*(monAnim->scriptPtr) < NUM_POKEMON_ANIM_COMMANDS);

        PokemonAnimCmd currAnimCmd = sPokemonAnimCmds[*(monAnim->scriptPtr)];
        currAnimCmd(monAnim);

        if (monAnim->endAnim) {
            break;
        } else {
            monAnim->scriptPtr++;

            if (monAnim->waitFrame) {
                break;
            } else if (monAnim->waitForTransform) {
                sub_020179D4(monAnim);
                sub_02017A1C(monAnim);
                break;
            }
        }

        if (monAnim->commandCount >= MAX_ANIM_SCRIPT_COMMANDS) {
            GF_ASSERT(FALSE);

            monAnim->endAnim = TRUE;
            break;
        }
    }
}

// The final parameter here is only ever invoked with a value of 1.
static int sub_020171F4(u32 *scriptPtr, u8 index, u8 one) {
    int ret = scriptPtr[index];

    if (one != 1) {
        GF_ASSERT(FALSE);
    }

    return ret;
}

static int sub_02017208(u32 *scriptPtr, u8 one) {
    return sub_020171F4(scriptPtr, 0, one);
}

static int sub_02017214(u32 *scriptPtr) {
    return sub_02017208(scriptPtr, 1);
}

static TransformData *sub_02017220(PokemonAnim *monAnim, u8 funcType) {
    for (u8 i = 0; i < MAX_ANIM_TRANSFORMS; i++) {
        TransformData *retPtr = &(monAnim->transforms[i]);

        if (retPtr->active == FALSE) {
            MI_CpuClear8(retPtr, sizeof(TransformData));

            retPtr->active = TRUE;
            retPtr->func = sTransformFuncToParams[funcType].func;

            return retPtr;
        }
    }

    GF_ASSERT(FALSE);
    return NULL;
}

static void sub_0201726C(PokemonAnim *monAnim, int *outInt) {
    monAnim->scriptPtr++;
    *outInt = (int)sub_02017214(monAnim->scriptPtr);
}

static void sub_02017280(PokemonAnim *monAnim, u8 *outU8) {
    monAnim->scriptPtr++;
    *outU8 = (u8)sub_02017214(monAnim->scriptPtr);
}

static void sub_02017294(PokemonAnim *monAnim, u8 *outIndex) {
    monAnim->scriptPtr++;
    *outIndex = (u8)sub_02017214(monAnim->scriptPtr);
    GF_ASSERT(*outIndex < MAX_POKEMON_ANIM_VARS);
}

static void sub_020172B4(PokemonAnim *monAnim, u8 *outIndex1, u8 *outIndex2) {
    sub_02017294(monAnim, outIndex1);
    sub_02017294(monAnim, outIndex2);
}

static void sub_020172C8(PokemonAnim *monAnim, u8 *outDestIndex, int *outOperand1, int *outOperand2) {
    u8 index1, index2, readType;

    sub_02017294(monAnim, outDestIndex);
    sub_02017280(monAnim, &readType);

    if (readType == ANIM_READ_TYPE_VALUE) {
        sub_02017294(monAnim, &index1);
        *outOperand1 = monAnim->vars[index1];
        sub_0201726C(monAnim, outOperand2);
    } else if (readType == ANIM_READ_TYPE_VAR) {
        sub_020172B4(monAnim, &index1, &index2);
        *outOperand1 = monAnim->vars[index1];
        *outOperand2 = monAnim->vars[index2];
    } else {
        GF_ASSERT(FALSE);
    }
}

static void sub_0201733C(PokemonAnim *monAnim, u8 *outDestIndex, int *outOperand1, int *outOperand2) {
    u8 index1, index2, readType1, readType2;

    sub_02017294(monAnim, outDestIndex);
    sub_02017280(monAnim, &readType1);
    sub_02017280(monAnim, &readType2);

    if (readType1 == ANIM_READ_TYPE_VALUE) {
        sub_0201726C(monAnim, outOperand1);
    } else if (readType1 == ANIM_READ_TYPE_VAR) {
        sub_02017294(monAnim, &index1);
        *outOperand1 = monAnim->vars[index1];
    } else {
        GF_ASSERT(FALSE);
    }

    if (readType2 == ANIM_READ_TYPE_VALUE) {
        sub_0201726C(monAnim, outOperand2);
    } else if (readType2 == ANIM_READ_TYPE_VAR) {
        sub_02017294(monAnim, &index2);
        *outOperand2 = monAnim->vars[index2];
    } else {
        GF_ASSERT(FALSE);
    }
}

static void sub_020173C8(PokemonAnim *monAnim, u8 *outDestIndex, int *outRadians, int *outAmplitude) {
    u8 radiansIndex, amplitudeIndex, offsetIndex, readType;
    int radians, offset;

    sub_020172B4(monAnim, outDestIndex, &radiansIndex);
    radians = monAnim->vars[radiansIndex];
    sub_02017280(monAnim, &readType);

    // The ANIM_READ_TYPE_VALUE and ANIM_READ_TYPE_VALUE2 constants seem to be used interchangeably.
    // Same with the corresponding _VAR constants.
    if (readType == ANIM_READ_TYPE_VALUE2) {
        sub_0201726C(monAnim, outAmplitude);
    } else if (readType == ANIM_READ_TYPE_VAR2) {
        sub_02017294(monAnim, &amplitudeIndex);
        (*outAmplitude) = monAnim->vars[amplitudeIndex];
    } else {
        GF_ASSERT(FALSE);
    }

    sub_02017280(monAnim, &readType);

    if (readType == ANIM_READ_TYPE_VALUE2) {
        sub_0201726C(monAnim, &offset);
    } else if (readType == ANIM_READ_TYPE_VAR2) {
        sub_02017294(monAnim, &offsetIndex);
        offset = monAnim->vars[offsetIndex];
    } else {
        GF_ASSERT(FALSE);
    }

    *outRadians = radians + offset;
    *outRadians %= MAX_ANIM_RADIANS;
}

static u8 sub_02017470(int *value1, int *value2) {
    int result = *value1 - *value2;

    if (result < 0) {
        return COMPARISON_RESULT_LESS_THAN;
    } else if (result > 0) {
        return COMPARISON_RESULT_GREATER_THAN;
    } else {
        return COMPARISON_RESULT_EQUAL;
    }
}

static void sub_02017488(PokemonAnim *monAnim) {
    int y = (-monAnim->scaleY) / 8;
    Pokepic_AddAttr(monAnim->sprite, POKEPIC_Y, y);
}

static void sub_020174A4(PokemonAnim *monAnim) {
    sub_020174BC(monAnim);

    monAnim->waitFrame = TRUE;
    monAnim->endAnim = TRUE;
}

static void sub_020174B4(PokemonAnim *monAnim) {
    monAnim->waitFrame = TRUE;
}

static void sub_020174BC(PokemonAnim *monAnim) {
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_X, monAnim->originalX);
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_Y, monAnim->originalY);

    Pokepic_SetAttr(monAnim->sprite, POKEPIC_ZROT, 0);
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_XPIVOT, 0);

    Pokepic_SetAttr(monAnim->sprite, POKEPIC_AFFINEW, 0x100);
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_AFFINEH, 0x100);
}

static void sub_02017504(PokemonAnim *monAnim) {
    u8 destIndex, originIndex;

    sub_020172B4(monAnim, &destIndex, &originIndex);
    monAnim->vars[destIndex] = monAnim->vars[originIndex];
}

static void sub_0201752C(PokemonAnim *monAnim) {
    u8 index;
    int operand1, operand2;

    sub_020172C8(monAnim, &index, &operand1, &operand2);
    monAnim->vars[index] = operand1 + operand2;
}

static void sub_02017550(PokemonAnim *monAnim) {
    u8 index;
    int operand1, operand2;

    sub_020172C8(monAnim, &index, &operand1, &operand2);
    monAnim->vars[index] = operand1 * operand2;
}

static void sub_02017578(PokemonAnim *monAnim) {
    u8 index;
    int operand1, operand2;

    sub_0201733C(monAnim, &index, &operand1, &operand2);
    monAnim->vars[index] = operand1 - operand2;
}

static void sub_0201759C(PokemonAnim *monAnim) {
    u8 index;
    int operand1, operand2;

    sub_0201733C(monAnim, &index, &operand1, &operand2);
    monAnim->vars[index] = operand1 / operand2;
}

static void sub_020175C4(PokemonAnim *monAnim) {
    u8 index;
    int operand1, operand2;

    sub_0201733C(monAnim, &index, &operand1, &operand2);
    monAnim->vars[index] = operand1 % operand2;
}

static void sub_020175EC(PokemonAnim *monAnim) {
    u8 index1, index2, condition, comparisonResult, readType;
    int value1, value2;

    sub_02017280(monAnim, &readType);

    if (readType == ANIM_READ_TYPE_VALUE2) {
        sub_02017294(monAnim, &index1);
        value1 = monAnim->vars[index1];
        sub_0201726C(monAnim, &value2);
    } else if (readType == ANIM_READ_TYPE_VAR2) {
        sub_020172B4(monAnim, &index1, &index2);
        value1 = monAnim->vars[index1];
        value2 = monAnim->vars[index2];
    } else {
        GF_ASSERT(FALSE);
    }

    sub_02017280(monAnim, &condition);
    GF_ASSERT(condition <= COMPARISON_RESULT_EQUAL);

    comparisonResult = sub_02017470(&value1, &value2);

    int newValue;

    sub_02017280(monAnim, &readType);

    if (readType == ANIM_READ_TYPE_VALUE2) {
        sub_02017294(monAnim, &index1);
        sub_0201726C(monAnim, &newValue);
    } else if (readType == ANIM_READ_TYPE_VAR2) {
        sub_020172B4(monAnim, &index1, &index2);
        newValue = monAnim->vars[index2];
    } else {
        GF_ASSERT(FALSE);
    }

    if (condition == comparisonResult) {
        monAnim->vars[index1] = newValue;
    }
}

static void sub_020176CC(PokemonAnim *monAnim) {
    u8 index;
    sub_02017294(monAnim, &index);

    monAnim->scriptPtr++;
    monAnim->vars[index] = (int)sub_02017214(monAnim->scriptPtr);
}

static void sub_020176F0(PokemonAnim *monAnim) {
    GF_ASSERT(monAnim->loopStart == NULL);

    monAnim->scriptPtr++;
    monAnim->loopStart = monAnim->scriptPtr;
    monAnim->loopMax = (int)sub_02017214(monAnim->scriptPtr);
    monAnim->loopCounter = 0;
}

static void sub_02017714(PokemonAnim *monAnim) {
    monAnim->loopCounter++;

    if (monAnim->loopCounter >= monAnim->loopMax) {
        monAnim->loopStart = NULL;
        monAnim->loopCounter = 0;
        monAnim->loopMax = 0;
    } else {
        monAnim->scriptPtr = monAnim->loopStart;
    }
}

static void sub_02017730(PokemonAnim *monAnim) {
    u8 index;
    int attribute;

    sub_0201726C(monAnim, &attribute);
    sub_02017294(monAnim, &index);
    Pokepic_SetAttr(monAnim->sprite, attribute, monAnim->vars[index]);
}

static void sub_0201775C(PokemonAnim *monAnim) {
    u8 index;
    int attribute;

    sub_0201726C(monAnim, &attribute);
    sub_02017294(monAnim, &index);
    Pokepic_AddAttr(monAnim->sprite, attribute, monAnim->vars[index]);
}

static void sub_02017788(PokemonAnim *monAnim) {
    int attribute, value;

    sub_0201726C(monAnim, &attribute);

    u8 index, readType;

    sub_02017280(monAnim, &readType);

    if (readType == ANIM_READ_TYPE_VALUE2) {
        sub_0201726C(monAnim, &value);
    } else if (readType == ANIM_READ_TYPE_VAR2) {
        sub_02017294(monAnim, &index);
        value = monAnim->vars[index];
    } else {
        GF_ASSERT(FALSE);
    }

    u8 updateType;

    sub_02017280(monAnim, &updateType);

    if (updateType == ANIM_ATTRIBUTE_SET) {
        Pokepic_SetAttr(monAnim->sprite, attribute, value);
    } else if (updateType == ANIM_ATTRIBUTE_ADD) {
        Pokepic_AddAttr(monAnim->sprite, attribute, value);
    } else {
        GF_ASSERT(FALSE);
    }
}

static void sub_02017808(PokemonAnim *monAnim) {
    u8 index;
    int radians, amplitude;

    sub_020173C8(monAnim, &index, &radians, &amplitude);
    monAnim->vars[index] = FX_Whole(FX_SinIdx(radians) * amplitude);
}

static void sub_0201783C(PokemonAnim *monAnim) {
    u8 index;
    int radians, amplitude;

    sub_020173C8(monAnim, &index, &radians, &amplitude);
    monAnim->vars[index] = FX_Whole(FX_CosIdx(radians) * amplitude);
}

static void sub_02017874(PokemonAnim *monAnim) {
    u8 index, translationType;

    sub_02017294(monAnim, &index);
    sub_02017280(monAnim, &translationType);

    if (translationType == ANIM_TRANSLATE_X) {
        monAnim->translateX = monAnim->vars[index];
    } else if (translationType == ANIM_TRANSLATE_Y) {
        monAnim->translateY = monAnim->vars[index];
    } else {
        GF_ASSERT(FALSE);
    }
}

static void sub_020178BC(PokemonAnim *monAnim) {
    u8 index, translationType;

    sub_02017294(monAnim, &index);
    sub_02017280(monAnim, &translationType);

    if (translationType == ANIM_TRANSLATE_X) {
        monAnim->translateX += monAnim->vars[index];
    } else if (translationType == ANIM_TRANSLATE_Y) {
        monAnim->translateY += monAnim->vars[index];
    } else {
        GF_ASSERT(FALSE);
    }
}

static void sub_0201790C(PokemonAnim *monAnim) {
    int *attributePtr;
    u8 attribute;

    sub_02017280(monAnim, &attribute);

    if (attribute == ANIM_TRANSLATE_X) {
        attributePtr = &monAnim->translateX;
    } else if (attribute == ANIM_TRANSLATE_Y) {
        attributePtr = &monAnim->translateY;
    } else if (attribute == ANIM_OFFSET_X) {
        attributePtr = &monAnim->offsetX;
    } else if (attribute == ANIM_OFFSET_Y) {
        attributePtr = &monAnim->offsetY;
    } else if (attribute == ANIM_SCALE_X) {
        attributePtr = &monAnim->scaleX;
    } else if (attribute == ANIM_SCALE_Y) {
        attributePtr = &monAnim->scaleY;
    } else if (attribute == ANIM_ROTATION_Z) {
        attributePtr = &monAnim->rotationZ;
    } else {
        GF_ASSERT(FALSE);
    }

    u8 index, readType;
    int value;

    sub_02017280(monAnim, &readType);

    if (readType == ANIM_READ_TYPE_VALUE2) {
        sub_0201726C(monAnim, &value);
    } else if (readType == ANIM_READ_TYPE_VAR2) {
        sub_02017294(monAnim, &index);
        value = monAnim->vars[index];
    } else {
        GF_ASSERT(FALSE);
    }

    u8 updateType;

    sub_02017280(monAnim, &updateType);

    if (updateType == ANIM_ATTRIBUTE_SET) {
        *attributePtr = value;
    } else if (updateType == ANIM_ATTRIBUTE_ADD) {
        *attributePtr += value;
    } else {
        GF_ASSERT(FALSE);
    }
}

static void sub_020179D4(PokemonAnim *monAnim) {
    if (monAnim->flipSprite) {
        Pokepic_SetAttr(monAnim->sprite, POKEPIC_X, monAnim->originalX - (monAnim->translateX + monAnim->offsetX));
    } else {
        Pokepic_SetAttr(monAnim->sprite, POKEPIC_X, monAnim->originalX + monAnim->translateX + monAnim->offsetX);
    }

    Pokepic_SetAttr(monAnim->sprite, POKEPIC_Y, monAnim->originalY + monAnim->translateY + monAnim->offsetY);
}

static void sub_02017A1C(PokemonAnim *monAnim) {
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_AFFINEW, 0x100 + monAnim->scaleX);
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_AFFINEH, 0x100 + monAnim->scaleY);
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_ZROT, (u16)monAnim->rotationZ);

    if (monAnim->yNormalization == Y_NORMALIZATION_NEGATIVE_SCALE) {
        if (monAnim->scaleY < 0) {
            sub_02017488(monAnim);
        }
    } else if (monAnim->yNormalization == Y_NORMALIZATION_ON) {
        if (monAnim->scaleY != 0) {
            sub_02017488(monAnim);
        }
    } else if (monAnim->yNormalization == Y_NORMALIZATION_OFF) {
        return;
    } else {
        GF_ASSERT(FALSE);
    }
}

static void sub_02017A84(PokemonAnim *monAnim) {
    u8 index, attribute;

    sub_02017294(monAnim, &index);

    monAnim->scriptPtr++;
    attribute = sub_02017214(monAnim->scriptPtr);

    if (attribute == ANIM_TRANSLATE_X || attribute == ANIM_OFFSET_X) {
        monAnim->offsetX = monAnim->vars[index];
    } else if (attribute == ANIM_TRANSLATE_Y || attribute == ANIM_OFFSET_Y) {
        monAnim->offsetY = monAnim->vars[index];
    } else {
        GF_ASSERT(FALSE);
    }
}

static void sub_02017AD8(PokemonAnim *monAnim) {
    sub_0201726C(monAnim, &monAnim->startDelay);
    monAnim->waitFrame = TRUE;
}

static void sub_02017AEC(PokemonAnim *monAnim) {
    u8 initAlpha, targetAlpha, delay;
    int color;

    sub_02017280(monAnim, &initAlpha);
    sub_02017280(monAnim, &targetAlpha);
    sub_02017280(monAnim, &delay);
    sub_0201726C(monAnim, &color);
    Pokepic_StartPaletteFade(monAnim->sprite, initAlpha, targetAlpha, delay, color);
}

static void sub_02017B2C(PokemonAnim *monAnim) {
    if (Pokepic_ResumePaletteFade(monAnim->sprite)) {
        monAnim->fadeActive = TRUE;
        monAnim->waitFrame = TRUE;
    }
}

static void sub_02017B48(PokemonAnim *monAnim) {
    monAnim->waitForTransform = TRUE;
}

static void sub_02017B54(PokemonAnim *monAnim) {
    sub_02017280(monAnim, &monAnim->yNormalization);
    GF_ASSERT(
        monAnim->yNormalization == Y_NORMALIZATION_NEGATIVE_SCALE
        || monAnim->yNormalization == Y_NORMALIZATION_ON
        || (monAnim->yNormalization == Y_NORMALIZATION_OFF
            // it doesn't match without this...
            && TRUE));
}

static void sub_02017B8C(PokemonAnim *monAnim) {
    sub_02017C78(monAnim, TRANSFORM_FUNC_CURVE);
}

static void sub_02017B98(PokemonAnim *monAnim) {
    sub_02017C78(monAnim, TRANSFORM_FUNC_CURVE_EVEN);
}

static void sub_02017BA4(PokemonAnim *monAnim) {
    sub_02017C78(monAnim, TRANSFORM_FUNC_LINEAR);
}

static void sub_02017BB0(PokemonAnim *monAnim) {
    sub_02017C78(monAnim, TRANSFORM_FUNC_LINEAR_EVEN);
}

static void sub_02017BBC(PokemonAnim *monAnim) {
    sub_02017C78(monAnim, TRANSFORM_FUNC_LINEAR_BOUNDED);
}

static void sub_02017BC8(u8 calcType, int *originalValue, int *currentValue, int *nextValue) {
    if (calcType == TRANSFORM_CALC_SET) {
        *nextValue = *currentValue;
    } else if (calcType == TRANSFORM_CALC_ADD) {
        *nextValue = *originalValue + *currentValue;
    } else if (calcType == TRANSFORM_CALC_INCREMENT) {
        *nextValue += *currentValue;
    } else {
        GF_ASSERT(FALSE);
    }
}

static void sub_02017BF8(u8 transformType, TransformData *transform, PokemonAnim *monAnim) {
    switch (transformType) {
    case TRANSFORM_TYPE_OFFSET_X:
        transform->transformMemberPtr = &transform->offsetX;
        transform->animMemberPtr = &monAnim->offsetX;
        transform->originalValue = monAnim->offsetX;
        break;
    case TRANSFORM_TYPE_OFFSET_Y:
        transform->transformMemberPtr = &transform->offsetY;
        transform->animMemberPtr = &monAnim->offsetY;
        transform->originalValue = monAnim->offsetY;
        break;
    case TRANSFORM_TYPE_SCALE_X:
        transform->transformMemberPtr = &transform->scaleX;
        transform->animMemberPtr = &monAnim->scaleX;
        transform->originalValue = monAnim->scaleX;
        break;
    case TRANSFORM_TYPE_SCALE_Y:
        transform->transformMemberPtr = &transform->scaleY;
        transform->animMemberPtr = &monAnim->scaleY;
        transform->originalValue = monAnim->scaleY;
        break;
    case TRANSFORM_TYPE_ROTATION_Z:
        transform->transformMemberPtr = &transform->rotationZ;
        transform->animMemberPtr = &monAnim->rotationZ;
        transform->originalValue = monAnim->rotationZ;
        break;
    default:
        GF_ASSERT(FALSE);
    }
}

static void sub_02017C78(PokemonAnim *monAnim, int funcType) {
    TransformData *transform = sub_02017220(monAnim, funcType);

    sub_02017280(monAnim, &transform->calcType);
    sub_02017280(monAnim, &transform->startDelay);

    for (u8 i = 0; i < sTransformFuncToParams[funcType].paramCount; i++) {
        sub_0201726C(monAnim, &transform->vars[i]);
    }

    int index = sTransformFuncToParams[funcType].transformTypeIndex;
    sub_02017BF8(transform->vars[index], transform, monAnim);

    if (transform->startDelay == 0) {
        transform->func(transform, monAnim);
    } else {
        transform->startDelay--;
    }
}

static void sub_02017D20(TransformData *transform, PokemonAnim *monAnim) {
    int *vars = transform->vars;
    u16 radians = (vars[3] * (vars[6] + 1)) + vars[4];

    switch (vars[0]) {
    case TRANSFORM_CURVE_SIN:
        *transform->transformMemberPtr = FX_Whole(FX_SinIdx(radians) * vars[2]);
        break;
    case TRANSFORM_CURVE_COS:
        *transform->transformMemberPtr = FX_Whole(FX_CosIdx(radians) * vars[2]);
        break;
    case TRANSFORM_CURVE_NEGATIVE_SIN:
        *transform->transformMemberPtr = -FX_Whole(FX_SinIdx(radians) * vars[2]);
        break;
    case TRANSFORM_CURVE_NEGATIVE_COS:
        *transform->transformMemberPtr = -FX_Whole(FX_CosIdx(radians) * vars[2]);
        break;
    default:
        GF_ASSERT(FALSE);
    }

    sub_02017BC8(transform->calcType, &(transform->originalValue), transform->transformMemberPtr, transform->animMemberPtr);

    vars[6]++;

    if (vars[6] >= vars[5]) {
        transform->active = FALSE;
    }
}

static void sub_02017DD8(TransformData *transform, PokemonAnim *monAnim) {
    int *vars = transform->vars;
    u16 radians = ((vars[3] * (vars[6] + 1)) / vars[5]) + vars[4];

    switch (vars[0]) {
    case TRANSFORM_CURVE_SIN:
        *transform->transformMemberPtr = FX_Whole(FX_SinIdx(radians) * vars[2]);
        break;
    case TRANSFORM_CURVE_COS:
        *transform->transformMemberPtr = FX_Whole(FX_CosIdx(radians) * vars[2]);
        break;
    case TRANSFORM_CURVE_NEGATIVE_SIN:
        *transform->transformMemberPtr = -FX_Whole(FX_SinIdx(radians) * vars[2]);
        break;
    case TRANSFORM_CURVE_NEGATIVE_COS:
        *transform->transformMemberPtr = -FX_Whole(FX_CosIdx(radians) * vars[2]);
        break;
    default:
        GF_ASSERT(FALSE);
    }

    sub_02017BC8(transform->calcType, &(transform->originalValue), transform->transformMemberPtr, transform->animMemberPtr);

    vars[6]++;

    if (vars[6] >= vars[5]) {
        transform->active = FALSE;
    }
}

static void sub_02017E98(TransformData *transform, PokemonAnim *monAnim) {
    int *vars = transform->vars;
    int distance = vars[1] + (vars[2] * vars[4]);

    *transform->transformMemberPtr += distance;

    sub_02017BC8(transform->calcType, &(transform->originalValue), transform->transformMemberPtr, transform->animMemberPtr);

    vars[4]++;

    if (vars[4] >= vars[3]) {
        transform->active = FALSE;
    }
}

static void sub_02017ED4(TransformData *transform, PokemonAnim *monAnim) {
    int *vars = transform->vars;
    int distance = ((vars[3] + 1) * vars[1]) / vars[2];

    *transform->transformMemberPtr = distance;

    sub_02017BC8(transform->calcType, &(transform->originalValue), transform->transformMemberPtr, transform->animMemberPtr);

    vars[3]++;

    if (vars[3] >= vars[2]) {
        transform->active = FALSE;
    }
}

static void sub_02017F10(TransformData *transform, PokemonAnim *monAnim) {
    int *vars = transform->vars;
    int distance = vars[1] + (vars[2] * vars[4]);

    *transform->transformMemberPtr += distance;

    if (transform->calcType == TRANSFORM_CALC_SET || transform->calcType == TRANSFORM_CALC_INCREMENT) {
        if (distance < 0) {
            if (*transform->transformMemberPtr <= vars[3]) {
                *transform->transformMemberPtr = vars[3];
                transform->active = FALSE;
            }
        } else {
            if (*transform->transformMemberPtr >= vars[3]) {
                *transform->transformMemberPtr = vars[3];
                transform->active = FALSE;
            }
        }
    } else if (transform->calcType == TRANSFORM_CALC_ADD) {
        int v2 = transform->originalValue + *transform->transformMemberPtr;

        if (distance < 0) {
            if (v2 <= vars[3]) {
                *transform->transformMemberPtr += (vars[3] - v2);
                transform->active = FALSE;
            }
        } else {
            if (v2 >= vars[3]) {
                *transform->transformMemberPtr -= (v2 - vars[3]);
                transform->active = FALSE;
            }
        }
    } else {
        GF_ASSERT(FALSE);
    }

    sub_02017BC8(transform->calcType, &(transform->originalValue), transform->transformMemberPtr, transform->animMemberPtr);

    vars[4]++;
}
