#ifndef USER_ITEM_ELEMENT_HEAD_FILE
#define USER_ITEM_ELEMENT_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�ӿ�˵��
interface IUserOrderParser;
interface IFaceItemControl;

//////////////////////////////////////////////////////////////////////////////////

//����ȼ�
#define PASSWORD_LEVEL_0			0									//û�еȼ�
#define PASSWORD_LEVEL_1			1									//û�еȼ�
#define PASSWORD_LEVEL_2			2									//û�еȼ�
#define PASSWORD_LEVEL_3			3									//û�еȼ�

//////////////////////////////////////////////////////////////////////////////////

//�û�Ԫ��
class SHARE_CONTROL_CLASS CUserItemElement
{
	//���ñ���
protected:
	IUserOrderParser *				m_pIUserOrderParser;				//�ȼ����
	IFaceItemControl *				m_pIFaceItemControl;				//ͷ�����

	//��̬����
protected:
	static CUserItemElement *		m_pUserItemElement;					//����ָ��

	//��������
public:
	//���캯��
	CUserItemElement();
	//��������
	virtual ~CUserItemElement();

	//���ú���
public:
	//���ýӿ�
	VOID SetUserOrderParser(IUserOrderParser * pIUserOrderParser);
	//���ýӿ�
	VOID SetFaceItemControl(IFaceItemControl * pIFaceItemControl);

	//���ܺ���
public:
	//�滭����
	VOID DrawExperience(CDC * pDC, INT nXPos, INT nYPos, DWORD dwExperience);
	//������Ƭ
	VOID ConstructNameCard(IClientUserItem * pIClientUserItem, IGameLevelParser * pIGameLevelParser, CImage&ImageNameCard);

	//�ȼ�����
public:
	//����ȼ�
	BYTE GetPasswordLevel(LPCTSTR pszPassword);
	//��ȡ�ȼ�
	WORD GetExperienceLevel(DWORD dwExperience);
	//�ȼ���Ϣ
	WORD GetExperienceIndex(WORD wUserLevel, BYTE cbOrderIndex[], WORD wMaxCount);

	//�Ϸ�Ч��
public:
	//Ч���ʺ�
	bool EfficacyAccounts(LPCTSTR pszAccounts, TCHAR szDescribe[], WORD wMaxCount);
	//Ч���ǳ�
	bool EfficacyNickName(LPCTSTR pszNickName, TCHAR szDescribe[], WORD wMaxCount);
	//Ч������
	bool EfficacyPassword(LPCTSTR pszPassword, TCHAR szDescribe[], WORD wMaxCount);
	//Ч��֤��
	bool EfficacyPassPortID(LPCTSTR pszPassPortID, TCHAR szDescribe[], WORD wMaxCount);

	//��̬����
public:
	//��ȡ����
	static CUserItemElement * GetInstance() { return m_pUserItemElement; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif