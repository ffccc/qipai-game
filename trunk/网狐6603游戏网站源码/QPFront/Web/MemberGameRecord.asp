<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/Conn.asp" -->
<!--#include file="Inc/Cls_Page.asp" -->

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
    <script src="JS/Calendar.js" type="text/javascript"></script>   
    
    <script type="text/javascript">
        function QueryInfo()
        {            
            var startDate = document.form1.StartDate.value;
            var endDate = document.form1.EndDate.value;
            if(!isDate(startDate)||!isDate(endDate))
            {
                alert("日期格式不正确！");
                return false;
            }
        }
        
        function showInfo(obj)
        {
            var ID = "DrawID"+obj.id;
            if(document.getElementById(ID).style.display == "none")
            {
                document.getElementById(ID).style.display = "";
                obj.src = "/Images/up1.gif"
            }  
            else
            {
                document.getElementById(ID).style.display="none";
                obj.src = "/Images/down1.gif"
            }
        }
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
				<div class="userRigthTitle">会员中心 - 会员服务 - 游戏记录</div>
				<div class="userRigthBg">
				<!--存款开始-->
	            <div class="title f14 bold lan">游戏记录</div>
				  	<br />
				  	<form name="form1" action="" method="get" onsubmit="return QueryInfo()">
				  	<table width="100%" border="0" cellspacing="0" cellpadding="0">
                      <tr>
                        <td align="right" style="width:60px;">日期查询：</td>
                        <td align="left">
                            <% If CxGame.GetInfo(0,"StartDate")<>"" Then %>
                            <input name="StartDate" type="text" class="text" style="width:70px;" value="<%=CxGame.GetInfo(0,"StartDate") %>" maxlength="10" /><img src="Images/btn_calendar.gif" onclick="calendar(StartDate)" style="cursor:pointer;vertical-align:text-bottom"/>                    
                            <% Else %>
                            <input name="StartDate" type="text" class="text" style="width:70px;" value="<%=Year(Date())&"-"&Month(Date())&"-1" %>" maxlength="10" /><img src="Images/btn_calendar.gif" onclick="calendar(StartDate)" style="cursor:pointer;vertical-align:text-bottom"/>  
                            <% End If %>
                            至
                            <% If CxGame.GetInfo(0,"EndDate")<>"" Then %>
                            <input name="EndDate" type="text" class="text" style="width:70px;" value="<%=CxGame.GetInfo(0,"EndDate") %>" maxlength="10" /><img src="Images/btn_calendar.gif" onclick="calendar(EndDate)" style="cursor:pointer;vertical-align:text-bottom"/>                    
                            <% Else%>
                            <input name="EndDate" type="text" class="text" style="width:70px;" value="<%=Date() %>"  maxlength="10"/><img src="Images/btn_calendar.gif" onclick="calendar(EndDate)" style="cursor:pointer;vertical-align:text-bottom"/>                    
                            <% End If %>                            
                            <input type="submit" value="查询" class="bnt" />
                            <input type="hidden" name="QueryType" />     
                        </td>
                      </tr>
                    </table>
                    </form>
                    <% 
                        Call ConnectGame(QPTreasureDB)
                        Call Main()
                        Call CloseGame()
                        
                        Sub Main()
                            Dim rs,nav,Page,i
                            Dim lCount, queryCondition, OrderStr
                            Dim startDate,endDate
                            OrderStr = "InsertTime DESC"                           
                            queryCondition = "UserID="&Session("UserID")&" And DrawID=0"
                            '查询条件
                            startDate = CxGame.GetInfo(0,"StartDate")&" 00:00:00"
                            endDate = CxGame.GetInfo(0,"EndDate")&" 23:59:59"
                            
                            queryCondition = "DrawID in (select DrawID from RecordDrawScore where UserID = "&Session("UserID")&") And (InsertTime between '"&startDate&"' And '"&endDate&"')"
                            
                            Dim fields
                            fields = "DrawID,Waste,Revenue,Present,UserCount,StartTime,ConcludeTime,InsertTime"    
                            
                            '==============================================================================================================
                            '执行存储过程
                                    
                            Set Page = new Cls_Page      '创建对象
                            Set Page.Conn = GameConn     '得到数据库连接对象        
                            With Page
                                .PageSize = 20					'每页记录条数
                                .PageParm = "page"				'页参数
                                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	                            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	                            .Pkey="DrawID"					'主键
	                            .Field=fields	'字段
                	            
	                            .Table="RecordDrawInfo"		    '表名
	                            .Condition=queryCondition		'条件,不需要where
	                            .OrderBy=OrderStr				'排序,不需要order by,需要asc或者desc
	                            .RecordCount = 0				'总记录数，可以外部赋值，0不保存（适合搜索），-1存为session，-2存为cookies，-3存为applacation

	                            .NumericJump = 1                '数字上下页个数，可选参数，默认为3，负数为跳转个数，0为显示所有
	                            .Template = "总记录数：{$RecordCount} 总页数：{$PageCount} 每页记录数：{$PageSize} 当前页数：{$PageIndex} {$FirstPage} {$PreviousPage} {$NumericPage} {$NextPage} {$LastPage} {$InputPage} {$SelectPage}" '整体模板，可选参数，有默认值
	                            .FirstPage = "首页"             '可选参数，有默认值
	                            .PreviousPage = "上一页"        '可选参数，有默认值
	                            .NextPage = "下一页"            '可选参数，有默认值
	                            .LastPage = "尾页"              '可选参数，有默认值
	                            .NumericPage = " {$PageNum} "   '数字分页部分模板，可选参数，有默认值
                            End With
                            
                            rs = Page.ResultSet()       '记录集
                            lCount = Page.RowCount()    '可选，输出总记录数
                            nav = Page.Nav()            '分页样式
                            
                            Page = Null
                            Set Page = Nothing
                            '==============================================================================================================
                    %>
                    <form name="myformList" method="post" action="">
			  	    <table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#BAD7E5" class="leftTop">
                      <tr align="center" class="bold">
                        <td class="leftTop trBg">开始时间</td>
                        <td class="leftTop trBg">结束时间</td>
                        <td class="leftTop trBg">损耗</td>
                        <td class="leftTop trBg">税收</td>
                        <td class="leftTop trBg">赠送</td>  
                        <td class="leftTop trBg">用户数</td>                              
                      </tr>   
                      <% 
                        Dim className
                        If IsNull(rs) Then
                            Response.Write("<tr><td bgcolor='#FFFFFF' colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                        Else
                        
                        For i=0 To Ubound(rs,2)
                        If i Mod 2 = 0 Then
                            className="trBg1"        
                        Else
                            className="trBg2"      
                        End If 
                    %>
                    <tr align="center" class="<%=className %>">
                        <td><%=rs(5,i) %>&nbsp;<img id="<%=rs(0,i) %>" src="/Images/down1.gif" style="cursor:pointer" onclick="showInfo(this)" /></td>  
                        <td><%=rs(6,i) %></td>  
                        <td><%=rs(1,i) %></td>  
                        <td><%=rs(2,i) %></td>  
                        <td><%=rs(3,i) %></td>  
                        <td><%=rs(4,i) %></td>  
                    </tr>
                    <tr style="display:none" id="DrawID<%=rs(0,i) %>">
                    <td colspan="100"> 
                    <table width="100%" border="0" cellpadding="0" cellspacing="0" bgcolor="#BAD7E5" class="leftTop">
                        <tr align="center" class="bold">
                            <td class="leftTop trBg">用户名</td>
                            <td class="leftTop trBg">输赢积分</td>
                            <td class="leftTop trBg">税收</td>
                            <td class="leftTop trBg">赠送</td>
                        </tr>         
                        <% 
                            Call ConnectGame(QPTreasureDB)        
                            Dim rs_d,sql
                            Set rs_d=Server.CreateObject("Adodb.RecordSet")
                            sql = "select * from RecordDrawScore(nolock) where DrawID="&rs(0,i)&" and UserID="&Session("UserID")
                            sql = sql&" union select * from RecordDrawScore(nolock) where DrawID="&rs(0,i)&" and UserID<>"&Session("UserID")
                            rs_d.Open sql,GameConn,1,3
                            do until rs_d.eof        
                        %>
                            <tr class="trBg1" align="center">
                                <td><%=CxGame.GetUserNameByUserID(rs_d("UserID")) %></td>
                                <td><%=rs_d("Score") %></td>
                                <td><%=rs_d("Revenue") %></td>
                                <td><%=rs_d("Present") %></td>                                
                            </tr> 
                        <% 
                            rs_d.movenext
                            loop
                            Call CloseGame()
                        %>
                                </table>
                            </td> 
                        </tr>   
                    <% 
                        Next
                        End If
                    %>    
                    </table>
                    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                        <tr>
                            <td class="page2" align="right"><%Response.Write nav%></td>
                        </tr>
                    </table>  
                    </form>
                    <% 
                        End Sub
                    %>
	            <!--存款结束-->
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
