#pragma once
#include "atltime.h"


// CDateDlg �Ի���

class CDateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDateDlg)

public:
	CDateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDateDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
//	CTime m_time1;
	CString m_time2;
	CString m_time1;
};
