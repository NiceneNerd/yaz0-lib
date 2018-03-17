/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

let yaz0 = require("./bindings.js");
const fs = require("fs");

let buffA = fs.readFileSync("/home/max/Documents/TEST/yaz0/DgnMrgPrt_Dungeon018.sbfres.out.bin");
let buffB = fs.readFileSync("/home/max/Documents/TEST/yaz0/DgnMrgPrt_Dungeon018.sbfres.yaz0.out.bin");

if(buffA.length != buffB.length)
{
    console.log("Different file size: %d != %d", buffA.length, buffB.length);
    return;
}

for(let i=0; i<buffA.length; ++i)
{
    if(buffA[i] != buffB[i])
    {
        console.log(`Difference @${i}: ${buffA[i]} != ${buffB[i]}`);
        break;
    }
}

return;

//let filePath = "M:/Documents/roms/wiiu/unpacked/TEST/unpacked/Enemy_Guardian_A.sbfres";
let filePath = "/home/max/Documents/TEST/yaz0/DgnMrgPrt_Dungeon018.sbfres.yaz0";
let buff = fs.readFileSync(filePath);

//let filePath = "M:/Documents/roms/wiiu/unpacked/TEST/Dungeon001/Model/DgnMrgPrt_Dungeon001.sbfres";

let buffOut = yaz0.decode(buff);
console.log(buffOut);
//return;

console.log(buffOut);
console.log("Length: " + buffOut.length);
console.log("");
fs.writeFileSync(filePath + ".out.bin", buffOut);
return;

let buffCompr = yaz0.encode(buffOut);
console.log(buffCompr);

fs.writeFileSync(filePath + ".yaz0", buffCompr);

