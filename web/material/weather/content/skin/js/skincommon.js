
Date.prototype.Format = function (fmt) { //author: meizz
    var o = {
        "M+": this.getMonth() + 1, //月份
        "d+": this.getDate(), //日
        "h+": this.getHours(), //小时
        "m+": this.getMinutes(), //分
        "s+": this.getSeconds(), //秒
        "q+": Math.floor((this.getMonth() + 3) / 3), //季度
        "S": this.getMilliseconds() //毫秒
    };
    if (/(y+)/.test(fmt)) fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
    for (var k in o)
        if (new RegExp("(" + k + ")").test(fmt)) fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));
    return fmt;
}

//四舍五入函数
function ForDight(Dight, How) {
    Dight = Math.round(Dight * Math.pow(10, How)) / Math.pow(10, How);
    return Dight;
}

function WeekCN(day) {
    var temp = "--";
    switch (day) {
        case 0:
            temp = "星期日";
            break;
        case 1:
            temp = "星期一";
            break;
        case 2:
            temp = "星期二";
            break;
        case 3:
            temp = "星期三";
            break;
        case 4:
            temp = "星期四";
            break;
        case 5:
            temp = "星期五";
            break;
        case 6:
            temp = "星期六";
            break;
    }
    return temp;
}

function WeekEN(day) {
    var temp = "--";
    switch (day) {
        case 0:
            temp = "Sunday";
            break;
        case 1:
            temp = "Monday";
            break;
        case 2:
            temp = "Tuesday";
            break;
        case 3:
            temp = "Wednesday";
            break;
        case 4:
            temp = "Thursday";
            break;
        case 5:
            temp = "Friday";
            break;
        case 6:
            temp = "Saturday";
            break;
    }
    return temp;
}

function advise(aqi) {  
    if (aqi <= 50) {
        $("#tishi").text("空气质量令人满意，基本无空气污染");
        $("#cuoshi").text("今天全市空气质量良好，适合户外运动");
        return;
    }
    else if (aqi <= 100) {
        $("#tishi").text("某些污染物可能对极少数异常敏感人群健康有较弱影响");
        $("#cuoshi").text("建议极少数异常敏感人群应减少户外活动");
        return;
    }
    else if (aqi <= 150) {
        $("#tishi").text("易感人群症状有轻度加剧，健康人群出现刺激症状");
        $("#cuoshi").text("建议儿童、老人及心脏病、呼吸系统疾病患者减少户外锻炼");
        return;
    }
    else if (aqi <= 200) {
        $("#tishi").text("加剧易感人群症状，可能对健康人心脏，呼吸系统有影响");
        $("#cuoshi").text("疾病患者避免过多高强度户外锻练，一般人减少户外运动");
        return;
    }
    else if (aqi <= 300) {
        $("#tishi").text("心脏病和肺病患者症状加剧，耐受力降低，健康人出现症状");
        $("#cuoshi").text("建议老人和心脏病、肺病患者停留在室内，减少体力活动");
        return;
    }
    else {
        $("#tishi").text("健康人群耐受力降低，有明显强烈症状，某些疾病提前");
        $("#cuoshi").text("建议留在室内，避免户外活动");
        return;
    }
}

function GetWdirect(ds) {
    ds = ds + 0;
    if (ds >= 0 && ds < 45) {
        return "北风";
    }
    if (ds >= 45 && ds < 90) {
        return "东北风";
    }
    if (ds >= 90 && ds < 135) {
        return "东风";
    }
    if (ds >=135 && ds < 180) {
        return "东南风";
    }
    if (ds >= 180 && ds < 225) {
        return "南风";
    }
    if (ds >= 225 && ds < 270) {
        return "西南风";
    }
    if (ds >= 270 && ds < 315) {
        return "西风";
    }
    if (ds >= 315 && ds < 360) {
        return "西北风";
    }
    return "未知";
}

function GetArrayObj(arry,item,value) {
    for (var i = 0; i < arry.length; i++) {
        if (arry[i][item] == value) {
            return arry[i];
        }
    }
    return undefined;
}