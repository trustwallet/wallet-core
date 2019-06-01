// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Base64.h"
#include "ChecksumEncoder.h"
#include "HexCoding.h"
#include "Identifiers.h"
#include <Data.h>
#include <Ethereum/RLP.h>

using namespace TW;
using namespace Aeternity;

/// implementation copied from
/// https://github.com/aeternity/aepp-sdk-go/blob/07aa8a77e5/aeternity/helpers.go#L367
Proto::SigningOutput Aeternity::Signer::sign(const TW::PrivateKey &privateKey, Transaction &transaction) {
    const std::string &txString = transaction.encode();
    auto txRaw = parseRawTransaction(txString);

    /// append networkId and txRaw
    auto msg = buildMessageToSign(txRaw);

    /// sign ed25519
    auto sigRaw = privateKey.sign(msg, TWCurveED25519);
    auto signature = ChecksumEncoder::encode(Identifiers::prefixSignature, sigRaw);

    /// encode the message using rlp
    auto rlpTxRaw = buildRlpTxRaw(txRaw, sigRaw);

    /// encode the rlp message with the prefix
    auto signedEncodedTx = ChecksumEncoder::encode(Identifiers::prefixTransaction, rlpTxRaw);

    return createProtoOutput(signature, signedEncodedTx);
}

Data TW::Aeternity::Signer::parseRawTransaction(const std::string &transaction) {
    auto trimPrefix = transaction.substr(Identifiers::prefixTransaction.size(), transaction.size() - 1);
    auto txWithChecksum = TW::Base64::decode(trimPrefix);

    /// trimChecksum
    auto start = txWithChecksum.begin();
    auto end = txWithChecksum.end() - ChecksumEncoder::checkSumSize;
    Data txRaw(start, end);

    return txRaw;
}

Data Signer::buildRlpTxRaw(Data &txRaw, Data &sigRaw) {
    auto rlpTxRaw = Data();
    auto signaturesList = Data();
    append(signaturesList, Ethereum::RLP::encode(sigRaw));

    append(rlpTxRaw, Ethereum::RLP::encode(Identifiers::objectTagSignedTransaction));
    append(rlpTxRaw, Ethereum::RLP::encode(Identifiers::rlpMessageVersion));
    append(rlpTxRaw, Ethereum::RLP::encodeList(signaturesList));
    append(rlpTxRaw, Ethereum::RLP::encode(txRaw));

    return Ethereum::RLP::encodeList(rlpTxRaw);
}

Data Signer::buildMessageToSign(Data &txRaw) {
    auto data = Data();
    Data bytes(Identifiers::networkId.begin(), Identifiers::networkId.end());
    append(data, bytes);
    append(data, txRaw);
    return data;
}

Proto::SigningOutput Signer::createProtoOutput(std::string signature, const std::string& signedTx) {
    auto output = Proto::SigningOutput();

    Data sigData(signature.begin(), signature.end());
    output.set_signature(sigData.data(), sigData.size());

    const Data txData(signedTx.begin(), signedTx.end());
    output.set_encoded(txData.data(), txData.size());

    return output;
}
