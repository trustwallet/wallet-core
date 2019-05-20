#include "Signer.h"

#include "../Hash.h"
#include "../HexCoding.h"
#include "../Base64.h"
#include "../PrivateKey.h"
#include "../Data.h"
#include "Serialization.h"

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <string>

std::vector<uint8_t> TW::TERRA::Signer::sign() const {
    auto key = PrivateKey(input.private_key());
    auto hash = Hash::sha256(signaturePreimage());
    auto signature = key.sign(hash, TWCurveSECP256k1);
    return std::vector<uint8_t>(signature.begin(), signature.end() - 1);
}


std::string TW::TERRA::Signer::signaturePreimage() const {
    return ::TW::TERRA::signaturePreimage(input).dump();
}

nlohmann::json TW::TERRA::Signer::buildTransactionJSON(const Data& signature) const {
    auto sig = Terra::Proto::Signature();
    sig.set_signature(signature.data(), signature.size());
    auto privateKey = PrivateKey(input.private_key());
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    sig.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto transaction = Terra::Proto::Transaction();
    *transaction.mutable_fee() = input.fee();
    transaction.set_memo(input.memo());

    if (input.has_send_coins_message()) {
        *transaction.mutable_send_coins_message() = input.send_coins_message();
    } else if (input.has_stake_message()) {
        *transaction.mutable_stake_message() = input.stake_message();
    }

    *transaction.mutable_signature() = sig;

    return transactionJson(transaction);
}

TW::Terra::Proto::SigningOutput TW::TERRA::Signer::build() const {
    auto output = Terra::Proto::SigningOutput();

    auto signature = sign();
    auto txJson = buildTransactionJSON(signature);
    auto txEncoded = nlohmann::json::to_cbor(txJson);

    output.set_json(txJson.dump());
    output.set_encoded(txEncoded.data(), txEncoded.size());

    return output;
}
