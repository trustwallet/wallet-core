// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TxComparisonHelper.h"

#include <TrustWalletCore/TWCoinType.h>

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "proto/Bitcoin.pb.h"
#include "Data.h"
#include "PrivateKey.h"
#include "HexCoding.h"
#include "BinaryCoding.h"

#include <iostream>
#include <cassert>

namespace TW::Bitcoin {

auto emptyTxOutPoint = OutPoint(parse_hex("1d0f172a0ecb48aee1be1f2687d2963ae33f71a1"), 0);

UTXO buildTestUTXO(int64_t amount) {
    UTXO utxo;
    utxo.amount = amount;
    utxo.outPoint = emptyTxOutPoint;
    utxo.outPoint.sequence = UINT32_MAX;
    utxo.script = Script(parse_hex("0014"
                                   "1d0f172a0ecb48aee1be1f2687d2963ae33f71a1"));
    return utxo;
}

UTXOs buildTestUTXOs(const std::vector<int64_t>& amounts) {
    UTXOs utxos;
    for (long long amount : amounts) {
        utxos.push_back(buildTestUTXO(amount));
    }
    return utxos;
}

SigningInput buildSigningInput(Amount amount, int byteFee, const UTXOs& utxos, bool useMaxAmount, enum TWCoinType coin, bool omitPrivateKey) {
    SigningInput input;
    input.amount = amount;
    input.byteFee = byteFee;
    input.useMaxAmount = useMaxAmount;
    input.coinType = coin;
    input.dustCalculator = std::make_shared<LegacyDustCalculator>(coin);

    if (!omitPrivateKey) {
        auto utxoKey = PrivateKey(parse_hex("619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9"));
        auto pubKey = utxoKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        auto utxoPubkeyHash = Hash::ripemd(Hash::sha256(pubKey.bytes));
        assert(hex(utxoPubkeyHash) == "1d0f172a0ecb48aee1be1f2687d2963ae33f71a1");
        input.privateKeys.push_back(utxoKey);
    }

    input.utxos = utxos;
    input.toAddress = "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx";
    input.changeAddress = "1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU";
    return input;
}

int64_t sumUTXOs(const UTXOs& utxos) {
    int64_t s = 0u;
    for (auto& utxo : utxos) {
        s += utxo.amount;
    }
    return s;
}

bool verifySelectedUTXOs(const UTXOs& selected, const std::vector<int64_t>& expectedAmounts) {
    bool ret = true;
    if (selected.size() != expectedAmounts.size()) {
        ret = false;
        std::cerr << "Wrong number of selected UTXOs, " << selected.size() << " vs. " << expectedAmounts.size() << std::endl;
    }
    int errorCount = 0;
    for (auto i = 0ul; i < selected.size() && i < expectedAmounts.size(); ++i) {
        if (expectedAmounts[i] != selected[i].amount) {
            ret = false;
            ++errorCount;
            if (errorCount < 10) {
                std::cerr << "Wrong UTXOs amount, pos " << i << " amount " << selected[i].amount << " expected " << expectedAmounts[i] << std::endl;
            }
        }
    }
    return ret;
}

bool verifyPlan(const TransactionPlan& plan, const std::vector<int64_t>& utxoAmounts, int64_t outputAmount, int64_t fee, Common::Proto::SigningError error) {
    bool ret = true;
    if (!verifySelectedUTXOs(plan.utxos, utxoAmounts)) {
        ret = false;
    }
    if (plan.amount != outputAmount) {
        ret = false;
        std::cerr << "Mismatch in amount, act " << plan.amount << ", exp " << outputAmount << std::endl;
    }
    if (plan.fee != fee) {
        ret = false;
        std::cerr << "Mismatch in fee, act " << plan.fee << ", exp " << fee << std::endl;
    }
    int64_t sumExpectedUTXOs = 0;
    for (long long utxoAmount : utxoAmounts) {
        sumExpectedUTXOs += utxoAmount;
    }
    if (plan.availableAmount != sumExpectedUTXOs) {
        ret = false;
        std::cerr << "Mismatch in availableAmount, act " << plan.availableAmount << ", exp " << sumExpectedUTXOs << std::endl;
    }
    int64_t expectedChange = sumExpectedUTXOs - outputAmount - fee;
    if (plan.change != expectedChange) {
        ret = false;
        std::cerr << "Mismatch in change, act " << plan.change << ", exp " << expectedChange << std::endl;
    }
    if (plan.error != Common::Proto::OK) {
        if (error != Common::Proto::OK) {
            if (plan.error != error) {
                ret = false;
                std::cerr << "Unexpected error, act " << std::to_string(plan.error) << ", exp " << std::to_string(plan.error) << std::endl;
            }
        } else {
            ret = false;
            std::cerr << "Unexpected error " << std::to_string(plan.error) << std::endl;
        }
    } else {
        if (error != Common::Proto::OK) {
            ret = false;
            std::cerr << "Missing expected error " << std::to_string(plan.error) << std::endl;
        }
    }
    return ret;
}

bool operator==(const EncodedTxSize& s1, const EncodedTxSize& s2) {
    return s1.virtualBytes == s2.virtualBytes && s1.segwit == s2.segwit && s1.nonSegwit == s2.nonSegwit;
}

EncodedTxSize getEncodedTxSize(const Transaction& tx) {
    EncodedTxSize size{};
    { // full segwit size
        Data data;
        tx.encode(data, Transaction::SegwitFormatMode::Segwit);
        size.segwit = data.size();
    }
    { // non-segwit
        Data data;
        tx.encode(data, Transaction::SegwitFormatMode::NonSegwit);
        size.nonSegwit = data.size();
    }
    int64_t witnessSize = 0;
    { // double check witness part: witness plus 2 bytes is the difference between segwit and non-segwit size
        Data data;
        tx.encodeWitness(data);
        witnessSize = data.size();
        assert(size.segwit - size.nonSegwit == 2ul + witnessSize);
    }
    // compute virtual size: 3/4 of (smaller) non-segwit + 1/4 of segwit size
    uint64_t sum = size.nonSegwit * 3 + size.segwit;
    size.virtualBytes = sum / 4 + (sum % 4 != 0);
    // alternative computation: (smaller) non-segwit + 1/4 of the diff (witness-only)
    uint64_t vSize2 = size.nonSegwit + (witnessSize + 2) / 4 + ((witnessSize + 2) % 4 != 0);
    assert(size.virtualBytes == vSize2);
    return size;
}

bool validateEstimatedSize(const Transaction& tx, int smallerTolerance, int biggerTolerance) {
    if (tx.previousEstimatedVirtualSize == 0) {
        // no estimated size, do nothing
        return true;
    }
    bool ret = true;
    auto estSize = tx.previousEstimatedVirtualSize;
    uint64_t vsize = getEncodedTxSize(tx).virtualBytes;
    int64_t diff = estSize - vsize;
    if (diff < smallerTolerance) {
        ret = false;
        std::cerr << "Estimated size too small! " << std::to_string(estSize) << " vs. " << std::to_string(vsize) << std::endl;
    }
    if (diff > biggerTolerance) {
        ret = false;
        std::cerr << "Estimated size too big! " << std::to_string(estSize) << " vs. " << std::to_string(vsize) << std::endl;
    }
    return ret;
}

void prettyPrintScript(const Script& script) {
    Data data;
    encodeVarInt(script.bytes.size(), data);
    std::cout << "  \"" << hex(data) << "\"";
    std::cout << "  \"" << hex(script.bytes) << "\"";
}

void prettyPrintTransaction(const Transaction& tx, bool useWitnessFormat) {
    Data data;
    encode32LE(tx._version, data);
    std::cout << "        \"" << hex(data) << "\" // version\n";

    if (useWitnessFormat) {
        std::cout << "        \"0001\" // marker & flag\n";
    }

    // txins
    data.clear();
    encodeVarInt(tx.inputs.size(), data);
    std::cout << "        \"" << hex(data) << "\" // inputs\n";
    for (auto& input : tx.inputs) {
        auto& outpoint = reinterpret_cast<const TW::Bitcoin::OutPoint&>(input.previousOutput);
        std::cout << "            \"" << hex(outpoint.hash) << "\"";
        data.clear();
        encode32LE(outpoint.index, data);
        std::cout << "  \"" << hex(data) << "\"";
        prettyPrintScript(input.script);
        data.clear();
        encode32LE(input.sequence, data);
        std::cout << "  \"" << hex(data) << "\"\n";
    }

    // txouts
    data.clear();
    encodeVarInt(tx.outputs.size(), data);
    std::cout << "        \"" << hex(data) << "\" // outputs\n";
    for (auto& output : tx.outputs) {
        data.clear();
        encode64LE(output.value, data);
        std::cout << "            \"" << hex(data) << "\"";
        prettyPrintScript(output.script);
        std::cout << "\n";
    }

    if (useWitnessFormat) {
        std::cout << "        // witness\n";
        for (auto& input : tx.inputs) {
            data.clear();
            encodeVarInt(input.scriptWitness.size(), data);
            std::cout << "            \"" << hex(data) << "\"\n";
            for (auto& item : input.scriptWitness) {
                data.clear();
                encodeVarInt(item.size(), data);
                std::cout << "                \"" << hex(data) << "\"";
                std::cout << "  \"" << hex(item) << "\"\n";
            }
        }
    }

    data.clear();
    encode32LE(tx.lockTime, data); // nLockTime
    std::cout << "        \"" << hex(data) << "\" // nLockTime\n";
    std::cout << "\n";
}

} // namespace TW::Bitcoin
