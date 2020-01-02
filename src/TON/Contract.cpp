// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Contract.h"
#include "../Data.h"
#include "../HexCoding.h"

#include <cassert>

namespace TW::TON {

using namespace TW;

//static const char* walletContract1 = "FF0020DDA4F260810200D71820D70B1FED44D0D31FD3FFD15112BAF2A122F901541044F910F2A2F80001D31F3120D74A96D307D402FB00DED1A4C8CB1FCBFFC9ED54";
//static const char* walletContract2 = "FF0020DDA4F260810200D71820D70B1FED44D0D7091FD709FFD15112BAF2A122F901541044F910F2A2F80001D7091F3120D74A97D70907D402FB00DED1A4C8CB1FCBFFC9ED54";
// Obtained from ton-lite-client new-wallet.fif, len=81
static const char* walletContract3 = "FF0020DD2082014C97BA9730ED44D0D70B1FE0A4F260810200D71820D70B1FED44D0D31FD3FFD15112BAF2A122F901541044F910F2A2F80001D31F3120D74A96D307D402FB00DED1A4C8CB1FCBFFC9ED54";
// Obtained from released testnet desktop wallet, len=96
//static const char* walletContract4 = "FF0020DD2082014C97BA9730ED44D0D70B1FE0A4F2608308D71820D31FD31FD31FF82313BBF263ED44D0D31FD31FD3FFD15132BAF2A15144BAF2A204F901541055F910F2A3F8009320D74A96D307D402FB00E8D101A4C8CB1FCB1FCBFFC9ED54";
Data Contract::walletContractDefault() {
    return parse_hex(walletContract3);
}

Cell Contract::createStateInit(const PublicKey& pubkey) {
    // smart contract code -- constant
    auto ccode = std::make_shared<Cell>();
    ccode->setSliceBytes(walletContractDefault());

    // data: 4 byte serial num (0), 32 byte public key
    Data data;
    append(data, Data(4));
    append(data, pubkey.bytes);
    assert(data.size() == 4 + 32);
    auto cdata = std::make_shared<Cell>();
    cdata->setSliceBytes(data);

    Cell stateInit;
    stateInit.setSliceBitsStr("30", 5); // b{00110}
    stateInit.addCell(ccode);
    stateInit.addCell(cdata);

    return stateInit;
}

} // namespace TW::TON
