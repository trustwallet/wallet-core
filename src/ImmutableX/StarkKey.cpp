// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <HDWallet.h>
#include <Hash.h>
#include <HexCoding.h>
#include <ImmutableX/Constants.h>
#include <ImmutableX/StarkKey.h>
#include <rust/bindgen/WalletCoreRSBindgen.h>

namespace TW::ImmutableX {

uint256_t hashKeyWithIndex(const Data& seed, std::size_t index) {
    auto data = seed;
    data.push_back(static_cast<uint8_t>(index));
    auto out = Hash::sha256(data);
    return load(out);
}

std::string grindKey(const Data& seed) {
    std::size_t index{0};
    int256_t key = hashKeyWithIndex(seed, index);
    while (key >= internal::gStarkDeriveBias) {
        key = hashKeyWithIndex(store(uint256_t(key)), index);
        index += 1;
    }
    auto finalKey = key % internal::gStarkCurveN;
    std::stringstream ss;
    ss << std::hex << finalKey;
    return ss.str();
}

PrivateKey getPrivateKeyFromSeed(const Data& seed, const DerivationPath& path) {
    auto key = HDWallet<32>::bip32DeriveRawSeed(TWCoinTypeEthereum, seed, path);
    auto data = parse_hex(grindKey(key.bytes), true);
    return PrivateKey(data);
}

PrivateKey getPrivateKeyFromEthPrivKey(const PrivateKey& ethPrivKey) {
    return PrivateKey(parse_hex(ImmutableX::grindKey(ethPrivKey.bytes), true));
}

PrivateKey getPrivateKeyFromRawSignature(const Data& signature, const DerivationPath& derivationPath) {
    using namespace internal;
    // The signature is `rsv`, where `s` starts at 32 and is 32 long.
    auto seed = subData(signature, 32, 32);
    return getPrivateKeyFromSeed(seed, derivationPath);
}

} // namespace TW::ImmutableX
