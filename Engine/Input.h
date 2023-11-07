#pragma once
#include<DirectXMath.h>
#include <dInput.h>
using namespace DirectX;
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Input
{
	HRESULT Initialize(HWND hWnd);
	HRESULT Update();
	bool IsKey(int keyCode);//�L�[�������Ă����true
	bool IsKeyDown(int keyCode);//�L�[���������u�Ԃ̂�true
	bool IsKeyUp(int keyCode);//�L�[�𗣂����u�Ԃ̂�true
	bool IsMouseButton(int buttonCode);//�}�E�X�̃{�^���������Ă����true
	bool IsMouseButtonDown(int buttonCode);//�{�^�����������u�Ԃ̂�true
	bool IsMouseButtonUp(int buttonCode);//�{�^���𗣂����u�Ԃ̂�true
	XMVECTOR GetMousePosition();
	void SetMousePosition(int x, int y);
	void Release();
};
