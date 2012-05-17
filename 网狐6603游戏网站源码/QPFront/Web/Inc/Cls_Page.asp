<%
Class Cls_Page
	Private oConn
	Private iPageSize
	Private iRecordCount
	Private iPageIndex
	Private sPageParm
	Private sDatabase
	Private sField
	Private sTable
	Private sCondition
	Private sOrderBy
	Private sPKey
	Private iPageCount
	Private sTemplate
	Private sFirstPage
	Private sPreviousPage
	Private sNextPage
	Private sLastPage
	Private sNumericPage
	Private iNumericJump
	Private Pager 

	Private Sub Class_Initialize
		iPageSize = 10
		sPageParm = "p"
		iPageIndex = -1
		sDatabase = "access"
		iRecordCount = 0
		sField=" * "
		sCondition = ""
		sOrderBy = ""
		iPageCount = 0
		sTemplate = "总记录数：{$RecordCount} 总页数：{$PageCount} 每页记录数：{$PageSize} 当前页数：{$PageIndex} {$FirstPage} {$PreviousPage} {$NumericPage} {$NextPage} {$LastPage} {$InputPage} {$SelectPage}"
		sFirstPage = "First"
		sPreviousPage = "Previous"
		sNextPage = "Next"
		sLastPage = "Last"
		sNumericPage = " {$PageNum} "
		iNumericJump = 3
		Set Pager = new Cls_Pager
	End Sub

	Private Sub Class_Terminate()
		If IsObject(oConn) Then oConn.Close:Set oConn=Nothing		
		Pager = Null:Set Pager = Nothing
	End Sub

	Public Property Set Conn(ByRef o)
		Set oConn = o
	End Property

	Public Property Let PageSize(ByVal i)
		iPageSize = CheckNum(i,0,-1) 
	End Property

	Public Property Let PageParm(ByVal s)
		If Len(s)>0 Then sPageParm = s
	End Property

	Public Property Let PageIndex(ByVal i)
		iPageIndex = CheckNum(i,0,-1)
	End Property

	Public Property Let Database(ByVal s)
		If Len(s)>0 Then sDatabase = LCase(s)
	End Property

	Public Property Let Field(ByVal s)
		If Len(s)>0 Then sField = s
	End Property

	Public Property Let Table(ByVal s)
		sTable = s
	End Property

	Public Property Let Condition(ByVal s)
		If Len(s)>0 Then sCondition = " WHERE " & s
	End Property

	Public Property Let OrderBy(ByVal s)
		If Len(s)>0 Then sOrderBy = " ORDER BY " & s
	End Property

	Public Property Let PKey(ByVal s)
		sPKey = s
	End Property

	Public Property Let Template(ByVal s)
		If Len(s)>0 Then sTemplate = s
	End Property

	Public Property Let FirstPage(ByVal s)
		sFirstPage = s
	End Property

	Public Property Let PreviousPage(ByVal s)
		sPreviousPage = s
	End Property

	Public Property Let NextPage(ByVal s)
		sNextPage = s
	End Property

	Public Property Let LastPage(ByVal s)
		sLastPage = s
	End Property

	Public Property Let NumericPage(ByVal s)
		sNumericPage = s
	End Property

	Public Property Let NumericJump(ByVal i)
		iNumericJump = CheckNum(i,-1,-1)
	End Property

	Public Property Let RecordCount(ByVal i)
		Select Case i
			Case -1
				If Session("yevg") = "" Then
					iRecordCount = RowCount()
					Session("yevg") = iRecordCount
				Else
					iRecordCount = Session("yevg")
				End If
			Case -2
				If Request.Cookies("yevg") = "" Then
					iRecordCount = RowCount()
					Response.Cookies("yevg") = iRecordCount
				Else
					iRecordCount = Request.Cookies("yevg")
				End If
			Case -3
				If Application("yevg") = "" Then
					iRecordCount = RowCount()
					Application.Lock
					Application("yevg") = iRecordCount
					Application.Unlock
				Else
					iRecordCount = Application("yevg")
				End If
			Case Else
				If i < 1 Then
					iRecordCount = RowCount()
				Else
					iRecordCount = i
				End If
		End Select
	End Property

	Public Property Get RowCount()
		If iRecordCount < 1 Then
			RowCount = oConn.Execute("SELECT COUNT(*) FROM " & sTable & " " & sCondition,0,1)(0)
		Else
			RowCount = iRecordCount
		End If
	End Property

	Public Property Get ResultSet()
		ResultSet = Null
		If iRecordCount > 0 Then
			Dim Rs,Cm
			iPageCount = (iRecordCount + iPageSize - 1) \ iPageSize
			If iPageIndex = -1 Then iPageIndex = Request.QueryString(sPageParm)
			iPageIndex = CheckNum(iPageIndex,1,iPageCount)
			Select Case sDatabase
				Case "mssql"
					Set Rs=server.CreateObject("Adodb.RecordSet")
					Set Cm=Server.CreateObject("Adodb.Command")
					Cm.CommandType = 4
					Cm.ActiveConnection = oConn
					Cm.CommandText="sp_Util_Page"
					Cm.parameters(1) = sField
					Cm.parameters(2) = sTable
					Cm.parameters(3) = sCondition
					Cm.parameters(4) = sOrderBy
					Cm.parameters(5) = sPKey
					Cm.parameters(6) = iPageIndex
					Cm.parameters(7) = iPageSize
					Cm.parameters(8) = iRecordCount
					Cm.parameters(9) = ""
					Rs.CursorLocation = 3
					Rs.LockType = 1
					Rs.Open Cm
				Case "mysql"
					Set Rs = oConn.Execute("SELECT " & sField & " FROM " & sTable & " " & sCondition & " " & sOrderBy & " LIMIT " & (iPageIndex-1)*iPageSize & "," & iPageSize)
				Case Else
					Set Rs = Server.CreateObject ("Adodb.RecordSet")
					Rs.Open "SELECT " & sField & " FROM " & sTable & " " & sCondition & " " & sOrderBy,oConn,1,1,&H0001
					'此处是为了修正access记录多并且最后一页只有一条记录则报Eof的bug，坏处就是最后一页会显示2条=___=
					If iPageIndex > 1 And iPageIndex = iPageCount And (iRecordCount Mod iPageSize = 1) Then
						Rs.AbsolutePosition = (iPageIndex - 1) * iPageSize
					Else
						Rs.AbsolutePosition = (iPageIndex - 1) * iPageSize + 1
					End If
			End Select
			If Not Rs.Eof Then ResultSet = Rs.GetRows(iPageSize)
			Rs.close
			Set Rs=Nothing
		End If
	End Property

	Private Function CheckNum(ByVal s,ByVal min,ByVal max)
		Dim i:i = 0
		s = Left(Trim("" & s),32)
		If IsNumeric(s) Then i = CDbl(s)
		If (min>-1) And (i < min) Then i = min
		If (max>-1) And (i > max) Then i = max
		CheckNum = i
	End Function

	Public Function Nav()
		With Pager
			.PageSize = iPageSize
			.RecordCount = iRecordCount
			.PageParm = sPageParm
			.NumericJump = iNumericJump
			.Template = sTemplate
			.FirstPage = sFirstPage
			.PreviousPage = sPreviousPage
			.NextPage = sNextPage
			.LastPage = sLastPage
			.NumericPage = sNumericPage
		End With
		Nav = Pager.Nav()
	End Function

End Class

Class Cls_Pager
	Private iPageIndex
	Private iPageSize
	Private iPageCount
	Private iRecordCount

	Private sPageUrl
	Private sPageParm
	Private sTemplate
	Private sFirstPage
	Private sPreviousPage
	Private sNextPage
	Private sLastPage
	Private sNumericPage
	Private iNumericJump

	Private Sub Class_Initialize()
		iPageIndex = -1
		iPageSize = 10
		iPageCount = 0
		iRecordCount = 0

		sPageUrl = ""
		sPageParm = "p"
		sTemplate = "总记录数：{$RecordCount} 总页数：{$PageCount} 每页记录数：{$PageSize} 当前页数：{$PageIndex} {$FirstPage} {$PreviousPage} {$NumericPage} {$NextPage} {$LastPage} {$InputPage} {$SelectPage}"
		sFirstPage = "First"
		sPreviousPage = "Previous"
		sNextPage = "Next"
		sLastPage = "Last"
		sNumericPage = " {$PageNum} "
		iNumericJump = 3
	End Sub

	Private Sub Class_Terminate()

	End Sub

	Public Property Let PageSize(ByVal i)
		iPageSize = CheckNum(i,1,-1)
	End Property

	Public Property Let PageCount(ByVal i)
		iPageCount = CheckNum(i,0,-1)
	End Property

	Public Property Let RecordCount(ByVal i)
		iRecordCount = CheckNum(i,0,-1)
	End Property

	Public Property Let PageUrl(ByVal s)
		sPageUrl = s
	End Property

	Public Property Let PageIndex(ByVal i)
		iPageIndex = CheckNum(i,0,-1)
	End Property

	Public Property Let PageParm(ByVal s)
		If Len(s)>0 Then sPageParm = s
	End Property

	Public Property Let Template(ByVal s)
		If Len(s)>0 Then sTemplate = s
	End Property

	Public Property Let FirstPage(ByVal s)
		sFirstPage = s
	End Property

	Public Property Let PreviousPage(ByVal s)
		sPreviousPage = s
	End Property

	Public Property Let NextPage(ByVal s)
		sNextPage = s
	End Property

	Public Property Let LastPage(ByVal s)
		sLastPage = s
	End Property

	Public Property Let NumericPage(ByVal s)
		sNumericPage = s
	End Property

	Public Property Let NumericJump(ByVal i)
		iNumericJump = CheckNum(i,-1,-1)
	End Property
	

	Public Property Get Nav()
		Dim v,x,i,minNumericPage,maxNumericPage,vNumericPage,vSelectPage,vInputPage

		v = sTemplate
		minNumericPage = 0
		maxNumericPage = 0
		vNumericPage = ""
		
		If Len(sPageUrl) < 11 Then 
			sPageUrl =  "?"
			For Each x In Request.QueryString
				If x <> sPageParm Then sPageUrl =  sPageUrl & x & "=" & Request.QueryString(x) & "&"
			Next
			sPageUrl = sPageUrl & sPageParm &"={$PageNum}"
		End If
		vSelectPage = "<select onchange=""location.href = '" & sPageUrl & "'.replace('{$PageNum}',this.value);"">"
		'vInputPage = "<input id=""txtValue"" type=""text"" onkeydown=""if (event.keyCode==13){location.href = '" & sPageUrl & "'.replace('{$PageNum}',this.value);}"" />"
		vInputPage = "<input id=""txtValue"" type=""text"" />"
		vInputPage = vInputPage&"<input type=""button"" value=""GO"" onclick=""javascript:location.href = '" & sPageUrl & "'.replace('{$PageNum}',document.getElementById('txtValue').value);"">"

		If iRecordCount > 0 Then iPageCount = (iRecordCount + iPageSize - 1)\iPageSize
		If iPageIndex = -1 Then iPageIndex = Request.QueryString(sPageParm)
		iPageIndex = CheckNum(iPageIndex,1,iPageCount)

		If iPageIndex > 1 Then
			sFirstPage = "<a href=""" & Replace(sPageUrl,"{$PageNum}",1) & """>"&sFirstPage&"</a>"
			sPreviousPage = "<a href=""" & Replace(sPageUrl,"{$PageNum}",iPageIndex - 1) & """>"&sPreviousPage&"</a>"
		Else
			sFirstPage = sFirstPage
			sPreviousPage = sPreviousPage
		End If

		If iPageCount > 1 And iPageIndex < iPageCount Then
			sNextPage = "<a href=""" & Replace(sPageUrl,"{$PageNum}",iPageIndex + 1) & """>"&sNextPage&"</a>"
			sLastPage = "<a href=""" & Replace(sPageUrl,"{$PageNum}",iPageCount) & """>"&sLastPage&"</a>"
		Else
			sNextPage = sNextPage
			sLastPage = sLastPage
		End If

		If iNumericJump > 0 Then
			minNumericPage = CheckNum(iPageIndex - iNumericJump,1,-1)
			maxNumericPage = CheckNum(iPageIndex + iNumericJump,-1,iPageCount)
		ElseIf iNumericJump < 0 Then
			iNumericJump = Abs(iNumericJump)
			minNumericPage = CheckNum(((iPageIndex-1)\iNumericJump)*iNumericJump + 1,1,-1)
			maxNumericPage = CheckNum(minNumericPage + iNumericJump - 1,-1,iPageCount)
		Else
			minNumericPage = 1
			maxNumericPage = iPageCount
		End If

		For i = minNumericPage To iPageIndex - 1
			vNumericPage = vNumericPage + Replace(sNumericPage,"{$PageNum}","<a href='" & Replace(sPageUrl,"{$PageNum}",i) & "'>" & i & "</a>")
			vSelectPage = vSelectPage + "<option value='"&i&"'>"&i&"</option>"
		Next
		vNumericPage = vNumericPage + Replace(sNumericPage,"{$PageNum}",iPageIndex)
		vSelectPage = vSelectPage + "<option value='"&iPageIndex&"' selected>"&iPageIndex&"</option>"
		For i = iPageIndex + 1 To maxNumericPage
			vNumericPage = vNumericPage + Replace(sNumericPage,"{$PageNum}","<a href='" & Replace(sPageUrl,"{$PageNum}",i) & "'>" & i & "</a>")
			vSelectPage = vSelectPage + "<option value='"&i&"'>"&i&"</option>"
		Next
		vSelectPage = vSelectPage + "</select>"

		v = Replace(v,"{$RecordCount}",iRecordCount)
		v = Replace(v,"{$PageCount}",iPageCount)
		v = Replace(v,"{$PageSize}",iPageSize)
		v = Replace(v,"{$PageIndex}",iPageIndex)
		v = Replace(v,"{$FirstPage}",sFirstPage)
		v = Replace(v,"{$PreviousPage}",sPreviousPage)
		v = Replace(v,"{$NextPage}",sNextPage)
		v = Replace(v,"{$LastPage}",sLastPage)
		v = Replace(v,"{$NumericPage}",vNumericPage)
		v = Replace(v,"{$SelectPage}",vSelectPage)
		v = Replace(v,"{$InputPage}",vInputPage)

		Nav = v
	End Property

	Private Function CheckNum(ByVal s,ByVal min,ByVal max)
		Dim i:i = 0
		s = Left(Trim("" & s),32)
		If IsNumeric(s) Then i = CDbl(s)
		If (min>-1) And (i < min) Then i = min
		If (max>-1) And (i > max) Then i = max
		CheckNum = i
	End Function

End Class
%>