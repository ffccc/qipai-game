#ifndef CMD_VIDEO_HEAD_FILE
#define CMD_VIDEO_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

#define MDM_GF_VIDEO				200									//��Ƶͨ��

#define SUB_GF_C_VIDEO_CONFIG		1									//��Ƶ����
#define SUB_GF_C_VIDEO_CLOSE		2									//�ر���Ƶ
#define SUB_GF_C_VIDEO_OPEN			3									//����Ƶ

#define SUB_GF_S_VIDEO_CONFIG		100									//��Ƶ����
#define SUB_GF_S_VIDEO_CLOSE		101									//�ر���Ƶ
#define SUB_GF_S_VIDEO_OPEN			102									//����Ƶ

//////////////////////////////////////////////////////////////////////////

//��Ƶ����
struct CMD_C_VideoConfig
{
	//������Ϣ
	WORD							wNatPort;							//����˿�
	WORD							wLocalPort;							//����˿�
	DWORD							dwNatAddr;							//�����ַ
	DWORD							dwLocalAddr;						//���ص�ַ

	//�û���ʶ
	DWORD							dwRemoteUserID;						//Զ���û�
};

//��Ƶ����
struct CMD_S_VideoConfig
{
	//������Ϣ
	WORD							wNatPort;							//����˿�
	WORD							wLocalPort;							//����˿�
	DWORD							dwNatAddr;							//�����ַ
	DWORD							dwLocalAddr;						//���ص�ַ

	//�û���ʶ
	DWORD							dwLocalUserID;						//�����û�
};

//�ر���Ƶ
struct CMD_S_VideoClose
{
	WORD							wCloseUser;							//�ر����
};

//����Ƶ
struct CMD_S_VideoOpen
{
	WORD							wOpenUser;							//�����
};

////////////////////////////////////////////////////////////////////////////////////////////////

#endif