#include "Stdafx.h"
#include "GlobalUnits.h"
#include "ShortMessage.h"
#include "RoomViewItem.h"
#include ".\shortmessage.h"

#define CONTROL_SPACE						2							//�ؼ����
#define PIC_BT_WIDE							20							//��ť���
#define PIC_BT_HIGHT						20							//��ť�߶�
#define BT_BUTTON_WIDE						65							//��ť���
#define BT_BUTTON_HIGHT						21							//��ť�߶�

BEGIN_MESSAGE_MAP(CShortMessage, CSkinDialogEx)
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(WM_HIT_EXPMESSTION,OnExpression)
	ON_BN_CLICKED(IDC_SEND_SHORT, OnBnClickedSendShort)
	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_EXPRESSION, OnBnClickedExpression)
	ON_BN_CLICKED(IDC_OPEN_CHAT_MESSAGE, OnBnClickedOpenChatMsg) 
	ON_WM_PAINT()
END_MESSAGE_MAP()

//���캯��
CShortMessage::CShortMessage(CWnd * pParentWnd) : CSkinDialogEx(IDD_SHORT_MESSAGE,pParentWnd)
{
	m_pParentWnd=NULL;
	m_dwTargetUserID=0L;
	m_pParentWnd=pParentWnd;
	m_pTargetUserItem=NULL;
}

//��������
CShortMessage::~CShortMessage()
{
}

//�ؼ��󶨺���
void CShortMessage::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEND_SHORT, m_BtSend);
	DDX_Control(pDX, IDC_EXPRESSION, m_BtExpression);
	DDX_Control(pDX, IDC_CLOSE, m_BtClose);
	DDX_Control(pDX, IDC_SEND_MESSAGE, m_SendMessage);
	DDX_Control(pDX, IDC_RECV_MESSAGE, m_RecvMessage);
	DDX_Control(pDX, IDC_OPEN_CHAT_MESSAGE, m_BtOpenChatMsg) ;
}

//��ʼ������
BOOL CShortMessage::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();

	m_cbButtonState[BST_MAX]=BUT_EXIST|BUT_DISABLE;
	m_cbButtonState[BST_MIN]=BUT_EXIST|BUT_NORMAL;

	//���ñ���
	if (m_pTargetUserItem!=NULL)
	{
		TCHAR szTitle[256];
		_snprintf(szTitle,sizeof(szTitle),TEXT("[ %s ] �� [ %s ]"),m_pTargetUserItem->GetUserData()->szName,m_strRoomName);
		SetWindowText(szTitle);
	}

	//��Ϣ����
	if (m_MessageProxyHelper.CreateInstance()==false) return false;
	if (m_MessageProxyHelper->SetRichEditHwnd(&m_RecvMessage)==false) return false;

	//��ťƤ��
	m_BtExpression.SetButtonImage(g_GlobalUnits.m_WhisperViewImage.uBtExpression,g_GlobalUnits.m_PlatformResourceModule->GetResInstance(),false);

	//���ÿؼ�
	m_RecvMessage.SetBackgroundColor(FALSE,RGB(230,249,255));
	m_SendMessage.LimitText(MAX_CHAT_LEN);
	m_SendMessage.SetFocus();

	//��Ϣͷ
	TCHAR szTitle[128]=TEXT("");
	_snprintf(szTitle,CountArray(szTitle),TEXT("����ܰ��ʾ��"));
	m_MessageProxyHelper->InsertGeneralString(szTitle,RGB(255,0,0),MS_NORMAL,false);

	//Ԥ����Ϣ
	TCHAR szMessage[128] ;
	_snprintf(szMessage,CountArray(szMessage),TEXT("�ͷ���Ա�������κ���ʽ������ȡ�����ʻ���Ϣ���������߾��裬�������Լ��ʺ����룡"));
	m_MessageProxyHelper->InsertShtickString(szMessage,RGB(0,0,0),MS_NORMAL,true);

	return FALSE;
}

//������Ϣ
void CShortMessage::SetTalkInfo(IUserItem * pTargerUserItem, TCHAR * szRoomName)
{
	//���ñ���
	bool bCleanMessage=((m_dwTargetUserID!=0L)&&(m_dwTargetUserID!=pTargerUserItem->GetUserData()->dwUserID));
	m_strRoomName=szRoomName;
	m_pTargetUserItem=pTargerUserItem;
	m_strTargerName=m_pTargetUserItem->GetUserData()->szName;
	m_dwTargetUserID=m_pTargetUserItem->GetUserData()->dwUserID;

	//���ÿؼ�
	if (GetSafeHwnd()!=NULL)
	{
		//���ñ���
		TCHAR szTitle[256];
		_snprintf(szTitle,sizeof(szTitle),TEXT("[ %s ] �� [ %s ]"),m_strTargerName,m_strRoomName);
		SetWindowText(szTitle);

		//������Ļ
		if (bCleanMessage==true)
		{
			m_MessageProxyHelper->CleanScreen();
			m_SendMessage.SetWindowText(TEXT(""));
		}
		m_SendMessage.SetFocus();

		//�ػ��ؼ�
		Invalidate(FALSE);
	}

	//����Ŀ¼
	TCHAR szPath[MAX_PATH] ;
	if(GetCurrentDirectory(sizeof(szPath), szPath))
	{
		//��β�ж�
		if(szPath[strlen(szPath)-1]!='\\') strcat(szPath, TEXT("\\")) ;

		//���ID
		CString strUserID ;
		strUserID.Format(TEXT("%d"), g_GlobalUnits.GetGolbalUserData().dwUserID) ;

		//����Ŀ¼
		strcat(szPath, TEXT("�����¼\\")) ;
		strcat(szPath, strUserID) ;

		//����Ŀ¼
		WORD wDirectoryEnd=0 ;
		do
		{
			if(0==szPath[wDirectoryEnd])
			{
				CreateDirectory(szPath, NULL) ;
				break ;
			}
			if('\\'==szPath[wDirectoryEnd])
			{
				szPath[wDirectoryEnd]=0 ;
				CreateDirectory(szPath, NULL) ;
				szPath[wDirectoryEnd]='\\' ;
			}

			++wDirectoryEnd ;
		}while(1) ;
	}

	//�ļ�����
	CString strTargetUserID ;
	strTargetUserID.Format(TEXT("\\%d.TXT"), pTargerUserItem->GetUserID()) ;
	strcat(szPath, strTargetUserID) ;
	m_strMsgFileName = szPath ;

	return;
}

//ȷ����ť
void CShortMessage::OnOK()
{
	OnBnClickedSendShort();
}

//�˳���ť
void CShortMessage::OnBnClickedClose()
{
	DestroyWindow();
	return;
}

//ȡ����ť
void CShortMessage::OnCancel()
{
	DestroyWindow();

	return;
}

//���Ͱ�ť
void CShortMessage::OnBnClickedSendShort()
{
	//�ж���Ϣ
	m_SendMessage.SetFocus();
	if (m_SendMessage.GetWindowTextLength()==0)	return;
	
	try
	{
		//��ȡʱ��
		long int dwNowTime=(long int)time(NULL);

		//�ж�Ŀ�����
		ASSERT(m_dwTargetUserID!=0L);
		if ((m_pTargetUserItem==NULL)||(m_dwTargetUserID!=m_pTargetUserItem->GetUserData()->dwUserID))
		{
			m_pTargetUserItem=((CRoomViewItem *)m_pParentWnd)->m_ClientUserManager.SearchUserByUserID(m_dwTargetUserID);
			if (m_pTargetUserItem==NULL) m_MessageProxyHelper->InsertSystemString(TEXT("Ŀ������Ѿ��뿪����Ϣ����ʧ�ܣ�"),MS_NORMAL);
			return;
		}

		//��ȡ��Ϣ
		CString strBuffer;
		m_SendMessage.GetWindowText(strBuffer);

		//������Ϣ
		if (strBuffer.Find(TEXT("{\\rtf"))!=-1) throw TEXT("���ܷ��ͷǷ��ַ�������ʧ�ܣ�");
		for (int i=strBuffer.GetLength();i>0;i--)
			if ((strBuffer[i-1]!=TEXT('\n'))&&(strBuffer[i-1]!=TEXT('\r'))&&(strBuffer[i-1]>=-1)&&(strBuffer[i-1]<30)) throw TEXT("���ܷ��ͷǷ��ַ�������ʧ�ܣ�");

		//������Ϣ
		CMD_GR_Wisper Wisper;
		Wisper.dwTargetUserID=m_dwTargetUserID;
		Wisper.crFontColor=g_GlobalOption.m_crChatTX;
		Wisper.dwSendUserID=g_GlobalUnits.GetGolbalUserData().dwUserID;
		lstrcpyn(Wisper.szChatMessage,strBuffer,CountArray(Wisper.szChatMessage));
		Wisper.wChatLength=CountStringBuffer(Wisper.szChatMessage);

		//��������
		WORD wSendSize=sizeof(Wisper)-sizeof(Wisper.szChatMessage)+Wisper.wChatLength;
		((CRoomViewItem *)m_pParentWnd)->m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_WISPER,&Wisper,wSendSize);

		//���ò���
		m_SendMessage.SetWindowText(TEXT(""));
	}
	catch (TCHAR * szErrorMessage) { m_MessageProxyHelper->InsertSystemString(szErrorMessage,MS_NORMAL); }
	catch (...) { m_MessageProxyHelper->InsertSystemString(TEXT("δ֪�쳣������Ϣ����ʧ�ܣ�"),MS_NORMAL); }

	return;
}

//��Ϣ����
BOOL CShortMessage::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		if ((::GetAsyncKeyState(VK_LCONTROL)&0xFFF0)||(::GetAsyncKeyState(VK_RCONTROL)&0xFFF0))
		{
			OnBnClickedSendShort();
			return TRUE;
		}
	}
	return __super::PreTranslateMessage(pMsg);
}

//�滭����
void CShortMessage::OnPaint()
{
	CPaintDC dc(this); 

	//�滭����
	DrawCaption(&dc);

	CRect ClientRect;
	GetClientRect(&ClientRect);
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();

	//�滭����
	COLORREF Color = RGB(170,213,244);
	dc.FillSolidRect(nXExcursion,nYExcursion,ClientRect.Width()-2*nXExcursion,4,Color);
	dc.FillSolidRect(nXExcursion,ClientRect.Height()-8,ClientRect.Width()-2*nXExcursion,7,Color);
	dc.FillSolidRect(nXExcursion,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,Color);
	dc.FillSolidRect(ClientRect.Width()-nXExcursion-12,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,Color);
	CRect mRect(nXExcursion,190,ClientRect.Width()-2*nXExcursion,ClientRect.Height());
	CBrush Brush(Color); 
	dc.FillRect(mRect,&Brush);

	//�����ʻ�
	CPen BorderPen(PS_SOLID,1,RGB(20,20,20));
	CPen * pOldPen=dc.SelectObject(&BorderPen);
	dc.SelectObject(m_SkinAttribute.m_brBackGround);

	//�滭�ڿ�
	CRect rcDrawRect(nXExcursion,nYExcursion,ClientRect.Width()-2*nXExcursion,200);
	rcDrawRect.DeflateRect(6,4,6,4);
	dc.RoundRect(&rcDrawRect,CPoint(8,8));

	//������Դ
	dc.SelectObject(pOldPen);
	BorderPen.DeleteObject();

	//�����ʻ�
	CPen BorderPen1(PS_SOLID,1,m_SkinAttribute.m_crInsideBorder);
	pOldPen=dc.SelectObject(&BorderPen1);

	//�滭�ڿ�
	rcDrawRect.DeflateRect(1,1,1,1);
	dc.RoundRect(&rcDrawRect,CPoint(8,8));
	CRect rcDrawRect1(nXExcursion+9,203,ClientRect.Width()-2*nXExcursion-8,ClientRect.Height()-37);
	dc.RoundRect(&rcDrawRect1,CPoint(4,4));

	//������Դ
	dc.SelectObject(pOldPen);
	BorderPen1.DeleteObject();

	return;
}

//�յ���Ϣ
void CShortMessage::OnRecvMessage(TCHAR * szSendName, TCHAR * szMessage, COLORREF crColor, bool bMeMessage)
{
	//��ȡʱ��
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	//��Ϣͷ
	TCHAR szTitle[128]=TEXT("");
	_snprintf(szTitle,CountArray(szTitle),TEXT("%s %02d:%02d:%02d\n   "),szSendName,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
	m_MessageProxyHelper->InsertGeneralString(szTitle,(bMeMessage==true)?RGB(0,128,64):RGB(0,0,255),MS_NORMAL,false);

	//������Ϣ
	m_MessageProxyHelper->InsertShtickString(szMessage,crColor,MS_NORMAL,true);
	m_MessageProxyHelper->InsertGeneralString(TEXT(""),crColor,MS_NORMAL,true);

	//д���ļ�
	try
	{
		TCHAR szTitle[128]=TEXT("");
		_snprintf(szTitle,CountArray(szTitle),TEXT("[%s] %u/%u/%u %02d:%02d:%02d"),szSendName,SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);

		//д���ļ�
		CFile MsgFile;
		if(MsgFile.Open(m_strMsgFileName, CFile::modeWrite|CFile::modeNoTruncate|CFile::modeCreate))
		{
			MsgFile.SeekToEnd() ;
			MsgFile.Write(szTitle, UINT(strlen(szTitle))) ;
			MsgFile.Write("\r\n", UINT(strlen("\r\n"))) ;
		
			MsgFile.Write(szMessage, UINT(strlen(szMessage))) ;
			MsgFile.Write("\r\n\r\n", UINT(strlen("\r\n\r\n"))) ;
			MsgFile.Close() ;
		}

	}
	catch(...){}

	return;
}

//��ȡλ��
void CShortMessage::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	lpMMI->ptMinTrackSize.x=300;
	lpMMI->ptMinTrackSize.y=250;
	return;
}

//������Ϣ
LRESULT CShortMessage::OnExpression(WPARAM wparam, LPARAM lparam)
{
	CExpressionItem * pExpItem=m_ExpressWnd.GetExpressItem((UINT)wparam);
	if (pExpItem!=NULL)
	{
		CString strBuffer;
		m_SendMessage.GetWindowText(strBuffer);
		strBuffer+=pExpItem->m_szTrancelate;
		m_SendMessage.SetWindowText(strBuffer);
		m_SendMessage.SetFocus();
		m_SendMessage.SetSel(strBuffer.GetLength(),strBuffer.GetLength());	
	}
	return 0;
}

//���鰴ť
void CShortMessage::OnBnClickedExpression()
{
	//�������鴰��
	if (m_ExpressWnd.GetSafeHwnd()==NULL) m_ExpressWnd.CreateExpression(this);

	//�ƶ�����
	CRect ButtonRect;
	CSize ExpressSize;
	m_ExpressWnd.GetFixSize(ExpressSize);
	m_BtExpression.GetWindowRect(&ButtonRect);
	m_ExpressWnd.MoveWindow(ButtonRect.left,ButtonRect.top-ExpressSize.cy,ExpressSize.cx,ExpressSize.cy);
	m_ExpressWnd.ShowWindow(SW_SHOW);
	m_ExpressWnd.SetFocus();

	return;
}

//�򿪼�¼
void CShortMessage::OnBnClickedOpenChatMsg()
{
	try
	{
		//�򿪼�¼
		TCHAR szBuffer[128] ;
		strcpy(szBuffer, m_strMsgFileName) ;
		if(!m_strMsgFileName.IsEmpty())	ShellExecute(NULL, NULL, m_strMsgFileName, NULL, NULL, SW_SHOWNORMAL);
	}
	catch(...){}
}
