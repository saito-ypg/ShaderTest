#include "Input.h"
#include<DirectXMath.h>
namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	LPDIRECTINPUTDEVICE8 pMouseDevice = nullptr;
	BYTE keyState[256] = { 0 };
	BYTE prevKeyState[256] = { 0 }; //前フレームでの各キーの状態
	DIMOUSESTATE mouseState;
	DIMOUSESTATE prevMouseState;
	XMVECTOR mousePosition;

	HRESULT Initialize(HWND hWnd)
	{
		HRESULT hr=S_OK;
		hr=DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);//Input本体作成
		if (FAILED(hr)) return hr;
		hr=pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);//キーボードとしてデバイスを作成
		if (FAILED(hr)) return hr;
		hr=pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr)) return hr;
		hr=pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		if (FAILED(hr)) return hr;
		
		//マウス登録
		hr = pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, nullptr);
		if (FAILED(hr)) return hr;
		hr = pMouseDevice->SetDataFormat(&c_dfDIMouse);
		if (FAILED(hr)) return hr;
		hr = pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		if (FAILED(hr)) return hr;

		return S_OK;
	}

	HRESULT Update()
	{
		//キーボード
		memcpy(prevKeyState,keyState,sizeof(keyState[0]) * 256);
		HRESULT hr=pKeyDevice->Acquire();
		if (FAILED(hr)) return hr;
		hr=pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
		if (FAILED(hr)) return hr;

		//マウス
		prevMouseState = mouseState;
		hr = pMouseDevice->Acquire();
		if (FAILED(hr)) return hr;
		hr = pMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
		if (FAILED(hr)) return hr;
		return S_OK;
	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode]&0x80)	
		{
			return true;
		}
		return false;
	}
	bool IsKeyDown(int keyCode)
	{
		//今は押してて、前回は押してない
		if ((keyState[keyCode] & 0x80)&& !(prevKeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		//今は押してなくて、前回は押してる
		if (!(keyState[keyCode] & 0x80) && (prevKeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}
	bool IsMouseButton(int buttonCode)
	{
		if (mouseState.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}
	bool IsMouseButtonDown(int buttonCode)
	{
		//今は押してて、前回は押してない
		if ((mouseState.rgbButtons[buttonCode] & 0x80) && !(prevMouseState.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}
	bool IsMouseButtonUp(int buttonCode)
	{
		//今は押してなくて、前回は押してる
		if (!(mouseState.rgbButtons[buttonCode] & 0x80) && (prevMouseState.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
		SAFE_RELEASE(pKeyDevice);
	}

	XMVECTOR GetMousePosition()
	{
		return mousePosition;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition = XMVectorSet((float)x, (float)y, 0, 0);
	}
}