// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"
#include "Address.h"
#include "Serialization.h"
#include "Signer.h"
#include "../proto/TransactionCompiler.pb.h"

using namespace TW::NEAR;
using namespace TW;
using namespace std;

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte,
                            const char*) const {
    return Address::isValid(address);
}

string Entry::normalizeAddress(TWCoinType coin, const string& address) const {
    return Address(address).string();
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte,
                            const char*) const {
    return Address(publicKey).string();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    auto input = Proto::SigningInput();
    auto output = TxCompiler::Proto::PreSigningOutput();
    Data transaction;
    if (input.ParseFromArray(txInputData.data(), (int)txInputData.size())) {
        transaction = TW::NEAR::transactionDataWithPublicKey(input);
        auto hash = Hash::sha256(transaction);
        output.set_data(transaction.data(), transaction.size());
        output.set_datahash(hash.data(), hash.size());
    } else {
        output.set_errorcode(TxCompiler::Proto::Error_input_parse);
        output.set_error("SigningInput parse failed");
    }
    return TW::data(output.SerializeAsString());
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    auto input = Proto::SigningInput();
    if (input.ParseFromArray(txInputData.data(), (int)txInputData.size()) &&
        (signatures.size() == 1)) {
        auto transaction = TW::NEAR::transactionDataWithPublicKey(std::move(input));
        auto signedTransaction = signedTransactionData(transaction, signatures[0]);
        auto output = Proto::SigningOutput();
        output.set_signed_transaction(signedTransaction.data(), signedTransaction.size());
        auto serializedOut = output.SerializeAsString();
        dataOut.insert(dataOut.end(), serializedOut.begin(), serializedOut.end());
    }
}