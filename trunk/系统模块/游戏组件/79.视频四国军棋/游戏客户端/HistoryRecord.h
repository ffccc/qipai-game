#pragma once
#ifndef HISTORYRECORD_HEAD_FILE
#define HISTORYRECORD_HEAD_FILE

//��ʷ��¼
#include "afxtempl.h"

struct SaveChessStruct
{
	BYTE									bSourceX ;							//��ʼλ��
	BYTE									bSourceY ;							//��ʼλ��
	BYTE									bTargetX ;							//Ŀ��λ��
	BYTE									bTargetY ;							//Ŀ��λ��
};



/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

 ע�⣺m_Chess��SaveChessStruct������꣬����ʱ�ǰ��շ��������걣��ģ�����ȡʱ����Կͻ���ת���ɿͻ��˵�����

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
class CHistoryRecord
{
public:
	CHistoryRecord(void);
	~CHistoryRecord(void);

public:
	CList<SaveChessStruct,SaveChessStruct>	m_list;								//����յ�
public:

	tagChessItem							m_Chess[17][17];					//������
	BYTE									m_Player;							//�������
	POSITION								m_pos;								//λ�ñ�־

public:
	//���沼��
	void SavePlacement(const tagChessItem Chess[17][17]);
	//�������
	void AddToList(SaveChessStruct);
	//ɾ������
	void RemoveAllFromList();
	//��ȡ����
	bool GetFirstFromList(SaveChessStruct &ChessStruct);
	//��ȡ����
	bool GetNextFromList(SaveChessStruct &ChessStruct);
	//��ȡ����
	bool GetPrevFromList(SaveChessStruct &ChessStruct);
	//��ȡ����
	bool GetOneFromList(short num , SaveChessStruct &ChessStruct);
	//��ȡ����
	LONG GetStepCount() ;
	//��ʼ����
	void Init();
	//����ת��
	void ClientToServer(const BYTE bChessColor , const BYTE bRealClientX , const BYTE bRealClientY , BYTE &bServerX , BYTE &bServerY)  ;
};
#endif
