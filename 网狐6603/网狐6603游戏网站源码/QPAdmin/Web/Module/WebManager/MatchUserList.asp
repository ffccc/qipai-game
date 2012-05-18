<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../Conn.asp"-->
<!--#include file="../../md5.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Calendar.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/Sort.js"></script>
</head>

	
<body>
   
     <!-- 头部菜单 Start -->
     
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：管理中心 - 比赛管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->  
    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
        <tr>
            <td height="28">
                <ul>
                    <li class="tab2" onclick="openWindowSelf('MatchList.asp')" >赛事管理</li>
	                <li class="tab1">用户管理</li>
	             
                </ul>
            </td>
        </tr>
    </table>  
    <form name="form2" action="">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    普通查询：
                </td>
                <td>
                    <select name="swhat" class="button">
                         <% 
                            Dim ArrayKind,n
                            ArrayKind = CxGame.GetMatchList()
                            For n=0 To Ubound(ArrayKind)                                                         
                        %>
                        <option value="<%=ArrayKind(n,0) %>"><%=ArrayKind(n,1) %></option>
                        <%                      
                            Next  
                            Set ArrayKind = nothing
                        %>
                    </select>
                    <input type="submit" value="查询" class="btn wd1" />
                </td>
            </tr>
        </table>
    </form>
     <%
     
	    Call ConnectGame("QPNativeWebDB")
        Select case lcase(Request("action"))
            case "save"
            Call Save()
            case else
            Call Main()
        End Select
        
        
       
        '冻结解冻
        Sub Nullity(lID,typeValue)
            Dim ID
            ID = lID
            GameConn.execute "Update GameMatchUserInfo Set Nullity="&typeValue&" Where UserID="&lID
        End Sub
        
        
        Sub Main()
            Dim cidArray, lLoop,acType,queryCondition,Fields
            cidArray = Split(Request("cid"),",")
            acType = Request("action")
            For lLoop=0 To UBound(cidArray)
                Select case acType
                    case "del"
                        Call DeleteInfo(cidArray(lLoop))
                    case "jinyong"
                        Call Nullity(cidArray(lLoop),1)
                    case "huanyuan"
                        Call Nullity(cidArray(lLoop),0)
                End Select
            Next            
            
            If Request("orders")<>""And Request("OrderType")<>"" Then
                If Request("OrderType")<>"-102" Then
                    OrderStr=Replace(Request("orders")," ","+")&" Asc "
                End If
                If Request.QueryString("OrderType")<>"-101" Then
                    OrderStr=Replace(Request("orders")," ","+")&" Desc "
                End If
            Else 
                OrderStr = "CollectDate Asc"
            End If   
            
            If Request("swhat")<>"" Then
                queryCondition=" MatchID="&Request("swhat")
            End If
            Fields="*"
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = 20				'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="UserID"					'主键
	            .Field=Fields	'字段
	            .Table="GameMatchUserInfo"			'表名
	            .Condition=queryCondition		'条件,不需要where
	            .OrderBy=OrderStr				'排序,不需要order by,需要asc或者desc
	            .RecordCount = 0				'总记录数，可以外部赋值，0不保存（适合搜索），-1存为session，-2存为cookies，-3存为applacation

	            .NumericJump = 5                '数字上下页个数，可选参数，默认为3，负数为跳转个数，0为显示所有
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
    <script type="text/javascript">
       function Operate(opType)
        {
            var opVal = document.form1.in_optype
            if(!confirm("确定要执行选定的操作吗？"))
            {
                return;
            }
            else
            {
                var cid="";
                var cids = GelTags("input");                
                for(var i=0;i<cids.length;i++)
                {
                    if(cids[i].checked)
                    {
                        if(cids[i].name == "cid")
                            cid+=cids[i].value+",";
                    }
                }       
                if(cid=="")
                {
                    showError("未选中任何数据");
                    return;
                }
                //操作处理
                if(opType=="del")
                    opVal.value = "del";
                if(opType=="jinyong")
                    opVal.value = "jinyong";
                if(opType=="huanyuan")
                    opVal.value = "huanyuan";
            }
            document.form1.action = "?action="+opVal.value;
            document.form1.submit();
        }
    </script>
    
    <form id="form1" name="form1" method="post" action="?action=orderinfo">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="冻结" class="btn wd1" onclick="Operate('jinyong')"/>
                    <input class="btnLine" type="button" />
                    <input type="button" value="解冻" class="btn wd1" onclick="Operate('huanyuan')"/>
                    <input name="in_optype" type="hidden" />      
                </td>    
            </tr>
        </table> 
        <div id="content">        
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <td class="listTitle1" style="width:38px;"><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></td>
                    <th class="listTitle1" style="width:100px;"><a class="l1"  href="" onclick="GetOrder('MatchID',this)">赛事标识</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('UserID',this)">用户标识</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('Accounts',this)">用户帐号</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('GameID',this)">游戏ID</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('Compellation',this)">姓名</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('Gender',this)">性别</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('PassportID',this)">身份证</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('MobilePhone',this)">手机</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('EMail',this)">电子邮箱</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('QQ',this)">QQ</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('DwellingPlace',this)">详细地址</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('PostalCode',this)">邮编</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('Nullity',this)">冻结状态</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('ClientIP',this)">报名地址</a></th>
                    <th class="listTitle" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('CollectDate',this)">收集日期</a></th>
                </tr>
                <% 
                    Dim rs,sql,id,className,rowNum,OrderStr
                   
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else                    
                    For i=0 To Ubound(rs,2)    
                    rowNum = rowNum+1
                    If rowNum Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If        
                %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">      
                    <td><input name='cid' type='checkbox' value='<%=rs(1,i)%>'/></td>   
                    <td><%=CxGame.GetMatchTitle(rs(0,i)) %></td>    
                    <td><%=rs(1,i) %></td>    
                    <td><%=rs(2,i) %></td>   
                    <td><%=rs(3,i) %></td>    
                    <td><%=rs(4,i) %></td>    
                    <td>
                        <% If rs(5,i)=0 Then%>
                        女                        
                        <% Else%>
                        男
                        <% End If %>
                    </td>
                     <td><%=rs(6,i) %></td>    
                    <td><%=rs(7,i) %></td>    
                    <td><%=rs(8,i) %></td>    
                    <td><%=rs(9,i) %></td>    
                    <td><%=rs(10,i) %></td> 
                    <td><%=rs(11,i) %></td>   
                    <td>
                        <% If rs(12,i)=0 Then%>
                        否                        
                        <% Else%>
                        <span style="color:red">是</span>
                        <% End If %>
                    </td>
                     <td><%=rs(13,i) %></td> 
                     <td><%=rs(14,i) %></td>  
                </tr>
                <% 
                    Next
                    End If
                  
                   
                %>
            </table>
        </div> 
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="listTitleBg"><span>选择：</span>&nbsp;<a class="l1" href="javascript:SelectAll(true);">全部</a>&nbsp;-&nbsp;<a class="l1" href="javascript:SelectAll(false);">无</a></td>
                <td class="page" align="right"><%Response.Write nav%></td>
            </tr>
        </table> 
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" id="OpList">
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="冻结" class="btn wd1" onclick="Operate('jinyong')"/>
                    <input class="btnLine" type="button" />
                    <input type="button" value="解冻" class="btn wd1" onclick="Operate('huanyuan')"/>
                </td>
            </tr>
        </table>        
    </form>
    <%
        End Sub
    %> 
</body>
</html>
