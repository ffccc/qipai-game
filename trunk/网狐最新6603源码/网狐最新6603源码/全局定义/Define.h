#ifndef DEFINE_HEAD_FILE
#define DEFINE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//��ֵ����

//ͷ���С
#define FACE_CX						48									//ͷ����
#define FACE_CY						48									//ͷ��߶�

//���ȶ���
#define LEN_LESS_ACCOUNTS			6									//����ʺ�
#define LEN_LESS_NICKNAME			6									//����ǳ�
#define LEN_LESS_PASSWORD			6									//�������

//��������
#define MAX_CHAIR					100									//�������
#define MAX_TABLE					512									//�������
#define MAX_COLUMN					32									//����б�
#define MAX_ANDROID					256									//������
#define MAX_PROPERTY				128									//������
#define MAX_WHISPER_USER			16									//���˽��

//�б���
#define MAX_KIND					128									//�������
#define MAX_SERVER					1024								//��󷿼�

//��������
#define INVALID_CHAIR				0xFFFF								//��Ч����
#define INVALID_TABLE				0xFFFF								//��Ч����

//˰�ն���
#define REVENUE_BENCHMARK			1000L								//˰�����
#define REVENUE_DENOMINATOR			1000L								//˰�շ�ĸ

//���ж���
#define INSURE_LESS_SCORE			1000L								//�������
#define INSURE_REVENUE_RATIO		1L/1000L							//����˰��

//////////////////////////////////////////////////////////////////////////////////
//ϵͳ����

//��������
#define SCORE						LONGLONG							//��������
#define SCORE_STRING				TEXT("%I64d")						//��������

//��Ϸ״̬
#define GAME_STATUS_FREE			0									//����״̬
#define GAME_STATUS_PLAY			100									//��Ϸ״̬
#define GAME_STATUS_WAIT			200									//�ȴ�״̬

//ϵͳ����
#define LEN_USER_CHAT				128									//���쳤��
#define TIME_USER_CHAT				1L									//������

//////////////////////////////////////////////////////////////////////////////////
//��������

//�б�����
#define PRIME_TYPE					11L									//������Ŀ
#define PRIME_KIND					53L									//������Ŀ
#define PRIME_NODE					101L								//�ڵ���Ŀ
#define PRIME_PAGE					53L									//�Զ���Ŀ
#define PRIME_SERVER				1009L								//������Ŀ

//��������
#define PRIME_SERVER_USER			503L								//��������
#define PRIME_ANDROID_USER			503L								//��������
#define PRIME_PLATFORM_USER			100003L								//ƽ̨����

//////////////////////////////////////////////////////////////////////////////////
//���ݳ���

//��������
#define LEN_MD5						33									//��������
#define LEN_USERNOTE				32									//��ע����
#define LEN_ACCOUNTS				32									//�ʺų���
#define LEN_NICKNAME				32									//�ǳƳ���
#define LEN_PASSWORD				33									//���볤��
#define LEN_GROUP_NAME				32									//��������
#define LEN_UNDER_WRITE				32									//����ǩ��

//���ݳ���
#define LEN_QQ						16									//Q Q ����
#define LEN_EMAIL					33									//�����ʼ�
#define LEN_USER_NOTE				256									//�û���ע
#define LEN_SEAT_PHONE				33									//�̶��绰
#define LEN_MOBILE_PHONE			12									//�ƶ��绰
#define LEN_PASS_PORT_ID			19									//֤������
#define LEN_COMPELLATION			16									//��ʵ����
#define LEN_DWELLING_PLACE			128									//��ϵ��ַ

//������ʶ
#define LEN_NETWORK_ID				13									//��������
#define LEN_MACHINE_ID				33									//���г���

//�б�����
#define LEN_TYPE					32									//���೤��
#define LEN_KIND					32									//���ͳ���
#define LEN_NODE					32									//�ڵ㳤��
#define LEN_PAGE					32									//���Ƴ���
#define LEN_SERVER					32									//���䳤��
#define LEN_PROCESS					32									//���̳���

//////////////////////////////////////////////////////////////////////////////////

//�û���ϵ
#define	CP_NORMAL					0									//δ֪��ϵ
#define	CP_FRIEND					1									//���ѹ�ϵ
#define	CP_DETEST					2									//����ϵ

//////////////////////////////////////////////////////////////////////////////////

//�Ա���
#define GENDER_FEMALE				0									//Ů���Ա�
#define GENDER_MANKIND				1									//�����Ա�

//////////////////////////////////////////////////////////////////////////////////

//��Ϸģʽ
#define GAME_GENRE_GOLD				0x0001								//�������
#define GAME_GENRE_SCORE			0x0002								//��ֵ����
#define GAME_GENRE_MATCH			0x0004								//��������
#define GAME_GENRE_EDUCATE			0x0008								//ѵ������

//����ģʽ
#define SCORE_GENRE_NORMAL			0x0100								//��ͨģʽ
#define SCORE_GENRE_POSITIVE		0x0200								//�Ǹ�ģʽ

//////////////////////////////////////////////////////////////////////////////////

//�û�״̬
#define US_NULL						0x00								//û��״̬
#define US_FREE						0x01								//վ��״̬
#define US_SIT						0x02								//����״̬
#define US_READY					0x03								//ͬ��״̬
#define US_LOOKON					0x04								//�Թ�״̬
#define US_PLAYING					0x05								//��Ϸ״̬
#define US_OFFLINE					0x06								//����״̬

//////////////////////////////////////////////////////////////////////////////////

//�������
#define SRL_LOOKON					0x00000001							//�Թ۱�־
#define SRL_OFFLINE					0x00000002							//���߱�־
#define SRL_SAME_IP					0x00000004							//ͬ����־

//�������
#define SRL_ROOM_CHAT				0x00000100							//�����־
#define SRL_GAME_CHAT				0x00000200							//�����־
#define SRL_WISPER_CHAT				0x00000400							//˽�ı�־
#define SRL_HIDE_USER_INFO			0x00000800							//���ر�־

//////////////////////////////////////////////////////////////////////////////////
//�б�����

//��Ч����
#define UD_NULL						0									//��Ч����
#define UD_IMAGE					100									//ͼ������
#define UD_CUSTOM					200									//�Զ�����

//��������
#define UD_GAME_ID					1									//��Ϸ��ʶ
#define UD_USER_ID					2									//�û���ʶ
#define	UD_NICKNAME					3									//�û��ǳ�

//��չ����
#define UD_GENDER					10									//�û��Ա�
#define UD_GROUP_NAME				11									//��������
#define UD_UNDER_WRITE				12									//����ǩ��

//״̬��Ϣ
#define UD_TABLE					20									//��Ϸ����
#define UD_CHAIR					21									//���Ӻ���

//������Ϣ
#define UD_SCORE					30									//�û�����
#define UD_GRADE					31									//�û��ɼ�
#define UD_USER_MEDAL				32									//�û�����
#define UD_EXPERIENCE				33									//�û�����
#define UD_LOVELINESS				34									//�û�����
#define UD_WIN_COUNT				35									//ʤ������
#define UD_LOST_COUNT				36									//�������
#define UD_DRAW_COUNT				37									//�;�����
#define UD_FLEE_COUNT				38									//�Ӿ�����
#define UD_PLAY_COUNT				39									//�ܾ�����

//���ֱ���
#define UD_WIN_RATE					40									//�û�ʤ��
#define UD_LOST_RATE				41									//�û�����
#define UD_DRAW_RATE				42									//�û�����
#define UD_FLEE_RATE				43									//�û�����
#define UD_GAME_LEVEL				44									//��Ϸ�ȼ�

//��չ��Ϣ
#define UD_NOTE_INFO				50									//�û���ע
#define UD_LOOKON_USER				51									//�Թ��û�

//ͼ���б�
#define UD_IMAGE_FLAG				(UD_IMAGE+1)						//�û���־
#define UD_IMAGE_GENDER				(UD_IMAGE+2)						//�û��Ա�
#define UD_IMAGE_STATUS				(UD_IMAGE+3)						//�û�״̬

//////////////////////////////////////////////////////////////////////////////////
//���ݿⶨ��

#define DB_ERROR 					-1  								//����ʧ��
#define DB_SUCCESS 					0  									//����ɹ�

//////////////////////////////////////////////////////////////////////////////////

#endif