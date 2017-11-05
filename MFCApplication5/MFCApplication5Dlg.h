
// MFCApplication5Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMFCApplication5Dlg 对话框
class CMFCApplication5Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication5Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCAPPLICATION5_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnExportXls();
	afx_msg void OnSubmit();
	afx_msg void OnImportXls();
	afx_msg void OnRefresh();
	afx_msg void OnDismiss();
	afx_msg void OnDeleteItem();
protected:
	int ListBatchOperate(CString op);
public:
	afx_msg void OnNewOrEdit();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	void ShowSelectedItem(bool bCanEdit);
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnExportHistory();
protected:
//	void ExportData(bool history);
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CComboBox m_table_id_ctrl;
	afx_msg void OnSelchangeCombo1();
	void DeleteAllColumns();
	void InsertColumn(int n, WCHAR* s, int width);
};
