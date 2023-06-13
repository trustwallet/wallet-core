// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Base64.h"
#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../proto/Stellar.pb.h"

#include <TrustWalletCore/TWStellarMemoType.h>

using namespace TW;

namespace TW::Stellar {
Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto signer = Signer(input);
    auto output = Proto::SigningOutput();
    output.set_signature(signer.sign());
    return output;
}

std::string Signer::sign() const noexcept {

    auto key = PrivateKey(Data(_input.private_key().begin(), _input.private_key().end()));
    auto account = Address(_input.account());
    auto encoded = encode(_input);

    auto encodedWithHeaders = Data();
    auto publicNetwork = _input.passphrase(); // Header
    auto passphrase = Hash::sha256(publicNetwork);
    encodedWithHeaders.insert(encodedWithHeaders.end(), passphrase.begin(), passphrase.end());
    auto transactionType = Data{0, 0, 0, 2};
    encodedWithHeaders.insert(encodedWithHeaders.end(), transactionType.begin(),
                              transactionType.end());
    encodedWithHeaders.insert(encodedWithHeaders.end(), encoded.begin(), encoded.end());

    auto hash = Hash::sha256(encodedWithHeaders);
    auto data = Data(hash.begin(), hash.end());

    auto sign = key.sign(data, TWCurveED25519);

    auto signature = Data();
    signature.insert(signature.end(), encoded.begin(), encoded.end());
    encode32BE(1, signature);
    signature.insert(signature.end(), account.bytes.end() - 4, account.bytes.end());
    encode32BE(static_cast<uint32_t>(sign.size()), signature);
    signature.insert(signature.end(), sign.begin(), sign.end());
    return Base64::encode(signature);
}

Data Signer::encode(const Proto::SigningInput& input) const {
    //    Address account, uint32_t fee, uint64_t sequence, uint32_t memoType,
    //    Data memoData, Address destination, uint64_t amount;
    auto data = Data();

    encodeAddress(Address(input.account()), data);
    encode32BE(input.fee(), data);
    encode64BE(input.sequence(), data);

    // Time bounds
    if (input.has_op_change_trust() && input.op_change_trust().valid_before() != 0) {
        encode32BE(1, data);
        encode64BE(0, data);                                      // from
        encode64BE(input.op_change_trust().valid_before(), data); // to
    } else {
        if (input.time_bounds() > 0) {
            encode32BE(1, data);
            encode64BE(0, data); //from
            encode64BE(input.time_bounds(), data); //to
        } else {
            encode32BE(0, data); // missing
        }
    }

    // Memo
    if (input.has_memo_id()) {
        encode32BE(TWStellarMemoTypeId, data);
        encode64BE(input.memo_id().id(), data);
    } else if (input.has_memo_text()) {
        encode32BE(TWStellarMemoTypeText, data);
        encode32BE(static_cast<uint32_t>(input.memo_text().text().size()), data);
        data.insert(data.end(), input.memo_text().text().begin(), input.memo_text().text().end());
        pad(data);
    } else if (input.has_memo_hash()) {
        encode32BE(TWStellarMemoTypeHash, data);
        data.insert(data.end(), input.memo_hash().hash().begin(), input.memo_hash().hash().end());
    } else if (input.has_memo_return_hash()) {
        encode32BE(TWStellarMemoTypeReturn, data);
        data.insert(data.end(), input.memo_return_hash().hash().begin(),
                    input.memo_return_hash().hash().end());
    } else {
        encode32BE(TWStellarMemoTypeNone, data);
    }

    // Operations
    encode32BE(1, data);                    // Operation list size. Only 1 operation.
    encode32BE(0, data);                    // Source equals account
    encode32BE(operationType(input), data); // Operation type

    switch (input.operation_oneof_case()) {
    case Proto::SigningInput::kOpCreateAccount:
    default:
        encodeAddress(Address(input.op_create_account().destination()), data);
        encode64BE(input.op_create_account().amount(), data);
        break;

    case Proto::SigningInput::kOpPayment:
        encodeAddress(Address(input.op_payment().destination()), data);
        encodeAsset(input.op_payment().asset(), data);
        encode64BE(input.op_payment().amount(), data);
        break;

    case Proto::SigningInput::kOpChangeTrust:
        encodeAsset(input.op_change_trust().asset(), data);
        encode64BE(0x7fffffffffffffff, data); // limit MAX
        break;

    case Proto::SigningInput::kOpCreateClaimableBalance: {
        const auto ClaimantTypeV0 = 0;
        encodeAsset(input.op_create_claimable_balance().asset(), data);
        encode64BE(input.op_create_claimable_balance().amount(), data);
        auto nClaimants = input.op_create_claimable_balance().claimants_size();
        encode32BE((uint32_t)nClaimants, data);
        for (auto i = 0; i < nClaimants; ++i) {
            encode32BE((uint32_t)ClaimantTypeV0, data);
            encodeAddress(Address(input.op_create_claimable_balance().claimants(i).account()), data);
            encode32BE((uint32_t)input.op_create_claimable_balance().claimants(i).predicate(), data);
            // Note: other predicates not supported, predicate-specific data would follow here
        }
    } break;

    case Proto::SigningInput::kOpClaimClaimableBalance: {
        const auto ClaimableBalanceIdTypeClaimableBalanceIdTypeV0 = 0;
        encode32BE((uint32_t)ClaimableBalanceIdTypeClaimableBalanceIdTypeV0, data);
        const auto balanceId = input.op_claim_claimable_balance().balance_id();
        if (balanceId.size() != 32) {
            return Data();
        }
        data.insert(data.end(), balanceId.begin(), balanceId.end());
    } break;
    }

    encode32BE(0, data); // Ext
    return data;
}

Data Signer::signaturePreimage() const {
    auto encoded = encode(_input);

    auto encodedWithHeaders = Data();
    auto publicNetwork = _input.passphrase(); // Header
    auto passphrase = Hash::sha256(publicNetwork);
    encodedWithHeaders.insert(encodedWithHeaders.end(), passphrase.begin(), passphrase.end());
    auto transactionType = Data{0, 0, 0, 2}; // Header
    encodedWithHeaders.insert(encodedWithHeaders.end(), transactionType.begin(),
                              transactionType.end());
    encodedWithHeaders.insert(encodedWithHeaders.end(), encoded.begin(), encoded.end());
    return encodedWithHeaders;
}

Proto::SigningOutput Signer::compile(const Data& sig) const {
    auto account = Address(_input.account());
    auto encoded = encode(_input);

    auto signature = Data();
    signature.insert(signature.end(), encoded.begin(), encoded.end());
    encode32BE(1, signature);
    signature.insert(signature.end(), account.bytes.end() - 4, account.bytes.end());
    encode32BE(static_cast<uint32_t>(sig.size()), signature);
    signature.insert(signature.end(), sig.begin(), sig.end());

    Proto::SigningOutput output;
    output.set_signature(Base64::encode(signature));
    return output;
}

uint32_t Signer::operationType(const Proto::SigningInput& input) {
    switch (input.operation_oneof_case()) {
    case Proto::SigningInput::kOpCreateAccount:
    default:
        return 0;
    case Proto::SigningInput::kOpPayment:
        return 1;
    case Proto::SigningInput::kOpChangeTrust:
        return 6;
    case Proto::SigningInput::kOpCreateClaimableBalance:
        return 14;
    case Proto::SigningInput::kOpClaimClaimableBalance:
        return 15;
    }
}

void Signer::encodeAddress(const Address& address, Data& data) {
    encode32BE(0, data);
    data.insert(data.end(), address.bytes.begin(), address.bytes.end());
}

void Signer::encodeAsset(const Proto::Asset& asset, Data& data) {
    uint32_t assetType = 0; // native
    std::string alphaUse;
    if (asset.issuer().length() > 0 && Address::isValid(asset.issuer()) && asset.alphanum4().length() > 0) {
        assetType = 1; // alphanum4
        alphaUse = asset.alphanum4();
    }
    encode32BE(assetType, data);
    if (assetType > 0) {
        for (auto i = 0ul; i < 4; ++i) {
            if (alphaUse.length() > i) {
                data.push_back(alphaUse[i]);
            } else {
                data.push_back(0); // pad with 0s
            }
        }
        encodeAddress(Address(asset.issuer()), data);
    }
}

void Signer::pad(Data& data) const {
    int pad = 0;
    int mod = static_cast<int>(data.size() % 4);
    if (mod > 0) {
        pad = 4 - mod;
    }
    while (pad-- > 0) {
        data.insert(data.end(), 0);
    }
}

} // namespace TW::Stellar
