#ifndef WND_VALIDATE_CODE_HEAD_FILE
#define WND_VALIDATE_CODE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

#define VALIDATE_COUNT				4									//��֤����

//////////////////////////////////////////////////////////////////////////////////

//��֤�ؼ�
class CWndValidateCode : public CStatic
{
	//��������
protected:
	bool							m_bValidateCode;					//��֤��־
	TCHAR							m_szValidateCode[VALIDATE_COUNT];	//��֤�ַ�

	//�ؼ�����
protected:
	CImage							m_ImageValidateCode;				//��֤ͼƬ

	//��������
public:
	//���캯��
	CWndValidateCode();
	//��������
	virtual ~CWndValidateCode();

	//���ܺ���
public:
	//������֤
	VOID RandValidateCode();
	//Ч����֤
	bool CheckValidateCode(LPCTSTR pszValidateCode);
	//����ͼƬ
	VOID CreateValidateCode(CImage & ImageValidateCode, INT nWidth, INT nHeight);

	//��Ϣ����
protected:
	//�ػ�����	
	VOID OnPaint();
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif