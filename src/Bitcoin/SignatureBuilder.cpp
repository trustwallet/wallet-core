// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SignatureBuilder.h"
#include "SigHashType.h"
#include "TransactionInput.h"
#include "TransactionOutput.h"

#include "../BinaryCoding.h"
#include "../HexCoding.h"

#include "../BitcoinDiamond/Transaction.h"
#include "../Groestlcoin/Transaction.h"
#include "../Verge/Transaction.h"
#include "../Zcash/Transaction.h"
#include "../Zcash/TransactionBuilder.h"

namespace TW::Bitcoin {

template <typename Transaction>
Result<Transaction, Common::Proto::SigningError> SignatureBuilder<Transaction>::sign() {
    if (plan.error != Common::Proto::OK) {
        // plan with error, fail
        return Result<Transaction, Common::Proto::SigningError>::failure(plan.error);
    }
    if (_transaction.inputs.size() == 0 || plan.utxos.size() == 0) {
        return Result<Transaction, Common::Proto::SigningError>::failure(Common::Proto::Error_missing_input_utxos);
    }

    transactionToSign = _transaction;
    transactionToSign.inputs.clear();
    std::copy(std::begin(_transaction.inputs), std::end(_transaction.inputs),
              std::back_inserter(transactionToSign.inputs));

    const auto hashSingle = hashTypeIsSingle(input.hashType);
    for (auto i = 0ul; i < plan.utxos.size(); i++) {
        // Only sign TWBitcoinSigHashTypeSingle if there's a corresponding output
        if (hashSingle && i >= _transaction.outputs.size()) {
            continue;
        }
        auto& utxo = plan.utxos[i];
        if (i < _transaction.inputs.size()) {
            auto result = sign(utxo.script, i, utxo);
            if (!result) {
                return Result<Transaction, Common::Proto::SigningError>::failure(result.error());
            }
        }
    }

    // save estimated size
    if ((input.byteFee > 0) && (plan.fee > 0)) {
        transactionToSign.previousEstimatedVirtualSize = static_cast<int>(plan.fee / input.byteFee);
    }

    return Result<Transaction, Common::Proto::SigningError>::success(std::move(transactionToSign));
}

template <typename Transaction>
Result<void, Common::Proto::SigningError> SignatureBuilder<Transaction>::sign(Script script, size_t index,
                                                                              const UTXO& utxo) {
    assert(index < _transaction.inputs.size());

    Script redeemScript;
    std::vector<Data> results;

    uint32_t signatureVersion = [this]() {
        if ((input.hashType & TWBitcoinSigHashTypeFork) != 0) {
            return WITNESS_V0;
        }
        return BASE;
    }();
    auto result = signStep(script, index, utxo, signatureVersion);
    if (!result) {
        return Result<void, Common::Proto::SigningError>::failure(result.error());
    }
    results = result.payload();
    assert(results.size() >= 1);
    auto txin = _transaction.inputs[index];

    if (script.isPayToScriptHash()) {
        script = Script(results[0]);
        auto signStepResult = signStep(script, index, utxo, signatureVersion);
        if (!signStepResult) {
            return Result<void, Common::Proto::SigningError>::failure(signStepResult.error());
        }
        results = signStepResult.payload();
        results.push_back(script.bytes);
        redeemScript = script;
    }

    std::vector<Data> witnessStack;
    Data data;
    if (script.matchPayToWitnessPublicKeyHash(data)) {
        auto witnessScript = Script::buildPayToPublicKeyHash(results[0]);
        auto _result = signStep(witnessScript, index, utxo, WITNESS_V0);
        if (!_result) {
            return Result<void, Common::Proto::SigningError>::failure(_result.error());
        }
        witnessStack = _result.payload();
        results.clear();
    } else if (script.matchPayToWitnessScriptHash(data)) {
        auto witnessScript = Script(results[0]);
        auto _result = signStep(witnessScript, index, utxo, WITNESS_V0);
        if (!_result) {
            return Result<void, Common::Proto::SigningError>::failure(_result.error());
        }
        witnessStack = _result.payload();
        witnessStack.push_back(std::move(witnessScript.bytes));
        results.clear();
    } else if (script.isWitnessProgram()) {
        // Error: Unrecognized witness program.
        return Result<void, Common::Proto::SigningError>::failure(Common::Proto::Error_script_witness_program);
    }

    if (!redeemScript.bytes.empty()) {
        results.push_back(redeemScript.bytes);
    }

    auto transactionInput = TransactionInput(txin.previousOutput, Script(pushAll(results)), txin.sequence);
    transactionInput.scriptWitness = witnessStack;
    transactionToSign.inputs[index] = transactionInput;
    return Result<void, Common::Proto::SigningError>::success();
}

template <typename Transaction>
Result<std::vector<Data>, Common::Proto::SigningError> SignatureBuilder<Transaction>::signStep(
    Script script, size_t index, const UTXO& utxo, uint32_t version) {

    Data data;
    std::vector<Data> keys;
    int required;

    if (script.matchPayToScriptHash(data) || script.matchPayToScriptHashReplay(data)) {
        auto redeemScript = scriptForScriptHash(data);
        if (redeemScript.empty()) {
            // Error: Missing redeem script
            return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_script_redeem);
        }
        return Result<std::vector<Data>, Common::Proto::SigningError>::success({redeemScript});
    }
    if (script.matchPayToWitnessScriptHash(data)) {
        auto scripthash = Hash::ripemd(data);
        auto redeemScript = scriptForScriptHash(scripthash);
        if (redeemScript.empty()) {
            // Error: Missing redeem script
            return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_script_redeem);
        }
        return Result<std::vector<Data>, Common::Proto::SigningError>::success({redeemScript});
    }
    if (script.matchPayToWitnessPublicKeyHash(data)) {
        return Result<std::vector<Data>, Common::Proto::SigningError>::success({data});
    }
    if (script.isWitnessProgram()) {
        // Error: Invalid output script
        return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_script_output);
    }
    if (script.matchMultisig(keys, required)) {
        auto results = std::vector<Data>{{}}; // workaround CHECKMULTISIG bug
        for (auto& pubKey : keys) {
            if (results.size() >= required + 1ul) {
                break;
            }
            auto keyHash = Hash::ripemd(Hash::sha256(pubKey));
            auto pair = keyPairForPubKeyHash(keyHash);
            if (!pair.has_value() && signingMode == SigningMode_Normal) {
                // Error: missing key
                return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_missing_private_key);
            }
            auto signature = createSignature(transactionToSign, script, keyHash, pair, index, utxo.amount, version);
            if (signature.empty()) {
                // Error: Failed to sign
                return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_signing);
            }
            results.push_back(signature);
        }
        results.resize(required + 1);
        return Result<std::vector<Data>, Common::Proto::SigningError>::success(std::move(results));
    }
    if (script.matchPayToPublicKey(data)) {
        auto keyHash = Hash::ripemd(Hash::sha256(data));
        auto pair = keyPairForPubKeyHash(keyHash);
        if (!pair.has_value() && signingMode == SigningMode_Normal) {
            // Error: Missing key
            return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_missing_private_key);
        }
        auto signature = createSignature(transactionToSign, script, keyHash, pair, index, utxo.amount, version);
        if (signature.empty()) {
            // Error: Failed to sign
            return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_signing);
        }
        return Result<std::vector<Data>, Common::Proto::SigningError>::success({signature});
    }
    if (script.matchPayToPublicKeyHash(data) || script.matchPayToPublicKeyHashReplay(data)) {
        // obtain public key
        auto pair = keyPairForPubKeyHash(data);
        Data pubkey;
        if (!pair.has_value()) {
            if (signingMode == SigningMode_SizeEstimationOnly || signingMode == SigningMode_HashOnly) {
                // estimation mode, key is missing: use placeholder for public key
                pubkey = Data(PublicKey::secp256k1Size);
            } else if (signingMode == SigningMode_External) {
                size_t _index = hashesForSigning.size();
                if (!externalSignatures.has_value() || externalSignatures.value().size() <= _index) {
                    // Error: no or not enough signatures provided
                    return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_signing);
                }
                pubkey = std::get<1>(externalSignatures.value()[_index]);
            } else {
                // Error: Missing keys
                return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_missing_private_key);
            }
        } else {
            pubkey = std::get<1>(pair.value()).bytes;
        }
        assert(!pubkey.empty());

        auto signature = createSignature(transactionToSign, script, data, pair, index, utxo.amount, version);
        if (signature.empty()) {
            // Error: Failed to sign
            return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_signing);
        }
        return Result<std::vector<Data>, Common::Proto::SigningError>::success({signature, pubkey});
    }
    // Error: Invalid output script
    return Result<std::vector<Data>, Common::Proto::SigningError>::failure(Common::Proto::Error_script_output);
}

template <typename Transaction>
Data SignatureBuilder<Transaction>::createSignature(
    const Transaction& transaction,
    const Script& script,
    const Data& publicKeyHash,
    const std::optional<KeyPair>& pair,
    size_t index,
    Amount amount,
    uint32_t version) {
    if (signingMode == SigningMode_SizeEstimationOnly) {
        // Don't sign, only estimate signature size. It is 71-72 bytes.  Return placeholder.
        return Data(72);
    }

    const Data sighash = transaction.getSignatureHash(script, index, input.hashType, amount,
                                                      static_cast<SignatureVersion>(version));

    if (signingMode == SigningMode_HashOnly) {
        // Don't sign, only store hash-to-be-signed + pubkeyhash.  Return placeholder.
        hashesForSigning.push_back(std::make_pair(sighash, publicKeyHash));
        return Data(72);
    }

    if (signingMode == SigningMode_External) {
        // Use externally-provided signature
        // Store hash, only for counting
        size_t _index = hashesForSigning.size();
        hashesForSigning.push_back(std::make_pair(sighash, publicKeyHash));

        if (!externalSignatures.has_value() || externalSignatures.value().size() <= _index) {
            // Error: no or not enough signatures provided
            return {};
        }

        Data externalSignature = std::get<0>(externalSignatures.value()[_index]);
        const Data publicKey = std::get<1>(externalSignatures.value()[_index]);

        // Verify provided signature
        if (!PublicKey::isValid(publicKey, TWPublicKeyTypeSECP256k1)) {
            // Error: invalid public key
            return {};
        }
        const auto publicKeyObj = PublicKey(publicKey, TWPublicKeyTypeSECP256k1);
        if (!publicKeyObj.verifyAsDER(externalSignature, sighash)) {
            // Error: Signature does not match publickey+hash
            return {};
        }
        externalSignature.push_back(static_cast<byte>(input.hashType));

        return externalSignature;
    }

    const auto key = std::get<0>(pair.value());
    const auto pk = PrivateKey(key);

    auto sig = pk.signAsDER(sighash);
    if (!sig.empty()) {
        sig.push_back(static_cast<byte>(input.hashType));
    }
    return sig;
}

template <typename Transaction>
Data SignatureBuilder<Transaction>::pushAll(const std::vector<Data>& results) {
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

template <typename Transaction>
std::optional<KeyPair> SignatureBuilder<Transaction>::keyPairForPubKeyHash(const Data& hash) const {
    for (auto& key : input.privateKeys) {
        auto pubKeyExtended = key.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        auto pubKey = pubKeyExtended.compressed();
        if (Hash::sha256ripemd(pubKey.bytes.data(), pubKey.bytes.size()) == hash) {
            return std::make_tuple(key, pubKey);
        }
        if (Hash::sha256ripemd(pubKeyExtended.bytes.data(), pubKeyExtended.bytes.size()) == hash) {
            return std::make_tuple(key, pubKeyExtended);
        }
    }
    return {};
}

template <typename Transaction>
Data SignatureBuilder<Transaction>::scriptForScriptHash(const Data& hash) const {
    auto hashString = hex(hash);
    auto it = input.scripts.find(hashString);
    if (it == input.scripts.end()) {
        // Error: Missing redeem script
        return {};
    }
    return it->second.bytes;
}

// Explicitly instantiate a Signers for compatible transactions.
template class SignatureBuilder<Bitcoin::Transaction>;
template class SignatureBuilder<BitcoinDiamond::Transaction>;
template class SignatureBuilder<Verge::Transaction>;
template class SignatureBuilder<Zcash::Transaction>;
template class SignatureBuilder<Groestlcoin::Transaction>;

} // namespace TW::Bitcoin
