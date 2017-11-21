// MyList.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "MyList.h"


// CMyList

IMPLEMENT_DYNAMIC(CMyList, CListCtrl)

CMyList::CMyList()
{

}

CMyList::~CMyList()
{
}


BEGIN_MESSAGE_MAP(CMyList, CListCtrl)
END_MESSAGE_MAP()



// CMyList 消息处理程序




BOOL CMyList::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==13)
	{
		GetParent()->PostMessageW(12345, 0, 0);
	}
	return CListCtrl::PreTranslateMessage(pMsg);
}


