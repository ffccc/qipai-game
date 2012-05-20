#pragma once

#define SICE_FLAME_COUNT				9

class CSiceControl
{
public:
	CSiceControl(void);
	~CSiceControl(void);

	//资源
public:
	static CPngImage				m_ImageSice01;				//图片资源
	static CPngImage				m_ImageSice02;				//图片资源
	static CPngImage				m_ImageSice03;				//图片资源
	static CPngImage				m_ImageSice04;				//图片资源
	static CPngImage				m_ImageSice05;				//图片资源
	static CPngImage				m_ImageSice06;				//图片资源

	//数据
private:
	BYTE							m_SiceValue;				//点数
	int								m_CurrentIndex;				//当前帧
	CPoint							m_pointBase;				//基准位置
	CPoint							m_pointOrigin;				//画原点
	bool							m_bVisbale;					//是否可见
	CSize							m_sizeRECT;					//显示范围
	CSize							m_FrameSize;				//帧大小

	//函数
public:
	//设置点数
	void SetSiceValue(BYTE nValue);

	//设置帧索引
	void SetFrameIndex(int nIndex);
	//获取帧索引
	int GetFrameIndex();

	//设置基准位置
	void SetPointBase(int x, int y);
	//设置基准位置
	void SetPointBase(CPoint pos);

	//显示完成
	bool IsFinish();

	//设置显示范围
	void SetSize(int w, int h);
	//设置显示范围
	void SetSize(CSize size);

	//是否显示
	bool IsVisable()
	{
		return m_bVisbale;
	}
	//是否显示
	void SetVisable(bool bVisbale)
	{
		m_bVisbale = bVisbale;
	}

	//绘画扑克
	VOID DrawCardControl(CDC * pDC);
};
