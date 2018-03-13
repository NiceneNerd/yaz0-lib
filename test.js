/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

let yaz0 = require("./bindings.js");
const fs = require("fs");

//let filePath = "M:/Documents/roms/wiiu/unpacked/TEST/unpacked/Enemy_Guardian_A.sbfres";
let filePath = "/home/max/Documents/TEST/yaz0/DgnMrgPrt_Dungeon018.sbfres";
let buff = fs.readFileSync(filePath);

//let filePath = "M:/Documents/roms/wiiu/unpacked/TEST/Dungeon001/Model/DgnMrgPrt_Dungeon001.sbfres";

let buffOut = yaz0.decode(buff);
/*
console.log(buffOut);
console.log("Length: " + buffOut.length);
console.log("");
fs.writeFileSync(filePath + ".out.bin", buffOut);
*/

let buffCompr = yaz0.encode(buffOut);
console.log(buffCompr);

fs.writeFileSync(filePath + ".yaz0", buffCompr);
