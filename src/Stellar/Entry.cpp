// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"

#include "Address.h"
#include "proto/TransactionCompiler.pb.h"
#include "Signer.h"
#include "Base32.h"
#include "Coin.h"
#include "HexCoding.h"
#include "Crc.h"

namespace TW::Stellar {

constexpr uint8_t ed25519SecretSeed = 18 << 3;

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Address::isValid(address);
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Address(publicKey).string();
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

TW::Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            Signer signer(input);

            auto preImage = signer.signaturePreimage();
            auto preImageHash = Hash::sha256(preImage);
            output.set_data_hash(preImageHash.data(), preImageHash.size());
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

            output = Signer(input).compile(signatures[0]);
        });
}

// Taken from: https://github.com/stellar/js-stellar-base/blob/087e2d651a59b5cbed01386b4b8c45862d358259/src/util/checksum.js#L1C1-L17C2
bool verifyChecksum(const Data& expected, const Data& actual) {
    if (expected.size() != actual.size()) {
        return false;
    }

    if (expected.empty()) {
        return true;
    }

    for (size_t i = 0; i < expected.size(); i++) {
        if (expected[i] != actual[i]) {
            return false;
        }
    }

    return true;
}

// Taken from: https://github.com/stellar/js-stellar-base/blob/087e2d651a59b5cbed01386b4b8c45862d358259/src/strkey.js#L290
PrivateKey Entry::decodePrivateKey(TWCoinType coin, const std::string& privateKey) const {
    Data decoded;
    Base32::decode(privateKey, decoded);

    if (decoded.size() != 35) {
        auto hexData = parse_hex(privateKey);
        return PrivateKey(hexData, TW::curve(coin));
    }
    
    uint8_t versionByte = decoded[0];
    if (versionByte != ed25519SecretSeed) {
        throw std::invalid_argument("Invalid version byte. Expected " + std::to_string(ed25519SecretSeed) + 
                                ", got " + std::to_string(versionByte));
    }
    
    Data payload(decoded.begin(), decoded.end() - 2);
    Data data(payload.begin() + 1, payload.end());
    Data checksum(decoded.end() - 2, decoded.end());

    const auto expectedChecksum = Crc::crc16_xmodem(payload);
    if (!verifyChecksum(expectedChecksum, checksum)) {
        throw std::invalid_argument("invalid checksum");
    }

    return PrivateKey(data, TW::curve(coin));
}

} // namespace TW::Stellar
