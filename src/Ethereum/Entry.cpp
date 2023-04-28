// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "proto/Common.pb.h"
#include "proto/TransactionCompiler.pb.h"
#include "Signer.h"

#include "proto/TransactionCompiler.pb.h"

namespace TW::Ethereum {

using namespace std;

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Address::isValid(address);
}

string Entry::normalizeAddress([[maybe_unused]] TWCoinType coin, const string& address) const {
    // normalized with EIP55 checksum
    return Address(address).string();
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Address(publicKey).string();
}

Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    const auto addr = Address(address);
    return {addr.bytes.begin(), addr.bytes.end()};
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

string Entry::signJSON([[maybe_unused]] TWCoinType coin, const std::string& json, const Data& key) const {
    return Signer::signJSON(json, key);
}

Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            const auto transaction = Signer::build(input);
            const auto chainId = load(data(input.chain_id())); // retrieve chainId from input
            auto preHash = transaction->preHash(chainId);
            auto preImage = transaction->serialize(chainId);
            output.set_data_hash(preHash.data(), preHash.size());
            output.set_data(preImage.data(), preImage.size());
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, [[maybe_unused]] const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() != 1) {
                output.set_error(Common::Proto::Error_signatures_count);
                output.set_error_message(Common::Proto::SigningError_Name(Common::Proto::Error_signatures_count));
                return;
            }
            output = Signer::compile(input, signatures[0]);
        });
}

Data Entry::buildTransactionInput([[maybe_unused]] TWCoinType coinType, [[maybe_unused]] const std::string& from, const std::string& to, const uint256_t& amount, [[maybe_unused]] const std::string& asset, [[maybe_unused]] const std::string& memo, const std::string& chainId) const {
    Proto::SigningInput input;

    auto chainIdData = store(uint256_t(1));
    if (chainId.length() > 0) {
        // parse amount
        uint256_t chainIdUint256{chainId};
        chainIdData = store(chainIdUint256);
    }
    input.set_chain_id(chainIdData.data(), chainIdData.size());

    if (!Address::isValid(to)) {
        throw std::invalid_argument("Invalid to address");
    }
    input.set_to_address(to);

    auto& transfer = *input.mutable_transaction()->mutable_transfer();
    const auto amountData = store(amount);
    transfer.set_amount(amountData.data(), amountData.size());

    // not set: nonce, gasPrice, gasLimit, tx_mode (need to be set afterwards)

    const auto txInputData = data(input.SerializeAsString());
    return txInputData;
}

} // namespace TW::Ethereum
