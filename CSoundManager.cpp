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
	m_hWnd = DXUTGetHWND(); // �ڵ鰪�ʱ�ȭ


					  // ���̷�Ʈ ���� ��ü ����

	HRESULT dsrval = ::DirectSoundCreate(NULL, &m_lpDirectSound, NULL);

	if (dsrval != DS_OK)

	{

		OUT_ERRORMESSAGE("��ü��������");

		return;

	}

	// ���̷�Ʈ������ȯ���������Ѵ�.

	dsrval = m_lpDirectSound->SetCooperativeLevel(m_hWnd, DSSCL_PRIORITY);



	if (dsrval != DS_OK) OUT_ERRORMESSAGE("��弳������");

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

	// ������ ����

	hmmioIn = mmioOpenA((LPSTR)path.c_str(), NULL, MMIO_ALLOCBUF | MMIO_READ);

	if (!hmmioIn) OUT_ERRORMESSAGE("WAV ����������������");

	// ���� ������ RIFF WAVE �������� Ȯ���Ѵ�. 

	ckInfoRIFF.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	if (mmioDescend(hmmioIn, &ckInfoRIFF, NULL, MMIO_FINDRIFF) != 0)

	{

		OUT_ERRORMESSAGE("RIFF WAVE �����̾ƴմϴ�");

	}

	// 'fmt' Chunk ���� Ȯ���Ѵ�

	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');

	if (mmioDescend(hmmioIn, &ckInfo, &ckInfoRIFF, MMIO_FINDCHUNK) != 0)

	{

		OUT_ERRORMESSAGE("������fmt Chunk���ƴմϴ�");

	}

	// ���� ������ �д´�

	ret = mmioRead(hmmioIn, (HPSTR)&pcmWaveFormat, (long) sizeof(pcmWaveFormat));

	if (ret != (long) sizeof(pcmWaveFormat))

	{

		OUT_ERRORMESSAGE("�����б����");

	}

	// ���� ȭ���� PCM �������� Ȯ���Ѵ�

	if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)

		wExtraAlloc = 0;

	else

	{

		ret = mmioRead(hmmioIn, (LPSTR)&wExtraAlloc, (long) sizeof(wExtraAlloc));

		if (ret != (long) sizeof(wExtraAlloc))

		{

			OUT_ERRORMESSAGE("�����б����");

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

			OUT_ERRORMESSAGE("�����б����");

		}

	}



	if (mmioAscend(hmmioIn, &ckInfo, 0) != MMSYSERR_NOERROR)

	{

		OUT_ERRORMESSAGE("����");

	}





	if (mmioSeek(hmmioIn, ckInfoRIFF.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)

	{

		OUT_ERRORMESSAGE("����");

	}

	// 'data' Chunk�� ã�´�

	MMCKINFO    pckIn;

	pckIn.ckid = mmioFOURCC('d', 'a', 't', 'a');

	if (mmioDescend(hmmioIn, &pckIn, &ckInfoRIFF, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)

	{

		OUT_ERRORMESSAGE("Data Chunk�� �ƴմϴ�");

	}

	dwDataSize = pckIn.cksize;

	lpData = NULL;



	// ����ƽ 2�� ���۸� ����� ���� ����ü�� �����Ѵ�.

	// ���� ������ ����� ����, �¿�뷱��, ���ļ� ������ �����ϵ��� �Ѵ�.

	memset(&dsbd, 0, sizeof(DSBUFFERDESC));

	dsbd.dwSize = sizeof(DSBUFFERDESC);

	dsbd.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;

	dsbd.dwBufferBytes = dwDataSize;

	dwWavSize = lpWaveFormat->cbSize + sizeof(WAVEFORMATEX);



	dsbd.lpwfxFormat = (LPWAVEFORMATEX)malloc(dwWavSize);

	memcpy(dsbd.lpwfxFormat, lpWaveFormat, dwWavSize);

	free(lpWaveFormat);



	// 2�� ���� ���۸� �����Ѵ�.

	if (m_lpDirectSound->CreateSoundBuffer(&dsbd, &lpDSTemp, NULL) != DS_OK)

	{

		OUT_ERRORMESSAGE("���� ���� ���� ����");

	}



	free(dsbd.lpwfxFormat);

	// �����ȹ��ۿ������Ѵ�

	ret = lpDSTemp->Lock(0, dwDataSize, (void**)&lpData, &dwBSize,
		&lpWrapPtr, &dwWrapBSize, 0L);

	if (ret != DS_OK)
	{
		OUT_ERRORMESSAGE("����");
	}

	dwDataSize = dwBSize;

	// ���� ������ ����(�����Ȳ)�� �˾ƿ´�

	if (mmioGetInfo(hmmioIn, &mmioinfoIn, 0) != MMSYSERR_NOERROR)

	{

		OUT_ERRORMESSAGE("����");

	}



	DataIn = dwDataSize;

	if (DataIn > pckIn.cksize) DataIn = pckIn.cksize;

	pckIn.cksize -= DataIn;



	for (i = 0; i < DataIn; i++)
	{

		if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)

		{

			ret = mmioAdvance(hmmioIn, &mmioinfoIn, MMIO_READ);

			if (ret != MMSYSERR_NOERROR) OUT_ERRORMESSAGE("����");

		}

		*((BYTE*)lpData + i) = *((BYTE*)mmioinfoIn.pchNext++);

	}

	if (mmioSetInfo(hmmioIn, &mmioinfoIn, 0) != MMSYSERR_NOERROR)
	{
		OUT_ERRORMESSAGE("����");
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
	// DSBPAN_LEFT  ( -10000 ) ���� ����Ŀ�� ���
	// DSBPAN_RIGHT (  10000 ) ������ ����Ŀ�� ���

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