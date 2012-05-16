#pragma once

#ifndef SKIN_EDIT_HEAD_FILE
#define SKIN_EDIT_HEAD_FILE

#include "SkinControls.h"
#include "SkinResourceManager.h"

//��������
//#define SKIN_EDIT_CRFOCUSTX			TEXT("�༭�򽹵���ɫ")				// COLORREF ���� ����д��
//#define SKIN_EDIT_CRFOCUSBK			TEXT("�༭�򽹵㱳��ɫ")			// COLORREF ���� ����д��
//#define SKIN_EDIT_CRNOFOCUSTX		TEXT("�༭��������ɫ")				// COLORREF ���� ����д��
//#define SKIN_EDIT_CRNOFOCUSBK		TEXT("�༭����������ɫ")			// COLORREF ���� ����д��
//#define SKIN_EDIT_CRDISFOCUSTX		TEXT("�༭���ֹ��ɫ")				// COLORREF ���� ����д��
//#define SKIN_EDIT_CRDISFOCUSBK		TEXT("�༭���ֹ����ɫ")			// COLORREF ���� ����д��

//////////////////////////////////////////////////////////////////////////

//�༭������
class CSkinEditAttribute : public CSkinResourceManager
{
	friend class CSkinEdit;

	//���ò���
public:
	COLORREF							m_crFocusTX;					//������ɫ
	COLORREF							m_crFocusBK;					//������ɫ
	COLORREF							m_crNoFocusTX;					//������ɫ
	COLORREF							m_crNoFocusBK;					//������ɫ
	COLORREF							m_crDisFocusTX;					//������ɫ
	COLORREF							m_crDisFocusBK;					//������ɫ

	//GDI ����
public:
	CBrush								m_brFocus;						//����ˢ
	CBrush								m_brNoFocus;					//����ˢ
	CBrush								m_brDisbale;					//����ˢ

	//��������
protected:
	//���캯��
	CSkinEditAttribute();
	//��������
	virtual ~CSkinEditAttribute();

	//�ӿں���
public:
	//��������
	bool UpdateSkinResource(ISkinResource * pISkinResource);
};

//////////////////////////////////////////////////////////////////////////

//�༭��
class SKIN_CONTROL_CLASS CSkinEdit : public CEdit
{
	//��������
protected:
	bool							m_bFocus;						//�����־

	//��̬����
public:
	static CSkinEditAttribute		m_SkinAttribute;				//��������

	//��������
public:
	//���캯��
	CSkinEdit();
	//��������
	virtual ~CSkinEdit();

	//��ѯ����
public:
	//��ȡ���Խӿ�
	static CSkinEditAttribute * GetSkinAttribute() { return &m_SkinAttribute; }

	//��Ϣ���� 
protected:
	//�滭�ؼ�
	afx_msg HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);
	//�õ�����
	afx_msg void OnSetFocus(CWnd * pOldWnd);
	//ʧȥ����
	afx_msg void OnKillFocus(CWnd * pNewWnd);

	//��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinEdit)
};

//////////////////////////////////////////////////////////////////////////

#endif