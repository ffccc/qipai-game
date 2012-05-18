#pragma once


struct tagApplyUser
{
	//玩家信息
	CString							strUserName;					//玩家帐号
	CString							stAname;					//玩家帐号
	DWORD							dwUserID;						//玩家帐号
	INT64							lUserScore;						//玩家金币
};

class CAppUserList
{
public:
	vector<tagApplyUser>			m_AppList;
public:
	CAppUserList(void);
	~CAppUserList(void);
private:
	int								m_index;//下标
public:
	// 申请上庄或下庄用户
	void AddOrDeletUser(tagApplyUser ApplyUser,bool bo=true);
	// 更新申请上庄用户分数
	void UpdateAppList(tagApplyUser ApplyUser);
	// 检查是否存在这个用户
	bool FindUser(DWORD UserID);
	//筹码按钮点击事件
	void DrawScore(SURFACE* psurfTarget,int nX,int nY);
	// 清空容器
	void clearVector(void);
	TCHAR* inst(TCHAR* szBuffer);
	//列表偏移
	void listPY(bool bo);
};
