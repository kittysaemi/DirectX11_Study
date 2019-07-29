
// WaveSoundPlayerDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CWaveSoundPlayerDlg 대화 상자
class CWaveSoundPlayerDlg : public CDialogEx
{
// 생성입니다.
public:
	CWaveSoundPlayerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAVESOUNDPLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


private:
	CInputClass * m_pInput;
	CSoundClass * m_pSound;

	int	m_nSelFileIdx;

	void SoundStatus();

public:
	virtual BOOL DestroyWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListBox m_playerList;
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnLbnDblclkPlayFileList();
};
