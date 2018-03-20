/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

const assert = require('assert');
var expect = require('chai').expect;
const yaz0   = require("./../bindings.js");

const Helper_Functions = require("./helper_functions.js");
const helper = new Helper_Functions();

describe('Yaz0 decoding', () =>
{
    describe('general', () =>
    {
        it('empty buffer / 0 bytes', function() 
        {   
            helper.decodeAndAssert([
                ...helper.createHeader(0), 0
            ],[]);
        });

        it('correct output size', function() 
        {   
            const size = 0x1230;
            helper.decodeAndAssert([
                ...helper.createHeader(size), 
                ...(new Array(size + (size/8))).fill(0xFF)
            ], new Array(size).fill(0xFF));
        });
    });

    describe('decoding', () =>
    {
        it('simple values', function() 
        {   
            helper.decodeAndAssert([
                ...helper.createHeader(4), 
                0b11110000,
                0xA1, 0xA2, 0xA3, 0xA4
            ],[
                0xA1, 0xA2, 0xA3, 0xA4
            ]);
        });

        it('multi header', function() 
        {   
            helper.decodeAndAssert([
                ...helper.createHeader(16), 
                0b11111111,
                1,2,3,4,5,6,7,8,
                0b11111111,
                11,12,13,14,15,16,17,18
            ],[
                1,2,3,4,5,6,7,8,11,12,13,14,15,16,17,18
            ]);
        });

        it('simple copy at the end', function() 
        {   
            helper.decodeAndAssert([
                ...helper.createHeader(6), 
                0b11100000,
                0xA0, 0xB0, 0xC0, ...helper.copyBytes(3, 3)
            ],[
                0xA0, 0xB0, 0xC0,  0xA0, 0xB0, 0xC0
            ]);
        });

        it('simple copy in the middle', function() 
        {   
            helper.decodeAndAssert([
                ...helper.createHeader(7), 
                0b11101000,
                0xAA, 0xBB, 0xCC, ...helper.copyBytes(3, 3), 0xDD
            ],[
                0xAA, 0xBB, 0xCC,  0xAA,0xBB,0xCC, 0xDD
            ]);
        });

        it('partially repeating copy', function() 
        {   
            helper.decodeAndAssert([
                ...helper.createHeader(6), 
                0b11100000,
                0xA0, 0xB0, 0xC0, ...helper.copyBytes(2, 3)
            ],[
                0xA0, 0xB0, 0xC0,  0xB0, 0xC0, 0xB0
            ]);
        });

        it('repeating copy', function() 
        {   
            helper.decodeAndAssert([
                ...helper.createHeader(6), 
                0b11100000,
                0xA0, 0xB0, 0xC0, ...helper.copyBytes(1, 3)
            ],[
                0xA0, 0xB0, 0xC0,  0xC0,0xC0,0xC0
            ]);
        });

        it('big copy', function() 
        {   
            let resultArray = new Array(101);
            resultArray.fill(0xAA);

            helper.decodeAndAssert([
                ...helper.createHeader(101), 
                0b10000000,
                0xAA, ...helper.copyBytes(1, 100)
            ], resultArray);
        });
    });

    describe('error handling', () =>
    {
        it('incomplete header', function() 
        {   
            expect(() => yaz0.decode(Buffer.from([42,42,42]))).throw("Error parsing file");
        });

        it('file-size in header too big', function() 
        {   
            expect(() => yaz0.decode(Buffer.from([
                ...helper.createHeader(10), 
                0b10000000,
                0xA0
            ]))).throw("Error parsing file");
        });

        it('file-size in header too big and no data', function() 
        {   
            expect(() => yaz0.decode(Buffer.from([
                ...helper.createHeader(10), 
                0b10101010,
            ]))).throw("Error parsing file");
        });

        it('header but no value', function() 
        {   
            expect(() => yaz0.decode(Buffer.from([
                ...helper.createHeader(1), 
                0b10000000
            ]))).throw("Error parsing file");
        });

        it('header but no copy-data', function() 
        {   
            expect(() => yaz0.decode(Buffer.from([
                ...helper.createHeader(1), 
                0b00000000
            ]))).throw("Error parsing file");
        });

        it('copy offset too big', function() 
        {   
            helper.decodeAndAssert([
                ...helper.createHeader(4), 
                0b10000000,
                0xA0, ...helper.copyBytes(10, 3)
            ], [
                0xA0, 0xA0, 0xA0, 0xA0
            ]);
        });

        it('copy length too big', function() 
        {   
            helper.decodeAndAssert([
                ...helper.createHeader(4), 
                0b10000000,
                0xA0, ...helper.copyBytes(10, 5)
            ], [
                0xA0, 0xA0, 0xA0, 0xA0
            ]);
        });

    });
});
