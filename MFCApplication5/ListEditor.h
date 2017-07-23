#pragma once


// CListEditor

class CListEditor : public CEdit
{
	DECLARE_DYNAMIC(CListEditor)

public:
	CListEditor();
	virtual ~CListEditor();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
public:
	int m_n_item;
	int m_n_sub_item;
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


