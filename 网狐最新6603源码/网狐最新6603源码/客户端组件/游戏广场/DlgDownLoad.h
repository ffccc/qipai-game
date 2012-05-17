#ifndef DLG_DOWN_LOAD_HEAD_FILE
#define DLG_DOWN_LOAD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//下载状态
#define DOWN_LOAD_IDLE					0								//空闲状态
#define DOWN_LOAD_LINK					1								//连接状态
#define DOWN_LOAD_READ					2								//读取状态
#define DOWN_LOAD_SETUP					3								//安装状态
#define DOWN_LOAD_ERROR					4								//错误状态
#define DOWN_LOAD_FINISH				5								//安装完成

//消息定义
#define WM_ITEM_STATUS_UPDATE			(WM_USER+100)					//状态更新

//////////////////////////////////////////////////////////////////////////////////

//下载回调
class CDownLoadSink : public CWnd, public IDownLoadSink
{
	//友员定义
	friend class CDlgDownLoad;

	//状态变量
protected:
	BYTE							m_cbDownLoadStatus;					//下载状态

	//下载速度
protected:
	DWORD							m_dwDownSpeed;						//下载速度
	DWORD							m_dwLastCalcSize;					//上次大小
	DWORD							m_dwLastCalcTime;					//上次时间

	//文件信息
protected:
	DWORD							m_dwDownLoadSize;					//下载大小
	DWORD							m_dwTotalFileSize;					//文件大小

	//任务属性
protected:
	WORD							m_wKindID;							//游戏标识
	WORD							m_wServerID;						//房间标识
	TCHAR							m_szClientName[LEN_KIND];			//任务名字

	//内核信息
protected:
	CFile							m_DownFile;							//下载文件
	CDownLoad						m_DownLoad;							//下载组件
	CDlgDownLoad *					m_pDlgDownLoad;						//下载窗口
	PROCESS_INFORMATION				m_ProcessInfoMation;				//进程信息

	//函数定义
public:
	//构造函数
	CDownLoadSink();
	//析构函数
	virtual ~CDownLoadSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//功能函数
public:
	//关闭下载
	bool CloseDownLoad();
	//执行下载
	bool PerformDownLoad(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

	//状态接口
public:
	//下载异常
	virtual bool OnDownLoadError(enDownLoadError DownLoadError);
	//下载状态
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus);

	//序列接口
public:
	//下载数据
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize);
	//下载信息
	virtual bool OnDataInformation(DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation);

	//内部函数
private:
	//执行安装
	bool PerformInstall();
	//创建文件
	bool CreateDownFile(LPCTSTR pszLocation);

	//消息函数
protected:
	//时间消息
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//下载控件
class CDlgDownLoad : public CSkinDialog
{
	//状态变量
protected:
	bool							m_bCreate;							//创建标志
	WORD							m_wCurrentPage;						//当前页面
	WORD							m_wViewPageCount;					//页面数目
	WORD							m_wViewLineCount;					//页面行数

	//子项状态
protected:
	WORD							m_wFocusItem;						//焦点子项
	WORD							m_wHoverItem;						//盘旋子项

	//闪烁变量
protected:
	WORD							m_wFlashItem;						//闪烁子项
	WORD							m_wFlashCount;						//闪烁次数

	//控件变量
protected:
	CSkinButton						m_btLast;							//转上一页
	CSkinButton						m_btNext;							//转下一页
	CSkinButton						m_btCancelOne;						//取消更新
	CSkinButton						m_btCancelAll;						//全新取消
	CSkinButton						m_btRetryAgain;						//再次重试

	//资源变量
protected:
	CSize							m_SizeItemImage;					//子项大小
	CSize							m_SizeScaleImage;					//进度大小

	//数据变量
protected:
	CWHArray<CDownLoadSink *>		m_DownLoadSinkArray;				//下载数组

	//函数定义
public:
	//构造函数
	CDlgDownLoad();
	//析构函数
	virtual ~CDlgDownLoad();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//取消消息
	virtual VOID OnCancel();

	//重载函数
protected:
	//绘画消息
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//功能函数
public:
	//下载游戏
	bool DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

	//辅助函数
private:
	//调整控件
	VOID RectifyControl();
	//闪烁子项
	VOID FlashDownLoadItem(WORD wFlashItem);

	//辅助函数
private:
	//转换子项
	WORD SwitchItemIndex(CPoint MousePoint);

	//状态函数
private:
	//取消判断
	bool VerdictCancelOne(WORD wFocuxIndex);
	//重试判断
	bool VerdictRetryAgain(WORD wFocuxIndex);
	//获取状态
	LPCTSTR GetStatusString(BYTE cbDownLoadStatus);

	//按钮消息
protected:
	//转上一页
	VOID OnBnClickedLast();
	//转下一页
	VOID OnBnClickedNext();
	//取消更新
	VOID OnBnClickedCancelOne();
	//再次重试
	VOID OnBnClickedRetryAgain();

	//消息函数
protected:
	//销毁消息
	VOID OnDestroy();
	//时间消息
	VOID OnTimer(UINT nIDEvent);
	//位置改变
	VOID OnWindowPosChanged(WINDOWPOS * lpwndpos);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	//自定消息
protected:
	//状态消息
	LRESULT OnItemStatusUpdateMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif