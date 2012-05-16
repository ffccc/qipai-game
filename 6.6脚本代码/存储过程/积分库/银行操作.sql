
----------------------------------------------------------------------------------------------------

USE QPGameScoreDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_ChangePassword]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_ChangePassword]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- 绑定机器
CREATE PROC GSP_GR_ChangePassword
	@dwUserID INT,								-- 用户ID	
	@strNewPassword NCHAR(32)
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN	

	-- 更新信息
	UPDATE QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo SET InsurePass=@strNewPassword WHERE UserID=@dwUserID

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
