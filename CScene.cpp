#include "DXUT.h"
#include "CScene.h"

CScene::CScene()
{
}


CScene::~CScene()
{
}

void CScene::Init()
{
}

void CScene::Update()
{
}

void CScene::Destroy()
{
}

void CScene::HandleMessage(lp_message msg)
{
	switch (msg->GetType())
	{
	case message_type::eMsgDestroy:

		Destroy();

		break;
	}

	return;
}
