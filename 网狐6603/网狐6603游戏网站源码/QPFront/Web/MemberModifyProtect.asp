<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/AutoCheck.asp" -->
<!--#include file="Inc/Conn.asp" -->

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
    <script src="JS/formValidator.js" type="text/javascript"></script>
    <script src="JS/formValidatorRegex.js" type="text/javascript"></script>
    
    <script type="text/javascript">
        $(document).ready(function(){
            //页面验证
            $.formValidator.initConfig({formid:"form2",alertmessage:false});
            $("#ddlQuestion1").formValidator({onshow:" ",onfocus:"请选择问题"}).inputValidator({min:1,onerror: "你是不是忘记选择问题了"})
                .functionValidator({
                    fun:function(val,elem)
                    {
                        if(val != $("#ddlQuestion2").val()&&val !=$("#ddlQuestion3").val())
                        {
                            return true;
                        }
                        else
                        {
                            return "不能选择相同的问题";
                        }
                    }
                });
            $("#txtMResponse1").formValidator({onshow:" ",onfocus:"答案至少2~16个英文或中文字符"}).inputValidator({min:2,max:16,onerror:"你输入的答案非法"})
                .regexValidator({regexp:"usernamecn",datatype:"enum",onerror:"答案格式不正确"});
            $("#ddlQuestion2").formValidator({onshow:" ",onfocus:"请选择问题"}).inputValidator({min:1,onerror: "你是不是忘记选择问题了"})
                .functionValidator({
                    fun:function(val,elem)
                    {
                        if(val != $("#ddlQuestion1").val()&&val !=$("#ddlQuestion3").val())
                        {
                            return true;
                        }
                        else
                        {
                            return "不能选择相同的问题";
                        }
                    }
                });
            $("#txtMResponse2").formValidator({onshow:" ",onfocus:"答案至少2~16个英文或中文字符"}).inputValidator({min:2,max:16,onerror:"你输入的答案非法"})
                .regexValidator({regexp:"usernamecn",datatype:"enum",onerror:"答案格式不正确"});
            $("#ddlQuestion3").formValidator({onshow:" ",onfocus:"请选择问题"}).inputValidator({min:1,onerror: "你是不是忘记选择问题了"})
                .functionValidator({
                    fun:function(val,elem)
                    {
                        if(val != $("#ddlQuestion1").val()&&val !=$("#ddlQuestion2").val())
                        {
                            return true;
                        }
                        else
                        {
                            return "不能选择相同的问题";
                        }
                    }
                });
            $("#txtMResponse3").formValidator({onshow:" ",onfocus:"答案至少2~16个英文或中文字符"}).inputValidator({min:2,max:16,onerror:"你输入的答案非法"})
                .regexValidator({regexp:"usernamecn",datatype:"enum",onerror:"答案格式不正确"});
            $("#txtSafeEmail").formValidator({onshow:" ",onfocus:"请输入邮箱号码"}).inputValidator({min:6,max:100,onerror:"你输入的邮箱长度非法"})
                .functionValidator({fun:isEmail,onerror:"输入的格式不对"});
            $("#txtSafeEmail2").formValidator({onshow:" ",onfocus:"请输入邮箱号码"}).inputValidator({min:6,max:100,onerror:"你输入的邮箱长度非法"})
                .compareValidator({desid:"txtSafeEmail",operateor:"=",onerror:"两次填写的邮箱号码不一致"});
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
		<div class="mainRight2">
			<div class="userRigthBody">
				<div class="userRigthTitle">会员中心 - 帐号安全 - 修改密码保护</div>
				<div class="userRigthBg">
				<!--修改密码保护开始-->
				<% 
				    ConnectGame(QPAccountsDB)
				    Select case lcase(Request("action"))
				        case "confirm"
				        Call ConfirmAccountProtect()
				        case "step2"
				        Call Step2()
				        case else
				        Call Step1()
				    End Select 
				    Call CloseGame() 
				    
				    '验证密码保护
				    Sub ConfirmAccountProtect()
				        Dim dwUserID,strResponse1,strResponse2,strResponse3,rs
                        dwUserID = Session("UserID")
                        strResponse1 = CxGame.GetInfo(0,"txtResponse1")
                        strResponse2 = CxGame.GetInfo(0,"txtResponse2")
                        strResponse3 = CxGame.GetInfo(0,"txtResponse3")
                        Set rs = GameConn.Execute("WSP_PW_ConfirmAccountProtect "&dwUserID&",'"&strResponse1&"','"&strResponse2&"','"&strResponse3&"'")
                        If rs(0)<>0 Then
                            CxGame.MessageBoxReload rs(1),"MemberModifyProtect.asp"
			                rs.Close:Set rs=Nothing
				            Exit Sub
	                    Else    
	                        Call Step2()
	                        rs.Close:Set rs=Nothing
	                    End If
				    End Sub
				    
				    Sub Step1()
				        Dim rs
				        Set rs = GameConn.Execute("WSP_PW_GetAccountProtectByUserID "&Session("UserID")&"")
				        If rs(0)<>0 Then
				            CxGame.MessageBoxReload rs(1),"MemberIndex.asp"
				        Else
				%>				  
	              <!--第一步开始-->
	              <form id="form1" name="form1" method="post" action="?action=confirm">
                  <table width="100%" cellpadding="0" cellspacing="0">
                    <tr class="bold">
                      <td width="26%" align="right">第一步：</td>
                      <td colspan="2" align="left">请回答下面问题</td>
                    </tr>                    
                    <tr bgcolor="#f7f9fd">
                      <td align="right">问题：</td>
                      <td colspan="2" align="left"><%=rs("Question1") %></td>
                    </tr>                   
                    <tr>
                      <td height="40" align="right">答案：</td>
                      <td width="27%" align="left"><input type="text" id="txtResponse1" name="txtResponse1" class="text" /></td>
                      <td width="47%" align="left"><span class="tipsM"></span></td>
                    </tr>
                    <tr bgcolor="#f7f9fd">
                      <td align="right">问题：</td>
                      <td colspan="2" align="left"><%=rs("Question2") %></td>
                    </tr>                   
                    <tr>
                      <td height="40" align="right">答案：</td>
                      <td width="27%" align="left"><input type="text" id="txtResponse2" name="txtResponse2" class="text" /></td>
                      <td width="47%" align="left"><span class="tipsM"></span></td>
                    </tr>
                    <tr bgcolor="#f7f9fd">
                      <td align="right">问题：</td>
                      <td colspan="2" align="left"><%=rs("Question3") %></td>
                    </tr>                   
                    <tr>
                      <td height="40" align="right">答案：</td>
                      <td width="27%" align="left"><input type="text" id="txtResponse3" name="txtResponse3" class="text" /></td>
                      <td width="47%" align="left"><span class="tipsM"></span></td>
                    </tr>                    
                    <tr>
                      <td height="40" align="right">&nbsp;</td>
                      <td colspan="2" align="left">
                        <input name="button2242" type="submit" class="bnt lan bold" value="确定" hidefocus="true"/>
                        <input name="button2432" type="reset" class="bnt lan bold" value="重置" hidefocus="true"/>
                        <input name="Confirm" type="hidden" value="true" />
                      </td>
                      </tr>
                  </table>
                  </form>
                  <% 
                        End If
                    End Sub
                        
                    Sub Step2()
                  %>
	              <!--第一步结束-->
	              <!--第二步开始-->
	              <form id="form2" name="form1" method="post" action="?action=step2">
                  <table width="100%" border="0" cellspacing="0" cellpadding="0">  
                    <tr class="bold">
                      <td width="26%" align="right">第二步：</td>
                      <td colspan="2" align="left">请填写新的帐号保护信息</td>
                    </tr>                       
                    <tr bgcolor="#F7F9FD">
                      <td width="180" align="right">问题一：</td>
                      <td width="180">
                        <select id="ddlQuestion1" name="ddlQuestion1">
                        <option value="0">请选择密保问题</option>
                        <option value="您母亲的姓名是？">您母亲的姓名是？</option>
                        <option value="您父亲的姓名是？">您父亲的姓名是？</option>
                        <option value="您配偶的名字是？">您配偶的名字是？</option>
                        <option value="您的出生地是？">您的出生地是？</option>
                        <option value="您的高中班主任是？">您的高中班主任是？</option>
                        <option value="您的初中班主任是？">您的初中班主任是？</option>
                        <option value="您的小学班主任是？">您的小学班主任是？</option>
                        <option value="您的小学校名是？">您的小学校名是？</option>
                        <option value="您的学号（或工号）是？">您的学号（或工号）是？</option>
                        <option value="您母亲的生日是？">您母亲的生日是？</option>
                        <option value="您父亲的生日是？">您父亲的生日是？</option>
                        <option value="您配偶的生日是？">您配偶的生日是？</option>
                        </select>
                      </td>
                      <td><span id="ddlQuestion1Tip"></span></td>
                    </tr>
                    <tr>
                      <td align="right">答案：</td>
                      <td><input type="text" id="txtMResponse1" name="txtMResponse1" class="text" style="width:162px;" /></td>
                      <td><span id="txtMResponse1Tip"></span></td>
                    </tr>
                    <tr bgcolor="#F7F9FD">
                      <td align="right">问题二：</td>
                      <td>
                        <select id="ddlQuestion2" name="ddlQuestion2">
                        <option value="0">请选择密保问题</option>
                        <option value="您母亲的姓名是？">您母亲的姓名是？</option>
                        <option value="您父亲的姓名是？">您父亲的姓名是？</option>
                        <option value="您配偶的名字是？">您配偶的名字是？</option>
                        <option value="您的出生地是？">您的出生地是？</option>
                        <option value="您的高中班主任是？">您的高中班主任是？</option>
                        <option value="您的初中班主任是？">您的初中班主任是？</option>
                        <option value="您的小学班主任是？">您的小学班主任是？</option>
                        <option value="您的小学校名是？">您的小学校名是？</option>
                        <option value="您的学号（或工号）是？">您的学号（或工号）是？</option>
                        <option value="您母亲的生日是？">您母亲的生日是？</option>
                        <option value="您父亲的生日是？">您父亲的生日是？</option>
                        <option value="您配偶的生日是？">您配偶的生日是？</option>
                        </select>
                      </td>
                      <td><span id="ddlQuestion2Tip"></span></td>
                    </tr>
                    <tr>
                      <td align="right">答案：</td>
                      <td><input type="text" id="txtMResponse2" name="txtMResponse2" class="text" style="width:162px;" /></td>
                      <td><span id="txtMResponse2Tip"></span></td>
                    </tr>
                    <tr bgcolor="#F7F9FD">
                      <td align="right">问题三：</td>
                      <td>
                        <select id="ddlQuestion3" name="ddlQuestion3">
                        <option value="0">请选择密保问题</option>
                        <option value="您母亲的姓名是？">您母亲的姓名是？</option>
                        <option value="您父亲的姓名是？">您父亲的姓名是？</option>
                        <option value="您配偶的名字是？">您配偶的名字是？</option>
                        <option value="您的出生地是？">您的出生地是？</option>
                        <option value="您的高中班主任是？">您的高中班主任是？</option>
                        <option value="您的初中班主任是？">您的初中班主任是？</option>
                        <option value="您的小学班主任是？">您的小学班主任是？</option>
                        <option value="您的小学校名是？">您的小学校名是？</option>
                        <option value="您的学号（或工号）是？">您的学号（或工号）是？</option>
                        <option value="您母亲的生日是？">您母亲的生日是？</option>
                        <option value="您父亲的生日是？">您父亲的生日是？</option>
                        <option value="您配偶的生日是？">您配偶的生日是？</option>
                        </select>
                      </td>
                      <td><span id="ddlQuestion3Tip"></span></td>
                    </tr>
                    <tr>
                      <td align="right">答案：</td>
                      <td><input type="text" id="txtMResponse3" name="txtMResponse3" class="text" style="width:162px;" /></td>
                      <td><span id="txtMResponse3Tip"></span></td>
                    </tr>                    
                    <tr>
                      <td align="right">电子邮箱：</td>
                      <td><input type="text" id="txtSafeEmail" name="txtSafeEmail" class="text" style="width:162px;" /></td>
                      <td><span id="txtSafeEmailTip"></span></td>
                    </tr>
                    <tr bgcolor="#F7F9FD">
                      <td align="right">确认邮箱：</td>
                      <td><input type="text" id="txtSafeEmail2" name="txtSafeEmail2" class="text" style="width:162px;" /></td>
                      <td><span id="txtSafeEmail2Tip"></span></td>
                    </tr>
                    <tr>
                      <td>&nbsp;</td>
                      <td>
                        <input name="button224" type="submit" class="bnt lan bold" value="确定" hidefocus="true"/>
                        <input name="button243" type="reset" class="bnt lan bold" value="重置" hidefocus="true"/>
                        <input name="Modify" type="hidden" value="true" />
                      </td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                  <% CxGame.AccountsProtect()%>
                  </form>
                  <% 
                        End Sub
                  %>
	              <!--第二步结束-->
                <!--修改密码保护结束-->
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
