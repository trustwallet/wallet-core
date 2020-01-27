// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Protobuf/ZilliqaMessage.pb.h"

#include "../Hash.h"
#include "../HexCoding.h"
#include "../uint256.h"

#include <cassert>

#include <nlohmann/json.hpp>

using namespace TW;
using namespace TW::Zilliqa;

static inline Data prependZero(Data& data) {
    if (data.size() < 16) {
        Data zero = Data(16 - data.size(), 0);
        zero.insert(zero.end(), data.begin(), data.end());
        return zero;
    }
    return Data(data);
}


Data Signer::getPreImage(const Proto::SigningInput& input, Address& address) noexcept {
    auto internal = ZilliqaMessage::ProtoTransactionCoreInfo();
    const auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    if (!Address::decode(input.to_address(), address)) {
        // invalid input address
        return Data(0);
    }
    const auto pubKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);

    internal.set_version(input.version());
    internal.set_nonce(input.nonce());
    internal.set_toaddr(address.getKeyHash().data(), address.getKeyHash().size());

    auto sender = new ZilliqaMessage::ByteArray();
    sender->set_data(pubKey.bytes.data(), pubKey.bytes.size());
    internal.set_allocated_senderpubkey(sender);

    auto amountArray = new ZilliqaMessage::ByteArray();
    auto amount = Data(input.amount().begin(), input.amount().end());
    amount = prependZero(amount);
    amountArray->set_data(amount.data(), amount.size());
    internal.set_allocated_amount(amountArray);

    auto gasPriceArray = new ZilliqaMessage::ByteArray();
    auto gasPrice = Data(input.gas_price().begin(), input.gas_price().end());
    gasPrice = prependZero(gasPrice);
    gasPriceArray->set_data(gasPrice.data(), gasPrice.size());
    internal.set_allocated_gasprice(gasPriceArray);

    internal.set_gaslimit(input.gas_limit());

    const auto serialized = internal.SerializeAsString();
    return Data(serialized.begin(), serialized.end());
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto output = Proto::SigningOutput();
    Address address;
    const auto preImage = Signer::getPreImage(input, address);
    const auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    const auto pubKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto signature = key.signSchnorr(preImage, TWCurveSECP256k1);

    // build json
    nlohmann::json json = {
        {"version", input.version()},
        {"toAddr", address.checksumed()},
        {"amount", toString(load(input.amount()))},
        {"nonce", input.nonce()},
        {"gasPrice", toString(load(input.gas_price()))},
        {"gasLimit", toString(input.gas_limit())},
        {"code", input.code()},
        {"data", input.data()},
        {"pubKey", hex(pubKey.bytes)},
        {"signature", hex(signature)},
    };
    output.set_json(json.dump());
    output.set_signature(signature.data(), signature.size());

    return output;
}
