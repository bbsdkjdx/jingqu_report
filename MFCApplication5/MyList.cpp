// MyList.cpp : ʵ���ļ�
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



// CMyList ��Ϣ�������




BOOL CMyList::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==13)
	{
		GetParent()->PostMessageW(12345, 0, 0);
	}
	return CListCtrl::PreTranslateMessage(pMsg);
}


