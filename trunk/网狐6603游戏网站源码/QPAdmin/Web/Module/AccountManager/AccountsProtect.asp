<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title><%=GetQPAdminSiteName() %></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：用户密码保护修改</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <% 
        Call ConnectGame(QPAccountsDB)
        Select case lcase(Request("action"))
            case "mod"
            Call Update()
            case "del"
            Call Delete()
            case else
            Call Main()
        End Select
        Call CloseGame()
        
        '修改
        Sub Update()
            Dim rs,sql
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from AccountsProtect where ProtectID="&Request("ProtectID")
            rs.Open sql,GameConn,1,3
            rs("Question1") = CxGame.GetInfo(0,"in_Question1")
            rs("Response1") = CxGame.GetInfo(0,"in_Response1")
            rs("Question2") = CxGame.GetInfo(0,"in_Question2")
            rs("Response2") = CxGame.GetInfo(0,"in_Response2")
            rs("Question3") = CxGame.GetInfo(0,"in_Question3")
            rs("Response3") = CxGame.GetInfo(0,"in_Response3")
            rs("SafeEmail") = CxGame.GetInfo(0,"in_SafeEmail")
            rs("ModifyIP")=Request.ServerVariables("REMOTE_ADDR")
            rs("ModifyDate")=Now()
            rs.update
            Response.Write "<script>window.close()</script>"
            rs.close()
            Set rs = Nothing  
        End Sub
        
        '删除
        Sub Delete()
            Dim sql
            sql = "delete AccountsProtect where ProtectID="&Request("ProtectID")
            sql = sql&";update AccountsInfo set ProtectID=0 where UserID="&CxGame.GetInfo(0,"in_UserID")
            GameConn.execute sql
            Response.Write "<script>opener.document.location.href='AccountsInfo.asp?id="&CxGame.GetInfo(0,"in_UserID")&"';window.close();</script>" 
        End Sub
        
        '证件类型
        Function GetPassportType(passType)
            Dim rValue
            Select case passType
                case 0
                    rValue = "没有设置"
                case 1
                    rValue = "身份证"
                case 2
                    rValue = "护照"
                case 3
                    rValue = "军官证"
                case 4
                    rValue = "驾驶执照"
                case else
                    rValue = "其他"
            End Select
            GetPassportType = rValue
        End Function
        
        Sub Main()
            Dim rs,sql
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from AccountsProtect where ProtectID="&Request("ProtectID")
            rs.Open sql,GameConn,1,3 
    %>
    <script type="text/javascript">
        function Operate(opType)
        {
            var opVal = document.form1.in_optype
            if(opType=="mod")
            {
                opVal.value="mod";
                if(document.form1.in_SafeEmail.value.trim()=="")
                {
                    alert("安全邮箱不能为空！");
                    return
                }
                if(document.form1.in_Response1.value.trim()=="")
                {
                    alert("问题答案一不能为空！");
                    return
                }
                if(document.form1.in_Response2.value.trim()=="")
                {
                    alert("问题答案二不能为空！");
                    return
                }
                if(document.form1.in_Response3.value.trim()=="")
                {
                    alert("问题答案三不能为空！");
                    return
                }
            }
            if(opType=="del")
            {
                opVal.value="del";                
                if(!confirm("确定要取消密码保护吗？"))
                {
                    return;
                }
            }
            document.form1.action = "?action="+opVal.value+"&ProtectID="+<%=Request("ProtectID") %>;
            document.form1.submit();
        }
    </script>
    <form name="form1" method="post" action="">
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close();" />
                    <input type="button" value="修改密保" class="btn wd2" onclick="Operate('mod')" />
                    <input type="button" value="取消密保" class="btn wd2" onclick="Operate('del')" />                    
                    <input name="in_optype" type="hidden" />
                </td>
            </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
	            <td class="listTdLeft">用户帐号：</td>
	            <td>
	                <span style="color:blue"><%=CxGame.GetUserNameByUserID(rs("UserID")) %></span>
                    <input name="in_UserID" type="hidden" value="<%=rs("UserID") %>" />
	            </td>
	            <td class="listTdLeft">安全邮箱：</td>
	            <td>
	                <input name="in_SafeEmail" type="text" class="text" value="<%=rs("SafeEmail") %>" />
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">问题一：</td>
	            <td>
	                <select name="in_Question1">
                        <option value="您的出生地是？"<% If rs("Question1")="您的出生地是？" Then %> selected="selected" <% End If %>>您的出生地是？</option>
                        <option value="您的小学校名是？"<% If rs("Question1")="您的小学校名是？" Then %> selected="selected" <% End If %>>您的小学校名是？</option>
                        <option value="您的小学班主任是？"<% If rs("Question1")="您的小学班主任是？" Then %> selected="selected" <% End If %>>您的小学班主任是？</option>
                        <option value="您的初中班主任是？"<% If rs("Question1")="您的初中班主任是？" Then %> selected="selected" <% End If %>>您的初中班主任是？</option>
                        <option value="您的高中班主任是？"<% If rs("Question1")="您的高中班主任是？" Then %> selected="selected" <% End If %>>您的高中班主任是？</option>
                        <option value="您的学号（或工号）是？"<% If rs("Question1")="您的学号（或工号）是？" Then %> selected="selected" <% End If %>>您的学号（或工号）是？</option>
                        <option value="您父亲的生日是？"<% If rs("Question1")="您父亲的生日是？" Then %> selected="selected" <% End If %>>您父亲的生日是？</option>
                        <option value="您父亲的姓名是？"<% If rs("Question1")="您父亲的姓名是？" Then %> selected="selected" <% End If %>>您父亲的姓名是？</option>
                        <option value="您母亲的生日是？"<% If rs("Question1")="您母亲的生日是？" Then %> selected="selected" <% End If %>>您母亲的生日是？</option>
                        <option value="您母亲的姓名是？"<% If rs("Question1")="您母亲的姓名是？" Then %> selected="selected" <% End If %>>您母亲的姓名是？</option>
                        <option value="您配偶的名字是？"<% If rs("Question1")="您配偶的名字是？" Then %> selected="selected" <% End If %>>您配偶的名字是？</option>
                    </select>
	            </td>
	            <td class="listTdLeft">答案一：</td>
	            <td>
	                <input name="in_Response1" type="text" class="text" value="<%=rs("Response1") %>" />
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">问题二：</td>
	            <td>
	                <select name="in_Question2">
                        <option value="您的出生地是？"<% If rs("Question2")="您的出生地是？" Then %> selected="selected" <% End If %>>您的出生地是？</option>
                        <option value="您的小学校名是？"<% If rs("Question2")="您的小学校名是？" Then %> selected="selected" <% End If %>>您的小学校名是？</option>
                        <option value="您的小学班主任是？"<% If rs("Question2")="您的小学班主任是？" Then %> selected="selected" <% End If %>>您的小学班主任是？</option>
                        <option value="您的初中班主任是？"<% If rs("Question2")="您的初中班主任是？" Then %> selected="selected" <% End If %>>您的初中班主任是？</option>
                        <option value="您的高中班主任是？"<% If rs("Question2")="您的高中班主任是？" Then %> selected="selected" <% End If %>>您的高中班主任是？</option>
                        <option value="您的学号（或工号）是？"<% If rs("Question2")="您的学号（或工号）是？" Then %> selected="selected" <% End If %>>您的学号（或工号）是？</option>
                        <option value="您父亲的生日是？"<% If rs("Question2")="您父亲的生日是？" Then %> selected="selected" <% End If %>>您父亲的生日是？</option>
                        <option value="您父亲的姓名是？"<% If rs("Question2")="您父亲的姓名是？" Then %> selected="selected" <% End If %>>您父亲的姓名是？</option>
                        <option value="您母亲的生日是？"<% If rs("Question2")="您母亲的生日是？" Then %> selected="selected" <% End If %>>您母亲的生日是？</option>
                        <option value="您母亲的姓名是？"<% If rs("Question2")="您母亲的姓名是？" Then %> selected="selected" <% End If %>>您母亲的姓名是？</option>
                        <option value="您配偶的名字是？"<% If rs("Question2")="您配偶的名字是？" Then %> selected="selected" <% End If %>>您配偶的名字是？</option>
                    </select>
	            </td>
	            <td class="listTdLeft">答案二：</td>
	            <td>
	                <input name="in_Response2" type="text" class="text" value="<%=rs("Response2") %>" />
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">问题三：</td>
	            <td>
	                <select name="in_Question3">
                        <option value="您的出生地是？"<% If rs("Question3")="您的出生地是？" Then %> selected="selected" <% End If %>>您的出生地是？</option>
                        <option value="您的小学校名是？"<% If rs("Question3")="您的小学校名是？" Then %> selected="selected" <% End If %>>您的小学校名是？</option>
                        <option value="您的小学班主任是？"<% If rs("Question3")="您的小学班主任是？" Then %> selected="selected" <% End If %>>您的小学班主任是？</option>
                        <option value="您的初中班主任是？"<% If rs("Question3")="您的初中班主任是？" Then %> selected="selected" <% End If %>>您的初中班主任是？</option>
                        <option value="您的高中班主任是？"<% If rs("Question3")="您的高中班主任是？" Then %> selected="selected" <% End If %>>您的高中班主任是？</option>
                        <option value="您的学号（或工号）是？"<% If rs("Question3")="您的学号（或工号）是？" Then %> selected="selected" <% End If %>>您的学号（或工号）是？</option>
                        <option value="您父亲的生日是？"<% If rs("Question3")="您父亲的生日是？" Then %> selected="selected" <% End If %>>您父亲的生日是？</option>
                        <option value="您父亲的姓名是？"<% If rs("Question3")="您父亲的姓名是？" Then %> selected="selected" <% End If %>>您父亲的姓名是？</option>
                        <option value="您母亲的生日是？"<% If rs("Question3")="您母亲的生日是？" Then %> selected="selected" <% End If %>>您母亲的生日是？</option>
                        <option value="您母亲的姓名是？"<% If rs("Question3")="您母亲的姓名是？" Then %> selected="selected" <% End If %>>您母亲的姓名是？</option>
                        <option value="您配偶的名字是？"<% If rs("Question3")="您配偶的名字是？" Then %> selected="selected" <% End If %>>您配偶的名字是？</option>
                    </select>
	            </td>
	            <td class="listTdLeft">答案三：</td>
	            <td >
	                <input name="in_Response3" type="text" class="text" value="<%=rs("Response3") %>" />
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">证件类型：</td>
	            <td>
	                <span><%=GetPassportType(rs("PassportType")) %></span>
	            </td>
	            <td class="listTdLeft">证件号码：</td>
	            <td>
	                <span><%=rs("PassportID") %></span>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">创建地址：</td>
	            <td>
	                <span><%=rs("CreateIP") %></span>
	            </td>
	            <td class="listTdLeft">创建日期：</td>
	            <td>
	                <span><%=rs("CreateDate") %></span>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">最后修改地址：</td>
	            <td>
	                <span><%=rs("ModifyIP") %></span>
	            </td>
	            <td class="listTdLeft">最后修改日期：</td>
	            <td>
	                <span><%=rs("ModifyDate") %></span>
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
