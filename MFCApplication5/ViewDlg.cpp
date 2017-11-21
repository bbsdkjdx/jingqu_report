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
	m_can_edit = false;
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
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CViewDlg::OnLvnItemchangedList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CViewDlg::OnClickList1)
	ON_MESSAGE(WM_EDIT_LOST_FOCUS, &CViewDlg::OnEditLostFocus)
	ON_NOTIFY(LVN_BEGINSCROLL, IDC_LIST1, &CViewDlg::OnLvnBeginScrollList1)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CViewDlg::OnCustomdrawList1)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CViewDlg::OnLvnItemchangedList1)
ON_WM_DESTROY()
//ON_NOTIFY(NM_SETFOCUS, IDC_LIST1, &CViewDlg::OnSetfocusList1)
ON_BN_CLICKED(IDC_BUTTON1, &CViewDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, &CViewDlg::OnBnClickedButton2)
ON_WM_KEYDOWN()
ON_NOTIFY(LVN_ENDSCROLL, IDC_LIST1, &CViewDlg::OnEndscrollList1)
ON_MESSAGE(12345, &CViewDlg::OnPressEnter)
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

	int L = m_p_title->size();
	for (int n = 0; n < L; ++n)
	{
		m_list.InsertItem(n, (*m_p_title)[n]);
		m_list.SetItemText(n, 1, (*m_p_data)[n]);
	}
	m_editor.Create(ES_AUTOHSCROLL | WS_CHILDWINDOW| ES_LEFT | ES_WANTRETURN | WS_BORDER, CRect(), this, 0);
	m_editor.SetFont(m_list.GetFont());
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CViewDlg::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iSubItem==0 || !m_can_edit)return;
	int item = pNMItemActivate->iItem;
	int sub_item = pNMItemActivate->iSubItem;
	StartEdit(item, sub_item);
	*pResult = 0;
}


afx_msg LRESULT CViewDlg::OnEditLostFocus(WPARAM wParam, LPARAM lParam)
{
	CString str;
	m_editor.ShowWindow(SW_HIDE);
	m_list.EnableWindow();
	m_editor.GetWindowTextW(str);
	m_list.SetItemText(m_editor.m_n_item, m_editor.m_n_sub_item, str);
	int n_max_itm = m_list.GetItemCount();
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


int CViewDlg::ShowDetail(bool bCanEdit, vector<CString> & title, vector<CString> & data)
{
	int L = title.size();
	if (data.size() != L)return 0;
	m_can_edit = bCanEdit;
	m_p_title = &title;
	m_p_data = &data;

	return (DoModal() == IDOK) ? 1 : 0;
}



void CViewDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CViewDlg::OnDestroy()
{
	int L = m_p_data->size();
	for (int n = 0; n < L;++n)
	{
		(*m_p_data)[n] = m_list.GetItemText(n, 1);
	}
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
}


//void CViewDlg::OnSetfocusList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO:  在此添加控件通知处理程序代码
//	AfxMessageBox(_T(""));
//	*pResult = 0;
//}


void CViewDlg::OnBnClickedButton1()
{
	OnOK();
}


void CViewDlg::OnBnClickedButton2()
{
	OnCancel();
}

void CViewDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	AfxMessageBox(_T("key down"));
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}




void CViewDlg::StartEdit(int item, int sub_item)
{
	m_list.EnableWindow(0);
//	m_list.SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	CRect rect;
	m_list.GetSubItemRect(item, sub_item, LVIR_LABEL, rect); //获取子表项的大小
	rect.left += 1;
	rect.right += 2;
	rect.bottom += 2;
	m_editor.MoveWindow(&rect);
	m_editor.ShowWindow(SW_SHOW);
	m_editor.m_n_item = item;
	m_editor.m_n_sub_item = sub_item;
	m_editor.SetWindowText(m_list.GetItemText(item, sub_item));//设置编辑框的内容为字表项的内容
	m_editor.SetFocus();
	m_editor.SetSel(-1);
}


void CViewDlg::OnEndscrollList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 5.5 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0560。
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
//	AfxMessageBox(_T(""));
	*pResult = 0;
}


afx_msg LRESULT CViewDlg::OnPressEnter(WPARAM wParam, LPARAM lParam)
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)return 0;
	int nItem = m_list.GetNextSelectedItem(pos);
	StartEdit(nItem, 1);
	return 0;
}
