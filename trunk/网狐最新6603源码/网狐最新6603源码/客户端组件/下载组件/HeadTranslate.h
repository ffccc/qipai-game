#ifndef HEAD_TRANSLATE_HEAD_FILE
#define HEAD_TRANSLATE_HEAD_FILE

#pragma once

#include "DownLoadHead.h"

//////////////////////////////////////////////////////////////////////////////////

//Э�鷭��
class DOWN_LOAD_CLASS CHeadTranslate
{
	//��������
protected:
	WORD							m_wDataLength;						//���ݳ���
	BYTE *							m_pcbHeadData;						//����ָ��

	//������Ϣ
protected:
	DWORD							m_dwTotalFileSize;					//�ļ���С
	TCHAR							m_szFileEntityTag[32];				//�ļ���ʶ

	//��������
public:
	//���캯��
	CHeadTranslate();
	//��������
	virtual ~CHeadTranslate();

	//������Ϣ
public:
	//�ļ���С
	DWORD GetTotalFileSize() { return m_dwTotalFileSize; }
	//�ļ���ʶ
	LPCTSTR GetFileEntityTag() { return m_szFileEntityTag; }
	
	//���ܺ���
public:
	//�������
	VOID CleanData();
	//���ò���
	VOID SetHeadData(BYTE * pcbHeadData, WORD wDataLength);
	//��ȡ����
	WORD GetLineString(WORD wLineIndex, TCHAR szDataBuffer[], WORD wCharCount);
};

//////////////////////////////////////////////////////////////////////////////////

#endif