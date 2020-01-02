// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Contract.h"
#include "Cell.h"

#include "../PrivateKey.h"
#include "../PublicKey.h"
#include "../HexCoding.h"
#include "../Hash.h"

#include <memory>
#include <iostream>
#include <cassert>

namespace TW::TON {

using namespace TW;
using namespace std;

/*
Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    // ...

    auto protoOutput = Proto::SigningOutput();
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubkey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto from = Address(pubkey);

    // ...
    
    return protoOutput;
}

Data Signer::sign(const PrivateKey &privateKey, Transaction &transaction) noexcept {
    // ...

    return Data();
}
*/

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

    // build cell for message
    Slice s;
    s.appendBits(parse_hex("88"), 7); // 7 bits b{1000100}
    // address (chainId + hash)
    s.appendBytes(data(&chainId, 1));
    s.appendBytes(stateInitHash);
    // 12 bits:  b{000010} b{00110} b{0}
    s.appendBits(parse_hex("08c0"), 12);
    s.appendBytes(signature);
    s.appendBytes(msg);
    //cerr << s.size() << " " << s.asBytesStr() << endl;

    Cell c;
    c.setSlice(s);
    c.addCell(stateInit.getCells()[0]);
    c.addCell(stateInit.getCells()[1]);

    // serialize it
    Data ss2;
    c.serialize(ss2, Cell::SerializationMode::WithCRC32C);

    return ss2;
}

} // namespace TW::TON
