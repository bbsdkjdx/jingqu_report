#pragma once
#include "afxwin.h"


// CDlgLogIn �Ի���

class CDlgLogIn : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLogIn)

public:
	CDlgLogIn(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLogIn();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CEdit m_edit_user;
	CEdit m_edit_pwd;
	CEdit m_edit_svr;
	bool LogIn();
};
