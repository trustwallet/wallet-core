// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"
#include "Base58.h"
#include "Base64.h"
#include "HexCoding.h"
#include "Identifiers.h"
#include <Data.h>
#include <Ethereum/RLP.h>

using namespace TW;

namespace TW::Aeternity {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    std::string sender_id = input.from_address();
    std::string recipient_id = input.to_address();
    std::string payload = input.payload();

    auto tx = Transaction(sender_id, recipient_id, load(input.amount()), load(input.fee()), payload, input.ttl(), input.nonce());

    return Signer::sign(privateKey, tx);
}

/// implementation copied from
/// https://github.com/aeternity/aepp-sdk-go/blob/07aa8a77e5/aeternity/helpers.go#L367
Proto::SigningOutput Signer::sign(const TW::PrivateKey& privateKey, Transaction& transaction) {
    auto txRlp = transaction.encode();

    /// append networkId and txRaw
    auto msg = buildMessageToSign(txRlp);

    /// sign ed25519
    auto sigRaw = privateKey.sign(msg, TWCurveED25519);
    auto signature = Identifiers::prefixSignature + Base58::encodeCheck(sigRaw);

    /// encode the message using rlp
    auto rlpTxRaw = buildRlpTxRaw(txRlp, sigRaw);

    /// encode the rlp message with the prefix
    auto signedEncodedTx = encodeBase64WithChecksum(Identifiers::prefixTransaction, rlpTxRaw);

    return createProtoOutput(signature, signedEncodedTx);
}

Data Signer::buildRlpTxRaw(const Data& txRaw, const Data& sigRaw) {
    EthereumRlp::Proto::EncodingInput input;
    auto* rlpList = input.mutable_item()->mutable_list();

    rlpList->add_items()->set_number_u64(Identifiers::objectTagSignedTransaction);
    rlpList->add_items()->set_number_u64(Identifiers::rlpMessageVersion);

    // Append a list of signatures.
    auto* signaturesList = rlpList->add_items()->mutable_list();
    signaturesList->add_items()->set_data(sigRaw.data(), sigRaw.size());

    rlpList->add_items()->set_data(txRaw.data(), txRaw.size());

    return Ethereum::RLP::encode(input);
}

Data Signer::buildMessageToSign(const Data& txRaw) {
    auto data = Data();
    Data bytes(Identifiers::networkId.begin(), Identifiers::networkId.end());
    append(data, bytes);
    append(data, txRaw);
    return data;
}

Proto::SigningOutput Signer::createProtoOutput(std::string& signature, const std::string& signedTx) {
    auto output = Proto::SigningOutput();

    output.set_signature(signature);
    output.set_encoded(signedTx);
    return output;
}

std::string Signer::encodeBase64WithChecksum(const std::string& prefix, const TW::Data& rawTx) {
    auto checksum = Hash::sha256(Hash::sha256(rawTx));
    std::vector<unsigned char> checksumPart(checksum.begin(), checksum.begin() + checkSumSize);

    auto data = Data();
    append(data, rawTx);
    append(data, checksumPart);

    return prefix + TW::Base64::encode(data);
}

} // namespace TW::Aeternity
