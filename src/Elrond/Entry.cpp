// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"
#include "Address.h"
#include "Signer.h"
#include <proto/TransactionCompiler.pb.h>

using namespace TW;
using namespace std;

namespace TW::Elrond {
// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const string &address, TW::byte, TW::byte, const char *) const {
    return Address::isValid(address);
}

string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey &publicKey, TW::byte, const char *) const {
    return Address(publicKey).string();
}

Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string &address) const {
    Address addr;
    if (!Elrond::Address::decode(address, addr)) {
        return Data();
    }
    return addr.getKeyHash();
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data &dataIn, TW::Data &dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

string Entry::signJSON([[maybe_unused]] TWCoinType coin, const std::string &json, const Data &key) const {
    return Signer::signJSON(json, key);
}

Data Entry::preImageHashes(TWCoinType coin, const Data &txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [=](const auto &input, auto &output) {
            auto unsignedTxBytes = Signer::buildUnsignedTxBytes(input);
            output.set_data(unsignedTxBytes.data(), unsignedTxBytes.size());
            output.set_data_hash(unsignedTxBytes.data(), unsignedTxBytes.size()); });
}

void Entry::compile(TWCoinType coin, const Data &txInputData, const std::vector<Data> &signatures,
                    const std::vector<PublicKey> &publicKeys, Data &dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto &input, auto &output) {
            if (signatures.size() == 0 || publicKeys.size() == 0) {
                output.set_error(Common::Proto::Error_invalid_params);
                output.set_error_message("empty signatures or publickeys");
                return;
            } 

            if (signatures.size() != 1) {
                output.set_error(Common::Proto::Error_no_support_n2n);
                output.set_error_message(
                    Common::Proto::SigningError_Name(Common::Proto::Error_no_support_n2n));
                return;
            }

            output = Signer::buildSigningOutput(input, signatures[0]); });
}

} // namespace TW::Elrond
