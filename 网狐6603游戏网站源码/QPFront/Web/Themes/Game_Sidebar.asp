<script type="text/javascript">    
    function setCookie2(sName, sValue) {
        var expires = new Date();
        expires.setTime(expires.getTime() + 16 * 60 * 1000);
        document.cookie = sName + "=" + escape(sValue) + "; expires=" + expires.toGMTString() + "; path=/";
    }
    
    function getCookie (sName) {
	    var aCookie = document.cookie.split("; ");
	    for (var i=0; i < aCookie.length; i++) {
		    var aCrumb = aCookie[i].split("=");
		    if (sName == aCrumb[0])
			    return unescape(aCrumb[1]);
	    }
	    return null;
    }
    
    function showHide(obj) {
        var oStyle = obj.style;
        var imgId = obj.id.replace("M","S");
//        oStyle.display == "none" ? oStyle.display = "block" : oStyle.display = "none";
//        oStyle.display == "none" ? document.getElementById(imgId).className = "gameListTitle2" : document.getElementById(imgId).className = "gameListTitle1";
        
        if(oStyle.display == "none")
        {
            oStyle.display = "block";
            document.getElementById(imgId).className = "gameListTitle1";
            setCookie2("G"+obj.id,"on")
        }
        else
        {
            oStyle.display = "none";
            document.getElementById(imgId).className = "gameListTitle2";
            setCookie2("G"+obj.id,"off")
        }
    }    
    window.onload = function() {
        var count = document.getElementById("TCount").value
        for(var i=1;i<=count;i++)
        {
            if (getCookie("GM_"+i) == null || getCookie("GM_"+i) == undefined || getCookie("GM_"+i) == "on") 
            {
                document.getElementById("M_"+i).style.display = "block";
                document.getElementById("S_"+i).className = "gameListTitle1";
            }
            else
            {
                document.getElementById("M_"+i).style.display = "none";
                document.getElementById("S_"+i).className = "gameListTitle2";
            }
        }
    }
</script>
<div class="LeftSide">
	<!--游戏列表开始-->
	<div class="gameList"><div class="clear"></div></div>
	<div class="gameListBg">
	    <% 
	        Call ConnectGame(QPPlatformDB)
	        Call Main()
	        CloseGame()
	        
	        Sub Main()
	            Call ConnectGame(QPPlatformDB)
	            Dim rsT,rsK,i
	            i=0
	            Set rsT = GameConn.Execute("select TypeID,TypeName from GameTypeItem(nolock) where Nullity=0 order by SortID")
	            If Not rsT.eof Then
	                do until rsT.eof   
	                i=i+1  
	    %>
	    <div class="gameListTitle1" id="S_<%=i %>" onclick="JavaScript:showHide(document.getElementById('M_<%=i %>'));"><label class="gameType<%=i %>"><%=rsT("TypeName") %></label></div>
	    <div class="clear"></div>
	    <ul id="M_<%=i %>">
	    <% 
	        Set rsK = GameConn.Execute("select KindID,KindName from GameKindItem(nolock) where TypeID="&rsT("TypeID")&" And Nullity=0 order by SortID")
	        If Not rsK.eof Then
	            do until rsK.eof     
	    %>
	    <li><span class="game<%=rsK("KindID") %>"><a href="GameRule.asp?KindID=<%=rsK("KindID") %>" class="lh"><%=rsK("KindName") %></a></span></li>
	    <% 
	            rsK.movenext
                loop
                rsK.Close:Set rsK=Nothing
	            End If
	    %>
	    </ul>
	    <% 
	            rsT.movenext
                loop
                rsT.Close:Set rsT=Nothing
	            End If
	        
	    %>
	    <input id="TCount" type="hidden" value="<%=i %>" />
	    <%
	        End Sub
	    %>		
	</div>
	<div class="gameListEnd"></div>
	<!--游戏列表结束-->
</div>
<div class="clear"></div>