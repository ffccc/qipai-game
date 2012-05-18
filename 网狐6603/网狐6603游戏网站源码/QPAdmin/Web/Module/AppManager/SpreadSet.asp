<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../Conn.asp"-->
<!--#include file="../../GameConn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>无标题页</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/Calendar.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：后台系统 - 推广管理</td>
        </tr>
    </table>
     <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7 ">
        <tr>
            <td height="28">
                <ul>
	                <li class="tab1" >推广设置</li>
                    <li class="tab2" onclick="openWindowSelf('SpreadManager.asp')" >推广明细</li>
	                <li class="tab2" onclick="openWindowSelf('FinanceInfo.asp')">财务明细</li>
                </ul>
            </td>
        </tr>
    </table>  
    <!-- 头部菜单 End --> 
    <% 
    
        Call ConnectGame(QPTreasureDB)
        Select case lcase(Request("action"))
            case "save"
            Call Save()
            case else
            Call Main()
        End Select
        
        Sub Save()
            Dim rs,sql
            Dim RegisterGrantScore,PlayTimeCount,PlayTimeGrantScore,FillGrantRate,BalanceRate,MinBalanceScore
            Call ConnectGame(QPTreasureDB)
            If Request("in_RegisterScore")<>"" Then
                RegisterGrantScore=Request("in_RegisterScore")
            End If
            
            If Request("in_TimesCounts")<>"" Then
                PlayTimeCount=Request("in_TimesCounts")
            End If
            
            If Request("in_TimesCountsScore")<>"" Then
                PlayTimeGrantScore=Request("in_TimesCountsScore")
            End If
            
            If Request("in_RechargeRate")<>"" Then
                FillGrantRate=Request("in_RechargeRate")
            End If
            
            If Request("in_ClearingRate")<>"" Then
                BalanceRate=Request("in_ClearingRate")
            End If
            
            If Request("in_MinGrant")<>"" Then
                MinBalanceScore=Request("in_MinGrant")
            End If   
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select top 1 * from GlobalSpreadInfo "
            rs.open sql,GameConn,1,3
            If rs.eof then 
                rs.addnew      
            End If  
            rs("RegisterGrantScore") = RegisterGrantScore
            rs("PlayTimeCount") = PlayTimeCount
            rs("PlayTimeGrantScore") = PlayTimeGrantScore
            rs("FillGrantRate") = FillGrantRate/100
            rs("BalanceRate") = BalanceRate/100
            rs("MinBalanceScore") = MinBalanceScore
            rs.update
            Call CxGame.ShowInfo2("修改成功！","SpreadSet.asp",1200)
        End Sub 
        
        Sub Main()
            Dim rs,sql
            Dim RegisterGrantScore,PlayTimeCount,PlayTimeGrantScore,FillGrantRate,BalanceRate,MinBalanceScore
            Set rs=server.createobject("adodb.recordset")    
            sql="select  top 1 * from GlobalSpreadInfo"
            rs.open sql,GameConn,1,3
            If  rs.eof  then  
                RegisterGrantScore=0
                PlayTimeCount = 0
                PlayTimeGrantScore = 0
                FillGrantRate = 0
                BalanceRate = 0
                MinBalanceScore=0
            Else
                RegisterGrantScore=rs(1)
                PlayTimeCount = rs(2)
                PlayTimeGrantScore = rs(3)
                FillGrantRate = CDbl(rs(4))*100
                BalanceRate = CDbl(rs(5))*100
                MinBalanceScore = rs(6)
            End If   
    %>
    <script>
        function CheckFormInfo()
        {
            var RegisterGrantScore = document.myFormInfo2.in_RegisterScore.value;
            var PlayTimeCount = document.myFormInfo2.in_TimesCounts.value;
            var PlayTimeGrantScore = document.myFormInfo2.in_TimesCountsScore.value;
            var FillGrantRate = document.myFormInfo2.in_RechargeRate.value;
            var BalanceRate = document.myFormInfo2.in_ClearingRate.value;
            var MinBalanceScore = document.myFormInfo2.in_MinGrant.value;
            if(IsPositiveInt(RegisterGrantScore)==false)
            {
                alert("注册赠送金币非数值型字符！");
                return false;
            }
            if(IsPositiveInt(PlayTimeCount)==false)
            {
                alert("在线时长非数值型字符！");
                return false;
            }
            if(IsPositiveInt(PlayTimeGrantScore)==false)
            {
                alert("在线时长赠送金币非数值型字符！");
                return false;
            }
            if(IsPositiveInt(FillGrantRate)==false)
            {
                alert("充值赠送比率非数值型字符！");
                return false;
            }
            if(IsPositiveInt(BalanceRate)==false)
            {
                alert("结算赠送比率非数值型字符！");
                return false;
            }
            if(IsPositiveInt(MinBalanceScore)==false)
            {
                alert("结算最小值非数值型字符！");
                return false;
            }
        }
    </script>
    <form name="myFormInfo2" method="post" action='?action=save' onsubmit="return CheckFormInfo()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="submit" value="保存" class="btn wd1" />  
                </td>
            </tr> 
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">注册赠送</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">金币：</td>
                <td>
                   <input name="in_RegisterScore" type="text" class="text" style="width:300px;" value="<%=RegisterGrantScore %>" />
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">游戏时长赠送</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">时长：</td>
                <td>
                    <input name="in_TimesCounts" type="text" class="text" style="width:300px;" value="<%=PlayTimeCount %>" /><span class="hui6">(单位:秒)</span>    
                    &nbsp;&nbsp;<span class="hui6">为空时为默认值</span>                       
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">金币：</td>
                <td>
                    <input name="in_TimesCountsScore" type="text" class="text" style="width:300px;" value="<%=PlayTimeGrantScore %>" />
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">充值赠送</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">比率：</td>
                <td>
                   <input name="in_RechargeRate" type="text" class="text" style="width:300px;" value="<%=FillGrantRate %>" /><span class="hui6">(单位%)</span>    
                    &nbsp;&nbsp;<span class="hui6">为空时为默认值</span>                       
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">结算赠送</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">比率：</td>
                <td>
                    <input name="in_ClearingRate" type="text" class="text" style="width:300px;" value="<%=BalanceRate %>" /><span class="hui6">(单位%)</span>    
                    &nbsp;&nbsp;<span class="hui6">为空时为默认值</span>                                    
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最低值：</td>
                <td>
                    <input name="in_MinGrant" type="text" class="text" style="width:300px;" value="<%=MinBalanceScore %>" />                
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="submit" value="保存" class="btn wd1" />  
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