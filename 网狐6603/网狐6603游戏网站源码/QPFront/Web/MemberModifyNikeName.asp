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
    <script type="text/javascript">
	    $(document).ready(function(){
            $("#form1").submit(function(){
                if($.trim($("#txtNikeName").val())=="")
                {
                    alert("昵称不能为空！");
                    return false;
                }
                else
                {
                    if(!confirm("您确定要修改昵称吗？"))
                    {
                        return false;
                    }
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
		<div class="mainRight2">
			<div class="userRigthBody">
				<div class="userRigthTitle">会员中心 - 帐号服务 - 修改昵称</div>
				<div class="userRigthBg">
				    <!--修改资料开始-->				    
				    <form id="form1" name="form1" method="post" action="MemberModifyNikeName.asp">
	                <table width="100%" border="0" cellspacing="0" cellpadding="0">
                      <tr>
                        <td align="right">当前昵称：</td>
                        <td class="c"><%=Session("NikeName") %></td>
                      </tr>
                      <tr>
                        <td align="right">ID 号码：</td>
                        <td><%=Session("GameID") %></td>
                      </tr>
                      <tr>
                        <td align="right">新昵称：</td>
                        <td><input type="text" id="txtNikeName" name="txtNikeName" class="text width200" /></td>
                      </tr>                      
                      <tr>
                        <td>&nbsp;</td>
                        <td>
                            <input name="button2" type="submit" class="bnt lan bold" value="确定" hidefocus="true"/>
                            <input type="hidden" name="Modify" value="true" />
                        </td>
                      </tr>                      
                    </table>   
                    <%CxGame.ModifyNikeName() %>                 
                    </form>
	                <!--修改资料结束-->
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
