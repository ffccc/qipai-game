<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>“<%=CxGame.GetUserNameByUserID(Request("id")) %>”记录信息</title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
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
                    <li class="tab2" onclick="openWindowSelf('AccountsInfo.asp?id=<%=Request("id") %>')">基本信息</li>
	                <li class="tab2" onclick="openWindowSelf('AccountsDetailInfo.asp?id=<%=Request("id") %>')">详细信息</li>
	                <li class="tab2" onclick="openWindowSelf('AccountsGoldInfo.asp?id=<%=Request("id") %>')">财富信息</li>
	                <li class="tab2" onclick="openWindowSelf('AccountsScoreInfo.asp?id=<%=Request("id") %>')">积分信息</li>
	                <li class="tab1">记录信息</li>
                </ul>
            </td>
        </tr>
    </table>   
    <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
        <tr>
            <td class="titleOpBg Lpd10">
                <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />                
            </td>
        </tr>
    </table>
    <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
        <tr>
            <td class="listTd" style="padding-left:88px;">
                用户名：&nbsp;<span style="color:blue"><%=CxGame.GetUserNameByUserID(Request("id")) %></span>&nbsp;&nbsp;&nbsp;
            </td>
        </tr>
        <tr>
            <td height="35" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">基本修改记录</div></td>
        </tr>
        <tr>
            <td class="listTd" style="padding-left:88px;">
                <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('RAccountsExpend.asp?id=<%=Request("id") %>','RAccounts_<%=Request("id") %>',800,500)">历史改名字记录</a>&nbsp;&nbsp;
			    <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('RPasswdExpend.asp?id=<%=Request("id") %>','RPasswd_<%=Request("id") %>',800,500)">历史改密码记录</a>&nbsp;&nbsp;
            </td>
        </tr>
        <tr>
            <td height="35" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">财富信息记录</div></td>
        </tr>
        <tr>
            <td class="listTd" style="padding-left:88px;">
                <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('GoldUserInoutRecord.asp?id=<%=Request("id") %>','GoldInout_<%=Request("id") %>',800,500)">进出记录</a>&nbsp;&nbsp;  
                <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('GoldUserGameRecord.asp?id=<%=Request("id") %>','GoldGame_<%=Request("id") %>',800,500)">游戏记录</a>&nbsp;&nbsp;
                <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('GoldUserInsureRecord.asp?id=<%=Request("id") %>','GoldInsure_<%=Request("id") %>',800,500)">银行记录</a>&nbsp;&nbsp;      
                <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('GoldUserOLRecord.asp?id=<%=Request("id") %>','GoldFill_<%=Request("id") %>',850,500)">充值记录</a>&nbsp;&nbsp;                        			
            </td>
        </tr>
        <tr>
            <td height="35" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">积分信息记录</div></td>
        </tr>
        <tr>
            <td class="listTd" style="padding-left:88px;">
                <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('ScoreUserInoutRecord.asp?id=<%=Request("id") %>','ScoreInout_<%=Request("id") %>',850,500)">进出记录</a>&nbsp;&nbsp;  
            </td>
        </tr>
        <tr>
            <td height="35" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">系统赠送记录</div></td>
        </tr>
        <tr>
            <td class="listTd" style="padding-left:88px;">
                <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('RGrantMemberList.asp?id=<%=Request("id") %>','RGrantMember_<%=Request("id") %>',800,500)">赠送会员记录</a>&nbsp;&nbsp;
			    <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('RGrantTreasureList.asp?id=<%=Request("id") %>','RGrantTreasure_<%=Request("id") %>',800,500)">赠送金币记录</a>&nbsp;&nbsp;
			    <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('RGrantScoreList.asp?id=<%=Request("id") %>','RGrantScore_<%=Request("id") %>',800,500)">赠送积分记录</a>&nbsp;&nbsp;
			    <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('RGrantGameIDList.asp?id=<%=Request("id") %>','RGrantGameID_<%=Request("id") %>',800,500)">赠送靓号记录</a>&nbsp;&nbsp;
			    <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('RGrantExperienceList.asp?id=<%=Request("id") %>','RGrantExperience_<%=Request("id") %>',800,500)">赠送经验记录</a>&nbsp;&nbsp;
			    <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('RGrantClearScoreList.asp?id=<%=Request("id") %>','RGrantClearScore_<%=Request("id") %>',800,500)">清零积分记录</a>&nbsp;&nbsp;
			    <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('RGrantFleeList.asp?id=<%=Request("id") %>','RGrantFlee_<%=Request("id") %>',800,500)">清零逃率记录</a>&nbsp;&nbsp;
            </td>
        </tr>
        <tr>
            <td height="35" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">魅力兑换记录</div></td>
        </tr>
        <tr>
            <td class="listTd" style="padding-left:88px;">
                <a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('RConvertPresent.asp?id=<%=Request("id") %>','RConvertPresent_<%=Request("id") %>',800,500)">魅力兑换记录</a>&nbsp;&nbsp;
            </td>
        </tr>
    </table>
    <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
        <tr>
            <td class="titleOpBg Lpd10">
                <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />                
            </td>
        </tr>
    </table>
</body>
</html>
