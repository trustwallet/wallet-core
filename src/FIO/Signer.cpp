// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"
#include "Address.h"
#include "Actor.h"

#include "../Base58.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../proto/Common.pb.h"
#include "TransactionBuilder.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>

#include <cassert>

namespace TW::FIO {

using namespace std;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    FIO::Proto::SigningOutput output;
    try {    
        const string json = TransactionBuilder::sign(input);
        output.set_json(json);
    } catch(const std::exception& e) {
        output.set_error(Common::Proto::Error_internal);
    }
    return output;
}

Proto::SigningOutput Signer::compile(const Proto::SigningInput& input, const Data& signature) noexcept {
    FIO::Proto::SigningOutput output;
    output = TransactionBuilder::buildSigningOutput(input, signature);
    return output;
}

Data Signer::signData(const PrivateKey& privKey, const Data& data) {
    Data hash = Hash::sha256(data);
    Data signature = privKey.sign(hash, TWCurveSECP256k1, isCanonical);
    return signature;
}

std::string Signer::signatureToBase58(const Data& sig) {
    Data sigWithSuffix(sig);
    append(sigWithSuffix, TW::data(SignatureSuffix));
    // take hash, ripemd, first 4 bytes 
    Data hash = Hash::ripemd(sigWithSuffix);
    Data sigWithChecksum(sig);
    append(sigWithChecksum, TW::data(hash.data(), 4));
    string s = SignaturePrefix + Base58::encode(sigWithChecksum);
    return s;
}

bool Signer::verify(const PublicKey& pubKey, const Data& data, const Data& signature) {
    return pubKey.verify(TW::data(signature.data() + 1, signature.size() - 1), data);
}

// canonical check for FIO, both R and S lenght is 32
int Signer::isCanonical([[maybe_unused]] uint8_t by, uint8_t sig[64]) {
    return !(sig[0] & 0x80)
        && !(sig[0] == 0 && !(sig[1] & 0x80))
        && !(sig[32] & 0x80)
        && !(sig[32] == 0 && !(sig[33] & 0x80));
}

} // namespace TW::FIO
