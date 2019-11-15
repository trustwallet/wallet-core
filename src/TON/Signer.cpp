// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Contract.h"
#include "Cell.h"

#include "../PrivateKey.h"
#include "../HexCoding.h"
#include "../Hash.h"

#include <boost/crc.hpp>  // for boost::crc_32_type

#include <memory>
#include <iostream>

namespace TW::TON {

using namespace TW;
using namespace std;

Data Signer::sign(const PrivateKey& privateKey, const Data& message) noexcept {
    auto signature = privateKey.sign(message, TWCurveED25519);
    //cerr << "sign " << signature.size() << " " << hex(signature) << endl;
    return signature;
}

TW::Data Signer::buildInitMessage(const PrivateKey& privkey) {
    // create pubkey
    PublicKey pubkey = privkey.getPublicKey(TWPublicKeyTypeED25519);
    // create address
    Address address = Address(pubkey);
    byte chainId = (byte)address.workchainId;

    // create msg
    Data msgData = parse_hex("00000000"); //data(string("TRUST"));

    Cell msgc;
    msgc.setSliceBytes(msgData);
    auto msgHash = msgc.hash();

    // sign
    auto signature = sign(privkey, msgHash);
    
    auto extMsg = buildInitMessage(chainId, pubkey, signature, msgData);
    return extMsg;
}

Data Signer::buildInitMessage(
    byte chainId, const PublicKey& pubkey, const Data& signature, const Data& msg
) {
    Cell stateInit = Contract::createStateInit(pubkey);
    assert(stateInit.cellCount() == 2);
    
    auto stateInitHash = stateInit.hash();

    Slice s;
    s.appendBits(parse_hex("88"), 7); // 7 bits b{1000100}
    s.appendBytes(data(&chainId, 1)); // chainid
    s.appendBytes(stateInitHash);
    s.appendBits(parse_hex("08c0"), 12); // 12 bits:  b{000010} b{00110} b{0}
    s.appendBytes(signature);
    s.appendBytes(msg);
    //cerr << s.size() << " " << s.asBytesStr() << endl;

    Cell c;
    c.setSlice(s);
    c.addCell(stateInit.getCells()[0]);
    c.addCell(stateInit.getCells()[1]);

    // simulate Cell serial
    Data ss;
    append(ss, parse_hex("b5ee9c72")); // magic
    ss.push_back(0x41);
    ss.push_back(0x01);
    ss.push_back(0x03);
    ss.push_back(0x01);
    ss.push_back(0x00);
    uint8_t len1 = s.size() + 4 + c.getCells()[0]->getSlice().size() + 2 + c.getCells()[1]->getSlice().size() + 2;
    //cerr << "len1 " << (int)len1 << endl;
    ss.push_back(len1);
    ss.push_back(0x00);
    // slice
    //uint16_t lens = s.size();
    //cerr << "lens " << (int)lens << endl;
    ss.push_back(0x02);
    ss.push_back(Cell::d2(s.sizeBits()));
    append(ss, s.data());
    ss.push_back(1); // ref1
    ss.push_back(2); // ref2
    // cell1
    ss.push_back(0);
    ss.push_back(Cell::d2(c.getCells()[0]->getSlice().sizeBits()));
    append(ss, c.getCells()[0]->getSlice().data());
    // cell2
    ss.push_back(0);
    ss.push_back(Cell::d2(c.getCells()[1]->getSlice().sizeBits()));
    append(ss, c.getCells()[1]->getSlice().data());
    // CRC
    //boost::crc_32_type  result;
    using crc_32c_type = boost::crc_optimal<32, 0x1EDC6F41, 0xFFFFFFFF, 0xFFFFFFFF, true, true>;
    crc_32c_type result;
    result.process_bytes(ss.data(), ss.size());
    uint32_t crc = result.checksum();
    //cerr << std::hex << crc() << endl;
    ss.push_back(crc & 0x000000FF);
    ss.push_back((crc & 0x0000FF00) >> 8);
    ss.push_back((crc & 0x00FF0000) >> 16);
    ss.push_back((crc & 0xFF000000) >> 24);
    
    //cerr << "final len " << ss.size() << endl;

    return ss;
}

} // namespace TW::TON
