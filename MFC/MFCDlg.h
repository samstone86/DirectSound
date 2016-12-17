
// MFCDlg.h: Headerdatei
//

#pragma once
#include "DirectSound.h"
#include "afxcmn.h"
#include "afxwin.h"

// CMFCDlg-Dialogfeld
class CMFCDlg : public CDialogEx
{
// Konstruktion
public:
	CMFCDlg(CWnd* pParent = NULL);	// Standardkonstruktor
	
	CDirectSound m_ds;
	LPDIRECTSOUNDBUFFER lpDSBSecondary;
	LPDIRECTSOUNDBUFFER lpDSB_C, lpDSB_D, lpDSB_E, lpDSB_F, lpDSB_G, lpDSB_A, lpDSB_H, lpDSB_C2;
	LPDIRECTSOUNDBUFFER lpDSB_PCM;
	double ton[9];
	FILE *fp, *fpg;

	typedef struct waveformat_extended_tag {
		WORD wFormatTag;
		WORD nChannels;
		DWORD nSamplesPerSec;
		DWORD nAvgBytesPerSec;
		WORD nBlockAlign;
		WORD wBitsPerSample;
		WORD cbSize;
	} WAVEFORMATEX;

// Dialogfelddaten
	enum { IDD = IDD_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEnde();
	afx_msg void OnBnClickedTest264();
	afx_msg void OnBnClickedCdurTon();
	afx_msg void OnBnClickedPcm();
	afx_msg void OnBnClickedCdurDrei();
	afx_msg void OnBnClickedC();
	afx_msg void OnBnClickedD();
	afx_msg void OnBnClickedE();
	afx_msg void OnBnClickedF();
	afx_msg void OnBnClickedG();
	afx_msg void OnBnClickedA();
	afx_msg void OnBnClickedH();
	afx_msg void OnBnClickedC2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMCustomdrawSlideLaut(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlideBalance(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl lautst;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl balance;
	CButton gitarre;
	afx_msg void OnBnClickedGitarre();
};
