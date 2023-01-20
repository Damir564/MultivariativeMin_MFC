
// MFC_Lab5Dlg.h : header file
//

#pragma once
#include "CCalculation.h"


// CMFCLab5Dlg dialog
class CMFCLab5Dlg : public CDialogEx
{
// Construction
public:
	CMFCLab5Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_LAB5_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	double m_eps;
	double m_ans_a;
	double m_ans_b;
	double m_ans_r;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio2();
	int m_method;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedButtonOpen();
	CCalculation m_app;
	double m_ans_c;
	double m_diff;
};
