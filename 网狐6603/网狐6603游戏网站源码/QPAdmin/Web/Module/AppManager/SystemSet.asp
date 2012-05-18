<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../Conn.asp"-->
<!--#include file="../../GameConn.asp"-->
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <title>无标题页</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script> 
    <script type="text/javascript" src="../../Js/comm.js"></script>  
    <script type="text/javascript" src="../../Js/Check.js"></script>  
    <script type="text/javascript" src="../../Js/Sort.js"></script> 
</head>
<%
    If CxGame.GetRoleValue("304",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：系统维护 - 系统设置</td>
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
        
        Sub Save()          
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='EnjoinInsure'"
            rs.open sql,GameConn,1,3
            If rs.eof then 
                rs.addnew    
                rs("StatusName") = "EnjoinInsure"  
            End If              
            rs("StatusValue") = CxGame.GetInfo(1,"in_EnjoinInsure")
            rs("StatusString") = CxGame.GetInfo(0,"in_EnjoinInsureNote")
            rs.update
            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='EnjoinLogon'"
            rs.open sql,GameConn,1,3
            If rs.eof then 
                rs.addnew      
                rs("StatusName") = "EnjoinLogon"
            End If              
            rs("StatusValue") = CxGame.GetInfo(1,"in_EnjoinLogon")
            rs("StatusString") = CxGame.GetInfo(0,"in_EnjoinLogonNote")
            rs.update
            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='EnjoinRegister'"
            rs.open sql,GameConn,1,3
            If rs.eof then 
                rs.addnew      
                rs("StatusName") = "EnjoinRegister"
            End If              
            rs("StatusValue") = CxGame.GetInfo(1,"in_EnjoinRegister")
            rs("StatusString") = CxGame.GetInfo(0,"in_EnjoinRegisterNote")
            rs.update
                        
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='GrantIPCount'"
            rs.open sql,GameConn,1,3
            If rs.eof then 
                rs.addnew      
                rs("StatusName") = "GrantIPCount"
            End If  
            rs("StatusValue") = CxGame.GetInfo(1,"in_GrantIPCount")
            rs("StatusString") = CxGame.GetInfo(0,"in_GrantIPCountNote")
            rs.update
            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='GrantScoreCount'"
            rs.open sql,GameConn,1,3
            If rs.eof then 
                rs.addnew      
                rs("StatusName") = "GrantScoreCount"
            End If  
            rs("StatusValue") = CxGame.GetInfo(1,"in_GrantScoreCount")
            rs("StatusString") = CxGame.GetInfo(0,"in_GrantScoreCountNote")
            rs.update
            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='RevenueRate'"
            rs.open sql,GameConn,1,3
            If rs.eof then 
                rs.addnew      
                rs("StatusName") = "RevenueRate"
            End If  
            rs("StatusValue") = CxGame.GetInfo(1,"in_RevenueRate")
            rs("StatusString") = CxGame.GetInfo(0,"in_RevenueRateNote")
            rs.update
            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='MedalRate'"
            rs.open sql,GameConn,1,3
            If rs.eof then 
                rs.addnew      
                rs("StatusName") = "MedalRate"
            End If  
            rs("StatusValue") = CxGame.GetInfo(1,"in_MedalRate")
            rs("StatusString") = CxGame.GetInfo(0,"in_MedalRateNote")
            rs.update
            Call CxGame.ShowInfo2("修改成功！","SystemSet.asp",1200)
        End Sub 
        
        Sub Main()
            Dim rs,sql
            Dim EnjoinInsure,EnjoinInsureNote,EnjoinLogon,EnjoinLogonNote,EnjoinRegister,EnjoinRegisterNote,GrantIPCount,GrantIPCountNote
            Dim GrantMachineCount,GrantMachineCountNote,GrantScoreCount,GrantScoreCountNote,MedalRate,MedalRateNote,RevenueRate,RevenueRateNote
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='EnjoinInsure'"
            rs.open sql,GameConn,1,3
            if  rs.eof  then  
                EnjoinInsure = "0"
                EnjoinInsureNote="由于系统维护，暂时停止游戏系统的保险柜服务，请留意网站公告信息！"
            Else
                EnjoinInsure = rs("StatusValue")
                EnjoinInsureNote=rs("StatusString")
            End If  
            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='EnjoinLogon'"
            rs.open sql,GameConn,1,3
            if  rs.eof  then  
                EnjoinLogon = "0"
                EnjoinLogonNote="由于系统维护，暂时停止游戏系统的登录服务，请留意网站公告信息！"
            Else
                EnjoinLogon = rs("StatusValue")
                EnjoinLogonNote=rs("StatusString")
            End If 
            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='EnjoinRegister'"
            rs.open sql,GameConn,1,3
            if  rs.eof  then  
                EnjoinRegister = "0"
                EnjoinRegisterNote="由于系统维护，暂时停止游戏系统的注册服务，请留意网站公告信息！"
            Else
                EnjoinRegister = rs("StatusValue")
                EnjoinRegisterNote=rs("StatusString")
            End If            
            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='GrantIPCount'"
            rs.open sql,GameConn,1,3
            if  rs.eof  then  
                GrantIPCount = "0"
                GrantIPCountNote="新用户注册IP每天赠送限制！"
            Else
                GrantIPCount = rs("StatusValue")
                GrantIPCountNote=rs("StatusString")
            End If 
            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='GrantScoreCount'"
            rs.open sql,GameConn,1,3
            if  rs.eof  then  
                GrantScoreCount = "0"
                GrantScoreCountNote="新用户注册系统送金币的数目！"
            Else
                GrantScoreCount = rs("StatusValue")
                GrantScoreCountNote=rs("StatusString")
            End If 
            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='RevenueRate'"
            rs.open sql,GameConn,1,3
            if  rs.eof  then  
                RevenueRate = "0"
                RevenueRateNote="银行操作税收比率（百分比）！"
            Else
                RevenueRate = rs("StatusValue")
                RevenueRateNote=rs("StatusString")
            End If 
            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select  * from dbo.SystemStatusInfo where StatusName='MedalRate'"
            rs.open sql,GameConn,1,3
            if  rs.eof  then  
                MedalRate = "0"
                MedalRateNote="奖牌系统返还比率（千分比），奖牌根据玩家每局游戏税收返还给玩家！"
            Else
                MedalRate = rs("StatusValue")
                MedalRateNote=rs("StatusString")
            End If 
    %>
    <script type="text/javascript">
        function CheckFormInfo()
        {
            var grantMachineCount=document.myFormInfo2.in_GrantMachineCount.value.trim();
            var grantScoreCount=document.myFormInfo2.in_GrantScoreCount.value.trim();
            
            if(IsNumeric(grantMachineCount)==false)
            {
                alert("每天赠送限制非数值型字符！");
                return false;
            }
            if(grantMachineCount<0)
            {
                alert("每天赠送限制不可为负数！");
                return false;
            }
            if(IsNumeric(grantScoreCount)==false)
            {
                alert("注册赠送金币数目非数值型字符！");
                return false;
            }
            if(grantScoreCount<0)
            {
                alert("注册赠送金币数目不可为负数！");
                return false;
            }
            
            if(IsNumeric(grantScoreCount)==false)
            {
                alert("注册赠送金币数目非数值型字符！");
                return false;
            }
            if(grantScoreCount<0)
            {
                alert("注册赠送金币数目不可为负数！");
                return false;
            }
        }
    </script>
    <form name="myFormInfo2" method="post" action='?action=save' onsubmit="return CheckFormInfo()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="submit" value="保存更改" class="btn wd2" />  
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">保险柜服务</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">状态：</td>
                <td>
                    <input name="in_EnjoinInsure" type="radio" id="in_EnjoinInsure0" value="0" <%If EnjoinInsure=0 Then %> checked="checked" <%End If %> /><label for="in_EnjoinInsure0">启用</label>
                    <input name="in_EnjoinInsure" type="radio" id="in_EnjoinInsure1" value="1" <%If EnjoinInsure=1 Then %> checked="checked" <%End If %> /><label for="in_EnjoinInsure1">停用</label>                      
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">说明：</td>
                <td>
                    <input name="in_EnjoinInsureNote" type="text" class="text" style="width:500px;" value="<%=EnjoinInsureNote %>" />  
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">登录服务</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">状态</td>
                <td>
                    <input name="in_EnjoinLogon" type="radio" id="in_EnjoinLogon0" value="0" <%If EnjoinLogon=0 Then %> checked="checked" <%End If %> /><label for="in_EnjoinLogon0">启用</label>
                    <input name="in_EnjoinLogon" type="radio" id="in_EnjoinLogon1" value="1" <%If EnjoinLogon=1 Then %> checked="checked" <%End If %> /><label for="in_EnjoinLogon1">停用</label>                                            
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">说明：</td>
                <td>
                    <input name="in_EnjoinLogonNote" type="text" class="text" style="width:500px;" value="<%=EnjoinLogonNote %>" />  
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">注册服务</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">状态：</td>
                <td>
                    <input name="in_EnjoinRegister" type="radio" id="in_EnjoinRegister0" value="0" <%If EnjoinRegister=0 Then %> checked="checked" <%End If %> /><label for="in_EnjoinRegister0">启用</label>
                    <input name="in_EnjoinRegister" type="radio" id="in_EnjoinRegister1" value="1" <%If EnjoinRegister=1 Then %> checked="checked" <%End If %> /><label for="in_EnjoinRegister1">停用</label>                                                                 
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">说明：</td>
                <td>
                    <input name="in_EnjoinRegisterNote" type="text" class="text" style="width:500px;" value="<%=EnjoinRegisterNote %>" />  
                </td>
            </tr>            
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">每天IP赠送限制</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">数目：</td>
                <td>
                    <input name="in_GrantIPCount" type="text" class="text" style="width:300px;" value="<%=GrantIPCount %>" />                        
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">说明：</td>
                <td>
                    <input name="in_GrantIPCountNote" type="text" class="text" style="width:500px;" value="<%=GrantIPCountNote %>" />  
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">注册赠送金币</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">数目：</td>
                <td>
                    <input name="in_GrantScoreCount" type="text" class="text" style="width:300px;" value="<%=GrantScoreCount %>" />                       
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">说明：</td>
                <td>
                    <input name="in_GrantScoreCountNote" type="text" class="text" style="width:500px;" value="<%=GrantScoreCountNote %>" />  
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">银行税收比率</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">数目：</td>
                <td>
                    <input name="in_RevenueRate" type="text" class="text" style="width:300px;" value="<%=RevenueRate %>" />%                      
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">说明：</td>
                <td>
                    <input name="in_RevenueRateNote" type="text" class="text" style="width:500px;" value="<%=RevenueRateNote %>" />  
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">奖牌返还比率</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">比率：</td>
                <td>
                    <input name="in_MedalRate" type="text" class="text" style="width:300px;" value="<%=MedalRate %>" />‰                 
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">说明：</td>
                <td>
                    <input name="in_MedalRateNote" type="text" class="text" style="width:500px;" value="<%=MedalRateNote %>" />  
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="submit" value="保存更改" class="btn wd2" />  
                </td>
            </tr>
        </table>
        <% 
            rs.Close()
            Set rs=nothing
            End Sub
        %>
    </form>
</body>
</html>
