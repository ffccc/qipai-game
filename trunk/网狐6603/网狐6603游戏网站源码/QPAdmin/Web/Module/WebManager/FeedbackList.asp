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
            <td width="1232" height="25" valign="top" align="left">你当前位置：管理中心 - 回馈管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->  
  
     <%
     
	    Call ConnectGame("QPNativeWebDB")
        Select case lcase(Request("action"))
            case "save"
            Call Save()
            case "newinfo"
            Call NewInfo()
            case else
            Call Main()
        End Select
        
       '删除
        Sub DeleteInfo(lID)
            Dim ID
            ID = lID
            GameConn.execute "Delete GameFeedbackInfo Where FeedbackID="&lID
            Call CxGame.ShowInfo2("删除成功！！","FeedbackList.asp",1200)
        End Sub
       
        '冻结解冻
        Sub Nullity(lID,typeValue)
            Dim ID
            ID = lID
            GameConn.execute "Update GameFeedbackInfo Set Nullity="&typeValue&" Where FeedbackID="&lID
        End Sub
        
        '新增更新
	    Sub Save()
	        If Request("FeedbackID")<>"" Then
	            GameConn.execute "update GameFeedbackInfo set RevertContent='"&Request("content1")&"',RevertDate='"&Now()&"',RevertUserID='"&trim(session("AdminID"))&"' Where FeedbackID="&Request("FeedbackID")
	            Call CxGame.ShowInfo2("回复成功！","FeedbackList.asp",1200)
	        End If
        End Sub
        
        Sub Main()
            Dim cidArray, lLoop,acType,Fields
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
                OrderStr = "FeedbackID Asc"
            End If
            
            Fields=" FeedbackID,FeedbackTitle,Accounts,FeedbackDate,RevertUserID,RevertDate,Nullity "
               
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = 20				'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="IssueID"					'主键
	            .Field=Fields	'字段
	            .Table="GameFeedbackInfo"			'表名
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
                    <input type="button" value="删除" class="btn wd1" onclick="Operate('del')" />
                    <input type="button" value="冻结" class="btn wd1" onclick="Operate('jinyong')"/>
                    <input type="button" value="解冻" class="btn wd1" onclick="Operate('huanyuan')"/>
                    <input name="in_optype" type="hidden" />      
                </td>    
            </tr>
        </table> 
        <div id="content">        
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <td class="listTitle1"><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></td>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('FeedbackID',this)">反馈标识</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('FeedbackTitle',this)">反馈标题</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Accounts',this)">游戏帐号</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('FeedbackDate',this)">提交时间</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('RevertUserID',this)">回复人</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('RevertDate',this)">回复日期</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('Nullity',this)">冻结标志</a></th>
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
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='pointer';"
                    onmouseout="this.style.backgroundColor=currentcolor">      
                    <td><input name='cid' type='checkbox' value='<%=rs(0,i)%>'/></td>   
                    <td onclick="Redirect('FeedbackList.asp?action=newinfo&FeedbackID=<%=rs(0,i) %>')"><%=rs(0,i) %></td>    
                    <td onclick="Redirect('FeedbackList.asp?action=newinfo&FeedbackID=<%=rs(0,i) %>')"><%=rs(1,i) %></td>    
                    <td onclick="Redirect('FeedbackList.asp?action=newinfo&FeedbackID=<%=rs(0,i) %>')"><%=rs(2,i) %></td>    
                    <td onclick="Redirect('FeedbackList.asp?action=newinfo&FeedbackID=<%=rs(0,i) %>')"><%=rs(3,i) %></td>    
                    <td onclick="Redirect('FeedbackList.asp?action=newinfo&FeedbackID=<%=rs(0,i) %>')"><%=CxGame.GetAdmin(rs(4,i)) %></td> 
                    <td onclick="Redirect('FeedbackList.asp?action=newinfo&FeedbackID=<%=rs(0,i) %>')"><%=rs(5,i) %></td> 
                    <td>
                        <% If rs(6,i)=0 Then%>
                        否                        
                        <% Else%>
                        <span style="color:red">是</span>
                        <% End If %>
                    </td>
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
                   <input type="button" value="删除" class="btn wd1" onclick="Operate('del')" />
                    <input type="button" value="冻结" class="btn wd1" onclick="Operate('jinyong')"/>
                    <input type="button" value="解冻" class="btn wd1" onclick="Operate('huanyuan')"/>
                </td>
            </tr>
        </table>        
    </form>
    <%
        End Sub
        
        Sub NewInfo()
            Dim rs,sql,FeedbackTitle,RevertContent
            FeedbackID=Request("FeedbackID")
            If(Request("FeedbackID")<>"") Then
                Set rs=server.createobject("adodb.recordset")
                sql="select * from GameFeedbackInfo where FeedbackID="&Request("FeedbackID")
                rs.open sql,GameConn,1,3
                FeedbackTitle=trim(rs("FeedbackTitle"))
                RevertContent=trim(rs("RevertContent"))
            Else
                FeedbackTitle=""
                RevertContent=""
            End If
    %> 
    <script type="text/javascript" src="../../kindeditor/kindeditor.js"></script>
    <script type="text/javascript">
		
		
		function CheckFormInfo()
		{
		    var content1 = document.form1.content1.value.trim();
		    if(content1=="")
		    {
		        alert("回复内容不能为空！");
		        return false;
		    }
		}
	</script>
    <form name="form1" method="post" action="?action=save&FeedbackID=<%=FeedbackID %>"   onsubmit="return CheckFormInfo();">
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
             <tr>
                 <td height="39" class="titleOpBg">
                     <input type="submit" value="保存" class="btn wd1" /><input type="hidden" name="in_FeedbackID" value="<%=FeedbackID%>"/>
                     <input class="btnLine" type="button" />
                     <input type="button" value="返回" class="btn wd1" onclick="Redirect('FeedbackList.asp')" />
                 </td>
             </tr>
         </table>  
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">回馈管理</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">回馈标题：</td>
                <td>
                   <%=FeedbackTitle %>
                </td>
            </tr>
             <tr>
                <td class="listTdLeft">回复内容：</td>
                <td>
                    <textarea id="content1"  name="content1" cols="100" rows="8" style="width:700px;height:120px;"><%=RevertContent %></textarea> 
                </td>
            </tr>
            <tr>
                <td style="height:10px">
                   
                </td>
            </tr>
            
         </table>
        
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
             <tr>
                 <td height="39" class="titleOpBg">
                     <input type="submit" value="保存" class="btn wd1" />
                     <input class="btnLine" type="button" />
                     <input type="button" value="返回" class="btn wd1" onclick="Redirect('FeedbackList.asp')" />
                 </td>
             </tr>
         </table>
     </form> 
     <%
        End Sub
      %>
</body>
</html>
