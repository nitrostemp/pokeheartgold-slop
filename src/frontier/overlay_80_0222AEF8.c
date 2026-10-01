#include "global.h"

#include "frontier/overlay_80_02236B78.h"
#include "frontier/overlay_80_02238034.h"

#include "heap.h"
#include "party.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pokemon.h"
#include "save.h"
#include "unk_02030A98.h"
#include "unk_02035900.h"
#include "unk_0205BFF0.h"

// Link-communication send/receive handlers for four Battle Frontier
// facilities. Only the fields these handlers touch are named in the
// facility work structs below.

typedef struct FrontierLinkWorkA {
    u8 unk000[0x18];
    u16 data[20]; // 0x018
    u8 unk040[0x220];
    u8 slot; // 0x260
    u8 unk261[0x7];
    u16 partnerData[20]; // 0x268
    u8 unk290[0x46C];
    SaveData *saveData; // 0x6FC
    u8 unk700[0x2C];
    u16 sendBuf[22]; // 0x72C
    u8 unk758[0x2C];
    u8 monBuf[512];     // 0x784
    u8 recvBuf[2][512]; // 0x984
    u8 unkD84[0x4];
    u8 partnerValue; // 0xD88
    u8 unkD89[0x3];
    void *partnerMon; // 0xD8C
    u8 recvCount;     // 0xD90
} FrontierLinkWorkA;

typedef struct FrontierLinkWorkB {
    u8 unk000[0x4];
    u8 type; // 0x004
    u8 unk005[0x3];
    u16 unk08; // 0x008
    u8 unk00A[0x2];
    u16 unk0C; // 0x00C
    u16 unk0E; // 0x00E
    u8 unk010[0x8];
    u16 data[14]; // 0x018
    u8 unk034[0x220];
    u16 partnerMonIds[6]; // 0x254
    u8 partnerIvs[6];     // 0x260
    u8 unk266[0x2];
    u32 partnerPersonality[6]; // 0x268
    u8 unk280[0x152];
    u16 enemyMonIds[4]; // 0x3D2
    u8 enemyIvs[4];     // 0x3DA
    u8 unk3DE[0x2];
    u32 enemyPersonality[4]; // 0x3E0
    u8 unk3F0[0xE4];
    Party *party; // 0x4D4
    u8 unk4D8[0x10];
    u16 monIds[6]; // 0x4E8
    u8 unk4F4[0x4];
    SaveData *saveData; // 0x4F8
    u8 unk4FC[0x8];
    u16 sendBuf[30]; // 0x504
    u8 unk540[0x3C];
    u8 partnerValue0;    // 0x57C
    u8 partnerValue1;    // 0x57D
    u16 partner57E;      // 0x57E
    u16 partner580;      // 0x580
    u16 partner582;      // 0x582
    u16 rentalMonIds[6]; // 0x584
    u8 rentalIvs[6];     // 0x590
    u8 unk596[0x2];
    u32 rentalPersonality[6]; // 0x598
    u8 unk5B0[0x152];
    u8 recvCount; // 0x702
} FrontierLinkWorkB;

typedef struct FrontierLinkWorkC {
    u8 unk000[0x4];
    SaveData *saveData; // 0x004
    u8 unk008[0x8];
    u8 type; // 0x010
    u8 unk011[0x3];
    u16 unk14; // 0x014
    u16 unk16; // 0x016
    u8 unk018[0x10];
    Party *party; // 0x028
    u8 unk02C[0x4];
    u16 data[14]; // 0x030
    u8 unk04C[0x220];
    u16 enemyMonIds[4];      // 0x26C
    u8 enemyIvs[4];          // 0x274
    u32 enemyPersonality[4]; // 0x278
    u8 unk288[0x138];
    u16 sendBuf[20]; // 0x3C0
    u8 unk3E8[0x28];
    u8 monBuf[512];     // 0x410
    u8 recvBuf[2][512]; // 0x610
    u8 partnerValue0;   // 0xA10
    u8 partnerValue1;   // 0xA11
    u16 partnerA12;     // 0xA12
    u8 unkA14[0x2];
    u16 partnerA16;   // 0xA16
    u8 choice;        // 0xA18
    u8 partnerChoice; // 0xA19
    u8 recvCount;     // 0xA1A
    u8 decision;      // 0xA1B
    u16 partnerA1C;   // 0xA1C
    u8 unkA1E[0x2];
    u16 *resultPtr; // 0xA20
} FrontierLinkWorkC;

typedef struct FrontierLinkWorkD {
    u8 unk000[0x4];
    SaveData *saveData; // 0x004
    u8 unk008[0x8];
    u8 type; // 0x010
    u8 unk011[0x7];
    u16 unk18; // 0x018
    u16 unk1A; // 0x01A
    u8 unk01C[0x54];
    Party *party; // 0x070
    u8 unk074[0x4];
    u16 data[14]; // 0x078
    u8 unk094[0x280];
    u16 enemyMonIds[4];      // 0x314
    u8 enemyIvs[4];          // 0x31C
    u32 enemyPersonality[4]; // 0x320
    u8 unk330[0xF4];
    u16 sendBuf[20]; // 0x424
    u8 unk44C[0x28];
    u8 monBuf[512];     // 0x474
    u8 recvBuf[2][512]; // 0x674
    u8 partnerValue0;   // 0xA74
    u8 partnerValue1;   // 0xA75
    u16 partnerA76;     // 0xA76
    u16 partnerA78;     // 0xA78
    u8 unkA7A[0x2];
    u8 recvCount; // 0xA7C
} FrontierLinkWorkD;

BOOL sub_02036FD8(int cmd, void *data, int size);
u8 ov80_02237B24(u8 type, int a1);

void ov80_0222AEF8(int netId, int size, void *src, FrontierLinkWorkA *work);
BOOL ov80_0222AF10(FrontierLinkWorkA *work);
void ov80_0222AF3C(int netId, int size, void *src, FrontierLinkWorkA *work);
BOOL ov80_0222AF54(FrontierLinkWorkA *work);
void ov80_0222AF84(int netId, int size, u16 *src, FrontierLinkWorkA *work);
BOOL ov80_0222AFB8(FrontierLinkWorkA *work);
void ov80_0222AFEC(int netId, int size, u16 *src, FrontierLinkWorkA *work);
BOOL ov80_0222B024(FrontierLinkWorkA *work, u16 value);
void ov80_0222B048(int netId, int size, u16 *src, FrontierLinkWorkA *work);
BOOL ov80_0222B070(FrontierLinkWorkA *work);
void ov80_0222B0B8(int netId, int size, void *src, FrontierLinkWorkA *work);
u8 *ov80_0222B0E8(int netId, FrontierLinkWorkA *work, int size);
BOOL ov80_0222B108(FrontierLinkWorkB *work);
void ov80_0222B140(int netId, int size, u16 *src, FrontierLinkWorkB *work);
BOOL ov80_0222B174(FrontierLinkWorkB *work);
void ov80_0222B1A4(int netId, int size, u16 *src, FrontierLinkWorkB *work);
BOOL ov80_0222B1DC(FrontierLinkWorkB *work);
void ov80_0222B24C(int netId, int size, u16 *src, FrontierLinkWorkB *work);
BOOL ov80_0222B2C4(FrontierLinkWorkB *work);
void ov80_0222B334(int netId, int size, u16 *src, FrontierLinkWorkB *work);
BOOL ov80_0222B3B0(FrontierLinkWorkB *work, u16 value);
void ov80_0222B3D4(int netId, int size, u16 *src, FrontierLinkWorkB *work);
BOOL ov80_0222B3FC(FrontierLinkWorkB *work, u16 value);
void ov80_0222B420(int netId, int size, u16 *src, FrontierLinkWorkB *work);
BOOL ov80_0222B448(FrontierLinkWorkB *work);
void ov80_0222B52C(int netId, int size, u16 *src, FrontierLinkWorkB *work);
BOOL ov80_0222B5C8(FrontierLinkWorkC *work);
void ov80_0222B628(int netId, int size, u16 *src, FrontierLinkWorkC *work);
BOOL ov80_0222B65C(FrontierLinkWorkC *work);
void ov80_0222B690(int netId, int size, u16 *src, FrontierLinkWorkC *work);
BOOL ov80_0222B6C8(FrontierLinkWorkC *work);
void ov80_0222B740(int netId, int size, u16 *src, FrontierLinkWorkC *work);
BOOL ov80_0222B7E4(FrontierLinkWorkC *work);
void ov80_0222B860(int netId, int size, u16 *src, FrontierLinkWorkC *work);
BOOL ov80_0222B8D8(FrontierLinkWorkC *work, u16 value);
void ov80_0222B8F8(int netId, int size, u16 *src, FrontierLinkWorkC *work);
BOOL ov80_0222B920(FrontierLinkWorkC *work, u16 value);
void ov80_0222B940(int netId, int size, u16 *src, FrontierLinkWorkC *work);
BOOL ov80_0222B968(FrontierLinkWorkC *work);
void ov80_0222B9CC(int netId, int size, u8 *src, FrontierLinkWorkC *work);
u8 *ov80_0222BA5C(int netId, FrontierLinkWorkC *work, int size);
BOOL ov80_0222BA7C(FrontierLinkWorkD *work);
void ov80_0222BAB0(int netId, int size, u16 *src, FrontierLinkWorkD *work);
BOOL ov80_0222BAE0(FrontierLinkWorkD *work);
void ov80_0222BB18(int netId, int size, u16 *src, FrontierLinkWorkD *work);
BOOL ov80_0222BB54(FrontierLinkWorkD *work);
void ov80_0222BBD0(int netId, int size, u16 *src, FrontierLinkWorkD *work);
BOOL ov80_0222BC48(FrontierLinkWorkD *work, u16 value);
void ov80_0222BC6C(int netId, int size, u16 *src, FrontierLinkWorkD *work);
BOOL ov80_0222BC94(FrontierLinkWorkD *work, u16 value);
void ov80_0222BCB8(int netId, int size, u16 *src, FrontierLinkWorkD *work);
BOOL ov80_0222BCE0(FrontierLinkWorkD *work);
void ov80_0222BD44(int netId, int size, u8 *src, FrontierLinkWorkD *work);
u8 *ov80_0222BDD4(int netId, FrontierLinkWorkD *work, int size);

void ov80_0222AEF8(int netId, int size, void *src, FrontierLinkWorkA *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
}

BOOL ov80_0222AF10(FrontierLinkWorkA *work) {
    Save_PlayerData_GetProfile(work->saveData);
    if (sub_02037030(0x22, work->sendBuf, 0x2C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222AF3C(int netId, int size, void *src, FrontierLinkWorkA *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
}

BOOL ov80_0222AF54(FrontierLinkWorkA *work) {
    int i;
    u16 *buf = work->sendBuf;

    for (i = 0; i < 20; i++) {
        buf[i] = work->data[i];
    }
    if (sub_02037030(0x23, buf, 0x2C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222AF84(int netId, int size, u16 *src, FrontierLinkWorkA *work) {
    int i;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    if (sub_0203769C() == 0) {
        return;
    }
    for (i = 0; i < 20; i++) {
        work->data[i] = src[i];
    }
}

BOOL ov80_0222AFB8(FrontierLinkWorkA *work) {
    int i;
    u16 *buf = work->sendBuf;

    for (i = 0; i < 20; i++) {
        buf[i] = work->partnerData[i];
    }
    if (sub_02037030(0x24, buf, 0x2C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222AFEC(int netId, int size, u16 *src, FrontierLinkWorkA *work) {
    int i;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    if (sub_0203769C() == 0) {
        return;
    }
    for (i = 0; i < 20; i++) {
        work->partnerData[i] = src[i];
    }
}

BOOL ov80_0222B024(FrontierLinkWorkA *work, u16 value) {
    work->sendBuf[0] = value;
    if (sub_02037030(0x25, work->sendBuf, 0x2C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B048(int netId, int size, u16 *src, FrontierLinkWorkA *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partnerValue = src[0];
}

BOOL ov80_0222B070(FrontierLinkWorkA *work) {
    int size = SizeOfStructPokemon();
    Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(work->saveData), work->slot);

    MI_CpuCopy8(mon, work->monBuf, size);
    if (sub_02036FD8(0x26, work->monBuf, 0x200) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B0B8(int netId, int size, void *src, FrontierLinkWorkA *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    MI_CpuCopy8(src, work->partnerMon, SizeOfStructPokemon());
}

u8 *ov80_0222B0E8(int netId, FrontierLinkWorkA *work, int size) {
    GF_ASSERT(size <= 0x200);
    return work->recvBuf[netId];
}

BOOL ov80_0222B108(FrontierLinkWorkB *work) {
    u16 *buf = work->sendBuf;

    Save_PlayerData_GetProfile(work->saveData);
    buf[1] = work->unk08;
    buf[2] = work->unk0C;
    buf[3] = work->unk0E;
    if (sub_02037030(0x16, buf, 0x3C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B140(int netId, int size, u16 *src, FrontierLinkWorkB *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partner580 = src[1];
    work->partner582 = src[2];
    work->partner57E = src[3];
}

BOOL ov80_0222B174(FrontierLinkWorkB *work) {
    int i;
    u16 *buf = work->sendBuf;

    for (i = 0; i < 14; i++) {
        buf[i] = work->data[i];
    }
    if (sub_02037030(0x17, buf, 0x3C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B1A4(int netId, int size, u16 *src, FrontierLinkWorkB *work) {
    int i;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    if (sub_0203769C() == 0) {
        return;
    }
    for (i = 0; i < 14; i++) {
        work->data[i] = src[i];
    }
}

BOOL ov80_0222B1DC(FrontierLinkWorkB *work) {
    int i;
    u16 *p;
    u16 *buf = work->sendBuf;

    for (i = 0; i < 6; i++) {
        buf[i] = work->rentalMonIds[i];
    }
    for (i = 0; i < 6; i++) {
        buf[6 + i] = work->rentalIvs[i];
    }
    for (i = 0, p = buf + 12; i < 6; i++) {
        p[0] = work->rentalPersonality[i];
        p[6] = work->rentalPersonality[i] >> 16;
        p++;
    }
    if (sub_02037030(0x18, buf, 0x3C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B24C(int netId, int size, u16 *src, FrontierLinkWorkB *work) {
    int i;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    if (sub_0203769C() == 0) {
        return;
    }
    for (i = 0; i < 6; i++) {
        work->partnerMonIds[i] = src[i];
    }
    for (i = 0; i < 6; i++) {
        work->partnerIvs[i] = src[6 + i];
    }
    for (i = 0, src += 12; i < 6; i++) {
        work->partnerPersonality[i] = src[0];
        work->partnerPersonality[i] |= src[6] << 16;
        src++;
    }
}

BOOL ov80_0222B2C4(FrontierLinkWorkB *work) {
    int i;
    u16 *p;
    u16 *buf = work->sendBuf;

    for (i = 0; i < 4; i++) {
        buf[i] = work->enemyMonIds[i];
    }
    for (i = 0; i < 4; i++) {
        buf[4 + i] = work->enemyIvs[i];
    }
    for (i = 0, p = buf + 8; i < 4; i++) {
        p[0] = work->enemyPersonality[i];
        p[4] = work->enemyPersonality[i] >> 16;
        p++;
    }
    if (sub_02037030(0x19, buf, 0x3C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B334(int netId, int size, u16 *src, FrontierLinkWorkB *work) {
    int i;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    if (sub_0203769C() == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        work->enemyMonIds[i] = src[i];
    }
    for (i = 0; i < 4; i++) {
        work->enemyIvs[i] = src[4 + i];
    }
    for (i = 0, src += 8; i < 4; i++) {
        work->enemyPersonality[i] = src[0];
        work->enemyPersonality[i] |= src[4] << 16;
        src++;
    }
}

BOOL ov80_0222B3B0(FrontierLinkWorkB *work, u16 value) {
    work->sendBuf[0] = value;
    if (sub_02037030(0x1A, work->sendBuf, 0x3C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B3D4(int netId, int size, u16 *src, FrontierLinkWorkB *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partnerValue0 = src[0];
}

BOOL ov80_0222B3FC(FrontierLinkWorkB *work, u16 value) {
    work->sendBuf[0] = value;
    if (sub_02037030(0x1B, work->sendBuf, 0x3C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B420(int netId, int size, u16 *src, FrontierLinkWorkB *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partnerValue1 = src[0];
}

BOOL ov80_0222B448(FrontierLinkWorkB *work) {
    u32 personality[2];
    u8 ivs[4];
    Pokemon *mon;
    int i;
    int offset;
    u16 *buf;
    int count;

    buf = work->sendBuf;
    offset = 0;
    count = ov80_02236DD4(work->type);
    for (i = 0; i < count; i++) {
        mon = Party_GetMonByIndex(work->party, i);
        ivs[i] = GetMonData(mon, MON_DATA_ATK_IV, NULL);
        personality[i] = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
    }
    for (i = 0; i < count; i++) {
        buf[i] = work->monIds[i];
    }
    offset += count;
    for (i = 0; i < count; i++) {
        buf[offset + i] = ivs[i];
    }
    offset += count;
    for (i = 0; i < count; i++) {
        buf[offset + i] = personality[i];
        buf[count + offset + i] = personality[i] >> 16;
    }
    if (sub_02037030(0x1C, buf, 0x3C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B52C(int netId, int size, u16 *src, FrontierLinkWorkB *work) {
    int i;
    int count;
    int offset = 0;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    count = ov80_02236DD4(work->type);
    for (i = 0; i < count; i++) {
        work->rentalMonIds[i] = src[i];
    }
    offset += count;
    for (i = 0; i < count; i++) {
        work->rentalIvs[i] = src[offset + i];
    }
    offset += count;
    for (i = 0; i < count; i++) {
        work->rentalPersonality[i] = src[offset + i];
        work->rentalPersonality[i] |= src[count + offset + i] << 16;
    }
}

BOOL ov80_0222B5C8(FrontierLinkWorkC *work) {
    FrontierSave *frontierSave;
    u32 a1;

    sub_02030E08(work->saveData);
    work->sendBuf[1] = work->unk14;
    work->sendBuf[2] = work->unk16;
    frontierSave = Save_Frontier_GetStatic(work->saveData);
    a1 = sub_0205C1F0(work->type);
    work->sendBuf[11] = FrontierSave_GetStat(frontierSave, a1, sub_0205C268(sub_0205C1F0(work->type)));
    if (sub_02037030(0x2A, work->sendBuf, 0x28) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B628(int netId, int size, u16 *src, FrontierLinkWorkC *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partnerA16 = src[1];
    work->partnerA12 = src[2];
    work->partnerA1C = src[11];
}

BOOL ov80_0222B65C(FrontierLinkWorkC *work) {
    int i;

    for (i = 0; i < 14; i++) {
        work->sendBuf[i] = work->data[i];
    }
    if (sub_02037030(0x2B, work->sendBuf, 0x28) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B690(int netId, int size, u16 *src, FrontierLinkWorkC *work) {
    int i;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    if (sub_0203769C() == 0) {
        return;
    }
    for (i = 0; i < 14; i++) {
        work->data[i] = src[i];
    }
}

BOOL ov80_0222B6C8(FrontierLinkWorkC *work) {
    work->sendBuf[0] = work->choice;
    if (sub_0203769C() == 0) {
        if (work->decision == 0) {
            work->decision = work->choice;
        } else if (work->decision - 6 == 4) {
            if (work->choice != 4) {
                work->decision = work->choice;
            }
        }
    } else {
        if (work->decision == 4 && work->choice != 4) {
            work->decision = work->choice + 6;
        }
    }
    work->sendBuf[1] = work->decision;
    if (sub_02037030(0x2C, work->sendBuf, 0x28) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B740(int netId, int size, u16 *src, FrontierLinkWorkC *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partnerChoice = src[0];
    if (sub_0203769C() == 0) {
        switch (work->decision) {
        case 4:
            if (work->partnerChoice != 4) {
                work->choice = work->partnerChoice + 6;
                work->decision = work->partnerChoice + 6;
            }
            break;
        case 0:
            work->decision = work->partnerChoice + 6;
            if (work->partnerChoice != 4) {
                *work->resultPtr = 0xEEDD;
            }
            break;
        }
    } else {
        work->decision = src[1];
        if (work->decision != 4) {
            *work->resultPtr = 0xEEDD;
        }
        if (work->partnerChoice == 4 && work->choice != 0 && work->choice != 4) {
            work->decision = work->choice + 6;
        }
    }
}

BOOL ov80_0222B7E4(FrontierLinkWorkC *work) {
    int i, j;

    for (i = 0; i < 4; i++) {
        work->sendBuf[i] = work->enemyMonIds[i];
    }
    for (i = 0; i < 4; i++) {
        work->sendBuf[4 + i] = work->enemyIvs[i];
    }
    for (i = 0, j = 8; i < 4; i++, j++) {
        work->sendBuf[j] = work->enemyPersonality[i];
        work->sendBuf[j + 4] = work->enemyPersonality[i] >> 16;
    }
    if (sub_02037030(0x2D, work->sendBuf, 0x28) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B860(int netId, int size, u16 *src, FrontierLinkWorkC *work) {
    int i;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    if (sub_0203769C() == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        work->enemyMonIds[i] = src[i];
    }
    for (i = 0; i < 4; i++) {
        work->enemyIvs[i] = src[4 + i];
    }
    for (i = 0, src += 8; i < 4; i++) {
        work->enemyPersonality[i] = src[0];
        work->enemyPersonality[i] |= src[4] << 16;
        src++;
    }
}

BOOL ov80_0222B8D8(FrontierLinkWorkC *work, u16 value) {
    work->sendBuf[0] = value;
    if (sub_02037030(0x2E, work->sendBuf, 0x28) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B8F8(int netId, int size, u16 *src, FrontierLinkWorkC *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partnerValue0 = src[0];
}

BOOL ov80_0222B920(FrontierLinkWorkC *work, u16 value) {
    work->sendBuf[0] = value;
    if (sub_02037030(0x2F, work->sendBuf, 0x28) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B940(int netId, int size, u16 *src, FrontierLinkWorkC *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partnerValue1 = src[0];
}

BOOL ov80_0222B968(FrontierLinkWorkC *work) {
    int count;
    int i;
    int size;

    count = ov80_02237B24(work->type, 0);
    size = SizeOfStructPokemon();
    for (i = 0; i < count; i++) {
        MI_CpuCopy8(Party_GetMonByIndex(work->party, i), work->monBuf + i * size, size);
    }
    if (sub_02036FD8(0x30, work->monBuf, 0x200) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222B9CC(int netId, int size, u8 *src, FrontierLinkWorkC *work) {
    int count;
    int monSize;
    Pokemon *mon;
    int i;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    count = ov80_02237B24(work->type, 0);
    monSize = SizeOfStructPokemon();
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < count; i++) {
        MI_CpuCopy8(src + i * monSize, mon, monSize);
        Party_AddMon(work->party, mon);
    }
    Heap_Free(mon);
    if (sub_0203769C() != 0) {
        Party_SwapSlots(work->party, 0, 2);
        Party_SwapSlots(work->party, 1, 3);
    }
}

u8 *ov80_0222BA5C(int netId, FrontierLinkWorkC *work, int size) {
    GF_ASSERT(size <= 0x200);
    return work->recvBuf[netId];
}

BOOL ov80_0222BA7C(FrontierLinkWorkD *work) {
    sub_02030FA0(work->saveData);
    work->sendBuf[1] = work->unk18;
    work->sendBuf[2] = work->unk1A;
    if (sub_02037030(0x41, work->sendBuf, 0x28) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222BAB0(int netId, int size, u16 *src, FrontierLinkWorkD *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partnerA78 = src[1];
    work->partnerA76 = src[2];
}

BOOL ov80_0222BAE0(FrontierLinkWorkD *work) {
    int i;

    for (i = 0; i < 14; i++) {
        work->sendBuf[i] = work->data[i];
    }
    if (sub_02037030(0x42, work->sendBuf, 0x28) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222BB18(int netId, int size, u16 *src, FrontierLinkWorkD *work) {
    int i;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    if (sub_0203769C() == 0) {
        return;
    }
    for (i = 0; i < 14; i++) {
        work->data[i] = src[i];
    }
}

BOOL ov80_0222BB54(FrontierLinkWorkD *work) {
    int i, j;

    for (i = 0; i < 4; i++) {
        work->sendBuf[i] = work->enemyMonIds[i];
    }
    for (i = 0; i < 4; i++) {
        work->sendBuf[4 + i] = work->enemyIvs[i];
    }
    for (i = 0, j = 8; i < 4; i++, j++) {
        work->sendBuf[j] = work->enemyPersonality[i];
        work->sendBuf[j + 4] = work->enemyPersonality[i] >> 16;
    }
    if (sub_02037030(0x43, work->sendBuf, 0x28) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222BBD0(int netId, int size, u16 *src, FrontierLinkWorkD *work) {
    int i;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    if (sub_0203769C() == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        work->enemyMonIds[i] = src[i];
    }
    for (i = 0; i < 4; i++) {
        work->enemyIvs[i] = src[4 + i];
    }
    for (i = 0, src += 8; i < 4; i++) {
        work->enemyPersonality[i] = src[0];
        work->enemyPersonality[i] |= src[4] << 16;
        src++;
    }
}

BOOL ov80_0222BC48(FrontierLinkWorkD *work, u16 value) {
    work->sendBuf[0] = value;
    if (sub_02037030(0x44, work->sendBuf, 0x28) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222BC6C(int netId, int size, u16 *src, FrontierLinkWorkD *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partnerValue0 = src[0];
}

BOOL ov80_0222BC94(FrontierLinkWorkD *work, u16 value) {
    work->sendBuf[0] = value;
    if (sub_02037030(0x45, work->sendBuf, 0x28) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222BCB8(int netId, int size, u16 *src, FrontierLinkWorkD *work) {
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partnerValue1 = src[0];
}

BOOL ov80_0222BCE0(FrontierLinkWorkD *work) {
    int count;
    int i;
    int size;

    count = BattleArcade_GetMonCount(work->type, 0);
    size = SizeOfStructPokemon();
    for (i = 0; i < count; i++) {
        MI_CpuCopy8(Party_GetMonByIndex(work->party, i), work->monBuf + i * size, size);
    }
    if (sub_02036FD8(0x46, work->monBuf, 0x200) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222BD44(int netId, int size, u8 *src, FrontierLinkWorkD *work) {
    int count;
    int monSize;
    Pokemon *mon;
    int i;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    count = BattleArcade_GetMonCount(work->type, 0);
    monSize = SizeOfStructPokemon();
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < count; i++) {
        MI_CpuCopy8(src + i * monSize, mon, monSize);
        Party_AddMon(work->party, mon);
    }
    Heap_Free(mon);
    if (sub_0203769C() != 0) {
        Party_SwapSlots(work->party, 0, 2);
        Party_SwapSlots(work->party, 1, 3);
    }
}

u8 *ov80_0222BDD4(int netId, FrontierLinkWorkD *work, int size) {
    GF_ASSERT(size <= 0x200);
    return work->recvBuf[netId];
}
