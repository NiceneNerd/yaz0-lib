/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

const assert = require('assert');
const yaz0   = require("./../bindings.js");

const Helper_Functions = require("./helper_functions.js");
let helper = new Helper_Functions();

describe('Yaz0 encoding', function() 
{
    describe('general', function() 
    {
        it('correct header & length', function() 
        {   
            helper.encodeAndAssert([
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 
            ],[ 
                0x59, 0x61, 0x7a, 0x30, 0x00, 0x00, 0x00, 0x08, // "Yaz0" + 8:int
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 8 padding zeros
            ], 0, 16);
        });

        it('empty buffer / 0 bytes', function() 
        {   
            helper.encodeAndAssert([/* nothing */],[ 
                0x59, 0x61, 0x7a, 0x30, 0x00, 0x00, 0x00, 0x00, // "Yaz0" + 8:int
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 8 padding zeros
                0x00
            ], 0);
        });
    });

    describe('encoding (simple)', function() 
    {
        it('end mid-header / 6 bytes (0x00..0x05)', function() 
        {   
            helper.encodeAndAssert([
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05
            ],[ 
                0b11111100,
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05
            ]);
        });

        it('mid-header after full header - value reset', function() 
        {   
            helper.encodeAndAssert([
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x10, 0x11, 0x12, 0x13
            ],[ 
                0b11111111,
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0b11110000,
                0x10, 0x11, 0x12, 0x13
            ]);
        });

        it('only value bytes (0x00..0x07)', function() 
        {   
            helper.encodeAndAssert([
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
            ],[ 
                0b11111111,
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
            ]);
        });

    });

    describe('encoding (copying)', function() 
    {
        it('simple copy n<3, shouldn\'t copy (2,2,4,2,2)', function() 
        {   
            helper.encodeAndAssert([
                0x02,0x02, 0x04, 0x02,0x02, 
            ],[ 
                0b11111000,
                0x02, 0x02, 0x04, 0x02, 0x02
            ]);
        });
    });

    describe('encoding (copying)', function() 
    {
        it('simple copy n=3, offset=4 (2,2,2,4,2,2,2)', function() 
        {   
            helper.encodeAndAssert([
                0x02,0x02,0x02, 0x04, 0x02,0x02,0x02, 
            ],[ 
                0b11110000,
                0x02, 0x02, 0x02, 0x04,
                ...helper.copyBytes(4, 3)         
            ]);
        });
    });
});
