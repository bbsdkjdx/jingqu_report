#pragma once
#include "afxwin.h"


// CDlgLogIn 对话框

class CDlgLogIn : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLogIn)

public:
	CDlgLogIn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLogIn();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CEdit m_edit_user;
	CEdit m_edit_pwd;
	CEdit m_edit_svr;
	bool LogIn();
};
