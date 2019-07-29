//////////////////////////////////////////////////////////////////////////
// Tutorial 12
// ȭ�鿡 �׸� ���ڿ��鿡 �ش��ϴ� ����/�ε��� ���۵��� ������.
// ���ø����̼��� �ʿ�� �ϴ� ��� 2D���ڿ��� �׸��� ���� ����Ѵ�.

#pragma once
#ifndef _TEXT_H_
#define _TEXT_H_

#include "FontClass.h"
#include "FontShader.h"

class CText
{
private:
	struct SentenceType
	{
		ID3D11Buffer* pVertBuf;
		ID3D11Buffer* pIdxBuf;
		int nVertCount;
		int nIdxCount;
		int nMaxLen;
		float nfRed;
		float nfGreen;
		float nfBlue;
	};
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	struct TextPoint
	{
		int sX;
		int sY;
		int sW;
		int sH;
	};
	struct FontColor
	{
		float red;
		float green;
		float blue;
	};

public:
	CText(void);
	CText(const CText& other);
	~CText(void);

	bool Initialize(ID3D11Device * _device, ID3D11DeviceContext * _devContext, HWND _hWnd, TextPoint _screenWH, D3DXMATRIX _baseViewMatrix);
	void Shutdown();
	bool Render(ID3D11DeviceContext * _devContext, D3DXMATRIX _worldMatrix, D3DXMATRIX _orthoMatrix);

	bool SetMousePosition(int posX, int posY, ID3D11DeviceContext* devContext);
	bool SetKeyBoardInputData(ID3D11DeviceContext* devContext, char * data);
	bool SetPlayStatusMsg(ID3D11DeviceContext* devContext, char * data);

private:
	bool InitializeSentence(SentenceType** _sentenceData, int _maxLen, ID3D11Device* _device);
	bool UpdateSentence(SentenceType* _sentence, char* _strText, int _posX, int _posY, FontColor _fColor, ID3D11DeviceContext* _devContext);
	void ReleaseSentence(SentenceType** _sentence);
	bool RenderSetence(ID3D11DeviceContext* _devContext, SentenceType * _sentence, D3DXMATRIX _worldMatrix, D3DXMATRIX _orthoMatrix);

	CFontClass * m_pFont;
	CFontShader * m_pFontShader;
	TextPoint m_nScreenWH;
	D3DXMATRIX m_baseViewMatrix;
	SentenceType * m_sentence[10];

	SentenceType * m_MouseMoveSetence;

	TextPoint m_nKeyBoardStart;
	SentenceType * m_KeyboadSetence;
	char m_sKeyBoardInputData[256];


	TextPoint m_nPlayStatusFontPoint;
	FontColor	m_PlayStatusFontColor;
	SentenceType * m_SoundSetence;
};

#endif


