#ifndef GLOBAL_UNITES_HEAD_FILE
#define GLOBAL_UNITES_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgDownLoad.h"

//////////////////////////////////////////////////////////////////////////////////

//ȫ�ֵ�Ԫ
class CGlobalUnits
{
	//�汾��Ϣ
protected:
	DWORD							m_dwPlazaVersion;					//�����汾
	DWORD							m_dwFrameVersion;					//��ܰ汾

	//���ָ��
protected:
	CDlgDownLoad *					m_pDlgDownLoad;						//�������

	//��̬����
protected:
	static CGlobalUnits *			m_pGlobalUnits;						//����ָ��

	//��������
public:
	//���캯��
	CGlobalUnits();
	//��������
	virtual ~CGlobalUnits();

	//�汾��Ϣ
public:
	//�����汾
	virtual DWORD GetPlazaVersion() { return m_dwPlazaVersion; }
	//��ܰ汾
	virtual DWORD GetFrameVersion() { return m_dwFrameVersion; }

	//�����Ϣ
public:
	//��¼���
	bool WriteUserCookie();
	//ɾ�����
	bool DeleteUserCookie();

	//��������
public:
	//��������
	bool PlayGameSound(LPCTSTR pszSoundName);
	//��������
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName);

	//���ܺ���
public:
	//������Ϸ
	bool DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

	//��̬����
public:
	//��ȡ����
	static CGlobalUnits * GetInstance() { return m_pGlobalUnits; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif