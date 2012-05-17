<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>赠送金币</title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：赠送金币</td>
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
            Dim UserArr,i,addGold,reason,masterID,clientIP
            addGold = CxGame.SqlCheckNum(Request("in_AddGold"))
            reason = CxGame.GetInfo(0,"in_Reason")
            masterID = session("AdminID")
            clientIP = Request.ServerVariables("REMOTE_ADDR")
            UserArr = Split(Request("userlist"),",")
            For i=0 To UBound(UserArr)
                GameConn.execute("WSP_PM_GrantTreasure "&masterID&",'"&clientIP&"',"&UserArr(i)&","&addGold&",'"&reason&"'")
            Next
            CxGame.MessageBoxClose("赠送金币成功！")
        End Sub
        
        Sub Main()
    %>
    <script type="text/javascript">
        function CheckForm()
        {
            var addGold = document.form1.in_AddGold.value.trim();
            var reason = document.form1.in_Reason.value.trim();
            if(addGold=="")
            {
                alert("赠送金币不能为空！");
                return false;
            }
            else
            {
                if(!IsPositiveInt64(addGold))
                {
                    alert("赠送金币输入非法！");
                    return false;
                }
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
                    <input type="submit" value="保存" class="btn wd1" />   
                </td>
            </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
	            <td class="listTdLeft">赠送金币数目：</td>
	            <td>
	                <input name="in_AddGold" type="text" class="text" />            
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
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />                
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
