// ViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "ViewDlg.h"
#include "afxdialogex.h"


// CViewDlg 对话框

IMPLEMENT_DYNAMIC(CViewDlg, CDialogEx)

CViewDlg::CViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViewDlg::IDD, pParent)
{

}

CViewDlg::~CViewDlg()
{
}

void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CViewDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CViewDlg::OnLvnItemchangedList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CViewDlg::OnClickList1)
	ON_MESSAGE(WM_EDIT_LOST_FOCUS, &CViewDlg::OnEditLostFocus)
	ON_NOTIFY(LVN_BEGINSCROLL, IDC_LIST1, &CViewDlg::OnLvnBeginScrollList1)
END_MESSAGE_MAP()


// CViewDlg 消息处理程序


void CViewDlg::OnBnClickedOk()
{
	OnEditLostFocus(0, 0);
	//CDialogEx::OnOK();
}


BOOL CViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DWORD dwStyle = m_list.GetExtendedStyle();    //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT;                //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES;                    //网格线（report风格时）
	m_list.SetExtendedStyle(dwStyle);            //设置扩展风格

	m_list.InsertColumn(0, _T("属性"), 0, 200);
	m_list.InsertColumn(1, _T("数值"), 0, 500);
	for (int x = 0; x < 30;++x)
{
	m_list.InsertItem(x, _T("abc"));
	m_list.SetItemText(x, 1, _T("abc"));
}
	m_editor.Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN | WS_BORDER, CRect(), this, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CViewDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

void CViewDlg::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iSubItem==0)return;

	CRect rect;
	int item = pNMItemActivate->iItem;
	int sub_item = pNMItemActivate->iSubItem;
	m_list.GetSubItemRect(item, sub_item, LVIR_LABEL, rect); //获取子表项的大小
	rect.left += 1;
	m_editor.MoveWindow(&rect);
	m_editor.ShowWindow(SW_SHOW);
	m_editor.m_n_item = item;
	m_editor.m_n_sub_item = sub_item;
	m_editor.SetWindowText(m_list.GetItemText(item,sub_item));//设置编辑框的内容为字表项的内容
	m_editor.SetFocus();
	m_editor.SetSel(-1);
	*pResult = 0;
}


afx_msg LRESULT CViewDlg::OnEditLostFocus(WPARAM wParam, LPARAM lParam)
{
	CString str;
	m_editor.ShowWindow(SW_HIDE);
	m_editor.GetWindowTextW(str);
	m_list.SetItemText(m_editor.m_n_item, m_editor.m_n_sub_item, str);
	return 0;
}


void CViewDlg::OnLvnBeginScrollList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 5.5 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0560。
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	OnEditLostFocus(0, 0);
	*pResult = 0;
}
