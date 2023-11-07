#include "Input.h"
#include<DirectXMath.h>
namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	LPDIRECTINPUTDEVICE8 pMouseDevice = nullptr;
	BYTE keyState[256] = { 0 };
	BYTE prevKeyState[256] = { 0 }; //�O�t���[���ł̊e�L�[�̏��
	DIMOUSESTATE mouseState;
	DIMOUSESTATE prevMouseState;
	XMVECTOR mousePosition;

	HRESULT Initialize(HWND hWnd)
	{
		HRESULT hr=S_OK;
		hr=DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);//Input�{�̍쐬
		if (FAILED(hr)) return hr;
		hr=pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);//�L�[�{�[�h�Ƃ��ăf�o�C�X���쐬
		if (FAILED(hr)) return hr;
		hr=pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr)) return hr;
		hr=pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		if (FAILED(hr)) return hr;
		
		//�}�E�X�o�^
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
		//�L�[�{�[�h
		memcpy(prevKeyState,keyState,sizeof(keyState[0]) * 256);
		HRESULT hr=pKeyDevice->Acquire();
		if (FAILED(hr)) return hr;
		hr=pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
		if (FAILED(hr)) return hr;

		//�}�E�X
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
		//���͉����ĂāA�O��͉����ĂȂ�
		if ((keyState[keyCode] & 0x80)&& !(prevKeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		//���͉����ĂȂ��āA�O��͉����Ă�
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
		//���͉����ĂāA�O��͉����ĂȂ�
		if ((mouseState.rgbButtons[buttonCode] & 0x80) && !(prevMouseState.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}
	bool IsMouseButtonUp(int buttonCode)
	{
		//���͉����ĂȂ��āA�O��͉����Ă�
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