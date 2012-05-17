#ifndef PROPERTY_HEAD_FILE
#define PROPERTY_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////

//���з�Χ
#define PT_ISSUE_AREA_WEB			0x0001								//�̳ǵ���
#define PT_ISSUE_AREA_GAME			0x0002								//��Ϸ����
#define PT_ISSUE_AREA_SERVER		0x0004								//�������

//ʹ�÷�Χ
#define PT_SERVICE_AREA_MESELF		0x0001								//�Լ���Χ
#define PT_SERVICE_AREA_PLAYER		0x0002								//��ҷ�Χ
#define PT_SERVICE_AREA_LOOKON		0x0004								//�Թ۷�Χ

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagPropertyInfo
{
	//������Ϣ
	WORD							wIndex;								//���߱�ʶ
	WORD							wDiscount;							//��Ա�ۿ�
	WORD							wIssueArea;							//������Χ

	//���ۼ۸�
	SCORE							lPropertyGold;						//���߽��
	DOUBLE							dPropertyCash;						//���߼۸�

	//��������
	SCORE							lSendLoveLiness;					//��������
	SCORE							lRecvLoveLiness;					//��������
};

//��������
struct tagPropertyAttrib
{
	WORD							wIndex;								//���߱�ʶ
	WORD							wServiceArea;						//ʹ�÷�Χ
	TCHAR							szPropertyName[32];					//��������
	TCHAR							szRegulationsInfo[256];				//ʹ����Ϣ
};

//��������
struct tagPropertyItem
{
	tagPropertyInfo					PropertyInfo;						//������Ϣ
	tagPropertyAttrib				PropertyAttrib;						//��������
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif