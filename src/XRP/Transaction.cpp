// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BinaryCoding.h"
#include "Transaction.h"
#include "../BinaryCoding.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>

namespace TW::Ripple {

const int NETWORK_PREFIX = 0x53545800;

Data Transaction::serialize() const {
	// See https://xrpl.org/serialization.html

    auto data = Data();

    /// field must be sorted by field type then by field name
    /// "type"
    encodeType(FieldType::int16, 2, data);
    encode16BE(uint16_t(transaction_type), data);

    /// "flags"
    encodeType(FieldType::int32, 2, data);
    encode32BE(static_cast<uint32_t>(flags), data);

    /// "sequence"
    encodeType(FieldType::int32, 4, data);
    encode32BE(sequence, data);

    /// "destinationTag"
    if ((transaction_type == TransactionType::payment) && encode_tag) {
        encodeType(FieldType::int32, 14, data);
        encode32BE(static_cast<uint32_t>(destination_tag), data);
    }

    /// "lastLedgerSequence"
    if (last_ledger_sequence > 0) {
        encodeType(FieldType::int32, 27, data);
        encode32BE(last_ledger_sequence, data);
    }

    /// "NFTokenId"
    if ((transaction_type == TransactionType::NFTokenCreateOffer) ||
        (transaction_type == TransactionType::NFTokenBurn)) {
        encodeType(FieldType::hash256, 10, data);
        data.insert(data.end(), nftoken_id.begin(), nftoken_id.end());
    }

    /// "NFTokenAcceptOffer"
    if (transaction_type == TransactionType::NFTokenAcceptOffer) {
        encodeType(FieldType::hash256, 29, data);
        data.insert(data.end(), sell_offer.begin(), sell_offer.end());
    }

    /// "amount"
    if ((transaction_type == TransactionType::payment) ||
        (transaction_type == TransactionType::NFTokenCreateOffer)) {
        encodeType(FieldType::amount, 1, data);
        append(data,
               (currency_amt.currency.size() > 0) ?
                 serializeCurrencyAmt(currency_amt) :
                 serializeAmount(amount));
    } else if (transaction_type == TransactionType::TrustSet) {
        encodeType(FieldType::amount, 3, data);
        append(data, serializeCurrencyAmt(limit_amt));
    }

    /// "fee"
    encodeType(FieldType::amount, 8, data);
    append(data, serializeAmount(fee));

    /// "signingPubKey"
    if (!pub_key.empty()) {
        encodeType(FieldType::vl, 3, data);
        encodeBytes(pub_key, data);
    }

    /// "txnSignature"
    if (!signature.empty()) {
        encodeType(FieldType::vl, 4, data);
        encodeBytes(signature, data);
    }

    /// "account"
    encodeType(FieldType::account, 1, data);
    encodeBytes(serializeAddress(account), data);

    /// "destination"
    if ((transaction_type == TransactionType::payment) ||
        (transaction_type == TransactionType::NFTokenCreateOffer)) {
        encodeType(FieldType::account, 3, data);
        encodeBytes(destination, data);
    }

    /// "NFTokenOffers"
    if (transaction_type == TransactionType::NFTokenCancelOffer) {
        // only support one offer
        encodeType(FieldType::vector256, 4, data);
        encodeBytes(token_offers, data);
    }

    return data;
}

Data Transaction::getPreImage() const {
    auto preImage = Data();
    encode32BE(NETWORK_PREFIX, preImage);
    append(preImage, serialize());
    return preImage;
}

Data Transaction::serializeAmount(int64_t amount) {
    if (amount < 0) {
        return Data();
    }
    auto data = Data();
    encode64BE(uint64_t(amount), data);
    /// clear first bit to indicate XRP
    data[0] &= 0x7F;
    /// set second bit to indicate positive number
    data[0] |= 0x40;
    return data;
}

Data Transaction::serializeCurrencyAmt(const CurrencyAmt& currency_amt) {
    // Calculate value
    // https://xrpl.org/serialization.html#token-amount-format
    int64_t sign = 0;
    int64_t mantissa = 0;
    int32_t exp = 0;
    try {
        // double stores 52-bit mantissa but ripple stores 54-bit mantissa
        // can parse to boost::multiprecision::float128_t but needs quadmath.h
        int32_t num_after_dot = 0;
        bool after_dot = false;
        bool after_e = false;
        bool has_exp = false;
        std::ostringstream mantissa_oss, exp_oss;
        for (auto i : currency_amt.value) {
            if (i == '.') {
                after_dot = true;
            } else if (i == 'e') {
                after_dot = false;
                after_e = true;
            } else if (after_e) {
                has_exp = true;
                exp_oss << i;
            } else {
                mantissa_oss << i;
                if (after_dot) {
                    num_after_dot++;
                }
            }
        }

        mantissa = std::stoll(mantissa_oss.str());
        sign = (mantissa >= 0) ? 1 : 0;
        mantissa = (mantissa < 0) ? (mantissa * -1) : mantissa;

        exp = has_exp ? std::stoi(exp_oss.str()) : 0;
        exp -= num_after_dot;
    } catch (const std::exception& e) {
        return Data();
    }

    int64_t min_mantissa = (uint64_t)pow(10, 15);
    int64_t max_mantissa = (uint64_t)pow(10, 16) - 1;
    int32_t min_exp = -96;
    int32_t max_exp = 80;

    while ((mantissa < min_mantissa) && (exp > min_exp)) {
        mantissa *= 10;
        exp--;
    }

    while (mantissa > max_mantissa) {
        if (exp >= max_exp) {
            return Data();
        }

        mantissa /= 10;
        exp++;
    }

    if (((exp < min_exp) || (mantissa < min_mantissa)) ||
        ((exp > max_exp) || (mantissa > max_mantissa))) {
        return Data();
    }

    typedef union {
        uint64_t value;
        struct {
            uint64_t mantissa : 54;
            uint64_t exp : 8;
            uint64_t sign : 1;
            uint64_t not_xrp : 1;
        } parts;
    } amt_cast;

    amt_cast ac;
    ac.parts.mantissa = mantissa;
    ac.parts.exp = exp + 97;
    ac.parts.sign = sign;
    ac.parts.not_xrp = 1;

    // Serialize fields
	// https://xrpl.org/serialization.html#amount-fields
    auto data = Data();
    encode64BE(ac.value, data);

    // ISO-4217 currency code
    encode0(1, data); // type code (0x00)
    encode0(11, data); // reserved
    if (currency_amt.currency.size() == 3) {
        data.insert(data.end(), currency_amt.currency.begin(), currency_amt.currency.end());
    } else {
        encode0(3, data); // none
    }

    encode0(5, data); // reserved
    data.insert(data.end(), currency_amt.issuer.begin(), currency_amt.issuer.end());
    return data;
}

Data Transaction::serializeAddress(Address address) {
    auto data = Data(20);
    std::copy(&address.bytes[0] + 1, &address.bytes[0] + std::min(address.bytes.size(), size_t(21)), &data[0]);
    return data;
}

} // namespace TW::Ripple
