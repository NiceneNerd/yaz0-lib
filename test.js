/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

let yaz0 = require("./bindings.js");
const fs = require("fs");
/*
//let buffA = fs.readFileSync("/home/max/Documents/TEST/yaz0/DgnMrgPrt_Dungeon018.sbfres.out");
//let buffB = fs.readFileSync("/home/max/Documents/TEST/yaz0/DgnMrgPrt_Dungeon018.sbfres.yaz0.out.bin");

let buffA = fs.readFileSync("/home/max/Documents/TEST/yaz0/DgnMrgPrt_Dungeon018.sbfres");
let buffB = fs.readFileSync("/home/max/Documents/TEST/yaz0/DgnMrgPrt_Dungeon018.sbfres.yaz0");

if(buffA.length != buffB.length)
{
    console.log("Different file size: %d != %d", buffA.length, buffB.length);
    return;
}

let diffs = 0;
let minSize = Math.min(buffA.length, buffB.length);
for(let i=0; i<minSize; ++i)
{
    if(buffA[i] != buffB[i])
    {
        console.log(`Difference @${i} | 0x${i.toString(16).toUpperCase()}: ${buffA[i]} != ${buffB[i]}`);

        if(++diffs > 3)
            break;
    }
}
*/
//return;

//let filePath = "M:/Documents/roms/wiiu/unpacked/TEST/unpacked/Enemy_Guardian_A.sbfres";
let filePath = "/home/max/Documents/TEST/yaz0/AncientBall.sbactorpack";
let buff = fs.readFileSync(filePath);

let buffOut = yaz0.decode(buff);

console.log(buffOut);
console.log("Length: " + buffOut.length);
console.log("");
fs.writeFileSync(filePath + ".out.bin", buffOut);

return;

let buffCompr = yaz0.encode(buffOut);
console.log(buffCompr);
fs.writeFileSync(filePath + ".yaz0", buffCompr);

