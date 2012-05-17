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
    <script src="JS/formValidator.js" type="text/javascript"></script>
    <script src="JS/formValidatorRegex.js" type="text/javascript"></script>
    
    <script type="text/javascript">
        $(document).ready(function(){
            //页面验证
            $.formValidator.initConfig({formid:"form1",alertmessage:false});
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
            $("#txtResponse1").formValidator({onshow:" ",onfocus:"答案至少2~16个英文或中文字符"}).inputValidator({min:2,max:16,onerror:"你输入的答案非法"})
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
            $("#txtResponse2").formValidator({onshow:" ",onfocus:"答案至少2~16个英文或中文字符"}).inputValidator({min:2,max:16,onerror:"你输入的答案非法"})
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
            $("#txtResponse3").formValidator({onshow:" ",onfocus:"答案至少2~16个英文或中文字符"}).inputValidator({min:2,max:16,onerror:"你输入的答案非法"})
                .regexValidator({regexp:"usernamecn",datatype:"enum",onerror:"答案格式不正确"});
            $("#ddlPassportType").formValidator({onshow:" ",onfocus:"请选择证件类型"}).inputValidator({min:1,onerror: "你是不是忘记选择证件类型了!"})      
            $("#txtPassportID").formValidator({onshow:" ",onfocus:"请输入证件号码"}).inputValidator({min:1,onerror: "证件号码不能为空"}).functionValidator({
                fun:function(val,elem){
                    if($("#ddlPassportType").val()=="1")
                    {
                        return isCardID(val);
                    }
                    else
                    {
                        return true;
                    }
	            }
            });                
            $("#txtPassportID2").formValidator({onshow:" ",onfocus:"请输入证件号码"}).inputValidator({min:1,onerror: "证件号码不能为空"})
                .compareValidator({desid:"txtPassportID",operateor:"=",onerror:"两次填写的证件号码不一致"});
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
				<div class="userRigthTitle">会员中心 - 帐号安全 - 申请密码保护</div>
				<div class="userRigthBg">
				  <!--申请密码保护开始-->	
				  <% 
				        ConnectGame(QPAccountsDB)
				        Dim rs
				        Set rs = GameConn.Execute("select * from AccountsProtect(nolock) where UserID= "&Session("UserID")&"")
				        If rs.eof Then
				  %>			  
				  <form id="form1" name="form1" method="post" action="">
                  <table width="100%" border="0" cellspacing="0" cellpadding="0">                    
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
                      <td><input type="text" id="txtResponse1" name="txtResponse1" class="text" style="width:162px;" /></td>
                      <td><span id="txtResponse1Tip"></span></td>
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
                      <td><input type="text" id="txtResponse2" name="txtResponse2" class="text" style="width:162px;" /></td>
                      <td><span id="txtResponse2Tip"></span></td>
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
                      <td><input type="text" id="txtResponse3" name="txtResponse3" class="text" style="width:162px;" /></td>
                      <td><span id="txtResponse3Tip"></span></td>
                    </tr>
                    <tr bgcolor="#F7F9FD">
                      <td align="right">证件类型：</td>
                      <td>
                        <select id="ddlPassportType" name="ddlPassportType" style="width:170px;">
                        <option value="0">请选择证件类型</option>
	                    <option value="1">身份证</option>
	                    <option value="2">学生证</option>
	                    <option value="3">军官证</option>
	                    <option value="4">驾驶证</option>
	                    <option value="5">其他</option>
                        </select>
                      </td>
                      <td><span id="ddlPassportTypeTip"></span></td>
                    </tr>
                    <tr>
                      <td align="right">证件号码：</td>
                      <td><input type="text" id="txtPassportID" name="txtPassportID" class="text" style="width:162px;"/></td>
                      <td><span id="txtPassportIDTip"></span></td>
                    </tr>
                    <tr bgcolor="#F7F9FD">
                      <td align="right">确认号码：</td>
                      <td><input type="text" id="txtPassportID2" name="txtPassportID2" class="text" style="width:162px;" /></td>
                      <td><span id="txtPassportID2Tip"></span></td>
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
                        <input name="Add" type="hidden" value="true" />
                      </td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                  <% CxGame.AccountsProtect()%>
                  </form>
                  <% 
                    Else
                  %>
                  <div class="loginHint">
				  <div class="Uright">您已经成功申请了密码保护！</div>
				  </div>
                  <%  
                    rs.Close:Set rs=Nothing:CloseGame()
                    End If                        
                  %>
	              <!--申请密码保护结束-->
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
