// DlgLogIn.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "DlgLogIn.h"
#include "afxdialogex.h"
#include "python_support.h"


// CDlgLogIn 对话框

IMPLEMENT_DYNAMIC(CDlgLogIn, CDialogEx)

CDlgLogIn::CDlgLogIn(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLogIn::IDD, pParent)
{

}

CDlgLogIn::~CDlgLogIn()
{
}

void CDlgLogIn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit_user);
	DDX_Control(pDX, IDC_EDIT2, m_edit_pwd);
	DDX_Control(pDX, IDC_EDIT3, m_edit_svr);
}


BEGIN_MESSAGE_MAP(CDlgLogIn, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgLogIn::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLogIn 消息处理程序


void CDlgLogIn::OnBnClickedOk()
{
	CString str;
	m_edit_user.GetWindowTextW(str);
	PySetStrW(str.GetBuffer(), 0);
	m_edit_pwd.GetWindowTextW(str);
	PySetStrW(str.GetBuffer(), 1);
	m_edit_svr.GetWindowTextW(str);
	PySetStrW(str.GetBuffer(), 2);
	PyExecW(_T("autorun.set_login_info()"));

	PyEvalW(_T("autorun.login()"));
	if (!PyGetInt())
	{
		return;
	}

	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


BOOL CDlgLogIn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	PyExecW(_T("autorun.get_login_info()"));
	m_edit_user.SetWindowTextW(PyGetStr(0));
	m_edit_pwd.SetWindowTextW(PyGetStr(1));
	m_edit_svr.SetWindowTextW(PyGetStr(2));


	m_edit_user.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


bool CDlgLogIn::LogIn()
{
	if (DoModal()==IDCANCEL)
	{
		return false;
	}
	return true;
}
