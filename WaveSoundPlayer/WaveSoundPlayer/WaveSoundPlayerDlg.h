
// WaveSoundPlayerDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CWaveSoundPlayerDlg ��ȭ ����
class CWaveSoundPlayerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CWaveSoundPlayerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAVESOUNDPLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
