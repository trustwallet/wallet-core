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

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto internal = ZilliqaMessage::ProtoTransactionCoreInfo();
    auto output = Proto::SigningOutput();

    const auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    const auto address = Address(input.to_address());

    const auto pubKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);

    internal.set_version(input.version());
    internal.set_nonce(input.nonce());
    internal.set_toaddr(address.bytes.data(), address.bytes.size());

    auto sender = new ZilliqaMessage::ByteArray();
    sender->set_data(pubKey.bytes.data(), pubKey.bytes.size());
    internal.set_allocated_senderpubkey(sender);

    auto amount = new ZilliqaMessage::ByteArray();
    amount->set_data(input.amount());
    internal.set_allocated_amount(amount);

    auto gas_price = new ZilliqaMessage::ByteArray();
    gas_price->set_data(input.gas_price());
    internal.set_allocated_gasprice(gas_price);

    internal.set_gaslimit(input.gas_limit());

    const auto serialized = internal.SerializeAsString();

    const auto signature = key.signSchnorr(Data(serialized.begin(), serialized.end()), TWCurveSECP256k1);

    output.set_signature(signature.data(), signature.size());

    return output;
}
