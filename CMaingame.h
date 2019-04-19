#pragma once


class CMaingame
{
public:
	CMaingame();
	~CMaingame();

	HRESULT Init();
	void Update();
	void Render();
	void Destroy();

	HRESULT Reset();
	void Lost();
};

