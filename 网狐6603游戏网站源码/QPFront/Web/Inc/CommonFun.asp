<%@LANGUAGE="VBSCRIPT" CODEPAGE="65001"%>
<% 
Response.Buffer=True
Response.Charset="utf-8" 
Session.CodePage=65001

Rem 游戏网页操作类
Class GameObj
    Public WebTitle,Channel,ManagerDomain
    Public MinSwapScore,RevenueRate,ConvertRate
    
    Private Sub Class_Terminate()
        
	End Sub 
	
	Private Sub Class_Initialize() 
	    ManagerDomain="http://127.0.0.1:6062"'管理后台地址(用于前台显示管理后台上传的图片)
	    WebTitle="网狐游戏棋牌演示平台"
	    MinSwapScore = 1000 '保险柜最小存取转金额
	    RevenueRate = 0.01'转账税收
	    ConvertRate = 50'魅力兑换比例
	End Sub
	

	
	Rem 获取页面信息
    Public Function GetInfo(Typ,FormName)
        GetInfo=Trim(Request(FormName))
        If Typ=1 Then
	        IF Not IsNumeric(GetInfo) Then
		        GetInfo=0
	        Else
		        GetInfo=CLng(GetInfo)
	        End IF
        End IF	
    End Function
    
    Rem 弹出信息提示
    Public Sub MessageBox(info)
        Dim rValue
        rValue = "<script>alert('"&info&"')</script>"
        Response.Write rValue
    End Sub
    
    Rem 弹出信息提示刷新页面
    Public Sub MessageBoxReload(info,url)
        Dim rValue
        rValue = "<script>alert('"&info&"');"
        rValue = rValue&"window.location.href='"&url&"'"
        rValue = rValue&"</script>"
        Response.Write rValue
    End Sub
    
    Rem 验证码验证
	Public Function CodeIsTrue()
		Dim CodeStr
		CodeStr=Lcase(Trim(Request("txtCode")))
		If CStr(Lcase(Session("GetCode")))=CStr(CodeStr) And CodeStr<>""  Then
			CodeIsTrue=True
			Session("GetCode")=empty
		Else
			CodeIsTrue=False
			Session("GetCode")=empty
		End If	
	End Function 
	
	Rem 获取IP
	Public Function GetIP()
	    Dim IP
	    IP = Left(Replace(Request.ServerVariables("HTTP_X_FORWARDED_FOR"),"'",""),15)
		If IP = "" Then IP = Request.ServerVariables("REMOTE_ADDR")
		GetIP = IP
	End Function
    
    Rem 获取新闻类型
    Public Function GetNewsTypeName(typeID)
        Dim rValue
        If typeID=1 Then
            rValue = "新闻"
        Else
            rValue = "公告"
        End If
        GetNewsTypeName = rValue
    End Function   
    
    Rem 获取会员等级名称
    Function GetMemberOrderName(memberOrder)
        Dim rInfo
        Select case memberOrder
            case 1
                rInfo = "紫钻会员"
            case 2
                rInfo = "黄钻会员"
            case 3
                rInfo = "蓝钻会员"
            case 4
                rInfo = "红钻会员"
            case else
                rInfo = "普通会员"
        End Select 
        GetMemberOrderName = rInfo
    End Function  
    
    Rem 获取推广提成类型名称
    Public Function GetSpreadGrantType(typeID)
        Dim rValue
        rValue="未知"
        If typeID=1 Then
            rValue = "注册"
        End If
        If typeID=2 Then
            rValue = "游戏时长"
        End If
        If typeID=3 Then
            rValue = "充值"
        End If
        If typeID=4 Then
            rValue = "结算"
        End If
        GetSpreadGrantType = rValue
    End Function
    
    Rem 分页
	Public Sub GoPage(pageCount,page)
	    Dim action,query
	    action = Request.ServerVariables("SCRIPT_NAME")
	    Response.Write("<div class=""page mtop13"">"& vbCrLf)
	    response.Write("<form method=""get"" onsubmit=""document.location = '" & action & "?Page='+ this.page.value;return false;"">"& vbCrLf )
	    If page<=1 Then
	        Response.Write("<a>首页</a>"& vbCrLf)
	        Response.Write("<a>上一页</a>"& vbCrLf)
	    Else
	        Response.Write("<a href=" & action & "?Page=1" & ">首页</a>" & vbCrLf)
			Response.Write("<a href=" & action & "?Page=" & (Page-1) & ">上页</a>" & vbCrLf)
	    End If 
	    If page>=pageCount Then
	        Response.Write("<a>下一页</a>"& vbCrLf)
			Response.Write("<a>尾页</a>"& vbCrLf)
	    Else
	        Response.Write("<a href=" & action & "?Page=" & (Page+1) & ">下一页</a>" & vbCrLf)
			Response.Write("<a href=" & action & "?Page=" & pagecount & ">尾页</a>" & vbCrLf)			
	    End if
	    Response.Write("<div class=""left"">第</div>" & vbCrLf)
	    Response.Write("<div class=""left""><input type=""text"" name=""page"" size=""2"" Maxlength=""5"" value=" & page & " class=""text""></div>" & "<div class=""left"">页</div>"& vbCrLf)
	    Response.Write("<div class=""left""><INPUT type=""submit"" value=""GO""></div>"&  vbCrLf)
		Response.Write("<div class=""left"">共 " & pageCount & " 页</div>" &  vbCrLf)			'pageCount是调用页面中赋值总页数
	    Response.Write("</form></div>")
	End Sub
    
    Rem 首页新闻
    Rem 调用方法 CxGame.NewsTop 5,15
    Public Sub NewsTop(Top,TitleLen)
        Response.Write "<ul>"
        ConnectGame(QPNativeWebDB)
        Dim rs,Title,typeName
        Set rs = GameConn.Execute("select Top "&Top&" NewsID,Subject,ClassID,IssueDate from News(nolock) order by OnTopAll desc,OnTop desc,NewsID desc")
        Do While Not rs.Eof
            Title=rs(1)
            If Len(Title)>TitleLen Then
                Title=Left(Title,TitleLen) & ".."
			End If
			typeName = GetNewsTypeName(rs(2))
			Response.Write "<li><span class=""hui6"">"&Tool.Format_Time(rs(3),2)&"</span>["&typeName&"]&nbsp;<a href=""NewsView.asp?id="&rs(0)&""" class=""lh"" target=""_blank"">"&Title&"</a></li>"
        rs.MoveNext
		Loop
		rs.Close:Set rs=Nothing:CloseGame()        
        CloseGame()
        Response.Write "</ul>"
    End Sub 	
    
    Rem 新闻列表
    Rem 调用方法 CxGame.NewsTop 10
    Public Sub NewsList(pageSize)
        Response.Write "<ul>"
        ConnectGame(QPNativeWebDB)
        Dim rs,Result_n,typeName
        Set Rs=Server.Createobject("Adodb.Recordset")
        rs.Open "select NewsID,Subject,ClassID,IssueDate from News(nolock) order by OnTopAll desc,OnTop desc,NewsID desc",GameConn,1,1
        rs.PageSize = pageSize
        Result_n = rs.RecordCount
        Dim Maxpage,Page,i 
        If Result_n>0 Then
            Maxpage = rs.PageCount
            Page = Request("Page")
            If Not IsNumeric(Page) or Page="" then
				Page=1
			Else
				Page=CInt(Page)
			End if
			If page<1 then
				page=1
			Elseif page>maxpage then
				page=maxpage
			End if	
			rs.AbsolutePage=Page
	    Else
			rs.Close:Set rs=Nothing:CloseGame()        
			Response.Write "对不起,没有任何信息!</ul>"
			Exit Sub
		End If 
        For i=0 To pageSize-1
            typeName = GetNewsTypeName(rs(2))
            Response.Write "<li><span>"&Tool.Format_Time(rs(3),2)&"</span><label></label>["&typeName&"]&nbsp;<a href=""NewsView.asp?id="&rs(0)&""" class=""lh"" target=""_blank"">"&rs(1)&"</a></li>"&vbCrLf
            rs.MoveNext
            If rs.Eof Then Exit For
        Next
        rs.Close:Set rs=Nothing:CloseGame()        
        Response.Write "</ul>"
        GoPage maxpage,page
    End Sub
    
    Rem 首页常见问题
    Rem 调用方法 CxGame.IssueTop 5,15
    Public Sub IssueTop(Top,TitleLen)
        Response.Write "<ul>"
        ConnectGame(QPNativeWebDB)
        Dim rs,Title
        Set rs = GameConn.Execute("select Top "&Top&" IssueID,IssueTitle from GameIssueInfo(nolock) Where Nullity=0")
        Do While Not rs.Eof
            Title=rs(1)
            If Len(Title)>TitleLen Then
                Title=Left(Title,TitleLen) & ".."
			End If
			Response.Write "<li><a href=""ServiceIssueView.asp?id="&rs(0)&""" target=""_blank"" class=""lh"" >"&Title&"</a></li>"
        rs.MoveNext
		Loop
		rs.Close:Set rs=Nothing:CloseGame()        
        CloseGame()
        Response.Write "</ul>"
    End Sub
    
    Rem 常见问题列表
    Rem 调用方法 CxGame.IssueList 10
    Public Sub IssueList(pageSize)
        Response.Write "<ul>"
        ConnectGame(QPNativeWebDB)
        Dim rs,Result_n,typeName
        Set Rs=Server.Createobject("Adodb.Recordset")
        rs.Open "Select IssueID,IssueTitle from GameIssueInfo(nolock) Where Nullity=0",GameConn,1,1
        rs.PageSize = pageSize
        Result_n = rs.RecordCount
        Dim Maxpage,Page,i 
        If Result_n>0 Then
            Maxpage = rs.PageCount
            Page = Request("Page")
            If Not IsNumeric(Page) or Page="" then
				Page=1
			Else
				Page=CInt(Page)
			End if
			If page<1 then
				page=1
			Elseif page>maxpage then
				page=maxpage
			End if	
			rs.AbsolutePage=Page
	    Else
			rs.Close:Set rs=Nothing:CloseGame()        
			Response.Write "对不起,没有任何信息!</ul>"
			Exit Sub
		End If 
        For i=0 To pageSize-1
            Response.Write "<li><label></label><a href=""ServiceIssueView.asp?id="&rs(0)&""" class=""lh"" target=""_blank"">"&rs(1)&"</a></li>"&vbCrLf
            rs.MoveNext
            If rs.Eof Then Exit For
        Next
        rs.Close:Set rs=Nothing:CloseGame()        
        Response.Write "</ul>"
        GoPage maxpage,page
    End Sub
    
    Rem 用户注册
    Public Sub UserReg()
        IF GetInfo(0,"reg")="true" Then
            IF Not CodeIsTrue Then
				MessageBox("错误:验证码错误,请重新输入!")
				Exit Sub
			End IF
			Dim Accounts,Nickname,inFaceID,Gender,LogonPass,InsurePass,Spreader,IP,Compellation,PassPortID
			Accounts = GetInfo(0,"txtAccounts")
			Nickname = GetInfo(0,"txtNickname")
			inFaceID = GetInfo(1,"inFaceID")
			Gender = GetInfo(1,"ddlGender")
			LogonPass = GetInfo(0,"txtLogonPass")
			InsurePass = GetInfo(0,"txtLogonPass")
			Spreader = GetInfo(0,"txtSpreader")
			Compellation = GetInfo(0,"txtCompellation")
			PassPortID = GetInfo(0,"txtPassPortID")
			IP = GetIP()
			LogonPass = md5(LogonPass,32)
			InsurePass = md5(InsurePass,32)
			Dim rs
			ConnectGame(QPAccountsDB)
			set rs = GameConn.Execute("Exec WSP_PW_RegisterAccounts '"&Accounts&"','"&Nickname&"','"&LogonPass&"','"&InsurePass&"',"&inFaceID&","&Gender&",'"&Spreader&"','"&Compellation&"','"&PassPortID&"','"&IP&"'")
			IF rs(0)<>0 Then
			    MessageBox(rs(1))
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
			Else
			    Session("UserID")=rs("UserID")
				Session("GameID")=rs("GameID")
				Session("UserName")=rs("Accounts")
				Session("NikeName") = rs("Nickname")
				Session("FaceID") = rs("FaceID")
				Session("MemberOrder") = rs("MemberOrder")
				Session("Loveliness") = rs("Loveliness")
			    MessageBoxReload "注册成功！","MemberIndex.asp"
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
			End If
        End If
    End Sub
    
    Rem 用户登录
	Public Sub UserLogin()
	    IF GetInfo(0,"login")="true" Then
	        IF Not CodeIsTrue Then
				MessageBox("错误:验证码错误,请重新输入!")
				Exit Sub
			End IF
			Dim Accounts,LogonPass,IP
			Accounts = GetInfo(0,"txtAccounts")
			LogonPass = GetInfo(0,"txtLogonPass")
			LogonPass = md5(LogonPass,32)
			IP = GetIP()
			Dim rs			
			ConnectGame(QPAccountsDB)
			set rs = GameConn.Execute("Exec WSP_PW_EfficacyAccounts '"&Accounts&"','"&LogonPass&"','"&IP&"'")
			IF rs(0)<>0 Then
			    MessageBox(rs(1))
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
			Else
			    Session("UserID")=rs(1)
				Session("GameID")=rs(2)
				Session("UserName")=rs(3)
				Session("NikeName") = rs(4)
				Session("FaceID") = rs(6)
				Session("MemberOrder") = rs(9)
				Session("Loveliness") = rs(12)
				rs.Close:Set rs=Nothing:CloseGame()
				If CxGame.Channel="Index" Then
				    Response.Redirect "index.asp"
				End If
				If CxGame.Channel="Member" Then
				    Response.Redirect "MemberIndex.asp"
				End If 
				If CxGame.Channel="Buy" Then
				    Response.Redirect "Buy.asp?CateID="&Request("CateID")
				End If
				If CxGame.Channel="Present" Then
				    Response.Redirect "Present.asp?CateID="&Request("CateID")
				End If
				
			End If
	    End If	    
	    IF GetInfo(0,"exit")="true" Then
			Session("UserID")=empty
			Session("GameID")=empty
			Session("UserName")=empty
			Session("NikeName")=empty
			Session("FaceID") = empty
			Session("MemberOrder") = empty
			Session("Loveliness") = empty
			Response.Redirect "index.asp"
		End IF
	End Sub
	
	Rem 大厅登录
	Public Function PlazaAutoLogin()
	    PlazaAutoLogin=false
	    Dim pUserid,pAccounts,pPassword

	    '读取大厅写 Cookie 部分	
	    pUserid=Request.Cookies("UserID")	'用户标识
	    pAccounts=Request.Cookies("Accounts")'用户帐号
	    pPassword=Request.Cookies("Password")'用户密码  

	    If pUserid="" AND Session("Refurbish")="" Then
	        Session("Refurbish")="1"
	        Response.write("<script>top.window.location.reload();</script>")	        	    	        
        Elseif pUserid<>"" THEN  
	        ConnectGame(QPAccountsDB)
	        
	        Set Rs=GameConn.Execute("Select UserID,GameID,Accounts,LogonPass,NickName,FaceID,MemberOrder,LoveLiness From AccountsInfo Where UserID="& pUserid &"")
            IF NOT Rs.Eof Then  
                IF Trim(pPassword)=Rs(3) THEN                              
                    Session("UserID")=Rs(0)
			        Session("GameID")=Rs(1)
			        Session("UserName")=Rs(2)
			        Session("NikeName")=Rs(4)
			        Session("FaceID") = Rs(5)
			        Session("MemberOrder") = Rs(6)
			        Session("Loveliness") = Rs(7)			        
				    rs.Close:Set rs=Nothing:CloseGame()				
				    PlazaAutoLogin=true
				 END IF 
            END IF
        END IF	       
	End Function
	
	Rem 检测是否登陆
	Public Function IsLogin()
		IsLogin=False
		IF Not IsEmpty(Session("UserID")) And Session("UserID")<>"" Then
			IsLogin=True
		End IF
	End Function
	
	Rem 检测是否登陆
	Public Function IsCheckLogin()
		IF Not IsLogin Then
			'MessageBoxReload "您还没有登入,请登入后再行操作！","Login.asp"
			Response.Redirect("/Login.asp")
			Response.End()
		End IF
	End Function
  
    Rem 修改昵称
    Public Sub ModifyNikeName()
        If GetInfo(0,"Modify") = "true" Then
            Dim nikeName
            nikeName = GetInfo(0,"txtNikeName")
            ConnectGame(QPAccountsDB)
            GameConn.Execute("Update AccountsInfo Set Nickname='"&nikeName&"' Where UserID = "&Session("UserID"))
            Session("NikeName") = nikeName
            MessageBoxReload "修改昵称成功！","MemberModifyNikename.asp"
            CloseGame()
        End If
    End Sub
    
    Rem 修改头像
    Public Sub ModifyFace()
        If GetInfo(0,"Modify") = "true" Then
            Dim face
            face = GetInfo(1,"inFaceID")
            ConnectGame(QPAccountsDB)
            GameConn.Execute("Update AccountsInfo Set FaceID="&face&" Where UserID = "&Session("UserID"))
            Session("FaceID") = face
            MessageBoxReload "修改头像称成功！","MemberModifyFace.asp"
            CloseGame()
        End If
    End Sub
    
    Rem 修改用户资料
    Public Sub ModifyUserInfo()
        If GetInfo(0,"Modify") = "true" Then
            Dim UserID,Gender,UnderWrite,Compellation,MobilePhone,Address,QQ,EMail,UserNote,SeatPhone
            UserID = Session("UserID")
            Gender = GetInfo(1,"ddlGender")
            UnderWrite = GetInfo(0,"txtUnderWrite")
            Compellation = GetInfo(0,"txtCompellation")
            MobilePhone = GetInfo(0,"txtMobilePhone")
            SeatPhone = GetInfo(0,"txtSeatPhone")
            Address = GetInfo(0,"txtAddress")
            QQ = GetInfo(0,"txtQQ")
            EMail = GetInfo(0,"txtEMail")
            UserNote = GetInfo(0,"txtUserNote")
            ConnectGame(QPAccountsDB)
            Dim rs
            Set rs = GameConn.Execute("Exec WSP_PW_ModifyUserInfo "&UserID&","&Gender&",'"&UnderWrite&"','"&Compellation&"','"&QQ&"','"&EMail&"','"&MobilePhone&"','"&SeatPhone&"','"&Address&"','"&UserNote&"'")
            If rs(0)<>0 Then
                MessageBox(rs(1))
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
		    Else
		        MessageBoxReload rs(1),"MemberModifyUserInfo.asp"
		        rs.Close:Set rs=Nothing:CloseGame()
		    End If
        End If
    End Sub
    
    Rem 修改密码
    Public Sub ModifyPassword(mType)
        If GetInfo(0,"Modify") = "true" Then
            Dim UserID,oldPass,newPass,IP
            UserID = Session("UserID")
            oldPass = GetInfo(0,"txtOldPass")
            newPass = GetInfo(0,"txtNewPass")
            oldPass = md5(oldPass,32)
            newPass = md5(newPass,32)
            IP = GetIP()
            ConnectGame(QPAccountsDB)
            Dim rs
            If mType="LogonPass" Then
                Set rs = GameConn.Execute("WSP_PW_ModifyLogonPass "&UserID&",'"&oldPass&"','"&newPass&"','"&IP&"'")
            Else
                Set rs = GameConn.Execute("WSP_PW_ModifyInsurePass "&UserID&",'"&oldPass&"','"&newPass&"','"&IP&"'")
            End If
            If rs(0)<>0 Then
                MessageBox(rs(1))
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
		    Else
		        If mType="LogonPass" Then
		            MessageBoxReload rs(1),"MemberModifyLogonPass.asp"
		        Else
		            MessageBoxReload rs(1),"MemberModifyInsurePass.asp"
		        End If
		        rs.Close:Set rs=Nothing:CloseGame()
		    End If
        End If
    End Sub
    
    Rem 密码保护
    Public Sub AccountsProtect()
        Dim UserID,Question1,Response1,Question2,Response2,Question3,Response3,PassportID,PassportType,SafeEmail,IP,rs   
        '申请密码保护    
        If GetInfo(0,"Add") = "true" Then
            UserID = Session("UserID")
            Question1 = GetInfo(0,"ddlQuestion1")
            Response1 = GetInfo(0,"txtResponse1")
            Question2 = GetInfo(0,"ddlQuestion2")
            Response2 = GetInfo(0,"txtResponse2")
            Question3 = GetInfo(0,"ddlQuestion3")
            Response3 = GetInfo(0,"txtResponse3")
            PassportID = GetInfo(0,"txtPassportID")
            PassportType = GetInfo(0,"ddlPassportType")
            SafeEmail = GetInfo(0,"txtSafeEmail")
            IP = GetIP()
            ConnectGame(QPAccountsDB)
            Set rs = GameConn.Execute("WSP_PW_AddAccountProtect "&UserID&",'"&Question1&"','"&Response1&"','"&Question2&"','"&Response2&"','"&Question3&"','"&Response3&"','"&PassportID&"',"&PassportType&",'"&SafeEmail&"','"&IP&"'")
            If rs(0)<>0 Then
                MessageBox(rs(1))
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
		    Else
		        MessageBoxReload rs(1),"MemberApplyProtect.asp"
		        rs.Close:Set rs=Nothing:CloseGame()
		    End If
        End If
        '修改密码保护
        If GetInfo(0,"Modify") = "true" Then            
            UserID = Session("UserID")
            Question1 = GetInfo(0,"ddlQuestion1")
            Response1 = GetInfo(0,"txtMResponse1")
            Question2 = GetInfo(0,"ddlQuestion2")
            Response2 = GetInfo(0,"txtMResponse2")
            Question3 = GetInfo(0,"ddlQuestion3")
            Response3 = GetInfo(0,"txtMResponse3")
            SafeEmail = GetInfo(0,"txtSafeEmail")
            IP = GetIP()
            ConnectGame(QPAccountsDB)
            Set rs = GameConn.Execute("WSP_PW_ModifyAccountProtect "&UserID&",'"&Question1&"','"&Response1&"','"&Question2&"','"&Response2&"','"&Question3&"','"&Response3&"','"&SafeEmail&"','"&IP&"'")
            If rs(0)<>0 Then
                MessageBox(rs(1))
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
		    Else
		        MessageBoxReload rs(1),"MemberModifyProtect.asp"
		        rs.Close:Set rs=Nothing:CloseGame()
		    End If
        End If        
    End Sub
    
    Rem 找回密码
    Public Sub ResetPassword()
        Dim UserID,NewPass,Response1,Response2,Response3,IP,rs
        '找回登录密码
        If GetInfo(0,"LogonPass") = "true" Then
            UserID = GetInfo(0,"hdnUserID")
            NewPass = GetInfo(0,"txtNewPass")
            NewPass = md5(NewPass,32)
            Response1 = GetInfo(0,"txtResponse1")
            Response2 = GetInfo(0,"txtResponse2")
            Response3 = GetInfo(0,"txtResponse3")
            IP = GetIP()
            ConnectGame(QPAccountsDB)
            Set rs = GameConn.Execute("WSP_PW_ResetLogonPasswd "&UserID&",'"&NewPass&"','"&Response1&"','"&Response2&"','"&Response3&"','"&IP&"'")
            If rs(0)<>0 Then
                MessageBoxReload rs(1),"MemberReLogonPass.asp"
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
		    Else
		        MessageBoxReload rs(1),"MemberReLogonPass.asp"
		        rs.Close:Set rs=Nothing:CloseGame()
		    End If
        End If
        '找回保险柜密码
        If GetInfo(0,"InsurePass") = "true" Then
            UserID = Session("UserID")
            NewPass = GetInfo(0,"txtNewPass")
            NewPass = md5(NewPass,32)
            Response1 = GetInfo(0,"txtResponse1")
            Response2 = GetInfo(0,"txtResponse2")
            Response3 = GetInfo(0,"txtResponse3")
            IP = GetIP()
            Set rs = GameConn.Execute("WSP_PW_ResetInsurePasswd "&UserID&",'"&NewPass&"','"&Response1&"','"&Response2&"','"&Response3&"','"&IP&"'")
            If rs(0)<>0 Then
                MessageBox(rs(1))
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
		    Else
		        MessageBoxReload rs(1),"MemberReInsurePass.asp"
		        rs.Close:Set rs=Nothing:CloseGame()
		    End If
        End If
    End Sub
    
    Rem 显示保险柜信息
    Public Sub ShowUserInsure()
        ConnectGame(QPTreasureDB)
        Dim rs
        Set rs = GameConn.Execute("Select Score,InsureScore From GameScoreInfo Where UserID="& Session("UserID") &"")
        IF Not Rs.Eof Then
			Session("InsureScore")=rs(1)	
			Session("Score")=rs(0)	
	    Else
	        Session("InsureScore")=0
			Session("Score")=0
		End IF
		rs.Close:Set rs=Nothing:CloseGame()
    End Sub
    
    Rem 保险柜操作
    Public Sub OPInsure()
        Dim rs,UserID,Score,Note,IP,InsurePass
        '存款
        If GetInfo(0,"InsureIn") = "true" Then
            ConnectGame(QPTreasureDB)
            UserID = Session("UserID")
            Score = GetInfo(0,"txtScore")
            Note = GetInfo(0,"txtNode")
            IP = GetIP()
            Set rs = GameConn.Execute("WSP_PW_InsureIn "&UserID&","&Score&","&MinSwapScore&",'"&IP&"','"&Note&"'")
            If rs(0)<>0 Then
                MessageBox(rs(1))
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
		    Else
		        MessageBoxReload rs(1),"MemberInsureIn.asp"
		        rs.Close:Set rs=Nothing:CloseGame()
		        Exit Sub
		    End If
        End If
        '取款
        If GetInfo(0,"InsureOut") = "true" Then
            ConnectGame(QPTreasureDB)
            UserID = Session("UserID")
            Score = GetInfo(0,"txtScore")
            Note = GetInfo(0,"txtNode")
            InsurePass = GetInfo(0,"txtInsurePass")
            InsurePass = md5(InsurePass,32)
            IP = GetIP()
            Set rs = GameConn.Execute("WSP_PW_InsureOut "&UserID&",'"&InsurePass&"',"&Score&","&MinSwapScore&",'"&IP&"','"&Note&"'")
            If rs(0)<>0 Then
                MessageBox(rs(1))
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
		    Else
		        MessageBoxReload rs(1),"MemberInsureOut.asp"
		        rs.Close:Set rs=Nothing:CloseGame()
		        Exit Sub
		    End If
        End If
        '转账
        If GetInfo(0,"InsureTransfer") = "true" Then
            Dim SrcUserID,DstUserID,radType,DstUser
            radType = GetInfo(1,"radType")
            DstUser = GetInfo(0,"txtUser")
            ConnectGame(QPAccountsDB)
            If radType=0 Then
                Set rs = GameConn.Execute("Select Top 1 UserID from AccountsInfo(nolock) where Accounts= '"&DstUser&"'")
            Else
                Set rs = GameConn.Execute("Select Top 1 UserID from AccountsInfo(nolock) where GameID= '"&DstUser&"'")
            End If
            If Not rs.EOF Then
                DstUserID = rs("UserID")
                rs.Close:Set rs=Nothing:CloseGame()
            Else
                MessageBox("抱歉！您要赠送的目标帐号不存在！")
			    rs.Close:Set rs=Nothing:CloseGame()
			    Exit Sub
            End If
            '转账操作
            ConnectGame(QPTreasureDB)
            SrcUserID = Session("UserID")
            Score = GetInfo(0,"txtScore")
            Note = GetInfo(0,"txtNode")
            InsurePass = GetInfo(0,"txtInsurePass")
            InsurePass = md5(InsurePass,32)
            IP = GetIP()
            Set rs = GameConn.Execute("WSP_PW_InsureTransfer "&SrcUserID&","&DstUserID&",'"&InsurePass&"',"&Score&","&MinSwapScore&","&RevenueRate&",'"&IP&"','"&Note&"'")
            If rs(0)<>0 Then
                MessageBox(rs(1))
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
		    Else
		        MessageBoxReload rs(1),"MemberInsureTransfer.asp"
		        rs.Close:Set rs=Nothing:CloseGame()
		        Exit Sub
		    End If
        End If
        ShowUserInsure()
    End Sub
    
    Rem 根据用户ID获取用户名称
    Public Function GetUserNameByUserID(userID)
        Dim rs,sql,userName
        userName=""
        Call ConnectGame(QPAccountsDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT Accounts FROM AccountsInfo(nolock)  WHERE UserID="&userID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            userName=rs("Accounts")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetUserNameByUserID = userName
    End Function 
    
    Rem 根据用户ID获取用户游戏ID
    Public Function GetUserGameIDByUserID(userID)
        Dim rs,sql,rValue
        Call ConnectGame(QPAccountsDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT GameID FROM AccountsInfo(nolock)  WHERE UserID="&userID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            rValue=rs("GameID")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetUserGameIDByUserID = rValue
    End Function 
    
    Rem 获取所有积分游戏列表
    Public Function GetScoreKindList()
        Dim rs,sql,n,j
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql = "SELECT a.KindID,a.KindName,b.DataBaseAddr,b.DataBaseName,a.TypeID FROM GameKindItem a,GameGameItem b WHERE a.GameID = b.GameID ORDER BY SortID"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,4)
        j=0
        do while not rs.eof 
            If rs("DataBaseName")<> "QPTreasureDB" Then
                ArrayList(j,0)=rs("KindID")
                ArrayList(j,1)=rs("KindName")
                ArrayList(j,2)=rs("DataBaseAddr")
                ArrayList(j,3)=rs("DataBaseName")
                ArrayList(j,4)=rs("TypeID")
                j=j+1
            End If
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetScoreKindList = ArrayList
    End Function
    
    Rem 获取游戏数据库名称
    Public Function GetKindDBNameByKindID(kindID)
        Dim rs,sql,rValue
        rValue=""
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql = "SELECT a.KindID,a.KindName,b.DataBaseAddr,b.DataBaseName,a.TypeID FROM GameKindItem a,GameGameItem b WHERE a.GameID = b.GameID AND KindID="&kindID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            rValue=Trim(rs("DataBaseName"))
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetKindDBNameByKindID = rValue
    End Function 
    
    Rem 根据房间标识获取房间名称
    Public Function GetRoomNameByRoomID(serverID)
        Dim rs,sql,serverName
        serverName=""
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT ServerName FROM GameRoomInfo(nolock)  WHERE ServerID="&serverID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            serverName=rs("ServerName")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetRoomNameByRoomID = serverName
    End Function
    
    Rem 实卡充值
    Public Sub CardPay()
        If GetInfo(0,"Pay") = "true" Then
            Dim opUser,accounts,cardNum,cardPwd,IP,rs
            opUser = Session("UserID")
            If IsEmpty(opUser) Then
                opUser=0
            End If
            accounts = GetInfo(0,"txtAccounts")
            cardNum = GetInfo(0,"txtSerialID")
            cardPwd = GetInfo(0,"txtPassword")
            cardPwd = md5(cardPwd,32)
            IP = GetIP()
            ConnectGame(QPTreasureDB)
            Set rs = GameConn.Execute("WEB_PW_FilledLivcard "&opUser&",'"&cardNum&"','"&cardPwd&"','"&accounts&"','"&IP&"'")
            MessageBox(rs(1))
			rs.Close:Set rs=Nothing:CloseGame()
        End If  
    End Sub
    
    Rem 获取会员卡类型名称
    Public Function GetCardTypeName(cardTypeID)
        Dim rs,sql,rValue
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT CardName FROM GlobalLivcard(nolock)  WHERE CardTypeID="&cardTypeID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            rValue=rs("CardName")
        End If 
        rs.close:set rs=nothing:CloseGame()     
        GetCardTypeName = rValue
    End Function
    
    Rem 快钱支付记录
    Public Sub CreateKQReturnInfo(merchantAcctId,version,language,signType,payType,bankId,orderId,orderTime,orderAmount,dealId,bankDealId,dealTime,payAmount,fee,payResult,errCode,signMsg,ext1,ext2) 
        Call ConnectGame(QPTreasureDB)
        GameConn.Execute("WSP_PW_AddReturnKQInfo '"&merchantAcctId&"','"&version&"',"&language&","&signType&",'"&payType&"','"&bankId&"','"&orderId&"','"&orderTime&"',"&orderAmount&",'"&dealId&"','"&bankDealId&"','"&dealTime&"',"&payAmount&","&fee&",'"&payResult&"','"&errCode&"','"&signMsg&"','"&ext1&"','"&ext2&"'")  
        CloseGame()     
    End Sub
    
    Rem 易宝支付记录
    Public Sub CreateYPReturnInfo(p1_MerId,r0_Cmd,r1_Code,r2_TrxId,r3_Amt,r4_Cur,r5_Pid,r6_Order,r7_Uid,r8_MP,r9_BType,rb_BankId,ro_BankOrderId,rp_PayDate,rq_CardNo,ru_Trxtime,hmac)
        Call ConnectGame(QPTreasureDB)
        GameConn.Execute("WSP_PW_AddReturnYPInfo '"&p1_MerId&"','"&r0_Cmd&"','"&r1_Code&"','"&r2_TrxId&"','"&r3_Amt&"','"&r4_Cur&"','"&r5_Pid&"','"&r6_Order&"','"&r7_Uid&"','"&r8_MP&"','"&r9_BType&"','"&rb_BankId&"','"&ro_BankOrderId&"','"&rp_PayDate&"','"&rq_CardNo&"','"&ru_Trxtime&"','"&hmac&"'")
        CloseGame()     
    End Sub
    
    Rem 在线充值
    Public Sub OnlinePay(orderId,IP)
        Call ConnectGame(QPTreasureDB)
        GameConn.Execute("WSP_PW_FilledOnLine '"&orderId&"','"&IP&"'")
        CloseGame()     
    End Sub
    
    Rem 获取充值服务列表
    Function GetShareInfoList()
        Dim rs,sql,n,j
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT ShareID,ShareName FROM GlobalShareInfo(nolock) ORDER BY ShareID"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,1)
        j=0
        do while not rs.eof 
            ArrayList(j,0)=rs("ShareID")
            ArrayList(j,1)=rs("ShareName")            
            j=j+1
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetShareInfoList = ArrayList
    End Function
    
    Rem 获取服务名称
    Function GetShareNameByShareID(shareID)
        Dim rs,sql,rValue
        rValue=""
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT ShareName FROM GlobalShareInfo(nolock)  WHERE ShareID="&shareID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            rValue=rs("ShareName")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetShareNameByShareID = rValue
    End Function
    
    Rem 获取会员卡名称
    Function GetCardNameByCardID(cardTypeID)
        Dim rs,sql,cardName
        cardName=""
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT CardName FROM GlobalLivcard(nolock)  WHERE CardTypeID="&cardTypeID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            cardName=rs("CardName")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetCardNameByCardID = cardName
    End Function 
    
    rem 下载跳转
	Public Sub GameDown()
		IF GetInfo(1,"KindID")=0 Then			
			Response.Redirect("/Download/plaza.exe")			
			exit sub
			Response.End
		end if
		Call ConnectGame(QPPlatformDB)
		Set Rs=GameConn.Execute("Select ProcessName,DownLoadUrl From GameKindItem(nolock) Where KindID="& GetInfo(1,"KindID") &"")
		IF Not Rs.Eof Then
		    If Rs(1)<>"" Then		    
			    Response.Redirect(Rs(1))
			Else
			    Response.Redirect("/Download/"&Rs(0))
			End If
			Response.End
		End IF
		rs.close:set rs=nothing:CloseGame()     
	End Sub
	
	Rem 规则跳转
	Public Sub GameRule()
		Call ConnectGame(QPPlatformDB)
		Set Rs=GameConn.Execute("Select GameRuleUrl From GameKindItem(nolock) Where KindID="& GetInfo(1,"KindID") &"")
		IF Not Rs.Eof Then
		    If Rs(0)<>"" Then		    
			    Response.Redirect(Rs(0))
			Else
			    Response.Redirect("GameRules.asp?KindID="&GetInfo(1,"KindID"))
			End If
			Response.End
		End IF
		rs.close:set rs=nothing:CloseGame()     
	End Sub
	
	Rem 财富排行
	Rem 调用方法 CxGame.GoldTop 10,8
	Public Sub GoldTop(Top,nameLen)
	    Call ConnectGame(QPTreasureDB)
	    Dim rs,name
	    Set rs=GameConn.Execute("select Top "&Top&" UserID,(Score+InsureScore) as GoldCount from GameScoreInfo order by GoldCount desc")
	    Response.Write "<ul>"
	    Do While Not rs.Eof	        
	        name = CxGame.GetUserNameByUserID(rs(0))
	        If Len(name)>nameLen Then
                name=Left(name,nameLen)
			End If
			Response.Write "<li><span class=""hui6"">"&rs(1)&"</span>"&name&"</li>"&vbCrLf
	    rs.MoveNext
		Loop
		rs.Close:Set rs=Nothing:CloseGame()        
	    Response.Write "</ul>"
	End Sub
	
	Rem 魅力点排行
	Rem 调用方法 CxGame.PresentTop 10,8
	Public Sub PresentTop(Top,nameLen)
	    Call ConnectGame(QPAccountsDB)
	    Dim rs,name
	    Set rs=GameConn.Execute("select Top 10 Accounts,Present from AccountsInfo Where Nullity=0 order by Present desc")
	    Response.Write "<ul>"
	    Do While Not rs.Eof	        
	        name = rs(0)
	        If Len(name)>nameLen Then
                name=Left(name,nameLen)
			End If
			Response.Write "<li><span class=""hui6"">"&rs(1)&"</span>"&name&"</li>"&vbCrLf
	    rs.MoveNext
		Loop
		rs.Close:Set rs=Nothing:CloseGame()        
	    Response.Write "</ul>"	    
	End Sub
	
	Rem 比赛报名
    Public Sub GameMatchApply()
        If GetInfo(0,"Apply") = "true" Then
            Dim matchID,accounts,pwd,compellation,gender,passportID,mobilePhone,eMail,qQ,dwellingPlace,postalCode,IP
            matchID = GetInfo(1,"id")
            accounts = GetInfo(0,"txtAccounts")
            pwd = GetInfo(0,"txtPwd")
            pwd = md5(pwd,32)
            compellation = GetInfo(0,"txtCompellation")
            gender = GetInfo(1,"radGender")
            passportID = GetInfo(0,"txtPassportID")
            mobilePhone = GetInfo(0,"txtMobilePhone")
            eMail = GetInfo(0,"txtEMail")
            qQ = GetInfo(0,"txtQQ")
            dwellingPlace = GetInfo(0,"txtDwellingPlace")
            postalCode = GetInfo(0,"txtPostalCode")
            IP = GetIP()
            ConnectGame(QPNativeWebDB)
            Dim rs
            Set rs = GameConn.Execute("WSP_PW_AddGameMatchUser "&matchID&",'"&accounts&"','"&pwd&"','"&compellation&"',"&gender&",'"&passportID&"','"&mobilePhone&"','"&eMail&"','"&qQ&"','"&dwellingPlace&"','"&postalCode&"','"&IP&"'")
            If rs(0)<>0 Then
                MessageBox(rs(1))
			    rs.Close:Set rs=Nothing:CloseGame()
				Exit Sub
		    Else
		        MessageBoxReload rs(1),"MatchIndex.asp"
		        rs.Close:Set rs=Nothing:CloseGame()
		    End If
        End If
    End Sub
    
    Rem 获取推荐人贡献金币量
    Public Function GetChildSpreadScore(userID)
        Dim rs,sql,rValue
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT Sum(Score) FROM RecordSpreadInfo(nolock)  WHERE ChildrenID="&userID
        rs.Open sql,GameConn,1,3
        If IsNull(rs(0)) Then
            rValue = 0
        Else
            rValue = rs(0)
        End If
        rs.close:set rs=nothing:CloseGame()     
        GetChildSpreadScore = rValue
    End Function
    
    Public Function CodeInjection(strQury)
        Dim Query_Badword,Form_Badword,i,Err_Message,name

        '------定义部份  头----------------------------------------------------------------------

        Err_Message = 1  '处理方式：1=提示信息,2=转向页面,3=先提示再转向

        Query_Badword="'‖and‖select‖update‖chr‖delete‖%20from‖;‖insert‖mid‖master.‖set‖chr(37)‖="     

        '在这部份定义get非法参数,使用"‖"号间隔     

        Form_Badword="'‖%‖&‖*‖#"     '在这部份定义post非法参数,使用"‖"号间隔

        'Form_Badword="'‖%‖&‖*‖#‖@"
        '------定义部份  尾-----------------------------------------------------------------------

        On Error Resume Next

        '----- 对 get query 值 的过滤.

        If request.QueryString<>"" then
            Chk_badword=split(Query_Badword,"‖")
            FOR EACH name IN Request.QueryString
                For i=0 to ubound(Chk_badword)
                    If Instr(LCase(request.QueryString(name)),Chk_badword(i))<>0 Then
                        Select Case Err_Message
                          Case "1"
                        Response.Write "<Script Language=JavaScript>alert('传参错误！参数 "&name&" 的值中包含非法字符串！\n\n请不要在参数中出现：and update delete ; insert mid master 等非法字符！');window.close();</Script>"
                          Case "2"
                        Response.Write "<Script Language=JavaScript>location.href='"&Err_Web&"'</Script>"
                          Case "3"
                        Response.Write "<Script Language=JavaScript>alert('传参错误！参数 "&name&"的值中包含非法字符串！\n\n请不要在参数中出现：and update delete ; insert mid master 等非法字符！');location.href='"&Err_Web&"';</Script>"
                        End Select
                        Response.End
                    End If
                NEXT
            NEXT
        End if
    End Function


    
End Class 

Rem 建立类对象
Dim CxGame
Set CxGame=New GameObj  
%>