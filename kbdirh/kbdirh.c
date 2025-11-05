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

// スキャンコード、仮想キーコード対応表
static USHORT SC2VK[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x07
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x08, 0xff, // 0x0f
    0xff, 0xff, 0xff, 0x52, 0xff, 0xff, 0xff, 0x49, // 0x17
    0xff, 0xff, 0xff, 0xff, 0x0d, 0xff, 0xff, 0xff, // 0x1f
    0xff, 0xff, 0xff, 0x48, 0xff, 0xff, 0xff, 0xff, // 0x27
    0xff, 0xf3, 0xa0,                               // 0x2a
};

static VSC_VK EOSC2VK[] = {
    {.Vsc = 0, .Vk = 0 }
};

static VSC_VK E1SC2VK[] = {
    {.Vsc = 0, .Vk = 0 }
};

// キーの名前一覧
// 修飾キーの名称を定義する
static VSC_LPWSTR KeyName[] = {
    {0x29, L"半角全角"},
    {0x2a, L"LShift"},
    {0x1c, L"改行"},
    {0x0e, L"BS"},
    {0x00, NULL}
};

// 修飾キーのビット位置を指定
// VK_SHIFTは左右両方のShiftを表す
static VK_TO_BIT VKBitPos[] = {
    { VK_SHIFT   , KBDSHIFT},
    { 0          , 0       }
};

// キーボードの修飾キー
// Shiftキーを有効な修飾キーとし、Shiftが押されているパターンを1とする
static MODIFIERS Modifier = {
    .pVkToBit = (PVK_TO_BIT)(VKBitPos),
    .wMaxModBits = 1,
    .ModNumber = {
        0, // 修飾キーが押されていない
        1, // Shiftキーが押されている         
    }
};

// キーの出力文字
// H, I, Rキーの出力文字を定義する
// 仮想キーコードVirtualKeyの出力文字をwchに持つ
// * wch[0]: Shiftキーが押されていない場合の文字 
// * wch[1]: Shiftキーが押されている場合の文字
static VK_TO_WCHARS2 VKOutputChar2[] = {
    {.VirtualKey = 0x48, .wch = {L'h' , L'H' }},
    {.VirtualKey = 0x49, .wch = {L'i' , L'I' }},
    {.VirtualKey = 0x52, .wch = {L'r' , L'R' }},
    {.VirtualKey = 0x0d, .wch = {L'\n', L'\n'}},
    {.VirtualKey = 0x08, .wch = {L'\b', L'\b'}},
    {.VirtualKey = 0x00, .wch = {L'\0', L'\0'}}
};

// 出力文字リスト一覧
static VK_TO_WCHAR_TABLE OutputCharLists[] = {
    {(PVK_TO_WCHARS1)(VKOutputChar2) , 2, sizeof(VKOutputChar2[0])},
    {NULL, 0, 0}
};

// レイアウト情報
static KBDTABLES LayoutInfo = {
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
static VK_F VKFuncTable[] = {
    {
        // 半角全角キーの操作を定義する
        .Vk = 0xf3 ,
        // キーの押し下げ時にイベントを起こす
        .NLSFEProcType = KBDNLS_TYPE_NORMAL,
        // NLSFEProcを最初に使用する
        .NLSFEProcCurrent = KBDNLS_INDEX_NORMAL,
        // NLSFEProcAltを使用しない
        .NLSFEProcSwitch = 0x00,
        // 修飾キーの押下パターン時の操作
        .NLSFEProc = {
            // IMEの半角/全角入力を切り替える
            {KBDNLS_SBCSDBCS,0}, // 修飾キーが押されていない 
            {KBDNLS_SBCSDBCS,0}, // Shift
            // 何もしない
            {KBDNLS_NOEVENT,0},  // Control
            {KBDNLS_NOEVENT,0},  // Shift, Control
            {KBDNLS_NOEVENT,0},  // Alt
            {KBDNLS_NOEVENT,0},  // Shift, Alt
            {KBDNLS_NOEVENT,0},  // Control, Alt
            {KBDNLS_NOEVENT,0}   // Shift, Control, Alt
        },
        .NLSFEProcAlt = {
            // 何もしない
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
static KBDNLSTABLES NLSInfo = {
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
