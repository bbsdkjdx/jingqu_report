// ListEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "ListEditor.h"


// CListEditor

IMPLEMENT_DYNAMIC(CListEditor, CEdit)

CListEditor::CListEditor()
{
	m_n_item = m_n_sub_item = -1;
}

CListEditor::~CListEditor()
{
}


BEGIN_MESSAGE_MAP(CListEditor, CEdit)
	ON_WM_KILLFOCUS()
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CListEditor 消息处理程序




void CListEditor::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	GetParent()->SendMessageW(WM_EDIT_LOST_FOCUS, 0, 0);
}


