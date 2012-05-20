#include "stdafx.h"
#include "Prompt.h"
#include ".\Prompt.h"

//��Ϣ����
#define IDM_EXIT					WM_USER+101							//�뿪��Ϣ

///////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPrompt, CDialog)

	ON_WM_PAINT()

END_MESSAGE_MAP()

//���캯��
CPrompt::CPrompt(): CDialog(IDD_DIALOG2)
{
	ZeroMemory(m_tStr,sizeof(m_tStr));
}

//��������
CPrompt::~CPrompt()
{
}

//�ؼ���
void CPrompt::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��ʼ������
BOOL CPrompt::OnInitDialog()
{
	__super::OnInitDialog();

	return TRUE;
}

//�ػ�����
void CPrompt::OnPaint() 
{
	CPaintDC dc(this); 

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,0));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//��������
	CFont ViewFont;
	ViewFont.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,1,TEXT("Arial"));
	CFont *pOldFont=dc.SelectObject(&ViewFont);

	CRect rcScore(0,30,220,30+19);
	dc.DrawText(m_tStr,lstrlen(m_tStr),&rcScore,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

	//��ԭ����
	dc.SelectObject(pOldFont);
	ViewFont.DeleteObject();

	return;
}

//������Ϣ
BOOL CPrompt::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	BYTE bTemp = (BYTE)wParam;

	if(bTemp==IDOK || bTemp==IDCANCEL)
	{
		AfxGetMainWnd()->PostMessage(IDM_EXIT,0,0);	

		OnOK();
		return TRUE;
	}

	return CDialog::OnCommand(wParam, lParam);
}

//���ö԰�
void CPrompt::SetString(TCHAR szString[])
{
	CopyMemory(m_tStr,szString,CountArray(m_tStr));

	//�滭��ͼ
	Invalidate(NULL);

	return ;
}

//������Ϣ
BOOL CPrompt::PreTranslateMessage(MSG* pMsg)
{
	//������ĸ
	if(pMsg->message==WM_KEYDOWN)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam); 
		if(bMesValue==VK_SPACE || bMesValue==VK_RETURN) return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
///////////////////////////////////////////////////////////////////////////

