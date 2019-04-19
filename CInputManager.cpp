#include "DXUT.h"
#include "CInputManager.h"


CInputManager::CInputManager()
{
	memset(m_bCurKey, 0, sizeof(m_bCurKey));
	memset(m_bOldKey, 0, sizeof(m_bOldKey));
}


CInputManager::~CInputManager()
{
}

void CInputManager::InputUpdate()
{
	for (size_t i = 0; i < 256; i++)
	{
		m_bOldKey[i] = m_bCurKey[i];
		m_bCurKey[i] = (bool)GetAsyncKeyState(i);
	}
}

bool CInputManager::GetKeyDown(int key_type)
{
	return (m_bOldKey[key_type] == false) && (m_bCurKey[key_type] == true);
}

bool CInputManager::GetKeyUp(int key_type)
{
	return (m_bOldKey[key_type] == true) && (m_bCurKey[key_type] == false);
}

bool CInputManager::GetKeyPress(int key_type)
{
	return (m_bOldKey[key_type] == true) && (m_bCurKey[key_type] == true);
}

Vector3 CInputManager::GetScreenMouse()
{
	POINT pt;

	GetCursorPos(&pt);

	ScreenToClient(DXUTGetHWND(), &pt);

	return { (float)pt.x,(float)pt.y,0 };
}

Vector3 CInputManager::GetScrollMouse()
{
	return GetScreenMouse() + CAMERA.GetPosition() - V3CENTER;
}
