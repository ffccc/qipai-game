#ifndef SKIN_RESOURCE_MANAGER_HEAD_FILE
#define SKIN_RESOURCE_MANAGER_HEAD_FILE

#pragma once

#include "SkinControlHead.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////

//��Դ����
class SKIN_CONTROL_CLASS CSkinResourceManager
{
	//������Դ
protected:

	//��������
protected:
	CFont							m_DefaultFont;						//Ĭ������

	//��̬����
protected:
	static CSkinResourceManager *	m_pSkinResourceManager;				//����ָ��

	//��������
public:
	//���캯��
	CSkinResourceManager();
	//��������
	virtual ~CSkinResourceManager();

	//��Դ����
public:
	//Ĭ������
	CFont & GetDefaultFont() { return m_DefaultFont; }

	//������Դ
public:
	//������Դ
	VOID SetSkinResource(const tagSkinRenderInfo & SkinRenderInfo);

	//��̬����
public:
	//ö�ٺ���
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	//��ȡʵ��
	static CSkinResourceManager * GetInstance() { return m_pSkinResourceManager; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif
