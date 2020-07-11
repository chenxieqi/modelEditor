//=============================================================================
// 入力処理 
//=============================================================================
#include "input_system.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		JOY_MIN_X		(-256)		// Ｘ軸最小値
#define		JOY_MAX_X		(256)		// Ｘ軸最大値
#define		JOY_MIN_Y		(-256)		// Ｙ軸最小値
#define		JOY_MAX_Y		(256)		// Ｙ軸最大値

//*****************************************************************************
// 静的変数
//*****************************************************************************
LPDIRECTINPUT8		InputSystem::m_pDInput = NULL;			// IDirectInput8インターフェースへのポインタ

//=============================================================================
// InputSystemコンストラスタ
//=============================================================================
InputSystem::InputSystem()
{
    m_pDIDevice = NULL;
}

//=============================================================================
// InputSystemデストラスタ
//=============================================================================
InputSystem::~InputSystem()
{
}

//=============================================================================
// 入力処理の初期化処理
//=============================================================================
HRESULT InputSystem::Init(HINSTANCE hInst, HWND hWnd)
{
    HRESULT hr = S_FALSE;

    // DirectInputオブジェクトの作成
    if (!m_pDInput)
    {
        hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
            IID_IDirectInput8, (void**)&m_pDInput, NULL);
    }

    return hr;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void InputSystem::Uninit(void)
{
    if (m_pDIDevice)
    {// デバイスオブジェクトの開放
        m_pDIDevice->Unacquire();

        m_pDIDevice->Release();
        m_pDIDevice = NULL;
    }

    if (m_pDInput)
    {// DirectInputオブジェクトの開放
        m_pDInput->Release();
        m_pDInput = NULL;
    }
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
HRESULT InputSystem::Update(void)
{
    return S_OK;
}

//=============================================================================
// InputKeyboardコンストラスタ
//=============================================================================
InputKeyboard::InputKeyboard()
{
    // 各ワークのクリア
    ZeroMemory(m_aKeyState, sizeof m_aKeyState);
    ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
    ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
    ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
    ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);
}

//=============================================================================
// InputKeyboardデストラスタ
//=============================================================================
InputKeyboard::~InputKeyboard()
{
}

//=============================================================================
// キーボードの初期化処理
//=============================================================================
HRESULT InputKeyboard::Init(HINSTANCE hInst, HWND hWnd)
{
    HRESULT hr;

    // 入力処理の初期化
    InputSystem::Init(hInst, hWnd);

    // デバイスオブジェクトを作成
    hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL);
    if (FAILED(hr) || m_pDIDevice == NULL)
    {
        MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
        return hr;
    }

    // データフォーマットを設定
    hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
        return hr;
    }

    // 協調モードを設定（フォアグラウンド＆非排他モード）
    hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    if (FAILED(hr))
    {
        MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
        return hr;
    }

    // m_pDIDeviceアクセス権を獲得(入力制御開始)
    m_pDIDevice->Acquire();

    return hr;
}

//=============================================================================
// キーボード入力処理の開放
//=============================================================================
void InputKeyboard::Uninit(void)
{
    // 入力処理の開放
    InputSystem::Uninit();
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
HRESULT InputKeyboard::Update(void)
{
    HRESULT hr;
    BYTE aKeyState[256];

    if (!m_pDIDevice)
    {
        return S_FALSE;
    }

    // デバイスからデータを取得
    hr = m_pDIDevice->GetDeviceState(sizeof(aKeyState), aKeyState);
    if (SUCCEEDED(hr))
    {
        for (int nCntKey = 0; nCntKey < 256; nCntKey++)
        {
            m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
            m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
            m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

            if (aKeyState[nCntKey])
            {
                m_aKeyStateRepeatCnt[nCntKey]++;
                if (m_aKeyStateRepeatCnt[nCntKey] >= 20)
                {
                    m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
                }
            }
            else
            {
                m_aKeyStateRepeatCnt[nCntKey] = 0;
                m_aKeyStateRepeat[nCntKey] = 0;
            }

            m_aKeyState[nCntKey] = aKeyState[nCntKey];
        }
    }
    else
    {
        m_pDIDevice->Acquire();
    }

    return hr;
}

//=============================================================================
// キーボードデータ取得(プレス)
//=============================================================================
BOOL InputKeyboard::GetKeyPress(int nKey)
{
    return (m_aKeyState[nKey] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// キーボードデータ取得(トリガー)
//=============================================================================
BOOL InputKeyboard::GetKeyTrigger(int nKey)
{
    return (m_aKeyStateTrigger[nKey] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// キーボードデータ取得(リリース)
//=============================================================================
BOOL InputKeyboard::GetKeyRelease(int nKey)
{
    return (m_aKeyStateRelease[nKey] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// キーボードデータ取得(リピート)
//=============================================================================
BOOL InputKeyboard::GetKeyRepeat(int nKey)
{
    return (m_aKeyStateRepeat[nKey] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// キーボードデータ削除(トリガー)
//=============================================================================
void InputKeyboard::FlushKeyTrigger(int nKey)
{
    m_aKeyStateTrigger[nKey] = 0;
}

//=============================================================================
// InputMouseコンストラスタ
//=============================================================================
InputMouse::InputMouse()
{
    ZeroMemory(&m_mouseState, sizeof(m_mouseState));
    ZeroMemory(&m_mouseStateTrigger, sizeof(m_mouseStateTrigger));
}

//=============================================================================
// InputMouseデストラスタ
//=============================================================================
InputMouse::~InputMouse()
{
}

//=============================================================================
// マウスの初期化
//=============================================================================
HRESULT InputMouse::Init(HINSTANCE hInst, HWND hWnd)
{
    HRESULT hr;

    // 入力処理の初期化
    InputSystem::Init(hInst, hWnd);

    // デバイスオブジェクトを作成
    hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevice, NULL);
    if (FAILED(hr) || m_pDIDevice == NULL)
    {
        MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
        return hr;
    }

    // データフォーマットを設定
    hr = m_pDIDevice->SetDataFormat(&c_dfDIMouse2);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
        return hr;
    }

    // 協調モードを設定（フォアグラウンド＆非排他モード）
    hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    if (FAILED(hr))
    {
        MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
        return hr;
    }

    // デバイスの設定
    {
        DIPROPDWORD dipdw;

        dipdw.diph.dwSize = sizeof(dipdw);
        dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
        dipdw.diph.dwObj = 0;
        dipdw.diph.dwHow = DIPH_DEVICE;
        dipdw.dwData = DIPROPAXISMODE_REL;

        hr = m_pDIDevice->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
    }

    // キーボードへのアクセス権を獲得(入力制御開始)
    m_pDIDevice->Acquire();

    return hr;
}

//=============================================================================
// マウス入力処理の開放
//=============================================================================
void InputMouse::Uninit(void)
{
    // 入力処理の開放
    InputSystem::Uninit();
}

//=============================================================================
// マウス入力処理更新
//=============================================================================
HRESULT InputMouse::Update(void)
{
    HRESULT hr;
    DIMOUSESTATE2 mouseState;

    if (!m_pDIDevice)
    {
        return S_FALSE;
    }

    // デバイスからデータを取得
    hr = m_pDIDevice->GetDeviceState(sizeof(mouseState), &mouseState);
    if (SUCCEEDED(hr))
    {
        m_mouseStateTrigger.lX = ((m_mouseState.lX ^ mouseState.lX) & mouseState.lX);
        m_mouseStateTrigger.lY = ((m_mouseState.lY ^ mouseState.lY) & mouseState.lY);
        m_mouseStateTrigger.lZ = ((m_mouseState.lZ ^ mouseState.lZ) & mouseState.lZ);
        for (int nCntKey = 0; nCntKey < 8; nCntKey++)
        {
            m_mouseStateTrigger.rgbButtons[nCntKey]
                = ((m_mouseState.rgbButtons[nCntKey] ^ mouseState.rgbButtons[nCntKey]) & mouseState.rgbButtons[nCntKey]);
        }

        m_mouseState = mouseState;

        // スクリーン座標を取得
        GetCursorPos(&m_posMouseWorld);
    }
    else
    {
        m_pDIDevice->Acquire();
    }

    return hr;
}

//=============================================================================
// マウスデータ取得(左プレス)
//=============================================================================
BOOL InputMouse::GetLeftPress(void)
{
    return (m_mouseState.rgbButtons[0] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// マウスデータ取得(左トリガー)
//=============================================================================
BOOL InputMouse::GetLeftTrigger(void)
{
    return (m_mouseStateTrigger.rgbButtons[0] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// マウスデータ取得(右プレス)
//=============================================================================
BOOL InputMouse::GetRightPress(void)
{
    return (m_mouseState.rgbButtons[1] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// マウスデータ取得(右トリガー)
//=============================================================================
BOOL InputMouse::GetRightTrigger(void)
{
    return (m_mouseStateTrigger.rgbButtons[1] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// マウスデータ取得(中央プレス)
//=============================================================================
BOOL InputMouse::GetCenterPress(void)
{
    return (m_mouseState.rgbButtons[2] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// マウスデータ取得(中央トリガー)
//=============================================================================
BOOL InputMouse::GetCenterTrigger(void)
{
    return (m_mouseStateTrigger.rgbButtons[2] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// マウスデータ取得(Ｘ軸移動)
//=============================================================================
LONG InputMouse::GetAxisX(void)
{
    return m_mouseState.lX;
}

//=============================================================================
// マウスデータ取得(Ｙ軸移動)
//=============================================================================
LONG InputMouse::GetAxisY(void)
{
    return m_mouseState.lY;
}

//=============================================================================
// マウスデータ取得(Ｚ軸移動)
//=============================================================================
LONG InputMouse::GetAxisZ(void)
{
    return m_mouseState.lZ;
}

//=============================================================================
// InputJoypadコンストラスタ
//=============================================================================
InputJoypad::InputJoypad()
{
}

//=============================================================================
// InputJoypadデストラスタ
//=============================================================================
InputJoypad::~InputJoypad()
{
}

//=============================================================================
// ジョイパッドの初期化
//=============================================================================
HRESULT InputJoypad::Init(HINSTANCE hInst, HWND hWnd)
{
    HRESULT hr;

    // 入力処理の初期化
    InputSystem::Init(hInst, hWnd);

    hr = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, InputJoypad::EnumJoyCallback, this, DIEDFL_ATTACHEDONLY);
    if (FAILED(hr) || m_pDIDevice == NULL)
    {
        //		MessageBox(hWnd, "ジョイパッドがねぇ！", "警告！", MB_ICONWARNING);
        return hr;
    }

    // データフォーマットを設定
    hr = m_pDIDevice->SetDataFormat(&c_dfDIJoystick2);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "ジョイパッドのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
        return hr;
    }

    // 協調モードを設定（フォアグラウンド＆非排他モード）
    hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    if (FAILED(hr))
    {
        MessageBox(hWnd, "ジョイパッドの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
        return hr;
    }

    hr = m_pDIDevice->EnumObjects(InputJoypad::EnumAxesCallback, this, DIDFT_AXIS);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "ジョイパッドの・・・", "警告！", MB_ICONWARNING);
        return hr;
    }

    // デバイスの設定
#if 0
    {
        DIPROPRANGE diprg;

        ZeroMemory(&diprg, sizeof(diprg));
        diprg.diph.dwSize = sizeof(diprg);
        diprg.diph.dwHeaderSize = sizeof(diprg.diph);

        diprg.diph.dwObj = DIJOFS_X;
        diprg.diph.dwHow = DIPH_BYOFFSET;
        diprg.lMin = JOY_MIN_X;
        diprg.lMax = JOY_MAX_X;
        m_pDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);	// Ｘ方向の範囲を指定

        diprg.diph.dwObj = DIJOFS_Y;
        diprg.diph.dwHow = DIPH_BYOFFSET;
        diprg.lMin = JOY_MIN_Y;
        diprg.lMax = JOY_MAX_Y;
        m_pDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);	// Ｙ方向の範囲を指定

        diprg.diph.dwObj = DIJOFS_RY;
        diprg.diph.dwHow = DIPH_BYOFFSET;
        diprg.lMin = JOY_MIN_Y;
        diprg.lMax = JOY_MAX_Y;
        m_pDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);	// Ｙ方向の範囲を指定
    }
#endif

    // キーボードへのアクセス権を獲得(入力制御開始)
    m_pDIDevice->Acquire();

    return hr;
}

//=============================================================================
// ジョイスティック問い合わせ用コールバック関数
//=============================================================================
BOOL CALLBACK InputJoypad::EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
    DIDEVCAPS	diDevCaps;			// デバイス情報
    HRESULT		hRes;
    InputJoypad *pThis = reinterpret_cast<InputJoypad*>(pvRef);

    // ジョイスティック用デバイスオブジェクトを作成
    hRes = pThis->m_pDInput->CreateDevice(lpddi->guidInstance, &pThis->m_pDIDevice, NULL);
    if (FAILED(hRes))
    {
        return DIENUM_CONTINUE;		// 列挙を続ける
    }

    // ジョイスティックの能力を調べる
    diDevCaps.dwSize = sizeof(DIDEVCAPS);
    hRes = pThis->m_pDIDevice->GetCapabilities(&diDevCaps);
    if (FAILED(hRes))
    {
        SAFE_RELEASE(pThis->m_pDIDevice);
        return DIENUM_CONTINUE;		// 列挙を続ける
    }

    return DIENUM_STOP;		// このデバイスを使うので列挙を終了する
}

BOOL CALLBACK InputJoypad::EnumAxesCallback(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
    HRESULT hr;
    InputJoypad *pThis = reinterpret_cast<InputJoypad*>(pvRef);

    // 軸の値の範囲を設定（-1000〜1000）
    DIPROPRANGE diprg;
    ZeroMemory(&diprg, sizeof(diprg));
    diprg.diph.dwSize = sizeof(diprg);
    diprg.diph.dwHeaderSize = sizeof(diprg.diph);
    diprg.diph.dwObj = lpddoi->dwType;
    diprg.diph.dwHow = DIPH_BYID;
    diprg.lMin = -1000;
    diprg.lMax = +1000;
    hr = pThis->m_pDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
    if (FAILED(hr))
    {
        return DIENUM_STOP;
    }

    return DIENUM_CONTINUE;
}

//=============================================================================
// ジョイパッド入力処理の開放
//=============================================================================
void InputJoypad::Uninit(void)
{
    // 入力処理の開放
    InputSystem::Uninit();
}

//=============================================================================
// ジョイパッド入力処理更新
//=============================================================================
HRESULT InputJoypad::Update(void)
{
    HRESULT hr;
    bool aKeyStateOld[KEY_MAX];

    if (!m_pDIDevice)
    {
        return S_FALSE;
    }

    // 前回のデータを保存
    for (int nCntKey = 0; nCntKey < KEY_MAX; nCntKey++)
    {
        aKeyStateOld[nCntKey] = m_aKeyState[nCntKey];
    }

    // デバイスからデータを取得
    hr = m_pDIDevice->GetDeviceState(sizeof(m_joypadState), &m_joypadState);
    if (SUCCEEDED(hr))
    {
        // キー情報設定
        SetKeyState();

        for (int nCntKey = 0; nCntKey < KEY_MAX; nCntKey++)
        {
            m_aKeyStateTrigger[nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyState[nCntKey]) & m_aKeyState[nCntKey];
            m_aKeyStateRelease[nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyState[nCntKey]) & !m_aKeyState[nCntKey];
            m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

            if (m_aKeyState[nCntKey])
            {
                m_aKeyStateRepeatCnt[nCntKey]++;
                if (m_aKeyStateRepeatCnt[nCntKey] >= 20)
                {
                    m_aKeyStateRepeat[nCntKey] = m_aKeyState[nCntKey];
                }
            }
            else
            {
                m_aKeyStateRepeatCnt[nCntKey] = 0;
                m_aKeyStateRepeat[nCntKey] = 0;
            }
        }
    }
    else
    {
        m_pDIDevice->Acquire();
    }

    return hr;
}

//=============================================================================
// ジョイパッドのキー情報設定
//=============================================================================
void InputJoypad::SetKeyState(void)
{
    if (m_joypadState.lX <= -8)
    {
        m_aKeyState[KEY_LEFT] = true;
    }
    else
    {
        m_aKeyState[KEY_LEFT] = false;
    }

    if (m_joypadState.lX >= 8)
    {
        m_aKeyState[KEY_RIGHT] = true;
    }
    else
    {
        m_aKeyState[KEY_RIGHT] = false;
    }

    if (m_joypadState.lY <= -8)
    {
        m_aKeyState[KEY_UP] = true;
    }
    else
    {
        m_aKeyState[KEY_UP] = false;
    }

    if (m_joypadState.lY >= 8)
    {
        m_aKeyState[KEY_DOWN] = true;
    }
    else
    {
        m_aKeyState[KEY_DOWN] = false;
    }

    if (m_joypadState.rgbButtons[0])
    {
        m_aKeyState[KEY_JUMP] = true;
    }
    else
    {
        m_aKeyState[KEY_JUMP] = false;
    }

    if (m_joypadState.rgbButtons[1])
    {
        m_aKeyState[KEY_SHOT] = true;
    }
    else
    {
        m_aKeyState[KEY_SHOT] = false;
    }

    if (m_joypadState.rgbButtons[6])
    {
        m_aKeyState[KEY_TURN_LEFT] = true;
    }
    else
    {
        m_aKeyState[KEY_TURN_LEFT] = false;
    }

    if (m_joypadState.rgbButtons[8])
    {
        m_aKeyState[KEY_TURN_RIGHT] = true;
    }
    else
    {
        m_aKeyState[KEY_TURN_RIGHT] = false;
    }

    if (m_joypadState.rgbButtons[7])
    {
        m_aKeyState[KEY_TURN_UP] = true;
    }
    else
    {
        m_aKeyState[KEY_TURN_UP] = false;
    }

    if (m_joypadState.rgbButtons[9])
    {
        m_aKeyState[KEY_TURN_DOWN] = true;
    }
    else
    {
        m_aKeyState[KEY_TURN_DOWN] = false;
    }

    if (m_joypadState.rgbButtons[11])
    {
        m_aKeyState[KEY_START] = true;
    }
    else
    {
        m_aKeyState[KEY_START] = false;
    }

    if (m_joypadState.rgbButtons[10])
    {
        m_aKeyState[KEY_SELECT] = true;
    }
    else
    {
        m_aKeyState[KEY_SELECT] = false;
    }
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool InputJoypad::GetKeyPress(KEY key)
{
    return m_aKeyState[key];
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool InputJoypad::GetKeyTrigger(KEY key)
{
    return m_aKeyStateTrigger[key];
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool InputJoypad::GetKeyRelease(KEY key)
{
    return m_aKeyStateRelease[key];
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool InputJoypad::GetKeyRepeat(KEY key)
{
    return m_aKeyStateRepeat[key];
}

