#include "DXUT.h"
#include "CStateMachine.h"

CStateMachine::CStateMachine(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj), m_pNowState(null), m_pNextState(null)
{

}

CStateMachine::~CStateMachine()
{
	
}

void CStateMachine::Init()
{

}

void CStateMachine::Update()
{
	if(m_pNowState)
		for (auto iter : m_stateNodeContainer)
		{
			if (iter->m_iPrev == m_pNowState->m_iKey && (*iter->m_pFlag) == (iter->m_bValue))
			{
				ChangeState(iter->m_iNext);
			}
		}

	if (m_pNextState)
	{
		if (m_pNowState)
		{
			m_pNowState->m_pDestroy();
		}

		m_pNowState = m_pNextState;
		m_pNowState->m_pInitialize();
		m_pNextState = null;
	}

	if (m_pNowState) {

		m_pNowState->m_pUpdate();
	}
}

void CStateMachine::Destroy()
{
	if (m_pNowState)
	{
		m_pNowState->m_pDestroy();
	}

	for (auto iter : m_stateContainer)
	{
		SAFE_DELETE(iter.second);
	}
	m_stateContainer.clear();

	for (auto iter : m_stateNodeContainer)
	{
		SAFE_DELETE(iter);
	}
	m_stateNodeContainer.clear();
}

void CStateMachine::CreateOnePattern(function<void()> init, function<void()> update, function<void()> destroy)
{
	AddState(0, init, update, destroy);
}
