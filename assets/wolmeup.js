var mac = [[0,0],[0,0],[0,0]];

function setMac(macNum, newMacString) {
    // strip the mac address
    //mac[macNum-1][0] = newMac & (Math.pow(16,12)-Math.pow(16,6));
    //mac[macNum-1][1] = newMac & Math.pow(16,6);
    mac[macNum-1][0] = parseInt(newMacString.substr(0,6),16);
    mac[macNum-1][1] = parseInt(newMacString.substr(6,6),16);
}

function getMac(macNum) {
    // form the mac address
    var tempMac = mac[macNum-1][0]*Math.pow(16,6) + mac[macNum-1][1];
    return tempMac;
}

// function to move the number up (without carrying over)
function macIncr(macNum, macDigit){
    var loru = macDigit > 6 ? 0 : 1; // lower or upper?
    var tempDigit = macDigit > 6 ? macDigit-6 : macDigit;
    // use bitmask
    var bitmask = (Math.pow(16,tempDigit)) - (Math.pow(16,tempDigit-1));
    if( (mac[macNum-1][loru]&bitmask) == (bitmask) ){
        // if it's an 0xf, reduce by 0xf
        mac[macNum-1][loru] = mac[macNum-1][loru] - (bitmask);
    }else{
        //..otherwise add one
        mac[macNum-1][loru] = mac[macNum-1][loru] + Math.pow(16,tempDigit-1);
    }
}

// function to move the number down (without carrying over)
function macDecr(macNum, macDigit){
    var loru = macDigit > 6 ? 0 : 1; // lower or upper?
    var tempDigit = macDigit > 6 ? macDigit-6 : macDigit;
    // use bitmask
    var bitmask = (Math.pow(16,tempDigit)) - (Math.pow(16,tempDigit-1));
    if( (mac[macNum-1][loru]&bitmask) == (0) ){
        // if it's a 0, add 0xf
        mac[macNum-1][loru] = mac[macNum-1][loru] + (bitmask);
    }else{
        //..otherwise subtract one
        mac[macNum-1][loru] = mac[macNum-1][loru] - Math.pow(16,tempDigit-1);
    }
}


// mac address to string
function macToStr(macNum) {
    var myMacStr1 = mac[macNum-1][0].toString(16);
    var myMacStr2 = mac[macNum-1][1].toString(16);
    if(myMacStr1.length < 6){
        var tempArr = new Array(6-myMacStr1.length+1).join("0");
        myMacStr1 = tempArr + myMacStr1;
    }
    if(myMacStr2.length < 6){
        var tempArr2 = new Array(6-myMacStr2.length+1).join("0");
        myMacStr2 = tempArr2 + myMacStr2;
    }

    return (myMacStr1+myMacStr2).toUpperCase();
}
