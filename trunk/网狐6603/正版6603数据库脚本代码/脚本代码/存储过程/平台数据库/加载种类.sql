
----------------------------------------------------------------------------------------------------

USE QPPlatformDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_LoadGameTypeItem]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_LoadGameTypeItem]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

--��������
CREATE  PROCEDURE dbo.GSP_GP_LoadGameTypeItem WITH ENCRYPTION AS

--��������
SET NOCOUNT ON

--��ѯ����
SELECT * FROM GameTypeItem(NOLOCK) WHERE Nullity=0 ORDER BY SortID

RETURN 0

GO

----------------------------------------------------------------------------------------------------
