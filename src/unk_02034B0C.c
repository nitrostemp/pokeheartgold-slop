#include "global.h"

#include "heap.h"
#include "mail_message.h"
#include "player_data.h"
#include "save_link_ruleset.h"
#include "system.h"

// Comm server/client discovery layer on top of the wireless manager
// (unk_02032844). Ported from pokeplatinum's unk_02033200.c.

typedef struct WMGameInfo {
    u16 magicNumber;            // 0x00
    u8 ver;                     // 0x02
    u8 platform;                // 0x03
    u32 ggid;                   // 0x04
    u16 tgid;                   // 0x08
    u8 userGameInfoLength;      // 0x0A
    u8 gameNameCount_attribute; // 0x0B
    u16 parentMaxSize;          // 0x0C
    u16 childMaxSize;           // 0x0E
    u16 userGameInfo[56];       // 0x10
} WMGameInfo;                   // size: 0x80

typedef struct WMBssDesc {
    u16 length;            // 0x00
    u16 rssi;              // 0x02
    u8 bssid[6];           // 0x04
    u16 ssidLength;        // 0x0A
    u8 ssid[32];           // 0x0C
    u16 capaInfo;          // 0x2C
    u16 rateSet[2];        // 0x2E
    u16 beaconPeriod;      // 0x32
    u16 dtimPeriod;        // 0x34
    u16 channel;           // 0x36
    u16 cfpPeriod;         // 0x38
    u16 cfpMaxDuration;    // 0x3A
    u16 gameInfoLength;    // 0x3C
    u16 otherElementCount; // 0x3E
    WMGameInfo gameInfo;   // 0x40
} WMBssDesc;               // size: 0xC0

typedef struct UnkStruct_0203330C {
    u32 unk_00;         // 0x00
    u8 unk_04;          // 0x04
    u8 unk_05;          // 0x05
    u8 unk_06;          // 0x06
    u8 unk_07;          // 0x07
    MailMessage unk_08; // 0x08
    u8 unk_10[0x20];    // 0x10
    u8 unk_30[0x24];    // 0x30
    u8 unk_54;          // 0x54
} UnkStruct_0203330C;

typedef struct UnkStruct_02034168 {
    u32 unk_00;      // 0x00
    u8 unk_04;       // 0x04
    u8 unk_05;       // 0x05
    u8 unk_06[2];    // 0x06
    u8 unk_08[0x54]; // 0x08
} UnkStruct_02034168;

#define USER_GAME_INFO_SIZE 0x5C

typedef struct CommServerClient {
    u8 unk_00[0x54];                    // 0x000
    WMBssDesc unk_54;                   // 0x054
    WMBssDesc unk_114[16];              // 0x114
    u8 unk_D14[8][6];                   // 0xD14
    u16 unk_D44[16];                    // 0xD44
    void *unk_D64;                      // 0xD64
    MailMessage unk_D68;                // 0xD68
    int unk_D70;                        // 0xD70
    u8 unk_D74;                         // 0xD74
    u8 unk_D75;                         // 0xD75
    PlayerProfile *personalTrainerInfo; // 0xD78
    LinkBattleRuleset *unk_D7C;         // 0xD7C
    u32 unk_D80;                        // 0xD80
    u32 unk_D84;                        // 0xD84
    u16 *unk_D88;                       // 0xD88
    u16 unk_D8C;                        // 0xD8C
    u16 unk_D8E;                        // 0xD8E
    u8 unk_D90;                         // 0xD90
    u8 unk_D91;                         // 0xD91
    u8 unk_D92;                         // 0xD92
    u8 unk_D93;                         // 0xD93
    u8 unk_D94;                         // 0xD94
    u8 unk_D95_0 : 1;                   // 0xD95
    u8 unk_D95_1 : 1;
    u8 unk_D95_2 : 1;
    u8 unk_D95_3 : 1;
    u8 unk_D95_4 : 1;
    u8 unk_D95_5 : 1;
    u8 unk_D95_6 : 1;
    u8 : 1;
} CommServerClient; // size: 0xD98

typedef void (*WirelessManagerScanFunc)(WMBssDesc *);

u16 WM_GetNextTgid(void);
u16 WM_GetDispersionBeaconPeriod(void);
int WVR_StartUpAsync(int wram, void *callback, void *arg);
int WVR_TerminateAsync(void *callback, void *arg);
void OS_Terminate(void);

BOOL sub_02032B84(int connectionType, const u8 *macAddress, u16 channel);
BOOL sub_02032C1C(WirelessManagerScanFunc scanCallback, const u8 *macAddress, u16 channel);
BOOL sub_02032E24(void);
void sub_02033234(u32 ggid);
void sub_02033240(u16 *userGameInfo, u16 size);
u16 sub_02033250(void);
int sub_02033298(void);
int sub_020332AC(void);
BOOL sub_020332C0(void);
u16 sub_02033468(void);
BOOL sub_02033528(void *heap, BOOL isNotListening);
int sub_020335B4(void);
BOOL sub_02033668(int connectionType, u16 tgid, u16 channel, u16 maxEntry, u16 beaconPeriod, BOOL entryFlag);
BOOL sub_0203373C(int connectionType, WMBssDesc *bssDesc);
void sub_020337D0(void *recvFunction, int port);
void sub_02033858(void);
BOOL sub_020338D0(void);
u16 sub_020338F4(void);
BOOL sub_02033920(void);
BOOL sub_0203393C(void);
BOOL sub_02033958(void);
BOOL sub_02033990(void);
void sub_020339B4(void *buffer, int size, int ggid, int tgid);
BOOL sub_02033A44(void);
void sub_02033A68(void);
BOOL sub_02033AB8(void);
u8 sub_02033FC4(u16 commType);
BOOL sub_0203401C(int commType);
BOOL sub_020347CC(void);
void sub_020367A8(void);
void sub_02036904(void);
BOOL sub_02037474(void);
int sub_0203993C(void);
u8 sub_02039954(void);
void sub_020399DC(u32 errorCode);

void sub_02034B0C(PlayerProfile *profile, BOOL isNotListening);
BOOL sub_02034BE4(void);
static BOOL sub_02034BF8(const u8 *a, const u8 *b, int len);
static void sub_02034C20(WMBssDesc *bssDesc);
static void sub_02034C94(void);
static void sub_02034D60(void *arg, int result);
static void sub_02034D78(void *arg, int result);
void sub_02034D8C(void);
BOOL sub_02034DB8(void);
BOOL sub_02034DCC(void);
void sub_02034DE0(void);
static void sub_02034DF0(BOOL isNotListening);
void sub_02034E2C(void);
static void sub_02034E64(BOOL a0);
static void sub_02034E8C(void);
BOOL sub_02034EF0(BOOL a0, BOOL a1, BOOL a2);
BOOL sub_02034F64(BOOL a0, BOOL a1);
BOOL sub_02034FE8(void);
BOOL sub_0203507C(void);
void sub_020350A8(BOOL isClosed);
static void sub_020350D4(void);
int sub_0203511C(void);
int sub_02035150(int index);
BOOL sub_02035184(void);
void sub_02035198(void);
int sub_020351AC(int index);
void sub_020351DC(int index, PlayerProfile *profile);
BOOL sub_02035218(u16 index);
void sub_0203528C(void);
static void sub_020352D8(void);
static void sub_020353B8(void);
static void sub_0203540C(u16 a0);
void sub_020355C8(u16 a0);
static BOOL sub_020355DC(u16 aid);
static int sub_02035610(void);
BOOL sub_02035630(void);
BOOL sub_02035650(void);
BOOL sub_02035664(void);
BOOL sub_0203567C(void);
BOOL sub_0203569C(void);
void sub_020356C0(BOOL a0);
void sub_020356EC(BOOL a0);
u16 sub_02035724(u16 commType);
WMBssDesc *sub_02035754(int index);
PlayerProfile *sub_02035784(void);
PlayerProfile *sub_02035798(int index);
void sub_020357C4(u8 *bssid, int index);
BOOL sub_020357FC(void);
void sub_0203581C(void);
void sub_02035838(MailMessage *msg);
void sub_02035854(void *ruleset);
LinkBattleRuleset *sub_02035878(void);
void sub_0203588C(void);
BOOL sub_020358B0(void);
void sub_020358B8(void *data);
const void *sub_020358D0(int index);

static struct {
    u16 tgid;
    volatile int wirelessDriverStatus;
    CommServerClient *client;
} sCommServerClientData;

void sub_02034B0C(PlayerProfile *profile, BOOL isNotListening) {
    if (sCommServerClientData.client != NULL) {
        return;
    }

    sCommServerClientData.client = (CommServerClient *)Heap_Alloc(HEAP_ID_15, sizeof(CommServerClient));
    MI_CpuClear8(sCommServerClientData.client, sizeof(CommServerClient));

    sCommServerClientData.client->unk_D64 = Heap_Alloc(HEAP_ID_15, sub_020335B4());
    MI_CpuClear8(sCommServerClientData.client->unk_D64, sub_020335B4());

    sCommServerClientData.client->unk_D7C = Heap_Alloc(HEAP_ID_15, LinkBattleRuleset_sizeof());
    MI_CpuClear8(sCommServerClientData.client->unk_D7C, LinkBattleRuleset_sizeof());

    sCommServerClientData.client->unk_D84 = (u32)Heap_Alloc(HEAP_ID_15, 112 + 32);
    sCommServerClientData.client->unk_D88 = (u16 *)(32 - (sCommServerClientData.client->unk_D84 % 32) + sCommServerClientData.client->unk_D84);

    sCommServerClientData.client->unk_D80 = 0x333;
    sCommServerClientData.client->personalTrainerInfo = profile;

    MailMsg_Init(&sCommServerClientData.client->unk_D68);
    sub_02034DF0(isNotListening);
    sCommServerClientData.tgid = WM_GetNextTgid();
}

BOOL sub_02034BE4(void) {
    if (sCommServerClientData.client) {
        return TRUE;
    }
    return FALSE;
}

static BOOL sub_02034BF8(const u8 *a, const u8 *b, int len) {
    const u8 *v1 = a;
    const u8 *v2 = b;
    int i;

    for (i = 0; i < len; i++) {
        if (*v1 != *v2) {
            return FALSE;
        }
        v1++;
        v2++;
    }
    return TRUE;
}

static void sub_02034C20(WMBssDesc *bssDesc) {
    UnkStruct_0203330C *v1;
    int v2 = sub_0203993C();
    int v3 = sub_02039954();

    v1 = (UnkStruct_0203330C *)bssDesc->gameInfo.userGameInfo;

    if (v2 == 14) {
        (void)0;
    } else if (sub_0203401C(v1->unk_04) && sub_0203401C(v2)) {
        (void)0;
    } else if (v1->unk_54 && v1->unk_04 == 10) {
        return;
    } else if (v1->unk_04 != v2) {
        return;
    }

    if (v2 != 14 && v1->unk_05 != v3) {
        return;
    }

    MI_CpuCopy8(bssDesc, &sCommServerClientData.client->unk_54, sizeof(WMBssDesc));
    sCommServerClientData.client->unk_D95_6 = 1;
}

static void sub_02034C94(void) {
    WMBssDesc *v0 = &sCommServerClientData.client->unk_54;
    int v1;

    if (!sCommServerClientData.client->unk_D95_6) {
        return;
    }

    sCommServerClientData.client->unk_D95_6 = 0;

    for (v1 = 0; v1 < 16; ++v1) {
        if (sCommServerClientData.client->unk_D44[v1] == 0) {
            continue;
        }
        if (sub_02034BF8(sCommServerClientData.client->unk_114[v1].bssid, v0->bssid, 6)) {
            sCommServerClientData.client->unk_D44[v1] = 30 * 10;
            MI_CpuCopy8(v0, &sCommServerClientData.client->unk_114[v1], sizeof(WMBssDesc));
            return;
        }
    }

    for (v1 = 0; v1 < 16; ++v1) {
        if (sCommServerClientData.client->unk_D44[v1] == 0) {
            break;
        }
    }

    if (v1 >= 16) {
        return;
    }

    sCommServerClientData.client->unk_D44[v1] = 30 * 10;
    MI_CpuCopy8(v0, &sCommServerClientData.client->unk_114[v1], sizeof(WMBssDesc));
    sCommServerClientData.client->unk_D74 = 1;
}

static void sub_02034D60(void *arg, int result) {
    if (result != 0) {
        OS_Terminate();
    } else {
        (void)0;
    }
    sCommServerClientData.wirelessDriverStatus = 2;
}

static void sub_02034D78(void *arg, int result) {
    sCommServerClientData.wirelessDriverStatus = 0;
    Sys_ClearSleepDisableFlag(4);
}

void sub_02034D8C(void) {
    Sys_SetSleepDisableFlag(4);
    sCommServerClientData.wirelessDriverStatus = 1;

    if (1 != WVR_StartUpAsync(8, sub_02034D60, NULL)) {
        OS_Terminate();
    } else {
        (void)0;
    }
}

BOOL sub_02034DB8(void) {
    return sCommServerClientData.wirelessDriverStatus == 2;
}

BOOL sub_02034DCC(void) {
    return sCommServerClientData.wirelessDriverStatus != 0;
}

void sub_02034DE0(void) {
    WVR_TerminateAsync(sub_02034D78, NULL);
}

static void sub_02034DF0(BOOL isNotListening) {
    sCommServerClientData.client->unk_D70 = 0;
    u32 v0 = (u32)sCommServerClientData.client->unk_D64;

    v0 = 32 - (v0 % 32) + v0;
    (void)sub_02033528((void *)v0, isNotListening);

    sub_02033234(sCommServerClientData.client->unk_D80);
}

void sub_02034E2C(void) {
    int v0;

    for (v0 = 0; v0 < 16; ++v0) {
        sCommServerClientData.client->unk_D44[v0] = 0;
    }

    MI_CpuClear8(sCommServerClientData.client->unk_114, sizeof(WMBssDesc) * 16);
}

static void sub_02034E64(BOOL a0) {
    sCommServerClientData.client->unk_D95_3 = a0;
}

static void sub_02034E8C(void) {
    sCommServerClientData.client->unk_D74 = 0;
    sCommServerClientData.client->unk_D95_0 = 0;
    sCommServerClientData.client->unk_D95_2 = 0;
    sCommServerClientData.client->unk_D92 = 0;
    sCommServerClientData.client->unk_D95_4 = 0;
    sCommServerClientData.client->unk_D94 = 0;
    sCommServerClientData.client->unk_D93 = 0;
}

BOOL sub_02034EF0(BOOL a0, BOOL a1, BOOL a2) {
    sub_02034E8C();
    sub_02034E64(a1);
    sub_02033A68();

    if (!sCommServerClientData.client->unk_D93) {
        sub_020337D0(sub_02036904, 14);
        sCommServerClientData.client->unk_D93 = 1;
    }

    sCommServerClientData.client->unk_D95_5 = a2;

    if (sub_02033298() == 1) {
        if (sub_020332C0()) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL sub_02034F64(BOOL a0, BOOL a1) {
    sub_02034E8C();

    if (a1) {
        sub_02034E2C();
    }

    if (!sCommServerClientData.client->unk_D93) {
        sub_020337D0(sub_020367A8, 14);
        sCommServerClientData.client->unk_D93 = 1;
    }

    if (sub_02033298() == 1) {
        const u8 v0[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

        if (sub_02032C1C(sub_02034C20, v0, 0)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL sub_02034FE8(void) {
    if (!sCommServerClientData.client) {
        return TRUE;
    }

    switch (sCommServerClientData.client->unk_D94) {
    case 0:
        if (sub_02033990()) {
            sub_02032E24();
            sCommServerClientData.client->unk_D94 = 1;
            break;
        }
        if (sub_0203393C()) {
            (void)0;
        } else {
            sub_02033858();
            sCommServerClientData.client->unk_D94 = 2;
        }
        break;
    case 1:
        if (!sub_0203393C()) {
            sub_02033858();
            sCommServerClientData.client->unk_D94 = 2;
        }
        break;
    case 2:
        if (sub_02033920()) {
            return TRUE;
        }
        if (sub_02033958()) {
            sCommServerClientData.client->unk_D94 = 1;
        }
        break;
    }
    return FALSE;
}

BOOL sub_0203507C(void) {
    if (sCommServerClientData.client) {
        if (sCommServerClientData.client->unk_D92 == 0) {
            sCommServerClientData.client->unk_D92 = 1;
            sub_02033858();
            return TRUE;
        }
    }
    return FALSE;
}

void sub_020350A8(BOOL isClosed) {
    if (!sCommServerClientData.client) {
        return;
    }

    if (isClosed) {
        sCommServerClientData.client->unk_D92 = 2;
    } else {
        sCommServerClientData.client->unk_D92 = 0;
        sub_02034DF0(TRUE);
    }
}

static void sub_020350D4(void) {
    Heap_Free(sCommServerClientData.client->unk_D7C);
    Heap_Free(sCommServerClientData.client->unk_D64);
    Heap_Free((void *)sCommServerClientData.client->unk_D84);
    Heap_Free(sCommServerClientData.client);
    sCommServerClientData.client = NULL;
}

int sub_0203511C(void) {
    if (!sub_02037474()) {
        return 0;
    }

    int v1 = 0;

    for (int i = 0; i < 16; ++i) {
        if (sCommServerClientData.client->unk_D44[i] != 0) {
            v1++;
        }
    }
    return v1;
}

int sub_02035150(int index) {
    int i, v1 = 0;

    for (i = 0; i < 16; i++) {
        if (sCommServerClientData.client->unk_D44[i] != 0) {
            if (v1 == index) {
                return i;
            }
            v1++;
        }
    }

    GF_ASSERT(FALSE);
    return 0;
}

BOOL sub_02035184(void) {
    return sCommServerClientData.client->unk_D74;
}

void sub_02035198(void) {
    sCommServerClientData.client->unk_D74 = 0;
}

int sub_020351AC(int index) {
    if (sCommServerClientData.client->unk_D44[index] != 0) {
        UnkStruct_0203330C *v1 = (UnkStruct_0203330C *)sCommServerClientData.client->unk_114[index].gameInfo.userGameInfo;

        if (v1->unk_06 == 0) {
            return 1;
        }
        return v1->unk_06;
    }
    return 0;
}

void sub_020351DC(int index, PlayerProfile *profile) {
    int i, v1 = 0;

    for (i = 0; i < 16; ++i) {
        if (sCommServerClientData.client->unk_D44[i] != 0) {
            if (index == v1) {
                PlayerProfile_Copy(sub_02035798(i), profile);
                return;
            }
            v1++;
        }
    }
}

BOOL sub_02035218(u16 index) {
    if (sub_02033298() == 2) {
        (void)sub_02032E24();
        return FALSE;
    }

    if (sub_02033298() == 1) {
        int v0 = sub_0203993C();
        sCommServerClientData.client->unk_D90 = sCommServerClientData.client->unk_114[index].channel;

        if (sub_0203401C(v0)) {
            sub_02032B84(1, sCommServerClientData.client->unk_114[index].bssid, 0);
        } else {
            sub_0203373C(1, &sCommServerClientData.client->unk_114[index]);
        }
        return TRUE;
    }
    return FALSE;
}

void sub_0203528C(void) {
    sub_02034C94();

    for (int i = 0; i < 16; i++) {
        if (sCommServerClientData.client->unk_D44[i] == 0) {
            continue;
        }
        if (sCommServerClientData.client->unk_D44[i] > 0) {
            sCommServerClientData.client->unk_D44[i]--;
            if (sCommServerClientData.client->unk_D44[i] == 0) {
                sCommServerClientData.client->unk_D74 = 1;
            }
        }
    }
}

static void sub_020352D8(void) {
    int v4 = sub_0203993C();
    PlayerProfile *v1 = sub_02035784();

    if (v4 != 15) {
        UnkStruct_0203330C *v2 = (UnkStruct_0203330C *)sCommServerClientData.client->unk_D88;

        GF_ASSERT(32 >= (int)LinkBattleRuleset_sizeof());
        GF_ASSERT(32 == PlayerProfile_sizeof());

        MI_CpuCopy8(v1, v2->unk_10, PlayerProfile_sizeof());
        MI_CpuCopy8(sCommServerClientData.client->unk_D7C, v2->unk_30, LinkBattleRuleset_sizeof());

        v2->unk_00 = PlayerProfile_GetTrainerID(v1);
        v2->unk_04 = sub_0203993C();
        v2->unk_05 = sub_02039954();

        MI_CpuCopy8(&sCommServerClientData.client->unk_D68, &v2->unk_08, sizeof(MailMessage));

        v2->unk_54 = sub_02033AB8();
    } else {
        UnkStruct_02034168 *v3 = (UnkStruct_02034168 *)sCommServerClientData.client->unk_D88;

        v3->unk_00 = PlayerProfile_GetTrainerID(v1);
        v3->unk_04 = sub_0203993C();
        v3->unk_05 = sub_02039954();

        MI_CpuCopy8(sCommServerClientData.client->unk_00, v3->unk_08, 0x54);
    }

    DC_FlushRange(sCommServerClientData.client->unk_D88, USER_GAME_INFO_SIZE);
    sub_02033240(sCommServerClientData.client->unk_D88, USER_GAME_INFO_SIZE);
}

static void sub_020353B8(void) {
    UnkStruct_0203330C *v0 = (UnkStruct_0203330C *)sCommServerClientData.client->unk_D88;

    if (sub_02035610() != v0->unk_06) {
        v0->unk_06 = sub_02035610();
        DC_FlushRange(sCommServerClientData.client->unk_D88, USER_GAME_INFO_SIZE);
        sub_02033240(sCommServerClientData.client->unk_D88, USER_GAME_INFO_SIZE);
        sub_020339B4(sCommServerClientData.client->unk_D88, USER_GAME_INFO_SIZE, sCommServerClientData.client->unk_D80, sCommServerClientData.tgid);
    }
}

static void sub_0203540C(u16 a0) {
    int v0 = sub_02033298();
    int v1 = sub_020347CC();

    sub_020353B8();

    if (sub_020338F4() == 0 && !sub_0203567C()) {
        if (sCommServerClientData.client->unk_D95_2) {
            sCommServerClientData.client->unk_D95_0 = 1;
        }
    }

    if (sCommServerClientData.client->unk_D8E == 0xffff) {
        sCommServerClientData.client->unk_D8E = a0;
    }

    if (sCommServerClientData.client->unk_D95_1) {
        if (sCommServerClientData.client->unk_D8E > a0) {
            sCommServerClientData.client->unk_D95_0 = 1;
        }
        if (v1) {
            sCommServerClientData.client->unk_D95_0 = 1;
        }
    }

    if (25 == sub_020332AC()) {
        sub_020399DC(0);
    }

    switch (v0) {
    case 0:
        if (sCommServerClientData.client->unk_D92 == 1) {
            sub_020350D4();
            return;
        }
        if (sCommServerClientData.client->unk_D92 == 2) {
            sCommServerClientData.client->unk_D92 = 3;
            return;
        }
        break;
    case 1:
        if (sCommServerClientData.client->unk_D92 == 1) {
            if (sub_020338D0()) {
                return;
            }
        }
        if (sCommServerClientData.client->unk_D92 == 2) {
            if (sub_020338D0()) {
                return;
            }
        }
        break;
    case 8:
    case 9:
        if (sCommServerClientData.client) {
            sCommServerClientData.client->unk_D95_0 = 1;
        }
        break;
    case 7: {
        u16 v2;

        v2 = sub_02033468();

        if (sCommServerClientData.client->unk_D91 == 0) {
            sCommServerClientData.client->unk_D8C = v2;
            sCommServerClientData.client->unk_D91 = 5;
        } else {
            sCommServerClientData.client->unk_D91--;
        }

        v2 = sCommServerClientData.client->unk_D8C;

        if (sCommServerClientData.client->unk_D95_3) {
            sCommServerClientData.tgid = WM_GetNextTgid();
        }

        sub_020352D8();
        (void)sub_02033668(0, sCommServerClientData.tgid, v2, sub_02033FC4(sub_0203993C()), sub_02035724(sub_0203993C()), sCommServerClientData.client->unk_D95_5);
        sCommServerClientData.client->unk_D90 = v2;
    } break;
    default:
        break;
    }
}

void sub_020355C8(u16 a0) {
    if (sCommServerClientData.client) {
        sub_0203540C(a0);
    }
}

static BOOL sub_020355DC(u16 aid) {
    if (!sCommServerClientData.client) {
        return FALSE;
    }

    if (sub_02033298() != 4) {
        return FALSE;
    }

    {
        u16 v0 = sub_02033250();

        if (v0 & (1 << aid)) {
            return TRUE;
        }
    }
    return FALSE;
}

static int sub_02035610(void) {
    int v0 = 0, v1;

    for (v1 = 0; v1 < 7 + 1; v1++) {
        if (sub_020355DC(v1)) {
            v0++;
        }
    }
    return v0;
}

BOOL sub_02035630(void) {
    if (sCommServerClientData.client && (sCommServerClientData.client->unk_D92 == 3)) {
        return TRUE;
    }
    return FALSE;
}

BOOL sub_02035650(void) {
    return sCommServerClientData.client != NULL;
}

BOOL sub_02035664(void) {
    if (sCommServerClientData.client) {
        return sub_02033920();
    }
    return TRUE;
}

BOOL sub_0203567C(void) {
    if (sCommServerClientData.client) {
        return sub_02033250() & 0xfffe;
    }
    return FALSE;
}

BOOL sub_0203569C(void) {
    if (sCommServerClientData.client) {
        if (sCommServerClientData.client->unk_D95_0) {
            return TRUE;
        }
    }
    return FALSE;
}

void sub_020356C0(BOOL a0) {
    if (sCommServerClientData.client) {
        sCommServerClientData.client->unk_D95_2 = a0;
    }
}

void sub_020356EC(BOOL a0) {
    if (sCommServerClientData.client) {
        sCommServerClientData.client->unk_D95_1 = a0;
        sCommServerClientData.client->unk_D8E = 0xffff;
    }
}

u16 sub_02035724(u16 commType) {
    u16 v0 = WM_GetDispersionBeaconPeriod();

    GF_ASSERT(commType < 41);

    if (10 == commType) {
        return v0 / 4;
    }
    if (9 == commType || 13 == commType) {
        return v0 / 4;
    }
    return v0;
}

WMBssDesc *sub_02035754(int index) {
    if (sCommServerClientData.client && (sCommServerClientData.client->unk_D44[index] != 0)) {
        return &sCommServerClientData.client->unk_114[index];
    }
    return NULL;
}

PlayerProfile *sub_02035784(void) {
    return sCommServerClientData.client->personalTrainerInfo;
}

PlayerProfile *sub_02035798(int index) {
    if (sCommServerClientData.client->unk_D44[index] == 0) {
        return NULL;
    }

    UnkStruct_0203330C *v1 = (UnkStruct_0203330C *)sCommServerClientData.client->unk_114[index].gameInfo.userGameInfo;
    PlayerProfile *v0 = (PlayerProfile *)&v1->unk_10[0];

    return v0;
}

void sub_020357C4(u8 *bssid, int index) {
    if (sCommServerClientData.client) {
        GF_ASSERT(index < 7 + 1);
        MI_CpuCopy8(bssid, sCommServerClientData.client->unk_D14[index], 6);
    }
}

BOOL sub_020357FC(void) {
    if (sCommServerClientData.client) {
        return sCommServerClientData.client->unk_D95_4;
    }
    return FALSE;
}

void sub_0203581C(void) {
    if (sCommServerClientData.client) {
        sCommServerClientData.client->unk_D95_4 = 1;
    }
}

void sub_02035838(MailMessage *msg) {
    MI_CpuCopy8(msg, &sCommServerClientData.client->unk_D68, sizeof(MailMessage));
}

void sub_02035854(void *ruleset) {
    MI_CpuCopy8(ruleset, sCommServerClientData.client->unk_D7C, LinkBattleRuleset_sizeof());
}

LinkBattleRuleset *sub_02035878(void) {
    return sCommServerClientData.client->unk_D7C;
}

void sub_0203588C(void) {
    sub_020352D8();
    sub_020339B4(sCommServerClientData.client->unk_D88, USER_GAME_INFO_SIZE, sCommServerClientData.client->unk_D80, sCommServerClientData.tgid);
}

BOOL sub_020358B0(void) {
    return sub_02033A44();
}

void sub_020358B8(void *data) {
    MI_CpuCopy8(data, sCommServerClientData.client->unk_00, 0x54);
    sub_0203588C();
}

const void *sub_020358D0(int index) {
    if (sCommServerClientData.client && sCommServerClientData.client->unk_D44[index] != 0) {
        UnkStruct_02034168 *v0 = (UnkStruct_02034168 *)sCommServerClientData.client->unk_114[index].gameInfo.userGameInfo;
        return v0->unk_08;
    }
    return NULL;
}
