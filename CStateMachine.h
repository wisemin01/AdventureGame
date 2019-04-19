#pragma once
#include "CBaseComponent.h"

#define NONE_STATE (0)

using state_key = int;

struct StateBlock
{
	std::function<void()> m_pInitialize;
	std::function<void()> m_pUpdate;
	std::function<void()> m_pDestroy;

	state_key m_iKey = NONE_STATE;
};

struct StateNode
{
	state_key m_iPrev = NONE_STATE;
	state_key m_iNext = NONE_STATE;

	bool* m_pFlag = nullptr;
	bool m_bValue = true;
};

class CStateMachine :
	public CBaseComponent
{
protected:
	std::unordered_map<int, StateBlock*> m_stateContainer;
	std::vector<StateNode*> m_stateNodeContainer;

	StateBlock* m_pNowState = null;
	StateBlock* m_pNextState = null;
public:
	CStateMachine(lp_gameobject owner_obj);
	virtual ~CStateMachine();

	template <typename T>
	void ChangeState(T change_state)
	{
		const int value = (const int)change_state;

		auto find = m_stateContainer.find(value);

		if (find != m_stateContainer.end()) {

			if (!m_pNowState)
				m_pNextState = find->second;

			if (m_pNowState && m_pNowState->m_iKey != value)
				m_pNextState = find->second;
		}
		else
			return;
	}

	template <typename T>
	void AddState(T new_state, function<void()> init, function<void()> update, function<void()> destroy)
	{
		const int c_new_state = (const int)new_state;
		auto find = m_stateContainer.find(c_new_state);

		if (find == m_stateContainer.end()) {

			StateBlock* newState = new StateBlock;

			newState->m_pInitialize = init;
			newState->m_pUpdate = update;
			newState->m_pDestroy = destroy;

			newState->m_iKey = c_new_state;

			m_stateContainer.insert(make_pair(c_new_state, newState));
		}
		else
			return;
	}
	
	template <typename T>
	void DeleteState(T del_state)
	{
		const int c_del_state = (const int)del_state;
		for (auto iter = m_stateContainer.begin(); iter != m_stateContainer.end();)
		{
			if (iter->first == c_del_state)
			{
				SAFE_DELETE(iter->second);
				iter = m_stateContainer.erase(iter);
			}
			else
				iter++;
		}
	}

	template <typename T>
	void AddNode(bool * flag, bool target_value, T prev_state, T next_state)
	{
		StateNode* new_node = new StateNode;

		new_node->m_iPrev = (int)prev_state;
		new_node->m_iNext = (int)next_state;

		new_node->m_pFlag = flag;
		new_node->m_bValue = target_value;

		m_stateNodeContainer.push_back(new_node);
	}

	void Init() override;
	void Update() override;
	void Destroy() override;

	int GetNowState() { return m_pNowState->m_iKey; }

	void CreateOnePattern(function<void()> init, function<void()> update, function<void()> destroy);
};

