<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/AutoCheck.asp" -->
<!--#include file="Inc/Conn.asp" -->
<!--#include file="Inc/md5.asp" -->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
    <meta name="Description" content="<%=CxGame.WebTitle %>" />
    <meta name="Keywords" content="<%=CxGame.WebTitle %>" />
    <meta name="rating" content="general" />
    <meta name="robots" content="all" />
    <title><%=CxGame.WebTitle %></title>
    <link rel="stylesheet" type="text/css" href="css/layout.css" />
    <link rel="stylesheet" type="text/css" href="css/global.css" />
    <link rel="stylesheet" type="text/css" href="Css/user_layout.css" />
    <link rel="stylesheet" type="text/css" href="Css/game_layout.css" />   
    <script src="JS/jquery_last.js" type="text/javascript"></script>
    <script src="JS/formValidatorRegex.js" type="text/javascript"></script>
    <script type="text/javascript">
	    $(document).ready(function(){
            $("#form1").submit(function(){
                if($.trim($("#txtEMail").val())!=""&&!isEmail($.trim($("#txtEMail").val())))
                {
                    alert("Email输入非法！");
                    return false;
                }                
            })
        })		        
	</script> 
</head>
<body>
<%
    CxGame.Channel = "Member" 
    CxGame.IsCheckLogin
%>
<!--#include file="Themes/Common_Header.asp" -->

<!--页面主体开始-->
<div class="userBody" style="margin-top:8px;">
	<div class="userTop"></div>
	<div class="userBg">
	    <!--左边页面开始-->
	    <div class="LeftSide">
			<!--会员登录开始-->
			<!--#include file="Themes/Common_Logon.asp" -->
			<!--会员登录结束-->
			<!--#include file="Themes/Member_Sidebar.asp" -->					
		</div>
		<!--左边页面结束-->
		
		<!--右边页面开始-->
		<% 
		    ConnectGame(QPAccountsDB)
		    Dim rs
		    Set rs = GameConn.Execute("WSP_PW_GetUserInfo "&Session("UserID")&"")
		    If rs(0) <> 0 Then
			    CxGame.MessageBoxReload rs(1),"MemberIndex.asp"
		    Else
		%>
		<div class="mainRight2">
			<div class="userRigthBody">
				<div class="userRigthTitle">会员中心 - 帐号服务 - 修改资料</div>
				<div class="userRigthBg">
				    <!--修改昵称开始-->				    
				    <form id="form1" name="form1" method="post" action="">
	                <table width="100%" cellpadding="0" cellspacing="0">
                    <tr>
                      <td bgcolor="#f7f9fd" colspan="4" align="left"><strong> 　1.帐号资料</strong></td>
                    </tr>
                    <tr>
                      <td colspan="4"><table id="table0" border="0" cellspacing="0" cellpadding="0" width="100%">
                          <tbody>
                            <tr>
                              <td width="17%" align="right">帐号：</td>
                              <td width="28%" align="left"><%=rs("Accounts") %></td>
                              <td width="18%" align="right">ＩＤ号码：</td>
                              <td width="37%" align="left"><%=rs("GameID") %></td>
                            </tr>
                          </tbody>
                      </table></td>
                    </tr>
                    <tr>
                      <td bgcolor="#F7F9FD" colspan="4" align="left"><strong>　2.基本资料</strong></td>
                    </tr>
                    <tr>
                      <td colspan="4"><table border="0" cellspacing="0" cellpadding="0" width="100%">
                          <tbody>
                            <tr>
                              <td width="17%" align="right">性别：</td>
                              <td width="83%" align="left">
                              <select name="ddlGender">                                
                                <option value="0"<%If rs("Gender")=0 Then %> selected="selected" <%End If %>>女性</option>
                                <option value="1"<%If rs("Gender")=1 Then %> selected="selected" <%End If %>>男性</option>
                              </select>                
                              </td>                              
                            </tr>
                            <tr>
                              <td width="17%" align="right">个性签名：</td>
                              <td width="83%" align="left">
                              <input type="text" name="txtUnderWrite" class="text" style="width:457px;" value="<%=rs("UnderWrite") %>" />         
                              </td>                              
                            </tr>
                          </tbody>
                      </table></td>
                    </tr>
                    <tr>
                      <td bgcolor="#F7F9FD" colspan="4" align="left"><strong>　3.我的身份资料</strong></td>
                    </tr>
                    <tr>
                      <td colspan="4"><table border="0" cellspacing="0" cellpadding="0" width="100%">
                          <tbody>
                            <tr>
                              <td  align="right">真实姓名：</td>
                              <td width="32%" align="left" colspan="3"><input type="text" name="txtCompellation" class="text" value="<%=rs("Compellation") %>" /></td>
                            </tr>     
                            <tr>
                              <td width="17%" align="right">手机号码：</td>
                              <td width="28%" align="left"><input type="text" name="txtMobilePhone" class="text" value="<%=rs("MobilePhone") %>" /></td>
                              <td width="18%" align="right">固定电话：</td>
                              <td width="37%" align="left"><input type="text" name="txtSeatPhone" class="text" value="<%=rs("SeatPhone") %>" /></td>
                            </tr>                       
                            <tr>
				              <td align="right">QQ/MSN：</td>
                              <td align="left"><input type="text" name="txtQQ" class="text" value="<%=rs("QQ") %>"/></td>
                              <td align="right">Email：</td>
                              <td align="left"><input type="text" id="txtEMail" name="txtEMail" class="text" value="<%=rs("Email") %>"/></td>
                            </tr>
                            <tr>
                              <td align="right">详细地址：</td>
                              <td align="left" colspan="3"><input type="text" name="txtAddress" class="text" value="<%=rs("DwellingPlace") %>" style="width:457px;"/></td>
                            </tr>
                          </tbody>
                      </table></td>
                    </tr>                    
                    <tr>
                      <td bgcolor="#F7F9FD" colspan="4" align="left"><strong>　4.我的其他说明 </strong></td>
                    </tr>
                    <tr>
                      <td colspan="4" align="middle"><textarea name="txtUserNote" rows="8" cols="60" class="mtop13"><%=rs("UserNote") %></textarea></td>
                    </tr>
                    <tr>
                      <td colspan="4" align="center">
                        <input name="button3" type="submit" class="bnt lan bold" value="确定" hidefocus="true"/>
                        <input name="Modify" type="hidden" value="true" />
                      </td>
                    </tr>
                    </table>
                    <%
                        rs.Close:Set rs=Nothing:CloseGame()
                        End If
                        CxGame.ModifyUserInfo() 
                    %>                 
                    </form>
	                <!--修改昵称结束-->
                <div class="clear"></div>
		 	    </div>
				<div class="userRightBottom"><div class="clear"></div></div>
				<div class="clear"></div>
			</div>
			<div class="clear"></div>
		</div>
		<!--右边页面结束-->
		<div class="clear"></div>
	</div>
	<div class="userBottom"><div class="clear"></div></div>
<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
