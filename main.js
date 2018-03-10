/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

let yaz0 = require("./bindings.js");
const fs = require("fs");

//let filePath = "M:/Documents/roms/wiiu/unpacked/TEST/unpacked/Enemy_Guardian_A.sbfres";
let filePath = "/home/max/Documents/TEST/sarc_test/dng08/Model/DgnMrgPrt_Dungeon018.sbfres";
let buff = fs.readFileSync(filePath);

//let filePath = "M:/Documents/roms/wiiu/unpacked/TEST/Dungeon001/Model/DgnMrgPrt_Dungeon001.sbfres";

let buffOut = yaz0.decode(buff, 300);
console.log(buffOut);
console.log(buffOut.length);

let buffCompr = yaz0.encode(buffOut, buffOut.length);
console.log(buffCompr);

//fs.writeFileSync("out.bin", buffOut);

/*
let buffOut4 = yaz0.decode(buff, 4);
console.log(buffOut4);
console.log(buffOut4.length);
*/

/*
magic:"Yaz0"
size:17401344
*/
