<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>帐号冻结</title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：帐号冻结</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <% 
        Call ConnectGame(QPAccountsDB)
        Select case lcase(Request("action"))
            case "save"
            Call Save()
            case else
            Call Main()
        End Select
        Call CloseGame()
        
        Sub Save()
            Dim userID,nullityOverDate,nullityNotice
            userID=Request("in_UserID")
            If Request("type")<>"" Then
                Select case Request("type")
                    case "day"
                        nullityOverDate= Split(DateAdd("D",1,Now())," ")(0)
                    case "week"
                        nullityOverDate=Split(DateAdd("D",1,Now())," ")(0)
                    case "month"
                        nullityOverDate=Split(DateAdd("m",1,Now())," ")(0)
                    case "forever"
                        nullityOverDate=Split(DateAdd("m",1200,Now())," ")(0)
                End Select   
            Else
                If Request("dongjieTime")<>"" Then
                    nullityOverDate=DateAdd("n",Request("dongjieTime"),Now())
                Else
                    nullityOverDate=Split(DateAdd("m",1200,Now())," ")(0)
                End If
            End If
            userID=Request("in_UserID")
            nullityNotice=Request("in_NullityNotice")
            GameConn.execute "Update AccountsInfo Set NullityOverDate='"&nullityOverDate&"',Nullity=1 where UserID="&userID
            CxGame.MessageBoxClose("冻结成功！")
        End Sub
        
        Sub Main()
    %>
    <script type="text/javascript">
        function QueryInfo(opType)
        { 
            var dongjieTime = document.form1.dongjieTime.value.trim();
            if(opType=="1")
            {
                document.form1.action = "?action=save&type=day";
            }
            if(opType=="2")
            {
                document.form1.action = "?action=save&type=week";
            }
            if(opType=="3")
            {
                document.form1.action = "?action=save&type=month";
            }
            if(opType=="4")
            {
                document.form1.action = "?action=save&type=forever";
            }
            if(dongjieTime!="")
            {
                if(IsPositiveInt(dongjieTime)==false)
                {
                    alert("冻结时间非数字字符或者超过允许大小！")
                    return false;
                }
            }
            document.form1.submit();
        }
        function CheckFormInfo()
        {
            var dongjieTime = document.form1.dongjieTime.value.trim();
            if(opType=="1")
            {
                document.form1.action = "?action=save&type=day";
            }
            if(opType=="2")
            {
                document.form1.action = "?action=save&type=week";
            }
            if(opType=="3")
            {
                document.form1.action = "?action=save&type=month";
            }
            if(opType=="4")
            {
                document.form1.action = "?action=save&type=forever";
            }
            if(IsPositiveInt(dongjieTime)==false)
            {
                alert("冻结时间非数字字符或者超过允许大小！")
                return false;
            }
           
        }
        
    </script>
    <form name="form1" method="post" action='?action=save' onsubmit="return CheckFormInfo()">
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.opener.refresh(); window.focus();window.opener=null;window.close()" />
                    <input type="submit" value="保存" class="btn wd1"/>              
                    <input type=hidden value="<%=request("UserID") %>" name="in_UserID" />
                </td>
            </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
	            <td class="listTdLeft">冻结会员时间：</td>
	            <td>
	                <input type="button" value="一天" class="btn wd1" onclick="QueryInfo('1')" />
                    <input type="button" value="一周" class="btn wd1" onclick="QueryInfo('2')"/>
                    <input type="button" value="一个月" class="btn wd6" onclick="QueryInfo('3')"/>
                    <input type="button" value="终生" class="btn wd1" onclick="QueryInfo('4')"/>
	            </td>
	        </tr>
             <tr>
	            <td class="listTdLeft">冻结会员时间：</td>
	            <td>
	                <input type="text" name="dongjieTime" class="text wd4" />&nbsp;单位（分钟）      
	            </td>
	        </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.opener.refresh(); window.focus();window.opener=null;window.close()" /> 
                    <input type="submit" value="保存" class="btn wd1"/>            
                </td>
            </tr>
        </table>
    </form>
    <% 
        End Sub
    %>
</body>
</html>
