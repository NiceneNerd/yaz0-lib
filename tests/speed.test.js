return;

/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

let yaz0 = require("./../bindings.js");
const fs = require("fs");

const testFiles = [
    //"/home/max/Documents/TEST/yaz0/DgnObj_AncientBallSwitch_A-00.sbfres", // 9sec

    //====== TIME ======
    //Total: 3372.91ms
    //Byte : 0.0468512ms/byte
    "/home/max/Documents/TEST/yaz0/Animal_Goat.sbfres", // 3,3sec
];

for(let filePath of testFiles)
{
    let buffIn = fs.readFileSync(filePath);

    console.log("File-path: " + filePath);
    console.log("File-size: " + buffIn.length);
    
    for(let i=0; i<3; ++i)
    {
        let startTime = process.hrtime();
        let buffCompr = yaz0.encode(buffIn);

        let timeDiff = process.hrtime(startTime);
        let msDiff = (timeDiff[0] * 1000) + (timeDiff[1] / 1000000);
        let msPerByte = msDiff / buffIn.length;

        console.log("====== TIME ======");
        console.log(`Total: ${msDiff.toPrecision(6)}ms`);
        console.log(`Byte : ${msPerByte.toPrecision(6)}ms/byte`);
        console.log(`Output-size: ${(buffCompr.length/1024).toPrecision(4)}kb`);
    }
    //fs.writeFileSync(filePath + ".yaz0", buffCompr);
}
