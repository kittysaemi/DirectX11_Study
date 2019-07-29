
// WaveSoundPlayerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "WaveSoundPlayer.h"
#include "WaveSoundPlayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaveSoundPlayerDlg ��ȭ ����



CWaveSoundPlayerDlg::CWaveSoundPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WAVESOUNDPLAYER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nSelFileIdx = -1;
}

void CWaveSoundPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAY_FILE_LIST, m_playerList);
}

BEGIN_MESSAGE_MAP(CWaveSoundPlayerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_PLAY, &CWaveSoundPlayerDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CWaveSoundPlayerDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_STOP, &CWaveSoundPlayerDlg::OnBnClickedBtnStop)
	ON_LBN_DBLCLK(IDC_PLAY_FILE_LIST, &CWaveSoundPlayerDlg::OnLbnDblclkPlayFileList)
END_MESSAGE_MAP()


// CWaveSoundPlayerDlg �޽��� ó����

BOOL CWaveSoundPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.4


	m_playerList.AddString(L"balloon.wav");
	m_playerList.AddString(L"Star.wav");



	m_pInput = new CInputClass;
	if (!m_pInput)
		return FALSE;

	SIZE s;
	ZeroMemory(&s, sizeof(SIZE));
	s.cx = (LONG)::GetSystemMetrics(SM_CXFULLSCREEN);
	s.cy = (LONG)::GetSystemMetrics(SM_CYFULLSCREEN);

	if (!m_pInput->Initialize(AfxGetInstanceHandle(), GetSafeHwnd(), s.cx, s.cy))
		return FALSE;

	m_pSound = new CSoundClass;
	if (!m_pSound)
		return FALSE;

	if (!m_pSound->Initialize(GetSafeHwnd()))
		return FALSE;

	// �̸� Init
	m_pSound->InitializeIndex("../WaveSoundPlayer/SoundFileList/balloon.wav", 0);
	m_pSound->InitializeIndex("../WaveSoundPlayer/SoundFileList/Star.wav", 1);



	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CWaveSoundPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	OutputDebugStringA("Print");
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CWaveSoundPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CWaveSoundPlayerDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	KillTimer(100);

	if (m_pSound)
	{
		m_pSound->Shutdown();
		delete m_pSound;
		m_pSound = 0;
	}

	if (m_pInput)
	{
		m_pInput->Shutdown();
		delete m_pInput;
		m_pInput = 0;
	}

	return CDialogEx::DestroyWindow();
}


void CWaveSoundPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{



	CDialogEx::OnTimer(nIDEvent);
}


void CWaveSoundPlayerDlg::OnBnClickedBtnPlay()
{
	m_pSound->PlayWaveFile(m_nSelFileIdx);

	SoundStatus();
}


void CWaveSoundPlayerDlg::OnBnClickedBtnPause()
{
	m_pSound->PauseWaveFIle(m_nSelFileIdx);

	SoundStatus();
}


void CWaveSoundPlayerDlg::OnBnClickedBtnStop()
{
	m_pSound->StopWaveFile(m_nSelFileIdx);

	SoundStatus();
}


void CWaveSoundPlayerDlg::OnLbnDblclkPlayFileList()
{
	CString selStr;

	m_pSound->StopWaveFile(m_nSelFileIdx);
	SoundStatus();
	
	m_nSelFileIdx = m_playerList.GetCurSel();
	m_playerList.GetText(m_nSelFileIdx, selStr);

	CString Msg;
	Msg.Format(L"select Sound File : %s", selStr);

	GetDlgItem(IDC_SELECT_FILE)->SetWindowTextW(Msg);
}

void CWaveSoundPlayerDlg::SoundStatus()
{
	CString selStr;

	CString Msg;
	Msg.Format(L"Play Status : %s", m_pSound->GetPlayStatus());

	GetDlgItem(IDC_PLAYSTATUSMSG)->SetWindowTextW(Msg);

}