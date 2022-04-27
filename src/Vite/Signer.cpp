// Copyright © 2019 Mart Roosmaa.
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../Base64.h"
#include "../uint256.h"

#include <boost/multiprecision/cpp_int.hpp>

using namespace TW;

namespace TW::Vite {

enum AddressType {
    Illegal = 0,
    Account = 1,
    Contract = 2
};

AddressType addressType(const std::string& address) {
    auto checksum = address.substr(45);
    auto addr = address.substr(5, 40);
    auto accountChecksum = hex(Hash::blake2b(parse_hex(addr + "00").data(), 21, 5));
    if (checksum == accountChecksum) return Account;
    auto contractChecksum = hex(Hash::blake2b(parse_hex(addr + "01").data(), 21, 5));
    if (checksum == contractChecksum) return Contract;
    return Illegal;
}

void appendAddress(Data msg, const std::string& address) {
        switch(addressType(address)) {
            case Illegal:
                break;
            case Account:
                append(msg, parse_hex(address.substr(5) + "00"));
                break;
            case Contract:
                append(msg, parse_hex(address.substr(5) + "01"));
                break;
        }
}

Data hashBlockData(const Proto::SigningInput& input) {
    Data msg = Data();
    append(msg, store(uint256_t(input.blocktype()), 1));
    append(msg, parse_hex(input.previoushash().substr(2)));
    append(msg, store(uint256_t(input.height()), 8));
    appendAddress(msg, input.address());
    switch(input.block_case()) {
        case Proto::SigningInput::kRequestBlock: {
            appendAddress(msg, input.requestblock().toaddress());
            append(msg, store(uint256_t(input.requestblock().amount()), 32));
            if (input.requestblock().tokenid().substr(0, 4) == "tti_")
                append(msg, parse_hex(input.requestblock().tokenid().substr(4)));
            break;
        }
        case Proto::SigningInput::kSendBlock: {
            append(msg, parse_hex(input.sendblock().sendblockhash()));
            break;
        }
        case Proto::SigningInput::BLOCK_NOT_SET: {
          break;
        }
    }
    Data data = Base64::decode(input.data());
    if (data.size() != 0) {
      append(msg, Hash::blake2b(data.data(), data.size(), 32));
    }
    append(msg, store(uint256_t(input.fee()), 32));
    append(msg, parse_hex(input.vmloghash()));
    append(msg, store(uint256_t(Base64::decode(input.nonce())), 8));
    for (int i = 0; i < input.triggeredsendblocklist_size(); i++) {
        append(msg, parse_hex(input.triggeredsendblocklist(i)));
    }
    return Hash::blake2b(msg.data(), msg.size(), 32);
}

Signer::Signer(const Proto::SigningInput& input)
  : privateKey(Data(input.privatekey().begin(), input.privatekey().end())),
    input(input),
    blockHash(hashBlockData(input)) {}


Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    Proto::SigningOutput output;
    try {
        auto signer = Signer(input);
        output = signer.build();
    }
    catch (...) {}
    return output;
}

Data Signer::sign() const noexcept {
    auto digest = Data(blockHash.begin(), blockHash.end());
    auto sig = privateKey.sign(digest, TWCurveED25519Blake2bNano);

    return sig;
}

Proto::SigningOutput Signer::build() const {
    auto output = Proto::SigningOutput();
    const auto signature = sign();
    output.set_encoded(signature.data(), signature.size());

    return output;
}

} // namespace TW::Vite
