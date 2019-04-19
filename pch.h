#pragma once


#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <thread>

#include "Typedef.h"
#include "Func.h"


#include "CMessage.h"
#include "CGameObject.h"

#include "CBaseRenderer.h"
#include "CBaseTransform.h"

#include "CScene.h"

#include "CEntityManager.h"
#include "CSceneManager.h"
#include "CResourceManager.h"
#include "CCameraManager.h"
#include "CCollisionManager.h"
#include "CInputManager.h"
#include "CSoundManager.h"

constexpr int screen_width = 1024;
constexpr int screen_height = 576;

static bool screen_mode = true;

#define V3CENTER Vector3(screen_width / 2, screen_height / 2, 0)

constexpr const wchar_t* screen_name = L"FLIGHT GAME FRAMEWORK";