#ifndef SKIN_RESOURCE_MANAGER_HEAD_FILE
#define SKIN_RESOURCE_MANAGER_HEAD_FILE

#pragma once

#include "SkinControls.h"

//////////////////////////////////////////////////////////////////////////

//��Դ����
class SKIN_CONTROL_CLASS CSkinResourceManager
{
	//��Դ����
protected:
	static CFont					m_DefaultFont;						//Ĭ������
	static ISkinResource *			m_pISkinResource;					//��Դ�ӿ�

	//��������
protected:
	//���캯��
	CSkinResourceManager();
	//��������
	virtual ~CSkinResourceManager();

	//��Դ����
public:
	//Ĭ������
	static HFONT GetDefaultFont() { return m_DefaultFont; }//?

	//������
public:
	//������Դ
	static bool LoadSkinResource(IUnknownEx * pIUnknownEx);
	//��ȡ��Դ
	static VOID * GetSkinResource(REFGUID Guid, DWORD dwQueryVer);
};

//////////////////////////////////////////////////////////////////////////

#endif
