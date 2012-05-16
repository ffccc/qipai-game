

----------------------------------------------------------------------------------------------------

USE QPGameMatchDB
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_RecordFlowerGrant]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_RecordFlowerGrant]
GO

----------------------------------------------------------------------------------------------------

-- �ʻ�����
CREATE PROC GSP_GR_RecordFlowerGrant
	@dwSendUserID INT,			-- ������
	@dwRcvUserID INT,			-- ������
	@dwSendUserLoveliness INT,	-- ����ֵ
	@dwRcvUserLoveliness INT,	-- ����ֵ
	@dwFlowerCount INT,			-- �ʻ���Ŀ
	@strFlowerName NVARCHAR(32),-- ��������
	@lFlowerPay BIGINT,			-- ���͸���
	@wKindID INT,				-- ���ͱ�ʶ
	@wServerID INT,				-- �����ʶ
	@strGrantIP	VARCHAR(15)		-- ���͵�ַ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN
	INSERT QPTreasureDBLink.QPTreasureDB.dbo.RecordFlowerGrant(SendUserID,RcvUserID,KindID,ServerID,FlowerName,FlowerCount,FlowerPay,SendUserLoveliness,RcvUserLoveliness,GrantIP)
    VALUES(@dwSendUserID,@dwRcvUserID,@wKindID,@wServerID,@strFlowerName,@dwFlowerCount,@lFlowerPay,@dwSendUserLoveliness,@dwRcvUserLoveliness,@strGrantIP)
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------