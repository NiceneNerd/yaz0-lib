/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

const assert = require('assert');
const yaz0   = require("./../bindings.js");

const Helper_Functions = require("./helper_functions.js");

let testFunctions = (helper) =>
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

        it('correct byte-order (big)', function() 
        {   
            let testLength = 0x123;
            let testData = new Array(testLength);
            testData.fill(0);

            helper.encodeAndAssert(testData,
            [ 
                0x59, 0x61, 0x7a, 0x30, 0x00, 0x00, 0x01, 0x23, // length should be Big-endian
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
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
        it('simple copy n<3, shouldn\'t copy (2,2,x,2,2)', function() 
        {   
            helper.encodeAndAssert([
                0x02,0x02, 0x04, 0x02,0x02, 
            ],[ 
                0b11111000,
                0x02, 0x02, 0x04, 0x02, 0x02
            ]);
        });

        it('simple copy n=3, offset=4, start=0 (2,2,2,x,2,2,2)', function() 
        {   
            helper.encodeAndAssert([
                0x02,0x02,0x02, 0x04, 0x02,0x02,0x02, 
            ],[ 
                0b11110000,
                0x02, 0x02, 0x02, 0x04,
                ...helper.copyBytes(4, 3)         
            ]);
        });

        it('simple copy n=3, offset=4, start=2 (x,x,2,2,2,x,2,2,2)', function() 
        {   
            helper.encodeAndAssert([
                0xA1,0xA2, 
                0x02,0x02,0x02, 0x04, 0x02,0x02,0x02, 
            ],[ 
                0b11111100,
                0xA1, 0xA2, 
                0x02, 0x02, 0x02, 0x04,
                ...helper.copyBytes(4, 3)         
            ]);
        });

        it('simple copy n=20, offset=4 (1...20,x,1...20)', function() 
        {   
            let N = 20;
            let testArray = new Array(N + 1 + N);
            testArray[N] = 0x04;

            for(let i=0; i<N; ++i)
                testArray[i] = testArray[i+N+1] = i+1;

            helper.encodeAndAssert(testArray,
            [ 
                0b11111111,   1,  2,  3,  4,  5,  6,  7,  8,
                0b11111111,   9, 10, 11, 12, 13, 14, 15, 16,
                0b11111000,  17, 18, 19, 20, 0x04,
                ...helper.copyBytes(N + 1, N)         
            ]);
        });

        it('repeating copy n=5 at start (1,1,1,1,1,0)', function() 
        {   
            helper.encodeAndAssert([
                1,1,1,1,1, 0
            ],[ 
                0b10100000, 
                0x01, ...helper.copyBytes(1, 4),
                0x00
            ]);
        });

        it('repeating copy n=5 at end (0,1,1,1,1,1)', function() 
        {   
            helper.encodeAndAssert([
                0, 1,1,1,1,1
            ],[ 
                0b11000000, 
                0x00,
                0x01 , ...helper.copyBytes(1, 4),
            ]);
        });

        it('repeating copy n=5 at middle (0,1,1,1,1,1,0)', function() 
        {   
            helper.encodeAndAssert([
                0, 1,1,1,1,1, 0
            ],[ 
                0b11010000, 
                0x00,
                0x01 , ...helper.copyBytes(1, 4),
                0x00,
            ]);
        });

        it('repeating copy n=6, whole stream (1...1)', function() 
        {   
            helper.encodeAndAssert([
                1,1,1, 1,1,1
            ],[ 
                0b10000000, 0x01,
                ...helper.copyBytes(1, 5)         
            ]);
        });

        it('repeating copy n > max. copy length, should split up', function() 
        {   
            let testLength = 0x222;
            let testData = new Array(testLength);
            testData.fill(0);

            helper.encodeAndAssert(testData,
            [ 
                0b10000000,
                0, ...helper.copyBytes(1, 0x111),
                ...helper.copyBytes(1, 0x110)
            ]);
        });

        it('multi-byte repeating copy at start (1,2,3, 2,3, 2,3,..)', function() 
        {   
            helper.encodeAndAssert([
                1,2,3, 2,3, 2,3, 2,3
            ],[ 
                0b11100000, 
                1,2,3,
                ...helper.copyBytes(2, 6),
            ]);
        });
    });

    describe('encoding (rules)', function() 
    {
        it('search for biggest copy length (last is best)', function() 
        {   
            // it should pick up the second numbers for copying because it's longer
            helper.encodeAndAssert([
                1,2,3, 0xA0, 1,2,3,4, 0xA1, 1,2,3,4
            ],[ 
                0b11110110, 
                1,2,3, 0xA0,
                ...helper.copyBytes(4, 3),
                4, 0xA1,
                ...helper.copyBytes(5, 4)
            ]);
        });

        it('search for biggest copy length (first is best)', function() 
        {   
            // the last copy should go back to the beginning (1-4) 
            // and shouldn't pick up the (1-3)
            helper.encodeAndAssert([
                1,2,3,4, 0xA0, 1,2,3, 0xA1, 1,2,3,4
            ],[ 
                0b11111010, 
                1,2,3,4, 0xA0,
                ...helper.copyBytes(5, 3),
                0xA1,
                ...helper.copyBytes(9, 4)
            ]);
        });

        it('check longer repeating copy over normal copies (@mid)', function() 
        {   
            helper.encodeAndAssert([
                1,1,1,1, 0xA1, 1,1,1,1, 1,1,1,1, 0xA2
            ],[ 
                0b10110100, 
                1, ...helper.copyBytes(1, 3),
                0xA1,
                1, ...helper.copyBytes(1, 7),
                0xA2
            ]);
        });

        it('check longer repeating copy over normal copies (@end)', function() 
        {   
            helper.encodeAndAssert([
                1,1,1,1, 0xA1, 1,1,1,1, 1,1,1,1
            ],[ 
                0b10110000, 
                1, ...helper.copyBytes(1, 3),
                0xA1,
                1, ...helper.copyBytes(1, 7)
            ]);
        });

        it('ignore shorter repeating copy over normal copies', function() 
        {   
            helper.encodeAndAssert([
                1,1,1,1,1, 0xA2, 1,1,1,1,
            ],[ 
                0b10100000, 
                1, ...helper.copyBytes(1, 4),
                0xA2,
                ...helper.copyBytes(5, 4)
            ]);
        });

        it('compare copy with possible future copies (TODO!)', function() 
        {   
            helper.encodeAndAssert([
                0xA0, 1,1,1,1,1,1, 
                0xA1, 5,1,1,1,
                0xA2, 5,1,1,1,1,1
            ],
            /*[  // correct, would require recursively checking for better copies by branching off and skipping the copy
                0b11011011,
                0xA0, 1, ...helper.copyBytes(1, 5),
                0xA1, 5, ...helper.copyBytes(5, 3),
                0xA2, 5,
                0b00000000,
                ...helper.copyBytes(12, 5)
            ],*/
            [  // currently
                0b11011010,
                0xA0, 1, ...helper.copyBytes(1, 5),
                0xA1, 5, ...helper.copyBytes(5, 3),
                0xA2, ...helper.copyBytes(5, 4),
                0b11000000,
                1,1
            ]);
        });
    });
};

let helper = new Helper_Functions();
helper.decodeTests = false;
describe('Yaz0 encoding', () => { testFunctions(helper) });

let helperDecode = new Helper_Functions();
helperDecode.decodeTests = true;
describe('Yaz0 decode encoded tests', () => { testFunctions(helperDecode) });
