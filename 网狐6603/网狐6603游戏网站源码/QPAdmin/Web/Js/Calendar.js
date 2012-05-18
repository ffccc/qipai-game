<!--
document.write("<div id=meizzCalendarLayer style='border:1px;position: absolute; z-index: 9999; width: 267; height: 150; display:none;'>");
document.write("<iframe name=meizzCalendarIframe scrolling=no frameborder=0 width=267 height=150></iframe></div>");
function writeIframe()
{
    var strIframe = "<html><head><style>"+ //<meta http-equiv='Content-Type' content='text/html; charset=gb2312'>
    "*{font-size: 12px; font-family: Verdana ;}"+  
    "table#tableMain{border-top-width: 1px;	border-right-width: 1px;border-bottom-width: 1px;border-left-width: 1px;"+
	"border-top-style: none;"+
	"border-right-style: outset;"+
	"border-bottom-style: ridge;"+
	"border-left-style: inset;"+
	"border-top-color: "+ WebCalendar.darkColor +";"+
	"border-right-color: "+ WebCalendar.darkColor +";"+
	"border-bottom-color: "+ WebCalendar.darkColor +";"+
	"border-left-color: "+ WebCalendar.darkColor +";}"+
    "table#tableWeek td{ color: "+ WebCalendar.darkColor +";background-color:"+ WebCalendar.lightColor +";border-bottom:black; 1px solid;}"+   // ""+
    "td#meizzYearHead, td#meizzYearMonth{ FONT-WEIGHT: bold; FONT-SIZE: 12px; COLOR: "+ WebCalendar.lightColor +"}"+
    ".out { text-align: center; color:"+WebCalendar.wordColor+";FONT-SIZE:12PX:FONT-FAMILY:新宋体;}"+
    ".over{ text-align: center; color:#555555;FONT-SIZE:12PX:FONT-FAMILY:新宋体;}"+ //已取消该效果
    " input{ border: 1px solid "+ WebCalendar.darkColor +"; padding-top: 1px; height: 10; cursor: hand;"+
    " color:"+ WebCalendar.wordColor +"; background-color: "+ WebCalendar.btnBgColor +"}"+
    ".checkedDay{background:url("+WebCalendar.SelectedImgUrl+") no-repeat center center;text-align: center; color:"+ WebCalendar.lightColor +";FONT-SIZE:12PX:FONT-FAMILY:新宋体;background-color:"+WebCalendar.dayBgColor+";}"+ 
    ".ToDay{background:url("+WebCalendar.ToDayImgUrl+") no-repeat center center;text-align: center; color:"+ WebCalendar.wordColor +";FONT-SIZE:12PX:FONT-FAMILY:新宋体;background-color:"+WebCalendar.dayBgColor+";}"+ 
    ".ToDayAndcheckedDay{background:url("+WebCalendar.SetectedToDayImgUrl+") no-repeat center center;text-align: center; color:"+ WebCalendar.lightColor +";FONT-SIZE:12PX:FONT-FAMILY:新宋体; background-color:"+WebCalendar.dayBgColor+";}"+ 
    ".Left_Up{background:url("+WebCalendar.LeftUpImgUrl+") no-repeat center center;text-align: center;}"+
    ".Left_down{background:url("+WebCalendar.LeftDownImgUrl+") no-repeat center center;text-align: center; }"+ 
    ".Right_Up{background:url("+WebCalendar.RightUpImgUrl+") no-repeat center center;text-align: center;}"+
    ".Right_down{background:url("+WebCalendar.RightDownImgUrl+") no-repeat center center;text-align: center; }"+  
    "</style></head><body onselectstart='return false' style='margin: 0px' oncontextmenu='return false'><form name=meizz>";


    if (WebCalendar.drag){ strIframe += "<scr"+"ipt language=javascript>"+
    "var drag=false, cx=0, cy=0, o = parent.WebCalendar.calendar; function document.onmousemove(){"+
    "if(parent.WebCalendar.drag && drag){if(o.style.left=='')o.style.left=0; if(o.style.top=='')o.style.top=0;"+
    "o.style.left = parseInt(o.style.left) + window.event.clientX-cx;"+
    "o.style.top  = parseInt(o.style.top)  + window.event.clientY-cy;}}"+
    "function document.onkeydown(){ switch(window.event.keyCode){  case 27 : parent.hiddenCalendar(); break;"+
    "case 37 : parent.prevM(); break; case 38 : parent.prevY(); break; case 39 : parent.nextM(); break; case 40 : parent.nextY(); break;"+
    "case 84 : document.forms[0].today.click(); break;} window.event.keyCode = 0; window.event.returnValue= false;}"+
    "function dragStart(){cx=window.event.clientX; cy=window.event.clientY; drag=true;}</scr"+"ipt>"}

    strIframe += "<select name=tmpYearSelect  onblur='parent.hiddenSelect(this)' style='z-index:1;position:absolute;top:3;left:75;display:none'"+
    " onchange='parent.WebCalendar.thisYear =this.value; parent.hiddenSelect(this); parent.writeCalendar();'></select>"+
    "<select name=tmpMonthSelect onblur='parent.hiddenSelect(this)' style='z-index:1; position:absolute;top:3;left:115;display:none'"+
    " onchange='parent.WebCalendar.thisMonth=this.value; parent.hiddenSelect(this); parent.writeCalendar();'></select>"+

    "<table id=tableMain width=\"100%\" border=0 cellspacing=0 cellpadding=0>"+
    "<tr><td bgcolor='"+WebCalendar.darkColor+"'>"+ //
    "    <table width=100% height=\"30px\" id=tableHead border=0 cellspacing=0 cellpadding=0><tr>"+ 
    "    <td width=10% title='向前翻 1 月&#13;快捷键：←' class=\"Left_Up\" onmousedown=\"parent.changeToLeftDown(this);\" onmouseup=\"parent.changeToLeftUp(this);\" style='cursor: hand' onclick='parent.prevM()'>&nbsp;</td>"+
    "    <td width=40% id='meizzYearHead' algin='center' "+
    " </td>"+
    "    <td width=40% id='meizzYearMonth' algin='left'"+
   " </td>"+
    "    <td width=10% title='向后翻 1 月&#13;快捷键：→' class=\"Right_Up\" onmousedown=\"parent.changeToRightDown(this);\" onmouseup=\"parent.changeToRightUp(this);\" onclick='parent.nextM()' style='cursor: hand'>&nbsp;</td></tr></table>"+
    "</td></tr><tr><td><table id=tableWeek style=\"margin-top:1px;\" border=0 width=100% cellpadding=0 cellspacing=0 ";
    if(WebCalendar.drag){strIframe += "onmousedown='dragStart()' onmouseup='drag=false' onmouseout='drag=false'";}
    strIframe += " borderColorLight='"+ WebCalendar.lightColor +"' borderColorDark='"+ WebCalendar.darkColor +"'>"+
    "    <tr align=\"center\" height=\"15px\" ><td nowrap >星期日</td><td nowrap>星期一</td><td nowrap>星期二</td><td nowrap>星期三</td><td nowrap>星期四</td><td nowrap>星期五</td><td nowrap>星期六</td></tr></table>"+
    "</td></tr>"+
   "<tr><td height=1 algin=\"center\" style=\"background-color:"+WebCalendar.darkColor+"\"></td></tr>"+
   " <tr><td algin=\"center\" valign=top bgcolor='"+ WebCalendar.lightColor +"'>"+
    "    <table id=tableDay width=100% border=0 cellspacing=0 cellpadding=0>";
         for(var x=0; x<5; x++){ strIframe += "<tr HEGHT=\"13PX\">";
         for(var y=0; y<7; y++)  strIframe += "<td class=out id='meizzDay"+ (x*7+y) +"'></td>"; strIframe += "</tr>";}
         strIframe += "<tr>";
         for(var x=35; x<39; x++) strIframe += "<td class=out id='meizzDay"+ x +"'></td>";
         strIframe +="<td colspan='3' title='"+ WebCalendar.regInfo +"' bgcolor=\""+WebCalendar.dayBgColor+"\">&nbsp;</td></tr></table>"+
    "</td></tr><tr><td bgcolor='"+ WebCalendar.lightColor +"'>"+
    " <table border=0 cellpadding=1 cellspacing=0 width=100% bgcolor=\""+WebCalendar.dayBgColor+"\">"+
    " <tr><td class='ToDay'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+ 
    " </td><td alt=\"点击选中今天，快捷键[T]\" align=\"left\" style=\"cursor:hand;\" onclick=\"parent.returnDate(new Date().getDate() +'/'+ (new Date().getMonth() +1) +'/'+ new Date().getFullYear())\">"+
    " <B>今天："+ new Date().getFullYear() +"-"+ (new Date().getMonth() +1) +"-"+ new Date().getDate() +"</B>"+
    "    </td></table>"+
    "</td></tr><table></form>"+
    "</body></html>";
    with(WebCalendar.iframe)
    {
        document.writeln(strIframe); document.close();
        for(var i=0; i<39; i++)
        {
            WebCalendar.dayObj[i] = eval("meizzDay"+ i);
            WebCalendar.dayObj[i].onmouseover = dayMouseOver;
            WebCalendar.dayObj[i].onmouseout  = dayMouseOut;           
            WebCalendar.dayObj[i].onclick     = returnDate;           
        }
    }
   //document.getElementById("22").innerText =  strIframe ;
   //alert(strIframe);
}
function WebCalendar() //初始化日历的设置
{
    this.regInfo    = "关闭的快捷键：[Esc]";
 
    this.daysMonth  = new Array(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
    this.day        = new Array(39);            //定义日历展示用的数组
    this.dayObj     = new Array(39);            //定义日期展示控件数组
    this.dateStyle  = null;                     //保存格式化后日期数组
    this.objExport  = null;                     //日历回传的显示控件
    this.eventSrc   = null;                     //日历显示的触发控件
    this.inputDate  = null;                     //转化外的输入的日期(d/m/yyyy)
    this.thisYear   = new Date().getFullYear(); //定义年的变量的初始值
    this.thisMonth  = new Date().getMonth()+ 1; //定义月的变量的初始值
    this.thisDay    = new Date().getDate();     //定义日的变量的初始值
    this.today      = this.thisDay +"/"+ this.thisMonth +"/"+ this.thisYear;   //今天(d/m/yyyy)
    this.iframe     = window.frames("meizzCalendarIframe"); //日历的 iframe 载体
    this.calendar   = getObjectById("meizzCalendarLayer");  //日历的层
    this.dateReg    = "";           //日历格式验证的正则式

    //自定义类别
    this.SelectedImgUrl="../../Images/Date/bgCurDate.gif" ;
    this.ToDayImgUrl="../../Images/Date/bgToday1.gif" ;
    this.SetectedToDayImgUrl="../../Images/DatebgToday2.gif" ;
    this.LeftUpImgUrl="../../Images/Date/left_up.gif" ;
    this.LeftDownImgUrl="../../Images/Date/left_down.gif" ;
    this.RightUpImgUrl="../../Images/Date/right_up.gif" ;
    this.RightDownImgUrl="../../Images/Date/right_down.gif" ; 
       
    this.yearFall   = 10 ;         //定义年下拉框的年差值
    this.format     = "yyyy-mm-dd"; //回传日期的格式
    this.timeShow   = false;        //是否返回时间
    this.drag       = true;         //是否允许拖动
    this.darkColor  = "#0A246A";    //控件的暗色
    this.lightColor = "#ffffff" ;   //"#FFFFFF";    //控件的亮色
    this.btnBgColor = "#FFF5A0";    //控件的按钮背景色
    this.wordColor  = "#000000";    //控件的文字颜色
    this.wordDark   = "#D4D0C8";    //控件的暗文字颜色
    this.dayBgColor = "#Fefefe";     //日期数字背景色
    
    this.todayColor = "#FF9933";    //今天在日历上的标示背景色,已经用图片取代    
    this.DarkBorder = "#FFE4C4";    //日期显示的立体表达色 ,未使用
    
}   var WebCalendar = new WebCalendar();

function calendar() //主调函数
{
    var e = window.event.srcElement;   writeIframe();
    var o = WebCalendar.calendar.style; WebCalendar.eventSrc = e;
	if (arguments.length == 0) WebCalendar.objExport = e;
    else WebCalendar.objExport = eval(arguments[0]);
    
   var ob =  WebCalendar.objExport ;
    
    WebCalendar.iframe.tableWeek.style.cursor = WebCalendar.drag ? "move" : "default";
	
	var t = ob.offsetTop,  h = ob.clientHeight, l = ob.offsetLeft, p = ob.type;
	while (ob = ob.offsetParent){t += ob.offsetTop; l += ob.offsetLeft;}
    o.display = ""; WebCalendar.iframe.document.body.focus();
    var cw = WebCalendar.calendar.clientWidth, ch = WebCalendar.calendar.clientHeight;
    var dw = document.body.clientWidth, dl = document.body.scrollLeft, dt = document.body.scrollTop;
    
    
    if (document.body.clientHeight + dt - t - h >= ch) o.top = (p=="image")? t + h : t + h + 6;
    else o.top  = (t - dt < ch) ? ((p=="image")? t + h : t + h + 6) : t - ch;
    if (dw + dl - l >= cw) o.left = l; else o.left = (dw >= cw) ? dw - cw + dl : dl;
    

    if  (!WebCalendar.timeShow) WebCalendar.dateReg = /^(\d{1,4})(-|\/|.)(\d{1,2})\2(\d{1,2})$/;
    else WebCalendar.dateReg = /^(\d{1,4})(-|\/|.)(\d{1,2})\2(\d{1,2}) (\d{1,2}):(\d{1,2}):(\d{1,2})$/;


    try{
        if (WebCalendar.objExport.value.trim() != "")
        {
            WebCalendar.dateStyle = WebCalendar.objExport.value.trim().match(WebCalendar.dateReg);
           
            if (WebCalendar.dateStyle == null)
            {
                WebCalendar.thisYear   = new Date().getFullYear();
                WebCalendar.thisMonth  = new Date().getMonth()+ 1;
                WebCalendar.thisDay    = new Date().getDate();
                //alert("原文本框里的日期有错误！\n可能与你定义的显示时分秒有冲突！");
                writeCalendar(); return false;
            }
            else
            {
              
                WebCalendar.thisYear   = parseInt(WebCalendar.dateStyle[1], 10);
                WebCalendar.thisMonth  = parseInt(WebCalendar.dateStyle[3], 10);
                WebCalendar.thisDay    = parseInt(WebCalendar.dateStyle[4], 10);
                WebCalendar.inputDate  = parseInt(WebCalendar.thisDay, 10) +"/"+ parseInt(WebCalendar.thisMonth, 10) +"/"+ 
                parseInt(WebCalendar.thisYear, 10); writeCalendar();
            }
        }  else{ writeCalendar();}
    }  catch(e){ 
     //writeCalendar();
    }
}
function funMonthSelect() //月份的下拉框
{
    var m = isNaN(parseInt(WebCalendar.thisMonth, 10)) ? new Date().getMonth() + 1 : parseInt(WebCalendar.thisMonth);
    var e = WebCalendar.iframe.document.forms[0].tmpMonthSelect;
    for (var i=1; i<13; i++) e.options.add(new Option(i +"月", i));
    e.style.display = ""; e.value = m; e.focus(); window.status = e.style.top;
}
function funYearSelect() //年份的下拉框
{
    var n = WebCalendar.yearFall;
    var e = WebCalendar.iframe.document.forms[0].tmpYearSelect;
    var y = isNaN(parseInt(WebCalendar.thisYear, 10)) ? new Date().getFullYear() : parseInt(WebCalendar.thisYear);
        y = (y <= 1000)? 1000 : ((y >= 9999)? 9999 : y);
    var min = (y - n >= 1000) ? y - n : 1000;
    var max = (y + n <= 9999) ? y + n : 9999;
        min = (max == 9999) ? max-n*2 : min;
        max = (min == 1000) ? min+n*2 : max;
    for (var i=min; i<=max; i++) e.options.add(new Option(i +"年", i));
    e.style.display = ""; e.value = y; e.focus();
}
function prevM()  //往前翻月份
{
    WebCalendar.thisDay = 1;
    if (WebCalendar.thisMonth==1)
    {
        WebCalendar.thisYear--;
        WebCalendar.thisMonth=13;
    }
    WebCalendar.thisMonth--; writeCalendar();
}
function nextM()  //往后翻月份
{
    WebCalendar.thisDay = 1;
    if (WebCalendar.thisMonth==12)
    {
        WebCalendar.thisYear++;
        WebCalendar.thisMonth=0;
    }
    WebCalendar.thisMonth++; writeCalendar();
}
function prevY(){WebCalendar.thisDay = 1; WebCalendar.thisYear--; writeCalendar();}//往前翻 Year
function nextY(){WebCalendar.thisDay = 1; WebCalendar.thisYear++; writeCalendar();}//往后翻 Year
function hiddenSelect(e){for(var i=e.options.length; i>-1; i--)e.options.remove(i); e.style.display="none";}
function getObjectById(id){ if(document.all) return(eval("document.all."+ id)); return(eval(id)); }
function hiddenCalendar(){getObjectById("meizzCalendarLayer").style.display = "none";};
function appendZero(n){return(("00"+ n).substr(("00"+ n).length-2));}//日期自动补零程序
function String.prototype.trim(){return this.replace(/(^\s*)|(\s*$)/g,"");}
function dayMouseOver()
{ 
    this.style.cursor="hand" ;
   // this.className = "over";
    //this.style.backgroundColor = WebCalendar.darkColor;
    //if(WebCalendar.day[this.id.substr(8)].split("/")[1] == WebCalendar.thisMonth)
    //this.style.color = WebCalendar.lightColor;
}
function dayMouseOut()
{   
    this.style.cursor="normal" ;
//    var d = WebCalendar.day[this.id.substr(8)], a = d.split("/");
//    this.style.removeAttribute('backgroundColor');
//    if(a[1] == WebCalendar.thisMonth && d != WebCalendar.today)
//    {
//        if(WebCalendar.dateStyle && a[0] == parseInt(WebCalendar.dateStyle[4], 10))
//        this.style.color = WebCalendar.lightColor;
//        this.style.color = WebCalendar.wordColor;
//    }
}
function changeToLeftDown(preobj)
{
    preobj.className="Left_Down" ;
}
function changeToLeftUp(preobj)
{
      preobj.className="Left_Up" ;
}
function changeToRightDown(backobj)
{
    backobj.className = "Right_Down" ;
}
function changeToRightUp(backobj)
{
    backobj.className = "Right_Up" ;
}
function writeCalendar() //对日历显示的数据的处理程序
{
    var y = WebCalendar.thisYear;
    var m = WebCalendar.thisMonth; 
    var d = WebCalendar.thisDay;
    WebCalendar.daysMonth[1] = (0==y%4 && (y%100!=0 || y%400==0)) ? 29 : 28;
    if (!(y<=9999 && y >= 1000 && parseInt(m, 10)>0 && parseInt(m, 10)<13 && parseInt(d, 10)>0)){
        alert("对不起，你输入了错误的日期！");
        WebCalendar.thisYear   = new Date().getFullYear();
        WebCalendar.thisMonth  = new Date().getMonth()+ 1;
        WebCalendar.thisDay    = new Date().getDate(); }
    y = WebCalendar.thisYear;
    m = WebCalendar.thisMonth;
    d = WebCalendar.thisDay;
    
    //写头了
    WebCalendar.iframe.meizzYearHead.innerHTML  =  "<span style=\"float:right;margin-right:2px;\"  title='点击此处选择年份'  onclick='parent.funYearSelect(parseInt(this.innerText, 10))'"+
    //" onmouseover='this.style.color=parent.WebCalendar.wordColor';"+
    //" onmouseout=\"this.style.color='' ;\
     ">"+ y +" 年</span>";
    WebCalendar.iframe.meizzYearMonth.innerHTML = "<span style=\"float:left;margin-right:2px;\" title='点击此处选择月份' algin=\"left\" onclick='parent.funMonthSelect(parseInt(this.innerText, 10))'"+
    //"  onmouseover='this.style.color=parent.WebCalendar.wordColor';"+
   // "  onmouseout=\"this.style.color='' ;\
    ">"+parseInt(m, 10) +" 月</span>"; 

    
    WebCalendar.daysMonth[1] = (0==y%4 && (y%100!=0 || y%400==0)) ? 29 : 28; //闰年二月为29天
    var w = new Date(y, m-1, 1).getDay();
    var prevDays = m==1  ? WebCalendar.daysMonth[11] : WebCalendar.daysMonth[m-2];
    for(var i=(w-1); i>=0; i--) //这三个 for 循环为日历赋数据源（数组 WebCalendar.day）格式是 d/m/yyyy
    {
        WebCalendar.day[i] = prevDays +"/"+ (parseInt(m, 10)-1) +"/"+ y;
        if(m==1) WebCalendar.day[i] = prevDays +"/"+ 12 +"/"+ (parseInt(y, 10)-1);
        prevDays--;
    }
    for(var i=1; i<=WebCalendar.daysMonth[m-1]; i++) WebCalendar.day[i+w-1] = i +"/"+ m +"/"+ y;
    for(var i=1; i<39-w-WebCalendar.daysMonth[m-1]+1; i++)
    {
        WebCalendar.day[WebCalendar.daysMonth[m-1]+w-1+i] = i +"/"+ (parseInt(m, 10)+1) +"/"+ y;
        if(m==12) WebCalendar.day[WebCalendar.daysMonth[m-1]+w-1+i] = i +"/"+ 1 +"/"+ (parseInt(y, 10)+1);
    }
    for(var i=0; i<39; i++)    //这个循环是根据源数组写到日历里显示
    {
        var a = WebCalendar.day[i].split("/");
        WebCalendar.dayObj[i].innerText    = a[0];
        WebCalendar.dayObj[i].title        = a[2] +"-"+ appendZero(a[1]) +"-"+ appendZero(a[0]);
        WebCalendar.dayObj[i].bgColor      = WebCalendar.dayBgColor;
        
        //WebCalendar.dayObj[i].style.color  = WebCalendar.wordColor;
        if ((i<10 && parseInt(WebCalendar.day[i], 10)>20) || (i>27 && parseInt(WebCalendar.day[i], 10)<12))
           WebCalendar.dayObj[i].style.color = WebCalendar.wordDark;
        
        if(WebCalendar.inputDate==WebCalendar.day[i] && WebCalendar.day[i] == WebCalendar.today )
        {
             WebCalendar.dayObj[i].className="ToDayAndcheckedDay" ;
        }  
        else  if (WebCalendar.inputDate==WebCalendar.day[i])    //设置输入框里的日期在日历上的颜色
        {
          WebCalendar.dayObj[i].className="checkedDay" ;
         
        }
        else if (WebCalendar.day[i] == WebCalendar.today)      //设置今天在日历上反应出来的颜色
        {
          WebCalendar.dayObj[i].className="ToDay" ;       
        }
        
    }
}
 function selectText(obj,sX)
   {            
        if(obj.value.length !=10 || obj.readOnly)  return;
        var aa=obj.createTextRange(); 
        if(sX >=0 && sX <=25) // 33 45  51 63
        {
          aa.moveEnd("word",-4);
        } 
        else if(sX >=33 && sX <=45) 
        {  
            aa.moveStart("word",2); 
            aa.moveEnd("word",-2); 
        }
        else if(sX >=51 && sX <=63) 
       {
            aa.moveStart("word",4); 
       } 
       else
       {
            return false ;
       }
        aa.select();             
      
        return  true ; 
     } 
    function getPos(obj)
    {
         obj.focus();
         var workRange=document.selection.createRange();
         obj.select();
         var allRange=document.selection.createRange();
         workRange.setEndPoint("StartToStart",allRange);
         var len=workRange.text.length;
         workRange.collapse(false);
         workRange.select();        
         return len ;
    } 
   
    function DateFormat(vDateName, vDateValue, e)
    {    
        if(vDateName.readOnly) return;  
	    var checkLen = getPos(vDateName);  
	    var strSeperator="-" ;
	    var whichCode = (window.Event) ? e.which : e.keyCode;	
    	
	    var alphaCheck = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/-";
	    if (alphaCheck.indexOf(vDateValue) >= 1) 
	    {    		
		    vDateName.value = vDateName.value.substr(0, (vDateValue.length-1));
		    return false;    		
	    }	
	    if (whichCode == 8) 
		    return false;
	    else 
	    {		
		    var strCheck = '47,48,49,50,51,52,53,54,55,56,57,58,59,95,96,97,98,99,100,101,102,103,104,105';
		    if (strCheck.indexOf(whichCode) != -1) 
		    {
		          
				 if (vDateValue.length == 4) 
				 {
					    vDateName.value = vDateValue+strSeperator;					    
				 }
		         if (vDateValue.length == 7)
		         {
		            if(vDateName.value.split(strSeperator).length==2)
			           vDateName.value = vDateValue+strSeperator;			     
		         }		         
		        	         
		         if(vDateValue.length ==10)
		         {
		            //alert(checkLen);
		            if(checkLen ==4)
		            {
		                selectText(vDateName,40) ;
		            }
		            if(checkLen ==7)
		            {
		                selectText(vDateName,60) ;
		            }
		         }
		    }
       } 
          
    }
function  CheckDateTime(str){     
          var   reg   =   /^(\d+)-(\d{1,2})-(\d{1,2})$/;     
          var   r   =   str.match(reg);     
          if(r==null)return   false;     
          r[2]=r[2]-1;     
          var   d=   new   Date(r[1],   r[2],r[3] );     
          if(d.getFullYear()!=r[1])return   false;     
          if(d.getMonth()!=r[2])return   false;     
          if(d.getDate()!=r[3])return   false;     
     
          return   true;   
  } 
 
   function checkInput(obj)
   {
        if(obj.value=="") return true;  
       var o = CheckDateTime(obj.value);
      
       if(!o)
       {
            alert("日期格式错误，请检查/正确的格斯如下：\n2006-09-01");
            obj.select();
            return false; 
       }
       return true ;
       
   }
function returnDate() //根据日期格式等返回用户选定的日期
{
    if(WebCalendar.objExport)
    {     
       
        var returnValue;
        var a = (arguments.length==0) ? WebCalendar.day[this.id.substr(8)].split("/") : arguments[0].split("/");
        var d = WebCalendar.format.match(/^(\w{4})(-|\/|.|)(\w{1,2})\2(\w{1,2})$/);
        if(d==null){alert("你设定的日期输出格式不对！\r\n\r\n请重新定义 WebCalendar.format ！"); return false;}
        var flag = d[3].length==2 || d[4].length==2; //判断返回的日期格式是否要补零
        returnValue = flag ? a[2] +d[2]+ appendZero(a[1]) +d[2]+ appendZero(a[0]) : a[2] +d[2]+ a[1] +d[2]+ a[0];
        if(WebCalendar.timeShow)
        {
            var h = new Date().getHours(), m = new Date().getMinutes(), s = new Date().getSeconds();
            returnValue += flag ? " "+ appendZero(h) +":"+ appendZero(m) +":"+ appendZero(s) : " "+  h  +":"+ m +":"+ s;
        }
        WebCalendar.objExport.value = returnValue;
        hiddenCalendar();
    }
}
function document.onclick()
{
    if(WebCalendar.eventSrc != window.event.srcElement) hiddenCalendar();
}
//-->