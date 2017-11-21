// ViewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "ViewDlg.h"
#include "afxdialogex.h"


// CViewDlg �Ի���

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


// CViewDlg ��Ϣ�������


void CViewDlg::OnBnClickedOk()
{
	OnEditLostFocus(0, 0);
	//CDialogEx::OnOK();
}


BOOL CViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DWORD dwStyle = m_list.GetExtendedStyle();    //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT;                //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES;                    //�����ߣ�report���ʱ��
	m_list.SetExtendedStyle(dwStyle);            //������չ���

	m_list.InsertColumn(0, _T("����"), 0, 200);
	m_list.InsertColumn(1, _T("��ֵ"), 0, 500);

	int L = m_p_title->size();
	for (int n = 0; n < L; ++n)
	{
		m_list.InsertItem(n, (*m_p_title)[n]);
		m_list.SetItemText(n, 1, (*m_p_data)[n]);
	}
	m_editor.Create(ES_AUTOHSCROLL | WS_CHILDWINDOW| ES_LEFT | ES_WANTRETURN | WS_BORDER, CRect(), this, 0);
	m_editor.SetFont(m_list.GetFont());
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
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
	// �˹���Ҫ�� Internet Explorer 5.5 ����߰汾��
	// ���� _WIN32_IE ������ >= 0x0560��
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ˴������Ϣ����������
}


//void CViewDlg::OnSetfocusList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	AfxMessageBox(_T("key down"));
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}




void CViewDlg::StartEdit(int item, int sub_item)
{
	m_list.EnableWindow(0);
//	m_list.SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	CRect rect;
	m_list.GetSubItemRect(item, sub_item, LVIR_LABEL, rect); //��ȡ�ӱ���Ĵ�С
	rect.left += 1;
	rect.right += 2;
	rect.bottom += 2;
	m_editor.MoveWindow(&rect);
	m_editor.ShowWindow(SW_SHOW);
	m_editor.m_n_item = item;
	m_editor.m_n_sub_item = sub_item;
	m_editor.SetWindowText(m_list.GetItemText(item, sub_item));//���ñ༭�������Ϊ�ֱ��������
	m_editor.SetFocus();
	m_editor.SetSel(-1);
}


void CViewDlg::OnEndscrollList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �˹���Ҫ�� Internet Explorer 5.5 ����߰汾��
	// ���� _WIN32_IE ������ >= 0x0560��
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
