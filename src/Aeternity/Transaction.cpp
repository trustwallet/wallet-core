// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Transaction.h"
#include "Identifiers.h"
#include <Base58.h>
#include <Ethereum/RLP.h>
#include <Hash.h>

namespace TW::Aeternity {

/// Aeternity network does not accept zero int values as rlp param,
/// instead empty byte array should be encoded
/// see https://forum.aeternity.com/t/invalid-tx-error-on-mainnet-goggle-says-it-looks-good/4118/5?u=defuera
EthereumRlp::Proto::RlpItem prepareSafeZero(const uint256_t& value) {
    EthereumRlp::Proto::RlpItem item;

    if (value == 0) {
        Data zeroValue{0};
        item.set_data(zeroValue.data(), zeroValue.size());
    } else {
        auto valueData = store(value);
        item.set_number_u256(valueData.data(), valueData.size());
    }

    return item;
}

/// RLP returns a byte serialized representation
Data Transaction::encode() {
    EthereumRlp::Proto::EncodingInput input;
    auto* rlpList = input.mutable_item()->mutable_list();

    auto senderIdTag = buildTag(sender_id);
    auto recipientIdTag = buildTag(recipient_id);

    rlpList->add_items()->set_number_u64(Identifiers::objectTagSpendTransaction);
    rlpList->add_items()->set_number_u64(Identifiers::rlpMessageVersion);
    rlpList->add_items()->set_data(senderIdTag.data(), senderIdTag.size());
    rlpList->add_items()->set_data(recipientIdTag.data(), recipientIdTag.size());

    *rlpList->add_items() = prepareSafeZero(amount);
    *rlpList->add_items() = prepareSafeZero(fee);
    *rlpList->add_items() = prepareSafeZero(ttl);
    *rlpList->add_items() = prepareSafeZero(nonce);

    rlpList->add_items()->set_data(payload.data(), payload.size());

    return Ethereum::RLP::encode(input);
}

TW::Data Transaction::buildTag(const std::string& address) {
    auto payload = address.substr(Identifiers::prefixTransaction.size(), address.size());

    auto data = Data();
    append(data, Identifiers::iDTagAccount);
    append(data, Base58::decodeCheck(payload));

    return data;
}

} // namespace TW::Aeternity
