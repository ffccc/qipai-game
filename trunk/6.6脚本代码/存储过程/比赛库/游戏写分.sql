
----------------------------------------------------------------------------------------------------

USE QPGameMatchDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_WriteGameScore]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_WriteGameScore]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- ��Ϸд��
CREATE PROC GSP_GR_WriteGameScore
	@dwUserID INT,								-- �û� I D
	@lScore BIGINT,								-- �û�����
	@lRevenue BIGINT,							-- ��Ϸ˰��
	@lWinCount INT,								-- ʤ������
	@lLostCount INT,							-- ʧ������
	@lDrawCount INT,							-- �;�����
	@lFleeCount INT,							-- ������Ŀ
	@lExperience INT,							-- �û�����
	@dwPlayTimeCount INT,						-- ��Ϸʱ��
	@dwOnLineTimeCount INT,						-- ����ʱ��
	@wKindID INT,							-- ��Ϸ I D
	@wServerID INT,						-- ���� I D
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

	-- �û�����
	UPDATE QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo SET Experience=Experience+@lExperience WHERE UserID=@dwUserID

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------