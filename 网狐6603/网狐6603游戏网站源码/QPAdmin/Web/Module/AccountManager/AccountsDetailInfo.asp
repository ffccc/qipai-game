<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>“<%=CxGame.GetUserNameByUserID(Request("id")) %>”详细信息</title>
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
            <td width="1232" height="25" valign="top" align="left">目前操作功能：用户信息</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
        <tr>
            <td height="28">
                <ul>
                    <li class="tab2" onclick="openWindowSelf('AccountsInfo.asp?id=<%=Request("id") %>')">基本信息</li>
	                <li class="tab1">详细信息</li>
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
            Call SaveUserInfo()
            case else
            Call Main()
        End Select
        Call CloseGame()
        
        Sub SaveUserInfo()
            Dim rs,sql
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from IndividualDatum where UserID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3
            If rs.eof Then
                rs.addnew
            End If
            rs("UserID") = Request("id")
            rs("Compellation") = CxGame.GetInfo(0,"in_Compellation")
            rs("QQ") = CxGame.GetInfo(0,"in_QQ")
            rs("EMail") = CxGame.GetInfo(0,"in_EMail")
            rs("SeatPhone") = CxGame.GetInfo(0,"in_SeatPhone")
            rs("MobilePhone") = CxGame.GetInfo(0,"in_MobilePhone")
            rs("DwellingPlace") = CxGame.GetInfo(0,"in_DwellingPlace")
            rs("PostalCode") = CxGame.GetInfo(0,"in_PostalCode")
            rs("UserNote") = CxGame.GetInfo(0,"in_UserNote")
            rs.update
            Call CxGame.MessageBoxReload("修改成功！","AccountsDetailInfo.asp?id="&Request("id"))
            rs.close()
            Set rs = Nothing   
        End Sub
        
        Sub Main()
            Dim rs,sql
            Dim compellation,qQ,eMail,seatPhone,mobilePhone,dwellingPlace,postalCode,userNote
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from IndividualDatum(nolock) where UserID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3  
            If Not rs.Bof And Not rs.Eof Then
                compellation = rs("Compellation")
                qQ = rs("QQ")
                eMail = rs("EMail")
                seatPhone = rs("SeatPhone")
                mobilePhone = rs("MobilePhone")
                dwellingPlace = rs("DwellingPlace")
                postalCode = rs("PostalCode")
                userNote = rs("UserNote")
            End If
    %>
    <script type="text/javascript">
        function CheckFormInfo()
        {
            var re = /^[1-9]+[0-9]*]*$/;
            var phonere = /\d{3}-\d{8}|\d{4}-\d{7}/; 
            var email=document.form1.in_EMail.value;
            var seatPhone = document.form1.in_SeatPhone.value;
            var compellation = document.form1.in_Compellation.value;
            var qq = document.form1.in_QQ.value;
            var mobilePhone = document.form1.in_MobilePhone.value;
            var postalCode = document.form1.in_PostalCode.value;
            var dwellingPlace = document.form1.in_DwellingPlace.value;
            var userNote = document.form1.in_UserNote.value;
            if(len(compellation)>16)
            {
                alert("真实姓名字符长度不可超过16个字符！");
                document.form1.in_Compellation.focus();
                return false;
            }
            if(!re.test(qq))
            {
                alert("QQ输入非法！");
                document.form1.in_QQ.focus();
                return false;
            }
            else if(len(qq)>16)
            {
                alert("QQ字符长度不可超过16个字符！");
                document.form1.in_QQ.focus();
                return false;
            }
            if(email!=""&&!IsEmail(email))
            {
                alert("电子邮箱输入非法！");
                document.form1.in_EMail.focus();
                return false;
            }
            else if(len(email)>32)
            {
                alert("电子邮箱字符长度不可超过32个字符！");
                document.form1.in_EMail.focus();
                return false;
            }
            if(!phonere.test(seatPhone))
            {
                alert("电话号码不正确，请输入形如 区号+电话号码 的数字！")
                document.form1.in_SeatPhone.focus();
                return false;
            }
            else if(len(seatPhone)>32)
            {
                alert("固定电话字符长度不可超过32个字符！");
                document.form1.in_EMail.focus();
                return false;
            }
            if(len(mobilePhone)>16)
            {
                alert("手机号码字符长度不可超过16个字符！");
                document.form1.in_MobilePhone.focus();
                return false;
            }
            if(len(postalCode)>8)
            {
                alert("邮政编码字符长度不可超过8个字符！");
                document.form1.in_PostalCode.focus();
                return false;
            }
            if(len(dwellingPlace)>128)
            {
                alert("居住地址字符长度不可超过128个字符！");
                document.form1.in_DwellingPlace.focus();
                return false;
            }
            if(len(userNote)>256)
            {
                alert("用户备注字符长度不可超过256个字符！");
                document.form1.in_UserNote.focus();
                return false;
            }
        }
    </script>
    <form name="form1" method="post" action='?action=save&id=<%=Request("id") %>' onsubmit="return CheckFormInfo()">
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
                <td class="listTdLeft">用户名：</td>
                <td>
                    <span class="Rpd20 lan bold"><%=CxGame.GetUserNameByUserID(Request("id")) %></span>                 
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">真实姓名：</td>
                <td>
                    <input name="in_Compellation" type="text" class="text wd4" value="<%=compellation %>"/>&nbsp;真实姓名字符长度不可超过16个字符                  
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">QQ：</td>
                <td>
                    <input name="in_QQ" type="text" class="text wd4" value="<%=qQ %>"/>&nbsp;QQ字符长度不可超过16个字符                                  
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">电子邮箱：</td>
                <td>
                    <input name="in_EMail" type="text" class="text wd4" value="<%=eMail %>"/>&nbsp;电子邮箱字符长度不可超过32个字符                                
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">固定电话：</td>
                <td>
                    <input name="in_SeatPhone" type="text" class="text wd4" value="<%=seatPhone %>"/>&nbsp;固定电话字符长度不可超过32个字符                                             
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">手机号码：</td>
                <td>
                    <input name="in_MobilePhone" type="text" class="text wd4" value="<%=mobilePhone %>"/>&nbsp;手机号码字符长度不可超过16个字符                                                
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">邮政编码：</td>
                <td>
                    <input name="in_PostalCode" type="text" class="text wd4" value="<%=postalCode %>"/>&nbsp;邮政编码字符长度不可超过8个字符                                                  
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">居住地址：</td>
                <td>
                    <input name="in_DwellingPlace" type="text" class="text wd4" style="width:500px;" value="<%=dwellingPlace %>"/>&nbsp;居住地址字符长度不可超过128个字符                                                         
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">用户备注：</td>
                <td>
                    <input name="in_UserNote" type="text" class="text wd4" style="width:500px;" value="<%=userNote %>"/>&nbsp;用户备注字符长度不可超过256个字符                                                           
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
