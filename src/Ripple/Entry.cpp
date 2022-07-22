// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "XAddress.h"
#include "Signer.h"
#include "../proto/TransactionCompiler.pb.h"
#include <TrezorCrypto/ecdsa.h>

using namespace TW;
using namespace TW::Ripple;
using namespace std;

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte, const char*) const {
    return Address::isValid(address) || XAddress::isValid(address);
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte, const char*) const {
    return Address(publicKey).string();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

TW::Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TW::TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            auto signer = Signer(input);
            auto preimage = signer.preImage();
            output.set_data(preimage.data(), preimage.size());
            auto hash = Hash::sha512(preimage);
            auto preimageHash = Data(hash.begin(), hash.begin() + 32);
            output.set_data_hash(preimageHash.data(), preimageHash.size());
        });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() == 0 || publicKeys.size() == 0) {
                output.set_error(Common::Proto::Error_invalid_params);
                output.set_error_message("empty signatures or publickeys");
                return;
            }

            // We only support one-to-one transfer now.
            if (signatures.size() != 1 || publicKeys.size() != 1) {
                output.set_error(Common::Proto::Error_no_support_n2n);
                output.set_error_message("signatures and publickeys size can only be one");
                return;
            }

            // Ripple signature is special, so we have to convert to Der format
            std::array<uint8_t, 72> sigBytes;
            size_t size = ecdsa_sig_to_der(signatures[0].data(), sigBytes.data());
            auto sig = Data{};
            std::copy(sigBytes.begin(), sigBytes.begin() + size, std::back_inserter(sig));

            auto signer = Signer(input);
            output = signer.compile(sig, publicKeys[0]);
        });
}