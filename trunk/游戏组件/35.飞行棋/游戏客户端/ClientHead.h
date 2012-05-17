#ifndef CLIENT_HEAD_FILE
#define CLIENT_HEAD_FILE




//���̸��Ӷ���
struct tagClientTileItem
{
	USHORT							usXPos;								//���̸�������
	USHORT							usYPos;								//���̸�������
	BYTE							cbColor;							//���̸�����ɫ
	BYTE							cbChessCount;						//���̵�������
};

//���Ӷ���
struct tagClientChessItem
{
	BYTE							cbTileIndex;						//���������̵�λ��
	BYTE							cbColor;							//������ɫ
	BYTE							cbChessID;							//���ӱ�ʶ���ɻ�0��1��2��3��
	BYTE							cbChessStatus;						//����״̬��0Ϊ�ȴ���ɣ�1Ϊ��ɣ�2Ϊ�ߣ�3Ϊ���죬4Ϊ��ɻػ��ء�
	USHORT							usDirection;						//�ɻ�����
	float							fXPos;								//���̸�������
	float							fYPos;								//���̸�������
};

#define CHESSMOVE_NOT_DEFINE		0xFF								//û�ж���
#define CHESSMOVE_TO_FINISH			0xFE								//�ɻ���ɣ�0��0xFDΪCHESSMOVE���Ե�������
//���̸��Ӷ���
struct ChessMoveRoute
{
	BYTE							cbMoveMapNum;						//�����ƶ�ID
	BYTE							cbChessMoveFlag;					//�����ƶ���־
	BYTE							cbRefChessMove;						//����������������±꣬�ڳ��ӵ�ʱ������
	USHORT							usXPos;								//·����������
	USHORT							usYPos;								//·����������
	float							fXIncrement;
	float							fYIncrement;
};



#define NULL_TILE					0
#define NORMAL_TILE					1
#define	ROUND_TILE					2
#define SKY_TILE					3
#define BASE_TILE					4
#define VOLPLANE_TILE				5

#define TILE_W						34
#define TILE_H						34

#define PLANE_W						43
#define PLANE_H						43

#define PLANE_TILE_XOFFSET			-4	
#define PLANE_TILE_YOFFSET			-4

#define			IDM_SOUND_MUSIC						WM_USER+161			//������Ϣ
#define			IDM_SOUND_ARRIVE					0					//�ɻ�����ó�����	
#define			IDM_SOUND_CRASH						1					//�ɻ���������	
#define			IDM_SOUND_LANDOFF					2					//�ɻ��������
#define			IDM_SOUND_TZBUTTON					3					//������������

const DWORD aSeatColor[GAME_PLAYER] = {0xFFFF0000, 0xFFFFFF00, 0xFF00FF00, 0xFF0000FF};
#endif