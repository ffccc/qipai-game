
----------------------------------------------------------------------------------------------------

USE QPGameScoreDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_LoadAndroidUser]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_LoadAndroidUser]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- ���ػ�����
CREATE PROC GSP_GR_LoadAndroidUser
	@wKindID INT,								-- ��Ϸ I D
	@wServerID INT								-- ���� I D
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ������Ϣ
	SELECT UserID FROM AndroidUserInfo WHERE KindID=@wKindID AND ServerID=@wServerID AND Nullity=0 ORDER BY NEWID()

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------