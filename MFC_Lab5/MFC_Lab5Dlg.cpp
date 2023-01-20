
// MFC_Lab5Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC_Lab5.h"
#include "MFC_Lab5Dlg.h"
#include "afxdialogex.h"
#include "CCalculation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCLab5Dlg dialog



CMFCLab5Dlg::CMFCLab5Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_LAB5_DIALOG, pParent)
	, m_eps(0)
	, m_ans_a(0)
	, m_ans_b(0)
	, m_ans_r(0)
	, m_ans_c(0)
	, m_diff(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCLab5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_eps);
	DDX_Text(pDX, IDC_EDIT_A, m_ans_a);
	DDX_Text(pDX, IDC_EDIT_B, m_ans_b);
	DDX_Text(pDX, IDC_EDIT_R, m_ans_r);
	DDX_Text(pDX, IDC_EDIT_C, m_ans_c);
	DDX_Text(pDX, IDC_EDIT2, m_diff);
}

BEGIN_MESSAGE_MAP(CMFCLab5Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CMFCLab5Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &CMFCLab5Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO2, &CMFCLab5Dlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CMFCLab5Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CMFCLab5Dlg::OnBnClickedButtonOpen)
END_MESSAGE_MAP()


// CMFCLab5Dlg message handlers

BOOL CMFCLab5Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCLab5Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCLab5Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCLab5Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCLab5Dlg::OnEnChangeEdit1()
{
}


void CMFCLab5Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// CDialogEx::OnOK();
	//CString str;
	//str.Format(L"%i", m_method);
	//MessageBox(str);
	UpdateData(TRUE);
	// CCalculation app;
	if (!m_app.Start(m_eps, m_ans_a, m_ans_b, m_ans_c, m_ans_r, m_diff))
		MessageBox(L"Error");
	//string vals = m_app.GetPoints();
	//CString str;
	//str.Format(L"%s", vals);
	//MessageBox(str);
	UpdateData(FALSE);
}


void CMFCLab5Dlg::OnBnClickedRadio1()
{
	// LinearDixotomy
	UpdateData(TRUE);
	m_method = 1;
	UpdateData(FALSE);
}

void CMFCLab5Dlg::OnBnClickedRadio2()
{
	// GoldenRatio
	UpdateData(TRUE);
	m_method = 2;
	UpdateData(FALSE);
}


void CMFCLab5Dlg::OnBnClickedButtonOpen()
{
	if (!m_app.ReadData())
		MessageBox(L"Can't read file");
	// TODO: Add your control notification handler code here
}
