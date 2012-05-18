/*--------------------------------------
Name: AJAXRequest
Version: 0.7.12
Author: xujiwei
E-mail: vipxjw@163.com
Website: http://www.xujiwei.cn
License: http://www.gnu.org/licenses/gpl.html GPL
Copyright (c) 2007, xujiwei, All Rights Reserved

AjaxRequest Deveoper Manual:
    http://ajax.xujiwei.cn/
--------------------------------------*/
function AJAXRequest() {
	var xmlPool=[],objPool=[],AJAX=this,ac=arguments.length,av=arguments;
	var xmlVersion=["MSXML2.XMLHTTP","Microsoft.XMLHTTP"];
	var eF=emptyFun=function(){};
	var av=ac>0?typeof(av[0])=="object"?av[0]:{}:{};
	var encode=$GEC(av.charset+"");
	this.url=getp(av.url,"");
	this.content=getp(av.content,"");
	this.method=getp(av.method,"POST");
	this.async=getp(av.async,true);
	this.timeout=getp(av.timeout,3600000);
	this.ontimeout=getp(av.ontimeout,eF);
	this.onrequeststart=getp(av.onrequeststart,eF);
	this.onrequestend=getp(av.onrequestend,eF);
	this.oncomplete=getp(av.oncomplete,eF);
	this.onexception=getp(av.onexception,eF);
	if(!getObj()) return false;
	function getp(p,d) { return p!=undefined?p:d; }
	function getObj() {
		var i,j,tmpObj;
		for(i=0,j=xmlPool.length;i<j;i++) if(xmlPool[i].readyState==0||xmlPool[i].readyState==4) return xmlPool[i];
		try { tmpObj=new XMLHttpRequest; }
		catch(e) {
			for(i=0,j=xmlVersion.length;i<j;i++) {
				try { tmpObj=new ActiveXObject(xmlVersion[i]); } catch(e2) { continue; }
				break;
			}
		}
		if(!tmpObj) return false;
		else { xmlPool[xmlPool.length]=tmpObj; return xmlPool[xmlPool.length-1]; }
	}
	function $(id){return document.getElementById(id);}
	function $N(d){var n=d*1;return(isNaN(n)?0:n);}
	function $VO(v){return(typeof(v)=="string"?(v=$(v))?v:false:v);}
	function $GID(){return((new Date)*1);}
	function $SOP(id,ct){objPool[id+""]=ct;}
	function $LOP(id){return(objPool[id+""]);}
	function $SRP(f,r,p){return(function(s){s=f(s);for(var i=0;i<r.length;i++) s=s.replace(r[i],p[i]);return(s);});}
	function $GEC(cs){
		if(cs.toUpperCase()=="UTF-8") return(encodeURIComponent);
		else return($SRP(escape,[/\+/g],["%2B"]));
	}
	function $ST(obj,text) {
		var nn=obj.nodeName.toUpperCase();
		if("INPUT|TEXTAREA".indexOf(nn)>-1) obj.value=text;
		else try{obj.innerHTML=text;} catch(e){};
	}
	function $CB(cb) {
		if(typeof(cb)=="function") return cb;
		else {
			cb=$VO(cb);
			if(cb) return(function(obj){$ST(cb,obj.responseText);});
			else return emptyFun; }
	}
	function $GP(p,v,d,f) {
		var i=0;
		while(i<v.length){p[i]=v[i]?f[i]?f[i](v[i]):v[i]:d[i];i++;}
		while(i<d.length){p[i]=d[i];i++;}
	}
	function send(purl,pc,pcbf,pm,pa) {
		var purl,pc,pcbf,pm,pa,ct,ctf=false,xmlObj=getObj(),ac=arguments.length,av=arguments;
		if(!xmlObj) return false;
		var pmp=pm.toUpperCase()=="POST"?true:false;
		if(!pm||!purl) return false;
		var ev={url:purl, content:pc, method:pm};
		purl+=(purl.indexOf("?")>-1?"&":"?")+"timestamp="+$GID();
		xmlObj.open(pm,purl,pa);
		AJAX.onrequeststart(ev);
		if(pmp) xmlObj.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		ct=setTimeout(function(){ctf=true;xmlObj.abort();},AJAX.timeout);
		xmlObj.onreadystatechange=function() {
			if(ctf) { AJAX.ontimeout(ev); AJAX.onrequestend(ev); }
			else if(xmlObj.readyState==4) {
				ev.status=xmlObj.status;
				try{ clearTimeout(ct); } catch(e) {};
				try{ if(xmlObj.status==200) pcbf(xmlObj); else AJAX.onexception(ev); }
				catch(e) { AJAX.onexception(ev); }
				AJAX.onrequestend(ev);
			}
		}
		if(pmp) xmlObj.send(pc); else xmlObj.send("");
		return true;
	}
	this.setcharset=function(cs) { encode=$GEC(cs); }
	this.get=function() {
		var p=[],av=arguments;
		$GP(p,av,[this.url,this.oncomplete],[null,$CB]);
		if(!p[0]&&!p[1]) return false;
		return(send(p[0],"",p[1],"GET",this.async));
	}
	this.update=function() {
		var p=[],purl,puo,pinv,pcnt,av=arguments;
		$GP(p,av,[emptyFun,this.url,-1,-1],[$CB,null,$N,$N]);
		if(p[2]==-1) p[3]=1;
		var sf=function(){send(p[1],"",p[0],"GET",this.async);};
		var id=$GID();
		var cf=function(cc) {
			sf(); cc--; if(cc==0) return;
			$SOP(id,setTimeout(function(){cf(cc);},p[2]));
		}
		cf(p[3]);
		return id;
	}
	this.stopupdate=function(id) {
		clearTimeout($LOP(id));
	}
	this.post=function() {
		var p=[],av=arguments;
		$GP(p,av,[this.url,this.content,this.oncomplete],[null,null,$CB]);
		if(!p[0]&&!p[2]) return false;
		return(send(p[0],p[1],p[2],"POST",this.async));
	}
	this.postf=function() {
		var p=[],fo,vaf,pcbf,purl,pc,pm,ac=arguments.length,av=arguments;
		fo=ac>0?$VO(av[0]):false;
		if(!fo||(fo&&fo.nodeName!="FORM")) return false;
		vaf=fo.getAttribute("onvalidate");
		vaf=vaf?(typeof(vaf)=="string"?new Function(vaf):vaf):null;
		if(vaf&&!vaf()) return false;
		$GP(p,[av[1],fo.getAttribute("action"),fo.getAttribute("method")],[this.oncomplete,this.url,this.method],[$CB,null,null]);
		pcbf=p[0];purl=p[1];
		if(!pcbf&&!purl) return false;
		pc=this.formToStr(fo); if(!pc) return false;
		if(p[2].toUpperCase()=="POST")
			return(send(purl,pc,pcbf,"POST",true));
		else {
			purl+=(purl.indexOf("?")>-1?"&":"?")+pc;
			return(send(purl,"",pcbf,"GET",true));
		}
	}
	/* formToStr
	// from SurfChen <surfchen@gmail.com>
	// @url     http://www.surfchen.org/
	// @license http://www.gnu.org/licenses/gpl.html GPL
	// modified by xujiwei
	// @url     http://www.xujiwei.cn/
	*/
	this.formToStr=function(fc) {
		var i,qs="",and="",ev="";
		for(i=0;i<fc.length;i++) {
			e=fc[i];
			if (e.name!='') {
				if (e.type=='select-one'&&e.selectedIndex>-1) ev=e.options[e.selectedIndex].value;
				else if (e.type=='checkbox' || e.type=='radio') {
					if (e.checked==false) continue;
					ev=e.value;
				}
				else ev=e.value;
				ev=encode(ev);
				qs+=and+e.name+'='+ev;
				and="&";
			}
		}
		return qs;
	}
}