/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/*
 * testByteBuffer.cpp
 *
 *  Created on: Oct 20, 2010
 *      Author: Miha Vitorovic
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>

#include <epicsUnitTest.h>
#include <testMain.h>

#include <pv/byteBuffer.h>
#include <pv/pvIntrospect.h>

using namespace epics::pvData;
using std::string;
using std::cout;

static
void testBasicOperations() {
    std::auto_ptr<ByteBuffer> buff(new ByteBuffer(32));

    testOk1(buff->getSize()==32);

    testOk1(buff->getPosition()==0);
    testOk1(buff->getLimit()==32);
    testOk1(buff->getRemaining()==32);

    buff->putBoolean(true);
    testOk1(buff->getPosition()==1);
    testOk1(buff->getRemaining()==31);

    buff->putByte(-12);
    testOk1(buff->getPosition()==2);
    testOk1(buff->getRemaining()==30);

    buff->putShort(10516);
    testOk1(buff->getPosition()==4);
    testOk1(buff->getRemaining()==28);

    buff->putInt(0x1937628B);
    testOk1(buff->getPosition()==8);
    testOk1(buff->getRemaining()==24);

    buff->putLong(2345678123LL);
    testOk1(buff->getPosition()==16);
    testOk1(buff->getRemaining()==16);

    float testFloat = 34.67f;
    buff->putFloat(testFloat);
    testOk1(buff->getPosition()==20);
    testOk1(buff->getRemaining()==12);

    double testDouble = -512.23974;
    buff->putDouble(testDouble);
    testOk1(buff->getPosition()==28);
    testOk1(buff->getRemaining()==4);

    // testing direct reads
    testOk1(buff->getBoolean(0)==true);
    testOk1(buff->getByte(1)==-12);
    testOk1(buff->getShort(2)==10516);
    testOk1(buff->getInt(4)==0x1937628B);
    testOk1(buff->getLong(8)==2345678123LL);
    testOk1(buff->getFloat(16)==testFloat);
    testOk1(buff->getDouble(20)==testDouble);

    buff->flip();
    testOk1(buff->getLimit()==28);
    testOk1(buff->getPosition()==0);
    testOk1(buff->getRemaining()==28);

    testOk1(buff->getBoolean()==true);
    testOk1(buff->getPosition()==1);

    testOk1(buff->getByte()==-12);
    testOk1(buff->getPosition()==2);

    testOk1(buff->getShort()==10516);
    testOk1(buff->getPosition()==4);

    testOk1(buff->getInt()==0x1937628B);
    testOk1(buff->getPosition()==8);

    testOk1(buff->getLong()==2345678123LL);
    testOk1(buff->getPosition()==16);

    testOk1(buff->getFloat()==testFloat);
    testOk1(buff->getPosition()==20);

    testOk1(buff->getDouble()==testDouble);
    testOk1(buff->getPosition()==28);

    buff->clear();
    testOk1(buff->getPosition()==0);
    testOk1(buff->getLimit()==32);
    testOk1(buff->getRemaining()==32);

    buff->setPosition(4);
    testOk1(buff->getPosition()==4);
    testOk1(buff->getLimit()==32);
    testOk1(buff->getRemaining()==(32-4));

    buff->setPosition(13);
    testOk1(buff->getPosition()==13);
    testOk1(buff->getLimit()==32);
    testOk1(buff->getRemaining()==(32-13));

    // testing absolute puts
    buff->clear();
    buff->setPosition(28);
    buff->putBoolean(0, true);
    buff->putByte(1, -12);
    buff->putShort(2, 10516);
    buff->putInt(4, 0x1937628B);
    buff->putLong(8, 2345678123LL);
    buff->putFloat(16, testFloat);
    buff->putDouble(20, testDouble);

    buff->flip();
    testOk1(buff->getLimit()==28);
    testOk1(buff->getPosition()==0);
    testOk1(buff->getRemaining()==28);

    testOk1(buff->getBoolean()==true);
    testOk1(buff->getPosition()==1);

    testOk1(buff->getByte()==-12);
    testOk1(buff->getPosition()==2);

    testOk1(buff->getShort()==10516);
    testOk1(buff->getPosition()==4);

    testOk1(buff->getInt()==0x1937628B);
    testOk1(buff->getPosition()==8);

    testOk1(buff->getLong()==2345678123LL);
    testOk1(buff->getPosition()==16);

    testOk1(buff->getFloat()==testFloat);
    testOk1(buff->getPosition()==20);

    testOk1(buff->getDouble()==testDouble);
    testOk1(buff->getPosition()==28);

    buff->clear();
    testOk1(buff->getPosition()==0);
    testOk1(buff->getLimit()==32);
    testOk1(buff->getRemaining()==32);

    char src[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
            'm' };
    char dst[] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            ' ' };

    buff->put(src, 2, 6);
    testOk1(buff->getPosition()==6);
    testOk1(strncmp(buff->getArray(),&src[2],6)==0);

    buff->flip();
    testOk1(buff->getLimit()==6);
    testOk1(buff->getPosition()==0);
    testOk1(buff->getRemaining()==6);

    buff->get(dst, 2, 6);
    testOk1(buff->getLimit()==6);
    testOk1(buff->getPosition()==6);
    testOk1(strncmp(&src[2],&dst[2],6)==0);

    cout<<"#    First 10 characters of destination: >>"<<string(dst, 10)<<"<<\n";
}

static
void testInverseEndianness() {
    testDiag("check byte swapping features");

#if EPICS_BYTE_ORDER==EPICS_ENDIAN_BIG
    std::auto_ptr<ByteBuffer> buf(new ByteBuffer(32,EPICS_ENDIAN_LITTLE));
    char refBuffer[] =
    {   (char)0x02, (char)0x01, (char)0x0D, (char)0x0C, (char)0x0B, (char)0x0A};
#else
    std::auto_ptr<ByteBuffer> buf(new ByteBuffer(32,EPICS_ENDIAN_BIG));
    ByteBuffer* buff = new ByteBuffer(32, EPICS_ENDIAN_BIG);
    char refBuffer[] = { (char)0x01, (char)0x02, (char)0x0A, (char)0x0B,
            (char)0x0C, (char)0x0D };
#endif

    buff->putShort(0x0102);
    buff->putInt(0x0A0B0C0D);

    testOk1(strncmp(buff->getArray(),refBuffer,6)==0);

    buff->flip();

    testOk1(buff->getShort()==0x0102);
    testOk1(buff->getInt()==0x0A0B0C0D);
}

static
void testSwap()
{
    testDiag("Check epics::pvData::swap<T>(v)");
    testOk1(swap<uint8>(0x80)==0x80);
    testOk1(swap<uint16>(0x7080)==0x8070);
    testOk1(swap<uint32>(0x10203040)==0x40302010);
    uint64 a = 0x10203040,
           b = 0x80706050;
    a <<= 32;
    b <<= 32;
    a |= 0x50607080;
    b |= 0x40302010;
    testOk1(swap<uint64>(a)==b);
}

static
void testUnaligned()
{
    testDiag("test correctness of unaligned access");

    ByteBuffer buf(32, EPICS_ENDIAN_BIG);

    // malloc() should give us a buffer aligned to at least native integer size
    buf.align(sizeof(int));
    testOk1(buf.getPosition()==0);

    buf.clear();
    buf.put<uint8>(0x42);
    buf.put<uint16>(0x1020);
    buf.align(2, '\x41');
    testOk1(buf.getPosition()==4);

    testOk1(memcmp(buf.getBuffer(), "\x42\x10\x20\x41", 4)==0);

    buf.clear();
    buf.put<uint8>(0x42);
    buf.put<uint32>(0x12345678);
    buf.align(4, '\x41');
    testOk1(buf.getPosition()==8);

    testOk1(memcmp(buf.getBuffer(), "\x42\x12\x34\x56\x78\x41\x41\x41", 8)==0);

    buf.clear();
    buf.put<uint8>(0x42);
    uint64 val = 0x12345678;
    val<<=32;
    val |= 0x90abcdef;
    buf.put<uint64>(val);
    buf.align(8, '\x41');
    testOk1(buf.getPosition()==16);

    testOk1(memcmp(buf.getBuffer(), "\x42\x12\x34\x56\x78\x90\xab\xcd\xef\x41\x41\x41", 8)==0);
}

MAIN(testByteBuffer)
{
    testPlan(93);
    testDiag("Tests byteBuffer");
    testBasicOperations();
    testInverseEndianness();
    testSwap();
    testUnaligned();
    return testDone();
}
