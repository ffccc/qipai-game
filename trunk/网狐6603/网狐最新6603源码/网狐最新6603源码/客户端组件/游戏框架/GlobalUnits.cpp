#include "StdAfx.h"
#include "Mmsystem.h"
#include "GlobalUnits.h"

//////////////////////////////////////////////////////////////////////////////////

//�ؼ���ɫ
#define OPTION_CHAT_TX				RGB(0,0,0)							//��������

//��̬����
CGlobalUnits * CGlobalUnits::m_pGlobalUnits=NULL;						//ȫ�ֵ�Ԫ

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalUnits::CGlobalUnits()
{
	//Ĭ�ϲ���
	DefaultParameter();

	//���ö���
	ASSERT(m_pGlobalUnits==NULL);
	if (m_pGlobalUnits==NULL) m_pGlobalUnits=this;

	return;
}

//��������
CGlobalUnits::~CGlobalUnits()
{
	//���ö���
	ASSERT(m_pGlobalUnits==this);
	if (m_pGlobalUnits==this) m_pGlobalUnits=NULL;

	//ɾ�����
	m_ClientKernelModule.CloseInstance();
	m_FaceItemControlModule.CloseInstance();
	m_GameLevelParserModule.CloseInstance();

	return;
}

//�������
bool CGlobalUnits::InitGlobalUnits()
{
	//���ز���
	LoadParameter();

	//������Դ
	m_ExpressionManager.LoadExpression();
	m_SkinResourceManager.SetSkinResource(m_SkinRenderInfo);

	//�������
	if (m_ClientKernelModule.CreateInstance()==false) return false;
	if (m_FaceItemControlModule.CreateInstance()==false) return false;
	if (m_GameLevelParserModule.CreateInstance()==false) return false;
	if (m_UserOrderParserModule.CreateInstance()==false) return false;

	//ע�����
	RegisterGlobalModule(MODULE_CLIENT_KERNEL,m_ClientKernelModule.GetInterface());
	RegisterGlobalModule(MODULE_FACE_ITEM_CONTROL,m_FaceItemControlModule.GetInterface());
	RegisterGlobalModule(MODULE_GAME_LEVEL_PARSER,m_GameLevelParserModule.GetInterface());

	//�û�Ԫ��
	m_UserItemElement.SetUserOrderParser(m_UserOrderParserModule.GetInterface());
	m_UserItemElement.SetFaceItemControl(m_FaceItemControlModule.GetInterface());

	//�����ں�
	if (m_ClientKernelModule->InitClientKernel(AfxGetApp()->m_lpCmdLine)==false) return false;

	return true;
}

//ע�����
bool CGlobalUnits::UnInitGlobalUnits()
{
	//�������
	SaveParameter();

	//�������
	if (m_ClientKernelModule.GetInterface()!=NULL) m_ClientKernelModule.CloseInstance();
	if (m_FaceItemControlModule.GetInterface()!=NULL) m_FaceItemControlModule.CloseInstance();
	if (m_GameLevelParserModule.GetInterface()!=NULL) m_GameLevelParserModule.CloseInstance();

	return true;
}

//���ز���
VOID CGlobalUnits::LoadParameter()
{
	//��������
	CWHRegKey RegOptionItem;

	//��ȡ����
	if (RegOptionItem.OpenRegKey(TEXT("GameFrame"),TEXT("Option"),false)==true)
	{
		//��ɫ����
		m_crChatTX=RegOptionItem.GetValue(TEXT("crChatTX"),OPTION_CHAT_TX);

		//��������
		m_bAllowSound=RegOptionItem.GetValue(TEXT("AllowSound"),TRUE)?true:false;
		m_bFullGameView=RegOptionItem.GetValue(TEXT("FullGameView"),FALSE)?true:false;
		m_bNotifyUserInOut=RegOptionItem.GetValue(TEXT("NotifyUserInOut"),TRUE)?true:false;

		//�Թ�ѡ��
		m_bAllowLookon=RegOptionItem.GetValue(TEXT("AllowLookon"),TRUE)?true:false;
		m_bAllowFriendLookon=RegOptionItem.GetValue(TEXT("AllowFriendLookon"),TRUE)?true:false;

		//��Ϣģʽ
		m_cbMessageMode=(BYTE)RegOptionItem.GetValue(TEXT("MessageMode"),m_cbMessageMode);
		switch (m_cbMessageMode)
		{
		case MESSAGE_MODE_ALL:
		case MESSAGE_MODE_FRIEND: { break; }
		default: { m_cbMessageMode=MESSAGE_MODE_DETEST; }
		}

		//������Դ
		m_SkinRenderInfo.wColorH=(WORD)RegOptionItem.GetValue(TEXT("RenderColorH"),m_SkinRenderInfo.wColorH);
		m_SkinRenderInfo.dRectifyS=(DOUBLE)(RegOptionItem.GetValue(TEXT("RenderColorS"),(INT)((m_SkinRenderInfo.dRectifyS+1.0)*100)))/100.0-1.0;
		m_SkinRenderInfo.dRectifyB=(DOUBLE)(RegOptionItem.GetValue(TEXT("RenderColorB"),(INT)((m_SkinRenderInfo.dRectifyB+1.0)*100)))/100.0-1.0;
	}

	return;
}

//�������
VOID CGlobalUnits::SaveParameter()
{
	//��������
	CWHRegKey RegOptionItem;

	//��������
	if (RegOptionItem.OpenRegKey(TEXT("GameFrame"),TEXT("Option"),true)==true)
	{
		//��ɫ����
		RegOptionItem.WriteValue(TEXT("crChatTX"),m_crChatTX);

		//ģʽ����
		RegOptionItem.WriteValue(TEXT("MessageMode"),m_cbMessageMode);

		//��������
		RegOptionItem.WriteValue(TEXT("AllowSound"),m_bAllowSound);
		RegOptionItem.WriteValue(TEXT("FullGameView"),m_bFullGameView);
		RegOptionItem.WriteValue(TEXT("NotifyUserInOut"),m_bNotifyUserInOut);

		//�Թ�����
		RegOptionItem.WriteValue(TEXT("AllowLookon"),m_bAllowLookon);
		RegOptionItem.WriteValue(TEXT("AllowFriendLookon"),m_bAllowFriendLookon);

		//������Դ
		RegOptionItem.WriteValue(TEXT("RenderColorH"),m_SkinRenderInfo.wColorH);
		RegOptionItem.WriteValue(TEXT("RenderColorS"),(DWORD)(m_SkinRenderInfo.dRectifyS*100+100));
		RegOptionItem.WriteValue(TEXT("RenderColorB"),(DWORD)(m_SkinRenderInfo.dRectifyB*100+100));
	}

	return;
}

//Ĭ�ϲ���
VOID CGlobalUnits::DefaultParameter()
{
	//����ѡ��
	m_bAllowSound=true;
	m_bFullGameView=false;
	m_bNotifyUserInOut=true;

	//�Թ�ѡ��
	m_bAllowLookon=false;
	m_bAllowFriendLookon=true;

	//��ɫ����
	m_crChatTX=OPTION_CHAT_TX;

	//ģʽ����
	m_cbMessageMode=MESSAGE_MODE_DETEST;

	//������Դ
	m_SkinRenderInfo.wColorH=DEF_COLOR_H;
	m_SkinRenderInfo.dRectifyS=DEF_COLOR_S;
	m_SkinRenderInfo.dRectifyB=DEF_COLOR_B;

	return;
}

//��������
bool CGlobalUnits::PlayGameSound(LPCTSTR pszSoundName)
{
	//�����ж�
	if (m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//��ȡ����
	CD3DSound * pD3DSound=CD3DSound::GetInstance();

	//��������
	if ((pD3DSound->PlaySound(pszSoundName,false))==false)
	{
		//�������
		ASSERT(FALSE);

		//Ĭ�ϲ���
		PlaySound(pszSoundName,AfxGetInstanceHandle(),SND_ASYNC|SND_NODEFAULT);
	}

	return true;
}

//��������
bool CGlobalUnits::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	//�����ж�
	if (m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//��ȡ����
	CD3DSound * pD3DSound=CD3DSound::GetInstance();

	//��������
	if ((pD3DSound->PlaySound(hInstance,pszSoundName,TEXT("WAVE"),false))==false)
	{
		//�������
		ASSERT(FALSE);

		//������Դ
		HRSRC hResource=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
		if (hResource==NULL) return false;

		//������Դ
		HGLOBAL hGlobal=LoadResource(hInstance,hResource);
		if (hGlobal==NULL) return false;

		//��������
		LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
		PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY);

		//������Դ
		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}

	return true;
}

//ע�����
bool CGlobalUnits::RegisterGlobalModule(WORD wModuleID, IUnknownEx * pIUnknownEx)
{
	//Ч�����
	ASSERT((wModuleID!=0L)&&(pIUnknownEx!=NULL));
	if ((wModuleID==0L)||(pIUnknownEx==NULL)) return false;

	//�������
	IUnknownEx * pIUnknownExMap=NULL;
	if (m_GlobalModuleMap.Lookup(wModuleID,pIUnknownExMap)==TRUE) 
	{
		ASSERT(FALSE);
		return false;
	}

	//ע�����
	m_GlobalModuleMap[wModuleID]=pIUnknownEx;

	return true;
}

//��ѯ���
VOID * CGlobalUnits::QueryGlobalModule(WORD wModuleID, REFGUID Guid, DWORD dwQueryVer)
{
	//�������
	IUnknownEx * pIUnknownEx=NULL;
	if (m_GlobalModuleMap.Lookup(wModuleID,pIUnknownEx)==FALSE)
	{
		ASSERT(FALSE);
		return NULL;
	}

	//��ѯ�ӿ�
	return pIUnknownEx->QueryInterface(Guid,dwQueryVer);
}

//////////////////////////////////////////////////////////////////////////////////

//ȫ�ֵ�Ԫ
CGlobalUnits						g_GlobalUnits;						//ȫ�ֵ�Ԫ

//////////////////////////////////////////////////////////////////////////////////
