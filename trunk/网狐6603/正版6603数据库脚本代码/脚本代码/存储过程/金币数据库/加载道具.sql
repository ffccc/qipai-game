
----------------------------------------------------------------------------------------------------

USE QPTreasureDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_LoadGameProperty]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_LoadGameProperty]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- ���ص���
CREATE PROC GSP_GR_LoadGameProperty
	@wKindID SMALLINT,							-- ��Ϸ I D
	@wServerID SMALLINT							-- ���� I D
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ���ص���
	SELECT ID, IssueArea, Cash, Gold, Discount, SendLoveLiness, RecvLoveLiness FROM GameProperty(NOLOCK)
	WHERE Nullity=0 AND (IssueArea&6)<>0

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------