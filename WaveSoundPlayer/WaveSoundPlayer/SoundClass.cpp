#include "stdafx.h"
#include "SoundClass.h"


// WaveFile ��ȯ�� FlicFlac ���α׷� �̿�.

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
	// DirectSoundAPI�� 1�� ���� �ʱ�ȭ
	// Wave ���� �ε� ��, 2�� ���� �ʱ�ȭ
	// Wave ���� ���

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
	// DirectSound�� 1�������� �������̽��� ���� ������ ���.
	// ��� ���� ��ġ�� ��ȸ �Ͽ� �� �� Ư�� ��ġ�� 1�� ���۷� ���� �����͸� ���� �� ������, 
	// ���⼱ �⺻ ���� ��ġ�� ���� ������ ����.

	HRESULT hResult = DirectSoundCreate8(NULL, &m_directSound, NULL);
	if(FAILED(hResult))
		return false;

	hResult = m_directSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if(FAILED(hResult))
		return false;

	// 1�� ���ۿ� �����ϱ� ���� ����� ���ϱ� ���� desc �ۼ�
	// �� ����ü������ dwFlags�� ���� �߿��ϴ�.
	// 1�� ������ ������ ������ �� �ֵ��� �ۼ��Ѵ�.

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

	// �⺻ ���� ��ġ�� 1�� ���۷� �Ͽ� ������� ����ٸ� �� ��ġ�� ������ �츮�� ����� ���� �������� �ٲ�� �Ѵ�.
	// ��ǰ���� ���� ǰ���� ���ϱ� ������ �� ���� CD ����� ǰ���� �����Ѵ�.

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
	// .wav ������ ���� ������ ��� ������ �д´�.
	// ������� ����� ������ ���� ������ �־ �̸� �̿��Ͽ� �����͸� ���� 2�� ���۸� ���� �� �� �ִ�.
	// ����� ������ ������� ���� �����Ͱ� �󸶳� ũ�� ��� �������� �����ϴ����� ���� ������ �ִ�.
	// ���⼭�� ��� �±׵��� �о ����� ������ �ջ���� �ʰ� RIFF�� WAVE, fmt, data, WAVE_FORMAT_PCM���� ������ �ùٸ� ����� �������� Ȯ���Ѵ�.
	// ��� �۾��� �� �Ͽ� 44.1KHz ��Ʈ���� 16bit ����� �������� Ȯ���Ѵ�. �� ������ 22.1KHz �̰ų� 8bit�̰ų� ��� �ٸ� ����� �� ������ �䱸������ ��Ȯ�� ��ġ ���� �ʾ� ���� �Ѵ�.

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

	// ���̺� ���� ��� ���� �Ϸ�

	// ������ ������ 2�� ���� ����
	// 1�� ���ۿ� ����ϰ� ���̺� ������ �����ϰ� ������ desc�� �ۼ��Ѵ�.
	// 1�� ���۰� �ƴϱ� ������ dwFlag�� dwBufferBytes�� �� ���� ���� �ٸ���.

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

	// 2�� ���۸� �����ϴ� ���
	// ������ DESC�� ������ �ӽ������� IDirectSoundBuffer��ü�� �����. 
	// QueryInterface�Լ��� IID_IDirectSoundBuffer8���ڿ� �Բ� ȣ���Ͽ�, �ӽ� ���۷� IDirectSoundBuffer8�� 2�� ���۸� ����� �ȴ�.
	// �ӽ� ���۸� �����ϰ� 2�� ���۸� ����Ѵ�.

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

	// 2�� ���۰� �غ� �Ǿ��ٸ� ����� ������ ���̺� �����͸� �����´�.
	// �޸� ���ۿ� �ҷ��鿩�� �ʿ��ϴٸ� ������ ������ �Ѵ�.
	// �� �� 2�� ���۸� ��� ���� ������ ����� ��, memcpy�� ����Ͽ� �����͸� �����ϰ� ����� ���� �Ѵ�.
	// 2�� ���� ����� �غ� �Ϸ�.
	// ����� �Ѱ�. 2�� ���۸� ��� �� �ΰ��� �����͸� �޾� ���ٴ� ��. ���۰� �������� ���� �Ǿ� �ֱ� ������ �߰����� ���۸� ���� �Ǵ� ��� �� ���������� ���� ����� �ƴ� ���� �ʿ��ϱ� ����.
	// �׷��� ������ ������ �Ѿ�� �ʰ� ���� �۾��� �� �� �ִ�.
	// �̷� ������ ������� ����ϴ� �Ͱ� ���� �Ͽ� �����ϴ�.
	// ����� ���ϰ� ���� ũ���� ���۸� ����� ������ ���ۺ��� ������ ����ϰ� �Ѵ�.

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
	// 2�� ���ۿ� ����Ǿ� �ִ� ����� ������ ����Ѵ�.
	// Play�Լ����� 1�� ������ ��������� �ͽ��� �ڵ����� ������ �� ����Ѵ�.
	// 2�� ������ ���� �κ����� ���� ��ġ�� ����(-> ���ϸ� �ֱٿ� ����Ǿ��� �κк��� �����ϰ� �ȴ�.
	// ���� ����

	if(m_bSoundPlay)
	{
		OutputDebugStringA("���� ���� ��");
		return false;
	}

	HRESULT hResult;

	if(!m_bSoundPause)
	{
		// �Ͻ����� ���°� �ƴϰ� ���� �����ϴ� ��쿡�� ��Ŀ�� �̵�.
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
		OutputDebugStringA("���� ���� �ƴ�");
		return false;
	}

	if(m_bSoundPause)
	{
		OutputDebugStringA("�̹� �Ͻ����� ��");
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
		// ���� �� ���������� ���� ���� ����
		m_bSoundPause = FALSE;
	}

	if(!m_bSoundPlay)
	{
		OutputDebugStringA("�������� ���� ����");
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