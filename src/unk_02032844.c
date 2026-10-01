#include <string.h>

#include "global.h"

// Wrapper around the NitroSDK wireless manager (wm). Ported from pokeplatinum's
// wireless_manager.c; the SDK types below only describe the fields used here.

typedef enum WMErrCode {
    WM_ERRCODE_SUCCESS = 0,
    WM_ERRCODE_FAILED = 1,
    WM_ERRCODE_OPERATING = 2,
    WM_ERRCODE_ILLEGAL_STATE = 3,
    WM_ERRCODE_FIFO_ERROR = 8,
    WM_ERRCODE_TIMEOUT = 9,
    WM_ERRCODE_NO_ENTRY = 11,
    WM_ERRCODE_OVER_MAX_ENTRY = 12,
    WM_ERRCODE_INVALID_POLLBITMAP = 13,
    WM_ERRCODE_SEND_FAILED = 15,
    WM_ERRCODE_MAX = 20,
} WMErrCode;

// Extension of the NitroSDK WMErrCode enum
enum ExtendedWMErrCode {
    WM_ERRCODE_DISCONNECTED = WM_ERRCODE_MAX,
    WM_ERRCODE_NO_SERVER,
    WM_ERRCODE_DISCONNECTED_SERVER,
    WM_ERRCODE_NO_CHANNEL,
    WM_ERRCODE_FATAL,
    WM_ERRCODE_GF_MAX,
};

enum WMStateCode {
    WM_STATECODE_PARENT_START = 0,
    WM_STATECODE_BEACON_SENT = 2,
    WM_STATECODE_SCAN_START = 3,
    WM_STATECODE_PARENT_NOT_FOUND = 4,
    WM_STATECODE_PARENT_FOUND = 5,
    WM_STATECODE_CONNECT_START = 6,
    WM_STATECODE_CONNECTED = 7,
    WM_STATECODE_BEACON_LOST = 8,
    WM_STATECODE_DISCONNECTED = 9,
    WM_STATECODE_MP_START = 10,
    WM_STATECODE_MPEND_IND = 11,
    WM_STATECODE_MP_IND = 12,
    WM_STATECODE_MPACK_IND = 13,
    WM_STATECODE_PORT_RECV = 21,
    WM_STATECODE_PORT_INIT = 25,
    WM_STATECODE_DISCONNECTED_FROM_MYSELF = 26,
};

#define WM_ATTR_FLAG_ENTRY 1
#define WM_ATTR_FLAG_MB    2

#define WM_AUTHMODE_OPEN_SYSTEM 0
#define WM_PRIORITY_NORMAL      2

enum WirelessManagerState {
    WIRELESS_STATE_STOP = 0,
    WIRELESS_STATE_IDLE,
    WIRELESS_STATE_SCAN,
    WIRELESS_STATE_BUSY,
    WIRELESS_STATE_CONNECTED,
    WIRELESS_STATE_TRANSMIT_DATA,
    WIRELESS_STATE_TRANSMIT_KEY,
    WIRELESS_STATE_CHECK_CHANNEL,
    WIRELESS_STATE_BAD_CONNECTION,
    WIRELESS_STATE_ERROR,
    WIRELESS_STATE_FATAL_ERROR,
};

enum WirelessConnectionType {
    WIRELESS_CONNECTION_MP_SERVER = 0,
    WIRELESS_CONNECTION_MP_CLIENT,
    WIRELESS_CONNECTION_TRANSMIT_KEY_SERVER,
    WIRELESS_CONNECTION_TRANSMIT_KEY_CLIENT,
    WIRELESS_CONNECTION_TRANSMIT_DATA_SERVER,
    WIRELESS_CONNECTION_TRANSMIT_DATA_CLIENT,
};

#define COMM_ERROR_RESET_SAVEPOINT 1
#define COMM_ERROR_RESET_TITLE     5

typedef void (*WMCallbackFunc)(void *arg);

typedef struct WMCallback {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
} WMCallback;

typedef struct WMIndCallback {
    u16 apiid;
    u16 errcode;
    u16 state;
    u16 reason;
} WMIndCallback;

typedef struct WMStartParentCallback {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;        // 0x08
    u8 macAddress[6]; // 0x0A
    u16 aid;          // 0x10
    u16 reason;       // 0x12
    u8 ssid[24];      // 0x14
    u16 parentSize;   // 0x2C
    u16 childSize;    // 0x2E
} WMStartParentCallback;

typedef struct WMStartMPCallback {
    u16 apiid;
    u16 errcode;
    u16 state; // 0x04
} WMStartMPCallback;

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

typedef struct WMStartScanCallback {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;           // 0x08
    u8 macAddress[6];    // 0x0A
    u16 channel;         // 0x10
    u16 linkLevel;       // 0x12
    u16 ssidLength;      // 0x14
    u16 ssid[16];        // 0x16
    u16 gameInfoLength;  // 0x36
    WMGameInfo gameInfo; // 0x38
} WMStartScanCallback;

typedef struct WMStartConnectCallback {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;  // 0x08
    u16 aid;    // 0x0A
    u16 reason; // 0x0C
} WMStartConnectCallback;

typedef struct WMPortSendCallback {
    u16 apiid;
    u16 errcode;
    u8 unk04[0x1C];
    void *arg; // 0x20
} WMPortSendCallback;

typedef struct WMPortRecvCallback {
    u16 apiid;
    u16 errcode;
    u16 state;     // 0x04
    u16 port;      // 0x06
    void *recvBuf; // 0x08
    u16 *data;     // 0x0C
    u16 length;    // 0x10
    u16 aid;       // 0x12
} WMPortRecvCallback;

typedef struct WMMeasureChannelCallback {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 channel;      // 0x08
    u16 ccaBusyRatio; // 0x0A
} WMMeasureChannelCallback;

typedef struct WMParentParam {
    u16 *userGameInfo;      // 0x00
    u16 userGameInfoLength; // 0x04
    u16 padding;            // 0x06
    u32 ggid;               // 0x08
    u16 tgid;               // 0x0C
    u16 entryFlag;          // 0x0E
    u16 maxEntry;           // 0x10
    u16 multiBootFlag;      // 0x12
    u16 KS_Flag;            // 0x14
    u16 CS_Flag;            // 0x16
    u16 beaconPeriod;       // 0x18
    u16 rsv1[4];            // 0x1A
    u16 rsv2[8];            // 0x22
    u16 channel;            // 0x32
    u16 parentMaxSize;      // 0x34
    u16 childMaxSize;       // 0x36
    u16 rsv[4];             // 0x38
} WMParentParam;            // size: 0x40

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

typedef struct WMScanParam {
    WMBssDesc *scanBuf; // 0x00
    u16 channel;        // 0x04
    u16 maxChannelTime; // 0x06
    u8 bssid[6];        // 0x08
    u16 rsv[9];         // 0x0E
} WMScanParam;          // size: 0x20

typedef struct WMStatus {
    u8 unk000[0x198];
    u32 wep_flag; // 0x198
} WMStatus;

typedef struct UnkStruct_0203330C {
    u8 unk_00[4];
    u8 unk_04;
} UnkStruct_0203330C;

typedef void (*WirelessManagerScanFunc)(WMBssDesc *);
typedef void (*WirelessManagerRecvFunc)(u16, u16 *, u16);
typedef void (*WirelessManagerSendFunc)(BOOL);
typedef void (*WirelessManagerGGIDScanFunc)(u32, int);
typedef void (*WirelessManagerConnectFunc)(int);

typedef struct WirelessManager {
    WMParentParam parentParam ALIGN(32);           // 0x0000
    u8 nitroManagerBuffer[3840] ALIGN(32);         // 0x0040
    u8 sendBuffer[224] ALIGN(32);                  // 0x0F40
    u8 recvBuffer[512] ALIGN(32);                  // 0x1020
    WMBssDesc bssDesc ALIGN(32);                   // 0x1220
    WMScanParam scanParam ALIGN(32);               // 0x12E0
    WirelessManagerScanFunc scanCallback;          // 0x1300
    s32 sendBufferSize;                            // 0x1304
    s32 recvBufferSize;                            // 0x1308
    u16 channel;                                   // 0x130C
    u16 autoConnect;                               // 0x130E
    int state;                                     // 0x1310
    int connectionType;                            // 0x1314
    WirelessManagerRecvFunc recvFunc;              // 0x1318
    void *unused_131C;                             // 0x131C
    WirelessManagerGGIDScanFunc ggidScanCallback;  // 0x1320
    WirelessManagerConnectFunc disconnectCallback; // 0x1324
    WirelessManagerConnectFunc connectCallback;    // 0x1328
    u16 aid;                                       // 0x132C
    u16 connectedBitmap;                           // 0x132E
    int errorCode;                                 // 0x1330
    u8 numConnectionsMax;                          // 0x1334
    u8 pauseConnectionClient;                      // 0x1335
    u32 rand;                                      // 0x1338
    u16 measureChannel;                            // 0x133C
    u16 measureChannelBusyRatio;                   // 0x133E
    u16 leastUsedChannelBitmap;                    // 0x1340
    u8 pauseConnection;                            // 0x1342
    u8 pauseConnectSystem;                         // 0x1343
    u8 setEntry;                                   // 0x1344
    u8 sentBeaconCount;                            // 0x1345
} WirelessManager;

WMErrCode WM_SetParentParameter(WMCallbackFunc callback, const WMParentParam *pparaBuf);
WMErrCode WM_StartParent(WMCallbackFunc callback);
WMErrCode WM_EndParent(WMCallbackFunc callback);
WMErrCode WM_StartMP(WMCallbackFunc callback, u16 *recvBuf, u16 recvBufSize, u16 *sendBuf, u16 sendBufSize, u16 mpFreq);
WMErrCode WM_EndMP(WMCallbackFunc callback);
u16 WM_GetAllowedChannel(void);
u16 WM_GetDispersionScanPeriod(void);
WMErrCode WM_StartScan(WMCallbackFunc callback, const WMScanParam *param);
WMErrCode WM_EndScan(WMCallbackFunc callback);
WMErrCode WM_StartConnectEx(WMCallbackFunc callback, const WMBssDesc *pInfo, const u8 *ssid, BOOL powerSave, u16 authMode);
WMErrCode WM_Disconnect(WMCallbackFunc callback, u16 aid);
WMErrCode WM_Reset(WMCallbackFunc callback);
WMErrCode WM_End(WMCallbackFunc callback);
WMErrCode WM_SetMPDataToPortEx(WMCallbackFunc callback, void *arg, const u16 *sendData, u16 sendDataSize, u16 destBitmap, u16 port, u16 prio);
WMErrCode WM_SetGameInfo(WMCallbackFunc callback, const u16 *userGameInfo, u16 userGameInfoSize, u32 ggid, u16 tgid, u8 attr);
WMErrCode WM_SetLifeTime(WMCallbackFunc callback, u16 tableNumber, u16 camLifeTime, u16 frameLifeTime, u16 mpLifeTime);
WMErrCode WM_MeasureChannel(WMCallbackFunc callback, u16 ccaMode, u16 edThreshold, u16 channel, u16 measureTime);
WMErrCode WM_SetEntry(WMCallbackFunc callback, BOOL enabled);
WMErrCode WM_Initialize(void *wmSysBuf, WMCallbackFunc callback, u16 dmaNo);
WMErrCode WM_InitializeForListening(void *wmSysBuf, WMCallbackFunc callback, u16 dmaNo, BOOL indicateWlChannel);
WMErrCode WM_SetIndCallback(WMCallbackFunc callback);
WMErrCode WM_SetPortCallback(u16 port, WMCallbackFunc callback, void *arg);
void *WMi_GetStatusAddress(void);
int WVR_TerminateAsync(void *callback, void *arg);

BOOL sub_020340C4(int cmd);
BOOL sub_02039918(void);
u8 sub_0203993C(void);
BOOL sub_02039AD8(int error);

static void sub_02032844(int state);
static void sub_02032858(int errorCode);
static BOOL sub_02032874(void);
static void sub_020328A4(void *arg);
static BOOL sub_020328C8(void);
static void sub_02032934(void *arg);
static BOOL sub_02032A40(void);
static void sub_02032AB0(void *arg);
static BOOL sub_02032B0C(void);
static void sub_02032B30(void *arg);
static BOOL sub_02032B50(void);
static void sub_02032B6C(void *arg);
BOOL sub_02032B84(int connectionType, const u8 *macAddress, u16 channel);
BOOL sub_02032C1C(WirelessManagerScanFunc scanCallback, const u8 *macAddress, u16 channel);
static BOOL sub_02032C84(void);
static void sub_02032D4C(void *arg);
BOOL sub_02032E24(void);
static BOOL sub_02032E48(void);
static void sub_02032E64(void *arg);
static BOOL sub_02032E9C(void);
static void sub_02032F0C(void *arg);
static BOOL sub_02032FCC(void);
static void sub_0203301C(void *arg);
static BOOL sub_02033080(void);
static void sub_020330A4(void *arg);
static BOOL sub_020330C8(void);
static void sub_020330F0(void *arg);
static BOOL sub_02033108(void);
static void sub_0203312C(void *arg);
static BOOL sub_0203314C(void *message, u16 size, int port, WirelessManagerSendFunc sendCallback);
static void sub_020331A4(void *arg);
static void sub_020331CC(void *arg);
static void sub_02033214(void *arg);
void sub_02033234(u32 ggid);
void sub_02033240(u16 *userGameInfo, u16 size);
u16 sub_02033250(void);
static u16 sub_02033264(void);
int sub_02033298(void);
int sub_020332AC(void);
BOOL sub_020332C0(void);
static u16 sub_0203335C(u16 channel);
static void sub_020333D8(void *arg);
static WMErrCode sub_02033454(WMCallbackFunc callback, u16 channel);
u16 sub_02033468(void);
static s16 sub_02033494(u16 bitmap);
BOOL sub_02033528(void *heap, BOOL isNotListening);
int sub_020335B4(void);
static void sub_020335BC(void *arg);
static BOOL sub_020335D4(BOOL isNotListening);
static void sub_02033620(void *arg);
static void sub_02033664(void *arg);
BOOL sub_02033668(int connectionType, u16 tgid, u16 channel, u16 maxEntry, u16 beaconPeriod, BOOL entryFlag);
BOOL sub_0203373C(int connectionType, WMBssDesc *bssDesc);
void sub_020337D0(WirelessManagerRecvFunc recvFunction, int port);
BOOL sub_02033800(void *message, u16 size, int port, WirelessManagerSendFunc callback);
static void sub_02033830(void);
void sub_02033858(void);
BOOL sub_020338D0(void);
u16 sub_020338F4(void);
void sub_02033908(int numConnectionsMax);
BOOL sub_02033920(void);
BOOL sub_0203393C(void);
BOOL sub_02033958(void);
BOOL sub_02033974(void);
BOOL sub_02033990(void);
void sub_020339B4(void *buffer, int size, int ggid, int tgid);
static void sub_020339F0(void *arg);
BOOL sub_02033A0C(BOOL enable);
BOOL sub_02033A44(void);
void sub_02033A68(void);
void sub_02033A7C(WirelessManagerGGIDScanFunc callback);
void sub_02033A90(WirelessManagerConnectFunc callback);
void sub_02033AA4(BOOL pause);
BOOL sub_02033AB8(void);
void sub_02033ACC(BOOL pause);

static struct {
    void (*debugPrint)(const char *, ...);
    WirelessManager *manager;
} sWirelessManagerData;

static void sub_02032844(int state) {
    sWirelessManagerData.manager->state = state;
}

static void sub_02032858(int errorCode) {
    if (sWirelessManagerData.manager->state == WIRELESS_STATE_ERROR || sWirelessManagerData.manager->state == WIRELESS_STATE_FATAL_ERROR) {
        return;
    }
    sWirelessManagerData.manager->errorCode = errorCode;
}

static BOOL sub_02032874(void) {
    sub_02032844(WIRELESS_STATE_BUSY);
    WMErrCode errorCode = WM_SetParentParameter(sub_020328A4, &sWirelessManagerData.manager->parentParam);
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        sub_02032844(WIRELESS_STATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

static void sub_020328A4(void *arg) {
    WMCallback *callback = (WMCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
        sub_02032844(WIRELESS_STATE_ERROR);
        return;
    }
    if (!sub_020328C8()) {
        sub_02032844(WIRELESS_STATE_ERROR);
    }
}

static BOOL sub_020328C8(void) {
    if ((sWirelessManagerData.manager->state == WIRELESS_STATE_CONNECTED) || (sWirelessManagerData.manager->state == WIRELESS_STATE_TRANSMIT_KEY) || (sWirelessManagerData.manager->state == WIRELESS_STATE_TRANSMIT_DATA)) {
        return TRUE;
    }

    WMStatus *status = (WMStatus *)WMi_GetStatusAddress();

    DC_InvalidateRange(&status->wep_flag, sizeof(status->wep_flag));
    status->wep_flag = 0;
    DC_FlushRange(&status->wep_flag, sizeof(status->wep_flag));

    WMErrCode errorCode = WM_StartParent(sub_02032934);
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        return FALSE;
    }

    sWirelessManagerData.manager->aid = 0;
    sWirelessManagerData.manager->connectedBitmap = 1;
    return TRUE;
}

static void sub_02032934(void *arg) {
    WMStartParentCallback *callback = (WMStartParentCallback *)arg;
    const u16 connected = (u16)(1 << callback->aid);

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
        sub_02032844(WIRELESS_STATE_ERROR);
        return;
    }

    switch (callback->state) {
    case WM_STATECODE_BEACON_SENT:
        sWirelessManagerData.manager->sentBeaconCount++;
        break;
    case WM_STATECODE_CONNECTED:
        if (sWirelessManagerData.manager->pauseConnectSystem == TRUE || sWirelessManagerData.manager->pauseConnection == TRUE || sub_02033264() >= sWirelessManagerData.manager->numConnectionsMax || callback->ssid[0] != sub_0203993C() || 0 != memcmp("DP", &callback->ssid[1], sizeof("DP"))) {
            WMErrCode disconnectCode;

            disconnectCode = WM_Disconnect(NULL, callback->aid);
            if (disconnectCode != WM_ERRCODE_OPERATING) {
                sub_02032858(disconnectCode);
                sub_02032844(WIRELESS_STATE_ERROR);
            }
            break;
        }
        sWirelessManagerData.manager->connectedBitmap |= connected;
        if (sWirelessManagerData.manager->connectCallback) {
            sWirelessManagerData.manager->connectCallback(callback->aid);
        }
        break;
    case WM_STATECODE_DISCONNECTED:
        sWirelessManagerData.manager->connectedBitmap &= ~connected;
        if (sWirelessManagerData.manager->disconnectCallback) {
            sWirelessManagerData.manager->disconnectCallback(callback->aid);
        }
        break;
    case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
        break;
    case WM_STATECODE_PARENT_START:
        if (!sub_02032A40()) {
            sub_02032844(WIRELESS_STATE_ERROR);
        }
        break;
    default:
        break;
    }
}

static BOOL sub_02032A40(void) {
    if ((sWirelessManagerData.manager->state == WIRELESS_STATE_CONNECTED) || (sWirelessManagerData.manager->state == WIRELESS_STATE_TRANSMIT_KEY) || (sWirelessManagerData.manager->state == WIRELESS_STATE_TRANSMIT_DATA)) {
        return TRUE;
    }

    sub_02032844(WIRELESS_STATE_CONNECTED);

    WMErrCode errorCode = WM_StartMP(sub_02032AB0, (u16 *)sWirelessManagerData.manager->recvBuffer, (u16)sWirelessManagerData.manager->recvBufferSize, (u16 *)sWirelessManagerData.manager->sendBuffer, (u16)sWirelessManagerData.manager->sendBufferSize, 1);
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        return FALSE;
    }
    return TRUE;
}

static void sub_02032AB0(void *arg) {
    WMStartMPCallback *callback = (WMStartMPCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
        sub_02032844(WIRELESS_STATE_ERROR);
        return;
    }

    switch (callback->state) {
    case WM_STATECODE_MP_START:
        if (sWirelessManagerData.manager->connectionType == WIRELESS_CONNECTION_TRANSMIT_KEY_SERVER) {
            if (sWirelessManagerData.manager->state != WIRELESS_STATE_CONNECTED && sWirelessManagerData.manager->state == WIRELESS_STATE_TRANSMIT_KEY) {
                return;
            }
        }
        sub_02032844(WIRELESS_STATE_CONNECTED);
        break;
    case WM_STATECODE_MPEND_IND:
        break;
    case WM_STATECODE_MP_IND:
    case WM_STATECODE_MPACK_IND:
    default:
        break;
    }
}

static BOOL sub_02032B0C(void) {
    sub_02032844(WIRELESS_STATE_BUSY);
    WMErrCode errorCode = WM_EndMP(sub_02032B30);
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        return FALSE;
    }
    return TRUE;
}

static void sub_02032B30(void *arg) {
    WMCallback *callback = (WMCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
        sub_02033830();
        return;
    }
    if (!sub_02032B50()) {
        sub_02033830();
        return;
    }
}

static BOOL sub_02032B50(void) {
    WMErrCode errorCode = WM_EndParent(sub_02032B6C);
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        return FALSE;
    }
    return TRUE;
}

static void sub_02032B6C(void *arg) {
    WMCallback *callback = (WMCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
        return;
    }
    sub_02032844(WIRELESS_STATE_IDLE);
}

BOOL sub_02032B84(int connectionType, const u8 *macAddress, u16 channel) {
    sWirelessManagerData.manager->recvBufferSize = 0x200;
    sWirelessManagerData.manager->sendBufferSize = 0x40;

    sub_02032844(WIRELESS_STATE_SCAN);

    sWirelessManagerData.manager->bssDesc.channel = 1;
    *(u16 *)(&sWirelessManagerData.manager->scanParam.bssid[4]) = *(u16 *)(macAddress + 4);
    *(u16 *)(&sWirelessManagerData.manager->scanParam.bssid[2]) = *(u16 *)(macAddress + 2);
    *(u16 *)(&sWirelessManagerData.manager->scanParam.bssid[0]) = *(u16 *)(macAddress + 0);

    sWirelessManagerData.manager->connectionType = connectionType;

    sWirelessManagerData.manager->scanCallback = NULL;
    sWirelessManagerData.manager->channel = channel;
    sWirelessManagerData.manager->scanParam.channel = 0;
    sWirelessManagerData.manager->autoConnect = TRUE;

    if (!sub_02032C84()) {
        sub_02032844(WIRELESS_STATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

BOOL sub_02032C1C(WirelessManagerScanFunc scanCallback, const u8 *macAddress, u16 channel) {
    sub_02032844(WIRELESS_STATE_SCAN);

    sWirelessManagerData.manager->scanCallback = scanCallback;
    sWirelessManagerData.manager->channel = channel;
    sWirelessManagerData.manager->scanParam.channel = 0;
    sWirelessManagerData.manager->autoConnect = FALSE;

    *(u16 *)(&sWirelessManagerData.manager->scanParam.bssid[4]) = *(u16 *)(macAddress + 4);
    *(u16 *)(&sWirelessManagerData.manager->scanParam.bssid[2]) = *(u16 *)(macAddress + 2);
    *(u16 *)(&sWirelessManagerData.manager->scanParam.bssid[0]) = *(u16 *)(macAddress);

    if (!sub_02032C84()) {
        sub_02032844(WIRELESS_STATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

static BOOL sub_02032C84(void) {
    u16 channel = WM_GetAllowedChannel();

    if (channel == 0x8000) {
        sub_02032858(WM_ERRCODE_ILLEGAL_STATE);
        sub_02039AD8(COMM_ERROR_RESET_SAVEPOINT);
        return FALSE;
    }
    if (channel == 0) {
        sub_02032858(WM_ERRCODE_DISCONNECTED_SERVER);
        sub_02039AD8(COMM_ERROR_RESET_SAVEPOINT);
        return FALSE;
    }

    if (sWirelessManagerData.manager->channel == 0) {
        while (TRUE) {
            sWirelessManagerData.manager->scanParam.channel++;
            if (sWirelessManagerData.manager->scanParam.channel > 16) {
                sWirelessManagerData.manager->scanParam.channel = 1;
            }
            if (channel & (0x1 << (sWirelessManagerData.manager->scanParam.channel - 1))) {
                break;
            }
        }
    } else {
        sWirelessManagerData.manager->scanParam.channel = (u16)sWirelessManagerData.manager->channel;
    }

    sWirelessManagerData.manager->scanParam.maxChannelTime = WM_GetDispersionScanPeriod() / 3;
    sWirelessManagerData.manager->scanParam.scanBuf = &sWirelessManagerData.manager->bssDesc;

    WMErrCode errorCode = WM_StartScan(sub_02032D4C, &sWirelessManagerData.manager->scanParam);
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        return FALSE;
    }
    return TRUE;
}

static void sub_02032D4C(void *arg) {
    WMStartScanCallback *callback = (WMStartScanCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
        sub_02032844(WIRELESS_STATE_ERROR);
        return;
    }

    if (sWirelessManagerData.manager->state != WIRELESS_STATE_SCAN) {
        sWirelessManagerData.manager->autoConnect = FALSE;
        if (!sub_02032E48()) {
            sub_02032844(WIRELESS_STATE_ERROR);
        }
        return;
    }

    switch (callback->state) {
    case WM_STATECODE_SCAN_START:
        return;
    case WM_STATECODE_PARENT_NOT_FOUND:
        break;
    case WM_STATECODE_PARENT_FOUND:
        DC_InvalidateRange(&sWirelessManagerData.manager->bssDesc, sizeof(WMBssDesc));

        if ((sWirelessManagerData.manager->ggidScanCallback) && (callback->gameInfoLength >= 8)) {
            UnkStruct_0203330C *v1 = (UnkStruct_0203330C *)callback->gameInfo.userGameInfo;

            sWirelessManagerData.manager->ggidScanCallback(callback->gameInfo.ggid, v1->unk_04);
        }

        if ((callback->gameInfoLength < 8) || (callback->gameInfo.ggid != sWirelessManagerData.manager->parentParam.ggid)) {
            break;
        }

        if ((callback->gameInfo.gameNameCount_attribute & (WM_ATTR_FLAG_ENTRY | WM_ATTR_FLAG_MB)) != WM_ATTR_FLAG_ENTRY) {
            break;
        }

        if (sWirelessManagerData.manager->scanCallback) {
            sWirelessManagerData.manager->scanCallback(&sWirelessManagerData.manager->bssDesc);
        }

        if (sWirelessManagerData.manager->autoConnect) {
            if (!sub_02032E48()) {
                sub_02032844(WIRELESS_STATE_ERROR);
            }
            return;
        }
        break;
    }

    if (!sub_02032C84()) {
        sub_02032844(WIRELESS_STATE_ERROR);
    }
}

BOOL sub_02032E24(void) {
    if (sWirelessManagerData.manager->state != WIRELESS_STATE_SCAN) {
        return FALSE;
    }
    sub_02032844(WIRELESS_STATE_BUSY);
    return TRUE;
}

static BOOL sub_02032E48(void) {
    WMErrCode errorCode = WM_EndScan(sub_02032E64);
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        return FALSE;
    }
    return TRUE;
}

static void sub_02032E64(void *arg) {
    WMCallback *callback = (WMCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
        return;
    }

    sub_02032844(WIRELESS_STATE_IDLE);

    if (!sWirelessManagerData.manager->autoConnect) {
        return;
    }
    if (!sub_02032E9C()) {
        sub_02032844(WIRELESS_STATE_ERROR);
    }
}

static BOOL sub_02032E9C(void) {
    u8 ssid[32];

    if ((sWirelessManagerData.manager->state == WIRELESS_STATE_CONNECTED) || (sWirelessManagerData.manager->state == WIRELESS_STATE_TRANSMIT_KEY) || (sWirelessManagerData.manager->state == WIRELESS_STATE_TRANSMIT_DATA)) {
        return TRUE;
    }

    sub_02032844(WIRELESS_STATE_BUSY);
    MI_CpuCopy8("DP", &ssid[1], sizeof("DP"));

    ssid[0] = sub_0203993C();
    WMErrCode errorCode = WM_StartConnectEx(sub_02032F0C, &sWirelessManagerData.manager->bssDesc, ssid, 1, WM_AUTHMODE_OPEN_SYSTEM);
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        return FALSE;
    }
    return TRUE;
}

static void sub_02032F0C(void *arg) {
    WMStartConnectCallback *callback = (WMStartConnectCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);

        if (callback->errcode == WM_ERRCODE_OVER_MAX_ENTRY) {
            sub_02032844(WIRELESS_STATE_ERROR);
            return;
        } else if (callback->errcode == WM_ERRCODE_NO_ENTRY) {
            sub_02032844(WIRELESS_STATE_ERROR);
            return;
        } else if (callback->errcode == WM_ERRCODE_FAILED) {
            if (sub_02039918()) {
                sub_02032844(WIRELESS_STATE_ERROR);
            } else {
                sub_02032844(WIRELESS_STATE_BAD_CONNECTION);
            }
            return;
        } else {
            sub_02032844(WIRELESS_STATE_ERROR);
        }
        return;
    }

    if (callback->state == WM_STATECODE_BEACON_LOST) {
        return;
    }

    if (callback->state == WM_STATECODE_CONNECTED) {
        if (sWirelessManagerData.manager->pauseConnectionClient) {
            sub_02032858(WM_ERRCODE_DISCONNECTED);
            sub_02032844(WIRELESS_STATE_ERROR);
            return;
        } else {
            sub_02032844(WIRELESS_STATE_CONNECTED);
            if (!sub_02032FCC()) {
                sub_02032844(WIRELESS_STATE_BUSY);
                return;
            }
            sWirelessManagerData.manager->aid = callback->aid;
            return;
        }
    } else if (callback->state == WM_STATECODE_CONNECT_START) {
        return;
    } else if (callback->state == WM_STATECODE_DISCONNECTED) {
        sub_02032858(WM_ERRCODE_DISCONNECTED);
        sub_02032844(WIRELESS_STATE_ERROR);
        return;
    } else if (callback->state == WM_STATECODE_DISCONNECTED_FROM_MYSELF) {
        return;
    }

    sub_02032844(WIRELESS_STATE_ERROR);
}

static BOOL sub_02032FCC(void) {
    WMErrCode errorCode = WM_StartMP(sub_0203301C, (u16 *)sWirelessManagerData.manager->recvBuffer, (u16)sWirelessManagerData.manager->recvBufferSize, (u16 *)sWirelessManagerData.manager->sendBuffer, (u16)sWirelessManagerData.manager->sendBufferSize, 1);
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        return FALSE;
    }
    return TRUE;
}

static void sub_0203301C(void *arg) {
    WMStartMPCallback *callback = (WMStartMPCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        if (callback->errcode == WM_ERRCODE_SEND_FAILED) {
            return;
        } else if (callback->errcode == WM_ERRCODE_TIMEOUT) {
            return;
        } else if (callback->errcode == WM_ERRCODE_INVALID_POLLBITMAP) {
            return;
        }
        sub_02032858(callback->errcode);
        sub_02032844(WIRELESS_STATE_ERROR);
        return;
    }

    switch (callback->state) {
    case WM_STATECODE_MP_START:
        if (sWirelessManagerData.manager->connectionType == WIRELESS_CONNECTION_TRANSMIT_KEY_CLIENT) {
            if (sWirelessManagerData.manager->state == WIRELESS_STATE_TRANSMIT_KEY) {
                return;
            }
        }
        sub_02032844(WIRELESS_STATE_CONNECTED);
        break;
    case WM_STATECODE_MP_IND:
        break;
    case WM_STATECODE_MPACK_IND:
        break;
    case WM_STATECODE_MPEND_IND:
    default:
        break;
    }
}

static BOOL sub_02033080(void) {
    sub_02032844(WIRELESS_STATE_BUSY);
    WMErrCode errorCode = WM_EndMP(sub_020330A4);
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        return FALSE;
    }
    return TRUE;
}

static void sub_020330A4(void *arg) {
    WMCallback *callback = (WMCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
        sub_02033858();
        return;
    }
    if (!sub_020330C8()) {
        sub_02032844(WIRELESS_STATE_ERROR);
    }
}

static BOOL sub_020330C8(void) {
    sub_02032844(WIRELESS_STATE_BUSY);
    WMErrCode errorCode = WM_Disconnect(sub_020330F0, 0);
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        sub_02033830();
        return FALSE;
    }
    return TRUE;
}

static void sub_020330F0(void *arg) {
    WMCallback *callback = (WMCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
        return;
    }
    sub_02032844(WIRELESS_STATE_IDLE);
}

static BOOL sub_02033108(void) {
    sub_02032844(WIRELESS_STATE_BUSY);
    WMErrCode errorCode = WM_Reset(sub_0203312C);
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        return FALSE;
    }
    return TRUE;
}

static void sub_0203312C(void *arg) {
    WMCallback *callback = (WMCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032844(WIRELESS_STATE_ERROR);
        sub_02032858(callback->errcode);
        return;
    }
    sub_02032844(WIRELESS_STATE_IDLE);
}

static BOOL sub_0203314C(void *message, u16 size, int port, WirelessManagerSendFunc sendCallback) {
    DC_FlushRange(sWirelessManagerData.manager->sendBuffer, (u32)sWirelessManagerData.manager->sendBufferSize);
    WMErrCode errorCode = WM_SetMPDataToPortEx(sub_020331A4, (void *)sendCallback, message, size, 0xFFFF, port, WM_PRIORITY_NORMAL);
    return errorCode == WM_ERRCODE_OPERATING;
}

static void sub_020331A4(void *arg) {
    WMPortSendCallback *callback = (WMPortSendCallback *)arg;

    if ((callback->errcode != WM_ERRCODE_SUCCESS) && (callback->errcode != WM_ERRCODE_SEND_FAILED)) {
        sub_02032858(callback->errcode);
        return;
    }
    if (callback->arg != NULL) {
        WirelessManagerSendFunc sendFunc = (WirelessManagerSendFunc)callback->arg;

        (*sendFunc)(callback->errcode == WM_ERRCODE_SUCCESS);
    }
}

static void sub_020331CC(void *arg) {
    WMPortRecvCallback *callback = (WMPortRecvCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
    } else if (sWirelessManagerData.manager->recvFunc != NULL) {
        if (callback->state == WM_STATECODE_PORT_INIT) {
            ;
        } else if (callback->state == WM_STATECODE_PORT_RECV) {
            (*sWirelessManagerData.manager->recvFunc)(callback->aid, callback->data, callback->length);
        } else if (callback->state == WM_STATECODE_DISCONNECTED) {
            (*sWirelessManagerData.manager->recvFunc)(callback->aid, NULL, 0);
        }
    }
}

static void sub_02033214(void *arg) {
    WMCallback *callback = (WMCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032844(WIRELESS_STATE_FATAL_ERROR);
        return;
    }
    WVR_TerminateAsync(NULL, NULL);
    sub_02032844(WIRELESS_STATE_STOP);
}

void sub_02033234(u32 ggid) {
    sWirelessManagerData.manager->parentParam.ggid = ggid;
}

void sub_02033240(u16 *userGameInfo, u16 size) {
    sWirelessManagerData.manager->parentParam.userGameInfo = userGameInfo;
    sWirelessManagerData.manager->parentParam.userGameInfoLength = size;
}

u16 sub_02033250(void) {
    return sWirelessManagerData.manager->connectedBitmap;
}

static u16 sub_02033264(void) {
    int cnt = 0, i;
    u16 connected = sWirelessManagerData.manager->connectedBitmap;

    for (i = 0; i < 16; i++) {
        if (connected & 0x1) {
            cnt++;
        }
        connected = connected >> 1;
    }
    return cnt;
}

int sub_02033298(void) {
    return sWirelessManagerData.manager->state;
}

int sub_020332AC(void) {
    return sWirelessManagerData.manager->errorCode;
}

BOOL sub_020332C0(void) {
    u8 macAddress[6];

    OS_GetMacAddress(macAddress);

    sWirelessManagerData.manager->rand = (u32)(OS_GetVBlankCount() + *(u16 *)&macAddress[0] + *(u16 *)&macAddress[2] + *(u16 *)&macAddress[4]);
    sWirelessManagerData.manager->rand = sWirelessManagerData.manager->rand * 69069UL + 12345;
    sWirelessManagerData.manager->measureChannel = 0;
    sWirelessManagerData.manager->measureChannelBusyRatio = 100 + 1;

    sub_02032844(WIRELESS_STATE_BUSY);

    u16 errorCode = sub_0203335C(1);

    if (errorCode == WM_ERRCODE_FATAL) {
        sub_02032858(WM_ERRCODE_FATAL);
        sub_02032844(WIRELESS_STATE_ERROR);
        sub_02039AD8(COMM_ERROR_RESET_SAVEPOINT);
        return FALSE;
    }
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        sub_02032844(WIRELESS_STATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

static u16 sub_0203335C(u16 channel) {
    u16 allowedChannel = WM_GetAllowedChannel();

    if (allowedChannel == 0x8000) {
        sub_02032858(WM_ERRCODE_ILLEGAL_STATE);
        sub_02032844(WIRELESS_STATE_ERROR);
        sub_02039AD8(COMM_ERROR_RESET_SAVEPOINT);
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    if (allowedChannel == 0) {
        sub_02032858(WM_ERRCODE_DISCONNECTED_SERVER);
        sub_02032844(WIRELESS_STATE_ERROR);
        sub_02039AD8(COMM_ERROR_RESET_SAVEPOINT);
        return WM_ERRCODE_FATAL;
    }

    while (((1 << (channel - 1)) & allowedChannel) == 0) {
        channel++;
        if (channel > 16) {
            return WM_ERRCODE_FATAL;
        }
    }

    u16 errorCode = sub_02033454(sub_020333D8, channel);
    return errorCode;
}

static void sub_020333D8(void *arg) {
    WMMeasureChannelCallback *measureChannelCallback = (WMMeasureChannelCallback *)arg;

    if (measureChannelCallback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(measureChannelCallback->errcode);
        sub_02032844(WIRELESS_STATE_ERROR);
        sub_02039AD8(COMM_ERROR_RESET_SAVEPOINT);
        return;
    }

    u16 channel = measureChannelCallback->channel;

    if (sWirelessManagerData.manager->measureChannelBusyRatio > measureChannelCallback->ccaBusyRatio) {
        sWirelessManagerData.manager->measureChannelBusyRatio = measureChannelCallback->ccaBusyRatio;
        sWirelessManagerData.manager->leastUsedChannelBitmap = (u16)(1 << (channel - 1));
    } else if (sWirelessManagerData.manager->measureChannelBusyRatio == measureChannelCallback->ccaBusyRatio) {
        sWirelessManagerData.manager->leastUsedChannelBitmap |= 1 << (channel - 1);
    }

    u16 errorCode = sub_0203335C(++channel);

    if (errorCode == WM_ERRCODE_FATAL) {
        sub_02032844(WIRELESS_STATE_CHECK_CHANNEL);
        return;
    }
    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032844(WIRELESS_STATE_ERROR);
        return;
    }
}

static WMErrCode sub_02033454(WMCallbackFunc callback, u16 channel) {
    return WM_MeasureChannel(callback, 3, 17, channel, 30);
}

u16 sub_02033468(void) {
    sub_02032844(WIRELESS_STATE_IDLE);
    sWirelessManagerData.manager->measureChannel = (u16)sub_02033494(sWirelessManagerData.manager->leastUsedChannelBitmap);
    return sWirelessManagerData.manager->measureChannel;
}

static s16 sub_02033494(u16 bitmap) {
    s16 i;
    s16 channel = 0;
    u16 cnt = 0;
    u16 randChannel;

    for (i = 0; i < 16; i++) {
        if (bitmap & (1 << i)) {
            channel = (s16)(i + 1);
            cnt++;
        }
    }

    if (cnt <= 1) {
        return channel;
    }

    randChannel = (u16)((((sWirelessManagerData.manager->rand = sWirelessManagerData.manager->rand * 69069UL + 12345) & 0xFF) * cnt) / 0x100);
    channel = 1;

    for (i = 0; i < 16; i++) {
        if (bitmap & 1) {
            if (randChannel == 0) {
                return (s16)(i + 1);
            }
            randChannel--;
        }
        bitmap >>= 1;
    }
    return 0;
}

BOOL sub_02033528(void *heap, BOOL isNotListening) {
    u32 heapAddress = (u32)heap;

    if (heapAddress % 32) {
        heapAddress += 32 - (heapAddress % 32);
    }

    sWirelessManagerData.manager = (WirelessManager *)heapAddress;
    sWirelessManagerData.manager->recvBufferSize = 0;
    sWirelessManagerData.manager->sendBufferSize = 0;
    sWirelessManagerData.manager->recvFunc = NULL;
    sWirelessManagerData.manager->aid = 0;
    sWirelessManagerData.manager->connectedBitmap = 1;
    sWirelessManagerData.manager->errorCode = WM_ERRCODE_SUCCESS;
    sWirelessManagerData.manager->state = WIRELESS_STATE_STOP;
    sWirelessManagerData.manager->parentParam.userGameInfo = NULL;
    sWirelessManagerData.manager->parentParam.userGameInfoLength = 0;
    sWirelessManagerData.manager->unused_131C = NULL;
    sWirelessManagerData.manager->numConnectionsMax = 7 + 1;
    sWirelessManagerData.manager->pauseConnectionClient = FALSE;
    sWirelessManagerData.manager->pauseConnection = 0;

    if (!sub_020335D4(isNotListening)) {
        return FALSE;
    }
    return TRUE;
}

int sub_020335B4(void) {
    return sizeof(WirelessManager) + 32;
}

static void sub_020335BC(void *arg) {
    WMIndCallback *callback = (WMIndCallback *)arg;

    if (callback->errcode == WM_ERRCODE_FIFO_ERROR) {
        sub_02032844(WIRELESS_STATE_ERROR);
        sub_02032858(WM_ERRCODE_GF_MAX);
    }
}

static BOOL sub_020335D4(BOOL isNotListening) {
    WMErrCode errorCode;

    sub_02032844(WIRELESS_STATE_BUSY);

    if (isNotListening == 1) {
        errorCode = WM_Initialize(&sWirelessManagerData.manager->nitroManagerBuffer, sub_02033620, 2);
    } else {
        errorCode = WM_InitializeForListening(&sWirelessManagerData.manager->nitroManagerBuffer, sub_02033620, 2, 0);
    }

    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        sub_02032844(WIRELESS_STATE_FATAL_ERROR);
        return FALSE;
    }
    return TRUE;
}

static void sub_02033620(void *arg) {
    WMCallback *callback = (WMCallback *)arg;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
        sub_02032844(WIRELESS_STATE_FATAL_ERROR);
        sub_02039AD8(COMM_ERROR_RESET_TITLE);
        return;
    }

    WMErrCode errorCode = WM_SetIndCallback(sub_020335BC);

    if (errorCode != WM_ERRCODE_SUCCESS) {
        sub_02032858(errorCode);
        sub_02032844(WIRELESS_STATE_FATAL_ERROR);
        sub_02039AD8(COMM_ERROR_RESET_TITLE);
        return;
    }
    sub_02032844(WIRELESS_STATE_IDLE);
}

static void sub_02033664(void *arg) {
}

BOOL sub_02033668(int connectionType, u16 tgid, u16 channel, u16 maxEntry, u16 beaconPeriod, BOOL entryFlag) {
    if (sub_020340C4(sub_0203993C())) {
        WM_SetLifeTime(sub_02033664, 0xFFFF, 100, 5, 100);
    }

    sWirelessManagerData.manager->recvBufferSize = 0x1C0;
    sWirelessManagerData.manager->sendBufferSize = 0xE0;

    sWirelessManagerData.manager->connectionType = connectionType;
    sub_02032844(WIRELESS_STATE_BUSY);

    sWirelessManagerData.manager->parentParam.tgid = tgid;
    sWirelessManagerData.manager->parentParam.channel = channel;
    sWirelessManagerData.manager->parentParam.beaconPeriod = beaconPeriod;

    switch (connectionType) {
    case WIRELESS_CONNECTION_MP_SERVER:
        sWirelessManagerData.manager->parentParam.parentMaxSize = 192;
        if (maxEntry >= 5) {
            sWirelessManagerData.manager->parentParam.childMaxSize = 12;
        } else {
            sWirelessManagerData.manager->parentParam.childMaxSize = 38;
        }
        break;
    case WIRELESS_CONNECTION_TRANSMIT_DATA_SERVER:
        sWirelessManagerData.manager->parentParam.parentMaxSize = 12 * (1 + 7) + 4;
        sWirelessManagerData.manager->parentParam.childMaxSize = 12;
        break;
    }

    sWirelessManagerData.manager->parentParam.maxEntry = maxEntry;
    sWirelessManagerData.manager->parentParam.CS_Flag = 0;
    sWirelessManagerData.manager->parentParam.multiBootFlag = 0;
    sWirelessManagerData.manager->parentParam.entryFlag = entryFlag;
    sWirelessManagerData.manager->parentParam.KS_Flag = (u16)((connectionType == WIRELESS_CONNECTION_TRANSMIT_KEY_SERVER) ? 1 : 0);

    switch (connectionType) {
    case WIRELESS_CONNECTION_MP_SERVER:
    case WIRELESS_CONNECTION_TRANSMIT_KEY_SERVER:
    case WIRELESS_CONNECTION_TRANSMIT_DATA_SERVER:
        return sub_02032874();
    default:
        break;
    }
    return FALSE;
}

BOOL sub_0203373C(int connectionType, WMBssDesc *bssDesc) {
    if (sub_020340C4(sub_0203993C())) {
        WM_SetLifeTime(sub_02033664, 0xFFFF, 100, 5, 100);
    }

    sWirelessManagerData.manager->recvBufferSize = 0x200;
    sWirelessManagerData.manager->sendBufferSize = 0x40;

    sWirelessManagerData.manager->connectionType = connectionType;
    sub_02032844(WIRELESS_STATE_BUSY);

    switch (connectionType) {
    case WIRELESS_CONNECTION_MP_CLIENT:
    case WIRELESS_CONNECTION_TRANSMIT_KEY_CLIENT:
    case WIRELESS_CONNECTION_TRANSMIT_DATA_CLIENT:
        MI_CpuCopy8(bssDesc, &sWirelessManagerData.manager->bssDesc, sizeof(sWirelessManagerData.manager->bssDesc));
        DC_FlushRange(&sWirelessManagerData.manager->bssDesc, sizeof(sWirelessManagerData.manager->bssDesc));
        DC_WaitWriteBufferEmpty();
        return sub_02032E9C();
    default:
        break;
    }
    return FALSE;
}

void sub_020337D0(WirelessManagerRecvFunc recvFunction, int port) {
    sWirelessManagerData.manager->recvFunc = recvFunction;

    if (WM_SetPortCallback(port, sub_020331CC, NULL) != WM_ERRCODE_SUCCESS) {
        sub_02032844(WIRELESS_STATE_ERROR);
        while (TRUE) {
            ;
        }
    }
}

BOOL sub_02033800(void *message, u16 size, int port, WirelessManagerSendFunc callback) {
    if ((sub_020338F4() == 0) && !(0xFE & sub_02033250())) {
        return FALSE;
    }
    return sub_0203314C(message, size, port, callback);
}

static void sub_02033830(void) {
    if (WIRELESS_STATE_SCAN == sWirelessManagerData.manager->state) {
        while (TRUE) {
        }
    }
    if (!sub_02033108()) {
        sub_02032844(WIRELESS_STATE_FATAL_ERROR);
    }
}

void sub_02033858(void) {
    if (sWirelessManagerData.manager->state == WIRELESS_STATE_IDLE) {
        return;
    }

    if ((sWirelessManagerData.manager->state != WIRELESS_STATE_TRANSMIT_KEY) && (sWirelessManagerData.manager->state != WIRELESS_STATE_TRANSMIT_DATA) && (sWirelessManagerData.manager->state != WIRELESS_STATE_CONNECTED)) {
        sub_02032844(WIRELESS_STATE_BUSY);
        sub_02033830();
        return;
    }

    sub_02032844(WIRELESS_STATE_BUSY);

    switch (sWirelessManagerData.manager->connectionType) {
    case WIRELESS_CONNECTION_TRANSMIT_KEY_CLIENT:
        break;
    case WIRELESS_CONNECTION_TRANSMIT_DATA_CLIENT:
    case WIRELESS_CONNECTION_MP_CLIENT:
        if (!sub_02033080()) {
            sub_02033830();
        }
        break;
    case WIRELESS_CONNECTION_TRANSMIT_KEY_SERVER:
        break;
    case WIRELESS_CONNECTION_TRANSMIT_DATA_SERVER:
    case WIRELESS_CONNECTION_MP_SERVER:
        if (!sub_02032B0C()) {
            sub_02033830();
        }
    }
}

BOOL sub_020338D0(void) {
    int errorCode;

    sub_02032844(WIRELESS_STATE_BUSY);
    errorCode = WM_End(sub_02033214);

    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032844(WIRELESS_STATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

u16 sub_020338F4(void) {
    return sWirelessManagerData.manager->aid;
}

void sub_02033908(int numConnectionsMax) {
    if (sWirelessManagerData.manager) {
        sWirelessManagerData.manager->numConnectionsMax = numConnectionsMax;
    }
}

BOOL sub_02033920(void) {
    return sWirelessManagerData.manager->state == WIRELESS_STATE_IDLE;
}

BOOL sub_0203393C(void) {
    return sWirelessManagerData.manager->state == WIRELESS_STATE_BUSY;
}

BOOL sub_02033958(void) {
    return sWirelessManagerData.manager->state == WIRELESS_STATE_ERROR;
}

BOOL sub_02033974(void) {
    return sWirelessManagerData.manager->state == WIRELESS_STATE_FATAL_ERROR;
}

BOOL sub_02033990(void) {
    if (sWirelessManagerData.manager) {
        return sWirelessManagerData.manager->state == WIRELESS_STATE_SCAN;
    }
    return FALSE;
}

void sub_020339B4(void *buffer, int size, int ggid, int tgid) {
    if (sWirelessManagerData.manager->state == WIRELESS_STATE_CONNECTED) {
        WM_SetGameInfo(NULL, buffer, size, ggid, tgid, WM_ATTR_FLAG_ENTRY);
    }
}

static void sub_020339F0(void *arg) {
    WMCallback *callback = arg;

    if (callback->errcode == WM_ERRCODE_SUCCESS) {
        sWirelessManagerData.manager->setEntry = TRUE;
    }
}

BOOL sub_02033A0C(BOOL enable) {
    sWirelessManagerData.manager->setEntry = 0;

    if (sWirelessManagerData.manager->state == WIRELESS_STATE_CONNECTED) {
        if (WM_ERRCODE_OPERATING == WM_SetEntry(sub_020339F0, enable)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL sub_02033A44(void) {
    if (sWirelessManagerData.manager) {
        return sWirelessManagerData.manager->sentBeaconCount >= 6;
    }
    return FALSE;
}

void sub_02033A68(void) {
    sWirelessManagerData.manager->sentBeaconCount = 0;
}

void sub_02033A7C(WirelessManagerGGIDScanFunc callback) {
    sWirelessManagerData.manager->ggidScanCallback = callback;
}

void sub_02033A90(WirelessManagerConnectFunc callback) {
    sWirelessManagerData.manager->connectCallback = callback;
}

void sub_02033AA4(BOOL pause) {
    sWirelessManagerData.manager->pauseConnection = pause;
}

BOOL sub_02033AB8(void) {
    return sWirelessManagerData.manager->pauseConnection;
}

void sub_02033ACC(BOOL pause) {
    sWirelessManagerData.manager->pauseConnectSystem = pause;
}
