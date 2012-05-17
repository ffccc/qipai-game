//收藏网站
function addFavorites(pageUrl, pageName) {
    if (document.all) {
        window.external.addFavorite(pageUrl, pageName);
    }
    else if (window.sidebar) {
        window.sidebar.addPanel(pageName, pageUrl, "");
    }
}

//设为首页
function setHomepage(pageUrl) {
    if (document.all) {
        var lnk = document.getElementById("lnkuchomePage");
        lnk.style.behavior = 'url(#default#homepage)';
        lnk.setHomePage(pageUrl);
    }
    else if (window.sidebar) {
        if (window.netscape) {
            try {
                netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
            }
            catch (e) { }
        }
        var prefs = Components.classes['@mozilla.org/preferences-service;1'].getService(Components.interfaces.nsIPrefBranch);
        prefs.setCharPref('browser.startup.homepage', pageUrl);
    }
}  