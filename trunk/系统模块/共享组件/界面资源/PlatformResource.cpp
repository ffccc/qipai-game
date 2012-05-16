#include "StdAfx.h"
#include "Resource.h"
#include "PlatformResource.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CPlatformResource::CPlatformResource()
{
	//��Դ����
	m_pPlazaViewImage =NULL;
	m_pServerViewImage =NULL;
	m_pPlatformFrameImage=NULL;
	m_pWhisperViewImage =NULL;
	m_pControlViewImage =NULL;
	m_pPropertyViewImage =NULL;
	m_pChatExpViewImage=NULL;
	m_pGameFrameImage=NULL;
	m_pVideoFrameImage =NULL;
	m_pPropertyMenuImage =NULL;
	m_pGiftFlashImage =NULL;

	//��Դ����
	m_cbResourceType=0;

	//�ں˱���
	m_hInstance=GetModuleHandle(SKIN_RESOURCE_DLL_NAME);

	return;
}

//��������
CPlatformResource::~CPlatformResource()
{
}

//�ӿڲ�ѯ
VOID * __cdecl CPlatformResource::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IPlatformResource,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IPlatformResource,Guid,dwQueryVer);
	return NULL;
}

//ʵ�����
HINSTANCE __cdecl CPlatformResource::GetResInstance()
{
	return m_hInstance;
}

//��ȡԤ��
bool __cdecl CPlatformResource::GetSkinPreview(CImage & ImagePreview)
{
	//������Դ
	ImagePreview.LoadFromResource(m_hInstance,DEF_IDB_SKIN_PREVIEW);

	return true;
}

//��Դ����
void __cdecl CPlatformResource::SetResourceType(BYTE cbResourceType)
{
	m_cbResourceType = cbResourceType;
	m_SkinResource.m_cbResourceType = cbResourceType;

	//��ȡ��Դ
	GetPlazaViewImage(m_pPlazaViewImage);
	GetServerViewImage(m_pServerViewImage);
	GetPlatformFrameImage(m_pPlatformFrameImage);
	GetWhisperViewImage(m_pWhisperViewImage);
	GetControlViewImage(m_pControlViewImage);
	GetPropertyViewImage(m_pPropertyViewImage);
	GetChatExpViewImage(m_pChatExpViewImage);
	GetGameFrameImage(m_pGameFrameImage);
	GetVideoFrameImage(m_pVideoFrameImage);
	GetPropertyMenuImage(m_pPropertyMenuImage);
	GetGiftFlashImage(m_pGiftFlashImage);

	return;
}

//������Դ
VOID * __cdecl CPlatformResource::GetSkinResource(REFGUID Guid, DWORD dwQueryVer)
{
	return m_SkinResource.QueryInterface(Guid,dwQueryVer);
}

//�㳡��Դ
bool __cdecl CPlatformResource::GetPlazaViewImage(tagPlazaViewImage * PlazaViewImage)
{
	if(PlazaViewImage==NULL)return false;
	if(m_pPlazaViewImage==NULL)m_pPlazaViewImage = PlazaViewImage;

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			//�����Դ
			PlazaViewImage->uBrowseML=DEF_IDB_BROWSE_ML;
			PlazaViewImage->uBrowseMR=DEF_IDB_BROWSE_MR;
			PlazaViewImage->uBrowseTL=DEF_IDB_BROWSE_TL;
			PlazaViewImage->uBrowseTM=DEF_IDB_BROWSE_TM;
			PlazaViewImage->uBrowseTR=DEF_IDB_BROWSE_TR;
			PlazaViewImage->uBrowseBL=DEF_IDB_BROWSE_BL;
			PlazaViewImage->uBrowseBM=DEF_IDB_BROWSE_BM;
			PlazaViewImage->uBrowseBR=DEF_IDB_BROWSE_BR;

			//��ť��Դ
			PlazaViewImage->uBtWebLast=DEF_IDB_BT_WEB_LAST;
			PlazaViewImage->uBtWebNext=DEF_IDB_BT_WEB_NEXT;
			PlazaViewImage->uBtWebStop=DEF_IDB_BT_WEB_STOP;
			PlazaViewImage->uBtWebHome=DEF_IDB_BT_WEB_HOME;
			PlazaViewImage->uBtWebReload=DEF_IDB_BT_WEB_RELOAD;
			break;
		}
	case RESOURCE_BLACK:
		{
			//�����Դ
			PlazaViewImage->uBrowseML=BLACK_IDB_BROWSE_ML;
			PlazaViewImage->uBrowseMR=BLACK_IDB_BROWSE_MR;
			PlazaViewImage->uBrowseTL=BLACK_IDB_BROWSE_TL;
			PlazaViewImage->uBrowseTM=BLACK_IDB_BROWSE_TM;
			PlazaViewImage->uBrowseTR=BLACK_IDB_BROWSE_TR;
			PlazaViewImage->uBrowseBL=BLACK_IDB_BROWSE_BL;
			PlazaViewImage->uBrowseBM=BLACK_IDB_BROWSE_BM;
			PlazaViewImage->uBrowseBR=BLACK_IDB_BROWSE_BR;

			//��ť��Դ
			PlazaViewImage->uBtWebLast=BLACK_IDB_BT_WEB_LAST;
			PlazaViewImage->uBtWebNext=BLACK_IDB_BT_WEB_NEXT;
			PlazaViewImage->uBtWebStop=BLACK_IDB_BT_WEB_STOP;
			PlazaViewImage->uBtWebHome=BLACK_IDB_BT_WEB_HOME;
			PlazaViewImage->uBtWebReload=BLACK_IDB_BT_WEB_RELOAD;
			break;
		}
	case RESOURCE_RED:
		{
			//�����Դ
			PlazaViewImage->uBrowseML=RED_IDB_BROWSE_ML;
			PlazaViewImage->uBrowseMR=RED_IDB_BROWSE_MR;
			PlazaViewImage->uBrowseTL=RED_IDB_BROWSE_TL;
			PlazaViewImage->uBrowseTM=RED_IDB_BROWSE_TM;
			PlazaViewImage->uBrowseTR=RED_IDB_BROWSE_TR;
			PlazaViewImage->uBrowseBL=RED_IDB_BROWSE_BL;
			PlazaViewImage->uBrowseBM=RED_IDB_BROWSE_BM;
			PlazaViewImage->uBrowseBR=RED_IDB_BROWSE_BR;

			//��ť��Դ
			PlazaViewImage->uBtWebLast=RED_IDB_BT_WEB_LAST;
			PlazaViewImage->uBtWebNext=RED_IDB_BT_WEB_NEXT;
			PlazaViewImage->uBtWebStop=RED_IDB_BT_WEB_STOP;
			PlazaViewImage->uBtWebHome=RED_IDB_BT_WEB_HOME;
			PlazaViewImage->uBtWebReload=RED_IDB_BT_WEB_RELOAD;
			break;
		}
	}

	return true;
}

//������Դ
bool __cdecl CPlatformResource::GetServerViewImage(tagServerViewImage * ServerViewImage)
{
	if(ServerViewImage==NULL)return false;
	if(m_pServerViewImage==NULL)m_pServerViewImage = ServerViewImage;

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//������Դ
	ServerViewImage->uChatML=DEF_IDB_CHAT_ML;
	ServerViewImage->uChatMR=DEF_IDB_CHAT_MR;
	ServerViewImage->uChatTL=DEF_IDB_CHAT_TL;
	ServerViewImage->uChatTM=DEF_IDB_CHAT_TM;
	ServerViewImage->uChatTR=DEF_IDB_CHAT_TR;
	ServerViewImage->uChatBL=DEF_IDB_CHAT_BL;
	ServerViewImage->uChatBM=DEF_IDB_CHAT_BM;
	ServerViewImage->uChatBR=DEF_IDB_CHAT_BR;

	//������Դ
	ServerViewImage->uTableML=DEF_IDB_TABLE_ML;
	ServerViewImage->uTableMR=DEF_IDB_TABLE_MR;
	ServerViewImage->uTableTL=DEF_IDB_TABLE_TL;
	ServerViewImage->uTableTM=DEF_IDB_TABLE_TM;
	ServerViewImage->uTableTR=DEF_IDB_TABLE_TR;
	ServerViewImage->uTableBL=DEF_IDB_TABLE_BL;
	ServerViewImage->uTableBM=DEF_IDB_TABLE_BM;
	ServerViewImage->uTableBR=DEF_IDB_TABLE_BR;

	//�б���Դ
	ServerViewImage->uUserListML=DEF_IDB_USER_LIST_ML;
	ServerViewImage->uUserListMR=DEF_IDB_USER_LIST_MR;
	ServerViewImage->uUserListTL=DEF_IDB_USER_LIST_TL;
	ServerViewImage->uUserListTM=DEF_IDB_USER_LIST_TM;
	ServerViewImage->uUserListTR=DEF_IDB_USER_LIST_TR;
	ServerViewImage->uUserListBL=DEF_IDB_USER_LIST_BL;
	ServerViewImage->uUserListBM=DEF_IDB_USER_LIST_BM;
	ServerViewImage->uUserListBR=DEF_IDB_USER_LIST_BR;

	//���찴ť
	ServerViewImage->uBtBugle=DEF_IDB_BT_BUGLE;
	ServerViewImage->uBtColorSet=DEF_IDB_BT_COLOR_SET;
	ServerViewImage->uBtExpression=DEF_IDB_BT_EXPESSION;
	ServerViewImage->uBtSendChat=DEF_IDB_BT_SEND_CHAT;
	ServerViewImage->uBtCleanScreen=DEF_IDB_BT_CLEAN_SCREEN;

	//���Ӱ�ť
	ServerViewImage->uBtAutoJoin=DEF_IDB_BT_AUTO_JOIN;
	ServerViewImage->uBtSearchUser=DEF_IDB_BT_SEARCH_USER;
	ServerViewImage->uBtQuitRoom=DEF_IDB_BT_QUIT_ROOM;
	ServerViewImage->uBtBank=DEF_IDB_BT_BANK;

	//�л���ť
	ServerViewImage->uBtShowUserList=DEF_IDB_SHOW_USER_LIST;
	ServerViewImage->uBtHideUserList=DEF_IDB_HIDE_USER_LIST;

	return true;
}

//�����Դ
bool __cdecl CPlatformResource::GetPlatformFrameImage(tagPlatformFrameImage * PlatformFrameImage)
{
	if(PlatformFrameImage==NULL)return false;
	if(m_pPlatformFrameImage==NULL)m_pPlatformFrameImage = PlatformFrameImage;

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//�����Դ
	PlatformFrameImage->uFrameML=DEF_IDB_FRAME_ML;
	PlatformFrameImage->uFrameMR=DEF_IDB_FRAME_MR;
	PlatformFrameImage->uFrameTL=DEF_IDB_FRAME_TL;
	PlatformFrameImage->uFrameTM=DEF_IDB_FRAME_TM;
	PlatformFrameImage->uFrameTR=DEF_IDB_FRAME_TR;
	PlatformFrameImage->uFrameBL=DEF_IDB_FRAME_BL;
	PlatformFrameImage->uFrameBM=DEF_IDB_FRAME_BM;
	PlatformFrameImage->uFrameBR=DEF_IDB_FRAME_BR;

	//�б���Դ
	PlatformFrameImage->uServerML=DEF_IDB_SERVER_ML;
	PlatformFrameImage->uServerMR=DEF_IDB_SERVER_MR;
	PlatformFrameImage->uServerTL=DEF_IDB_SERVER_TL;
	PlatformFrameImage->uServerTM=DEF_IDB_SERVER_TM;
	PlatformFrameImage->uServerTR=DEF_IDB_SERVER_TR;
	PlatformFrameImage->uServerBL=DEF_IDB_SERVER_BL;
	PlatformFrameImage->uServerBM=DEF_IDB_SERVER_BM;
	PlatformFrameImage->uServerBR=DEF_IDB_SERVER_BR;

	//��ܰ�ť
	PlatformFrameImage->uBtFrameMin=DEF_IDB_FRAME_BT_MIN;
	PlatformFrameImage->uBtFrameClose=DEF_IDB_FRAME_BT_CLOSE;
	PlatformFrameImage->uBtFrameMax=DEF_IDB_FRAME_BT_MAX;
	PlatformFrameImage->uBtFrameResore=DEF_IDB_FRAME_BT_RESORE;
	PlatformFrameImage->uBtFrameSelfSet=DEF_IDB_FRAME_BT_SELF_SET;
	PlatformFrameImage->uBtFrameSwitch=DEF_IDB_FRAME_BT_SWITCH;
	PlatformFrameImage->uBtFrameOption=DEF_IDB_FRAME_BT_OPTION;
	PlatformFrameImage->uFrameServiceBtHide1=DEF_IDB_SERVER_BT_HIDE_1;
	PlatformFrameImage->uFrameServiceBtHide2=DEF_IDB_SERVER_BT_HIDE_2;
	PlatformFrameImage->uFrameServiceBtShow1=DEF_IDB_SERVER_BT_SHOW_1;
	PlatformFrameImage->uFrameServiceBtShow2=DEF_IDB_SERVER_BT_SHOW_2;

	//�л���ť
	PlatformFrameImage->uBtShowGameList=DEF_IDB_SHOW_GAME_LIST;
	PlatformFrameImage->uBtHideGameList=DEF_IDB_HIDE_GAME_LIST;
	PlatformFrameImage->uBtFrameItemNormal=DEF_IDB_BT_ITEM_NORMAL;
	PlatformFrameImage->uBtFrameItemActive=DEF_IDB_BT_ITEM_AVTIVE;

	//������ť
	PlatformFrameImage->uFrameNavigation[0]=DEF_IDB_FRAME_BT_NAVIGATION_1;
	PlatformFrameImage->uFrameNavigation[1]=DEF_IDB_FRAME_BT_NAVIGATION_2;
	PlatformFrameImage->uFrameNavigation[2]=DEF_IDB_FRAME_BT_NAVIGATION_3;
	PlatformFrameImage->uFrameNavigation[3]=DEF_IDB_FRAME_BT_NAVIGATION_4;
	PlatformFrameImage->uFrameNavigation[4]=DEF_IDB_FRAME_BT_NAVIGATION_5;

	return true;
}

//˽����Դ
bool __cdecl CPlatformResource::GetWhisperViewImage(tagWhisperViewImage * WhisperViewImage)
{
	if(WhisperViewImage==NULL)return false;
	if(m_pWhisperViewImage==NULL)m_pWhisperViewImage = WhisperViewImage;

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//������Դ
	WhisperViewImage->uChatML=DEF_IDB_CHAT_WHISPER_ML;
	WhisperViewImage->uChatMR=DEF_IDB_CHAT_WHISPER_MR;
	WhisperViewImage->uChatTL=DEF_IDB_CHAT_WHISPER_TL;
	WhisperViewImage->uChatTM=DEF_IDB_CHAT_WHISPER_TM;
	WhisperViewImage->uChatTR=DEF_IDB_CHAT_WHISPER_TR;
	WhisperViewImage->uChatBL=DEF_IDB_CHAT_WHISPER_BL;
	WhisperViewImage->uChatBM=DEF_IDB_CHAT_WHISPER_BM;
	WhisperViewImage->uChatBR=DEF_IDB_CHAT_WHISPER_BR;

	//�û���Դ
	WhisperViewImage->uUserML=DEF_IDB_USER_WHISPER_ML;
	WhisperViewImage->uUserMR=DEF_IDB_USER_WHISPER_MR;
	WhisperViewImage->uUserTL=DEF_IDB_USER_WHISPER_TL;
	WhisperViewImage->uUserTM=DEF_IDB_USER_WHISPER_TM;
	WhisperViewImage->uUserTR=DEF_IDB_USER_WHISPER_TR;
	WhisperViewImage->uUserBL=DEF_IDB_USER_WHISPER_BL;
	WhisperViewImage->uUserBM=DEF_IDB_USER_WHISPER_BM;
	WhisperViewImage->uUserBR=DEF_IDB_USER_WHISPER_BR;

	//������Դ
	WhisperViewImage->uInputML=DEF_IDB_INPUT_WHISPER_ML;
	WhisperViewImage->uInputMR=DEF_IDB_INPUT_WHISPER_MR;
	WhisperViewImage->uInputTL=DEF_IDB_INPUT_WHISPER_TL;
	WhisperViewImage->uInputTM=DEF_IDB_INPUT_WHISPER_TM;
	WhisperViewImage->uInputTR=DEF_IDB_INPUT_WHISPER_TR;
	WhisperViewImage->uInputBL=DEF_IDB_INPUT_WHISPER_BL;
	WhisperViewImage->uInputBM=DEF_IDB_INPUT_WHISPER_BM;
	WhisperViewImage->uInputBR=DEF_IDB_INPUT_WHISPER_BR;

	//��ť��Դ
	WhisperViewImage->uBtAddUser=DEF_IDB_BT_WHISPER_ADD_USER;
	WhisperViewImage->uBtColorSet=DEF_IDB_BT_WHISPER_COLOR_SET;
	WhisperViewImage->uBtLeaveChat=DEF_IDB_BT_WHISPER_LEAVE_CHAT;
	WhisperViewImage->uBtExpression=DEF_IDB_BT_WHISPER_EXPESSION;
	WhisperViewImage->uBtCleanScreen=DEF_IDB_BT_WHISPER_CLEAN_SCREEN;
	WhisperViewImage->uBtMoreFunction=DEF_IDB_BT_WHISPER_MORE_FUNCTION;

	//���ư�ť
	WhisperViewImage->uBtWhisper=DEF_IDB_BT_WHISPER;
	WhisperViewImage->uBtSendControl=DEF_IDB_BT_WHISPER_SEND_CONTROL;

	//������Դ
	WhisperViewImage->uChatSplitter=DEF_IDB_WHISPER_SPLITTER_CHAT;

	return true;
}

//�ؼ���Դ
bool __cdecl CPlatformResource::GetControlViewImage(tagControlViewImage * ControlViewImage)
{
	if(ControlViewImage==NULL)return false;
	if(m_pControlViewImage==NULL)m_pControlViewImage = ControlViewImage;

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//�û���Ϣ
	ControlViewImage->uUserInfoL=DEF_IDB_USER_INFO_L;
	ControlViewImage->uUserInfoM=DEF_IDB_USER_INFO_M;
	ControlViewImage->uUserInfoR=DEF_IDB_USER_INFO_R;

	//������Դ
	ControlViewImage->uCollocateBarActive=DEF_IDB_BAR_OP_ACTIVE;
	ControlViewImage->uCollocateBarBackground=DEF_IDB_BAR_OP_BACKGROUND;
	ControlViewImage->uCollocateBarHot=DEF_IDB_BAR_OP_HOT;	
	ControlViewImage->uCollocateBarNormal=DEF_IDB_BAR_OP_NORMAL;

	return true;
}

//������Դ
bool __cdecl CPlatformResource::GetPropertyViewImage(tagPropertyViewImage * PropertyViewImage)
{
	if(PropertyViewImage==NULL)return false;
	if(m_pPropertyViewImage==NULL)m_pPropertyViewImage = PropertyViewImage;

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//�һ���Դ
	PropertyViewImage->pszExChagneArrowhead = TEXT("EXCHANGE_ARROWHEAD");

	//������Դ
	PropertyViewImage->uBtBugleExpression = DEF_IDB_BT_BUGLE_EXPESSION;
	PropertyViewImage->uBtBugleColorSet = DEF_IDB_BT_BUGLE_COLOR_SET;

	//��ť��Դ
	PropertyViewImage->uBtGift[0]=DEF_IDB_BT_GIFT1;
	PropertyViewImage->uBtGift[1]=DEF_IDB_BT_GIFT2;
	PropertyViewImage->uBtGift[2]=DEF_IDB_BT_GIFT3;
	PropertyViewImage->uBtGift[3]=DEF_IDB_BT_GIFT4;
	PropertyViewImage->uBtGift[4]=DEF_IDB_BT_GIFT5;
	PropertyViewImage->uBtGift[5]=DEF_IDB_BT_GIFT6;
	PropertyViewImage->uBtGift[6]=DEF_IDB_BT_GIFT7;
	PropertyViewImage->uBtGift[7]=DEF_IDB_BT_GIFT8;
	PropertyViewImage->uBtGift[8]=DEF_IDB_BT_GIFT9;
	PropertyViewImage->uBtGift[9]=DEF_IDB_BT_GIFT10;
	PropertyViewImage->uBtGift[10]=DEF_IDB_BT_GIFT11;
	PropertyViewImage->uBtProperty[0]= DEF_IDB_BT_PROPERTY1;
	PropertyViewImage->uBtProperty[1]= DEF_IDB_BT_PROPERTY2;
	PropertyViewImage->uBtProperty[2]= DEF_IDB_BT_PROPERTY3;
	PropertyViewImage->uBtProperty[3]= DEF_IDB_BT_PROPERTY4;
	PropertyViewImage->uBtProperty[4]= DEF_IDB_BT_PROPERTY5;
	PropertyViewImage->uBtProperty[5]= DEF_IDB_BT_PROPERTY6;
	PropertyViewImage->uBtProperty[6]= DEF_IDB_BT_PROPERTY7;
	PropertyViewImage->uBtProperty[7]= DEF_IDB_BT_PROPERTY8;
	PropertyViewImage->uBtProperty[8]= DEF_IDB_BT_PROPERTY9;
	PropertyViewImage->uBtProperty[9]= DEF_IDB_BT_PROPERTY10;
	PropertyViewImage->uBtProperty[10]=DEF_IDB_BT_PROPERTY11;
	PropertyViewImage->uBtProperty[11]=DEF_IDB_BT_PROPERTY12;
	PropertyViewImage->uBtProperty[12]=DEF_IDB_BT_PROPERTY13;
	PropertyViewImage->uBtProperty[13]=DEF_IDB_BT_PROPERTY14;
	PropertyViewImage->uBtProperty[14]=DEF_IDB_BT_PROPERTY15;

	//�����Դ
	PropertyViewImage->uBtPropertyToLeft=DEF_IDB_PROPERTY_FRAME_TO_L;
	PropertyViewImage->uBtPropertyToRight=DEF_IDB_PROPERTY_FRAME_TO_R;
	PropertyViewImage->uPropertyFrameL=DEF_IDB_PROPERTY_FRAME_AREA_L;
	PropertyViewImage->uPropertyFrameM=DEF_IDB_PROPERTY_FRAME_AREA_M;
	PropertyViewImage->uPropertyFrameR=DEF_IDB_PROPERTY_FRAME_AREA_R;

	//������Դ
	PropertyViewImage->uPropertyFormer[0]= DEF_IDB_PROPERTY_FORMER1;
	PropertyViewImage->uPropertyFormer[1]= DEF_IDB_PROPERTY_FORMER2;
	PropertyViewImage->uPropertyFormer[2]= DEF_IDB_PROPERTY_FORMER3;
	PropertyViewImage->uPropertyFormer[3]= DEF_IDB_PROPERTY_FORMER4;
	PropertyViewImage->uPropertyFormer[4]= DEF_IDB_PROPERTY_FORMER5;
	PropertyViewImage->uPropertyFormer[5]= DEF_IDB_PROPERTY_FORMER6;
	PropertyViewImage->uPropertyFormer[6]= DEF_IDB_PROPERTY_FORMER7;
	PropertyViewImage->uPropertyFormer[7]= DEF_IDB_PROPERTY_FORMER8;
	PropertyViewImage->uPropertyFormer[8]= DEF_IDB_PROPERTY_FORMER9;
	PropertyViewImage->uPropertyFormer[9]= DEF_IDB_PROPERTY_FORMER10;
	PropertyViewImage->uPropertyFormer[10]=DEF_IDB_PROPERTY_FORMER11;
	PropertyViewImage->uPropertyFormer[11]=DEF_IDB_PROPERTY_FORMER12;
	PropertyViewImage->uPropertyFormer[12]=DEF_IDB_PROPERTY_FORMER13;
	PropertyViewImage->uPropertyFormer[13]=DEF_IDB_PROPERTY_FORMER14;
	PropertyViewImage->uPropertyFormer[14]=DEF_IDB_PROPERTY_FORMER15;

	return true;
}

//������Դ
bool __cdecl CPlatformResource::GetChatExpViewImage(tagChatExpViewImage * ChatExpViewImage)
{	
	if(ChatExpViewImage==NULL)return false;
	if(m_pChatExpViewImage==NULL)m_pChatExpViewImage = ChatExpViewImage;

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//������Դ
	ChatExpViewImage->uChatBugle=DEF_IDB_CHAT_BUGLE;
	ChatExpViewImage->uChatGift[0]=DEF_IDB_CHAT_GIFT1;
	ChatExpViewImage->uChatGift[1]=DEF_IDB_CHAT_GIFT2;
	ChatExpViewImage->uChatGift[2]=DEF_IDB_CHAT_GIFT3;
	ChatExpViewImage->uChatGift[3]=DEF_IDB_CHAT_GIFT4;
	ChatExpViewImage->uChatGift[4]=DEF_IDB_CHAT_GIFT5;
	ChatExpViewImage->uChatGift[5]=DEF_IDB_CHAT_GIFT6;
	ChatExpViewImage->uChatGift[6]=DEF_IDB_CHAT_GIFT7;
	ChatExpViewImage->uChatGift[7]=DEF_IDB_CHAT_GIFT8;
	ChatExpViewImage->uChatGift[8]=DEF_IDB_CHAT_GIFT9;
	ChatExpViewImage->uChatGift[9]=DEF_IDB_CHAT_GIFT10;
	ChatExpViewImage->uChatGift[10]=DEF_IDB_CHAT_GIFT11;

	return true;
}

//��Ϸ���
bool __cdecl CPlatformResource::GetGameFrameImage(tagGameFrameImage * GameFrameImage)
{
	if(GameFrameImage==NULL)return false;
	if(m_pGameFrameImage==NULL)m_pGameFrameImage = GameFrameImage;

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//������Դ
	GameFrameImage->uChatML=DEF_IDB_GAME_CHAT_ML;
	GameFrameImage->uChatMR=DEF_IDB_GAME_CHAT_MR;
	GameFrameImage->uChatTL=DEF_IDB_GAME_CHAT_TL;
	GameFrameImage->uChatTM=DEF_IDB_GAME_CHAT_TM;
	GameFrameImage->uChatTR=DEF_IDB_GAME_CHAT_TR;
	GameFrameImage->uChatBL=DEF_IDB_GAME_CHAT_BL;
	GameFrameImage->uChatBM=DEF_IDB_GAME_CHAT_BM;
	GameFrameImage->uChatBR=DEF_IDB_GAME_CHAT_BR;

	//�б���Դ
	GameFrameImage->uUserListML=DEF_IDB_GAME_USER_LIST_ML;
	GameFrameImage->uUserListMR=DEF_IDB_GAME_USER_LIST_MR;
	GameFrameImage->uUserListTL=DEF_IDB_GAME_USER_LIST_TL;
	GameFrameImage->uUserListTM=DEF_IDB_GAME_USER_LIST_TM;
	GameFrameImage->uUserListTR=DEF_IDB_GAME_USER_LIST_TR;
	GameFrameImage->uUserListBL=DEF_IDB_GAME_USER_LIST_BL;
	GameFrameImage->uUserListBM=DEF_IDB_GAME_USER_LIST_BM;
	GameFrameImage->uUserListBR=DEF_IDB_GAME_USER_LIST_BR;

	//���찴ť
	GameFrameImage->uBtBugle=DEF_IDB_BT_GAME_BUGLE;
	GameFrameImage->uBtColorSet=DEF_IDB_BT_GAME_COLOR_SET;
	GameFrameImage->uBtExpression=DEF_IDB_BT_GAME_EXPESSION;
	GameFrameImage->uBtSendChat=DEF_IDB_BT_GAME_SEND_CHAT;
	GameFrameImage->uBtCleanScreen=DEF_IDB_BT_GAME_CLEAN_SCREEN;

	//�л���ť
	GameFrameImage->uBtShowUserList=DEF_IDB_GAME_SHOW_USER_LIST;
	GameFrameImage->uBtHideUserList=DEF_IDB_GAME_HIDE_USER_LIST;

	//���ð�ť
	GameFrameImage->uBtGameOption	=DEF_IDB_BT_GAME_OPTION	;
	GameFrameImage->uBtGameRule		=DEF_IDB_BT_GAME_RULE	;
	GameFrameImage->uBtGameLookOn	=DEF_IDB_BT_GAME_LOOKON	;
	GameFrameImage->uBtGameUnLookOn	=DEF_IDB_BT_GAME_UNLOOKON;
	GameFrameImage->uBtGameSound		=DEF_IDB_BT_GAME_SOUND	;
	GameFrameImage->uBtGameUnSound	=DEF_IDB_BT_GAME_UNSOUND;

	//�û���Ϣ
	GameFrameImage->uBtExchangeCharm = DEF_IDB_BT_GAME_EXCHANGE;
	GameFrameImage->uFemaleFace = DEF_IDB_GAME_FEMALE_FACE;
	GameFrameImage->uMaleFace = DEF_IDB_GAME_MALE_FACE;

	return true;
}

//��Ƶ��Դ
bool __cdecl CPlatformResource::GetVideoFrameImage(tagVideoFrameImage * VideoFrameImage)
{
	if(VideoFrameImage==NULL)return false;
	if(m_pVideoFrameImage==NULL)m_pVideoFrameImage = VideoFrameImage;

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//���ܰ�ť
	VideoFrameImage->uBtBigFlower  = DEF_IDB_BT_VIDEO_BIG_FLOWER  ; 
	VideoFrameImage->uBtBigMore    = DEF_IDB_BT_VIDEO_BIG_MORE    ;  
	VideoFrameImage->uBtBigNosee   = DEF_IDB_BT_VIDEO_BIG_NOSEE   ;  
	VideoFrameImage->uBtBigNotalk  = DEF_IDB_BT_VIDEO_BIG_NOTALK  ; 
	VideoFrameImage->uBtBigResore  = DEF_IDB_BT_VIDEO_BIG_RESORE  ; 
	VideoFrameImage->uBtBigSee     = DEF_IDB_BT_VIDEO_BIG_SEE     ;  
	VideoFrameImage->uBtBigTalk    = DEF_IDB_BT_VIDEO_BIG_TALK    ;  
	VideoFrameImage->uBtSmallFlower= DEF_IDB_BT_VIDEO_SMALL_FLOWER; 
	VideoFrameImage->uBtSmallMore  = DEF_IDB_BT_VIDEO_SMALL_MORE  ; 
	VideoFrameImage->uBtSmallNosee = DEF_IDB_BT_VIDEO_SMALL_NOSEE ; 
	VideoFrameImage->uBtSmallNotalk= DEF_IDB_BT_VIDEO_SMALL_NOTALK; 
	VideoFrameImage->uBtSmallResore= DEF_IDB_BT_VIDEO_SMALL_RESORE; 
	VideoFrameImage->uBtSmallSee   = DEF_IDB_BT_VIDEO_SMALL_SEE   ;  
	VideoFrameImage->uBtSmallTalk  = DEF_IDB_BT_VIDEO_SMALL_TALK  ; 

	//�����Դ
	VideoFrameImage->uBigFemale		=	DEF_IDB_VIDEO_LARGE_FEMALE	  ; 
	VideoFrameImage->uBigFrame		 =	DEF_IDB_VIDEO_LARGE_FRAME	  ; 
	VideoFrameImage->uBigMale		 =	DEF_IDB_VIDEO_LARGE_MALE	  ; 
	VideoFrameImage->uBigPhoto		=	DEF_IDB_VIDEO_LARGE_PHOTO	  ; 
	VideoFrameImage->uBigVolume		=	DEF_IDB_VIDEO_LARGE_VOLUME	  ; 
	VideoFrameImage->uBigVolumeEx	 =	DEF_IDB_VIDEO_LARGE_VOLUME_EX ;
	VideoFrameImage->uSmallFemale	 =	DEF_IDB_VIDEO_SMALL_FEMALE	  ; 
	VideoFrameImage->uSmallFrame		=	DEF_IDB_VIDEO_SMALL_FRAME	 ; 
	VideoFrameImage->uSmallMale		=	DEF_IDB_VIDEO_SMALL_MALE	  ; 
	VideoFrameImage->uSmallPhoto		=	DEF_IDB_VIDEO_SMALL_PHOTO	  ; 
	VideoFrameImage->uSmallVolume	=	DEF_IDB_VIDEO_SMALL_VOLUME	 ; 
	VideoFrameImage->uSmallVolumeEx =	DEF_IDB_VIDEO_SMALL_VOLUME_EX; 

	return true;
}

//���߲˵�
bool __cdecl CPlatformResource::GetPropertyMenuImage(tagPropertyMenuImage * PropertyMenuImage)
{
	if(PropertyMenuImage==NULL)return false;
	if(m_pPropertyMenuImage==NULL)m_pPropertyMenuImage = PropertyMenuImage;

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	PropertyMenuImage->uMenuGift[0]=  DEF_IDB_MENU_GIFT_1  ;
	PropertyMenuImage->uMenuGift[1]= DEF_IDB_MENU_GIFT_2  ;	
	PropertyMenuImage->uMenuGift[2]= DEF_IDB_MENU_GIFT_3  ;	
	PropertyMenuImage->uMenuGift[3]=  DEF_IDB_MENU_GIFT_4  ;
	PropertyMenuImage->uMenuGift[4]=  DEF_IDB_MENU_GIFT_5  ;
	PropertyMenuImage->uMenuGift[5]= DEF_IDB_MENU_GIFT_6  ;	
	PropertyMenuImage->uMenuGift[6]= DEF_IDB_MENU_GIFT_7  ;	
	PropertyMenuImage->uMenuGift[7]=  DEF_IDB_MENU_GIFT_8 ; 
	PropertyMenuImage->uMenuGift[8]=  DEF_IDB_MENU_GIFT_9  ;
	PropertyMenuImage->uMenuGift[9]= DEF_IDB_MENU_GIFT_10 ;	
	PropertyMenuImage->uMenuGift[10]=DEF_IDB_MENU_GIFT_11; 	

	return true;
}

//������Դ
bool __cdecl CPlatformResource::GetGiftFlashImage(tagGiftFlashImage * GiftFlashImage)
{
	if(GiftFlashImage==NULL)return false;
	if(m_pGiftFlashImage==NULL)m_pGiftFlashImage = GiftFlashImage;

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//�ƶ���Դ
	GiftFlashImage->pszGiftMove[0]= TEXT("MOVE_GIFT1");
	GiftFlashImage->pszGiftMove[1]= TEXT("MOVE_GIFT2");
	GiftFlashImage->pszGiftMove[2]= TEXT("MOVE_GIFT3");
	GiftFlashImage->pszGiftMove[3]= TEXT("MOVE_GIFT4");
	GiftFlashImage->pszGiftMove[4]= TEXT("MOVE_GIFT5");
	GiftFlashImage->pszGiftMove[5]= TEXT("MOVE_GIFT6");
	GiftFlashImage->pszGiftMove[6]= TEXT("MOVE_GIFT7");
	GiftFlashImage->pszGiftMove[7]= TEXT("MOVE_GIFT8");
	GiftFlashImage->pszGiftMove[8]= TEXT("MOVE_GIFT9");
	GiftFlashImage->pszGiftMove[9]= TEXT("MOVE_GIFT10");
	GiftFlashImage->pszGiftMove[10]=TEXT("MOVE_GIFT11");

	return true;
}

//////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(PlatformResource);

//////////////////////////////////////////////////////////////////////////
