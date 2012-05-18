<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>赠送会员</title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：赠送会员</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <% 
        Call ConnectGame(QPRecordDB)
        Select case lcase(Request("action"))
            case "save"
            Call Save()
            case else
            Call Main()
        End Select
        Call CloseGame()
        
        Sub Save()
            Dim UserArr,i,cardType,reason,masterID,clientIP,memberDays
            cardType = CxGame.GetInfo(1,"cardType")
            reason = CxGame.GetInfo(0,"in_Reason")
            masterID = session("AdminID")
            clientIP = Request.ServerVariables("REMOTE_ADDR")
            memberDays= CxGame.GetInfo(0,"memberDays")
            UserArr = Split(Request("userlist"),",")
            For i=0 To UBound(UserArr)
                GameConn.execute("WSP_PM_GrantMember "&masterID&",'"&clientIP&"',"&UserArr(i)&","&cardType&",'"&reason&"',"&memberDays)
            Next
            CxGame.MessageBoxClose("赠送会员成功！")
        End Sub
        
        Sub Main()
    %>
    <script type="text/javascript">
        function CheckForm()
        {
            var reason = document.form1.in_Reason.value.trim();
            var memberDays = document.form1.MemberDays.value.trim()
            var re = /^[1-9]+[0-9]*]*$/;
            if(!re.test(memberDays))
            {
                alert("赠送天数必须为正整数！");
                return false;
            }
            if(reason == "")
            {
                alert("赠送原因不能为空！");
                return false;
            }
            
        }
    </script>
    <form name="form1" method="post" action='?action=save&userlist=<%=request("UserID") %>' onsubmit="return CheckForm()">
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />                
                    <input type="submit" value="保存" class="btn wd1" />&nbsp;
                </td>
            </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
	            <td class="listTdLeft">赠送会员类别：</td>
	            <td>
	                <select name="cardType" style="width:100px;"> 
                    <option value="1">蓝钻</option>
                    <option value="2">黄钻</option>
                    <option value="3">白钻</option>
                    <option value="4">红钻</option>
                    </select> 
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">赠送天数：</td>
	            <td>
	                <input type="text" name="MemberDays" class="text" style="width:94px;" />
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">赠送原因：</td>
	            <td>
	                <textarea name="in_Reason" cols="20" rows="2" style="height:50px;width:300px;"></textarea>       
	            </td>
	        </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.opener.refresh(); window.focus();window.opener=null;window.close()" />                
                    <input type="submit" value="保存" class="btn wd1" />   
                </td>
            </tr>
        </table>
    </form>
    <% 
        End Sub
    %>
</body>
</html>
