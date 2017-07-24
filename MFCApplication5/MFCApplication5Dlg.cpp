
// MFCApplication5Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "MFCApplication5Dlg.h"
#include "afxdialogex.h"
#include "DlgLogIn.h"
#include "python_support.h"
#include "ViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

CMFCApplication5Dlg *g_p_dlg = nullptr;

void delete_all_items()
{
	if (!g_p_dlg)return;
	g_p_dlg->m_list.DeleteAllItems();
}

void insert_item(int n, WCHAR *str)
{
	if (!g_p_dlg)return;
	g_p_dlg->m_list.InsertItem(n, str);
}

void set_item_text(int n, int nsub, WCHAR *str)
{
	if (!g_p_dlg)return;
	g_p_dlg->m_list.SetItemText(n, nsub, str);
}

int get_item_count()
{
	if (!g_p_dlg)return -1;
	return g_p_dlg->m_list.GetItemCount();
}


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication5Dlg 对话框



CMFCApplication5Dlg::CMFCApplication5Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication5Dlg::IDD, pParent)
{

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CMFCApplication5Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CMFCApplication5Dlg::OnBnClickedOk)
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication5Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication5Dlg::OnSubmit)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication5Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCApplication5Dlg::OnRefresh)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication5Dlg::OnDismiss)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCApplication5Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication5Dlg::OnBnClickedButton5)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMFCApplication5Dlg::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMFCApplication5Dlg::OnDblclkList1)
END_MESSAGE_MAP()


// CMFCApplication5Dlg 消息处理程序

BOOL CMFCApplication5Dlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	CDlgLogIn cdl;
	if (!cdl.LogIn())
	{
		CDialogEx::OnOK();
	}

	g_p_dlg = this;
	DWORD dwStyle = m_list.GetExtendedStyle();    //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT;                //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES;                    //网格线（report风格时）
	m_list.SetExtendedStyle(dwStyle);            //设置扩展风格

	PyEvalW(_T("autorun.get_table_head()"));
	int len = (int)PyGetInt();
	m_list.InsertColumn(0, _T("流水号"), 0, 0);
	m_list.InsertColumn(1, _T("数据来源"), 0, 100);
	m_list.InsertColumn(2, _T("数据状态"), 0, 100);
	for (int x = 0; x < len; ++x)
	{
		m_list.InsertColumn(x+3, PyGetStr(x), 0, 100);
	}

	CRect rct;
	CWnd *pwnd = GetDlgItem(IDC_BUTTON1);
	pwnd->GetWindowRect(&rct);
	int h = rct.Height();
	GetClientRect(&rct);
	m_list.SetWindowPos(0, 0, 0, rct.Width(), rct.Height()-h-10, SWP_NOOWNERZORDER);

	REG_EXE_FUN(delete_all_items, "#", "");
	REG_EXE_FUN(insert_item, "#lS", "");
	REG_EXE_FUN(set_item_text, "#llS", "");
	REG_EXE_FUN(get_item_count, "l", "");
	OnRefresh();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication5Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication5Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication5Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication5Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rct;
	for (int x = IDC_BUTTON1; x <= IDC_BUTTON7;++x)
	{
		CWnd *pwnd = GetDlgItem(x);
		if (!pwnd)
		{
			return;
		}
		pwnd->GetWindowRect(&rct);
		ScreenToClient(&rct);
		pwnd->SetWindowPos(0, rct.left,cy-rct.Height()-5, 0, 0, SWP_NOSIZE);
	}

	int h = rct.Height();
	GetClientRect(&rct);
	m_list.SetWindowPos(0, 0, 0, rct.Width(), cy-h-10, SWP_NOOWNERZORDER);

	// TODO:  在此处添加消息处理程序代码
}


void CMFCApplication5Dlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CMFCApplication5Dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 655;
	lpMMI->ptMinTrackSize.y = 120;
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CMFCApplication5Dlg::OnBnClickedButton2()
{
}


void CMFCApplication5Dlg::OnSubmit()
{
	ListBatchOperate(_T("autorun.submit_piece"));
}


void CMFCApplication5Dlg::OnBnClickedButton1()
{
	BOOL isOpen = TRUE;     //是否打开(否则为保存)  
	CString defaultDir = L"";   //默认打开的文件路径  
	CString fileName = L"";         //默认打开的文件名  
	CString filter = L"文件 (*.xlsx; *.xls)|*.xlsx;*.xls||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = L"";
	INT_PTR result = openFileDlg.DoModal();
	CString cmd;
	if (result ==IDOK) 
	{
		PySetStr(openFileDlg.GetPathName().GetBuffer(),0);
		PyExecW(_T("autorun.load_excel()"));
	}
}


void CMFCApplication5Dlg::OnRefresh()
{
	PyExecW(_T("autorun.refresh()"));
}


void CMFCApplication5Dlg::OnDismiss()
{
	ListBatchOperate(_T("autorun.dismiss_piece"));
}


void CMFCApplication5Dlg::OnBnClickedButton6()
{
	ListBatchOperate(_T("autorun.delete_piece"));
}


int CMFCApplication5Dlg::ListBatchOperate(CString op)
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return 0;
	}
	else
	{
		while (pos)
		{
			int nItem = m_list.GetNextSelectedItem(pos);
			CString str;
			str.Format(_T("%s('%s')"),op, m_list.GetItemText(nItem, 0));
			PyExecW(str.GetBuffer());
		}
	}
	OnRefresh();
	return 1;
}


void CMFCApplication5Dlg::OnBnClickedButton5()
{
	ShowSelectedItem(true);
}


void CMFCApplication5Dlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	//*pResult = 0;
}


void CMFCApplication5Dlg::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	ShowSelectedItem(false);
	//*pResult = 0;
}


void CMFCApplication5Dlg::ShowSelectedItem(bool bCanEdit)
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)return;
	int nItem = m_list.GetNextSelectedItem(pos);
	vector<CString> title;
	vector<CString>data;


	PyEvalW(_T("autorun.get_table_head()"));
	int L = PyGetInt();
	for (int n = 0; n < L; ++n)
	{
		CString str;
		title.push_back(PyGetStr(n));
		str = m_list.GetItemText(nItem, n+3 );
		data.push_back(str);
	}
	CViewDlg cvd;
	cvd.ShowDetail(bCanEdit, title, data);
	if (bCanEdit)
	{
		int n = 0;
		for (n=0; n < L;++n)
		{
			PySetStr(data[n].GetBuffer(), n);
		}
		PySetInt(0, n);
		PyExecW(_T("autorun.new_piece_from_stack()"));
		PySetStr(m_list.GetItemText(nItem,0).GetBuffer(),0);
		PyExecW(_T("autorun.delete_piece(stack__[0])"));
		PyExecW(_T("autorun.refresh()"));
	}
}
