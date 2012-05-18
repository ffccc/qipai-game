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
    <link rel="stylesheet" type="text/css" href="Css/news_layout.css" />   
</head>
<body>
<%
    CxGame.Channel = "Member" 
    'CxGame.IsCheckLogin
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
				<div class="userRigthTitle">会员中心 - 推广服务 - 推广明细查询</div>
				<div class="userRigthBg">
                <!--推广明细开始-->
                <% 
                    Select case lcase(Request("action"))
                        case "info"
				        Call GetSpreadInfo()				        
				        case else
				        Call Main()
                    End Select 
                    
                    Sub Main()
                        ConnectGame(QPAccountsDB)
                        Dim rs,nav,Page,i
                        Dim lCount, queryCondition, OrderStr
                        OrderStr = "UserID ASC"
                        queryCondition = "SpreaderID="&Session("UserID")
                        
                        Dim fields
                        fields = "UserID,Accounts,GameID,RegisterDate"    
                        
                        '==============================================================================================================
                        '执行存储过程
                                
                        Set Page = new Cls_Page      '创建对象
                        Set Page.Conn = GameConn     '得到数据库连接对象        
                        With Page
                            .PageSize = 20					'每页记录条数
                            .PageParm = "page"				'页参数
                            '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
                            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
                            .Pkey="UserID"					'主键
                            .Field=fields	'字段
            	            
                            .Table="AccountsInfo"	'表名
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
                <table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#BAD7E5" class="leftTop">
                <tr align="center" class="bold">
                    <td class="leftTop trBg">帐号</td>
                    <td class="leftTop trBg">游戏ＩＤ</td>   
                    <td class="leftTop trBg">贡献金币</td> 
                    <td class="leftTop trBg">推荐时间</td>     
                    <td class="leftTop trBg">&nbsp;</td>                                                                  
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
                    <td><%=rs(1,i) %></td>
                    <td><%=rs(2,i) %></td>
                    <td><%=FormatNumber(CxGame.GetChildSpreadScore(rs(0,i)),2,-1) %></td>
                    <td><%=rs(3,i) %></td>
                    <td><a class="ll" href="?action=info&UserID=<%=rs(0,i) %>">贡献明细</a></td>
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
                <% 
                    Call CloseGame()  
                    End Sub
                    
                    Sub GetSpreadInfo()
                        ConnectGame(QPTreasureDB)
                        Dim rs,nav,Page,i
                        Dim lCount, queryCondition, OrderStr
                        OrderStr = "CollectDate DESC"                               
                        queryCondition = "ChildrenID="&CxGame.GetInfo(1,"UserID")
                        
                        Dim fields
                        fields = "RecordID,CollectDate,Score,TypeID,CollectNote"    
                        
                        '统计
                        Dim rs_1,sql,revenueSum
                        Set rs_1=Server.CreateObject("Adodb.RecordSet")
                        sql = "select Sum(Score) as ScoreSum from RecordSpreadInfo where "&queryCondition
                        rs_1.Open sql,GameConn,1,3
                            revenueSum = rs_1(0)
                        rs_1.Close
                        Set rs_1 = nothing
                            
                        '==============================================================================================================
                        '执行存储过程
                                
                        Set Page = new Cls_Page      '创建对象
                        Set Page.Conn = GameConn     '得到数据库连接对象        
                        With Page
                            .PageSize = 20					'每页记录条数
                            .PageParm = "page"				'页参数
                            '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
                            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
                            .Pkey="RecordID"					'主键
                            .Field=fields	'字段
            	            
                            .Table="RecordSpreadInfo"	'表名
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
                <div class="viewTop">被推荐人：<span class="hong"><%=CxGame.GetUserNameByUserID(CxGame.GetInfo(1,"UserID")) %></span></div>
                <div class="page"><a href="MemberSpreadInfo.asp" title="返回推广明细">返回推广明细</a></div>
		  	    <table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#BAD7E5" class="leftTop box">
                  <tr align="center" class="bold">
                    <td class="leftTop trBg">日期</td>
                    <td class="leftTop trBg">贡献金币</td>                        
                    <td class="leftTop trBg">类别</td>
                    <td class="leftTop trBg">备注</td>                                            
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
                    <td><%=rs(1,i) %></td>
                    <td><%=FormatNumber(rs(2,i)) %></td>
                    <td><%=CxGame.GetSpreadGrantType(rs(3,i)) %></td>
                    <td align="left"><%=rs(4,i) %></td>
                </tr>
                <% 
                    Next
                %>    
                <tr class="trBg1" align="center">
                    <td>合计：</td>
                    <td class="leftTop trBg"><%=FormatNumber(revenueSum) %></td>
                    <td colspan="100"></td>
                </tr>
                <% 
                    End If
                %>
                </table>
                <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td class="page2" align="right"><%Response.Write nav%></td>
                    </tr>
                </table>  
                <% 
                    Call CloseGame()  
                    End Sub
                %>
                <!--推广明细结束-->
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
