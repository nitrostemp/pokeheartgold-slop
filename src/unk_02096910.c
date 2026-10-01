#include "unk_02096910.h"

#include "global.h"

#include "constants/items.h"

#include "overlay_80_022384D8.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "save.h"
#include "unk_02030A98.h"
#include "unk_02033AE0.h"
#include "unk_02035900.h"

// Battle Frontier link command table and the handlers for the facilities
// whose link code lives in the static module.

typedef void (*FrontierLinkHandler)(int netId, int size, void *src, void *work);
typedef u32 (*FrontierLinkSizeGetter)(void);
typedef void *(*FrontierLinkRecvBufGetter)(int netId, void *work, int size);

typedef struct FrontierLinkCommand {
    FrontierLinkHandler handler;
    FrontierLinkSizeGetter getSize;
    FrontierLinkRecvBufGetter getRecvBuf;
} FrontierLinkCommand;

typedef struct FrontierLinkWorkE {
    u8 unk000[0x10];
    u8 unk10_0 : 3;
    u8 flag : 1;
    u8 unk10_4 : 1;
    u8 unk10_5 : 3;
    u8 unk011[0x1];
    u8 unk12; // 0x012
    u8 unk013[0x1];
    u16 unk14; // 0x014
    u16 unk16; // 0x016
    u16 unk18; // 0x018
    u8 unk01A[0x14];
    u16 unk2E; // 0x02E
    u16 unk30; // 0x030
    u8 unk032[0xC];
    u8 linkData[0x1C]; // 0x03E
    u8 unk05A[0x7E4];
    u8 sendBuf[0x1C]; // 0x83E
    u8 unk85A[0x7A];
    u8 recvCount; // 0x8D4
    u8 unk8D5[0x3];
    u16 result; // 0x8D8
} FrontierLinkWorkE;

typedef struct FrontierLinkWorkF {
    SaveData *saveData; // 0x000
    u8 unk004[0x4];
    u16 sendBuf[20]; // 0x008
    u8 unk030[0x29];
    u8 partner59; // 0x059
    u8 unk05A[0x10];
    u8 slotA; // 0x06A
    u8 slotB; // 0x06B
    u8 unk06C[0x2];
    u8 partner6E; // 0x06E
    u8 recvCount; // 0x06F
    u8 unk070[0x1];
    u8 partner71;  // 0x071
    u16 partner72; // 0x072
    u16 partner74; // 0x074
    u16 speciesA;  // 0x076
    u16 speciesB;  // 0x078
    u8 unk07A[0x4];
    u16 itemA; // 0x07E
    u16 itemB; // 0x080
    u8 unk082[0x4];
    u16 partnerSpeciesA; // 0x086
    u16 partnerSpeciesB; // 0x088
    u8 unk08A[0x4];
    u16 partnerItemA; // 0x08E
    u16 partnerItemB; // 0x090
    u8 unk092[0xE];
    u8 type; // 0x0A0
} FrontierLinkWorkF;

void ov80_0222AEF8(int netId, int size, void *src, void *work);
void ov80_0222AF3C(int netId, int size, void *src, void *work);
void ov80_0222AF84(int netId, int size, void *src, void *work);
void ov80_0222AFEC(int netId, int size, void *src, void *work);
void ov80_0222B048(int netId, int size, void *src, void *work);
void ov80_0222B0B8(int netId, int size, void *src, void *work);
void *ov80_0222B0E8(int netId, void *work, int size);
void ov80_0222B140(int netId, int size, void *src, void *work);
void ov80_0222B1A4(int netId, int size, void *src, void *work);
void ov80_0222B24C(int netId, int size, void *src, void *work);
void ov80_0222B334(int netId, int size, void *src, void *work);
void ov80_0222B3D4(int netId, int size, void *src, void *work);
void ov80_0222B420(int netId, int size, void *src, void *work);
void ov80_0222B52C(int netId, int size, void *src, void *work);
void ov80_0222B628(int netId, int size, void *src, void *work);
void ov80_0222B690(int netId, int size, void *src, void *work);
void ov80_0222B740(int netId, int size, void *src, void *work);
void ov80_0222B860(int netId, int size, void *src, void *work);
void ov80_0222B8F8(int netId, int size, void *src, void *work);
void ov80_0222B940(int netId, int size, void *src, void *work);
void ov80_0222B9CC(int netId, int size, void *src, void *work);
void *ov80_0222BA5C(int netId, void *work, int size);
void ov80_0222BAB0(int netId, int size, void *src, void *work);
void ov80_0222BB18(int netId, int size, void *src, void *work);
void ov80_0222BBD0(int netId, int size, void *src, void *work);
void ov80_0222BC6C(int netId, int size, void *src, void *work);
void ov80_0222BCB8(int netId, int size, void *src, void *work);
void ov80_0222BD44(int netId, int size, void *src, void *work);
void *ov80_0222BDD4(int netId, void *work, int size);
void ov81_02241020(int netId, int size, void *src, void *work);
void ov81_022410C8(int netId, int size, void *src, void *work);
void ov81_0224113C(int netId, int size, void *src, void *work);
void ov81_02241238(int netId, int size, void *src, void *work);
void ov82_0223F764(int netId, int size, void *src, void *work);
void ov82_0223F7B4(int netId, int size, void *src, void *work);
void ov82_0223F814(int netId, int size, void *src, void *work);
void ov83_0224140C(int netId, int size, void *src, void *work);
void ov83_02241474(int netId, int size, void *src, void *work);
void ov83_022414DC(int netId, int size, void *src, void *work);
void ov83_02241510(int netId, int size, void *src, void *work);
void ov83_0224514C(int netId, int size, void *src, void *work);
void ov83_022451B8(int netId, int size, void *src, void *work);
void ov83_02245220(int netId, int size, void *src, void *work);
void ov83_02245254(int netId, int size, void *src, void *work);
void ov84_0223ED34(int netId, int size, void *src, void *work);
void ov84_0223EDA8(int netId, int size, void *src, void *work);
void ov84_0223EE08(int netId, int size, void *src, void *work);

static void sub_02096924(int netId, int size, void *src, void *work);
BOOL sub_02096998(FrontierLinkWorkE *work);
static void sub_020969C4(int netId, int size, void *src, void *work);
static void sub_020969F8(int netId, int size, void *src, void *work);
BOOL sub_02096A34(FrontierLinkWorkF *work);
static void sub_02096A7C(int netId, int size, void *src, void *work);
BOOL sub_02096AAC(FrontierLinkWorkF *work, u16 value);
static void sub_02096ACC(int netId, int size, void *src, void *work);
BOOL sub_02096AF4(FrontierLinkWorkF *work, u8 slotA, u8 slotB);
static void sub_02096BB8(int netId, int size, void *src, void *work);
BOOL sub_02096BF8(FrontierLinkWorkF *work, u16 value);
static void sub_02096C18(int netId, int size, void *src, void *work);
BOOL sub_02096C40(FrontierLinkWorkF *work, u16 value);
static void sub_02096C60(int netId, int size, void *src, void *work);

static const FrontierLinkCommand sFrontierLinkCommands[] = {
    { ov80_0222B140, sub_020342B8, NULL          },
    { ov80_0222B1A4, sub_020342B8, NULL          },
    { ov80_0222B24C, sub_020342B8, NULL          },
    { ov80_0222B334, sub_020342B8, NULL          },
    { ov80_0222B3D4, sub_020342B8, NULL          },
    { ov80_0222B420, sub_020342B8, NULL          },
    { ov80_0222B52C, sub_020342B8, NULL          },
    { ov81_02241020, sub_020342B8, NULL          },
    { ov81_022410C8, sub_020342B8, NULL          },
    { ov81_0224113C, sub_020342B8, NULL          },
    { ov81_02241238, sub_020342B8, NULL          },
    { ov80_0222AEF8, sub_020342B8, NULL          },
    { ov80_0222AF3C, sub_020342B8, NULL          },
    { ov80_0222AF84, sub_020342B8, NULL          },
    { ov80_0222AFEC, sub_020342B8, NULL          },
    { ov80_0222B048, sub_020342B8, NULL          },
    { ov80_0222B0B8, sub_020342B8, ov80_0222B0E8 },
    { ov82_0223F764, sub_020342B8, NULL          },
    { ov82_0223F7B4, sub_020342B8, NULL          },
    { ov82_0223F814, sub_020342B8, NULL          },
    { ov80_0222B628, sub_020342B8, NULL          },
    { ov80_0222B690, sub_020342B8, NULL          },
    { ov80_0222B740, sub_020342B8, NULL          },
    { ov80_0222B860, sub_020342B8, NULL          },
    { ov80_0222B8F8, sub_020342B8, NULL          },
    { ov80_0222B940, sub_020342B8, NULL          },
    { ov80_0222B9CC, sub_020342B8, ov80_0222BA5C },
    { ov83_0224140C, sub_020342B8, NULL          },
    { ov83_02241474, sub_020342B8, NULL          },
    { ov83_022414DC, sub_020342B8, NULL          },
    { ov83_02241510, sub_020342B8, NULL          },
    { ov83_0224514C, sub_020342B8, NULL          },
    { ov83_022451B8, sub_020342B8, NULL          },
    { ov83_02245220, sub_020342B8, NULL          },
    { ov83_02245254, sub_020342B8, NULL          },
    { sub_02096A7C,  sub_020342B8, NULL          },
    { sub_02096ACC,  sub_020342B8, NULL          },
    { sub_02096BB8,  sub_020342B8, NULL          },
    { sub_02096C18,  sub_020342B8, NULL          },
    { sub_02096C60,  sub_020342B8, NULL          },
    { sub_02096924,  sub_020342B8, NULL          },
    { sub_020969C4,  sub_020342B8, NULL          },
    { sub_020969F8,  sub_020342B8, NULL          },
    { ov80_0222BAB0, sub_020342B8, NULL          },
    { ov80_0222BB18, sub_020342B8, NULL          },
    { ov80_0222BBD0, sub_020342B8, NULL          },
    { ov80_0222BC6C, sub_020342B8, NULL          },
    { ov80_0222BCB8, sub_020342B8, NULL          },
    { ov80_0222BD44, sub_020342B8, ov80_0222BDD4 },
    { ov84_0223ED34, sub_020342B8, NULL          },
    { ov84_0223EDA8, sub_020342B8, NULL          },
    { ov84_0223EE08, sub_020342B8, NULL          },
};

void sub_02096910(void *data) {
    sub_0203410C(sFrontierLinkCommands, NELEMS(sFrontierLinkCommands), data);
}

static void sub_02096924(int netId, int size, void *src, void *data) {
    FrontierLinkWorkE *work = data;
    u16 *msg = src;
    u16 result = 0;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->unk12 = msg[0];
    work->unk16 = msg[1];
    work->unk18 = msg[2];
    work->unk14 = msg[3];
    work->unk10_5 = work->unk12 + 5;
    if (work->unk2E == work->unk16 || work->unk2E == work->unk18) {
        result += 1;
    }
    if (work->unk30 == work->unk16 || work->unk30 == work->unk18) {
        result += 2;
    }
    work->result = result;
}

BOOL sub_02096998(FrontierLinkWorkE *work) {
    MI_CpuCopy8(work->linkData, work->sendBuf, sizeof(work->sendBuf));
    if (sub_02037030(0x3F, work->sendBuf, sizeof(work->sendBuf)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static void sub_020969C4(int netId, int size, void *src, void *data) {
    FrontierLinkWorkE *work = data;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    if (sub_0203769C() == 0) {
        return;
    }
    MI_CpuCopy8(src, work->linkData, sizeof(work->linkData));
}

static void sub_020969F8(int netId, int size, void *src, void *data) {
    FrontierLinkWorkE *work = data;
    u16 *msg = src;

    work->result = 0;
    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    if (work->flag || msg[0] != 0) {
        work->result = 1;
    }
}

BOOL sub_02096A34(FrontierLinkWorkF *work) {
    int stat;
    FrontierSave *frontierSave;

    work->sendBuf[0] = work->type;
    stat = ov80_022385D8(work->type);
    frontierSave = Save_Frontier_GetStatic(work->saveData);
    work->sendBuf[1] = FrontierSave_GetStat(frontierSave, stat, sub_0205C268(stat));
    if (sub_02037030(0x39, work->sendBuf, sizeof(work->sendBuf)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static void sub_02096A7C(int netId, int size, void *src, void *data) {
    FrontierLinkWorkF *work = data;
    u16 *msg = src;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partner6E = msg[0];
    work->partner72 = msg[1];
}

BOOL sub_02096AAC(FrontierLinkWorkF *work, u16 value) {
    work->sendBuf[0] = value;
    if (sub_02037030(0x3A, work->sendBuf, sizeof(work->sendBuf)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static void sub_02096ACC(int netId, int size, void *src, void *data) {
    FrontierLinkWorkF *work = data;
    u16 *msg = src;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partner59 = msg[0];
}

BOOL sub_02096AF4(FrontierLinkWorkF *work, u8 slotA, u8 slotB) {
    Pokemon *mon;
    Party *party = SaveArray_Party_Get(work->saveData);

    work->slotA = slotA;
    work->slotB = slotB;
    if (slotA == 0xFF) {
        work->speciesA = SPECIES_NONE;
        work->itemA = ITEM_NONE;
        work->speciesB = SPECIES_NONE;
        work->itemB = ITEM_NONE;
    } else {
        mon = Party_GetMonByIndex(party, slotA);
        work->speciesA = GetMonData(mon, MON_DATA_SPECIES, NULL);
        work->itemA = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
        mon = Party_GetMonByIndex(party, slotB);
        work->speciesB = GetMonData(mon, MON_DATA_SPECIES, NULL);
        work->itemB = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    }
    work->sendBuf[0] = work->speciesA;
    work->sendBuf[1] = work->itemA;
    work->sendBuf[2] = work->speciesB;
    work->sendBuf[3] = work->itemB;
    if (sub_02037030(0x3B, work->sendBuf, sizeof(work->sendBuf)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static void sub_02096BB8(int netId, int size, void *src, void *data) {
    FrontierLinkWorkF *work = data;
    u16 *msg = src;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partnerSpeciesA = msg[0];
    work->partnerItemA = msg[1];
    work->partnerSpeciesB = msg[2];
    work->partnerItemB = msg[3];
}

BOOL sub_02096BF8(FrontierLinkWorkF *work, u16 value) {
    work->sendBuf[0] = value;
    if (sub_02037030(0x3C, work->sendBuf, sizeof(work->sendBuf)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static void sub_02096C18(int netId, int size, void *src, void *data) {
    FrontierLinkWorkF *work = data;
    u16 *msg = src;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partner74 = msg[0];
}

BOOL sub_02096C40(FrontierLinkWorkF *work, u16 value) {
    work->sendBuf[0] = value;
    if (sub_02037030(0x3D, work->sendBuf, sizeof(work->sendBuf)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static void sub_02096C60(int netId, int size, void *src, void *data) {
    FrontierLinkWorkF *work = data;
    u16 *msg = src;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    work->partner71 = msg[0];
}
