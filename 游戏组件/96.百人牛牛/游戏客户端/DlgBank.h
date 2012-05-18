#pragma once

void MakeString(CString &strNum,LONGLONG lNumber);
void MakeStringToNum(CString str,LONGLONG &Num);
// CDlgBank dialog
#define  WM_CREADED						WM_USER+1
class CDlgBank : public CSkinDialogEx
{
	//变量定义
public:
	bool							m_bBankStorage;					//存储标识
	tagUserData						*m_pMeUserData;					//用户信息
	LONGLONG						m_lGameGold;					//银行数目
	LONGLONG						m_lStorageGold;					//存储数目
	LONGLONG						m_lInCount;						//输入游戏币
	LONGLONG						m_OrInCount;
	bool							m_bCanDraw;
	bool							m_blCanStore;
	//bool							m_bStorageAllGold;				//存入所有金币
	//bool							m_bDrawAllGold;					//取出所有金币
	//控件变量
protected:
	CSkinButton						m_btOK;							//确定按钮
	CSkinButton						m_btCancel;						//取消按钮
	CEdit							m_Edit;
	CStatic							m_Static;

	bool							m_blUsingPassWord;



	//接口变量
protected:	
	IClientKernel					* m_pIClientKernel;				//内核接口


public:
	CDlgBank(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBank();

	//辅助函数
public:
	//更新界面
	void UpdateView();

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange* pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定按钮
	afx_msg void OnBnClickedOk();
	//鼠标信息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//鼠标信息
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//命令信息
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//时间消息
	afx_msg void OnTimer(UINT nIDEvent);
	//输入信息
	afx_msg void OnEnChangeInCount();

	afx_msg void OnEnRadioStore();

	afx_msg void OnEnRadioGet();

    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//接口函数
public:
	//重置控件
	void ShowItem();
	//允许取款
	void AllowDraw(bool bAllow);
	//允许存款
	void AllowStorage(bool bAllow);
	//设置位置
	void SetPostPoint(CPoint Point);
	//设置类型
	void SetBankerActionType(bool bStorage);
	//功能函数
public:
	//是否选中
	bool IsButtonSelected(UINT uButtonID);
	//选中按钮
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	//设置组件
	void SetSendInfo(IClientKernel *pClientKernel,tagUserData const*pMeUserDatas);
	//发送信息
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDlgBank)
protected:
	virtual void OnCancel();
};
