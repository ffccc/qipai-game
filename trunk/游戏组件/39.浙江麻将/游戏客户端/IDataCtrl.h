#ifndef _IDATACTRL_H_
#define _IDATACTRL_H_

class IDataCtrl
{
public:
	virtual BYTE	GetGameStatus()						= 0;		//��ȡ��Ϸ״̬
	virtual BYTE	GetKingCardData()					= 0;		//��ȡ����
	virtual BYTE	GetKingBrotherData()				= 0;		//��ȡ����ĵܵ�
	virtual WORD	GetBankerUser()						= 0;		//��ȡׯ��
	virtual bool	IsUserReady(WORD wChairID)			= 0;		//�Ƿ�׼��
	virtual bool	IsConsign()							= 0;		//�Ƿ��й�
	virtual LONG    GetScoreLeft(WORD wChairID)			= 0;		//��ȡʣ��Ƹ�
	virtual LONG	GetCellScore()						= 0;		//��ȡ��ע

	virtual bool	IsLookOn()							= 0;		//�Ƿ��Թ���
	virtual bool	IsAllowLookOn()						= 0;		//�Ƿ������Թ�
	virtual bool	IsEnableLookOn()					= 0;		//�Ƿ�����Թ�
	virtual bool	IsEnableSound()						= 0;		//�Ƿ�������Ч

};
#endif