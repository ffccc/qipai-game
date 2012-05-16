

----------------------------------------------------------------------------------------------------

USE QPTreasureDB
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_SavingGameGold]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_SavingGameGold]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_DrawoutGameGold]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_DrawoutGameGold]
GO


IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_Transfer]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_Transfer]
GO
----------------------------------------------------------------------------------------------------

-- �洢���
CREATE PROC GSP_GR_SavingGameGold
	@dwUserID INT,
	@lSwapScore BIGINT,
	@dwKindID INT,
	@dwServerID INT,
	@strClientIP NVARCHAR(15)
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

DECLARE @SourceGameID INT
DECLARE @SourceAccounts NVARCHAR(31)
DECLARE @InsureScore BIGINT
DECLARE @TradeType TINYINT

-- ִ���߼�
BEGIN
	SET @TradeType=1
	SELECT @InsureScore=InsureScore FROM GameScoreInfo WHERE UserID=@dwUserID

--	UPDATE GameScoreInfo SET Score=Score-@lSwapScore,InsureScore=InsureScore+@lSwapScore
--	WHERE UserID=@dwUserID

	SELECT @SourceGameID=GameID,@SourceAccounts=Accounts FROM QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo
	WHERE UserID=@dwUserID

	INSERT RecordInsure(KindID,ServerID,SourceUserID,SourceGameID,SourceAccounts,InsureScore,SwapScore,TradeType,ClientIP)
	VALUES (@dwKindID,@dwServerID,@dwUserID,@SourceGameID,@SourceAccounts,@InsureScore,@lSwapScore,@TradeType,@strClientIP)
	
END
RETURN 0
GO

----------------------------------------------------------------------------------------------------
-- ȡ�����
CREATE PROC GSP_GR_DrawoutGameGold
	@dwUserID INT,
	@lSwapScore BIGINT,
	@dwKindID INT,
	@dwServerID INT,
	@strClientIP NVARCHAR(15)
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

DECLARE @SourceGameID INT
DECLARE @SourceAccounts NVARCHAR(31)
DECLARE @InsureScore BIGINT
DECLARE @TradeType TINYINT

-- ִ���߼�
BEGIN
	SET @TradeType=2
	SELECT @InsureScore=InsureScore FROM GameScoreInfo WHERE UserID=@dwUserID

--	UPDATE GameScoreInfo SET Score=Score+@lSwapScore,InsureScore=InsureScore-@lSwapScore
--	WHERE UserID=@dwUserID

	SELECT @SourceGameID=GameID,@SourceAccounts=Accounts FROM QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo
	WHERE UserID=@dwUserID

	INSERT RecordInsure(KindID,ServerID,SourceUserID,SourceGameID,SourceAccounts,InsureScore,SwapScore,TradeType,ClientIP)
	VALUES (@dwKindID,@dwServerID,@dwUserID,@SourceGameID,@SourceAccounts,@InsureScore,@lSwapScore,@TradeType,@strClientIP)
END

RETURN 0

GO




-- ת�˼�¼
CREATE PROC GSP_GR_Transfer
	@dwSourceUserID INT,
	@dwTargetUserID INT,
	@lSwapScore BIGINT,
	@Revenue INT,
	@dwKindID INT,
	@dwServerID INT,
	@strClientIP NVARCHAR(15)
WITH ENCRYPTION AS

DECLARE @AccountsSource NVARCHAR(31)
DECLARE @AccountsTarget NVARCHAR(31)

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN
	SELECT @AccountsSource=Accounts FROM QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo WHERE UserID=@dwSourceUserID
	SELECT @AccountsTarget=Accounts FROM QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo WHERE UserID=@dwTargetUserID
	INSERT QPTreasureDBLink.QPTreasureDB.dbo.RecordInsure(KindID,ServerID,SourceUserID,SourceGameID,SourceAccounts,InsureScore,SwapScore,TradeType,ClientIP,
	TargetUserID,Revenue,TargetAccounts)
	VALUES (@dwKindID,@dwServerID,@dwSourceUserID,0,@AccountsSource,0,@lSwapScore,3,@strClientIP,@dwTargetUserID,@Revenue,@AccountsTarget)
END

RETURN 0

GO
