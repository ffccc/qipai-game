// JScript 文件
function GetQueryString(name) 
{ 
    var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)"); 
    var r = window.location.search.substr(1).match(reg); 
    if(r!=null)return  unescape(r[2]); return null; 
} 
function GetOrder(orderFilds,link)
{
    var url ="";
    if(GetQueryString("orders")!=null&&GetQueryString("orders").length>1)
    {
        url= window.location.href.split("?")[1]
        if(GetQueryString("OrderType")=="-101")
        {
            link.href="?"+url.replace(GetQueryString("orders"),orderFilds).replace(GetQueryString("OrderType"),"-102");
        }
        else
        {
            link.href="?"+url.replace(GetQueryString("orders"),orderFilds).replace(GetQueryString("OrderType"),"-101");
        }
    }
    else
    {
        url= window.location.href.split("?")[1]
        
        if(url!=null)
        {
            link.href="?"+url+"&orders="+orderFilds+"&OrderType=-101";
        }
        else
        {
            link.href="?orders="+orderFilds+"&OrderType=-101";
        }
    }
}