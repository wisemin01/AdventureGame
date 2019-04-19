#include "DXUT.h"
#include "CSoundManager.h"

#include <process.h>

#define OUT_ERRORMESSAGE(msg) console_log(msg)

#define DSPercentToVolume(percent)	(((percent) * 100) - 10000)
#define DSVolumeToPercent(volume)	( (volume + 10000) / 100 )

CSoundManager::CSoundManager()
{
	DirectSoundInit();
}

CSoundManager::~CSoundManager(void)
{
	DirectSoundDestroy();
}


void CSoundManager::DirectSoundInit()
{
	m_hWnd = DXUTGetHWND(); // 핸들값초기화


					  // 다이렉트 사운드 객체 생성

	HRESULT dsrval = ::DirectSoundCreate(NULL, &m_lpDirectSound, NULL);

	if (dsrval != DS_OK)

	{

		OUT_ERRORMESSAGE("객체생성실패");

		return;

	}

	// 다이렉트사운드사용환경을지정한다.

	dsrval = m_lpDirectSound->SetCooperativeLevel(m_hWnd, DSSCL_PRIORITY);



	if (dsrval != DS_OK) OUT_ERRORMESSAGE("모드설정실패");

}

void CSoundManager::DirectSoundDestroy()
{
	DeleteAllSound();

	SAFE_RELEASE(m_lpDirectSound);
}

BOOL CSoundManager::AddSoundBuffer(const string& key, LPDIRECTSOUNDBUFFER buffer)
{
	auto find = m_mapSoundBuffers.find(key);

	if (find == m_mapSoundBuffers.end())
	{
		m_mapSoundBuffers.insert(make_pair(key, buffer));

		return TRUE;
	}

	return FALSE;
}

void CSoundManager::AddSound(const string & key, const string & wavPath, int flag)
{
	AddSoundBuffer(key, LoadWav(wavPath, flag));
}

void CSoundManager::DeleteSound(const string& key)

{
	auto find = m_mapSoundBuffers.find(key);

	if (find == m_mapSoundBuffers.end()) return;

	if (find->second == NULL) return;

	SAFE_RELEASE(find->second);

	m_mapSoundBuffers.erase(find);
}

void CSoundManager::DeleteAllSound()
{
	for (auto iter : m_mapSoundBuffers) {
		if (iter.second != NULL)
		{
			SAFE_RELEASE(iter.second);
		}
	}
	m_mapSoundBuffers.clear();
}

LPDIRECTSOUNDBUFFER CSoundManager::LoadWav(const string& path, int Flag)
{

	int ret;
	WORD                    wExtraAlloc;
	UINT                    i, DataIn;
	LPDIRECTSOUNDBUFFER     lpDSTemp;
	HMMIO                   hmmioIn;
	MMIOINFO                mmioinfoIn;
	MMCKINFO                ckInfo, ckInfoRIFF;
	LPWAVEFORMATEX          lpWaveFormat = 0;
	PCMWAVEFORMAT           pcmWaveFormat;
	LPBYTE                  lpData;
	LPVOID                  lpWrapPtr;
	DWORD                   dwBSize, dwWavSize, dwDataSize, dwWrapBSize;
	DSBUFFERDESC            dsbd;

	// 파일을 연다

	hmmioIn = mmioOpenA((LPSTR)path.c_str(), NULL, MMIO_ALLOCBUF | MMIO_READ);

	if (!hmmioIn) OUT_ERRORMESSAGE("WAV 파일을읽을수없음");

	// 열린 파일이 RIFF WAVE 파일인지 확인한다. 

	ckInfoRIFF.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	if (mmioDescend(hmmioIn, &ckInfoRIFF, NULL, MMIO_FINDRIFF) != 0)

	{

		OUT_ERRORMESSAGE("RIFF WAVE 파일이아닙니다");

	}

	// 'fmt' Chunk 인지 확인한다

	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');

	if (mmioDescend(hmmioIn, &ckInfo, &ckInfoRIFF, MMIO_FINDCHUNK) != 0)

	{

		OUT_ERRORMESSAGE("파일이fmt Chunk가아닙니다");

	}

	// 열린 파일을 읽는다

	ret = mmioRead(hmmioIn, (HPSTR)&pcmWaveFormat, (long) sizeof(pcmWaveFormat));

	if (ret != (long) sizeof(pcmWaveFormat))

	{

		OUT_ERRORMESSAGE("파일읽기실패");

	}

	// 읽은 화일이 PCM 형식인지 확인한다

	if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)

		wExtraAlloc = 0;

	else

	{

		ret = mmioRead(hmmioIn, (LPSTR)&wExtraAlloc, (long) sizeof(wExtraAlloc));

		if (ret != (long) sizeof(wExtraAlloc))

		{

			OUT_ERRORMESSAGE("파일읽기실패");

		}

	}



	lpWaveFormat = (LPWAVEFORMATEX)malloc(sizeof(WAVEFORMATEX) + wExtraAlloc);

	memcpy(lpWaveFormat, &pcmWaveFormat, sizeof(pcmWaveFormat));

	lpWaveFormat->cbSize = wExtraAlloc;

	if (wExtraAlloc != 0)

	{

		ret = mmioRead(hmmioIn,

			(LPSTR)(((BYTE*)&(lpWaveFormat->cbSize)) + sizeof(wExtraAlloc)),

			(long)(wExtraAlloc));

		if (ret != (long) sizeof(wExtraAlloc))

		{

			OUT_ERRORMESSAGE("파일읽기실패");

		}

	}



	if (mmioAscend(hmmioIn, &ckInfo, 0) != MMSYSERR_NOERROR)

	{

		OUT_ERRORMESSAGE("오류");

	}





	if (mmioSeek(hmmioIn, ckInfoRIFF.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)

	{

		OUT_ERRORMESSAGE("오류");

	}

	// 'data' Chunk를 찾는다

	MMCKINFO    pckIn;

	pckIn.ckid = mmioFOURCC('d', 'a', 't', 'a');

	if (mmioDescend(hmmioIn, &pckIn, &ckInfoRIFF, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)

	{

		OUT_ERRORMESSAGE("Data Chunk가 아닙니다");

	}

	dwDataSize = pckIn.cksize;

	lpData = NULL;



	// 스태틱 2차 버퍼를 만들기 위해 구조체를 생성한다.

	// 사운드 버퍼의 제어는 볼륨, 좌우밸런스, 주파수 조절이 가능하도록 한다.

	memset(&dsbd, 0, sizeof(DSBUFFERDESC));

	dsbd.dwSize = sizeof(DSBUFFERDESC);

	dsbd.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;

	dsbd.dwBufferBytes = dwDataSize;

	dwWavSize = lpWaveFormat->cbSize + sizeof(WAVEFORMATEX);



	dsbd.lpwfxFormat = (LPWAVEFORMATEX)malloc(dwWavSize);

	memcpy(dsbd.lpwfxFormat, lpWaveFormat, dwWavSize);

	free(lpWaveFormat);



	// 2차 사운드 버퍼를 생성한다.

	if (m_lpDirectSound->CreateSoundBuffer(&dsbd, &lpDSTemp, NULL) != DS_OK)

	{

		OUT_ERRORMESSAGE("사운드 버퍼 생성 실패");

	}



	free(dsbd.lpwfxFormat);

	// 생성된버퍼에기입한다

	ret = lpDSTemp->Lock(0, dwDataSize, (void**)&lpData, &dwBSize,
		&lpWrapPtr, &dwWrapBSize, 0L);

	if (ret != DS_OK)
	{
		OUT_ERRORMESSAGE("오류");
	}

	dwDataSize = dwBSize;

	// 열린 파일의 상태(진행상황)를 알아온다

	if (mmioGetInfo(hmmioIn, &mmioinfoIn, 0) != MMSYSERR_NOERROR)

	{

		OUT_ERRORMESSAGE("오류");

	}



	DataIn = dwDataSize;

	if (DataIn > pckIn.cksize) DataIn = pckIn.cksize;

	pckIn.cksize -= DataIn;



	for (i = 0; i < DataIn; i++)
	{

		if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)

		{

			ret = mmioAdvance(hmmioIn, &mmioinfoIn, MMIO_READ);

			if (ret != MMSYSERR_NOERROR) OUT_ERRORMESSAGE("오류");

		}

		*((BYTE*)lpData + i) = *((BYTE*)mmioinfoIn.pchNext++);

	}

	if (mmioSetInfo(hmmioIn, &mmioinfoIn, 0) != MMSYSERR_NOERROR)
	{
		OUT_ERRORMESSAGE("오류");
	}

	lpDSTemp->Unlock(lpData, dwBSize, lpWrapPtr, dwWrapBSize);

	return lpDSTemp;
}

void CSoundManager::Stop(const string& key)
{
	if (IsBufIndex(key)) {
		m_mapSoundBuffers[key]->Stop();
	}
}

void CSoundManager::AllStop()
{
	for (auto iter : m_mapSoundBuffers)
	{
		iter.second->Stop();
	}
}

BOOL CSoundManager::IsPlaying(const string& key)
{
	DWORD   dwPlayCursol;
	DWORD   dwWriteCursol;

	m_mapSoundBuffers[key]->GetCurrentPosition(&dwPlayCursol, &dwWriteCursol);

	if (dwPlayCursol != 0) return TRUE;

	return FALSE;
}

void CSoundManager::Play(const string& key, BOOL Loop)
{
	if (IsBufIndex(key))
	{
		if (IsPlaying(key))
		{
			Stop(key);
		}

		LPDIRECTSOUNDBUFFER lpDS = m_mapSoundBuffers[key];

		lpDS->SetCurrentPosition(0);
		lpDS->Play(0, 0, (Loop == TRUE) ? 1 : 0);
	}
}

void CSoundManager::DuplicatePlay(const string & key, int volumePercent)
{
	if (IsBufIndex(key))
	{
		if (IsPlaying(key))
		{
			Stop(key);
		}

		LPDIRECTSOUNDBUFFER lpDS;

		long volume = DSPercentToVolume(volumePercent);

		m_lpDirectSound->DuplicateSoundBuffer(m_mapSoundBuffers[key], &lpDS);

		lpDS->SetVolume(volume);
		lpDS->SetCurrentPosition(0);

		lpDS->Play(0, 0, 0);
	}
}

void CSoundManager::DuplicateDistancePlay(const string & key, Vector3 position, int volumePercent)
{
	if (IsBufIndex(key))
	{
		if (IsPlaying(key))
		{
			Stop(key);
		}

		LPDIRECTSOUNDBUFFER lpDS;

		m_lpDirectSound->DuplicateSoundBuffer(m_mapSoundBuffers[key], &lpDS);

		lpDS->SetCurrentPosition(0);

		Vector3 listenPos = CAMERA.GetPosition();

		long pan = -10 * (listenPos.x - position.x);
		long volume = DSPercentToVolume(volumePercent);

		if (pan < DSBPAN_LEFT) pan = DSBPAN_LEFT;
		if (pan > DSBPAN_RIGHT) pan = DSBPAN_RIGHT;

		if (volume < DSBVOLUME_MIN) volume = DSBVOLUME_MIN;
		if (volume > DSBVOLUME_MAX) volume = DSBVOLUME_MAX;

		lpDS->SetPan(pan);
		lpDS->SetVolume(volume);

		lpDS->Play(0, 0, 0);
	}
}

BOOL CSoundManager::IsBufIndex(const string& key)
{
	auto find = m_mapSoundBuffers.find(key);

	if (find == m_mapSoundBuffers.end())
		return FALSE;

	return TRUE;
}

void CSoundManager::SetPan(const string& key, long pan)
{
	// DSBPAN_LEFT  ( -10000 ) 왼쪽 스피커로 재생
	// DSBPAN_RIGHT (  10000 ) 오른쪽 스피커로 재생

	if (m_lpDirectSound == NULL) return;

	if (IsBufIndex(key))
		m_mapSoundBuffers[key]->SetPan(pan);
}

void CSoundManager::SetVol(const string& key, int volPercent)
{
	if (volPercent < 0) volPercent = 0;
	if (volPercent > 100) volPercent = 100;

	if (m_lpDirectSound == NULL) return;

	volPercent = DSPercentToVolume(volPercent);

	if (IsBufIndex(key)) m_mapSoundBuffers[key]->SetVolume(volPercent);
}

int  CSoundManager::GetVol(const string & key)
{
	if (IsBufIndex(key))
	{
		long pOut;
		m_mapSoundBuffers[key]->GetVolume(&pOut);

		return DSVolumeToPercent(pOut);
	}

	return -1;
}

void CSoundManager::AddVol(const string & key, int volPercent)
{
	if (IsBufIndex(key))
	{
		long pOut;
		m_mapSoundBuffers[key]->GetVolume(&pOut);

		int per = DSVolumeToPercent(pOut) + volPercent;

		long vol = DSPercentToVolume(per);

		if (vol <= DSBVOLUME_MIN) vol = DSBVOLUME_MIN;
		if (vol >= DSBVOLUME_MAX) vol = DSBVOLUME_MAX;

		m_mapSoundBuffers[key]->SetVolume(vol);
	}
}