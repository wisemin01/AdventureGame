#pragma once
#include "CSingleton.h"
class CInputManager :
	public CSingleton<CInputManager>
{
private:
	bool m_bCurKey[256];
	bool m_bOldKey[256];
public:
	CInputManager();
	virtual ~CInputManager();

	void InputUpdate();

	bool GetKeyDown(int key_type);
	bool GetKeyUp(int key_type);
	bool GetKeyPress(int key_type);

	Vector3 GetScreenMouse();
	Vector3 GetScrollMouse();
};

#define INPUT (*CInputManager::Instance())