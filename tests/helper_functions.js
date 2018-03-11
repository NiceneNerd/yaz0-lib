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
        return assert.equal(buffData.compare(
            buffYaz, 
            offset != null ? offset : YAZ0_HEADER_SIZE, 
            length != null ? length : buffYaz.length
        ), 0);
    }

    /**
     * 
     * @param {Buffer} bufferIn 
     * @param {Buffer} bufferTest 
     * @param {Integer} length 
     */
    encodeAndAssert(bufferInArray, bufferTestArray, offset = null, length = null,)
    {
        let buffCompr = yaz0.encode(Buffer.from(bufferInArray));

        try{
            this.assertData(buffCompr, Buffer.from(bufferTestArray), offset, length);
        }catch(e)
        {
            this.printBuffer(Buffer.from(bufferInArray), "Input-Buffer");
            this.printBuffer(buffCompr, "Yaz0-Buffer");
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
            bytes[0]  = ((length-3)  & 0xF) << 4;
        }

        bytes[0] |= (offset >> 8) & 0xF;
        bytes[1]  = offset & 0xFF;

        return bytes;
    }
};