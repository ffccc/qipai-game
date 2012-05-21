
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

-- 加载道具
CREATE PROC GSP_GR_LoadGameProperty
	@wKindID SMALLINT,							-- 游戏 I D
	@wServerID SMALLINT							-- 房间 I D
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 加载道具
	SELECT ID, IssueArea, Cash, Gold, Discount, SendLoveLiness, RecvLoveLiness FROM GameProperty(NOLOCK)
	WHERE Nullity=0 AND (IssueArea&6)<>0

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------