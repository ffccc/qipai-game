<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->
<!--#include file="../../md5.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>“<%=CxGame.GetUserNameByUserID(Request("id")) %>”帐号信息</title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Calendar.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：用户信息</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
        <tr>
            <td height="28">
                <ul>
                    <li class="tab1">基本信息</li>
	                <li class="tab2" onclick="openWindowSelf('AccountsDetailInfo.asp?id=<%=Request("id") %>')">详细信息</li>
	                <li class="tab2" onclick="openWindowSelf('AccountsGoldInfo.asp?id=<%=Request("id") %>')">财富信息</li>
	                <li class="tab2" onclick="openWindowSelf('AccountsScoreInfo.asp?id=<%=Request("id") %>')">积分信息</li>
	                <li class="tab2" onclick="openWindowSelf('AccountsRecordInfo.asp?id=<%=Request("id") %>')">记录信息</li>
                </ul>
            </td>
        </tr>
    </table>    
    <% 
        Call ConnectGame(QPAccountsDB)
        Select case lcase(Request("action"))
            case "save"
            Call InsertRecord()
            Call SaveUserInfo()
            case else
            Call Main()
        End Select
        Call CloseGame()
        
        '新增记录信息
        Sub InsertRecord()
            Dim masterID,clientIP
            masterID = session("AdminID")
            clientIP = Request.ServerVariables("REMOTE_ADDR")
            
            '新增帐号修改记录
            If CxGame.GetInfo(0,"in_Accounts") <> CxGame.GetInfo(0,"hidAccounts") Then
                Dim sql_rUser
                sql_rUser = "insert into QPRecordDBLink.QPRecordDB.dbo.RecordAccountsExpend(OperMasterID,UserID,ReAccounts,ClientIP) values("
                sql_rUser = sql_rUser&masterID&","&Request("id")&",'"&CxGame.GetInfo(0,"in_Accounts")&"','"&clientIP&"')"
                GameConn.execute(sql_rUser)
            End If
            
            '新增改密码记录
            If CxGame.GetInfo(0,"in_LogonPass")<>"" Or CxGame.GetInfo(0,"in_InsurePass")<>"" Then
                Dim Sql_Passwd,newLogonPwd,newInsurePwd
                If CxGame.GetInfo(0,"in_LogonPass") <> "" Then
                    newLogonPwd = md5(CxGame.GetInfo(0,"in_LogonPass"),32)
                End If
                If CxGame.GetInfo(0,"in_InsurePass")<>"" Then
                    newInsurePwd = md5(CxGame.GetInfo(0,"in_InsurePass"),32)
                End If 
                Sql_Passwd = "Insert into QPRecordDBLink.QPRecordDB.dbo.RecordPasswdExpend(OperMasterID,UserID,ReLogonPasswd,ReInsurePasswd,ClientIP) values("
                Sql_Passwd = Sql_Passwd&masterID&","&Request("id")&",'"&newLogonPwd&"','"&newInsurePwd&"','"&clientIP&"')"
                GameConn.execute(Sql_Passwd)
            End If
        End Sub 
        
        '保存
        Sub SaveUserInfo()
            If CxGame.GetInfo(0,"in_NullityOverDate") <> "" Then
                If CDate(CxGame.GetInfo(0,"in_NullityOverDate"))<CDate(Now()) Then
                    Call CxGame.MessageBoxReload("冻结时间设置错误，修改失败！","?id="&Request("id"))
                End If
            End If
            Dim rs1,sql1,rs2,sql2  
            Set rs1=Server.CreateObject("Adodb.RecordSet")
            Set rs2=Server.CreateObject("Adodb.RecordSet")
            sql1 = "select * from AccountsInfo where  UserID<>'"&Request("id")&"' AND Accounts='"&CxGame.GetInfo(0,"in_Accounts")&"'"  
            sql2 = "select * from AccountsInfo where UserID<>'"&Request("id")&"' AND NickName='"&CxGame.GetInfo(0,"in_Nickname")&"'"  
            rs1.Open sql1,GameConn,1,3
            rs2.Open sql2,GameConn,1,3
            If rs1.eof and rs2.eof Then
                Dim UserRightArr,UserRight,MasterRightArr,MasterRight
                Dim rs,sql,RL_i
                Set rs=Server.CreateObject("Adodb.RecordSet")
                sql = "select * from AccountsInfo where UserID='"&Request("id")&"'"  
                rs.Open sql,GameConn,1,3
                '基本资料
                rs("Accounts") = CxGame.GetInfo(0,"in_Accounts")    
                rs("Nickname") = CxGame.GetInfo(0,"in_Nickname")
                rs("UnderWrite") = CxGame.GetInfo(0,"in_UnderWrite")  
                rs("FaceID") = CxGame.GetInfo(1,"in_FaceID")  
                rs("Experience") = CxGame.GetInfo(1,"in_Experience") 
                rs("Present") = CxGame.GetInfo(1,"in_Present")
                rs("LoveLiness") = CxGame.GetInfo(1,"in_LoveLiness") 
                rs("Gender") = CxGame.GetInfo(1,"in_Gender") 
                '帐号安全
                If CxGame.GetInfo(0,"in_LogonPass")<>"" Then
                    rs("LogonPass") = md5(CxGame.GetInfo(0,"in_LogonPass"),32)
                End If
                If CxGame.GetInfo(0,"in_InsurePass")<>"" Then
                    rs("InsurePass") = md5(CxGame.GetInfo(0,"in_InsurePass"),32)
                End If 
                rs("StunDown") = CxGame.GetInfo(1,"in_StunDown") 
                rs("MoorMachine") = CxGame.GetInfo(1,"in_MoorMachine") 
                rs("Nullity") = CxGame.GetInfo(1,"in_Nullity") 
                If CxGame.GetInfo(0,"in_NullityOverDate") <> "" Then
                    rs("NullityOverDate") = CxGame.GetInfo(0,"in_NullityOverDate")
                End If
                '权限分配
                UserRightArr = Split(Request("in_UserRight"),",")
                UserRight=0
                For RL_i=0 To UBound(UserRightArr)
                    UserRight = UserRight Or UserRightArr(RL_i)
                Next
                rs("UserRight") = UserRight
                MasterRightArr = Split(Request("in_MasterRight"),",")
                MasterRight = 0
                For RL_i=0 To UBound(MasterRightArr)
                    MasterRight = MasterRight Or MasterRightArr(RL_i)
                Next
                rs("MasterRight") = MasterRight
                rs("MasterOrder")= CxGame.GetInfo(1,"in_MasterOrder")
                rs.update
                rs.close()
                Set rs = Nothing   
                Call CxGame.MessageBoxReload("修改成功！","?id="&Request("id")) 
            Else 
                 Call CxGame.MessageBoxReload("唯一索引重复，修改失败！","?id="&Request("id"))      
            End If    
        End Sub
        
        Sub Main()
            Dim rs,sql
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from AccountsInfo(nolock) where UserID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3     
    %>
    <script type="text/javascript">
        function CheckForm()
        {
            var re = /^(-?[1-9][0-9]*|0)$/; 
            var userName = document.form1.in_Accounts.value;
            var experience = document.form1.in_Experience.value;
            var loveLiness = document.form1.in_LoveLiness.value;
            var nickname = document.form1.in_Nickname.value;
            var underWrite = document.form1.in_UnderWrite.value;
            if(userName.trim()=="")
            {
                alert("用户名不能为空！");
                document.form1.in_Accounts.focus();
                return false;
            }
            else if(len(userName.trim())>31)
            {
                alert("用户名字符长度不可超过31个字符！");
                document.form1.in_Accounts.focus();
                return false;
            }
            if(len(nickname.trim())>31)
            {
                alert("用户昵称字符长度不可超过31个字符！");
                document.form1.in_Nickname.focus();
                return false;
            }
            if(len(underWrite.trim())>63)
            {
                alert("个性签名字符长度不可超过63个字符！");
                document.form1.in_UnderWrite.focus();
                return false;
            }
            if (!re.test(experience))
            {
                alert("经验值必须为正整数！");
                document.form1.in_Experience.focus();
                return false;
            }
            else
            {
                if(experience<0)
                {
                    alert("经验值必须为正整数！");
                    document.form1.in_Experience.focus();
                    return false;
                }
            }
            if (!re.test(loveLiness))
            {
                alert("魅力值必须为整数！");
                document.form1.in_LoveLiness.focus();
                return false;
            }

        }
    </script>
    <form name="form1" method="post" action='?action=save&id=<%=Request("id") %>' onsubmit="return CheckForm()">
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />                
                    <input type="submit" value="保存" class="btn wd1" />   
                </td>
            </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">基本信息</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">ID号码(GameID)：</td>
                <td>
                    <span class="Rpd20 lan bold"><%=rs("GameID") %></span>
                    原用户名：<span class="Rpd20 lan bold"><%=rs("RegAccounts") %></span>
                </td>
            </tr>
            <tr>
	            <td class="listTdLeft"> 用户名：</td>
	            <td>
	                <input name="hidAccounts" type="hidden" value="<%=rs("Accounts") %>"/>
	                <input name="in_Accounts" type="text" class="text wd4" value="<%=rs("Accounts") %>"/>&nbsp;用户名字符长度不可超过31个字符          
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">用户昵称：</td>
	            <td>
	                <input name="in_Nickname" type="text" class="text wd4" value="<%=rs("Nickname") %>"/>&nbsp;用户昵称字符长度不可超过31个字符              
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">登录密码：</td>
	            <td>
	                <input name="in_LogonPass" type="password" class="text wd4" />&nbsp;留空不修改                 
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">银行密码：</td>
	            <td>
	                <input name="in_InsurePass" type="password" class="text wd4" />&nbsp;留空不修改                                
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">性别：</td>
	            <td>
	                <input name="in_Gender" id="in_Gender0" type="radio" value="0"<% If rs("Gender")=0 Then %> checked<% End If %> /><label for="in_Gender0"><img src="/Images/girl.gif" alt="女性" />MM</label>
                    <input name="in_Gender" id="in_Gender1" type="radio" value="1"<% If rs("Gender")=1 Then %> checked<% End If %> /><label for="in_Gender1"><img src="/Images/boy.gif" alt="男性" />GG</label>                                                                      
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">密码保护：</td>
	            <td>
	                <% If rs("ProtectID")=0 Then%>
                    <span style="font-weight:bold;">未申请</span>
                    <% Else%>
                    <span style="font-weight:bold;">已申请</span>&nbsp;<a href="javascript:openWindow('AccountsProtect.asp?ProtectID=<%=rs("ProtectID") %>',550,320)" class="l1">点击查看</a>
                    <% End If %>                
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">头像：</td>
	            <td>
	                <img width="32" height="32" name="in_FaceID" style="border: 1px solid #6e94b6;" src="/gamepic/face<%= rs("FaceID") %>.gif" />
                    <select name="in_FaceID" onChange="document.images.in_FaceID.src='/gamepic/face'+this.value+'.gif';" style="vertical-align:bottom;">
                        <% Dim RL_i  
                           For RL_i=0 To 247
                        %>    
                        <option value="<%=RL_i%>" <% If RL_i = rs("FaceID") Then %> selected="selected" <% End If %>>头像<%= RL_i+1 %></option>
                        <% Next %>
                    </select>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">个性签名：</td>
	            <td>
	                <input name="in_UnderWrite" type="text" class="text wd4" style="width:500px" value="<%=rs("UnderWrite") %>"/>&nbsp;个性签名字符长度不可超过63个字符    
	            </td>
	        </tr>	        
	        <tr>
	            <td class="listTdLeft">帐号状态：</td>
	            <td>
	                <input name="in_Nullity" disabled="disabled" id="in_Nullity0" type="radio" value="0"<% If rs("Nullity")=0 Then %> checked="checked"<% End If %> onclick="ChangeOverDate(this)" /><label for="in_Nullity0">正常</label>
                    <input name="in_Nullity" disabled="disabled" id="in_Nullity1" type="radio" value="1"<% If rs("Nullity")=1 Then %> checked="checked"<% End If %> onclick="ChangeOverDate(this)" /><label for="in_Nullity1">冻结</label> 
                    <input name="in_NullityOverDate" disabled="disabled"  value="<%=rs("NullityOverDate") %>"  type="text" style="width:110px;" class="text" <% If rs("Nullity")=0 Then %> style="display:none" <% End If %> /><img name="imgDate" src="../../Images/btn_calendar.gif" onclick="calendar(in_NullityOverDate)" style="cursor:pointer;vertical-align:middle" <% If rs("Nullity")=0 Then %> style="display:none" <% End If %>/>  
                    
                    <script type="text/javascript">
                        function ChangeOverDate(obj)
                        {
                            if(obj.value==0)
                            {
                                document.form1.in_NullityOverDate.style.display="none";
                                document.form1.in_NullityOverDate.value="";
                                document.form1.imgDate.style.display="none"
                            }
                            else
                            {
                                document.form1.in_NullityOverDate.style.display="";
                                document.form1.imgDate.style.display=""
                            }
                        }
                    </script>                 
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">安全关闭：</td>
	            <td>
	                <input name="in_StunDown" id="in_StunDown1" type="radio" value="0"<% If rs("StunDown")=0 Then %> checked="checked"<% End If %> /><label for="in_StunDown1">正常</label>
                    <input name="in_StunDown" id="in_StunDown2" type="radio" value="1"<% If rs("StunDown")=1 Then %> checked="checked"<% End If %> /><label for="in_StunDown2">关闭</label>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">固定机器：</td>
	            <td>
	                <input name="in_MoorMachine" id="in_MoorMachine1" type="radio" value="0"<% If rs("MoorMachine")=0 Then %> checked="checked"<% End If %> /><label for="in_MoorMachine1">未启用</label>
                    <input name="in_MoorMachine" id="in_MoorMachine2" type="radio" value="1"<% If rs("MoorMachine")=1 Then %> checked="checked"<% End If %> /><label for="in_MoorMachine2">启用</label>
                    <input name="in_MoorMachine" id="in_MoorMachine3" type="radio" value="0"<% If rs("MoorMachine")=2 Then %> checked="checked"<% End If %> /><label for="in_MoorMachine3">等待绑定中</label>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">经验值：</td>
	            <td>
	                <input name="in_Experience" type="text" class="text wd4" value="<%=rs("Experience") %>"/>      
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">魅力值：</td>
	            <td>
	                <input name="in_LoveLiness" type="text" class="text wd4" value="<%=rs("LoveLiness") %>"/>                   
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">会员级别：</td>
	            <td>
	                <%=CxGame.GetMemberOrderName(rs("MemberOrder")) %>                   
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">会员到期时间：</td>
	            <td>
	                <%=rs("MemberOverDate") %>               
	            </td>
	        </tr>
	        <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">登录信息</div></td>
            </tr>
            <tr>
	            <td class="listTdLeft">网站登录次数：</td>
	            <td>
	                <%=rs("WebLogonTimes") %> 次
                    <span style="padding-left:100px;">大厅登录次数：</span>
                    <span style="padding-left:10px;"><%=rs("GameLogonTimes") %> 次</span>            
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">登录时间：</td>
	            <td>
	                <%=rs("LastLogonDate") %>&nbsp;&nbsp;<%=Common.FormatDateSpan(rs("LastLogonDate")) %>                               
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">登录地址：</td>
	            <td>
	                <%=rs("LastLogonIP") %>&nbsp;&nbsp;<%=GetCityFromIP(rs("LastLogonIP")) %>                         
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">登录机器：</td>
	            <td>
	                <%=rs("LastLogonMachine") %>                         
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">注册时间：</td>
	            <td>
	                <%=rs("RegisterDate") %>&nbsp;&nbsp;<%=Common.FormatDateSpan(rs("RegisterDate")) %>             
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">注册地址：</td>
	            <td>
	                <%=rs("RegisterIP") %>&nbsp;&nbsp;<%=GetCityFromIP(rs("RegisterIP")) %>        
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">注册机器：</td>
	            <td>
	                <%=rs("RegisterMachine") %>                 
	            </td>
	        </tr>	  
	        <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">权限信息</div></td>
            </tr>
		    <!--#include file="../../Inc/userpowerT.asp"-->	    
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />                
                    <input type="submit" value="保存" class="btn wd1" />   
                </td>
            </tr>
        </table>    
    </form>
    <% 
        rs.Close()
        Set rs=nothing
        End Sub
    %>
</body>
</html>
