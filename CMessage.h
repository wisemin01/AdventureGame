#pragma once

class CObject;
class CMessage
{
public:
	message_type m_eType;

	lp_object m_pSender = null;
	lp_object m_pListener;

	lp_void m_pPostScript;

	unsigned long m_ulDispatchTime;

	bool m_bDestPostScript = false;

public:
	CMessage(message_type type, lp_object sender, lp_object listener, lp_void extra_info, bool is_auto_delete = false)
		: m_eType(type), m_pSender(sender), m_pListener(listener),
		m_pPostScript(extra_info), m_bDestPostScript(is_auto_delete) {}
	virtual ~CMessage() { if (m_bDestPostScript) safe_delete(m_pPostScript); }

	void SetTime(unsigned long dispatched_time) { m_ulDispatchTime = dispatched_time; }
	message_type GetType() { return m_eType; }

	bool operator < (CMessage tel) const { return m_ulDispatchTime < tel.m_ulDispatchTime; }
};

