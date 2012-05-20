#pragma once

#define SICE_FLAME_COUNT				9

class CSiceControl
{
public:
	CSiceControl(void);
	~CSiceControl(void);

	//��Դ
public:
	static CPngImage				m_ImageSice01;				//ͼƬ��Դ
	static CPngImage				m_ImageSice02;				//ͼƬ��Դ
	static CPngImage				m_ImageSice03;				//ͼƬ��Դ
	static CPngImage				m_ImageSice04;				//ͼƬ��Դ
	static CPngImage				m_ImageSice05;				//ͼƬ��Դ
	static CPngImage				m_ImageSice06;				//ͼƬ��Դ

	//����
private:
	BYTE							m_SiceValue;				//����
	int								m_CurrentIndex;				//��ǰ֡
	CPoint							m_pointBase;				//��׼λ��
	CPoint							m_pointOrigin;				//��ԭ��
	bool							m_bVisbale;					//�Ƿ�ɼ�
	CSize							m_sizeRECT;					//��ʾ��Χ
	CSize							m_FrameSize;				//֡��С

	//����
public:
	//���õ���
	void SetSiceValue(BYTE nValue);

	//����֡����
	void SetFrameIndex(int nIndex);
	//��ȡ֡����
	int GetFrameIndex();

	//���û�׼λ��
	void SetPointBase(int x, int y);
	//���û�׼λ��
	void SetPointBase(CPoint pos);

	//��ʾ���
	bool IsFinish();

	//������ʾ��Χ
	void SetSize(int w, int h);
	//������ʾ��Χ
	void SetSize(CSize size);

	//�Ƿ���ʾ
	bool IsVisable()
	{
		return m_bVisbale;
	}
	//�Ƿ���ʾ
	void SetVisable(bool bVisbale)
	{
		m_bVisbale = bVisbale;
	}

	//�滭�˿�
	VOID DrawCardControl(CDC * pDC);
};
