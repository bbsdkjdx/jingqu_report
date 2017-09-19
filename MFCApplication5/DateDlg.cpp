// DateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "DateDlg.h"
#include "afxdialogex.h"


// CDateDlg 对话框

IMPLEMENT_DYNAMIC(CDateDlg, CDialogEx)

CDateDlg::CDateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDateDlg::IDD, pParent)
	, m_time2(_T(""))
	, m_time1(_T(""))
{

}

CDateDlg::~CDateDlg()
{
}

void CDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_time1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_time2);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_time1);
}


BEGIN_MESSAGE_MAP(CDateDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDateDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDateDlg 消息处理程序


void CDateDlg::OnBnClickedOk()
{
	UpdateData();
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
