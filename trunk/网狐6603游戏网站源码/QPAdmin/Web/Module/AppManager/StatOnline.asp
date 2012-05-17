<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../function.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>   
</head>
<%
    If CxGame.GetRoleValue("307",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body onload="InitPage()">
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：系统维护 - 在线统计</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->    
    <script type="text/javascript">     
        var MonHead = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];   
        function InitPage()
        {           
            //绑定年
            WriteYear();
            //绑定月份
            WriteMonth();
            //绑定日
            var n = MonHead[new Date().getMonth()];
            if (new Date().getMonth() ==1 && IsPinYear(YYYYvalue)) n++;
            WriteDay(n);    
            
            if(<%=CxGame.GetInfo(1,"selectType") %> == 1)
            {
                document.form1.selectMonth.style.display = "";
            }
            if(<%=CxGame.GetInfo(1,"selectType") %> == 2)
            {
                document.form1.selectMonth.style.display = "";
                document.form1.selectDay.style.display = "";
            }
        }
        
        //绑定年下拉框
        function WriteYear()
        {
            var selectYear = document.form1.selectYear.outerHTML
            var y = new Date().getFullYear();
            var str = selectYear.substring(0, selectYear.length - 9);
            for (var i = (y-5); i < (y+1); i++) //以今年为准，前5年
            {
                str += "<option value='" + i + "'> " + i + " 年" + "</option>\r\n";                    
            }
            document.form1.selectYear.outerHTML = str +"</select>";
            
            if(<%=CxGame.GetInfo(1,"selectYear") %>==0)
                document.form1.selectYear.value = y;
            else
                document.form1.selectYear.value = <%=CxGame.GetInfo(1,"selectYear") %>;
        }
        
        //绑定月下拉框
        function WriteMonth()
        {
            var selectMonth = document.form1.selectMonth.outerHTML;
            var str = selectMonth.substring(0, selectMonth.length - 9);
            for (var i = 1; i < 13; i++)
            {
                str += "<option value='" + i + "'> " + i + " 月" + "</option>\r\n";
            }
            document.form1.selectMonth.outerHTML = str +"</select>";
            
            if(<%=CxGame.GetInfo(1,"selectMonth") %>==0)
                document.form1.selectMonth.value = new Date().getMonth() + 1;
            else
                document.form1.selectMonth.value = <%=CxGame.GetInfo(1,"selectMonth") %>;
        }
        
        //绑定日下拉框
        function WriteDay(n)
        {
            document.getElementById("selectDay").length=0;
            var selectDay = document.form1.selectDay.outerHTML;
            var str = selectDay.substring(0, selectDay.length - 9);
            for(var i=1;i<(n+1);i++)
            {
                str += "<option value='" + i + "'> " + i + " 日" + "</option>\r\n";
            }
            document.form1.selectDay.outerHTML = str +"</select>";
            
            if(<%=CxGame.GetInfo(1,"selectDay") %>==0)
                document.form1.selectDay.value = new Date().getDate();    
            else
                document.form1.selectDay.value = <%=CxGame.GetInfo(1,"selectDay") %>;  
        }

        //判断是否闰平年
        function IsPinYear(year)
        {
            return (0 == year%4 && (year%100 !=0 || year%400 ==0))
        } 
        
        function ChangeType(str)
        {
            var selectMonth = document.form1.selectMonth;
            var selectDay = document.form1.selectDay;
            if(str==0)
            {
                selectMonth.style.display="none";
                selectDay.style.display="none";
            }
            if(str==1)
            {
                selectMonth.style.display="";
                selectDay.style.display="none";
            }
            if(str==2)
            {
                selectMonth.style.display="";
                selectDay.style.display="";
            }
        }
        
        function ChangeYear(str)
        {
            var month = document.form1.selectMonth.value;
            var n = MonHead[month-1]
            if(month==2&&IsPinYear(str)) n++;
            WriteDay(n);
        }    
        
        function ChangeMonth(str)
        {
            var year = document.form1.selectYear.value;
            var n = MonHead[str-1]
            if(str==2 && IsPinYear(year)) n++;
            WriteDay(n);
        }  
        
        function QueryInfo(opType)
        {
            var selectType = document.form1.selectType.value;
            var selectYear = document.form1.selectYear.value;
            var selectMonth = document.form1.selectMonth.value;
            var selectDay = document.form1.selectDay.value;
            var url="?action=query&QueryType="+opType;            
            document.form1.action=url;
            document.form1.submit();
        }
    </script>
    <% 
        Call ConnectGame(QPPlatformDB)
        Select case lcase(Request("action"))
            case "query"
            Main()
            Call QueryInfo()
            case else
            Call Main()
        End Select
        Call CloseGame()
        
        Sub Main()                 
    %>
    <form name="form1" action="" method="post">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7 titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    查询方式：
                </td>
                <td>
                    <select name="selectType" onchange="ChangeType(this.value)">
                        <option value="0"<% If CxGame.GetInfo(1,"selectType") = 0 Then %> selected="selected" <% End If %>>按年</option>
                        <option value="1"<% If CxGame.GetInfo(1,"selectType") = 1 Then %> selected="selected" <% End If %>>按月</option>
                        <option value="2"<% If CxGame.GetInfo(1,"selectType") = 2 Then %> selected="selected" <% End If %>>按日</option>
                    </select>
                    <select name="selectYear" id="selectYear" onchange="ChangeYear(this.value)">         
                    </select><select name="selectMonth" id="selectMonth" style="display:none;" onchange="ChangeMonth(this.value)">                        
                    </select><select name="selectDay" id="selectDay" style="display:none;">                        
                    </select>
                    <input type="button" value="查询" class="btn wd1" onclick="QueryInfo('1')" />  
                    <input type="button" value="今天" class="btn wd1" onclick="QueryInfo('2')" />              
                    <input type="button" value="昨天" class="btn wd1" onclick="QueryInfo('3')" />              
                    <input type="button" value="前天" class="btn wd1" onclick="QueryInfo('4')" />                          
                </td>
            </tr>
        </table>        
    </form>
    <%  
        End Sub
        
        Sub QueryInfo()
            Dim rs,yearStr,monthStr,dayStr,queryType,selectType
            yearStr = CxGame.GetInfo(1,"SelectYear")
            monthStr = CxGame.GetInfo(1,"SelectMonth")
            dayStr = CxGame.GetInfo(1,"SelectDay")
            If monthStr<10 Then
                monthStr = "0"&monthStr
            End If
            If dayStr <10 Then
                dayStr = "0"&dayStr
            End If           
            
            queryType = CxGame.GetInfo(1,"QueryType")      
            Select case queryType
                case 1 '直接查询
                    selectType = CxGame.GetInfo(1,"selectType")
                    If selectType = 0 Then
                        monthStr="-1"
                        dayStr = "-1"
                    Elseif selectType=1 Then
                        dayStr = "-1"
                    End If
                case 2 '今天
                    yearStr = Year(Date())
                    monthStr = Month(Date())
                    dayStr = Day(Date())
                    If monthStr<10 Then
                        monthStr = "0"&monthStr
                    End If                    
                    If dayStr<10 Then
                        dayStr = "0"&dayStr
                    End If
                case 3 '昨天
                    yearStr = Year(DateAdd("d",-1,Date()))
                    monthStr = Month(DateAdd("d",-1,Date()))
                    dayStr = Day(DateAdd("d",-1,Date()))
                    If monthStr<10 Then
                        monthStr = "0"&monthStr
                    End If                    
                    If dayStr<10 Then
                        dayStr = "0"&dayStr
                    End If
                case 4 '前天
                    yearStr = Year(DateAdd("d",-2,Date()))
                    monthStr = Month(DateAdd("d",-2,Date()))
                    dayStr = Day(DateAdd("d",-2,Date()))
                    If monthStr<10 Then
                        monthStr = "0"&monthStr
                    End If                    
                    If dayStr<10 Then
                        dayStr = "0"&dayStr
                    End If
                case else '传日期参数查询
                    Dim timeStr,timeArray
                    timeStr = CxGame.GetInfo(0,"Time")    
                    timeArray =  Split(timeStr,"-")
                    If UBound(timeArray) = 1 Then
                        yearStr = timeArray(0)
                        monthStr = timeArray(1)
                        dayStr = "-1"
                    Else
                        yearStr = timeArray(0)
                        monthStr = timeArray(1)
                        dayStr = timeArray(2)
                    End If
            End Select
            
            Dim Array
            Array = Null
            Set rs = GameConn.execute("WSP_PM_StatOnLineStream '"&yearStr&"','"&monthStr&"','"&dayStr&"'")
            If Not rs.eof Then
            Array = rs.GetRows()
            End If
    %>
    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box Tmg7">
        <tr align="center" class="bold">
            <th class="listTitle1">统计时间</th>
            <th class="listTitle2">最高在线人数</th>
            <th class="listTitle2">最低在线人数</th>
            <th class="listTitle">平均在线人数</th>
        </tr>
        <% 
            Dim i_1,i_2,dateInfo,ret,index,className   
            If monthStr = "-1" Then '根据年查询                               
                For i_1=1 To 12
                index=0 
                '行样式
                If i_1 Mod 2 = 0 Then
                    className="list"        
                Else
                    className="listBg"      
                End If 
                
                If i_1<10 Then
                    dateInfo = yearStr&"-0"&i_1
                Else
                    dateInfo = yearStr&"-"&i_1
                End If
                If IsNull(Array) Then
                    ret = false
                Else
                    For i_2=0 To UBound(Array,2)      
                         If dateInfo = Array(0,i_2) Then
                             ret = true  
                             Exit For
                         Else
                            ret = false
                         End If
                    index=index+1
                    Next   
                End If
                If ret = true Then
        %>       
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">              
                    <td><a class="l" href="javascript:void(0)" onclick="Redirect('StatOnline.asp?action=query&Time=<%=Array(0,index) %>')"><%=Array(0,index) %></a></td>  
                    <td><%=Array(1,index) %></td> 
                    <td><%=Array(2,index) %></td>
                    <td><%=Array(3,index) %></td> 
                </tr>
                <%Else %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">              
                    <td><a class="l" href="javascript:void(0)" onclick="Redirect('StatOnline.asp?action=query&Time=<%=dateInfo %>')"><%=dateInfo %></a></td>
                    <td>--</td>  
                    <td>--</td>  
                    <td>--</td>  
                </tr>   
        <%        
                End If
                Next
            Elseif dayStr = "-1" Then '根据月查询
                Dim dayCount
                dayCount = Common.GetDayCountOfMonth(yearStr,monthStr)
                For i_1 =1 To dayCount
                index=0
                '行样式
                If i_1 Mod 2 = 0 Then
                    className="list"        
                Else
                    className="listBg"      
                End If 
                
                If i_1<10 Then
                    dateInfo = yearStr&"-"&monthStr&"-0"&i_1
                Else
                    dateInfo = yearStr&"-"&monthStr&"-"&i_1
                End If
                If IsNull(Array) Then
                    ret = false
                Else
                    For i_2=0 To UBound(Array,2)     
                        If dateInfo = Array(0,i_2) Then
                            ret = true  
                            Exit For
                        Else
                            ret = false
                        End If
                    index = index+1
                    Next
                End If
                If ret = true Then
        %>   
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">  
                    <td><a class="l" href="javascript:void(0)" onclick="Redirect('StatOnline.asp?action=query&Time=<%=Array(0,index) %>')"><%=Array(0,index) %></a></td>              
                    <td><%=Array(1,index) %></td> 
                    <td><%=Array(2,index) %></td>
                    <td><%=Array(3,index) %></td> 
                </tr>
                <%Else %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">              
                    <td><a class="l" href="javascript:void(0)" onclick="Redirect('StatOnline.asp?action=query&Time=<%=dateInfo %>')"><%=dateInfo %></a></td>
                    <td>--</td>  
                    <td>--</td>  
                    <td>--</td>  
                </tr>                   
        <%    
                End If
                Next
            Else '根据日统计
                For i_1 =0 To 23
                index=0
                '行样式
                If i_1 Mod 2 = 0 Then
                    className="list"        
                Else
                    className="listBg"      
                End If 
                
                If IsNull(Array) Then
                    ret = false
                Else
                    For i_2=0 To UBound(Array,2)     
                        If i_1 = Array(0,i_2) Then
                            ret = true  
                            Exit For
                        Else
                            ret = false
                        End If
                    index = index+1
                    Next
                End If
                If ret = true Then
        %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">              
                    <td><%=Array(0,index) %>:00 - <%=Array(0,index) %>:59</td>  
                    <td><%=Array(1,index) %></td> 
                    <td><%=Array(2,index) %></td>
                    <td><%=Array(3,index) %></td> 
                </tr>
                <%Else %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">              
                    <td><%=i_1 %>:00 - <%=i_1 %>:59</td>  
                    <td>--</td>  
                    <td>--</td>  
                    <td>--</td>  
                </tr>         
        <% 
                End If
                Next
            End If
        %>
    </table>
    <br />
    <% 
        End Sub
    %>
</body>
</html>
