
----------------------------------------------------------------------------------------------------

USE QPPlatformDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GS_LoadGameGameItem]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GS_LoadGameGameItem]
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- ����ģ��
CREATE  PROCEDURE dbo.GSP_GS_LoadGameGameItem
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ����ģ��
	SELECT * FROM GameGameItem(NOLOCK) ORDER BY GameName

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
