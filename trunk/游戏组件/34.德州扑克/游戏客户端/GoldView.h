#ifndef GOLD_VIEW_HEAD_FILE
#define GOLD_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
//�궨��
#define GOLD_IMAGE_WIDTH		20										//�����
#define GOLD_IMAGE_HEIGHT		19										//�����

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CGoldView
{
	//��������
public:
	LONG								m_lGold;						//������Ŀ
	LONG								m_lMaxLayer;					//������
	LONG								m_lGoldCount[7];	 			//������Ŀ

	//��������
protected:
	static bool							m_bInit;						//��ʼ��־
	static CSkinImage					m_ImageGold;					//����ͼƬ
	CSkinImage							m_ImageNumber;					//����ͼƬ

	//��������
public:
	//���캯��
	CGoldView();
	//��������
	virtual ~CGoldView();

	//���ܺ���
public:
	//���ó���
	void SetGold(LONG lGold);
	//��ȡ����
	LONG GetGold() { return m_lGold; };
	//���ò���
	void SetMaxGoldLayer(LONG lMaxLayer);
	//��ȡ����
	LONG GetMaxGoldLayer(){return m_lMaxLayer;}
	//�滭����
	void DrawGoldView(CDC * pDC, int nXPos, int nYPos, bool bCount,bool bCenter=false,BYTE bDUser=0);
	//���Ա��ַ�
	LPCTSTR GetGlodString(LONG lGold, TCHAR szString[]);

	//�ڲ�����
private:
	//���������
	void RectifyGoldLayer();
};

//////////////////////////////////////////////////////////////////////////

#endif