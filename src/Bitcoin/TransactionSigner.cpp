// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionSigner.h"

#include "TransactionInput.h"
#include "TransactionOutput.h"
#include "UnspentSelector.h"
#include "SigHashType.h"

#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../Zcash/Transaction.h"
#include "../Groestlcoin/Transaction.h"

using namespace TW;
using namespace TW::Bitcoin;

template <typename Transaction, typename TransactionBuilder>
Result<Transaction> TransactionSigner<Transaction, TransactionBuilder>::sign() {
    if (transaction.inputs.size() == 0 || plan.utxos.size() == 0) {
        return Result<Transaction>::failure("Missing inputs or UTXOs");
    }

    signedInputs.clear();
    std::copy(std::begin(transaction.inputs), std::end(transaction.inputs),
              std::back_inserter(signedInputs));

    const auto hashSingle = hashTypeIsSingle(static_cast<enum TWBitcoinSigHashType>(input.hash_type()));
    for (auto i = 0; i < plan.utxos.size(); i++) {
        // Only sign TWBitcoinSigHashTypeSingle if there's a corresponding output
        if (hashSingle && i >= transaction.outputs.size()) {
            continue;
        }
        auto& utxo = plan.utxos[i];
        auto script = Script(utxo.script().begin(), utxo.script().end());
        if (i < transaction.inputs.size()) {
            auto result = sign(script, i, utxo);
            if (!result) {
                return Result<Transaction>::failure(result.error());
            }
        }
    }

    Transaction tx(transaction);
    tx.inputs = move(signedInputs);
    tx.outputs = transaction.outputs;
    // save estimated size
    if ((input.byte_fee()) > 0 && (plan.fee > 0)) {
        tx.previousEstimatedVirtualSize = static_cast<int>(plan.fee / input.byte_fee());
    }

    return Result<Transaction>::success(std::move(tx));
}

template <typename Transaction, typename TransactionBuilder>
Result<void> TransactionSigner<Transaction, TransactionBuilder>::sign(Script script, size_t index,
                                                  const Bitcoin::Proto::UnspentTransaction& utxo) {
    assert(index < transaction.inputs.size());

    Script redeemScript;
    std::vector<Data> results;

    uint32_t signatureVersion = [this]() {
        if ((input.hash_type() & TWBitcoinSigHashTypeFork) != 0) {
            return WITNESS_V0;
        } else {
            return BASE;
        }
    }();
    auto result = signStep(script, index, utxo, signatureVersion);
    if (!result) {
        return Result<void>::failure(result.error());
    }
    results = result.payload();
    assert(results.size() >= 1);
    auto txin = transaction.inputs[index];

    if (script.isPayToScriptHash()) {
        script = Script(results[0]);
        auto result = signStep(script, index, utxo, signatureVersion);
        if (!result) {
            return Result<void>::failure(result.error());
        }
        results = result.payload();
        results.push_back(script.bytes);
        redeemScript = script;
    }

    std::vector<Data> witnessStack;
    Data data;
    if (script.matchPayToWitnessPublicKeyHash(data)) {
        auto witnessScript = Script::buildPayToPublicKeyHash(results[0]);
        auto result = signStep(witnessScript, index, utxo, WITNESS_V0);
        if (!result) {
            return Result<void>::failure(result.error());
        }
        witnessStack = result.payload();
        results.clear();
    } else if (script.matchPayToWitnessScriptHash(data)) {
        auto witnessScript = Script(results[0]);
        auto result = signStep(witnessScript, index, utxo, WITNESS_V0);
        if (!result) {
            return Result<void>::failure(result.error());
        }
        witnessStack = result.payload();
        witnessStack.push_back(move(witnessScript.bytes));
        results.clear();
    } else if (script.isWitnessProgram()) {
        // Error: Unrecognized witness program.
        return Result<void>::failure("Unrecognized witness program");
    }

    if (!redeemScript.bytes.empty()) {
        results.push_back(redeemScript.bytes);
    }

    signedInputs[index] =
        TransactionInput(txin.previousOutput, Script(pushAll(results)), txin.sequence);
    signedInputs[index].scriptWitness = witnessStack;
    return Result<void>::success();
}

template <typename Transaction, typename TransactionBuilder>
Result<std::vector<Data>> TransactionSigner<Transaction, TransactionBuilder>::signStep(
    Script script, size_t index, const Bitcoin::Proto::UnspentTransaction& utxo, uint32_t version) const {
    Transaction transactionToSign(transaction);
    transactionToSign.inputs = signedInputs;
    transactionToSign.outputs = transaction.outputs;

    Data data;
    std::vector<Data> keys;
    int required;

    if (script.matchPayToScriptHash(data)) {
        auto redeemScript = scriptForScriptHash(data);
        if (redeemScript.empty()) {
            // Error: Missing redeem script
            return Result<std::vector<Data>>::failure("Missing redeem script.");
        }
        return Result<std::vector<Data>>::success({redeemScript});
    }
    if (script.matchPayToWitnessScriptHash(data)) {
        auto scripthash = TW::Hash::ripemd(data);
        auto redeemScript = scriptForScriptHash(scripthash);
        if (redeemScript.empty()) {
            // Error: Missing redeem script
            return Result<std::vector<Data>>::failure("Missing redeem script.");
        }
        return Result<std::vector<Data>>::success({redeemScript});
    }
    if (script.matchPayToWitnessPublicKeyHash(data)) {
        return Result<std::vector<Data>>::success({data});
    }
    if (script.isWitnessProgram()) {
        // Error: Invalid sutput script
        return Result<std::vector<Data>>::failure("Invalid output script.");
    }
    if (script.matchMultisig(keys, required)) {
        auto results = std::vector<Data>{{}}; // workaround CHECKMULTISIG bug
        for (auto& pubKey : keys) {
            if (results.size() >= required + 1) {
                break;
            }
            auto keyHash = TW::Hash::ripemd(TW::Hash::sha256(pubKey));
            auto key = keyForPublicKeyHash(keyHash);
            if (key.empty() && !estimationMode) {
                // Error: missing key
                return Result<std::vector<Data>>::failure("Missing private key.");
            }
            auto signature =
                createSignature(transactionToSign, script, key, index, utxo.amount(), version);
            if (signature.empty()) {
                // Error: Failed to sign
                return Result<std::vector<Data>>::failure("Failed to sign.");
            }
            results.push_back(signature);
        }
        results.resize(required + 1);
        return Result<std::vector<Data>>::success(std::move(results));
    }
    if (script.matchPayToPublicKey(data)) {
        auto keyHash = TW::Hash::ripemd(TW::Hash::sha256(data));
        auto key = keyForPublicKeyHash(keyHash);
        if (key.empty() && !estimationMode) {
            // Error: Missing key
            return Result<std::vector<Data>>::failure("Missing private key.");
        }
        auto signature =
            createSignature(transactionToSign, script, key, index, utxo.amount(), version);
        if (signature.empty()) {
            // Error: Failed to sign
            return Result<std::vector<Data>>::failure("Failed to sign.");
        }
        return Result<std::vector<Data>>::success({signature});
    }
    if (script.matchPayToPublicKeyHash(data)) {
        auto key = keyForPublicKeyHash(data);
        if (key.empty() && !estimationMode) {
            // Error: Missing keys
            return Result<std::vector<Data>>::failure("Missing private key.");
        }

        auto signature =
            createSignature(transactionToSign, script, key, index, utxo.amount(), version);
        if (signature.empty()) {
            // Error: Failed to sign
            return Result<std::vector<Data>>::failure("Failed to sign.");
        }
        if (key.empty() && estimationMode) {
            // estimation mode, key is missing: use placeholder for public key
            return Result<std::vector<Data>>::success({signature, Data(PublicKey::secp256k1Size)});
        }
        auto pubkey = PrivateKey(key).getPublicKey(TWPublicKeyTypeSECP256k1);
        return Result<std::vector<Data>>::success({signature, pubkey.bytes});
    }
    // Error: Invalid output script
    return Result<std::vector<Data>>::failure("Invalid output script.");
}

template <typename Transaction, typename TransactionBuilder>
Data TransactionSigner<Transaction, TransactionBuilder>::createSignature(const Transaction& transaction,
                                                     const Script& script, const Data& key,
                                                     size_t index, Amount amount,
                                                     uint32_t version) const {
    if (estimationMode) {
        // Don't sign, only estimate signature size. It is 71-72 bytes.  Return placeholder.
        return Data(72);
    }
    Data sighash = transaction.getSignatureHash(script, index, static_cast<TWBitcoinSigHashType>(input.hash_type()), amount,
                                                static_cast<SignatureVersion>(version));
    auto pk = PrivateKey(key);
    auto sig = pk.signAsDER(sighash, TWCurveSECP256k1);
    if (!sig.empty()) {
        sig.push_back(static_cast<uint8_t>(input.hash_type()));
    }
    return sig;
}

template <typename Transaction, typename TransactionBuilder>
Data TransactionSigner<Transaction, TransactionBuilder>::pushAll(const std::vector<Data>& results) {
    Data data;
    for (auto& result : results) {
        if (result.empty()) {
            data.push_back(OP_0);
        } else if (result.size() == 1 && result[0] >= 1 && result[0] <= 16) {
            data.push_back(Script::encodeNumber(result[0]));
        } else if (result.size() < OP_PUSHDATA1) {
            data.push_back(static_cast<uint8_t>(result.size()));
        } else if (result.size() <= 0xff) {
            data.push_back(OP_PUSHDATA1);
            data.push_back(static_cast<uint8_t>(result.size()));
        } else if (result.size() <= 0xffff) {
            data.push_back(OP_PUSHDATA2);
            encode16LE(static_cast<uint16_t>(result.size()), data);
        } else {
            data.push_back(OP_PUSHDATA4);
            encode32LE(static_cast<uint32_t>(result.size()), data);
        }
        std::copy(begin(result), end(result), back_inserter(data));
    }
    return data;
}

template <typename Transaction, typename TransactionBuilder>
Data TransactionSigner<Transaction, TransactionBuilder>::keyForPublicKeyHash(const Data& hash) const {
    for (auto& key : input.private_key()) {
        auto publicKey = PrivateKey(key).getPublicKey(TWPublicKeyTypeSECP256k1);
        auto keyHash = TW::Hash::ripemd(TW::Hash::sha256(publicKey.bytes));
        if (keyHash == hash) {
            return Data(key.begin(), key.end());
        }
    }
    return {};
}

template <typename Transaction, typename TransactionBuilder>
Data TransactionSigner<Transaction, TransactionBuilder>::scriptForScriptHash(const Data& hash) const {
    auto hashString = hex(hash);
    auto it = input.scripts().find(hashString);
    if (it == input.scripts().end()) {
        // Error: Missing redeem script
        return {};
    }
    return Data(it->second.begin(), it->second.end());
}

// Explicitly instantiate a Signers for compatible transactions.
template class TW::Bitcoin::TransactionSigner<Bitcoin::Transaction, Bitcoin::TransactionBuilder>;
template class TW::Bitcoin::TransactionSigner<Zcash::Transaction, Zcash::TransactionBuilder>;
template class TW::Bitcoin::TransactionSigner<Groestlcoin::Transaction, Bitcoin::TransactionBuilder>;
