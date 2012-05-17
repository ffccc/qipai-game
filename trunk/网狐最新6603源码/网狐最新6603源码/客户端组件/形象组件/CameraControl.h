#ifndef DLG_CAMERA_HEAD_FILE
#define DLG_CAMERA_HEAD_FILE

#pragma once

#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//����ͷ��
class AVATAR_CONTROL_CLASS CCameraControl : public CSkinDialog
{
	//��Ԫ����
	friend class CFaceItemCustomWnd;

	//��������
protected:
	bool							m_bInit;							//���ñ�ʶ
	HWND							m_hWndCamera;						//��ʾ����
	CImage							m_ImageResult;						//���ͼ��
	
	//�豸��Ϣ
protected:
	CAPSTATUS						m_CapStatus;						//��ǰ״̬
	CAPTUREPARMS					m_CapTureparms;						//��׽����
	CAPDRIVERCAPS					m_CapDrivercaps;					//��������

	//�ؼ�����
protected:
	CSkinButton						m_btOk;								//����༭
	CSkinButton						m_btRepeat;							//��������
	CSkinButton						m_btCameraDict;						//����֡

	//�����ؼ�
protected:
	CStatic							m_StringControl;					//��ʾ�ؼ�
	CStatic							m_CameraControl;					//��Ƶ����

	//��������
public:
	//���캯��
	CCameraControl(CWnd * pParent);
	//��������
	virtual ~CCameraControl();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//���ú���
	virtual BOOL OnInitDialog();
	
	//��Ϣ����
protected:
	//��������
	VOID OnBnClickedRepeat();
	//����ͼƬ
	VOID OnBnClickedCameraDict();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif