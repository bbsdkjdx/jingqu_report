#pragma once
#include "afxcmn.h"
#include "ListEditor.h"
#include <vector>


// CViewDlg 对话框
using std::vector;
class CViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewDlg)

public:
	CViewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CListCtrl m_list;
	CListEditor m_editor;
	vector<CString> *m_p_title;
	vector<CString> *m_p_data;
	bool m_can_edit;
//	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	afx_msg LRESULT OnEditLostFocus(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLvnBeginScrollList1(NMHDR *pNMHDR, LRESULT *pResult);
	int ShowDetail(bool bCanEdit, vector<CString> & title, vector<CString> & data);
//	afx_msg void OnCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
};
