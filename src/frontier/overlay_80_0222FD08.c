#include "global.h"

#include "constants/game_stats.h"

#include "game_stats.h"
#include "heap.h"
#include "math_util.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "save.h"
#include "save_vars_flags.h"
#include "scrcmd_9.h"
#include "sys_vars.h"
#include "unk_02030A98.h"
#include "unk_0205BFF0.h"

// Battle Factory work state: allocation, rental/opponent generation,
// trading a rental for an opponent's Pokemon, and writing the run back to
// the save.

typedef struct BattleFactoryWork {
    int heapId;   // 0x000
    u8 type;      // 0x004
    u8 level;     // 0x005
    u8 battleNum; // 0x006
    u8 unk007[0x1];
    u16 winStreak; // 0x008
    u8 unk0A;      // 0x00A
    u8 unk00B[0x1];
    u16 unk0C; // 0x00C
    u16 unk0E; // 0x00E
    u32 unk10; // 0x010
    u8 unk014[0x4];
    u16 trainerIds[14]; // 0x018
    u8 unk034[0x220];
    u16 rentalIds[6]; // 0x254
    u8 rentalIvs[6];  // 0x260
    u8 unk266[0x2];
    u32 rentalPids[6];         // 0x268
    FrontierMon rentalMons[6]; // 0x280
    u8 unk3D0[0x2];
    u16 enemyIds[4]; // 0x3D2
    u8 enemyIvs[4];  // 0x3DA
    u8 unk3DE[0x2];
    u32 enemyPids[4];         // 0x3E0
    FrontierMon enemyMons[4]; // 0x3F0
    u8 unk4D0[0x4];
    Party *playerParty; // 0x4D4
    Party *enemyParty;  // 0x4D8
    u16 swapSlots[6];   // 0x4DC
    u16 monIds[6];      // 0x4E8
    void *frontierData; // 0x4F4
    SaveData *saveData; // 0x4F8
    u8 unk4FC[0x8];
    u16 sendBuf[30]; // 0x504
    u8 unk540[0x3C];
    u8 partnerValue0;  // 0x57C
    u8 partnerValue1;  // 0x57D
    u16 partner57E;    // 0x57E
    u16 partner580;    // 0x580
    u16 partner582;    // 0x582
    u16 partnerIds[6]; // 0x584
    u8 partnerIvs[6];  // 0x590
    u8 unk596[0x2];
    u32 partnerPids[6];         // 0x598
    FrontierMon partnerMons[6]; // 0x5B0
    u8 unk700[0x2];
    u8 recvCount; // 0x702
    u8 unk703[0x5];
} BattleFactoryWork;

typedef struct BattleFactoryOtherWork {
    u8 unk00[0x10];
    u16 unk10[6]; // 0x10
} BattleFactoryOtherWork;

typedef struct BattleFactoryTrainer {
    u8 unk00[0x4];
    u16 unk04; // 0x04
    u8 unk06[0x2A];
} BattleFactoryTrainer; // size: 0x30

void *sub_0203094C(SaveData *saveData);
void sub_02030940(void *frontierData);
void sub_02030964(void *frontierData, int a1);
void sub_02030978(void *frontierData, int field, u8 index, void *value);
u32 sub_02030A24(void *frontierData, int field, u8 index, void *a3);
u32 sub_02030AD4(int a0, int a1, u8 a2, void *a3);
int sub_0205C01C(u8 a0, u8 a1);
int sub_0205C074(u8 a0, u8 a1);
void *ov80_02229F04(BattleFactoryTrainer *dst, u16 trainerId, enum HeapID heapId, int narcId);
void ov80_0222A140(FrontierMon *src, Pokemon *mon, int level);
u16 ov80_0222A30C(u8 a0);
void ov80_0222A3BC(SaveData *saveData, Party *party, Pokemon *mon);
void ov80_0222A52C(FrontierMon *mons, u16 *ids, u8 *ivs, u32 *pids, void *a4, int count, enum HeapID heapId, int a7);
void ov80_0222A840(SaveData *saveData);
BOOL ov80_0222B108(BattleFactoryWork *work);
BOOL ov80_0222B174(BattleFactoryWork *work);
BOOL ov80_0222B1DC(BattleFactoryWork *work);
BOOL ov80_0222B2C4(BattleFactoryWork *work);
BOOL ov80_0222B3B0(BattleFactoryWork *work, u8 value);
BOOL ov80_0222B3FC(BattleFactoryWork *work, u8 value);
BOOL ov80_0222B448(BattleFactoryWork *work);
void ov80_02236BE4(u8 type, int a1, u16 *trainerIds, int count);
void *ov80_02236C2C(u16 trainerId, u8 level);
int ov80_02236C9C(u16 *species, u16 *items, int count, int a3, u16 *ids, enum HeapID heapId, void *a6, int a7, u8 *ivs);
int ov80_02236DD4(u8 type);
int ov80_02236DF8(u8 type, int a1);
void ov80_02236E24(int a0, u8 level, u16 *ids, FrontierMon *mons, u8 *ivs, u32 *pids, u16 winStreak, u16 *species, u16 *items);
void ov80_02236E90(int a0, u16 trainerId, u8 level, FrontierMon *candidates, u16 *ids, FrontierMon *mons, u8 *ivs, u32 *pids, int count);
int ov80_02237120(BattleFactoryWork *work);
int ov80_02237254(u8 type);
int ov80_022372B4(BattleFactoryWork *work);

extern const u8 ov80_0223BDD4[];
extern const u8 ov80_0223BDE0[];

BattleFactoryWork *ov80_0222FD08(SaveData *saveData, BOOL resume, u8 type, u8 level);
void ov80_0222FEEC(BattleFactoryWork *work, BOOL resume);
static void ov80_0222FF00(BattleFactoryWork *work);
static void ov80_022300D4(BattleFactoryWork *work, int index, BOOL partner);
static void ov80_02230270(BattleFactoryWork *work);
void ov80_02230424(BattleFactoryWork *work);
void ov80_02230460(BattleFactoryWork *work, BattleFactoryOtherWork *other);
static u16 ov80_02230484(BattleFactoryOtherWork *other, u8 index);
void ov80_0223049C(BattleFactoryWork *work, int mode);
u8 ov80_02230784(BattleFactoryWork *work);
u8 ov80_02230790(BattleFactoryWork *work);
u16 ov80_02230794(BattleFactoryWork *work, int a1);
void ov80_022307C8(BattleFactoryWork *work);
void ov80_022307D4(BattleFactoryWork *work);
void ov80_022307F0(BattleFactoryWork *work);
void ov80_022308C4(BattleFactoryWork *work);
void ov80_022309F8(BattleFactoryWork *work);
void ov80_02230A60(BattleFactoryWork *work);
void ov80_02230AE4(BattleFactoryWork *work);
BOOL ov80_02230AF8(BattleFactoryWork *work, u16 cmd, u16 arg);
u8 ov80_02230B4C(BattleFactoryWork *work);

static BattleFactoryWork *sBattleFactoryWork;

BattleFactoryWork *ov80_0222FD08(SaveData *saveData, BOOL resume, u8 type, u8 level) {
    BattleFactoryWork *work;
    BattleFactoryWork *work2;
    void *frontierData;
    int records;
    u8 value;
    u16 stat;

    sBattleFactoryWork = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleFactoryWork));
    MI_CpuFill8(sBattleFactoryWork, 0, sizeof(BattleFactoryWork));
    work = sBattleFactoryWork;
    work->frontierData = sub_0203094C(saveData);
    work->saveData = saveData;
    work->heapId = HEAP_ID_FIELD2;
    work = sBattleFactoryWork;
    work->playerParty = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
    work->enemyParty = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
    frontierData = work->frontierData;
    records = sub_02030AE8(saveData);
    if (resume == FALSE) {
        work = sBattleFactoryWork;
        work->type = type;
        work->level = level;
        work->battleNum = 0;
        sub_02030940(frontierData);
        work = sBattleFactoryWork;
        if (work->type == 3) {
            value = Save_VarsFlags_GetVar4052(Save_VarsFlags_Get(work->saveData));
        } else {
            value = sub_02030AD4(records, 10, work->type + work->level * 4, NULL);
        }
        if (value == TRUE) {
            work = sBattleFactoryWork;
            work->unk0C = FrontierSave_GetStat(Save_Frontier_GetStatic(work->saveData), sub_0205BFF0(work->level, work->type), sub_0205C268(sub_0205BFF0(work->level, work->type)));
            work2 = sBattleFactoryWork;
            stat = FrontierSave_GetStat(Save_Frontier_GetStatic(work2->saveData), sub_0205C048(work2->level, work2->type), sub_0205C268(sub_0205C048(work->level, work->type)));
        } else {
            work2 = sBattleFactoryWork;
            work2->unk0C = 0;
            stat = 0;
        }
        work2->winStreak = stat;
        work2->unk10 = 0;
    } else {
        type = sub_02030A24(frontierData, 1, 0, NULL);
        work = sBattleFactoryWork;
        work->type = type;
        work->level = sub_02030A24(frontierData, 0, 0, NULL);
        work->battleNum = sub_02030A24(frontierData, 2, 0, NULL);
        work2 = sBattleFactoryWork;
        work2->unk0C = FrontierSave_GetStat(Save_Frontier_GetStatic(work2->saveData), sub_0205BFF0(work2->level, work2->type), sub_0205C268(sub_0205BFF0(work->level, work->type)));
        work = sBattleFactoryWork;
        work->winStreak = FrontierSave_GetStat(Save_Frontier_GetStatic(work->saveData), sub_0205C048(work->level, work->type), sub_0205C268(sub_0205C048(work2->level, work2->type)));
    }
    work = sBattleFactoryWork;
    work->unk0E = work->unk0C / 7;
    if (ov80_02237254(work->type) == TRUE) {
        ov80_0222A840(sBattleFactoryWork->saveData);
    }
    return sBattleFactoryWork;
}

void ov80_0222FEEC(BattleFactoryWork *work, BOOL resume) {
    if (resume == FALSE) {
        ov80_0222FF00(work);
    } else {
        ov80_02230270(work);
    }
}

static void ov80_0222FF00(BattleFactoryWork *work) {
    FrontierMon candidates[12];
    u16 species[6];
    u16 items[6];
    int count;
    int i;
    Pokemon *mon;

    ov80_02236BE4(work->type, ov80_022372B4(work), work->trainerIds, 14);
    ov80_02236E24(ov80_022372B4(work), work->level, work->rentalIds, work->rentalMons, work->rentalIvs, work->rentalPids, work->winStreak, NULL, NULL);
    ov80_022300D4(work, 4, FALSE);
    ov80_022300D4(work, 5, FALSE);
    count = 6;
    for (i = 0; i < 6; i++) {
        candidates[i] = work->rentalMons[i];
    }
    if (ov80_02237254(work->type) == TRUE) {
        for (i = 0; i < 6; i++) {
            species[i] = work->rentalMons[i].species;
            items[i] = work->rentalMons[i].item;
        }
        ov80_02236E24(ov80_022372B4(work), work->level, work->partnerIds, work->partnerMons, work->partnerIvs, work->partnerPids, work->partner580, species, items);
        ov80_022300D4(work, 4, TRUE);
        ov80_022300D4(work, 5, TRUE);
        count = 12;
        for (i = 0; i < 6; i++) {
            candidates[6 + i] = work->partnerMons[i];
        }
    }
    ov80_02236E90(ov80_02236DF8(work->type, 1), work->trainerIds[work->battleNum], work->level, candidates, work->enemyIds, work->enemyMons, work->enemyIvs, work->enemyPids, count);
    for (i = 0; i < 6; i++) {
        mon = AllocMonZeroed(HEAP_ID_FIELD2);
        ov80_0222A140(&work->rentalMons[i], mon, ov80_02237120(work));
        ov80_0222A3BC(work->saveData, work->playerParty, mon);
        Heap_Free(mon);
    }
    for (i = 0; i < 6; i++) {
        Party_GetMonByIndex(work->playerParty, i);
    }
}

static void ov80_022300D4(BattleFactoryWork *work, int index, BOOL partner) {
    u16 id;
    u8 iv;
    u32 pid;
    FrontierMon mon;
    u16 other = LCRandom() % 6;

    if (partner == FALSE) {
        id = work->rentalIds[index];
        iv = work->rentalIvs[index];
        pid = work->rentalPids[index];
        mon = work->rentalMons[index];

        work->rentalIds[index] = work->rentalIds[other];
        work->rentalIvs[index] = work->rentalIvs[other];
        work->rentalPids[index] = work->rentalPids[other];
        work->rentalMons[index] = work->rentalMons[other];
        work->rentalIds[other] = id;
        work->rentalIvs[other] = iv;
        work->rentalPids[other] = pid;
        work->rentalMons[other] = mon;
    } else {
        id = work->partnerIds[index];
        iv = work->partnerIvs[index];
        pid = work->partnerPids[index];
        mon = work->partnerMons[index];

        work->partnerIds[index] = work->partnerIds[other];
        work->partnerIvs[index] = work->partnerIvs[other];
        work->partnerPids[index] = work->partnerPids[other];
        work->partnerMons[index] = work->partnerMons[other];
        work->partnerIds[other] = id;
        work->partnerIvs[other] = iv;
        work->partnerPids[other] = pid;
        work->partnerMons[other] = mon;
    }
}

static void ov80_02230270(BattleFactoryWork *work) {
    FrontierMon mons[6];
    u16 ids[6];
    u32 pids[6];
    u8 ivs[6];
    int i;
    Pokemon *mon;

    ov80_02236DD4(work->type);
    for (i = 0; i < 14; i++) {
        work->trainerIds[i] = sub_02030A24(work->frontierData, 3, i, NULL);
    }
    for (i = 0; i < 4; i++) {
        ids[i] = sub_02030A24(work->frontierData, 4, i, NULL);
        pids[i] = sub_02030A24(work->frontierData, 6, i, NULL);
        ivs[i] = sub_02030A24(work->frontierData, 5, i, NULL);
        work->monIds[i] = ids[i];
    }
    ov80_0222A52C(mons, ids, ivs, pids, NULL, 4, HEAP_ID_FIELD2, 0xCD);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < 4; i++) {
        ov80_0222A140(&mons[i], mon, ov80_02237120(work));
        ov80_0222A3BC(work->saveData, work->playerParty, mon);
    }
    Heap_Free(mon);
    for (i = 0; i < 4; i++) {
        ids[i] = sub_02030A24(work->frontierData, 7, i, NULL);
        pids[i] = sub_02030A24(work->frontierData, 9, i, NULL);
        ivs[i] = sub_02030A24(work->frontierData, 8, i, NULL);
        work->enemyIds[i] = ids[i];
    }
    ov80_0222A52C(mons, ids, ivs, pids, NULL, 4, HEAP_ID_FIELD2, 0xCD);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < 4; i++) {
        ov80_0222A140(&mons[i], mon, ov80_02237120(work));
        ov80_0222A3BC(work->saveData, work->enemyParty, mon);
    }
    Heap_Free(mon);
}

void ov80_02230424(BattleFactoryWork *work) {
    if (work == NULL) {
        return;
    }
    if (work->playerParty != NULL) {
        Heap_Free(work->playerParty);
    }
    if (work->enemyParty != NULL) {
        Heap_Free(work->enemyParty);
    }
    MI_CpuFill8(work, 0, sizeof(BattleFactoryWork));
    Heap_Free(work);
}

void ov80_02230460(BattleFactoryWork *work, BattleFactoryOtherWork *other) {
    int i;

    for (i = 0; i < 6; i++) {
        work->swapSlots[i] = ov80_02230484(other, i);
    }
}

static u16 ov80_02230484(BattleFactoryOtherWork *other, u8 index) {
    if (index >= 6) {
        GF_ASSERT(FALSE);
        return 0;
    }
    return other->unk10[index];
}

void ov80_0223049C(BattleFactoryWork *work, int mode) {
    int records;
    struct {
        u16 value16[4];
        u8 value8[4];
        u32 value32[4];
    } buf;
    FrontierSave *frontierSave;
    int stat;
    u32 prevStat;
    u32 newStat;
    u16 i;
    u32 count;
    Pokemon *mon;

    records = sub_02030AE8(work->saveData);
    frontierSave = Save_Frontier_GetStatic(work->saveData);
    ov80_02236DD4(work->type);
    ov80_02236DF8(work->type, 1);
    buf.value8[0] = work->level;
    sub_02030978(work->frontierData, 0, 0, buf.value8);
    buf.value8[0] = work->type;
    sub_02030978(work->frontierData, 1, 0, buf.value8);
    sub_02030964(work->frontierData, 1);
    buf.value8[0] = work->battleNum;
    sub_02030978(work->frontierData, 2, 0, buf.value8);
    stat = sub_0205C048(work->level, work->type);
    sub_02031108(frontierSave, stat, sub_0205C268(sub_0205C048(work->level, work->type)), work->winStreak);
    stat = sub_0205BFF0(work->level, work->type);
    sub_02031108(frontierSave, stat, sub_0205C268(sub_0205BFF0(work->level, work->type)), work->unk0C);
    if (mode != 2) {
        stat = sub_0205C01C(work->level, work->type);
        prevStat = FrontierSave_GetStat(frontierSave, stat, sub_0205C268(sub_0205C01C(work->level, work->type)));
        stat = sub_0205C01C(work->level, work->type);
        sub_0203126C(frontierSave, stat, sub_0205C268(sub_0205C01C(work->level, work->type)), work->unk0C);
        stat = sub_0205C01C(work->level, work->type);
        newStat = FrontierSave_GetStat(frontierSave, stat, sub_0205C268(sub_0205C01C(work->level, work->type)));
        if (work->unk0C == prevStat) {
            stat = sub_0205C074(work->level, work->type);
            sub_0203126C(frontierSave, stat, sub_0205C268(sub_0205C074(work->level, work->type)), work->winStreak);
        } else if (prevStat < newStat) {
            stat = sub_0205C074(work->level, work->type);
            sub_02031108(frontierSave, stat, sub_0205C268(sub_0205C074(work->level, work->type)), work->winStreak);
        }
        buf.value8[0] = work->unk0A;
        sub_02030AA4(records, 10, work->type + work->level * 4, buf.value8);
        if (work->type == 3) {
            if (work->level == 0) {
                stat = 0x66;
            } else {
                stat = 0x68;
            }
            sub_02031108(frontierSave, stat, sub_0205C268(stat), work->unk0A);
        }
    }
    for (i = 0; i < 14; i++) {
        buf.value16[0] = work->trainerIds[i];
        sub_02030978(work->frontierData, 3, i, buf.value16);
    }
    count = Party_GetCount(work->playerParty);
    for (i = 0; i < count; i++) {
        mon = Party_GetMonByIndex(work->playerParty, i);
        buf.value16[0] = work->monIds[i];
        sub_02030978(work->frontierData, 4, i, buf.value16);
        buf.value8[0] = GetMonData(mon, MON_DATA_ATK_IV, NULL);
        sub_02030978(work->frontierData, 5, i, buf.value8);
        buf.value32[0] = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
        sub_02030978(work->frontierData, 6, i, buf.value32);
    }
    count = Party_GetCount(work->enemyParty);
    for (i = 0; i < count; i++) {
        mon = Party_GetMonByIndex(work->enemyParty, i);
        buf.value16[0] = work->enemyIds[i];
        sub_02030978(work->frontierData, 7, i, buf.value16);
        buf.value8[0] = GetMonData(mon, MON_DATA_ATK_IV, NULL);
        sub_02030978(work->frontierData, 8, i, buf.value8);
        buf.value32[0] = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
        sub_02030978(work->frontierData, 9, i, buf.value32);
    }
}

u8 ov80_02230784(BattleFactoryWork *work) {
    work->battleNum++;
    return work->battleNum;
}

u8 ov80_02230790(BattleFactoryWork *work) {
    return work->battleNum;
}

u16 ov80_02230794(BattleFactoryWork *work, int a1) {
    BattleFactoryTrainer trainer;

    Heap_Free(ov80_02229F04(&trainer, work->trainerIds[(u8)(work->battleNum + a1 * 7)], HEAP_ID_FIELD2, 0xCC));
    return ov80_0222A30C(trainer.unk04);
}

void ov80_022307C8(BattleFactoryWork *work) {
    ov80_0223049C(work, 1);
}

void ov80_022307D4(BattleFactoryWork *work) {
    work->unk0A = 1;
    if (work->unk0E < 8) {
        work->unk0E++;
    }
    work->battleNum = 0;
    ov80_0223049C(work, 0);
}

void ov80_022307F0(BattleFactoryWork *work) {
    int count;
    int enemyCount;
    Pokemon *mon;
    int i;

    count = ov80_02236DD4(work->type);
    enemyCount = ov80_02236DF8(work->type, 1);
    SaveArray_Party_Init(work->playerParty);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < count; i++) {
        ov80_0222A140(&work->rentalMons[work->swapSlots[i]], mon, ov80_02237120(work));
        ov80_0222A3BC(work->saveData, work->playerParty, mon);
        work->monIds[i] = work->rentalIds[work->swapSlots[i]];
    }
    for (i = 0; i < enemyCount; i++) {
        ov80_0222A140(&work->enemyMons[i], mon, ov80_02237120(work));
        ov80_0222A3BC(work->saveData, work->enemyParty, mon);
    }
    Heap_Free(mon);
}

// NONMATCHING: retail spills the enemy-loop work walker above enemyCount
// (sp+0x18 vs sp+0x14); declaration order and loop shape do not move it
#ifdef NONMATCHING
void ov80_022308C4(BattleFactoryWork *work) {
    u16 species[8];
    u16 items[8];
    Pokemon *enemyMon;
    Pokemon *mon;
    int playerCount;
    int count;
    int enemyCount;
    int i;

    for (i = 0; i < 8; i++) {
        species[i] = 0;
        items[i] = 0;
    }
    count = ov80_02236DF8(work->type, 1);
    playerCount = Party_GetCount(work->playerParty);
    for (i = 0; i < playerCount; i++) {
        mon = Party_GetMonByIndex(work->playerParty, i);
        species[i] = GetMonData(mon, MON_DATA_SPECIES, NULL);
        items[i] = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    }
    enemyCount = Party_GetCount(work->enemyParty);
    for (i = 0; i < enemyCount; i++) {
        enemyMon = Party_GetMonByIndex(work->enemyParty, i);
        species[playerCount + i] = GetMonData(enemyMon, MON_DATA_SPECIES, NULL);
        items[playerCount + i] = GetMonData(enemyMon, MON_DATA_HELD_ITEM, NULL);
        work->rentalIds[i] = work->enemyIds[i];
    }
    ov80_02236C9C(species, items, playerCount + enemyCount, count, work->enemyIds, HEAP_ID_FIELD2, ov80_02236C2C(work->trainerIds[work->battleNum], work->level), 0, work->enemyIvs);
    ov80_0222A52C(work->enemyMons, work->enemyIds, work->enemyIvs, NULL, work->enemyPids, count, HEAP_ID_FIELD2, 0xCD);
}
#else
// clang-format off
asm void ov80_022308C4(BattleFactoryWork *work) {
    push {r4, r5, r6, r7, lr}
    sub sp, #0x4c
    mov r1, #0
    add r5, r0, #0
    add r2, sp, #0x3c
    add r3, sp, #0x2c
    add r0, r1, #0
_022308D2:
    add r1, r1, #1
    strh r0, [r2, #0]
    strh r0, [r3, #0]
    add r2, r2, #2
    add r3, r3, #2
    cmp r1, #8
    blt _022308D2
    ldrb r0, [r5, #4]
    mov r1, #1
    bl ov80_02236DF8
    str r0, [sp, #0x1c]
    ldr r0, [pc, #0x100]
    ldr r0, [r5, r0]
    bl Party_GetCount
    mov r7, #0
    str r0, [sp, #0x20]
    cmp r0, #0
    ble _0223092C
    add r6, sp, #0x3c
    add r4, sp, #0x2c
_022308FE:
    ldr r0, [pc, #0xec]
    add r1, r7, #0
    ldr r0, [r5, r0]
    bl Party_GetMonByIndex
    str r0, [sp, #0x24]
    mov r1, #5
    mov r2, #0
    bl GetMonData
    strh r0, [r6, #0]
    ldr r0, [sp, #0x24]
    mov r1, #6
    mov r2, #0
    bl GetMonData
    strh r0, [r4, #0]
    ldr r0, [sp, #0x20]
    add r7, r7, #1
    add r6, r6, #2
    add r4, r4, #2
    cmp r7, r0
    blt _022308FE
_0223092C:
    ldr r0, [pc, #0xc0]
    ldr r0, [r5, r0]
    bl Party_GetCount
    mov r7, #0
    str r0, [sp, #0x14]
    cmp r0, #0
    ble _0223098A
    ldr r0, [sp, #0x20]
    str r5, [sp, #0x18]
    lsl r1, r0, #1
    add r0, sp, #0x3c
    add r6, r0, r1
    add r0, sp, #0x2c
    add r4, r0, r1
_0223094A:
    ldr r0, [pc, #0xa4]
    add r1, r7, #0
    ldr r0, [r5, r0]
    bl Party_GetMonByIndex
    str r0, [sp, #0x28]
    mov r1, #5
    mov r2, #0
    bl GetMonData
    strh r0, [r6, #0]
    ldr r0, [sp, #0x28]
    mov r1, #6
    mov r2, #0
    bl GetMonData
    strh r0, [r4, #0]
    ldr r1, [sp, #0x18]
    ldr r0, [pc, #0x84]
    add r7, r7, #1
    ldrh r2, [r1, r0]
    mov r0, #0x95
    lsl r0, r0, #2
    strh r2, [r1, r0]
    add r0, r1, #0
    add r0, r0, #2
    str r0, [sp, #0x18]
    ldr r0, [sp, #0x14]
    add r6, r6, #2
    add r4, r4, #2
    cmp r7, r0
    blt _0223094A
_0223098A:
    ldrb r0, [r5, #6]
    ldrb r1, [r5, #5]
    lsl r0, r0, #1
    add r0, r5, r0
    ldrh r0, [r0, #0x18]
    bl ov80_02236C2C
    ldr r1, [pc, #0x58]
    ldr r3, [sp, #0x20]
    add r2, r5, r1
    str r2, [sp]
    mov r2, #0xb
    str r2, [sp, #4]
    str r0, [sp, #8]
    mov r0, #0
    ldr r2, [sp, #0x14]
    add r1, #8
    str r0, [sp, #0xc]
    add r0, r5, r1
    str r0, [sp, #0x10]
    add r2, r3, r2
    ldr r3, [sp, #0x1c]
    add r0, sp, #0x3c
    add r1, sp, #0x2c
    bl ov80_02236C9C
    mov r2, #0x3e
    lsl r2, r2, #4
    add r0, r5, r2
    str r0, [sp]
    ldr r0, [sp, #0x1c]
    add r1, r2, #0
    str r0, [sp, #4]
    mov r0, #0xb
    str r0, [sp, #8]
    mov r0, #0xcd
    str r0, [sp, #0xc]
    add r0, r2, #0
    add r0, #0x10
    sub r1, #0xe
    sub r2, r2, #6
    add r0, r5, r0
    add r1, r5, r1
    add r2, r5, r2
    mov r3, #0
    bl ov80_0222A52C
    add sp, #0x4c
    pop {r4, r5, r6, r7, pc}
    dcd 0x000004D4
    dcd 0x000004D8
    dcd 0x000003D2
}
// clang-format on
#endif // NONMATCHING

void ov80_022309F8(BattleFactoryWork *work) {
    if (work->swapSlots[0] != 0xFF) {
        Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(work->playerParty, work->swapSlots[0], Party_GetMonByIndex(work->enemyParty, work->swapSlots[1]));
        work->monIds[work->swapSlots[0]] = work->rentalIds[work->swapSlots[1]];
        ov80_02230AE4(work);
        GameStats_Inc(Save_GameStats_Get(work->saveData), GAME_STAT_UNK65);
    }
}

void ov80_02230A60(BattleFactoryWork *work) {
    int count;
    Pokemon *mon;
    int i;

    ov80_02236DD4(work->type);
    count = ov80_02236DF8(work->type, 1);
    SaveArray_Party_Init(work->enemyParty);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < count; i++) {
        ov80_0222A140(&work->enemyMons[i], mon, ov80_02237120(work));
        ov80_0222A3BC(work->saveData, work->enemyParty, mon);
    }
    Heap_Free(mon);
    for (i = 0; i < count; i++) {
        Party_GetMonByIndex(work->enemyParty, i);
    }
}

void ov80_02230AE4(BattleFactoryWork *work) {
    if (work->winStreak < 9999) {
        work->winStreak++;
    }
}

BOOL ov80_02230AF8(BattleFactoryWork *work, u16 cmd, u16 arg) {
    switch (cmd) {
    case 0:
        return ov80_0222B108(work);
    case 1:
        return ov80_0222B174(work);
    case 2:
        return ov80_0222B1DC(work);
    case 3:
        return ov80_0222B2C4(work);
    case 4:
        return ov80_0222B3B0(work, arg);
    case 5:
        return ov80_0222B3FC(work, arg);
    case 6:
        return ov80_0222B448(work);
    }
}

u8 ov80_02230B4C(BattleFactoryWork *work) {
    u8 value;

    if (work->type <= 1) {
        if (work->unk0E >= 8) {
            value = 9;
        } else {
            value = ov80_0223BDD4[work->unk0E];
        }
    } else {
        if (work->unk0E >= 8) {
            value = 21;
        } else {
            value = ov80_0223BDE0[work->unk0E];
        }
    }
    if (work->type == 0 && (work->unk0C == 21 || work->unk0C == 49)) {
        value = 20;
    }
    return value;
}
