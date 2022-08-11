// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Signer.h"
#include "proto/Bitcoin.pb.h"
#include "../Bitcoin/Address.h"
#include "../Bitcoin/SegwitAddress.h"

using namespace TW::BitcoinDiamond;
using namespace TW;
using namespace std;

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte,
                            const char* hrp) const {
    return Bitcoin::Address::isValid(address) || TW::Bitcoin::SegwitAddress::isValid(address, hrp);
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte p2pkh,
                            const char* hrp) const {
    return deriveAddress(coin, TWDerivationDefault, publicKey, p2pkh, hrp);
}

std::string Entry::deriveAddress(TWCoinType coin, TWDerivation derivation, const PublicKey& publicKey,
                          TW::byte p2pkh, const char* hrp) const {
    switch (derivation) {
    case TWDerivationBitcoinLegacy:
    case TWDerivationDefault:
        return Bitcoin::Address(publicKey, p2pkh).string();
    default:
        return TW::Bitcoin::SegwitAddress(publicKey, hrp).string();
    }
}

TW::Data Entry::addressToData(TWCoinType coin, const std::string& address) const {
    const auto decoded = Bitcoin::SegwitAddress::decode(address);
    if (!std::get<2>(decoded)) {
        // check if it is a legacy address
        if (Bitcoin::Address::isValid(address)) {
            const auto addr = Bitcoin::Address(address);
            return {addr.bytes.begin() + 1, addr.bytes.end()};
        }
        return {Data()};
    }
    return std::get<0>(decoded).witnessProgram;
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Bitcoin::Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan(TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    planTemplate<Signer, Bitcoin::Proto::SigningInput>(dataIn, dataOut);
}

Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<SigningInput, PreSigningOutput>(
        txInputData,
        [](const auto& input, auto& output) { output = Signer::preImageHashes(input); });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<SigningInput, SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() == 0 || publicKeys.size() == 0) {
                output.set_error(Common::Proto::Error_invalid_params);
                output.set_error_message("empty signatures or publickeys");
                return;
            }

            if (signatures.size() != publicKeys.size()) {
                output.set_error(Common::Proto::Error_invalid_params);
                output.set_error_message("signatures size and publickeys size not equal");
                return;
            }

            HashPubkeyList externalSignatures;
            auto n = signatures.size();
            for (auto i = 0; i < n; ++i) {
                externalSignatures.push_back(std::make_pair(signatures[i], publicKeys[i].bytes));
            }

            output = Signer::sign(input, externalSignatures);
        });
}
