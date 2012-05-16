#ifndef GLOBAL_PROPERTY_HEAD_FILE
#define GLOBAL_PROPERTY_HEAD_FILE

////////////////////////////////////////////////////////////////////////////////////////////////

//�ʻ�����
#define FLOWER_1					0
#define FLOWER_2					1
#define FLOWER_3					2
#define FLOWER_4					3
#define FLOWER_5					4
#define FLOWER_6					5
#define FLOWER_7					6
#define FLOWER_8					7
#define FLOWER_9					8
#define FLOWER_10					9
#define FLOWER_11					10

BYTE const g_FlowerTypeList[]=
{
	FLOWER_1,
	FLOWER_2,
	FLOWER_3,
	FLOWER_4,
	FLOWER_5,
	FLOWER_6,
	FLOWER_7,
	FLOWER_8,
	FLOWER_9,
	FLOWER_10,
	FLOWER_11,
};

//�ʻ���Ŀ
#define FLOWER_COUNT		(sizeof(g_FlowerTypeList)/sizeof(g_FlowerTypeList[0]))//������Ŀ

//////////////////////////////////////////////////////////////////////////
//�궨��
#define CHARM_EXCHANGE_RATE			50									//�һ�����
#define MAX_FLOWER_COUNT			100									//�����Ŀ

//�ʻ��ṹ
struct tagGiftInfo
{
	TCHAR							szName[64];							//�ʻ�����
	TCHAR							szAction[128];						//��������
	TCHAR							szQuantifier1[20];					//������
	TCHAR							szResult[128];						//�������
	TCHAR							szQuantifier2[6];					//������
	TCHAR							szCharmResult[128];					//�������
};

//�ʻ�����
tagGiftInfo const g_FlowerDescribe[FLOWER_COUNT]=
{
	{TEXT("����"),TEXT(" ��������� "),TEXT(" ����1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("����"),TEXT(" ��������� "),TEXT(" ����1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("�ʻ�"),TEXT(" ��������� "),TEXT(" �ʻ�1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("ơ��"),TEXT(" ��������� "),TEXT(" ơ��1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("����"),TEXT(" ��������� "),TEXT(" ����1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("���"),TEXT(" ��������� "),TEXT(" ���1ö"),TEXT("����ϲ "),TEXT("ö "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("�γ�"),TEXT(" ��������� "),TEXT(" �γ�1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("����"),TEXT(" ��������� "),TEXT(" ����1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("����"),TEXT(" �ݺݵ����� "),TEXT(" ����1��"),TEXT("����Ǹ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("שͷ"),TEXT(" �ݺݵ����� "),TEXT(" שͷ1��"),TEXT("����Ǹ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("ը��"),TEXT(" �ݺݵ����� "),TEXT(" ը��1ö"),TEXT("����Ǹ "),TEXT("ö "),TEXT(" ����ֵ���� %ld �㣡")}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//��������
#define PROP_DOUBLE						0									//˫�����ֿ�
#define PROP_FOURDOLD					1									//�ı����ֿ�
#define PROP_NEGAGIVE					2									//��������
#define PROP_FLEE						3									//��������
#define PROP_BUGLE						4									//С����
#define PROP_KICK						5									//���߿�
#define PROP_SHIELD						6									//�����
#define PROP_MEMBER_1					7									//��Ա����
#define PROP_MEMBER_2					8									//��Ա����
#define PROP_MEMBER_3					9									//��Ա����
#define PROP_MEMBER_4					10									//��Ա����
#define PROP_MEMBER_5					11									//��Ա����
#define PROP_MEMBER_6					12									//��Ա����
#define PROP_MEMBER_7					13									//��Ա����
#define PROP_MEMBER_8					14									//��Ա����
BYTE const g_PropTypeList[]=
{
	PROP_DOUBLE	,
	PROP_FOURDOLD,
	PROP_NEGAGIVE,
	PROP_FLEE	,
	PROP_BUGLE	,
	PROP_KICK	,
	PROP_SHIELD	,
	PROP_MEMBER_1,
	PROP_MEMBER_2,
	PROP_MEMBER_3,
	PROP_MEMBER_4,
	PROP_MEMBER_5,
	PROP_MEMBER_6,
	PROP_MEMBER_7,
	PROP_MEMBER_8,
};
#define PROPERTY_COUNT			(sizeof(g_PropTypeList)/sizeof(g_PropTypeList[0]))//������Ŀ

//���߶���
#define MAX_PROPERTY_COUNT				100									//�����Ŀ
#define BUGLE_MAX_CHAR					120									//���ȸ���
#define BULESSING_MAX_CHAR				100									//ף������

//��������
struct tagPropertyDescribe
{
	TCHAR							szName[32];							//��������
	TCHAR							szDescribe[255];					//���߼�ֵ
};

tagPropertyDescribe const g_PropertyDescribe[PROPERTY_COUNT] =
{
	{TEXT("˫�����ֿ�"),TEXT("ӵ����˫�����ֿ�������ϷӮ�ַ��������˲���ۣ�������Ч����ι���ʱ���ۼӡ���ע��ֻ���ڹ������Ϸ����ʹ�á���")},
	{TEXT("�ı����ֿ�"),TEXT("ӵ�����ı����ֿ�������ϷӮ�ַ��ı������˲���ۣ�������Ч����ι���ʱ���ۼӡ���ע��ֻ���ڹ������Ϸ����ʹ�á���")},
	{TEXT("��������"),TEXT("ʹ�øõ��ߺ�������Ϸ���ֽ��ָ���ʼ״̬������Ϊ���ֶ����գ�")},
	{TEXT("��������"),TEXT("ʹ�øõ��ߺ����������ʽ��ָ���ʼ״̬������Ϊ���ܶ����գ�")},
	{TEXT("С����"),TEXT("�����͵���Ϣ���ڷ��估�����ڵ�������Ϸ����������Ŀ�ķ�ʽ��ʾ��")},	
	{TEXT("���߿�"),TEXT("����õ��ߺ������ٵ��ı���Ա�߳���Ϸ����")},
	{TEXT("�����"),TEXT("����õ��ߺ�����ֲ��۷֣�����Ϊ���ֶ����գ���ע��ֻ���ڹ������Ϸ����ʹ�á���")},
	{TEXT("�����Ա��"),TEXT("��������/����ʱ������9���Żݣ�������ͨ��ң��к����Ա�����ǣ���Ա�ڼ䣬����ID�ڴ����������к�ɫ������ʾ,���Ƚ����������䣡")},
	{TEXT("�����Ա��"),TEXT("��������/����ʱ������9���Żݣ�������ͨ�������Ա���������Ա�����ǣ���Ա�ڼ䣬����ID�ڴ����������к�ɫ������ʾ�����Ƚ����������䣡")},
	{TEXT("�����Ա��"),TEXT("��������/����ʱ������9���Żݣ�������ͨ�����ꡢ������ң��л�Ա�����ǣ���Ա�ڼ䣬����ID�ڴ����������к�ɫ������ʾ�����Ƚ����������䣡")},
	{TEXT("�����Ա��"),TEXT("��������/����ʱ������9���Żݣ�������ͨ�����ꡢ���ꡢ������ң��л�Ա�����ǣ���Ա�ڼ䣬����ID�ڴ����������к�ɫ������ʾ�����Ƚ����������䣡")},

	{TEXT("�����Ա��"),TEXT("��Ա���д����ʺź�ɫ��ʾ������ͷ�����˹��ܣ����߹�����ۣ����Ƚ����������䣡")},
	{TEXT("�����Ա��"),TEXT("��Ա���д����ʺź�ɫ��ʾ������ͷ�����˹��ܣ����߹�����ۣ����Ƚ����������䣡")},
	{TEXT("7�����Ա��"),TEXT("��Ա���д����ʺź�ɫ��ʾ������ͷ�����˹��ܣ����߹�����ۣ����Ƚ����������䣡")},
	{TEXT("������Ա��"),TEXT("��Ա���д����ʺź�ɫ��ʾ������ͷ�����˹��ܣ����߹�����ۣ����Ƚ����������䣡")}
};

//������Ϣ�ṹ
struct tagPropertyInfo
{
	int							nPropertyID;						//����ID
	DWORD						dwPropCount1;						//������Ŀ
	DWORD						dwPropCount2;						//������Ŀ
	DWORD						dwPropCount3;						//������Ŀ
	DWORD						dwPropCount4;						//������Ŀ
	DWORD						dwPropCount5;						//������Ŀ
	DWORD						dwPropCount6;						//������Ŀ
	DWORD						dwPropCount7;						//������Ŀ
	DWORD						dwPropCount8;						//������Ŀ
	DWORD						dwPropCount9;						//������Ŀ
	DWORD						dwPropCount10;						//������Ŀ
	LONG						lPrice1;							//���߼۸�
	LONG						lPrice2;							//���߼۸�
	LONG						lPrice3;							//���߼۸�
	LONG						lPrice4;							//���߼۸�
	LONG						lPrice5;							//���߼۸�
	LONG						lPrice6;							//���߼۸�
	LONG						lPrice7;							//���߼۸�
	LONG						lPrice8;							//���߼۸�
	LONG						lPrice9;							//���߼۸�
	LONG						lPrice10;							//���߼۸�
	BYTE						cbDiscount;							//��Ա�ۿ�
	bool						bNullity;							//��ֹ��ʶ
};

//�ʻ���Ϣ�ṹ
struct tagFlowerInfo
{
	int							nFlowerID;							//�ʻ�ID
	LONG						lPrice;								//�ʻ��۸�
	LONG						lSendUserCharm;						//��������
	LONG						lRcvUserCharm;						//��������
	BYTE						cbDiscount;							//��Ա�ۿ�
	bool						bNullity;							//��ֹ��ʶ
};

//////////////////////////////////////////////////////////////////////////////

#endif