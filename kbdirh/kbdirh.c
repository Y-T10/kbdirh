#pragma once

// 不要な機能を除外する
#define WIN32_LEAN_AND_MEAN
#define NOCOM

#include <Windows.h>
#include <dontuse.h>
#include <kbd.h>

/* レイアウトデータの属性設定 */
#if defined(_M_IA64)
    #error "IA64 is not supported."
#endif

// 広域変数のデータセクション名を指定する
#pragma data_seg(".data")
// キーボードデータの属性マクロを定義する
#define ATTR_KBD_DATA

// スキャンコード、仮想キーコード対応表
static ATTR_KBD_DATA USHORT SC2VK[] = {
    0xff, 0xff, 0xff      , 0xff, 0xff    , 0xff, 0xff, 0xff, 0xff      , 0xff,
    0xff, 0xff, 0xff      , 0xff, VK_BACK , 0xff, 0xff, 0xff, 0xff      , 'R' ,
    0xff, 0xff, 0xff      , 'I' , 0xff    , 0xff, 0xff, 0xff, VK_RETURN , 0xff,
    0xff, 0xff, 0xff      , 0xff, 0xff    , 'H' , 0xff, 0xff, 0xff      , 0xff,
    0xff, VK_OEM_AUTO | KBDSPECIAL, VK_LSHIFT ,
};

static ATTR_KBD_DATA VSC_VK EOSC2VK[] = {
    {.Vsc = 0, .Vk = 0 }
};

static ATTR_KBD_DATA VSC_VK E1SC2VK[] = {
    {.Vsc = 0, .Vk = 0 }
};

// キーの名前一覧
// 修飾キーの名称を定義する
static ATTR_KBD_DATA VSC_LPWSTR KeyName[] = {
    {0x29, L"半角全角"},
    {0x2a, L"LShift"},
    {0x1c, L"改行"},
    {0x0e, L"BS"},
    {0x00, NULL}
};

// 修飾キーのビット位置を指定
// VK_SHIFTは左右両方のShiftを表す
static ATTR_KBD_DATA VK_TO_BIT VKBitPos[] = {
    { VK_SHIFT   , KBDSHIFT},
    { 0          , 0       }
};

// キーボードの修飾キー
// Shift,Control,Alt,かなキーを有効な修飾キーとする
// 下記の修飾キー押下状態のみを扱い、これらに番号を振る
// * 1: Shiftキー押下
// * 2: かなキー押下
static ATTR_KBD_DATA MODIFIERS Modifier = {
    .pVkToBit = (PVK_TO_BIT)(VKBitPos),
    .wMaxModBits = 1,
    .ModNumber = {
        //-----押下状態-----||--押下状態番号--*/              
        /*  3,      2,    1*/ 0,
        /*            Shift*/ 1,          
    }
};

// キーの出力文字
// H, I, Rキーの出力文字を定義する
// M扱われる修飾キー押下状態に対応する文字を、wchにて定義する
// * wchの配列の添え字は、odifierConf.ModNumberの番号となる
// 属性CAPLOKは、CapsLock状態の影響を受けることを示す
static ATTR_KBD_DATA VK_TO_WCHARS2 VKOutputChar2[] = {
    // 仮想キーコード   , キーの属性            ,        {none ,Shift }
    {.VirtualKey = 'H',       .wch = {L'h' , L'H' }},
    {.VirtualKey = 'I',       .wch = {L'i' , L'I' }},
    {.VirtualKey = 'R',       .wch = {L'r' , L'R' }},
    {.VirtualKey = VK_RETURN, .wch = {L'\n', L'\n'}},
    {.VirtualKey = VK_BACK  , .wch = {L'\b', L'\b'}},
    {.VirtualKey = 0  ,       .wch = {L'\0', L'\0'}}
};

// 出力文字リスト一覧
static ATTR_KBD_DATA VK_TO_WCHAR_TABLE OutputCharLists[] = {
    {(PVK_TO_WCHARS1)(VKOutputChar2) , 2, sizeof(VKOutputChar2[0])},
    {NULL, 0, 0}
};

// レイアウト情報
ATTR_KBD_DATA KBDTABLES LayoutInfo = {
    // 修飾情報
    .pCharModifiers = &Modifier,
    // 出力文字
    .pVkToWcharTable = OutputCharLists,
    // キー名
    .pKeyNames = KeyName,
    .pKeyNamesExt = NULL,
    .pKeyNamesDead = NULL,
    // スキャンコード変換情報
    .pusVSCtoVK = SC2VK,
    .bMaxVSCtoVK = sizeof(SC2VK) / sizeof(SC2VK[0]),
    .pVSCtoVK_E0 = EOSC2VK,
    .pVSCtoVK_E1 = E1SC2VK,
    // レイアウト情報
    .dwType = KEYBOARD_TYPE_JAPAN
};

// キーの機能テーブル
static ATTR_KBD_DATA VK_F VKFuncTable[] = {
    {
        .Vk = VK_OEM_AUTO ,
        .NLSFEProcType = KBDNLS_TYPE_NORMAL,     // キーの押し下げ時にイベントを起こす
        .NLSFEProcCurrent = KBDNLS_INDEX_NORMAL, // NLSFEProcを最初に使用する
        .NLSFEProcSwitch = 0x00,                 // NLSFEProcAltを使用しない
        .NLSFEProc = {
        //-----押下状態-----|||------入力処理------*/ 
        /*  3,      2,    1*/ {KBDNLS_SBCSDBCS,0},
        /*            Shift*/ {KBDNLS_SBCSDBCS,0}, 
        /*    Control      */ {KBDNLS_NULL,0},
        /*    Control Shift*/ {KBDNLS_NULL,0},
        /*Alt              */ {KBDNLS_NULL,0},
        /*Alt         Shift*/ {KBDNLS_NULL,0},
        /*Alt Control      */ {KBDNLS_NULL,0},
        /*Alt Control Shift*/ {KBDNLS_NULL,0}
        },
        .NLSFEProcAlt = {
        /*  3,      2,    1*/ {KBDNLS_NULL,0},
        /*            Shift*/ {KBDNLS_NULL,0},
        /*    Control      */ {KBDNLS_NULL,0},
        /*    Control Shift*/ {KBDNLS_NULL,0},
        /*Alt              */ {KBDNLS_NULL,0},
        /*Alt         Shift*/ {KBDNLS_NULL,0},
        /*Alt Control      */ {KBDNLS_NULL,0},
        /*Alt Control Shift*/ {KBDNLS_NULL,0}
        }
    }
};

//  言語固有のレイアウト情報
static ATTR_KBD_DATA KBDNLSTABLES NLSInfo = {
    .NumOfMouseVKey = 0,
    .pusMouseVKey = NULL,
    .pVkToF = VKFuncTable,
    .NumOfVkToF = sizeof(VKFuncTable) / sizeof(VKFuncTable[0])
};

// キーボードレイアウト情報を返す
KBDTABLES* KbdLayerDescriptor() {
    return &LayoutInfo;
}

// 言語固有の機能情報を返す
KBDNLSTABLES* KbdNlsLayerDescriptor() {
    return &NLSInfo;
}
