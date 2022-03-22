// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "proto/Common.pb.h"
#include "proto/TransactionCompiler.pb.h"
#include "Signer.h"

using namespace TW::Ethereum;
using namespace TW;
using namespace std;

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte, const char*) const {
    return Address::isValid(address);
}

string Entry::normalizeAddress(TWCoinType coin, const string& address) const {
    // normalized with EIP55 checksum
    return Address(address).string();
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte, const char*) const {
    return Address(publicKey).string();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

string Entry::signJSON(TWCoinType coin, const std::string& json, const Data& key) const { 
    return Signer::signJSON(json, key);
}

Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            const auto transaction = Signer::build(input);
            const auto chainId = load(data(input.chain_id())); // retrieve chainId from input
            // return preimage hash and dummy pubkeyhash (not available here, and only one signature anyways)
            auto preHash = transaction->preHash(chainId);
            output.set_datahash(preHash.data(), preHash.size());
        });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() != 1) {
                output.set_errorcode(Common::Proto::Error_no_support_n2n);
                output.set_error(Common::Proto::SigningError_Name(Common::Proto::Error_no_support_n2n));
                return;
            }
            output = Signer::compile(input, signatures[0]);
        });
}

Data Entry::buildTransactionInput(TWCoinType coinType, const std::string& from, const std::string& to, const uint256_t& amount, const std::string& asset, const std::string& memo, const std::string& chainId) const {
    Proto::SigningInput input;

    auto chainIdData = store(uint256_t(1));
    if (chainId.length() > 0) {
        // parse amount
        uint256_t chainIdUint256 { chainId };
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
