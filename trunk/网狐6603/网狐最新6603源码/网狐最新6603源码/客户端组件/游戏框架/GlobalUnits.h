#ifndef GLOBAL_UNITS_HEAD_FILE
#define GLOBAL_UNITS_HEAD_FILE

#pragma once

#include "AfxTempl.h"
#include "GameFrameHead.h"
#include "GameFrameEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//ȫ�����
#define MODULE_CLIENT_KERNEL		1									//��Ϸ�ں�
#define MODULE_STRING_MESSAGE		2									//
#define MODULE_FACE_ITEM_CONTROL	3									//ͷ�����
#define MODULE_GAME_LEVEL_PARSER	4									//�ȼ�����
#define MODULE_GAME_USER_MANAGER	5									//�û�����

//������
#define MODULE_GAME_FRAME_WND		10									//��Ϸ���
#define MODULE_GAME_FRAME_VIEW		11									//�����ͼ
#define MODULE_GAME_FRAME_ENGINE	12									//�������
#define MODULE_GAME_FRAME_SERVICE	13									//��ܷ���
#define MODULE_GAME_FRAME_CONTROL	14									//���ƿ��
#define MODULE_GAME_FRAME_TOOL_BAR	15									//���߿��

//ģ������
typedef CMap<WORD,WORD,IUnknownEx *,IUnknownEx * &> CGlobalModuleMap;

//////////////////////////////////////////////////////////////////////////////////

//ȫ�ֵ�Ԫ
class GAME_FRAME_CLASS CGlobalUnits
{
	//��������
public:
	bool							m_bAllowSound;						//��������
	bool							m_bFullGameView;					//ȫ��ģʽ
	bool							m_bNotifyUserInOut;					//������Ϣ

	//�Թ�����
public:
	bool							m_bAllowLookon;						//�����Թ�
	bool							m_bAllowFriendLookon;				//�����Թ�

	//ģʽ����
public:
	BYTE							m_cbMessageMode;					//��Ϣģʽ

	//��������
public:
	COLORREF						m_crChatTX;							//��������
	tagSkinRenderInfo				m_SkinRenderInfo;					//��Ⱦ��ɫ

	//�������
public:
	CClientKernelHelper				m_ClientKernelModule;				//��Ϸ�ں�
	CFaceItemControlHelper			m_FaceItemControlModule;			//ͷ�����
	CUserOrderParserHelper			m_UserOrderParserModule;			//�û��ȼ�
	CGameLevelParserHelper			m_GameLevelParserModule;			//��Ϸ�ȼ�

	//�������
public:
	CGlobalModuleMap				m_GlobalModuleMap;					//ģ������
	CUserItemElement				m_UserItemElement;					//�û�Ԫ��
	CExpressionManager				m_ExpressionManager;				//�������
	CGamePropertyManager			m_GamePropertyManager;				//���߹���

	//��Դ���
public:
	CSkinRenderManager				m_SkinRenderManager;				//��Ⱦ����
	CSkinResourceManager			m_SkinResourceManager;				//��Դ����

	//��̬����
protected:
	static CGlobalUnits *			m_pGlobalUnits;						//ȫ�ֵ�Ԫ

	//��������
public:
	//���캯��
	CGlobalUnits();
	//��������
	virtual ~CGlobalUnits();

	//�������
public:
	//�������
	bool InitGlobalUnits();
	//ע�����
	bool UnInitGlobalUnits();

	//���ú���
public:
	//���ز���
	VOID LoadParameter();
	//�������
	VOID SaveParameter();
	//Ĭ�ϲ���
	VOID DefaultParameter();

	//��������
public:
	//��������
	bool PlayGameSound(LPCTSTR pszSoundName);
	//��������
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName);

	//�������
public:
	//ע�����
	bool RegisterGlobalModule(WORD wModuleID, IUnknownEx * pIUnknownEx);
	//��ѯ���
	VOID * QueryGlobalModule(WORD wModuleID, REFGUID Guid, DWORD dwQueryVer);

	//��̬����
public:
	//��ȡʵ��
	static CGlobalUnits * GetInstance() { return m_pGlobalUnits; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif