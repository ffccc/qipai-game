

----------------------------------------------------------------------------------------------------

USE QPTreasureDB
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_ExchangeLoveliness]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_ExchangeLoveliness]
GO

----------------------------------------------------------------------------------------------------
-- ����ֵ�һ�
CREATE PROC GSP_GR_ExchangeLoveliness
	@dwUserID INT,
	@lLoveliness BIGINT,
	@lInsureScore BIGINT,
	@dwKindID INT,
	@wServerID INT,
	@strClientIP NVARCHAR(15)
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

DECLARE @CharmValue BIGINT

-- ִ���߼�
BEGIN
	-- �����¼
	INSERT RecordExchangeLoveliness (UserID,KindID,ServerID,Loveliness,InsureScore,ClientIP)
	VALUES (@dwUserID,@dwKindID,@wServerID,@lLoveliness,@lInsureScore,@strClientIP)	

END

RETURN 0

GO


----------------------------------------------------------------------------------------------------