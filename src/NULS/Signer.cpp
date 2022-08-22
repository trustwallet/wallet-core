// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include <TrezorCrypto/ecdsa.h>

#include "Address.h"
#include "BinaryCoding.h"
#include "../Hash.h"
#include "../PrivateKey.h"

using namespace TW;

namespace TW::NULS {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto output = Proto::SigningOutput();
    try {
        auto signer = Signer(input);
        auto data = signer.sign();
        output.set_encoded(data.data(), data.size());
    } catch (...) {
    }
    return output;
}

Signer::Signer(const Proto::SigningInput& input) : input(input) {
    uint256_t balance = load(input.balance());

    Proto::TransactionCoinTo coinTo;
    coinTo.set_to_address(input.to());
    coinTo.set_id_amount(input.amount());
    coinTo.set_assets_chainid(input.chain_id());
    coinTo.set_assets_id(input.idassets_id());
    coinTo.set_lock_time(0);
    *tx.add_output() = coinTo;

    tx.set_type(TX_TYPE);

    TW::uint256_t fromNULSAmount;
    // get mainnet chain id from address
    auto from = Address(input.from());
    if (input.chain_id() == from.chainID() && input.idassets_id() == 1) {
        // asset is NULS
        uint256_t txAmount = load(input.amount());
        uint32_t txSize =
            CalculatorTransactionSize(1, 1, static_cast<uint32_t>(tx.remark().size()));
        uint256_t fee = (uint256_t)CalculatorTransactionFee(txSize);
        fromNULSAmount = txAmount + fee;
        // update the amount with fee included
        Data amountWithFee = store(fromNULSAmount);
        std::string amountWithFeeStr;
        amountWithFeeStr.insert(amountWithFeeStr.begin(), amountWithFee.begin(),
                                amountWithFee.end());
        Proto::TransactionCoinFrom coinFrom;
        coinFrom.set_from_address(input.from());
        coinFrom.set_assets_chainid(input.chain_id());
        coinFrom.set_assets_id(input.idassets_id());
        coinFrom.set_id_amount(amountWithFeeStr);
        coinFrom.set_nonce(input.nonce());
        // default unlocked
        coinFrom.set_locked(0);
        *tx.add_input() = coinFrom;
    } else {
        // asset is not NULS
        // coinFrom
        // asset
        Proto::TransactionCoinFrom asset;
        asset.set_from_address(input.from());
        asset.set_assets_chainid(input.chain_id());
        asset.set_assets_id(input.idassets_id());
        asset.set_id_amount(input.amount());
        asset.set_nonce(input.asset_nonce());
        // default unlocked
        asset.set_locked(0);
        *tx.add_input() = asset;

        // fee
        uint32_t txSize = CalculatorTransactionSize(
            2, 1,
            static_cast<uint32_t>(
                tx.remark().size())); // 2 inputs, one for the asset, another for NULS fee
        uint256_t fee = (uint256_t)CalculatorTransactionFee(txSize);
        fromNULSAmount = fee;
        Data feeData = store(fee);
        std::string feeStr;
        feeStr.insert(feeStr.begin(), feeData.begin(), feeData.end());
        // add new input for fee
        Proto::TransactionCoinFrom txFee;
        txFee.set_from_address(input.from());
        txFee.set_assets_chainid(from.chainID());
        // network asset id 1 is NULS
        txFee.set_assets_id(1);
        txFee.set_id_amount(feeStr);
        txFee.set_nonce(input.nonce());
        // default unlocked
        txFee.set_locked(0);
        *tx.add_input() = txFee;
    }
    if (fromNULSAmount > balance) {
        throw std::invalid_argument("User account balance not sufficient");
    }

    tx.set_remark(input.remark());
    tx.set_timestamp(input.timestamp());
    tx.set_tx_data("");
}

Data Signer::sign() const {
    if (input.private_key().empty()) {
        throw std::invalid_argument("Must have private key string");
    }
    auto dataRet = Data();
    // Transaction Type
    encode16LE(static_cast<uint16_t>(tx.type()), dataRet);
    // Timestamp
    encode32LE(tx.timestamp(), dataRet);
    // Remark
    std::string remark = tx.remark();
    serializerRemark(remark, dataRet);
    // txData
    encodeVarInt(0, dataRet);
    // coinData
    serializerCoinData(tx, dataRet);
    // Calc transaction hash
    Data txHash = calcTransactionDigest(dataRet);

    Data privKey = data(input.private_key());
    auto priv = PrivateKey(privKey);
    auto transactionSignature = makeTransactionSignature(priv, txHash);
    encodeVarInt(transactionSignature.size(), dataRet);
    std::copy(transactionSignature.begin(), transactionSignature.end(),
              std::back_inserter(dataRet));

    return dataRet;
}

Data Signer::buildUnsignedTx() const {
    auto dataRet = Data();
    // Transaction Type
    encode16LE(static_cast<uint16_t>(tx.type()), dataRet);
    // Timestamp
    encode32LE(tx.timestamp(), dataRet);
    // Remark
    std::string remark = tx.remark();
    serializerRemark(remark, dataRet);
    // txData
    encodeVarInt(0, dataRet);
    // coinData
    serializerCoinData(tx, dataRet);

    return dataRet;
}

Data Signer::buildSignedTx(Data pubkey, Data sigBytes) const {
    auto unsignedTxBytes = buildUnsignedTx();

    Data transactionSignature = Data();
    encodeVarInt(pubkey.size(), transactionSignature);
    std::copy(pubkey.begin(), pubkey.end(), std::back_inserter(transactionSignature));

    std::array<uint8_t, 72> tempSigBytes;
    size_t size = ecdsa_sig_to_der(sigBytes.data(), tempSigBytes.data());
    auto signature = Data{};
    std::copy(tempSigBytes.begin(), tempSigBytes.begin() + size, std::back_inserter(signature));

    encodeVarInt(signature.size(), transactionSignature);
    std::copy(signature.begin(), signature.end(), std::back_inserter(transactionSignature));

    Data signedTxBytes = unsignedTxBytes;
    encodeVarInt(transactionSignature.size(), signedTxBytes);
    std::copy(transactionSignature.begin(), transactionSignature.end(),
              std::back_inserter(signedTxBytes));
    return signedTxBytes;
}

uint32_t Signer::CalculatorTransactionSize(uint32_t inputCount, uint32_t outputCount,
                                           uint32_t remarkSize) const {
    uint32_t size = TRANSACTION_FIX_SIZE + TRANSACTION_SIG_MAX_SIZE +
                    TRANSACTION_INPUT_SIZE * inputCount + TRANSACTION_OUTPUT_SIZE * outputCount +
                    remarkSize;
    return size;
}

uint64_t Signer::CalculatorTransactionFee(uint64_t size) const {
    uint64_t fee = (size / 1024) * MIN_PRICE_PRE_1024_BYTES;
    if (size % 1024 > 0) {
        fee += MIN_PRICE_PRE_1024_BYTES;
    }
    return fee;
}

} // namespace TW::NULS
