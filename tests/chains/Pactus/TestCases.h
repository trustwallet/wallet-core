// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "proto/Pactus.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <string>

const std::string PRIVATE_KEY_HEX = "4e51f1f3721f644ac7a193be7f5e7b8c2abaa3467871daf4eacb5d3af080e5d6";

namespace TransferTransaction {
// Successfully broadcasted transaction:
// https://pacviewer.com/transaction/1b6b7226f7935a15f05371d1a1fefead585a89704ce464b7cc1d453d299d235f
static TW::Pactus::Proto::SigningInput createSigningInput() {
    TW::Pactus::Proto::SigningInput input;
    TW::Pactus::Proto::TransactionMessage* trx = input.mutable_transaction();
    trx->set_lock_time(2335524);
    trx->set_fee(10000000);
    trx->set_memo("wallet-core");

    TW::Pactus::Proto::TransferPayload* pld = trx->mutable_transfer();
    pld->set_sender("pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr");
    pld->set_receiver("pc1r0g22ufzn8qtw0742dmfglnw73e260hep0k3yra");
    pld->set_amount(200000000);

    return input;
}

const std::string transactionID = "1b6b7226f7935a15f05371d1a1fefead585a89704ce464b7cc1d453d299d235f";
const std::string signature = "4ed8fee3d8992e82660dd05bbe8608fc56ceabffdeeee61e3213b9b49d33a0fc"
                              "8dea6d79ee7ec60f66433f189ed9b3c50b2ad6fa004e26790ee736693eda8506";
const std::string dataToSign = "0124a3230080ade2040b77616c6c65742d636f726501037098338e0b6808119d"
                               "fd4457ab806b9c2059b89b037a14ae24533816e7faaa6ed28fcdde8e55a7df21"
                               "8084af5f";
const std::string signedData = "000124a3230080ade2040b77616c6c65742d636f726501037098338e0b680811"
                               "9dfd4457ab806b9c2059b89b037a14ae24533816e7faaa6ed28fcdde8e55a7df"
                               "218084af5f4ed8fee3d8992e82660dd05bbe8608fc56ceabffdeeee61e3213b9"
                               "b49d33a0fc8dea6d79ee7ec60f66433f189ed9b3c50b2ad6fa004e26790ee736"
                               "693eda850695794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560b"
                               "b72145f4fa";
} // namespace TransferTransaction

namespace BondWithPublicKeyTransaction {
// Successfully broadcasted transaction:
// https://pacviewer.com/transaction/d194b445642a04ec78ced4448696e50b733f2f0b517a23871882c0eefaf1c28f

static TW::Pactus::Proto::SigningInput createSigningInput() {
    TW::Pactus::Proto::SigningInput input;
    TW::Pactus::Proto::TransactionMessage* trx = input.mutable_transaction();
    trx->set_lock_time(2339009);
    trx->set_fee(10000000);
    trx->set_memo("wallet-core");

    TW::Pactus::Proto::BondPayload* pld = trx->mutable_bond();
    pld->set_sender("pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr");
    pld->set_receiver("pc1p9y5gmu9l002tt60wak9extgvwm69rq3a9ackrl");
    pld->set_stake(1000000000);
    pld->set_public_key("public1pnz75msstqdrq5eguvcwanug0zauhqjw2cc4flmez3qethnp68y64ehc4k69amapj7x4na2uda0snqz4yxujgx3jsse4f64fgy7jkh0xauvhrc5ts09vfk48g85t0js66hvajm6xruemsvlxqv3xvkyur8v9v0mtn");

    return input;
}

const std::string transactionID = "d194b445642a04ec78ced4448696e50b733f2f0b517a23871882c0eefaf1c28f";
const std::string signature = "0d7bc6d94927534b89e2f53bcfc9fc849e0e2982438955eda55b4338328adac7"
                              "9d4ee3216d143f0e1629764ab650734f8ba188e716d71f9eff65e39ce7006300";
const std::string dataToSign = "01c1b0230080ade2040b77616c6c65742d636f726502037098338e0b6808119d"
                               "fd4457ab806b9c2059b89b0129288df0bf7bd4b5e9eeed8b932d0c76f451823d"
                               "6098bd4dc20b03460a651c661dd9f10f17797049cac62a9fef228832bbcc3a39"
                               "355cdf15b68bddf432f1ab3eab8debe1300aa43724834650866a9d552827a56b"
                               "bcdde32e3c517079589b54e83d16f9435abb3b2de8c3e677067cc0644ccb1383"
                               "3b8094ebdc03";
const std::string signedData = "0001c1b0230080ade2040b77616c6c65742d636f726502037098338e0b680811"
                               "9dfd4457ab806b9c2059b89b0129288df0bf7bd4b5e9eeed8b932d0c76f45182"
                               "3d6098bd4dc20b03460a651c661dd9f10f17797049cac62a9fef228832bbcc3a"
                               "39355cdf15b68bddf432f1ab3eab8debe1300aa43724834650866a9d552827a5"
                               "6bbcdde32e3c517079589b54e83d16f9435abb3b2de8c3e677067cc0644ccb13"
                               "833b8094ebdc030d7bc6d94927534b89e2f53bcfc9fc849e0e2982438955eda5"
                               "5b4338328adac79d4ee3216d143f0e1629764ab650734f8ba188e716d71f9eff"
                               "65e39ce700630095794161374b22c696dabb98e93f6ca9300b22f3b904921fbf"
                               "560bb72145f4fa";
} // namespace BondWithPublicKeyTransaction

namespace BondWithoutPublicKeyTransaction {
// Successfully broadcasted transaction:
// https://pacviewer.com/transaction/f83f583a5c40adf93a90ea536a7e4b467d30ca4f308d5da52624d80c42adec80

static TW::Pactus::Proto::SigningInput createSigningInput() {
    TW::Pactus::Proto::SigningInput input;
    TW::Pactus::Proto::TransactionMessage* trx = input.mutable_transaction();
    trx->set_lock_time(2335580);
    trx->set_fee(10000000);
    trx->set_memo("wallet-core");

    TW::Pactus::Proto::BondPayload* pld = trx->mutable_bond();
    pld->set_sender("pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr");
    pld->set_receiver("pc1p6taz5l2kq5ppnxv4agnqj48svnvsy797xpe6wd");
    pld->set_stake(1000000000);

    return input;
}

const std::string transactionID = "f83f583a5c40adf93a90ea536a7e4b467d30ca4f308d5da52624d80c42adec80";
const std::string signature = "9e6279fb64067c7d7316ac74630bbb8589df268aa4548f1c7d85c087a8748ff0"
                              "715b9149afbd94c5d8ee6b37c787ec63e963cbb38be513ebc436aa58f9a8f00d";
const std::string dataToSign = "015ca3230080ade2040b77616c6c65742d636f726502037098338e0b6808119d"
                               "fd4457ab806b9c2059b89b01d2fa2a7d560502199995ea260954f064d90278be"
                               "008094ebdc03";
const std::string signedData = "00015ca3230080ade2040b77616c6c65742d636f726502037098338e0b680811"
                               "9dfd4457ab806b9c2059b89b01d2fa2a7d560502199995ea260954f064d90278"
                               "be008094ebdc039e6279fb64067c7d7316ac74630bbb8589df268aa4548f1c7d"
                               "85c087a8748ff0715b9149afbd94c5d8ee6b37c787ec63e963cbb38be513ebc4"
                               "36aa58f9a8f00d95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf"
                               "560bb72145f4fa";

} // namespace BondWithoutPublicKeyTransaction

struct TestCase {
    std::function<TW::Pactus::Proto::SigningInput()> createSigningInput;
    std::string transactionID;
    std::string signature;
    std::string dataToSign;
    std::string signedData;
};

const TestCase TEST_CASES[] = {
    {
        TransferTransaction::createSigningInput,
        TransferTransaction::transactionID,
        TransferTransaction::signature,
        TransferTransaction::dataToSign,
        TransferTransaction::signedData,
    },
    {
        BondWithPublicKeyTransaction::createSigningInput,
        BondWithPublicKeyTransaction::transactionID,
        BondWithPublicKeyTransaction::signature,
        BondWithPublicKeyTransaction::dataToSign,
        BondWithPublicKeyTransaction::signedData,
    },
    {
        BondWithoutPublicKeyTransaction::createSigningInput,
        BondWithoutPublicKeyTransaction::transactionID,
        BondWithoutPublicKeyTransaction::signature,
        BondWithoutPublicKeyTransaction::dataToSign,
        BondWithoutPublicKeyTransaction::signedData,
    },
};