#pragma once
#include "atltime.h"


// CDateDlg 对话框

class CDateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDateDlg)

public:
	CDateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDateDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
//	CTime m_time1;
	CString m_time2;
	CString m_time1;
};
