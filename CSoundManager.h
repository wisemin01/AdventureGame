#pragma once

#include "CSingleton.h"

#include <mmsystem.h>
#include <mmreg.h>
#include <DSound.h>

#pragma comment (lib , "dxerr.lib")
#pragma comment (lib , "winmm.lib")
#pragma comment (lib , "dsound.lib")
#pragma comment (lib , "dxguid.lib")

class CSoundManager
	: public CSingleton<CSoundManager>
{
private:

	HWND									m_hWnd;						// �ڵ�
	LPDIRECTSOUND							m_lpDirectSound;			// �������
	std::map<string, LPDIRECTSOUNDBUFFER>	m_mapSoundBuffers;			// ������۹迭

public:

	CSoundManager();
	virtual ~CSoundManager();

	// ���� ����� ������ ������ ��ȯ.
	int                 GetBufferSize() { return m_mapSoundBuffers.size(); }

	// ����� �ʿ��� ���۸� ã�� ���.
	void                Play(const string& key, BOOL Loop = FALSE);

	// ����� �ʿ��� ���۸� ã�� �� ���� �޸� ������ �����ϴ� �� ���۸� ����� ���.
	// �� ������� �������� ����Ǵ� �Ҹ� (�ѼҸ� ��) �� ���Ӿ��� ����� �� �ִ�.
	void				DuplicatePlay(const string& key, int volumePercent = 100);
	// �̰� ī�޶���� x ���� ����ؼ� ������, ������ �����ؼ� ���.
	void				DuplicateDistancePlay(const string& key, Vector3 position, int volumePercent = 100);
	// ���۸� ã�� ����.
	void                Stop(const string& key);
	// ��� ���۸� ã�� ����.
	void                AllStop();

	// �� ���� ( -10000 ~ 10000 ) ���� ���ϼ��� ���ʿ� ��� ��� ���� �����ʿ� �Ҹ��� ���.
	void                SetPan(const string& key, long pan);
	// �� ������ ����.
	void                SetVol(const string& key, int volPercent); // 0 ~ 100 % ����.
																   // ������ ����.
	int 				GetVol(const string& key);
	// ������ ����.
	void				AddVol(const string& key, int volPercent);

	// ���� �� ������ ����ǰ� �ִ����� ��ȯ.
	BOOL                IsPlaying(const string& key);


	// ���� �߰�.
	void				AddSound(const string& key, const string& wavPath, int flag = 0);
	// ���� ����.
	void                DeleteSound(const string& key);
	void				DeleteAllSound();

private:

	// �� �ȿ� ������ ����Ǿ��ִ����� �˾ƿ�
	BOOL                IsBufIndex(const string& key);

	// �ʱ�ȭ.
	void                DirectSoundInit();
	// ����.
	void				DirectSoundDestroy();

	// WAV ������ �ε� DirectSound�� mp3�� �������� ����.
	LPDIRECTSOUNDBUFFER LoadWav(const string& path, int Flag = 0);
	// �� ���۸� �ʿ� �߰���.
	BOOL                AddSoundBuffer(const string& key, LPDIRECTSOUNDBUFFER buffer);
};

#define SOUND (*CSoundManager::Instance())