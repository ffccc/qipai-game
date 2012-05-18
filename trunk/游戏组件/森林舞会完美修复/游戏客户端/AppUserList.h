#pragma once


struct tagApplyUser
{
	//�����Ϣ
	CString							strUserName;					//����ʺ�
	CString							stAname;					//����ʺ�
	DWORD							dwUserID;						//����ʺ�
	INT64							lUserScore;						//��ҽ��
};

class CAppUserList
{
public:
	vector<tagApplyUser>			m_AppList;
public:
	CAppUserList(void);
	~CAppUserList(void);
private:
	int								m_index;//�±�
public:
	// ������ׯ����ׯ�û�
	void AddOrDeletUser(tagApplyUser ApplyUser,bool bo=true);
	// ����������ׯ�û�����
	void UpdateAppList(tagApplyUser ApplyUser);
	// ����Ƿ��������û�
	bool FindUser(DWORD UserID);
	//���밴ť����¼�
	void DrawScore(SURFACE* psurfTarget,int nX,int nY);
	// �������
	void clearVector(void);
	TCHAR* inst(TCHAR* szBuffer);
	//�б�ƫ��
	void listPY(bool bo);
};
