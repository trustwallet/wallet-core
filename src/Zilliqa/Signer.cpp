// Copyright © 2017-2019 Trust Wallet.
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

Data Signer::getPreImage(const Proto::SigningInput& input) noexcept {
    auto internal = ZilliqaMessage::ProtoTransactionCoreInfo();

    const auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    const auto address = Address(input.to_address());

    const auto pubKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);

    internal.set_version(input.version());
    internal.set_nonce(input.nonce());
    internal.set_toaddr(address.bytes.data(), address.bytes.size());

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

Proto::SigningOutput Signer::sign(const Data& preImage, const PrivateKey& key) noexcept {
    auto output = Proto::SigningOutput();

    const auto signature = key.signSchnorr(preImage, TWCurveSECP256k1);

    output.set_signature(signature.data(), signature.size());

    return output;
}
