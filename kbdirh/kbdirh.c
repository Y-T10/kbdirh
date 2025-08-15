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
// スキャンコードmに対応する仮想キーコードで、SA2VK[m]を初期化する
static ATTR_KBD_DATA USHORT SC2VK[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 'R' ,
    0xff, 0xff, 0xff, 'I' , 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 'H' , 0xff, 0xff, 0xff, 0xff, 0xff,
    VK_OEM_AUTO | KBDSPECIAL, VK_LSHIFT,
};

// キーの名前一覧
// 修飾キーの名称を定義する
static ATTR_KBD_DATA VSC_LPWSTR KeyName[] = {
    {0x29, L"半角全角"},
    {0x2a, L"LShift"},
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
    .wMaxModBits = 8,
    .ModNumber = {
        /*添字   */               // 押下されているキー
        /*0b0000*/ 0,            // なし
        /*0b0001*/ 1,            // Shift
    }
};

// キーの出力文字
// H, I, Rキーの出力文字を定義する
// M扱われる修飾キー押下状態に対応する文字を、wchにて定義する
// * wchの配列の添え字は、odifierConf.ModNumberの番号となる
// 属性CAPLOKは、CapsLock状態の影響を受けることを示す
static ATTR_KBD_DATA VK_TO_WCHARS2 VKOutputChar2[] = {
    // 仮想キーコード   , キーの属性            ,        {none ,Shift }
    {.VirtualKey = 'H', .Attributes = CAPLOK, .wch = {L'h' , L'H' }},
    {.VirtualKey = 'I', .Attributes = CAPLOK, .wch = {L'i' , L'I' }},
    {.VirtualKey = 'R', .Attributes = CAPLOK, .wch = {L'r' , L'R' }},
    {.VirtualKey = 0  , .Attributes = 0     , .wch = {L'\0', L'\0'}}
};

// 出力文字リスト一覧
static ATTR_KBD_DATA VK_TO_WCHAR_TABLE OutputCharLists[] = {
    {(PVK_TO_WCHARS1)(VKOutputChar2) , 2, sizeof(VKOutputChar2[0])},
    {NULL, 0, 0}
};

// レイアウト情報
static ATTR_KBD_DATA KBDTABLES LayoutInfo = {
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
    .pVSCtoVK_E0 = NULL,
    .pVSCtoVK_E1 = NULL,
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
        // 修飾キーの押下状態
        // TODO: 各動作を確認する
        {KBDNLS_SBCSDBCS, 0},   // none              
        {KBDNLS_SBCSDBCS, 0},   // Shift             
        {KBDNLS_NULL,0},        // Control           
        {KBDNLS_NULL,0},        // Shift,Control     
        {KBDNLS_NULL,0},        // Alt               
        {KBDNLS_NULL,0},        // Shift,Alt         
        {KBDNLS_NULL,0},        // Control,Alt       
        {KBDNLS_NULL,0}         // Shift,Control,Alt 
        },
        .NLSFEProcAlt = {
            {KBDNLS_NULL,0},
            {KBDNLS_NULL,0},
            {KBDNLS_NULL,0},
            {KBDNLS_NULL,0},
            {KBDNLS_NULL,0},
            {KBDNLS_NULL,0},
            {KBDNLS_NULL,0},
            {KBDNLS_NULL,0}
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
