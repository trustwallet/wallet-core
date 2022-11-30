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

uint256_t hashKeyWithIndex(const std::string& seed, std::size_t index) {
    auto data = parse_hex(seed);
    data.push_back(static_cast<uint8_t>(index));
    auto out = Hash::sha256(data);
    return load(out);
}

std::string grindKey(const std::string& seed) {
    std::size_t index{0};
    int256_t key = hashKeyWithIndex(seed, index);
    while (key >= internal::gStarkDeriveBias) {
        std::stringstream ss;
        ss << std::hex << key;
        key = hashKeyWithIndex(ss.str(), index);
        index += 1;
    }
    auto final = key % internal::gStarkCurveN;
    std::stringstream ss;
    ss << std::hex << final;
    return ss.str();
}

std::string getPrivateKeyFromSeed(const std::string& seed, const std::string& path) {
    auto dataSeed = parse_hex(seed);
    auto key = HDWallet::bip32DeriveRawSeed(TWCoinTypeEthereum, dataSeed, DerivationPath(path));
    auto data = parse_hex(grindKey(hex(key.bytes)), true);
    return hex(data);
}

PrivateKey getPrivateKeyFromEthPrivKey(const PrivateKey& ethPrivKey) {
    return PrivateKey(parse_hex(ImmutableX::grindKey(hex(ethPrivKey.bytes)), true));
}

std::string getPrivateKeyFromRawSignature(const std::string& signature, const std::string& ethAddress) {
    using namespace internal;
    auto data = parse_hex(signature);
    auto ethSignature = Ethereum::Signer::signatureDataToStructSimple(data);
    auto seed = store(ethSignature.s);
    return getPrivateKeyFromSeed(hex(seed), Ethereum::accountPathFromAddress(ethAddress, gLayer, gApplication, gIndex));
}

std::string getPublicKeyFromPrivateKey(const std::string& privateKey) {
    auto pubKey = starknet_pubkey_from_private(privateKey.c_str());
    std::string pubKeyStr = pubKey;
    free_string(pubKey);
    return pubKeyStr;
}

} // namespace TW::ImmutableX
