// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
    EscrowCreate = 1,
    EscrowFinish = 2,
    EscrowCancel = 4,
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
    int64_t cancel_after;
    int64_t finish_after;
    Data owner;
    int32_t offer_sequence;
    Data condition;
    Data fulfillment;
    Data nftoken_id;
    Data sell_offer;
    Data token_offers;
    TransactionType transaction_type;

    Transaction(int64_t p_fee, int64_t p_flags, int32_t p_sequence, int32_t p_last_ledger_sequence, Address p_account)
        : amount(0)
        , fee(p_fee)
        , flags(p_flags)
        , sequence(p_sequence)
        , last_ledger_sequence(p_last_ledger_sequence)
        , account(p_account)
        , encode_tag(false)
        , destination_tag(0)
        , cancel_after(0)
        , finish_after(0)
        , offer_sequence(0)
        , nftoken_id(0)
        , sell_offer(0)
        , token_offers(0)
        , transaction_type(TransactionType::no_type) {}

    void createXrpPayment(int64_t p_amount, const std::string& p_destination, int64_t p_destination_tag) {
        transaction_type = TransactionType::payment;
        amount = p_amount;
        setDestination(p_destination, p_destination_tag);
    }

    void createTrustSet(const std::string& p_currency, const std::string& p_issuer) { 
        // Use maximum amount
        // https://xrpl.org/currency-formats.html
        std::string value("9999999999999999e80");
        createTrustSet(p_currency, value, p_issuer);
    }

    void createTrustSet(const std::string& p_currency, const std::string& p_value, const std::string& p_issuer) {
        transaction_type = TransactionType::TrustSet;
        setCurrencyAmount(limit_amount, p_currency, p_value, p_issuer);
    }

    void createTokenPayment(const std::string& p_currency, const std::string& p_value, const std::string& p_issuer,
                            const std::string& p_destination, int64_t p_destination_tag) {
        transaction_type = TransactionType::payment;
        setDestination(p_destination, p_destination_tag);
        setCurrencyAmount(currency_amount, p_currency, p_value, p_issuer);
    }

    void createEscrowCreate(int64_t p_amount, const std::string& p_destination, int64_t p_destination_tag,
                            int64_t p_cancel_after, int64_t p_finish_after, const std::string& p_condition) {
        transaction_type = TransactionType::EscrowCreate;
        if (p_cancel_after == 0 && p_finish_after == 0) {
            throw std::invalid_argument("Either CancelAfter or FinishAfter must be specified");
        } else if (p_finish_after == 0 && p_condition.empty()) {
            throw std::invalid_argument("Either Condition or FinishAfter must be specified");
        }
        this->amount = p_amount;
        setDestination(p_destination, p_destination_tag);
        this->cancel_after = p_cancel_after;
        this->finish_after = p_finish_after;
        this->condition = parse_hex(p_condition);
    }

    void createEscrowCancel(const std::string& p_owner, int32_t p_offer_sequence) {
        transaction_type = TransactionType::EscrowCancel;
        setAccount(p_owner, this->owner);
         this->offer_sequence = p_offer_sequence;
    }

    void createEscrowFinish(const std::string& p_owner, int32_t p_offer_sequence,
                            const std::string& p_condition, const std::string& p_fulfillment) {
        transaction_type = TransactionType::EscrowFinish;
        setAccount(p_owner, this->owner);
        this->offer_sequence = p_offer_sequence;
        this->condition = parse_hex(p_condition);
        this->fulfillment = parse_hex(p_fulfillment);
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

    void createNFTokenCancelOffer(const std::vector<std::string>& p_token_offers) {
        transaction_type = TransactionType::NFTokenCancelOffer;
        for (const auto& i : p_token_offers) {
            Data token_offer = parse_hex(i);
            token_offers.insert(token_offers.end(), token_offer.begin(), token_offer.end());
        }
    }

  public:
    /// simplified serialization format tailored for Payment transaction type
    /// exclusively.
    Data serialize() const;
    Data getPreImage() const;

    static Data serializeAmount(int64_t p_amount);
    static Data serializeCurrencyAmount(const CurrencyAmount& currency_amount);
    static Data serializeAddress(Address address);

  private:
    void setCurrencyAmount(CurrencyAmount& p_currency_amount, const std::string& p_currency, const std::string& p_value, const std::string& p_issuer) {
        p_currency_amount.currency = Data(p_currency.begin(), p_currency.end());
        p_currency_amount.value = Data(p_value.begin(), p_value.end());
        setAccount(p_issuer, p_currency_amount.issuer);
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

    static void setAccount(const std::string& p_account, Data& p_data) {
        try {
            auto address = Address(p_account);
            p_data = Data(address.bytes.begin() + 1, address.bytes.end());
        } catch(const std::exception& e) {
            auto xAddress = XAddress(p_account);
            p_data = Data(xAddress.bytes.begin(), xAddress.bytes.end());
        }
    }
};

} // namespace TW::Ripple
