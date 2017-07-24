
// MFCApplication5Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CMFCApplication5Dlg �Ի���
class CMFCApplication5Dlg : public CDialogEx
{
// ����
public:
	CMFCApplication5Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCAPPLICATION5_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	afx_msg void OnBnClickedButton2();
	afx_msg void OnSubmit();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnRefresh();
	afx_msg void OnDismiss();
	afx_msg void OnBnClickedButton6();
protected:
	int ListBatchOperate(CString op);
public:
	afx_msg void OnBnClickedButton5();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	void ShowSelectedItem(bool bCanEdit);
};
