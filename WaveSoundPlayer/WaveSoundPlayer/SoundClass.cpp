#include "stdafx.h"
#include "SoundClass.h"


// WaveFile 변환은 FlicFlac 프로그램 이용.

CSoundClass::CSoundClass(void)
{
	m_directSound = 0;
	m_primaryBuffer = 0;
	m_secondaryBuffer[0] = 0;
	m_secondaryBuffer[1] = 0;
	m_bSoundPlay = FALSE;
	m_bSoundPause = FALSE;
}

CSoundClass::CSoundClass(const CSoundClass& other)
{

}

CSoundClass::~CSoundClass(void)
{
}

bool CSoundClass::Initialize(HWND hWnd)
{
	// DirectSoundAPI와 1차 버퍼 초기화
	// Wave 파일 로드 후, 2차 버퍼 초기화
	// Wave 파일 재생

	// Initialize direct sound and the primary sound buffer.
	if(!InitializeDirectSound(hWnd))
		return false;

// 	// Load a wave audio file onto a secondary buffer.
// 	if(!LoadWaveFile("../DirectX11_Engine/data/Star.wav", &m_secondaryBuffer[0]))
// 		return false;
// 
// 	if(!LoadWaveFile("../DirectX11_Engine/data/balloon.wav", &m_secondaryBuffer[1]))
// 		return false;

	// Play the wave file now that it has been loaded.
// 	if(!PlayWaveFile())
// 		return false;

	return true;
}
bool CSoundClass::InitializeIndex(char * filePath, int index)
{
	if (!LoadWaveFile(filePath, &m_secondaryBuffer[index]))
		return false;
}

void CSoundClass::Shutdown()
{
	ShutdownWaveFile(&m_secondaryBuffer[0]);
	ShutdownWaveFile(&m_secondaryBuffer[1]);

	ShutdownDirectSound();
}

bool CSoundClass::InitializeDirectSound(HWND hWnd)
{
	// DirectSound와 1차버퍼의 인터페이스에 대한 포인터 얻기.
	// 모든 음향 장치를 조회 하여 그 중 특정 장치를 1차 버퍼로 정해 포인터를 얻을 수 있지만, 
	// 여기선 기본 음향 장치에 대한 포인터 얻음.

	HRESULT hResult = DirectSoundCreate8(NULL, &m_directSound, NULL);
	if(FAILED(hResult))
		return false;

	hResult = m_directSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if(FAILED(hResult))
		return false;

	// 1차 버퍼에 접근하기 위한 방법을 명세하기 위한 desc 작성
	// 이 구조체에서는 dwFlags가 가장 중요하다.
	// 1차 버퍼의 볼륨을 조절할 수 있도록 작성한다.

	DSBUFFERDESC bufDesc;
	bufDesc.dwSize = sizeof(DSBUFFERDESC);
	bufDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufDesc.dwBufferBytes = 0;
	bufDesc.dwReserved = 0;
	bufDesc.lpwfxFormat = NULL;
	bufDesc.guid3DAlgorithm = GUID_NULL;

	hResult = m_directSound->CreateSoundBuffer(&bufDesc, &m_primaryBuffer, NULL);
	if(FAILED(hResult))
		return false;

	// 기본 음향 장치를 1차 버퍼로 하여 제어권을 얻었다면 이 장치의 포맷을 우리의 오디오 파일 포맷으로 바꿔야 한다.
	// 고품질의 사운드 품질을 원하기 때문에 비 압축 CD 오디오 품질로 설정한다.

	WAVEFORMATEX wavForamt;
	wavForamt.wFormatTag = WAVE_FORMAT_PCM;
	wavForamt.nSamplesPerSec = 44100;
	wavForamt.wBitsPerSample = 16;
	wavForamt.nChannels = 2;
	wavForamt.nBlockAlign = (wavForamt.wBitsPerSample / 8) * wavForamt.nChannels;
	wavForamt.nAvgBytesPerSec = wavForamt.nSamplesPerSec * wavForamt.nBlockAlign;
	wavForamt.cbSize = 0;

	hResult = m_primaryBuffer->SetFormat(&wavForamt);
	if(FAILED(hResult))
		return false;

	return true;
}

void CSoundClass::ShutdownDirectSound()
{
	if(m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	if(m_directSound)
	{
		m_directSound->Release();
		m_directSound = 0;
	}
}

bool CSoundClass::LoadWaveFile(char* fileName, IDirectSoundBuffer8 ** _buffer)
{
	// .wav 파일을 열고 파일의 헤더 정보를 읽는다.
	// 헤더에는 오디오 파일의 각종 정보가 있어서 이를 이용하여 데이터를 담을 2차 버퍼를 생성 할 수 있다.
	// 오디오 파일의 헤더에는 또한 데이터가 얼마나 크고 어느 지점부터 시작하는지에 대한 정보도 있다.
	// 여기서는 모든 태그들을 읽어서 오디오 파일이 손상되지 않고 RIFF나 WAVE, fmt, data, WAVE_FORMAT_PCM들을 포함한 올바른 오디오 파일인지 확인한다.
	// 몇가지 작업을 더 하여 44.1KHz 스트레오 16bit 오디오 파일인지 확인한다. 이 파일이 22.1KHz 이거나 8bit이거나 등등 다른 경우라면 이 파일은 요구사항테 정확히 일치 하지 않아 실패 한다.

	FILE * ptrFile;
	// Open the wave file in binary
	if(fopen_s(&ptrFile, fileName, "rb") != 0)
		return false;

	WaveHeaderType waveHeder;
	// Read in the wave file header
	if(fread(&waveHeder, sizeof(waveHeder), 1, ptrFile) != 1)
		return false;

	// Check that the chunk ID is the RIFF format
	if((waveHeder.chunkId[0] != 'R') || (waveHeder.chunkId[1] != 'I') || (waveHeder.chunkId[2] != 'F') || (waveHeder.chunkId[3] != 'F') )
		return false;

	// Check that the file format is the WAVE format.
	if((waveHeder.format[0] != 'W') || (waveHeder.format[1] != 'A') || (waveHeder.format[2] != 'V') || (waveHeder.format[3] != 'E'))
		return false;

	// Check that the sub Chunk ID is the fmt format
	if((waveHeder.subChunkId[0] != 'f') || (waveHeder.subChunkId[1] != 'm') || (waveHeder.subChunkId[2] != 't') || (waveHeder.subChunkId[3] != ' '))
		return false;

	// Check that the audio format is WAVE_FORMAT_PCM.
	if(waveHeder.audioFormat != WAVE_FORMAT_PCM)
		return false;

	// Check that the wave file was recorded in streo format
	if(waveHeder.numChannels != 2)
		return false;

	// Check that the wave file was recorded at a sample rate of 44.1 KHz
	if(waveHeder.sampleRate != 44100)
		return false;

	// Ensure that the wave file was recorded in 16 bit format
	if(waveHeder.bitsPerSample != 16)
		return false;

	// Check for the data chunk header
	if((waveHeder.dataChunkId[0] != 'd') || (waveHeder.dataChunkId[1] != 'a') || (waveHeder.dataChunkId[2] != 't')|| (waveHeder.dataChunkId[3] != 'a'))
		return false;

	// 웨이브 파일 헤더 검증 완료

	// 데이터 복사할 2차 버퍼 생성
	// 1차 버퍼와 비슷하게 웨이브 포맷을 설정하고 버퍼의 desc을 작성한다.
	// 1차 버퍼가 아니기 떄문에 dwFlag나 dwBufferBytes는 그 값이 조금 다르다.

	// set the wave format of secondary buffer that this wave file will be loaded onto.
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	DSBUFFERDESC bufDesc;
	bufDesc.dwSize = sizeof(DSBUFFERDESC);
	bufDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufDesc.dwBufferBytes = waveHeder.dataSize;
	bufDesc.dwReserved = 0;
	bufDesc.lpwfxFormat = &waveFormat;
	bufDesc.guid3DAlgorithm = GUID_NULL;

	// 2차 버퍼를 생성하는 방법
	// 버퍼의 DESC를 가지고 임시적으로 IDirectSoundBuffer객체를 만든다. 
	// QueryInterface함수를 IID_IDirectSoundBuffer8인자와 함꼐 호출하여, 임시 버퍼로 IDirectSoundBuffer8의 2차 버퍼를 만들게 된다.
	// 임시 버퍼를 해제하고 2차 버퍼를 사용한다.

	//Create a temporary sound buffer with the specific buffer settings.
	IDirectSoundBuffer * tempBuf;
	HRESULT hResult = m_directSound->CreateSoundBuffer(&bufDesc, &tempBuf, NULL);
	if(FAILED(hResult))
		return false;

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	hResult = tempBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*_buffer);
	if(FAILED(hResult))
		return false;

	tempBuf->Release();
	tempBuf = 0;

	// 2차 버퍼가 준비 되었다면 오디오 파일의 웨이브 데이터를 가져온다.
	// 메모리 버퍼에 불러들여서 필요하다면 적절히 수정을 한다.
	// 그 뒤 2차 버퍼를 잠궈 접근 권한을 취득한 뒤, memcpy를 사용하여 데이터를 복사하고 잠금을 해제 한다.
	// 2차 버퍼 사용할 준비 완료.
	// 기억해 둘것. 2차 버퍼를 잠글 때 두개의 포인터를 받아 쓴다는 것. 버퍼가 원형으로 구성 되어 있기 때문에 중간부터 버퍼를 쓰게 되는 경우 그 지점에서의 버퍼 사이즈를 아는 것이 필요하기 때문.
	// 그래야 버퍼의 범위를 넘어가지 않게 쓰기 작업을 할 수 있다.
	// 이런 구조는 오디오를 재생하는 것과 같은 일에 유용하다.
	// 오디오 파일과 같은 크기의 버퍼를 만들고 버퍼의 시작부터 파일을 기록하게 한다.

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(ptrFile, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	unsigned char * wavBuf = new unsigned char[waveHeder.dataSize];
	if(!wavBuf)
		return false;

	// Read in the wave file data into the newly created buffer.
	if(fread(wavBuf, 1, waveHeder.dataSize, ptrFile) != waveHeder.dataSize)
		return false;

	// Close the file once done reading
	if(fclose(ptrFile) != 0)
		return false;

	// Lock the secondary buffer to write wave data into it.
	unsigned char * ptrBuf;
	unsigned long sizeBuf;
	hResult = (*_buffer)->Lock(0, waveHeder.dataSize, (void**)&ptrBuf, (DWORD*)&sizeBuf, NULL , 0, 0);
	if(FAILED(hResult))
		return false;

	// Copy the wave data into the buffer.
	memcpy(ptrBuf, wavBuf, waveHeder.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	hResult = (*_buffer)->Unlock((void*)ptrBuf, sizeBuf, NULL, 0);
	if(FAILED(hResult))
		return false;

	// Release the wave data since it was copied into the secondary buffer
	delete [] wavBuf;
	wavBuf = 0;
	

	return true;
}

void CSoundClass::ShutdownWaveFile(IDirectSoundBuffer8 ** _buffer)
{
	if(*_buffer)
	{
		(*_buffer)->Release();
		*_buffer = 0;
	}
}

bool CSoundClass::PlayWaveFile(int index)
{
	// 2차 버퍼에 저장되어 있는 오디오 파일을 재생한다.
	// Play함수에서 1차 버퍼의 오디오와의 믹싱을 자동으로 수행한 뒤 재생한다.
	// 2차 버퍼의 시작 부분으로 시작 위치를 지정(-> 안하면 최근에 재생되었던 부분부터 시작하게 된다.
	// 음량 조절

	if(m_bSoundPlay)
	{
		OutputDebugStringA("파일 실행 중");
		return false;
	}

	HRESULT hResult;

	if(!m_bSoundPause)
	{
		// 일시정지 상태가 아니고 새로 시작하는 경우에만 포커스 이동.
		hResult = m_secondaryBuffer[index]->SetCurrentPosition(0);
		if(FAILED(hResult))
			return false;
	}


	hResult = m_secondaryBuffer[index]->SetVolume(DSBVOLUME_MAX);
	if(FAILED(hResult))
		return false;

	hResult = m_secondaryBuffer[index]->Play(0, 0, DSBPLAY_LOOPING);
	if(FAILED(hResult))
		return false;


	m_bSoundPlay = TRUE;
	m_bSoundPause = FALSE;

	return true;
}

bool CSoundClass::PauseWaveFIle(int index)
{
	if(!m_bSoundPlay)
	{
		OutputDebugStringA("실행 중이 아님");
		return false;
	}

	if(m_bSoundPause)
	{
		OutputDebugStringA("이미 일시정지 중");
		return false;
	}
	
	if(!StopWaveFile(index))
		return false;

	m_bSoundPause = TRUE;

	return true;
}

bool CSoundClass::StopWaveFile(int index)
{

	if (m_bSoundPause)
	{
		// 정지 된 상태이지만 완전 종료 예정
		m_bSoundPause = FALSE;
	}

	if(!m_bSoundPlay)
	{
		OutputDebugStringA("실행중인 파일 없음");
		return false;
	}

	HRESULT hResult = m_secondaryBuffer[index]->Stop();
	if(FAILED(hResult))
		return false;

	m_bSoundPlay = FALSE;

	return true;
}

WCHAR * CSoundClass::GetPlayStatus()
{
	if(m_bSoundPlay)
		return L"Play";

	if(m_bSoundPause)
		return L"Pause";

	return L"Stop";
}