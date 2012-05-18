<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>赠送靓号</title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <style>
        .gamelistbox {width:300px; background:#FFFFD2;font-size:12px;color:#333; line-height: 18px; text-align:left;
        border: 1px solid #FFCC66;display:none; z-index:999;  position: absolute; top:160px; right:22px;padding:5px;
        filter:progid:DXImageTransform.Microsoft.DropShadow(color=#9a8559,offX=1,offY=1,positives=true); }
    </style>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：赠送靓号</td>
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
            Dim reGameID,reason,masterID,clientIP,rValue
            reGameID = CxGame.SqlCheckNum(Request("in_ReGameID"))
            reason = CxGame.GetInfo(0,"in_Reason")
            masterID = session("AdminID")
            clientIP = Request.ServerVariables("REMOTE_ADDR")
            rValue = GameConn.execute("WSP_PM_GrantGameID "&masterID&","&request("id")&","&reGameID&",'"&clientIP&"','"&reason&"'")(0)
            If rValue="" OR IsNull(rValue) Then
                CxGame.MessageBoxClose("赠送靓号成功！")
            Else
                Call CxGame.MessageBoxReload(rValue,"RGrantGameID.asp?UserID="&Request("id"))
            End If
        End Sub
        
        Sub Main()
            Dim rs,sql
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo where UserID='"&Request("UserID")&"'"
            rs.Open sql,GameConn,1,3              
    %>
    <script type="text/javascript">
        function CheckForm()
        {
            var reGameID = document.form1.in_ReGameID.value.trim();
            var reason = document.form1.in_Reason.value.trim();
            if(reGameID == "")
            {
                alert("赠送号码不能为空！");
                return false;
            }
            else
            {
                if(strLen(reGameID)>10)
                {
                    alert("赠送号码输入非法！");
                    return false;
                }
                else
                {
                    if(!IsPositiveInt64(reGameID))
                    {
                        alert("赠送号码输入非法！");
                        return false;
                    }
                }
            }
            if(reason == "")
            {
                alert("赠送原因不能为空！");
                return false;
            }
        }
        
    </script>  
    <form name="form1" method="post" action='?action=save&id=<%=request("UserID") %>' onsubmit="return CheckForm()">
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
	            <td class="listTdLeft">用户名(GameID)：</td>
	            <td>
	                <span style="color:Blue;"><%=rs("Accounts") %>(<%=rs("GameID") %>)</span> 
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">赠送靓号ID：</td>
	            <td>
	                <% 
		                Dim rs_Game,sql_Game,i
		                Set rs_Game=Server.CreateObject("Adodb.RecordSet")
		                sql_Game = "select Top 10 * from QPAccountsDBLink.QPAccountsDB.dbo.ReserveIdentifier order by newid()"
		                rs_Game.Open sql_Game,GameConn,1,3    
		            %>
		            <input name="in_ReGameID" type="text" class="text" value="<%=rs_Game(0) %>" />     
			        <select name="gameIDSelect" style="width:80px;" onchange="document.form1.in_ReGameID.value = this.value">
			        <% do until rs_Game.eof  %>
			        <option value="<%=rs_Game(0) %>"><%=rs_Game(0) %></option>
			        <%  
			            rs_Game.movenext
                        loop
			        %>
			        </select>         
			        <%  
			            Set rs_Game = Nothing
			        %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">赠送原因</td>
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
        rs.Close()
        Set rs=nothing
        End Sub
    %>
</body>
</html>
