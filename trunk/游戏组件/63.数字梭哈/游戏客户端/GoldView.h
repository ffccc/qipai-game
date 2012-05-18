#ifndef GOLD_VIEW_HEAD_FILE
#define GOLD_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CGoldView
{
	//��������
public:
	LONG								m_lGold;						//������Ŀ
	LONG								m_lMaxLayer;					//������
	LONG								m_lGoldCount[6];	 			//������Ŀ

	//��������
protected:
	static bool							m_bInit;						//��ʼ��־
	static CSkinImage					m_ImageGold;					//����ͼƬ

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
	//�滭����
	void DrawGoldView(CDC * pDC, int nXPos, int nYPos);

	//�ڲ�����
private:
	//���������
	void RectifyGoldLayer();
	//���Ա��ַ�
	LPCTSTR GetGlodString(LONG lGold, TCHAR szString[]);
};

//////////////////////////////////////////////////////////////////////////

#endif
