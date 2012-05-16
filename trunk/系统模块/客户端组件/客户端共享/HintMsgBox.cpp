#include "StdAfx.h"
#include "Resource.h"
#include "HintMsgBox.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_MESSAGE					100									//��Ϣ��ʶ

//����λ��
#define SMB_WINDOW_WIDTH			230									//��С���

//ͼ��ƫ��
#define SMB_ICON_LEFT				20									//ͼ��ƫ��
#define SMB_ICON_WIDTH				32									//ͼ����
#define SMB_ICON_HEIGHT				32									//ͼ��߶�

//�ַ�ƫ��
#define SMB_STRING_TOP				25									//�ַ�ƫ��
#define SMB_STRING_LEFT				20									//�ַ�ƫ��
#define SMB_STRING_RIGHT			40									//�ַ�ƫ��
#define SMB_STRING_WIDTH			600									//�����
#define SMB_STRING_HEIGHT			600									//�����

//��ťƫ��
#define SMB_BUTTON_TOP				20									//��ťƫ��
#define SMB_BUTTON_LEFT				12									//��ťƫ��
#define SMB_BUTTON_RIGHT			12									//��ťƫ��
#define SMB_BUTTON_BUTTOM			13									//��ťƫ��
#define SMB_BUTTON_SPACE			10									//��ť���
#define SMB_BUTTON_WIDTH			70									//��ťƫ��
#define SMB_BUTTON_HEIGHT			22									//��ťƫ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CHintMsgBox, CSkinDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CHintMsgBox::CHintMsgBox(CWnd * pParentWnd) : CSkinDialogEx(IDD_HINT_MSG,pParentWnd)
{
	//���ñ���
	m_uType=MB_OK;

	//��������
	m_nElapse=0;
	m_nBtCount=0;
	m_hResInstance=NULL;

	return;
}

//��������
CHintMsgBox::~CHintMsgBox()
{
}

//��ʼ����Ϣ
BOOL CHintMsgBox::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(m_strCaption);

	//�رհ�ť
	if((m_uType&MB_DEFBUTTON2)!=0)m_cbButtonState[BST_CLOSE]&=(~BUT_EXIST);
	else m_cbButtonState[BST_CLOSE]|=BUT_EXIST;

	//��ť����
	CreateBoxButton();
	INT nButtonAreaHeight=SMB_BUTTON_HEIGHT+SMB_BUTTON_TOP+SMB_BUTTON_BUTTOM;
	INT nButtonAreaWidth=m_nBtCount*SMB_BUTTON_WIDTH+(m_nBtCount+1)*SMB_BUTTON_SPACE+SMB_BUTTON_LEFT+SMB_BUTTON_RIGHT;

	//�ַ��ռ�
	CClientDC ClientDC(this);
	CRect rcString(0,0,SMB_STRING_WIDTH,SMB_STRING_HEIGHT);
	ClientDC.SelectObject(CSkinResourceManager::GetDefaultFont());
	ClientDC.DrawText(m_strString,rcString,DT_CALCRECT|DT_EXPANDTABS|DT_NOCLIP|DT_WORD_ELLIPSIS);

	//�ַ�����
	INT nStringAreaHeight=rcString.Height()+SMB_STRING_TOP;
	INT nStringAreaWidth=rcString.Width()+SMB_ICON_LEFT+SMB_ICON_WIDTH+SMB_STRING_LEFT+SMB_STRING_RIGHT;

	//��������
	INT nWindowAreaHeight=nStringAreaHeight+nButtonAreaHeight;
	INT nWindowAreaWidth=__max(SMB_WINDOW_WIDTH,__max(nStringAreaWidth,nButtonAreaWidth));

	//���λ��
	if (nWindowAreaWidth>nStringAreaWidth)
	{
		m_rcString.top=m_SkinAttribute.m_nCaptionHeigth+SMB_STRING_TOP;
		m_rcString.left=GetXBorder()+SMB_ICON_LEFT+SMB_ICON_WIDTH+SMB_STRING_LEFT+(nWindowAreaWidth-nStringAreaWidth)/2;
		m_rcString.right=m_rcString.left+rcString.Width();
		m_rcString.bottom=m_rcString.top+rcString.Height();
	}
	else
	{
		m_rcString.top=m_SkinAttribute.m_nCaptionHeigth+SMB_STRING_TOP;
		m_rcString.left=GetXBorder()+SMB_ICON_LEFT+SMB_ICON_WIDTH+SMB_STRING_LEFT;
		m_rcString.right=m_rcString.left+rcString.Width();
		m_rcString.bottom=m_rcString.top+rcString.Height();
	}

	//��������
	CRect rcWindow(0,0,0,0);
	rcWindow.right=nWindowAreaWidth+GetXBorder()*2;
	rcWindow.bottom=nWindowAreaHeight+m_SkinAttribute.m_nCaptionHeigth+GetYBorder();
	SetWindowPos(NULL,0,0,rcWindow.Width(),rcWindow.Height(),SWP_NOMOVE|SWP_NOZORDER);

	//������ť
	INT nYButton=m_SkinAttribute.m_nCaptionHeigth+nWindowAreaHeight-SMB_BUTTON_BUTTOM-SMB_BUTTON_HEIGHT;
	INT nXButton=GetXBorder()+(nWindowAreaWidth-m_nBtCount*SMB_BUTTON_WIDTH-(m_nBtCount+1)*SMB_BUTTON_SPACE)/2+SMB_BUTTON_SPACE;
	for (UINT i=0;i<m_nBtCount;i++)
	{
		m_btButton[i].SetWindowPos(NULL,nXButton,nYButton,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
		nXButton+=SMB_BUTTON_SPACE+SMB_BUTTON_WIDTH;
	}

	//Ĭ�ϰ�ť
	UINT uDefaultIndex=(m_uType&MB_DEFMASK)>>8;
	if (uDefaultIndex>=m_nBtCount) uDefaultIndex=0;
	m_btButton[uDefaultIndex].SetFocus();
	m_btButton[uDefaultIndex].SetButtonStyle(m_btButton[uDefaultIndex].GetButtonStyle()|BS_DEFPUSHBUTTON);

	//���д���
	CenterWindow(GetParent());

	//����ʱ��
	if (m_nElapse!=0) SetTimer(IDI_MESSAGE,1000,NULL);

	return FALSE;
}

//������Ϣ
BOOL CHintMsgBox::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��ť��ʶ
	UINT uButtonID=LOWORD(wParam);

	//�رմ���
	if (uButtonID==IDOK)
	{
		UINT uDefaultIndex=(m_uType&MB_DEFMASK)>>8;
		if (uDefaultIndex>=m_nBtCount) uDefaultIndex=0;
		EndDialog(GetWindowLong(m_btButton[uDefaultIndex],GWL_ID));
	}
	else if (uButtonID==IDCANCEL)
	{
		EndDialog(GetWindowLong(m_btButton[m_nBtCount-1],GWL_ID));
	}
	else EndDialog(uButtonID);

	return TRUE;
}

//��ʾ��Ϣ
INT CHintMsgBox::ShowMessageBox(LPCTSTR pszString, UINT uType, UINT nElapse)
{
	return ShowMessageBox(TEXT("��ʾ��Ϣ"),pszString,uType,nElapse);
}

//��ʾ��Ϣ
INT CHintMsgBox::ShowMessageBox(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType, UINT nElapse)
{
	//���ñ���
	m_uType=uType;
	m_nElapse=nElapse;
	m_strString=pszString;
	m_strCaption=pszCaption;

	//��ʾ����
	INT nResult=(INT)DoModal();

	return nResult;
}

//���ť
VOID CHintMsgBox::CreateBoxButton()
{
	//��������
	UINT uButtonID[3]={0,0,0};
	LPCTSTR pszString[3]={NULL,NULL,NULL};

	//���ð�ť
	switch (m_uType&MB_TYPEMASK)
	{
	case MB_OKCANCEL:
		{
			m_nBtCount=2;
			uButtonID[0]=IDOK;
			uButtonID[1]=IDCANCEL;
			pszString[0]=TEXT("ȷ��(&O)");
			pszString[1]=TEXT("ȡ��(&C)");
			break;
		}
	case MB_YESNO:
		{
			m_nBtCount=2;
			uButtonID[0]=IDYES;
			uButtonID[1]=IDNO;
			pszString[0]=TEXT("��(&Y)");
			pszString[1]=TEXT("��(&N)");
			break;
		}
	case MB_YESNOCANCEL:
		{
			m_nBtCount=3;
			uButtonID[0]=IDYES;
			uButtonID[1]=IDNO;
			uButtonID[2]=IDCANCEL;
			pszString[0]=TEXT("��(&Y)");
			pszString[1]=TEXT("��(&N)");
			pszString[2]=TEXT("ȡ��(&C)");
			break;
		}
	case MB_RETRYCANCEL:
		{
			m_nBtCount=2;
			uButtonID[0]=IDRETRY;
			uButtonID[1]=IDCANCEL;
			pszString[0]=TEXT("����(&R)");
			pszString[1]=TEXT("ȡ��(&C)");
			break;
		}
	case MB_ABORTRETRYIGNORE:
		{
			m_nBtCount=3;
			uButtonID[0]=IDABORT;
			uButtonID[1]=IDRETRY;
			uButtonID[2]=IDIGNORE;
			pszString[0]=TEXT("��ֹ(&A)");
			pszString[1]=TEXT("����(&R)");
			pszString[2]=TEXT("����(&I)");
			break;
		}
	default:
		{
			m_nBtCount=1;
			uButtonID[0]=IDOK;
			pszString[0]=TEXT("ȷ��(&O)");
			break;
		}
	}

	//��ʾ��ť
	CRect rcButton(0,0,SMB_BUTTON_WIDTH,SMB_BUTTON_HEIGHT);
	for (UINT i=0;i<m_nBtCount;i++) m_btButton[i].Create(pszString[i],WS_CHILD,rcButton,this,uButtonID[i]);

	return;
}

//�滭��Ϣ
VOID CHintMsgBox::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	DrawCaption(&dc);
	DrawBackGround(&dc);
	DrawBorder(&dc);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����ͼ��
	HICON hIcon=NULL;
	UINT uIconType=(m_uType&MB_ICONMASK);
	if (uIconType==MB_ICONHAND) hIcon=LoadIcon(NULL,IDI_HAND);
	else if (uIconType==MB_ICONQUESTION) hIcon=LoadIcon(NULL,IDI_QUESTION);
	else if (uIconType==MB_ICONEXCLAMATION) hIcon=LoadIcon(NULL,IDI_EXCLAMATION);
	else hIcon=LoadIcon(NULL,IDI_ASTERISK);

	//�滭ͼ��
	INT nYPos=(m_rcString.bottom+m_rcString.top)/2-SMB_ICON_HEIGHT/2;
	DrawIconEx(dc,GetXBorder()+SMB_ICON_LEFT,nYPos,hIcon,SMB_ICON_WIDTH,SMB_ICON_HEIGHT,0,NULL,DI_NORMAL);

	//�滭����
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(10,10,10));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());
	dc.DrawText(m_strString,&m_rcString,DT_EXPANDTABS|DT_NOCLIP|DT_WORD_ELLIPSIS);

	return;
}

//ʱ����Ϣ
VOID CHintMsgBox::OnTimer(UINT_PTR nIDEvent)
{
	//����ʱ��
	if (nIDEvent==IDI_MESSAGE)
	{
		//ɾ���ж�
		if (m_nElapse==0)
		{
			PostMessage(WM_CLOSE,0,0);
			return;
		}

		//���ý���
		TCHAR szTitle[32]=TEXT("");
		_sntprintf(szTitle,CountArray(szTitle),TEXT("%s ��%ld �룩"),m_strCaption,m_nElapse--);
		SetWindowText(szTitle);

		return;
	}

	__super::OnTimer(nIDEvent);
}


//////////////////////////////////////////////////////////////////////////

//��ʾ��Ϣ
extern "C" CLIENT_SHARE_CLASS INT __cdecl ShowInformation(LPCTSTR pszString, UINT nElapse, UINT uType)
{
	CHintMsgBox HintMsgBox;

	return HintMsgBox.ShowMessageBox(pszString,uType,nElapse);
}

//��ʾ��Ϣ
extern "C" CLIENT_SHARE_CLASS INT __cdecl ShowInformationEx(LPCTSTR pszString, UINT nElapse, UINT uType ,LPCTSTR pszCaption)
{
	CHintMsgBox HintMsgBox;

	return HintMsgBox.ShowMessageBox(pszCaption,pszString,uType,nElapse);
}

//////////////////////////////////////////////////////////////////////////


