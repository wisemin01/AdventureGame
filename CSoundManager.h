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

	HWND									m_hWnd;						// 핸들
	LPDIRECTSOUND							m_lpDirectSound;			// 사운드버퍼
	std::map<string, LPDIRECTSOUNDBUFFER>	m_mapSoundBuffers;			// 사운드버퍼배열

public:

	CSoundManager();
	virtual ~CSoundManager();

	// 현재 저장된 사운드의 개수를 반환.
	int                 GetBufferSize() { return m_mapSoundBuffers.size(); }

	// 저장된 맵에서 버퍼를 찾아 재생.
	void                Play(const string& key, BOOL Loop = FALSE);

	// 저장된 맵에서 버퍼를 찾은 뒤 같은 메모리 공간을 공유하는 새 버퍼를 만들어 재생.
	// 이 방법으로 연속으로 재생되는 소리 (총소리 등) 을 끊임없이 재생할 수 있다.
	void				DuplicatePlay(const string& key, int volumePercent = 100);
	// 이건 카메라와의 x 간격 계산해서 오른쪽, 왼쪽을 조절해서 재생.
	void				DuplicateDistancePlay(const string& key, Vector3 position, int volumePercent = 100);
	// 버퍼를 찾아 멈춤.
	void                Stop(const string& key);
	// 모든 버퍼를 찾아 멈춤.
	void                AllStop();

	// 팬 설정 ( -10000 ~ 10000 ) 음수 쪽일수록 왼쪽에 출력 양수 쪽은 오른쪽에 소리가 출력.
	void                SetPan(const string& key, long pan);
	// 새 볼륨을 지정.
	void                SetVol(const string& key, int volPercent); // 0 ~ 100 % 조정.
																   // 볼륨을 얻어옴.
	int 				GetVol(const string& key);
	// 볼륨을 더함.
	void				AddVol(const string& key, int volPercent);

	// 지금 이 음원이 재생되고 있는지를 반환.
	BOOL                IsPlaying(const string& key);


	// 사운드 추가.
	void				AddSound(const string& key, const string& wavPath, int flag = 0);
	// 사운드 삭제.
	void                DeleteSound(const string& key);
	void				DeleteAllSound();

private:

	// 맵 안에 음원이 저장되어있는지를 알아옴
	BOOL                IsBufIndex(const string& key);

	// 초기화.
	void                DirectSoundInit();
	// 해제.
	void				DirectSoundDestroy();

	// WAV 파일을 로드 DirectSound는 mp3를 지원하지 않음.
	LPDIRECTSOUNDBUFFER LoadWav(const string& path, int Flag = 0);
	// 새 버퍼를 맵에 추가함.
	BOOL                AddSoundBuffer(const string& key, LPDIRECTSOUNDBUFFER buffer);
};

#define SOUND (*CSoundManager::Instance())