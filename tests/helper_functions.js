/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

const assert = require('assert');
const yaz0   = require("./../bindings.js");

const YAZ0_HEADER_SIZE = 16;

module.exports = class Helper_Functions
{
    constructor()
    {
        this.decodeTests = false;
    }

    printBuffer(buff, name = "Buffer")
    {
        let tabs = "".padStart(6);

        console.log(`\n${tabs}== ${name} ${buff.length} ==`);

        let str = "";
        for(let i=0; i<buff.length; ++i)
        {
            str += buff.toString('hex', i, i+1) + " ";

            if((i+1) % 8 == 0) 
            {
                console.log(tabs + str);
                str = "";
            }
        }
        console.log(tabs + str);
    }
    
    /**
     * @param {Buffer} buffYaz 
     * @param {Buffer} buffData 
     */
    assertData(buffYaz, buffData, offset = null, length = null,)
    {
        return assert(buffData.compare(
            buffYaz, 
            offset != null ? offset : YAZ0_HEADER_SIZE, 
            length != null ? length : buffYaz.length
        ) == 0);
    }

    /**
     * 
     * @param {Buffer} bufferIn 
     * @param {Buffer} bufferTest 
     * @param {Integer} length 
     */
    encodeAndAssert(bufferInArray, bufferTestArray, offset = null, length = null,)
    {
        let bufferIn = Buffer.from(bufferInArray);
        let buffCompr = yaz0.encode(bufferIn);
        let bufferDecoded = null;

        try{
            this.assertData(buffCompr, Buffer.from(bufferTestArray), offset, length);
            if(this.decodeTests)
            {
                bufferDecoded = yaz0.decode(buffCompr);
                
                assert(bufferDecoded instanceof Buffer);
                assert(bufferDecoded.compare(bufferIn) == 0);
            }
        }catch(e)
        {
            this.printBuffer(Buffer.from(bufferInArray), "Input-Buffer");
            this.printBuffer(buffCompr, "Yaz0-Buffer");
            this.printBuffer(Buffer.from(bufferTestArray), "Test-Buffer");

            if(bufferDecoded)
                this.printBuffer(bufferDecoded, "Decoded-Buffer");  

            console.log("");

            throw e;
        }
    }

    decodeAndAssert(bufferInArray, bufferTestArray)
    {
        let bufferIn = Buffer.from(bufferInArray);
        let buffUncompr = yaz0.decode(bufferIn);

        try{
            this.assertData(buffUncompr, Buffer.from(bufferTestArray), 0);
        }catch(e)
        {
            this.printBuffer(Buffer.from(bufferInArray), "Input-Buffer");
            this.printBuffer(buffUncompr, "Yaz0-Buffer");
            this.printBuffer(Buffer.from(bufferTestArray), "Test-Buffer");
            console.log("");

            throw e;
        }
    }

    copyBytes(offset, length)
    {
        let bytes = [0,0];

        --offset;
        if(length > 0x11) // 2 bytes
        {
            bytes.push((length - 0x12) & 0xFF);
        }else{ // 3 bytes
            bytes[0]  = ((length-2)  & 0xF) << 4;
        }

        bytes[0] |= (offset >> 8) & 0xF;
        bytes[1]  = offset & 0xFF;

        return bytes;
    }

    createHeader(length)
    {
        return [
            0x59, 0x61, 0x7a, 0x30, 
            ((length >> 24) & 0xFF), ((length >> 16) & 0xFF), ((length >> 8) & 0xFF), (length & 0xFF), // "Yaz0"
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        ];
    }
};
