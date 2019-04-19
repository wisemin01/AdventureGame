#pragma once


class CObject
{
protected:
	object_id	m_sObjectId;
	object_tag	m_sObjectTag;

public:
	CObject();
	virtual ~CObject();

	virtual void Init();
	virtual void Update();
	virtual void Destroy();

	virtual void HandleMessage(lp_message msg) PURE;

	const object_id& GetObjectID() { return m_sObjectId; }
	const object_tag& GetObjectTag() { return m_sObjectTag; }

	void SetObjectID(const object_id& id) { m_sObjectId = id; }
	void SetObjectTag(const object_tag& tag) { m_sObjectTag = tag; }
};

