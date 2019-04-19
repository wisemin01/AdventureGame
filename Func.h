#pragma once

//template <typename T>
//void safe_delete(T* obj)
//{
//	if (obj)
//	{
//		delete obj;
//		obj = null;
//	}
//}

float GetRealRandom(float min, float max);

int GetIntRandom(int min, int max);

#define lerp(target, start, end, speed) (*target) = (start) + ((end) - (start)) * (speed)

#define safe_delete(p) SAFE_DELETE(p)

#define GetD3D9Device() DXUTGetD3D9Device()
#define GetNowTime()	timeGetTime()
#define GetDeltaTime()	DXUTGetElapsedTime()

#define GC(com) GetComponent<com>()
#define AC(com) AddComponent<com>()

//#define error_log(log) MessageBoxA(DXUTGetHWND(), "ERROR", log, MB_OK);
#define error_log(log) std::cout << log << std::endl;

#define D3DXSPRITE_NONE 0

#define COLLISION_FUNC(other) [&](CCollider2D* other)

#if DEBUG
#define console_log(log) std::cout << log << std::endl
#else
#define console_log(log)  
#endif