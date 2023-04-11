// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "TransactionInput.h"
#include "TransactionOutput.h"
#include "../Bitcoin/SigHashType.h"
#include "../Bitcoin/SignatureBuilder.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"

namespace TW::Decred {

Bitcoin::Proto::TransactionPlan Signer::plan(const Bitcoin::Proto::SigningInput& input) noexcept {
    auto signer = Signer(input);
    return signer.txPlan.proto();
}

Proto::SigningOutput Signer::sign(const Bitcoin::Proto::SigningInput& input) noexcept {
    auto signer = Signer(input);
    auto result = signer.sign();
    auto output = Proto::SigningOutput();
    if (!result) {
        output.set_error(result.error());
        return output;
    }

    const auto& tx = result.payload();
    *output.mutable_transaction() = tx.proto();

    Data encoded;
    tx.encode(encoded);
    output.set_encoded(encoded.data(), encoded.size());

    auto txHash = Hash::blake256(encoded);
    output.set_transaction_id(hex(txHash));
    return output;
}

Result<Transaction, Common::Proto::SigningError> Signer::sign() {
    if (txPlan.utxos.empty() || _transaction.inputs.empty()) {
        return Result<Transaction, Common::Proto::SigningError>::failure(Common::Proto::Error_missing_input_utxos);
    }

    signedInputs = _transaction.inputs;

    const auto hashSingle = Bitcoin::hashTypeIsSingle(static_cast<enum TWBitcoinSigHashType>(input.hash_type()));
    for (auto i = 0ul; i < txPlan.utxos.size(); i += 1) {
        auto& utxo = txPlan.utxos[i];

        // Only sign TWBitcoinSigHashTypeSingle if there's a corresponding output
        if (hashSingle && i >= _transaction.outputs.size()) {
            continue;
        }
        auto result = sign(utxo.script, i);
        if (!result) {
            return Result<Transaction, Common::Proto::SigningError>::failure(result.error());
        }
        signedInputs[i].script = result.payload();
    }

    Transaction tx(_transaction);
    tx.inputs = std::move(signedInputs);
    tx.outputs = _transaction.outputs;
    return Result<Transaction, Common::Proto::SigningError>::success(std::move(tx));
}

Result<Bitcoin::Script, Common::Proto::SigningError> Signer::sign(Bitcoin::Script script, size_t index) {
    assert(index < _transaction.inputs.size());

    Bitcoin::Script redeemScript;
    std::vector<Data> results;

    auto result = signStep(script, index);
    if (result) {
        results = result.payload();
    } else {
        return Result<Bitcoin::Script, Common::Proto::SigningError>::failure(result.error());
    }
    auto txin = _transaction.inputs[index];

    if (script.isPayToScriptHash()) {
        script = Bitcoin::Script(results.front().begin(), results.front().end());
        auto result_ = signStep(script, index);
        if (!result_) {
            return Result<Bitcoin::Script, Common::Proto::SigningError>::failure(result_.error());
        }
        results = result_.payload();
        results.push_back(script.bytes);
        redeemScript = script;
        results.push_back(redeemScript.bytes);
    }

    return Result<Bitcoin::Script, Common::Proto::SigningError>::success(Bitcoin::Script(Bitcoin::SignatureBuilder<Bitcoin::Transaction>::pushAll(results)));
}

Result<std::vector<Data>, Common::Proto::SigningError> Signer::signStep(Bitcoin::Script script, size_t index) {
    Transaction transactionToSign(_transaction);
    transactionToSign.inputs = signedInputs;
    transactionToSign.outputs = _transaction.outputs;

    Data data;
    std::vector<Data> keys;
    int required;

    if (script.matchPayToPublicKey(data)) {
        auto keyHash = TW::Hash::ripemd(TW::Hash::blake256(data));
        auto key = keyForPublicKeyHash(keyHash);
        if (key.empty()) {
            // Error: Missing key
            return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_missing_private_key);
        }
        auto signature = createSignature(transactionToSign, script, key, index);
        if (signature.empty()) {
            // Error: Failed to sign
            return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_signing);
        }
        return Result<std::vector<Data>, Common::Proto::SigningError>::success({signature});
    } else if (script.matchPayToPublicKeyHash(data)) {
        auto key = keyForPublicKeyHash(data);
        if (key.empty()) {
            // Error: Missing keyxs
            return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_missing_private_key);
        }

        auto pubkey = PrivateKey(key).getPublicKey(TWPublicKeyTypeSECP256k1);
        auto signature = createSignature(transactionToSign, script, key, index);
        if (signature.empty()) {
            // Error: Failed to sign
            return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_signing);
        }
        return Result<std::vector<Data>, Common::Proto::SigningError>::success({signature, pubkey.bytes});
    } else if (script.matchPayToScriptHash(data)) {
        auto redeemScript = scriptForScriptHash(data);
        if (redeemScript.empty()) {
            // Error: Missing redeem script
            return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_script_redeem);
        }
        return Result<std::vector<Data>, Common::Proto::SigningError>::success({redeemScript});
    } else if (script.matchMultisig(keys, required)) {
        auto results = std::vector<Data>{{}};
        for (auto& pubKey : keys) {
            if (results.size() >= required + 1ul) {
                break;
            }
            auto keyHash = TW::Hash::ripemd(TW::Hash::blake256(pubKey));
            auto key = keyForPublicKeyHash(keyHash);
            if (key.empty()) {
                // Error: missing key
                return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_missing_private_key);
            }
            auto signature = createSignature(transactionToSign, script, key, index);
            if (signature.empty()) {
                // Error: Failed to sign
                return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_signing);
            }
            results.push_back(signature);
        }
        results.resize(required + 1);
        return Result<std::vector<Data>, Common::Proto::SigningError>::success(std::move(results));
    } else {
        // Error: Invalid output script
        return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_script_output);
    }
}

Data Signer::createSignature(const Transaction& transaction, const Bitcoin::Script& script,
                             const Data& key, size_t index) {
    auto sighash = transaction.computeSignatureHash(script, index, static_cast<TWBitcoinSigHashType>(input.hash_type()));
    auto pk = PrivateKey(key);
    auto signature = pk.signAsDER(Data(begin(sighash), end(sighash)));
    if (script.empty()) {
        return {};
    }
    signature.push_back(static_cast<uint8_t>(input.hash_type()));

    return signature;
}

Data Signer::keyForPublicKeyHash(const Data& hash) const {
    for (auto& key : input.private_key()) {
        auto publicKey = PrivateKey(key).getPublicKey(TWPublicKeyTypeSECP256k1);
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

} // namespace TW::Decred
