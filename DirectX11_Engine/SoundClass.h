//////////////////////////////////////////////////////////////////////////
// Tutorial 14
// Direct Sound�� ��ɰ� .wav ������ �ε� �� ��� ����� ĸ��ȭ

#pragma once
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <MMSystem.h>
#include <dsound.h>
#include <stdio.h>

class CSoundClass
{
private:
	// WaveHeaderType, .wav���� ������ ���� ��.
	// .wav ������ �ε��� ���� �켱 ������ ��� ������ �о� ����� �����͸� �ε��ϱ� ���� �������� �����´�.
	// ���� �ٸ� ����� ������ ����Ѵٸ� �׿� ���߾� �ٸ� ��� �������� �ٲپ�� �Ѵ�.
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	CSoundClass(void);
	CSoundClass(const CSoundClass& other);
	~CSoundClass(void);

	bool Initialize(HWND hWnd);
	void Shutdown();


	bool PlayWaveFile();
	bool PauseWaveFIle();
	bool StopWaveFile();

	char* GetPlayStatus();

private:
	bool InitializeDirectSound(HWND hWnd);
	void ShutdownDirectSound();

	bool LoadWaveFile(char* fileName, IDirectSoundBuffer8 ** _buffer);
	void ShutdownWaveFile(IDirectSoundBuffer8 ** _buffer);


	IDirectSound8 * m_directSound;
	IDirectSoundBuffer * m_primaryBuffer;
	IDirectSoundBuffer8 * m_secondaryBuffer[2];

	BOOL m_bSoundPlay;
	BOOL m_bSoundPause;
};

#endif


