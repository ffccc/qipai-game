#ifndef FLOWER_EFFECT_CONTROL_HEAD
#define FLOWER_EFFECT_CONTROL_HEAD

#pragma once

#include "PropertyModule.h"

///////////////////////////////////////////////////////////////////////////////////

//Gif�ṹ
typedef struct tagGifImage{
	WORD lWidth;
	WORD lHeight;
	WORD width;
	WORD height;
	struct flag{
		BYTE d:3;
		BYTE c:1;
		BYTE b:3;
        BYTE a:1;
	}Flag;
}GifImage,*PGifImage;

///////////////////////////////////////////////////////////////////////////////////

//�ʻ��߳�
class PROPERTY_MODULE_CLASS CFlowerEffectThread : public CServiceThread
{
	//��������
protected:
	HWND							m_hFlowerEffectControl;					//Ч������
	WORD							m_wFlowerID;							//�ʻ�����
	CSkinImage						m_ImageBuffer;							//����λͼ
	CSkinImage						m_ImageBufferScreen;					//��Ļ����

	//��������
public:
	//���캯��
	CFlowerEffectThread(void);
	//��������
	~CFlowerEffectThread(void);

	//���ú���
public:
	//��ʼ����
	void InitThread(HWND hWnd);
	//��������
	bool StartFlowerEffect(WORD wFlowerID);

	//�¼�����
private:
	//���к���
	virtual bool OnEventThreadRun();
	//��ʼ�¼�   
	virtual bool OnEventThreadStrat();
	//ֹͣ�¼�
	virtual bool OnEventThreadConclude();

	//�ڲ�����
private:
	//���к���
	bool RepetitionRunBMP();
	//���к���
	bool RepetitionRunPNG();
};

///////////////////////////////////////////////////////////////////////////////////

//�ʻ��ؼ�
class PROPERTY_MODULE_CLASS CFlowerEffectControl : public CDialog
{
	//��������
public:
	CFlowerEffectThread				*m_pFlowerEffectThread;					//�����߳�

	//��������
public:
	//��������
	bool StartFlowerEffect(UINT uFlowerEffectID);
	//��������
	BOOL CreateControl(CWnd *pParent);

	//��������
public:
	//���캯��
	CFlowerEffectControl(); 
	//��������
	virtual ~CFlowerEffectControl();

protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);

	//�̳к���
public:
	//��ʼ����
	virtual BOOL OnInitDialog();

	//��Ϣ����
public:
	//�滭����
	afx_msg void OnPaint();
	
	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
	
};
///////////////////////////////////////////////////////////////////////////////////

#endif