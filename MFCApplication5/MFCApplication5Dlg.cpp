
// MFCApplication5Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "MFCApplication5Dlg.h"
#include "afxdialogex.h"
#include "DlgLogIn.h"
#include "python_support.h"
#include "ViewDlg.h"
#include "DateDlg.h"

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

void delete_all_columns()
{
	if (!g_p_dlg)return;
	g_p_dlg->DeleteAllColumns();
}

void insert_column(int n, WCHAR *s, int width)
{
	if (g_p_dlg)
	{
		g_p_dlg->InsertColumn(n, s, width);
	}
}

void insert_combo_data(int n, WCHAR *s, int id)
{
	if (g_p_dlg)
	{
		g_p_dlg->InsertComboData(n, s, id);
	}
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
	DDX_Control(pDX, IDC_COMBO1, m_table_id_ctrl);
}

BEGIN_MESSAGE_MAP(CMFCApplication5Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CMFCApplication5Dlg::OnBnClickedOk)
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication5Dlg::OnExportXls)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication5Dlg::OnSubmit)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication5Dlg::OnImportXls)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCApplication5Dlg::OnRefresh)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication5Dlg::OnDismiss)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCApplication5Dlg::OnDeleteItem)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication5Dlg::OnNewOrEdit)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMFCApplication5Dlg::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMFCApplication5Dlg::OnDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMFCApplication5Dlg::OnRclickList1)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCApplication5Dlg::OnExportHistory)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMFCApplication5Dlg::OnSelchangeCombo1)
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
		PostMessage(WM_CLOSE, 0, 0);
		return TRUE;
		//CDialogEx::OnOK();
	}
	if (PyEvalA("autorun.get_title()"))
	{
		SetWindowText(PyGetStr());
	}

	g_p_dlg = this;
	DWORD dwStyle = m_list.GetExtendedStyle();    //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT;                //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES;                    //网格线（report风格时）
	m_list.SetExtendedStyle(dwStyle);            //设置扩展风格

	//place list ctrl.
	CRect rct;
	CWnd *pwnd = GetDlgItem(IDC_BUTTON1);
	pwnd->GetWindowRect(&rct);
	int h = rct.Height();
	GetClientRect(&rct);
	m_list.SetWindowPos(0, 0, 0, rct.Width(), rct.Height()-h-10, SWP_NOOWNERZORDER);

	//reg functions for python.
	REG_EXE_FUN(delete_all_items, "#", "");
	REG_EXE_FUN(insert_item, "#lS", "void insert_item(int n, WCHAR *str)");
	REG_EXE_FUN(set_item_text, "#llS", "");
	REG_EXE_FUN(get_item_count, "l", "");
	REG_EXE_FUN(delete_all_columns, "#", "");
	REG_EXE_FUN(insert_column, "#lSl", "");
	REG_EXE_FUN(insert_combo_data, "#lSl", "");

	//fill tables combobox.
	PyEvalW(_T("autorun.fill_table_combo()"));
	m_table_id_ctrl.SetCurSel(0);
	OnSelchangeCombo1();

	
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


HCURSOR CMFCApplication5Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int reloc_ctrl[] =//controls need to relocate when resizing window.
{ IDC_BUTTON1, IDC_BUTTON2, IDC_BUTTON3, IDC_BUTTON4, IDC_BUTTON5, IDC_BUTTON6, IDC_BUTTON7, IDC_BUTTON8,IDC_COMBO1};

void CMFCApplication5Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rct;
	for (int x = 0; x < sizeof(reloc_ctrl)/sizeof(int);++x)
	{
		int cid = reloc_ctrl[x];
		CWnd *pwnd = GetDlgItem(cid);
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


void CMFCApplication5Dlg::OnExportXls()
{
	PyExecA("autorun.export_xls(b_history=0)");
}


void CMFCApplication5Dlg::OnSubmit()
{
	ListBatchOperate(_T("autorun.submit_piece"));
}


void CMFCApplication5Dlg::OnImportXls()
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
		PySetStrW(openFileDlg.GetPathName().GetBuffer(),0);
		//PyExecW(_T("autorun.load_excel()"));
		PyExecA("autorun.load_excel()");
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


void CMFCApplication5Dlg::OnDeleteItem()
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


void CMFCApplication5Dlg::OnNewOrEdit()
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
	int nItem =pos? m_list.GetNextSelectedItem(pos):-1;
	vector<CString> title;
	vector<CString>data;


	PyEvalA("autorun.get_table_head()");
	int L = (int)PyGetInt();
	for (int n = 0; n < L; ++n)
	{
		CString str;
		title.push_back(PyGetStr(n));
		str = m_list.GetItemText(nItem, n+3 );
		data.push_back(str);
	}
	CViewDlg cvd;
	int pressok=cvd.ShowDetail(bCanEdit, title, data);
	if (pressok && (bCanEdit || nItem==-1))//nItem==-1 is add new item.
	{
		//create new piece from input.
		int n = 0;
		for (n=0; n < L;++n)
		{
			PySetStrW(data[n].GetBuffer(), n);
		}
		PySetInt(0, n);
		PyExecA("autorun.new_piece_from_stack()");

		//if in edit mode,delete the old piece.
		PySetStrW( nItem!=-1 ? m_list.GetItemText(nItem,0).GetBuffer():_T(""),0);
		PyExecA("autorun.delete_piece(stack__[0])");
		PyExecA("autorun.refresh()");
	}
}


void CMFCApplication5Dlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	PyEvalW(_T("'耕保' in autorun.get_title()"));
	if (!PyGetInt())return;

	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (!pos) return;
	int nItem = m_list.GetNextSelectedItem(pos);

	CMenu Menu;
	Menu.CreatePopupMenu();
	Menu.AppendMenu(MF_STRING, 1, _T("复制图斑号"));
	CPoint pt;
	::GetCursorPos(&pt);
	int sel = (int)Menu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, pt.x, pt.y, this, nullptr);
	if (sel == 1)
	{
		CString str=m_list.GetItemText(nItem, 20);
		OpenClipboard();
		EmptyClipboard();
		size_t cbStr = (str.GetLength() + 1) * sizeof(TCHAR);
		HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);
		memcpy_s(GlobalLock(hData), cbStr, str.LockBuffer(), cbStr);
		GlobalUnlock(hData);
		str.UnlockBuffer();
		UINT uiFormat = (sizeof(TCHAR) == sizeof(WCHAR)) ? CF_UNICODETEXT : CF_TEXT;
		SetClipboardData(uiFormat, hData);
		CloseClipboard();
	}
	*pResult = 0;
}


void CMFCApplication5Dlg::OnExportHistory()
{
	CDateDlg cdd;
	if (cdd.DoModal() == IDOK)
	{
		PySetStrW(cdd.m_time1.GetBuffer(), 0);
		PySetStrW(cdd.m_time2.GetBuffer(), 1);
		PyExecA("autorun.export_xls(b_history=1)");
	}
}



BOOL CMFCApplication5Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message==256 && pMsg->wParam==123 && GetAsyncKeyState(0x11)&0x8000 )//Ctrl+F12 pressed.
	{
		InteractInConsole();
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMFCApplication5Dlg::OnSelchangeCombo1()
{
	PySetInt(m_table_id_ctrl.GetItemData(m_table_id_ctrl.GetCurSel()), 0);
	PyExecA("autorun.switch_table()");
	OnRefresh();
}


void CMFCApplication5Dlg::DeleteAllColumns()
{
	int nColumnCount = m_list.GetHeaderCtrl()->GetItemCount();

	// Delete all of the columns. 
	for (int i = 0; i < nColumnCount; i++)
	{
		m_list.DeleteColumn(0);
	}
}


void CMFCApplication5Dlg::InsertColumn(int n, WCHAR* s, int width)
{
	m_list.InsertColumn(n, s, 0, width);
}


void CMFCApplication5Dlg::InsertComboData(int n, WCHAR* s, int id)
{
	m_table_id_ctrl.InsertString(n, s);
	m_table_id_ctrl.SetItemData(n, id);
}
