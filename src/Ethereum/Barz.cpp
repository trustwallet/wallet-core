// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ABI.h"
#include "AddressChecksum.h"
#include "EIP1014.h"
#include "Hash.h"
#include "HexCoding.h"
#include <WebAuthn.h>
#include "../proto/Barz.pb.h"

namespace TW::Barz {

using ParamBasePtr = std::shared_ptr<TW::Ethereum::ABI::ParamBase>;
using ParamCollection = std::vector<ParamBasePtr>;

std::string getCounterfactualAddress(const TW::Barz::Proto::ContractAddressInput input) {
    auto params = TW::Ethereum::ABI::ParamTuple();
    params.addParam(std::make_shared<TW::Ethereum::ABI::ParamAddress>(parse_hex(input.diamond_cut_facet())));
    params.addParam(std::make_shared<TW::Ethereum::ABI::ParamAddress>(parse_hex(input.account_facet())));
    params.addParam(std::make_shared<TW::Ethereum::ABI::ParamAddress>(parse_hex(input.verification_facet())));
    params.addParam(std::make_shared<TW::Ethereum::ABI::ParamAddress>(parse_hex(input.entry_point())));
    params.addParam(std::make_shared<TW::Ethereum::ABI::ParamAddress>(parse_hex(input.security_manager())));
    params.addParam(std::make_shared<TW::Ethereum::ABI::ParamAddress>(parse_hex(input.facet_registry())));

    Data publicKey;
    switch (input.owner().kind_case()) {
    case TW::Barz::Proto::ContractOwner::KindCase::KIND_NOT_SET:
        return "";
    case TW::Barz::Proto::ContractOwner::KindCase::kPublicKey:
        publicKey = parse_hex(input.owner().public_key());
        break;
    case TW::Barz::Proto::ContractOwner::KindCase::kAttestationObject:
        const auto attestationObject = parse_hex(input.owner().attestation_object());
        publicKey = subData(TW::WebAuthn::getPublicKey(attestationObject)->bytes, 1); // Drop the first byte which corresponds to the public key type
        break;
    }
    params.addParam(std::make_shared<TW::Ethereum::ABI::ParamByteArray>(publicKey));

    Data encoded;
    params.encode(encoded);

    Data initCode = parse_hex(input.bytecode());
    append(initCode, encoded);

    const Data initCodeHash = Hash::keccak256(initCode);
    const Data salt(32, 0);
    return Ethereum::checksumed(TW::Ethereum::Address(hexEncoded(TW::Ethereum::create2Address(input.factory(), salt, initCodeHash))));
}

} // namespace TW::Barz
