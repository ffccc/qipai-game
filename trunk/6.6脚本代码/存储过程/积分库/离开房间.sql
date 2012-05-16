
----------------------------------------------------------------------------------------------------
-- �����û�����(�޸ĺ��)���������,@lScore(�ı�ֵ)
----------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------

USE QPGameScoreDB
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
	@dwUserID INT,								-- �û� I D
	@lScore BIGINT,								-- �û�����
	@lGameGold BIGINT,							-- ��Ϸ���
	@lInsureScore BIGINT,						-- ��Ϸ���
	@lLoveliness BIGINT,						-- �������	
	@lRevenue BIGINT,							-- �û�˰��
	@lWinCount INT,								-- ʤ������
	@lLostCount INT,							-- ʧ������
	@lDrawCount INT,							-- �;�����
	@lFleeCount INT,							-- ������Ŀ
	@lExperience INT,							-- �û�����
	@dwPlayTimeCount INT,						-- ��Ϸʱ��
	@dwOnLineTimeCount INT,						-- ����ʱ��
	@wKindID INT,								-- ��Ϸ I D
	@wServerID INT,								-- ���� I D
	@strClientIP NVARCHAR(15)					-- ���ӵ�ַ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- �û�����
	UPDATE GameScoreInfo SET Score=Score+@lScore, WinCount=WinCount+@lWinCount, LostCount=LostCount+@lLostCount, 
		DrawCount=DrawCount+@lDrawCount, FleeCount=FleeCount+@lFleeCount, PlayTimeCount=PlayTimeCount+@dwPlayTimeCount,
		OnLineTimeCount=OnLineTimeCount+@dwOnLineTimeCount
	WHERE UserID=@dwUserID

	-- �뿪����
	INSERT RecordUserLeave (UserID, Score, KindID, ServerID, PlayTimeCount, OnLineTimeCount) 
	VALUES (@dwUserID, @lScore, @wKindID, @wServerID, @dwPlayTimeCount, @dwOnLineTimeCount)

	-- ��Ҳ��� ------------------------------------------------------------------------------------
	-- �������
	DELETE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreLocker WHERE UserID=@dwUserID

	-- �ۼӽ��
	UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score=Score+@lGameGold,InsureScore=InsureScore+@lInsureScore WHERE UserID=@dwUserID

	IF @@ROWCOUNT=0
	BEGIN
		-- ���ͽ��
		INSERT QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo(UserID, RegisterIP,LastLogonIP)
		VALUES (@dwUserID, @strClientIP, @strClientIP)

		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score=Score+@lGameGold,InsureScore=InsureScore+@lInsureScore WHERE UserID=@dwUserID
	END

	-- �û�����
	INSERT QPTreasureDBLink.QPTreasureDB.dbo.RecordUserLoveliness (UserID, KindID, ServerID,Loveliness,ClientIP)
	VALUES (@dwUserID,@wKindID,@wServerID,@lLoveliness,@strClientIP)

	-- ��Ҳ��� ------------------------------------------------------------------------------------

	-- �û�����
	UPDATE QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo SET Experience=Experience+@lExperience,Loveliness=@lLoveliness WHERE UserID=@dwUserID

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------