#ifndef DLG_CUSTOM_FACE_HEAD_FILE
#define DLG_CUSTOM_FACE_HEAD_FILE

#pragma once

#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////

//ͷ��ѡ��
class CDlgCustomFace : public CSkinDialog, public CMissionItem, public IImageEditorEvent, public IFaceItemSelectEvent
{
	//״̬����
protected:
	BYTE							m_cbMode;							//�༭ģʽ
	WORD							m_wFaceID;							//ѡ��ͷ��
	tagCustomFaceInfo				m_CustomFaceInfo;					//�Զ�ͷ��

	//�ӿڱ���
protected:
	ICustomFaceEvent *				m_pICustomFaceEvent;				//ͷ���¼�

	//���ܰ�ť
protected:
	CSkinButton			 			m_btOk;								//�洢�˳�
	CSkinButton			 			m_btCancel;							//������ť

	//�ؼ�����
protected:
	CSkinTabCtrl		 			m_TabControl;						//ѡ������
	CMissionManager					m_MissionManager;					//�������
	CFaceItemCustomWnd				m_FaceItemCustomWnd;				//ͷ���Զ�
	CFaceItemSelectWnd	 			m_FaceItemSelectWnd;				//ͷ��ѡ��

	//��������
public:
	//���캯��
	CDlgCustomFace();
	//��������
	virtual ~CDlgCustomFace();

	//�༭�ӿ�
public:
	//ͼ���¼�
	virtual VOID OnEventFaceSelect(WORD wFaceID);
	//ͼ���¼�
	virtual VOID OnEventImageSelect(bool bLoadImage, bool bOperateing);

	//���ú���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//���ں���
protected:
	//�滭��Ϣ
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//���غ���
protected:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//���ܺ���
public:
	//���ýӿ�
	bool SetCustomFaceEvent(IUnknownEx * pIUnknownEx);

	//�ڲ�����
private:
	//�ؼ�����
	VOID EnableControl(bool bEnableControl);

	//��Ϣ��Ӧ
protected:
	//���͸ı�
	VOID OnTcnSelAlterFaceMode(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif