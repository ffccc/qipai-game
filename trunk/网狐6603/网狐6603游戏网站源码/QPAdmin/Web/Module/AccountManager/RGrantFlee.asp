<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>清零逃率</title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：清零逃率</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <% 
        Select case lcase(Request("action"))
            case "save"
            Call Save()
            case else
            Call Main()
        End Select
        
        Sub Save()
            Dim kindID,dname
            kindID = CxGame.GetInfo(1,"in_KindID")
            dname="QPGameScoreDB"
            
            IF kindID<=0 Then
                dname="QPGameScoreDB"
            Else
                dname = CxGame.GetKindDBNameByKindID(kindID)
                If IsNull(dname) Or dname="" Then
                    dname="QPGameScoreDB"
                End If
            End If  
            
            Dim UserArr,i,reason,masterID,clientIP,scoreList,scoreArr
            reason = CxGame.GetInfo(0,"in_Reason")
            masterID = session("AdminID")
            clientIP = Request.ServerVariables("REMOTE_ADDR")
            UserArr = Split(Request("userlist"),",")
            
            '赠送积分
            dim re
            re=""
            Call ConnectGame(dname)
                For i=0 To UBound(UserArr)
                    re = GameConn.execute("WSP_PM_GrantClearFlee "&masterID&",'"&clientIP&"',"&UserArr(i)&","&kindID&",'"&reason&"'")(0)
                Next                    
            Call CloseGame()
             If re<>"" Then
                CxGame.MessageBoxClose(re)
            Else
                CxGame.MessageBoxClose("清零逃率成功！")
            End If            
        End Sub
        
        Sub Main()
    %>
    <script type="text/javascript">
        function CheckForm()
        {
            var reason = document.form1.in_Reason.value.trim();
            if(reason == "")
            {
                alert("清零原因不能为空！");
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
	            <td class="listTdLeft">游戏名称：</td>
	            <td>
	                <select name="in_KindID" style="width:159px;">
                    <% 
                        Dim ArrayList,i
                        ArrayList = CxGame.GetScoreKindList
                        For i=0 To Ubound(ArrayList) 
                        If Not IsEmpty(ArrayList(i,0)) Then
                    %>
                    <option value="<%=ArrayList(i,0) %>"><%=ArrayList(i,1) %></option>           
                    <% 
                        End If
                        Next  
                        Set ArrayList = nothing
                    %>         
                    </select>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">清零原因：</td>
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
