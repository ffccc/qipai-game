#include "StdAfx.h"
#include "Resource.h"
#include "Expression.h"
#include "RichEditMessage.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CRichEditMessage, CSkinRichEdit)
	ON_NOTIFY_REFLECT(EN_LINK, OnEventLink)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CRichEditMessage::CRichEditMessage()
{
	//��Դ����
	m_crFillColor=RGB(255,255,255);

	//���ñ���
	m_pExpressionManager=NULL;
	m_pIRichEditMessageSink=NULL;

	return;
}

//��������
CRichEditMessage::~CRichEditMessage()
{
}

//�ӿڲ�ѯ
VOID * CRichEditMessage::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IStringMessage,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IStringMessage,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CRichEditMessage::InsertUserEnter(LPCTSTR pszUserName)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//�����¼�
	InsertUserAccounts(pszUserName);
	InsertString(TEXT("������"),COLOR_EVENT);

	return true;
}

//�뿪�¼�
bool CRichEditMessage::InsertUserLeave(LPCTSTR pszUserName)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//�����¼�
	InsertUserAccounts(pszUserName);
	InsertString(TEXT("�뿪��"),COLOR_EVENT);

	return true;
}

//�����¼�
bool CRichEditMessage::InsertUserOffLine(LPCTSTR pszUserName)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//�����¼�
	InsertUserAccounts(pszUserName);
	InsertString(TEXT("������"),COLOR_WARN);

	return true;
}

//��ͨ��Ϣ
bool CRichEditMessage::InsertNormalString(LPCTSTR pszString)
{
	//������Ϣ
	InsertString(pszString,COLOR_NORMAL);

	return true;
}

//ϵͳ��Ϣ
bool CRichEditMessage::InsertSystemString(LPCTSTR pszString)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//����ͼƬ
	if (m_ImageDataSystem.GetBitmap()==NULL)
	{
		HINSTANCE hInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);
		LoadRichEditImage(hInstance,IDB_IMAGE_SYSTEM,m_ImageDataSystem);
	}

	//����ͼƬ
	InsertDataObject(&m_ImageDataSystem);

	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=COLOR_SYSTEM_STRING;
	CharFormat.yOffset=STRING_OFFSET;

	//������Ϣ
	InsertString(pszString,CharFormat);

	return true;
}

//��ʾ��Ϣ
bool CRichEditMessage::InsertPromptString(LPCTSTR pszString)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//����ͼƬ
	if (m_ImageDataPrompt.GetBitmap()==NULL)
	{
		HINSTANCE hInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);
		LoadRichEditImage(hInstance,IDB_IMAGE_PROMPT,m_ImageDataPrompt);
	}

	//����ͼƬ
	InsertDataObject(&m_ImageDataPrompt);

	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=COLOR_SYSTEM_STRING;
	CharFormat.yOffset=STRING_OFFSET;

	//������Ϣ
	InsertString(pszString,CharFormat);

	return true;
}

//������Ϣ
bool CRichEditMessage::InsertAfficheString(LPCTSTR pszString)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//����ͼƬ
	if (m_ImageDataAffiche.GetBitmap()==NULL)
	{
		HINSTANCE hInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);
		LoadRichEditImage(hInstance,IDB_IMAGE_AFFICHE,m_ImageDataAffiche);
	}
	
	//����ͼƬ
	InsertDataObject(&m_ImageDataAffiche);

	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=COLOR_SYSTEM_STRING;
	CharFormat.yOffset=STRING_OFFSET;

	//������Ϣ
	InsertString(pszString,CharFormat);

	return true;
}

//�Զ���Ϣ
bool CRichEditMessage::InsertCustomString(LPCTSTR pszString, COLORREF crColor)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//������Ϣ
	InsertString(pszString,crColor);

	return true;
}

//������Ϣ
bool CRichEditMessage::InsertCustomString(LPCTSTR pszString, COLORREF crColor, COLORREF crBackColor)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//������Ϣ
	InsertString(pszString,crColor,crBackColor);

	return true;
}

//�û�����
bool CRichEditMessage::InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//������Ϣ
	InsertUserAccounts(pszSendUser);
	InsertString(TEXT("˵��"),COLOR_EVENT);
	TranslateInsertString(pszString,crColor);

	return true;
}

//�û�����
bool CRichEditMessage::InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszString, COLORREF crColor)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//������Ϣ
	InsertUserAccounts(pszSendUser);
	InsertString(TEXT("��"),COLOR_EVENT);
	InsertUserAccounts(pszRecvUser);
	InsertString(TEXT("˵��"),COLOR_EVENT);
	TranslateInsertString(pszString,crColor);

	return true;
}

//�û�����
bool CRichEditMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath)
{
	//Ч�����
	ASSERT((pszImagePath!=NULL)&&(pszImagePath[0]!=0));
	if ((pszImagePath==NULL)||(pszImagePath[0]==0)) return false;

	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//������Ϣ
	InsertUserAccounts(pszSendUser);
	InsertString(TEXT("˵��"),COLOR_EVENT);

	//�������
	InsertImage(pszImagePath);

	return true;
}

//�û�����
bool CRichEditMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath, bool bMyselfString)
{
	//��ȡʱ��
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	//����ѡ��
	CHARRANGE CharRange;
	GetSel(CharRange);

	//����ǰ��
	TCHAR szTimeString[64]=TEXT("");
	_sntprintf(szTimeString,CountArray(szTimeString),TEXT("\r\n%s [ %02d:%02d:%02d ]\r\n"),pszSendUser,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
	InsertString(szTimeString,(bMyselfString==true)?COLOR_MYSELF:COLOR_USER);

	//��ʽ����
	PARAFORMAT2 ParaFormat;
	ZeroMemory(&ParaFormat,sizeof(ParaFormat));
	ParaFormat.cbSize=sizeof(ParaFormat);
	ParaFormat.dwMask=PFM_OFFSETINDENT;
	ParaFormat.dxStartIndent=200L;

	//��������
	SetSel(-1L,-1L);
	SetParaFormat(ParaFormat);

	//�������
	InsertImage(pszImagePath);
	InsertString(TEXT("\r\n"),RGB(255,255,255));

	//��ʽ����
	ParaFormat.dwMask=PFM_OFFSETINDENT;
	ParaFormat.dxStartIndent=-200L;

	//��ԭ��ʽ
	SetSel(-1L,-1L);
	SetParaFormat(ParaFormat);

	//�ָ���Ϣ
	if (CharRange.cpMax!=CharRange.cpMin) SetSel(CharRange);
	else PostMessage(WM_VSCROLL,SB_BOTTOM,0);

	return true;
}

//�û�����
bool CRichEditMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszImagePath)
{
	//Ч�����
	ASSERT((pszImagePath!=NULL)&&(pszImagePath[0]!=0));
	if ((pszImagePath==NULL)||(pszImagePath[0]==0)) return false;

	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//������Ϣ
	InsertUserAccounts(pszSendUser);
	InsertString(TEXT("��"),COLOR_EVENT);
	InsertUserAccounts(pszRecvUser);
	InsertString(TEXT("˵��"),COLOR_EVENT);

	//�������
	InsertImage(pszImagePath);

	return true;
}

//�û�˽��
bool CRichEditMessage::InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString)
{
	//��ȡʱ��
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	//����ѡ��
	CHARRANGE CharRange;
	GetSel(CharRange);

	//����ǰ��
	TCHAR szTimeString[64]=TEXT("");
	_sntprintf(szTimeString,CountArray(szTimeString),TEXT("\r\n%s [ %02d:%02d:%02d ]\r\n"),pszSendUser,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
	InsertString(szTimeString,(bMyselfString==true)?COLOR_MYSELF:COLOR_USER);

	//��ʽ����
	PARAFORMAT2 ParaFormat;
	ZeroMemory(&ParaFormat,sizeof(ParaFormat));
	ParaFormat.cbSize=sizeof(ParaFormat);
	ParaFormat.dwMask=PFM_OFFSETINDENT;
	ParaFormat.dxStartIndent=200L;

	//��������
	SetSel(-1L,-1L);
	SetParaFormat(ParaFormat);

	//�������
	TranslateInsertString(pszString,crColor);
	InsertString(TEXT("\r\n"),crColor);

	//��ʽ����
	ParaFormat.dwMask=PFM_OFFSETINDENT;
	ParaFormat.dxStartIndent=-200L;

	//��ԭ��ʽ
	SetSel(-1L,-1L);
	SetParaFormat(ParaFormat);

	//�ָ���Ϣ
	if (CharRange.cpMax!=CharRange.cpMin) SetSel(CharRange);
	else PostMessage(WM_VSCROLL,SB_BOTTOM,0);

	return true;
}

//�ؼ���
VOID CRichEditMessage::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//���ù���
	m_SkinScrollBar.InitScroolBar(this);

	//������ɫ
	SetBackgroundColor(FALSE,RGB(255,255,255));

	//���ÿؼ�
	SetEventMask(ENM_LINK);
	SetAutoURLDetect(FALSE);
	SetOptions(ECOOP_OR,ECO_AUTOWORDSELECTION);

	return;
}

//���ýӿ�
bool CRichEditMessage::SetRichEditMessageSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IRichEditMessageSink)!=NULL);
		m_pIRichEditMessageSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IRichEditMessageSink);

		//�ɹ��ж�
		if (m_pIRichEditMessageSink==NULL) return false;
	}
	else m_pIRichEditMessageSink=NULL;

	return true;
}

//�������
bool CRichEditMessage::SetExpressionManager(CExpressionManager * pExpressionManager, COLORREF crFillColor)
{
	//���ñ���
	m_crFillColor=crFillColor;
	m_pExpressionManager=pExpressionManager;

	return true;
}

//����ʱ��
bool CRichEditMessage::InsertSystemTime(COLORREF crColor)
{
	//��ȡʱ��
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	//������Ϣ
	TCHAR szTimeBuffer[64]=TEXT("");
	_sntprintf(szTimeBuffer,CountArray(szTimeBuffer),TEXT("02d:%02d:%02d"),SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);

	//������Ϣ
	InsertString(szTimeBuffer,crColor);

	return true;
}

//�û�����
bool CRichEditMessage::InsertUserAccounts(LPCTSTR pszAccounts)
{
	//Ч�����
	ASSERT(pszAccounts!=NULL);
	if (pszAccounts==NULL) return false;

	//��������
	InsertString(TEXT("��"),COLOR_NORMAL);
	InsertHyperLink(pszAccounts);
	InsertString(TEXT("��"),COLOR_NORMAL);

	return true;
}

//�����ַ�
bool CRichEditMessage::TranslateInsertString(LPCTSTR pszString, COLORREF crColor)
{
	//�����ж�
	if (m_pExpressionManager==NULL) return false;

	//��������
	tagTranslateResult TranslateResult;
	ZeroMemory(&TranslateResult,sizeof(TranslateResult));

	//�ַ�����
	INT nTranslateIndex=0;
	INT nStringLength=lstrlen(pszString);

	while ((nStringLength-nTranslateIndex)>0)
	{
		//�����ַ�
		bool bSuccess=m_pExpressionManager->TranslateString(&pszString[nTranslateIndex],TranslateResult);

		//�������
		if (bSuccess==true)
		{
			//�����ַ�
			if (TranslateResult.nStartPos>0)
			{
				//��������
				TCHAR szString[LEN_USER_CHAT]=TEXT("");
				WORD wCopyCount=__min(TranslateResult.nStartPos,CountArray(szString));

				//�����ַ�
				szString[wCopyCount]=0;
				CopyMemory(szString,&pszString[nTranslateIndex],wCopyCount*sizeof(TCHAR));

				//�����ַ�
				InsertString(szString,crColor);
			}

			//����·��
			TCHAR ImagePath[MAX_PATH]=TEXT("");
			TranslateResult.pExpressionItem->GetExpressionPath(ImagePath,CountArray(ImagePath));

			//�������
			InsertImage(ImagePath);

			//��������
			nTranslateIndex+=TranslateResult.nStartPos+TranslateResult.nDescribeLen;
		}
		else
		{
			//�����ַ�
			InsertString(&pszString[nTranslateIndex],crColor);
			break;
		}

	}

	return true;
}

//����ͼƬ
bool CRichEditMessage::LoadRichEditImage(HINSTANCE hInstance, UINT uResourceID, CDataObject&ImageDataObject)
{
	//����ͼƬ
	CBitImage ImageFile;
	ImageFile.LoadFromResource(hInstance,uResourceID);

	//ʧ���ж�
	if (ImageFile.IsNull()==true)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	CImage ImageRichEdit;
	ImageRichEdit.Create(ImageFile.GetWidth(),ImageFile.GetHeight(),32);

	//�滭����
	CDC * pDC=CDC::FromHandle(ImageRichEdit.GetDC());
	pDC->FillSolidRect(0,0,ImageFile.GetWidth(),ImageFile.GetHeight(),m_crFillColor);
	ImageFile.TransDrawImage(pDC,0,0,RGB(255,0,255));

	//����ͼƬ
	ImageRichEdit.ReleaseDC();
	ImageDataObject.SetImage(ImageRichEdit.Detach());

	return true;
}

//��������
VOID CRichEditMessage::OnEventLink(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	ENLINK * pEnLink=reinterpret_cast<ENLINK *>(pNMHDR);

	//�����Ϣ
	if ((m_pIRichEditMessageSink!=NULL)&&((pEnLink->msg==WM_LBUTTONDOWN)||(pEnLink->msg==WM_LBUTTONDBLCLK)))
	{
		//��������
		CString strString;
		TEXTRANGE TextRange;

		//���ȼ���
		LONG lLastIndex=pEnLink->chrg.cpMin;
		LONG lFirstIndex=pEnLink->chrg.cpMax;

		//�������
		TextRange.chrg.cpMin=pEnLink->chrg.cpMin;
		TextRange.chrg.cpMax=pEnLink->chrg.cpMax;
		TextRange.lpstrText=strString.GetBuffer((lFirstIndex-lLastIndex+1)*2L);

		//��ȡ�ַ�
		LONG lLength=(LONG)SendMessage(EM_GETTEXTRANGE,0,(LPARAM)&TextRange);

		//���ó���
		strString.ReleaseBuffer(lLength);

		//�¼�֪ͨ
		CA2CT strTextRange((LPCSTR)(LPCTSTR)strString);
		m_pIRichEditMessageSink->OnEventStringLink((LPCTSTR)strTextRange,pEnLink->msg);
	}

	//���ñ���
	*pResult=0;

	return;
}

//////////////////////////////////////////////////////////////////////////////////
