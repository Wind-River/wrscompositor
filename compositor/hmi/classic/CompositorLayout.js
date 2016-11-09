
//item1.prefferedPosition  = "top";
//item2.prefferedPosition  = "bottom";


function elementInspect(o, i) {
    if (typeof i=='undefined') i='';
    if(i.length > 5) return '[MAX ITERATIONS]';
    var r=[];
    for(var p in o){
        var t=typeof o[p];
        r.push(i+'"'+p+'" ('+t+') => '+(t=='object' ? 'object:'+xinspect(o[p],i+'  ') : o[p]+''));
    }
    return r.join(i+'\n');
}

var compositorElementsPreferredPossition = new Array();
compositorElementsPreferredPossition["StatusBar"]       = "top";
compositorElementsPreferredPossition["LaunchBar"]       = "bottom";
compositorElementsPreferredPossition["LaunchWindow"]    = "middle-full";
compositorElementsPreferredPossition["ProjectionMode"]  = "full";
compositorElementsPreferredPossition["Maps"]            = "middle";
compositorElementsPreferredPossition["Email"]           = "middle";
compositorElementsPreferredPossition["Music"]           = "middle";

var compositorRules = new Array();

//Rules for camera: Camera full screen + status bar
compositorRules["StatusBar+Camera"] = "10;top";
compositorRules["Camera"] = "100";
//Rules for Projection Mode
compositorRules["gal_media"] = "100";
compositorRules["DiO-WrDemo"] = "100";
compositorRules["StatusBar"] = "10;top";
compositorRules["LauncherBar"] = "20;bottom";
compositorRules["LauncherWindow+other"] = "75";
compositorRules["Maps+Phone"] = "50";
compositorRules["Maps+other"] = "75";
compositorRules["Phone+other"] = "75";
compositorRules["other"] = "100";


//ProjectionMode            -> 100%
//StatusBar                 -> 10%;top
//LauncherBar:              -> 10%;bottom
//LauncherWindow + *other   -> 75%
//Maps + Phone              -> 50%
//Maps + 1*other            -> 75%
//Phone + 1*other           -> 75%
//3*other                   -> proportional


var compositorElements = new Array();

/**
 * To be called by compositor root output when a new compositor element is added
 * Layout is recalculated
 */
function onWindowAdded(compositorElement) {
    console.log("[DEBUG] Add compositorElement:" + compositorElement.subItemName);
    compositorElements.push(compositorElement);
    compositorReLayout();
}

/**
 * To be called by compositor root output when an existing compositor element is removed
 * Layout is recalculated
 */
function onWindowRemoved(compositorElement) {
    for (var i = 0; i < compositorElements.length; i++) {
        if (compositorElements[i] == compositorElement) {
            compositorElements.splice(i, 1);
            break;
        }
    }
    compositorReLayout();
}

/**
 * Get an existing compositor element based on element Unique NAME
 */
function getCompositorElementByName(name) {
    for (var i = 0; i < compositorElements.length; i++) {
        if (compositorElements[i].subItemName == name) {
            return compositorElements[i];
        }
    }
    return null;
}

/**
 * Get an existing compositor element based on element Weston surface
 */
function getCompositorElementBySurface(surface) {
    for (var i = 0; i < compositorElements.length; i++) {
        if (compositorElements[i].subItemSurface == surface) {
            return compositorElements[i];
        }
    }
    return null;
}


function compositorElementPositionToString(compositorElement) {
    return "[" +
            compositorElement.x + ", " +
            compositorElement.y + ", " +
            compositorElement.width + ", " +
            compositorElement.height +
            "]";
}


function isCompositorRuleApplicable(compositorElement, elementsList, rule) {
    var rules = rule.split("+");
    console.log(">>> IS COMPOSITOR RULE APPLICABLE?: >>>>>>>>" + rules[0] + " " + rules[1] + "====>" + compositorElement.subItemName);
    if (rules.length == 1) {
        if (rules[0] == compositorElement.subItemName) {
            console.log(">>>>>>>> YES-case 1:")
            return true;
        } else if (rules[0] == "other") {
            return false;//return false, this "other" case is treated separately
        } else {
            return false;
        }
    } else {
        if (rules[0] == compositorElement.subItemName) {
            if (rules[1] == "other") {
                if (elementsList.length == 1) {
                    return false;
                } else {
                    for (var j = 0; j < elementsList.length; j++) {
                        console.log(">>>>>>>> YES-case 2:" + elementsList[j].subItemName);
                    }
                    return true;
                }
            } else {
                for (var i = 0; i < elementsList.length; i++) {
                    if (rules[1] == elementsList[i].subItemName) {
                        console.log(">>>>>>>> YES-case 3:")
                        return true;
                    }
                }
                return false;
            }
        }
    }
    return false;
}

/**
 * Apply layout algorightm based on:
 * Surfaces preffered size
 * Surfaces preffered position
 * Internal priorities roules
 */
function compositorReLayout() {
    var topSpace        = 0;
    var bottomSpace     = 0;
    var middleElements  = 0;

    // checked preffered possition of elements
    // check top/bottom
    // then left the middle area for everything else

    //Available rectangle for surfaces (0,0 -> 100%, 100% of output)
    var y= 0;
    var x = 0;
    var width = output.width;
    var height = output.height;

    var targetX;
    var targetY;
    var targetW;
    var targetH;

    var other = new Array();
    for (var i = 0; i < compositorElements.length; i++) {
        other.push(compositorElements[i]);
    }

    console.log("[DEBUG] LAYOUT BEGIN ===================================================================")
    for (var ruleKey in compositorRules) {
        var ruleValue = compositorRules[ruleKey];
        var otherNew = new Array();
        //console.log("[DEBUG] Compositor rule: " + ruleKey + " -> " + ruleValue);
        for (var i = 0; i < other.length; i++) {
            console.log("[DEBUG]    Check if rule apply to:" + other[i].subItemName + " :index of other:" + ruleKey.indexOf("other") + "other lenght:" + other.length);
            if (isCompositorRuleApplicable(other[i], other, ruleKey)) {
                console.log("[DEBUG] --> is applicable");
                var percent = parseInt(ruleValue.split(";")[0]);
                if (ruleValue.indexOf("top") > -1) {
                    targetH = height * percent / 100;
                    targetW = width;
                    targetX = x;
                    targetY = y;
                    //console.log("[DEBUG]        Align top:" + other[i].subItemName + compositorElementPositionToString(other[i]));
                    //calculate remaining rectangle
                    x = x;
                    y = y + targetH;
                    width = width;
                    height = height - targetH;
                    //resize
                    other[i].newSize(targetX, targetY, targetW, targetH);
                } else if (ruleValue.indexOf("bottom") > -1) {
                    targetH = height * percent / 100;
                    targetW = width;
                    targetX = x;
                    targetY = y + height - targetH;
                    //console.log("[DEBUG]        Align bottom:" + other[i].subItemName + compositorElementPositionToString(other[i]));
                    //calculate remaining rectangle
                    x = x;
                    y = y;
                    width = width;
                    height = height - targetH;
                    //resize
                    other[i].newSize(targetX, targetY, targetW, targetH);
                } else {
                    targetX = x;
                    targetY = y;
                    if (width > height) {
                        targetW = width * percent / 100;
                        targetH = height;
                        //calculate remaining rectangle
                        x = x + targetW;
                        y = y;
                        width = width - targetW;
                        height = height;
                    } else {
                        targetW = width;
                        targetH = height * percent / 100;
                        //calculate remaining rectangle
                        x = x;
                        y = y + targetH;
                        width = width;
                        height = height - targetH;
                    }
                    //resize
                    other[i].newSize(targetX, targetY, targetW, targetH);
                    console.log("[DEBUG]        Align else:" + other[i].subItemName + compositorElementPositionToString(other[i]));
                }
            } else if (ruleKey == "other") {
                console.log("[DEBUG] Apply rule other to:->>>" + other[i].subItemName);
                var percent = parseInt(ruleValue.split(";")[0]);
                console.log("[DEBUG] Percent:" + percent);
                targetX = x;
                targetY = y;
                if (width > height) {
                    targetW = width * percent / 100;
                    targetH = height;
                    //calculate remaining rectangle
                    x = x + targetW;
                    y = y;
                    width = width - targetW;
                    height = height;
                    console.log("[DEBUG]    ---->1    " + targetX + "/" + targetY + "/" + targetW + "/" + targetH);
                } else {
                    targetW = width;
                    targetH = height * percent / 100;
                    //calculate remaining rectangle
                    x = x;
                    y = y + targetH;
                    width = width;
                    height = height - targetH;
                    console.log("[DEBUG]    ---->2    "  + targetX + "/" + targetY + "/" + targetW + "/" + targetH);
                }
                //resize
                other[i].newSize(targetX, targetY, targetW, targetH);
                console.log("[DEBUG]    ---->    Align other:" + other[i].subItemName + compositorElementPositionToString(other[i]));
            } else {
                otherNew.push(other[i]);
            }
        }
        other = new Array();
        for (var i = 0; i < otherNew.length; i++) {
            console.log("[DEBUG] -----> other[" + i + "]->" + otherNew[i].subItemName);
            other.push(otherNew[i]);
        }
        //do until other list is empty
    }
    console.log("[DEBUG] LAYOUT END ===================================================================\n\n\n");

//        //console.log("compositorElements[" + i + "]=" + xinspect(compositorElements[i]));
//        if (compositorElements[i].prefferedPosition == "top") {
//            compositorElements[i].y = output.y;
//            compositorElements[i].x = 0;
//            compositorElements[i].width = output.width;
//            compositorElements[i].height = 50;
//            topSpace = 50;
//        } else if (compositorElements[i].prefferedPosition == "bottom") {
//            compositorElements[i].y = output.height - 50;
//            compositorElements[i].x = 0;
//            compositorElements[i].width = output.width;
//            compositorElements[i].height = 50;
//            bottomSpace = 50;
//        } else {
//            middleElements += 1;
//        }
//    }
//    console.log("Middle elements:" + middleElements + " / " + output.width + " ..." + (output.width / middleElements));
//    var middleElement = 0;
//    for (var i = 0; i < compositorElements.length; i++) {
//        var delta = output.width / middleElements;

//        if (compositorElements[i].prefferedPosition != "top" &&
//                compositorElements[i].prefferedPosition != "bottom") {
//            compositorElements[i].x = (middleElement * delta);
//            compositorElements[i].y = topSpace;
//            compositorElements[i].width = delta;
//            compositorElements[i].height = output.height - (topSpace + bottomSpace);
//            console.log("Do layout for window:" + i + "[" +
//                        compositorElements[i].x + "," +
//                        compositorElements[i].y + "," +
//                        compositorElements[i].width + "," +
//                        compositorElements[i].height + "," +
//                        + "]");
//            middleElement += 1;
//        }
//    }
}
