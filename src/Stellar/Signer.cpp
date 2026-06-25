// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"
#include "Base64.h"
#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../proto/Stellar.pb.h"

#include <TrustWalletCore/TWStellarMemoType.h>

using namespace TW;

namespace TW::Stellar {
Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) {
    auto signer = Signer(input);
    auto signResult = signer.sign();
    if (!signResult) {
        Proto::SigningOutput output;
        output.set_error(Common::Proto::Error_invalid_memo);
        output.set_error_message(signResult.error());
        return output;
    }
    Proto::SigningOutput output;
    output.set_signature(signResult.payload());
    return output;
}

Result<std::string> Signer::sign() const {
    auto encodeResult = encode(_input);
    if (!encodeResult) {
        return Result<std::string>::failure(encodeResult.error());
    }
    auto encoded = encodeResult.payload();

    auto key = PrivateKey(_input.private_key(), TWCurveED25519);
    auto account = Address(_input.account());

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

    auto sig = key.sign(data);

    auto signature = Data();
    signature.insert(signature.end(), encoded.begin(), encoded.end());
    encode32BE(1, signature);
    signature.insert(signature.end(), account.bytes.end() - 4, account.bytes.end());
    encode32BE(static_cast<uint32_t>(sig.size()), signature);
    signature.insert(signature.end(), sig.begin(), sig.end());
    return Result<std::string>::success(Base64::encode(signature));
}

Result<Data> Signer::encode(const Proto::SigningInput& input) const {
    if (input.has_memo_hash() && input.memo_hash().hash().size() != 32) {
        return Result<Data>::failure("memo_hash must be exactly 32 bytes");
    }
    if (input.has_memo_return_hash() && input.memo_return_hash().hash().size() != 32) {
        return Result<Data>::failure("memo_return_hash must be exactly 32 bytes");
    }

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
            return Result<Data>::failure("balance_id must be exactly 32 bytes");
        }
        data.insert(data.end(), balanceId.begin(), balanceId.end());
    } break;
    }

    encode32BE(0, data); // Ext
    return Result<Data>::success(std::move(data));
}

Result<Data> Signer::signaturePreimage() const {
    auto encodeResult = encode(_input);
    if (!encodeResult) {
        return Result<Data>::failure(encodeResult.error());
    }
    auto encoded = encodeResult.payload();

    auto encodedWithHeaders = Data();
    auto publicNetwork = _input.passphrase(); // Header
    auto passphrase = Hash::sha256(publicNetwork);
    encodedWithHeaders.insert(encodedWithHeaders.end(), passphrase.begin(), passphrase.end());
    auto transactionType = Data{0, 0, 0, 2}; // Header
    encodedWithHeaders.insert(encodedWithHeaders.end(), transactionType.begin(),
                              transactionType.end());
    encodedWithHeaders.insert(encodedWithHeaders.end(), encoded.begin(), encoded.end());
    return Result<Data>::success(std::move(encodedWithHeaders));
}

Proto::SigningOutput Signer::compile(const Data& sig) const {
    auto encodeResult = encode(_input);
    if (!encodeResult) {
        Proto::SigningOutput output;
        output.set_error(Common::Proto::Error_invalid_memo);
        output.set_error_message(encodeResult.error());
        return output;
    }
    auto encoded = encodeResult.payload();
    auto account = Address(_input.account());

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
