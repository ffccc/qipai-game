
----------------------------------------------------------------------------------------------------

USE QPTreasureDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_ChangePassword]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_ChangePassword]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- �󶨻���
CREATE PROC GSP_GR_ChangePassword
	@dwUserID INT,								-- �û�ID	
	@strNewPassword NCHAR(32)
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN	

	-- ������Ϣ
	UPDATE QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo SET InsurePass=@strNewPassword WHERE UserID=@dwUserID

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
