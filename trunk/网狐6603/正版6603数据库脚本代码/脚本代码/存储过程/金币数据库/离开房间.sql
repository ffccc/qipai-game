
----------------------------------------------------------------------------------------------------

USE QPTreasureDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_LeaveGameServer]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_LeaveGameServer]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- �뿪����
CREATE PROC GSP_GR_LeaveGameServer

	-- �û���Ϣ
	@dwUserID INT,								-- �û� I D
	@dwOnLineTimeCount INT,						-- ����ʱ��

	-- ϵͳ��Ϣ
	@dwInoutIndex INT,							-- ��������
	@dwLeaveReason INT,							-- �뿪ԭ��

	-- ��¼�ɼ�
	@lRecordScore BIGINT,						-- �û�����
	@lRecordGrade BIGINT,						-- �û��ɼ�
	@lRecordInsure BIGINT,						-- �û�����
	@lRecordRevenue BIGINT,						-- ��Ϸ˰��
	@lRecordWinCount INT,						-- ʤ������
	@lRecordLostCount INT,						-- ʧ������
	@lRecordDrawCount INT,						-- �;�����
	@lRecordFleeCount INT,						-- ������Ŀ
	@lRecordUserMedal INT,						-- �û�����
	@lRecordExperience INT,						-- �û�����
	@lRecordLoveLiness INT,						-- �û�����
	@dwRecordPlayTimeCount INT,					-- ��Ϸʱ��

	-- ����ɼ�
	@lVariationScore BIGINT,					-- �û�����
	@lVariationGrade BIGINT,					-- �û��ɼ�
	@lVariationInsure BIGINT,					-- �û�����
	@lVariationRevenue BIGINT,					-- ��Ϸ˰��
	@lVariationWinCount INT,					-- ʤ������
	@lVariationLostCount INT,					-- ʧ������
	@lVariationDrawCount INT,					-- �;�����
	@lVariationFleeCount INT,					-- ������Ŀ
	@lVariationUserMedal INT,					-- �û�����
	@lVariationExperience INT,					-- �û�����
	@lVariationLoveLiness INT,					-- �û�����
	@dwVariationPlayTimeCount INT,				-- ��Ϸʱ��

	-- ������Ϣ
	@wKindID INT,								-- ��Ϸ I D
	@wServerID INT,								-- ���� I D
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strMachineSerial NVARCHAR(32)				-- ������ʶ

WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��ʼ����
	SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
	BEGIN TRAN

	-- �û�����
	UPDATE GameScoreInfo SET Score=Score+@lVariationScore, InsureScore=InsureScore+@lVariationInsure, Revenue=Revenue+@lVariationRevenue,
		WinCount=WinCount+@lVariationWinCount, LostCount=LostCount+@lVariationLostCount, DrawCount=DrawCount+@lVariationDrawCount,
		FleeCount=FleeCount+@lVariationFleeCount, PlayTimeCount=PlayTimeCount+@dwVariationPlayTimeCount, OnLineTimeCount=OnLineTimeCount+@dwOnLineTimeCount
	WHERE UserID=@dwUserID

	-- �������
	DELETE GameScoreLocker WHERE UserID=@dwUserID AND ServerID=@wServerID

	-- �뿪��¼
	UPDATE RecordUserInout SET LeaveTime=GetDate(), LeaveReason=@dwLeaveReason, LeaveMachine=@strMachineSerial,LeaveClientIP=@strClientIP,
		Score=@lRecordScore, Grade=@lRecordGrade, Insure=@lRecordInsure, Revenue=@lRecordRevenue, WinCount=@lRecordWinCount, LostCount=@lRecordLostCount,
		DrawCount=@lRecordDrawCount, FleeCount=@lRecordFleeCount, UserMedal=@lRecordUserMedal, Experience=@lRecordExperience, LoveLiness=@lRecordLoveLiness,
		PlayTimeCount=@dwRecordPlayTimeCount, OnLineTimeCount=@dwOnLineTimeCount
	WHERE ID=@dwInoutIndex AND UserID=@dwUserID

	-- ��������
	COMMIT TRAN
	SET TRANSACTION ISOLATION LEVEL READ COMMITTED

	-- ȫ����Ϣ
	IF @lVariationExperience>0 OR @lVariationLoveLiness>0 OR @lVariationUserMedal>0
	BEGIN
		UPDATE QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo SET Experience=Experience+@lVariationExperience, LoveLiness=@lVariationLoveLiness
		WHERE UserID=@dwUserID
	END

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------