
// WaveSoundPlayerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "WaveSoundPlayer.h"
#include "WaveSoundPlayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaveSoundPlayerDlg 대화 상자



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


// CWaveSoundPlayerDlg 메시지 처리기

BOOL CWaveSoundPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.4


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

	// 미리 Init
	m_pSound->InitializeIndex("../WaveSoundPlayer/SoundFileList/balloon.wav", 0);
	m_pSound->InitializeIndex("../WaveSoundPlayer/SoundFileList/Star.wav", 1);



	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CWaveSoundPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	OutputDebugStringA("Print");
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CWaveSoundPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CWaveSoundPlayerDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
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