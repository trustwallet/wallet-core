// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "TransactionInput.h"
#include "TransactionOutput.h"

#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrustWalletCore/TWBitcoinOpCodes.h>

using namespace TW;
using namespace TW::Decred;

Result<Transaction> Signer::sign() {
    signedInputs.clear();
    std::copy(std::begin(transaction.inputs), std::end(transaction.inputs),
              std::back_inserter(signedInputs));

    const bool hashSingle =
        ((input.hash_type() & ~TWSignatureHashTypeAnyoneCanPay) == TWSignatureHashTypeSingle);
    for (auto i = 0; i < plan.utxos.size(); i += 1) {
        auto& utxo = plan.utxos[i];

        // Only sign TWSignatureHashTypeSingle if there's a corresponding output
        if (hashSingle && i >= transaction.outputs.size()) {
            continue;
        }
        auto script = Bitcoin::Script(utxo.script().begin(), utxo.script().end());
        auto result = sign(script, i);
        if (!result) {
            return Result<Transaction>::failure(result.error());
        }
        signedInputs[i].script = result.payload();
    }

    Transaction tx(transaction);
    tx.inputs = move(signedInputs);
    tx.outputs = transaction.outputs;
    return Result<Transaction>::success(std::move(tx));
}

Result<Bitcoin::Script> Signer::sign(Bitcoin::Script script, size_t index) {
    Bitcoin::Script redeemScript;
    std::vector<Data> results;

    auto result = signStep(script, index);
    if (result) {
        results = result.payload();
    } else {
        return Result<Bitcoin::Script>::failure(result.error());
    }
    auto txin = transaction.inputs[index];

    if (script.isPayToScriptHash()) {
        script = Bitcoin::Script(results.front().begin(), results.front().end());
        auto result = signStep(script, index);
        if (!result) {
            return Result<Bitcoin::Script>::failure(result.error());
        }
        results = result.payload();
        results.push_back(script.bytes);
        redeemScript = script;
        results.push_back(redeemScript.bytes);
    }

    return Result<Bitcoin::Script>::success(Bitcoin::Script(pushAll(results)));
}

Result<std::vector<Data>> Signer::signStep(Bitcoin::Script script, size_t index) {
    Transaction transactionToSign(transaction);
    transactionToSign.inputs = signedInputs;
    transactionToSign.outputs = transaction.outputs;

    Data data;
    std::vector<Data> keys;
    int required;

    if (script.matchPayToPubkey(data)) {
        auto keyHash = TW::Hash::ripemd(TW::Hash::blake256(data));
        auto key = keyForPublicKeyHash(keyHash);
        if (key.empty()) {
            // Error: Missing key
            return Result<std::vector<Data>>::failure("Missing private key.");
        }
        auto signature = createSignature(transactionToSign, script, key, index);
        if (signature.empty()) {
            // Error: Failed to sign
            return Result<std::vector<Data>>::failure("Failed to sign.");
        }
        return Result<std::vector<Data>>::success({signature});
    } else if (script.matchPayToPubkeyHash(data)) {
        auto key = keyForPublicKeyHash(data);
        if (key.empty()) {
            // Error: Missing keyxs
            return Result<std::vector<Data>>::failure("Missing private key.");
        }

        auto pubkey = PrivateKey(key).getPublicKey(PublicKeyType::secp256k1);
        auto signature = createSignature(transactionToSign, script, key, index);
        if (signature.empty()) {
            // Error: Failed to sign
            return Result<std::vector<Data>>::failure("Failed to sign.");
        }
        return Result<std::vector<Data>>::success({signature, pubkey.bytes});
    } else if (script.matchPayToScriptHash(data)) {
        auto redeemScript = scriptForScriptHash(data);
        if (redeemScript.empty()) {
            // Error: Missing redeem script
            return Result<std::vector<Data>>::failure("Missing redeem script.");
        }
        return Result<std::vector<Data>>::success({redeemScript});
    } else if (script.matchMultisig(keys, required)) {
        auto results = std::vector<Data>{{}};
        for (auto& pubKey : keys) {
            if (results.size() >= required + 1) {
                break;
            }
            auto keyHash = TW::Hash::ripemd(TW::Hash::blake256(pubKey));
            auto key = keyForPublicKeyHash(keyHash);
            if (key.empty()) {
                // Error: missing key
                return Result<std::vector<Data>>::failure("Missing private key.");
            }
            auto signature = createSignature(transactionToSign, script, key, index);
            if (signature.empty()) {
                // Error: Failed to sign
                return Result<std::vector<Data>>::failure("Failed to sign.");
            }
            results.push_back(signature);
        }
        results.resize(required + 1);
        return Result<std::vector<Data>>::success(std::move(results));
    } else {
        // Error: Invalid output script
        return Result<std::vector<Data>>::failure("Invalid output script.");
    }
}

Data Signer::createSignature(const Transaction& transaction, const Bitcoin::Script& script,
                             const Data& key, size_t index) {
    auto sighash = transaction.computeSignatureHash(script, index, input.hash_type());
    auto pk = PrivateKey(key);
    auto signature = pk.signAsDER(Data(begin(sighash), end(sighash)), TWCurveSECP256k1);
    if (script.empty()) {
        return {};
    }
    signature.push_back(static_cast<uint8_t>(input.hash_type()));

    return signature;
}

Data Signer::pushAll(const std::vector<Data>& results) {
    auto data = Data{};
    for (auto& result : results) {
        if (result.empty()) {
            data.push_back(OP_0);
        } else if (result.size() == 1 && result[0] >= 1 && result[0] <= 16) {
            data.push_back(Bitcoin::Script::encodeNumber(result[0]));
        } else if (result.size() < OP_PUSHDATA1) {
            data.push_back(static_cast<uint8_t>(result.size()));
        } else if (result.size() <= 0xff) {
            data.push_back(OP_PUSHDATA1);
            data.push_back(static_cast<uint8_t>(result.size()));
        } else if (result.size() <= 0xffff) {
            data.push_back(OP_PUSHDATA2);
            encode16LE(result.size(), data);
        } else {
            data.push_back(OP_PUSHDATA4);
            encode32LE(static_cast<uint32_t>(result.size()), data);
        }
        std::copy(begin(result), end(result), back_inserter(data));
    }
    return data;
}

Data Signer::keyForPublicKeyHash(const Data& hash) const {
    for (auto& key : input.private_key()) {
        auto publicKey = PrivateKey(key).getPublicKey(PublicKeyType::secp256k1);
        auto keyHash = TW::Hash::ripemd(TW::Hash::blake256(publicKey.bytes));
        if (std::equal(std::begin(keyHash), std::end(keyHash), std::begin(hash), std::end(hash))) {
            return Data(key.begin(), key.end());
        }
    }
    return {};
}

Data Signer::scriptForScriptHash(const Data& hash) const {
    auto hashString = hex(hash.begin(), hash.end());
    auto it = input.scripts().find(hashString);
    if (it == input.scripts().end()) {
        // Error: Missing redeem script
        return {};
    }
    return Data(it->second.begin(), it->second.end());
}
