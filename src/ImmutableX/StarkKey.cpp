// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Ethereum/EIP2645.h>
#include <Ethereum/Signer.h>
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
    //auto data = parse_hex(signature);
    auto ethSignature = Ethereum::Signer::signatureDataToStructSimple(signature);
    auto seed = store(ethSignature.s);
    return getPrivateKeyFromSeed(seed, derivationPath);
}

Data getPublicKeyFromPrivateKey(const Data& privateKey) {
    auto pubKey = Rust::starknet_pubkey_from_private(hex(privateKey).c_str());
    if (pubKey.code != Rust::OK_CODE) {
        return {};
    }
    const auto toReturn = parse_hex(pubKey.result, true);
    Rust::free_string(pubKey.result);
    return toReturn;
}

Data sign(const Data& privateKey, const Data& digest) {
    auto privKeyStr = hex(privateKey);
    auto hexDigest = hex(digest);
    auto resultSignature = Rust::starknet_sign(privKeyStr.c_str(), hexDigest.c_str());
    if (resultSignature.code != Rust::OK_CODE) {
        return {};
    }
    auto toReturn = parse_hex(resultSignature.result);
    Rust::free_string(resultSignature.result);
    return toReturn;
}

bool verify(const Data& pubKey, const Data& signature, const Data& digest) {
    if (signature.size() != 64) {
        return false;
    }
    auto r = hex(subData(signature, 0, 32));
    auto s = hex(subData(signature, 32));
    auto pubKeyStr = hex(pubKey);
    auto digestStr = hex(digest);
    const auto res = Rust::starknet_verify(pubKeyStr.c_str(), digestStr.c_str(), r.c_str(), s.c_str());
    return res.code == Rust::OK_CODE && res.result;
}

} // namespace TW::ImmutableX
