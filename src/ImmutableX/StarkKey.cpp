// Copyright © 2017-2022 Trust Wallet.
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
    auto pubKey = starknet_pubkey_from_private(hex(privateKey).c_str());
    std::string pubKeyStr = pubKey;
    free_string(pubKey);
    return parse_hex(pubKeyStr, true);
}

Data sign(const Data& privateKey, const Data& digest) {
    auto privKeyStr = hex(privateKey);
    auto hexDigest = hex(digest);
    auto resultSignature = starknet_sign(privKeyStr.c_str(), hexDigest.c_str());
    auto toReturn = parse_hex(resultSignature);
    free_string(resultSignature);
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
    return starknet_verify(pubKeyStr.c_str(), digestStr.c_str(), r.c_str(), s.c_str());
}

} // namespace TW::ImmutableX
