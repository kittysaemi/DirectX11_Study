//////////////////////////////////////////////////////////////////////////
// Tutorial 14
// Direct Sound의 기능과 .wav 파일의 로드 및 재생 기능을 캡슐화

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
	// WaveHeaderType, .wav파일 포맷을 위한 것.
	// .wav 파일을 로드할 때는 우선 파일의 헤더 정보를 읽어 오디오 데이터를 로드하기 위한 정보들을 가져온다.
	// 만약 다른 오디오 포맷을 사용한다면 그에 맞추어 다른 헤더 형식으로 바꾸어야 한다.
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


