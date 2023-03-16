// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "XAddress.h"
#include "Data.h"
#include "../proto/Ripple.pb.h"
#include "../HexCoding.h"

namespace TW::Ripple {

// See https://github.com/XRPLF/rippled/blob/master/src/ripple/protocol/SField.h#L57-L74
enum class FieldType: int {
    int16     = 1,
    int32     = 2,
    hash256   = 5,
    amount    = 6,
    vl        = 7,
    account   = 8,
    vector256 = 19
};

// See https://github.com/XRPLF/xrpl.js/blob/main/packages/ripple-binary-codec/src/enums/definitions.json
enum class TransactionType {
    no_type = -1,
    payment = 0,
    TrustSet = 20,
    NFTokenBurn = 26,
    NFTokenCreateOffer = 27,
    NFTokenCancelOffer = 28,
    NFTokenAcceptOffer = 29
};

// See https://xrpl.org/nftokencreateoffer.html
enum class NFTokenCreateOfferFlags: int64_t {
    tfSellNFToken = 0x00000001
};

class Transaction {
    /// Float and negative amounts are not supported.
    /// See https://github.com/trezor/trezor-core/tree/master/src/apps/ripple#transactions
  public:
      struct CurrencyAmount {
        Data currency;
        Data value;
        Data issuer;
    };

    int64_t amount;
    CurrencyAmount currency_amount;
    CurrencyAmount limit_amount;
    int64_t fee;
    int64_t flags;
    int32_t sequence;
    int32_t last_ledger_sequence;
    Address account;
    Data destination;
    bool encode_tag;
    int64_t destination_tag;
    Data pub_key;
    Data signature;
    Data nftoken_id;
    Data sell_offer;
    Data token_offers;
    TransactionType transaction_type;

    Transaction(int64_t fee, int64_t flags, int32_t sequence, int32_t last_ledger_sequence, Address p_account)
        : amount(0)
        , fee(fee)
        , flags(flags)
        , sequence(sequence)
        , last_ledger_sequence(last_ledger_sequence)
        , account(p_account)
        , encode_tag(false)
        , destination_tag(0)
        , nftoken_id(0)
        , sell_offer(0)
        , token_offers(0)
        , transaction_type(TransactionType::no_type) {}

    void createXrpPayment(int64_t p_amount, const std::string& p_destination, int64_t p_destination_tag) {
        transaction_type = TransactionType::payment;
        amount = p_amount;
        setDestination(p_destination, p_destination_tag);
    }

    void createTrustSet(const std::string& currency, const std::string& issuer) { 
        // Use maximum amount
        // https://xrpl.org/currency-formats.html
        std::string value("9999999999999999e80");
        createTrustSet(currency, value, issuer);
    }

    void createTrustSet(const std::string& currency, const std::string& value, const std::string& issuer) {
        transaction_type = TransactionType::TrustSet;
        setCurrencyAmount(limit_amount, currency, value, issuer);
    }

    void createTokenPayment(const std::string& currency, const std::string& value, const std::string& issuer,
                            const std::string& p_destination, int64_t p_destination_tag) {
        transaction_type = TransactionType::payment;
        setDestination(p_destination, p_destination_tag);
        setCurrencyAmount(currency_amount, currency, value, issuer);
    }

    void createNFTokenBurn(const std::string& p_nftoken_id) {
        transaction_type = TransactionType::NFTokenBurn;
        nftoken_id = parse_hex(p_nftoken_id);
    }

    void createNFTokenCreateOffer(const std::string& p_nftoken_id, const std::string& p_destination) {
        transaction_type = TransactionType::NFTokenCreateOffer;
        flags = int64_t(NFTokenCreateOfferFlags::tfSellNFToken);
        nftoken_id = parse_hex(p_nftoken_id);
        setAccount(p_destination, destination);
    }

    void createNFTokenAcceptOffer(const std::string& p_sell_offer) {
        transaction_type = TransactionType::NFTokenAcceptOffer;
        sell_offer = parse_hex(p_sell_offer);
    }

    void createNFTokenCancelOffer(const std::vector<std::string> p_token_offers) {
        transaction_type = TransactionType::NFTokenCancelOffer;
        for (auto i : p_token_offers) {
            Data token_offer = parse_hex(i);
            token_offers.insert(token_offers.end(), token_offer.begin(), token_offer.end());
        }
    }

  public:
    /// simplified serialization format tailored for Payment transaction type
    /// exclusively.
    Data serialize() const;
    Data getPreImage() const;

    static Data serializeAmount(int64_t amount);
    static Data serializeCurrencyAmount(const CurrencyAmount& currency_amount);
    static Data serializeAddress(Address address);

  private:
    void setCurrencyAmount(CurrencyAmount& p_currency_amount, const std::string& currency, const std::string& value, const std::string& issuer) {
        p_currency_amount.currency = Data(currency.begin(), currency.end());
        p_currency_amount.value = Data(value.begin(), value.end());
        setAccount(issuer, p_currency_amount.issuer);
    }

    void setDestination(const std::string& p_destination, int64_t p_destination_tag) {
        try {
            auto address = Address(p_destination);
            encode_tag = p_destination_tag > 0;
            destination_tag = p_destination_tag;
            destination = Data(address.bytes.begin() + 1, address.bytes.end());
        } catch(const std::exception& e) {
            auto xAddress = XAddress(p_destination);
            encode_tag = xAddress.flag != TagFlag::none;
            destination_tag = xAddress.tag;
            destination = Data(xAddress.bytes.begin(), xAddress.bytes.end());
        }
    }

    void setAccount(const std::string& p_account, Data& data) {
        try {
            auto address = Address(p_account);
            data = Data(address.bytes.begin() + 1, address.bytes.end());
        } catch(const std::exception& e) {
            auto xAddress = XAddress(p_account);
            data = Data(xAddress.bytes.begin(), xAddress.bytes.end());
        }
    }
};

} // namespace TW::Ripple
