
// MFCDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "MFC.h"
#include "MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDlg-Dialogfeld



CMFCDlg::CMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDE_LAUT, lautst);
	DDX_Control(pDX, IDC_SLIDE_BALANCE, balance);
	DDX_Control(pDX, IDC_GITARRE, gitarre);
}

BEGIN_MESSAGE_MAP(CMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ENDE, &CMFCDlg::OnBnClickedEnde)
	ON_BN_CLICKED(IDC_TEST264, &CMFCDlg::OnBnClickedTest264)
	ON_BN_CLICKED(IDC_CDUR_TON, &CMFCDlg::OnBnClickedCdurTon)
	ON_BN_CLICKED(IDC_PCM, &CMFCDlg::OnBnClickedPcm)
	ON_BN_CLICKED(IDC_CDUR_DREI, &CMFCDlg::OnBnClickedCdurDrei)
	ON_BN_CLICKED(IDC_C, &CMFCDlg::OnBnClickedC)
	ON_BN_CLICKED(IDC_D, &CMFCDlg::OnBnClickedD)
	ON_BN_CLICKED(IDC_E, &CMFCDlg::OnBnClickedE)
	ON_BN_CLICKED(IDC_F, &CMFCDlg::OnBnClickedF)
	ON_BN_CLICKED(IDC_G, &CMFCDlg::OnBnClickedG)
	ON_BN_CLICKED(IDC_A, &CMFCDlg::OnBnClickedA)
	ON_BN_CLICKED(IDC_H, &CMFCDlg::OnBnClickedH)
	ON_BN_CLICKED(IDC_C2, &CMFCDlg::OnBnClickedC2)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDE_LAUT, &CMFCDlg::OnNMCustomdrawSlideLaut)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDE_BALANCE, &CMFCDlg::OnNMCustomdrawSlideBalance)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_GITARRE, &CMFCDlg::OnBnClickedGitarre)
END_MESSAGE_MAP()


// CMFCDlg-Meldungshandler

BOOL CMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Symbol für dieses Dialogfeld festlegen.  Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	// TODO: Hier zusätzliche Initialisierung einfügen
	if (!m_ds.Create(this))
		OnCancel();

	// create a 4 second sound buffer
	if ((lpDSBSecondary = m_ds.CreateSoundBuffer(2, 16, 22050, 4)) == 0)
		OnCancel();

	if ((lpDSB_PCM = m_ds.CreateSoundBuffer(2, 16, 22050, 4)) == 0)
		OnCancel();

	void *lpvPtr1, *lpvPtr2; DWORD dwBytes1, dwBytes2;
	if (!m_ds.LockBuffer(lpDSBSecondary, 0, 2, // we use the first 2 seconds
		&lpvPtr1, // get pointer 1
		&dwBytes1, // get bytes available there
		&lpvPtr2, // get pointer 2 (the buffer is circular)
		&dwBytes2)) // get bytes available there
		return false;
	// write a sinus sound now (88040/63 = 1397 Hz)
	DWORD i; short int *t; // points to 16 Bit
	for (i = 0, t = (short int*)lpvPtr1; i<(dwBytes1 + dwBytes2); i += 4, t += 2) {
		if (i == dwBytes1) t = (short int*)lpvPtr2;
		// 2 channels with 16 Bit each
		*t = *(t + 1) = (short int)(sin(i / 10.0) * 30000);
	}
	if (!m_ds.UnlockBuffer(lpDSBSecondary,
		lpvPtr1, // pointer 1
		dwBytes1, // bytes written there
		lpvPtr2, // pointer 2
		dwBytes2)) // bytes written there
		return false;

	lautst.SetRange(0, 2000);
	lautst.SetPos(2000);

	balance.SetRange(0, 8000);
	balance.SetPos(4000);


	
	// init major scale C
	int c = 264; //Hz
	// ton[0] = c;			//C
	// ton[1] = c * 9 / 8.;	//D
	// ton[2] = c * 5 / 4.;	//E
	// ton[3] = c * 4 / 3.;	//F
	// ton[4] = c * 3 / 2.;	//G
	// ton[5] = c * 5 / 3.;	//A
	// ton[6] = c * 15 / 8.;//H
	// ton[7] = c * 2;		//C2
	// ton[8] = 0;
	// oder temperierte Stimmung
	ton[0] = c;
	ton[1] = c*pow(pow(2,1/12.),2);
	ton[2] = c*pow(pow(2,1/12.),4);
	ton[3] = c*pow(pow(2,1/12.),5);
	ton[4] = c*pow(pow(2,1/12.),7);
	ton[5] = c*pow(pow(2,1/12.),9);
	ton[6] = c*pow(pow(2,1/12.),10);
	ton[7] = c*pow(pow(2,1/12.),12);
	ton[8] = 0;

	if ((lpDSB_C = m_ds.CreateSoundBuffer(2, 16, 22050, 4)) == 0)
		OnCancel();
	if (!m_ds.SetPlaybackVolume(lpDSB_C, -920))
		OnCancel();

	if ((lpDSB_D = m_ds.CreateSoundBuffer(2, 16, 22050, 4)) == 0)
		OnCancel();
	if (!m_ds.SetPlaybackVolume(lpDSB_D, -920))
		OnCancel();

	if ((lpDSB_E = m_ds.CreateSoundBuffer(2, 16, 22050, 4)) == 0)
		OnCancel();
	if (!m_ds.SetPlaybackVolume(lpDSB_E, -920))
		OnCancel();

	if ((lpDSB_F = m_ds.CreateSoundBuffer(2, 16, 22050, 4)) == 0)
		OnCancel();
	if (!m_ds.SetPlaybackVolume(lpDSB_F, -920))
		OnCancel();

	if ((lpDSB_G = m_ds.CreateSoundBuffer(2, 16, 22050, 4)) == 0)
		OnCancel();
	if (!m_ds.SetPlaybackVolume(lpDSB_G, -920))
		OnCancel();

	if ((lpDSB_A = m_ds.CreateSoundBuffer(2, 16, 22050, 4)) == 0)
		OnCancel();
	if (!m_ds.SetPlaybackVolume(lpDSB_A, -920))
		OnCancel();

	if ((lpDSB_H = m_ds.CreateSoundBuffer(2, 16, 22050, 4)) == 0)
		OnCancel();
	if (!m_ds.SetPlaybackVolume(lpDSB_H, -920))
		OnCancel();

	if ((lpDSB_C2 = m_ds.CreateSoundBuffer(2, 16, 22050, 4)) == 0)
		OnCancel();
	if (!m_ds.SetPlaybackVolume(lpDSB_C2, -920))
		OnCancel();

	m_ds.GenerateSound(lpDSB_C, 0, 1, ton[0]);
	m_ds.GenerateSound(lpDSB_D, 0, 1, ton[1]);
	m_ds.GenerateSound(lpDSB_E, 0, 1, ton[2]);
	m_ds.GenerateSound(lpDSB_F, 0, 1, ton[3]);
	m_ds.GenerateSound(lpDSB_G, 0, 1, ton[4]);
	m_ds.GenerateSound(lpDSB_A, 0, 1, ton[5]);
	m_ds.GenerateSound(lpDSB_H, 0, 1, ton[6]);
	m_ds.GenerateSound(lpDSB_C2, 0, 1, ton[7]);

	// PCM
	if ((fp = fopen("Sound_22050_stereo_16Bit.pcm", "rb")) == NULL) {
		AfxMessageBox(L"err open file");
		OnCancel();
	}

	return TRUE;  // TRUE zurückgeben, wenn der Fokus nicht auf ein Steuerelement gesetzt wird
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie
//  den nachstehenden Code, um das Symbol zu zeichnen.  Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCDlg::OnBnClickedEnde()
{
	exit(0);
}


void CMFCDlg::OnBnClickedTest264()
{
	m_ds.GenerateSound(lpDSBSecondary, 0, 4, 264);
	if (!m_ds.Play(lpDSBSecondary, false))
		OnCancel();
}


void CMFCDlg::OnBnClickedCdurTon()
{
	m_ds.GenerateSound(lpDSBSecondary, 0, 2, ton[0]);
	if (!m_ds.Play(lpDSBSecondary, true))
		OnCancel();
	SetTimer(1, 700, NULL);
}


void CMFCDlg::OnBnClickedPcm()		// PCM
{
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	if (fp != NULL) {
		m_ds.GeneratePCMSound(lpDSB_PCM, 0, 2, fp);
		if (!m_ds.Play(lpDSB_PCM, true))
			OnCancel();
		SetTimer(2, 700, NULL);
	}
	else {
		AfxMessageBox(L"ERR");
	}
}


void CMFCDlg::OnBnClickedCdurDrei()	//dreiklang
{
	m_ds.GenerateSound(lpDSB_C, 0, 2, ton[0]);
	if (!m_ds.Play(lpDSB_C, false))
		OnCancel();
	m_ds.GenerateSound(lpDSB_E, 0, 2, ton[2]);
	if (!m_ds.Play(lpDSB_E, false))
		OnCancel();
	m_ds.GenerateSound(lpDSB_G, 0, 2, ton[4]);
	if (!m_ds.Play(lpDSB_G, false))
		OnCancel();
}


void CMFCDlg::OnBnClickedC()
{
	/*if (gitarre.GetCheck()) {
		fpg = fopen("C.RAW", "rb");
		m_ds.GeneratePCMSound(lpDSB_C, 0, 2, fpg);
		fclose(fpg);
	}
	else {
		m_ds.GenerateSound(lpDSB_C, 0, 1, ton[0]);
	}*/
	if (!m_ds.Play(lpDSB_C, false))
		OnCancel();
}


void CMFCDlg::OnBnClickedD()
{
	if (!m_ds.Play(lpDSB_D, false))
		OnCancel();
}


void CMFCDlg::OnBnClickedE()
{
	if (!m_ds.Play(lpDSB_E, false))
		OnCancel();
}


void CMFCDlg::OnBnClickedF()
{
	if (!m_ds.Play(lpDSB_F, false))
		OnCancel();
}


void CMFCDlg::OnBnClickedG()
{
	if (!m_ds.Play(lpDSB_G, false))
		OnCancel();
}


void CMFCDlg::OnBnClickedA()
{
	if (!m_ds.Play(lpDSB_A, false))
		OnCancel();
}


void CMFCDlg::OnBnClickedH()
{
	if (!m_ds.Play(lpDSB_H, false))
		OnCancel();
}


void CMFCDlg::OnBnClickedC2()
{
	if (!m_ds.Play(lpDSB_C2, false))
		OnCancel();
}


void CMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) {
		static int j = 0, buffnr = 1, playpos;
		if ((playpos = m_ds.GetPlayPosition(lpDSBSecondary)) == -1) {
			KillTimer(1); return;
		}
		if (((playpos > 50) && (buffnr == 0)) || ((playpos < 50) && (buffnr == 1))) {
			if ((++j) == 9) { // major scale finished
				KillTimer(1);
				j = 0;
				if (!m_ds.Stop(lpDSBSecondary))
					return;
				return;
			}
			m_ds.GenerateSound(lpDSBSecondary, buffnr * 2, 2, ton[j]);
			if (buffnr == 1) buffnr = 0; // change buffer
			else buffnr = 1;
		}
	}

	if (nIDEvent == 2) {
		static boolean stop = false, buffnr = 1, playpos;
		if ((playpos = m_ds.GetPlayPosition(lpDSB_PCM)) == -1) {
			KillTimer(2); return;
		}
		if (stop == true) {
			stop = false;
			m_ds.Stop(lpDSB_PCM);
			KillTimer(2);
		} else if (((playpos > 50) && (buffnr == 0)) || ((playpos < 50) && (buffnr == 1))) {
			int ende = m_ds.GeneratePCMSound(lpDSB_PCM, buffnr * 2, 2, fp);
			if (ende == 2)
				stop = true;
			if (buffnr == 1) buffnr = 0; // change buffer
			else buffnr = 1;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMFCDlg::OnNMCustomdrawSlideLaut(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	*pResult = 0;
}


void CMFCDlg::OnNMCustomdrawSlideBalance(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	*pResult = 0;
}


void CMFCDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein, und/oder benutzen Sie den Standard.
	CSliderCtrl * lt = (CSliderCtrl*)GetDlgItem(IDC_SLIDE_LAUT);
	int laut = lt->GetPos()-2000;
	m_ds.SetPlaybackVolume(lpDSBSecondary, laut);
	m_ds.SetPlaybackVolume(lpDSB_C, laut);
	m_ds.SetPlaybackVolume(lpDSB_D, laut);
	m_ds.SetPlaybackVolume(lpDSB_E, laut);
	m_ds.SetPlaybackVolume(lpDSB_F, laut);
	m_ds.SetPlaybackVolume(lpDSB_G, laut);
	m_ds.SetPlaybackVolume(lpDSB_A, laut);
	m_ds.SetPlaybackVolume(lpDSB_H, laut);
	m_ds.SetPlaybackVolume(lpDSB_C2, laut);
	m_ds.SetPlaybackVolume(lpDSB_PCM, laut);

	CSliderCtrl * bl = (CSliderCtrl*)GetDlgItem(IDC_SLIDE_BALANCE);
	int bal = bl->GetPos()-8000;
	m_ds.SetBalance(lpDSBSecondary, bal);
	m_ds.SetBalance(lpDSB_C, bal);
	m_ds.SetBalance(lpDSB_D, bal);
	m_ds.SetBalance(lpDSB_E, bal);
	m_ds.SetBalance(lpDSB_F, bal);
	m_ds.SetBalance(lpDSB_G, bal);
	m_ds.SetBalance(lpDSB_A, bal);
	m_ds.SetBalance(lpDSB_H, bal);
	m_ds.SetBalance(lpDSB_C2, bal);
	m_ds.SetBalance(lpDSB_PCM, bal);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CMFCDlg::OnBnClickedGitarre()
{
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	if (gitarre.GetCheck()) {
		fpg = fopen("C.RAW", "rb");
		m_ds.GeneratePCMSound(lpDSB_C, 0, 2, fpg);
		fclose(fpg);

		fpg = fopen("D.RAW", "rb");
		m_ds.GeneratePCMSound(lpDSB_D, 0, 2, fpg);
		fclose(fpg);

		fpg = fopen("E.RAW", "rb");
		m_ds.GeneratePCMSound(lpDSB_E, 0, 2, fpg);
		fclose(fpg);

		fpg = fopen("F.RAW", "rb");
		m_ds.GeneratePCMSound(lpDSB_F, 0, 2, fpg);
		fclose(fpg);

		fpg = fopen("G.RAW", "rb");
		m_ds.GeneratePCMSound(lpDSB_G, 0, 2, fpg);
		fclose(fpg);

		fpg = fopen("H.RAW", "rb");
		m_ds.GeneratePCMSound(lpDSB_H, 0, 2, fpg);
		fclose(fpg);

		fpg = fopen("A.RAW", "rb");
		m_ds.GeneratePCMSound(lpDSB_A, 0, 2, fpg);
		fclose(fpg);

		fpg = fopen("C_Hoch.RAW", "rb");
		m_ds.GeneratePCMSound(lpDSB_C2, 0, 2, fpg);
		fclose(fpg);
	} else {
		m_ds.GenerateSound(lpDSB_C, 0, 1, ton[0]);
		m_ds.GenerateSound(lpDSB_D, 0, 1, ton[1]);
		m_ds.GenerateSound(lpDSB_E, 0, 1, ton[2]);
		m_ds.GenerateSound(lpDSB_F, 0, 1, ton[3]);
		m_ds.GenerateSound(lpDSB_G, 0, 1, ton[4]);
		m_ds.GenerateSound(lpDSB_A, 0, 1, ton[5]);
		m_ds.GenerateSound(lpDSB_H, 0, 1, ton[6]);
		m_ds.GenerateSound(lpDSB_C2, 0, 1, ton[7]);
	}
}

