// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Everscale/Messages.h"
#include "Everscale/Signer.h"

#include "Base64.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Everscale {

TEST(EverscaleSigner, TransferWithDeploy) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_behavior(Proto::MessageBehavior::SimpleTransfer);
    transfer.set_amount(500000000);
    transfer.set_expired_at(1680770631);
    transfer.set_to("0:db18a67f4626f15ac0537a18445937f685f9b30184f0d7b28be4bdeb92d2fd90");

    // NOTE: There is `set_encoded_contract_data` because contract was not deployed yet

    auto privateKey = parse_hex("542bd4288352f1c6b270046f153d406aec054a0a06000ab9b36b5c6dd3050ad4");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(Cell::fromBase64(output.encoded())->hash), "bfb18e56e9d00d783c7eb1726f08bf613dd0f01a110a130c0f8f91bb13390a39");

    // Link to the message: https://everscan.io/messages/bfb18e56e9d00d783c7eb1726f08bf613dd0f01a110a130c0f8f91bb13390a39
    ASSERT_EQ(output.encoded(), "te6ccgICAAQAAQAAAUoAAAPhiAG+Ilaz1wTyTEauoymMGl6o+NGqhszIlHS8BXAmXniYrBGMBTen55/RbfcIBoeCrPB1cxPMcHRx7xyBzJmdtewBPaTu/WuHgnqg09jQaxTEcii+Nuqm7p3b6iMq+/6598ggCXUlsUyF0MjgAAAAAHAAAwACAAEAaEIAbYxTP6MTeK1gKb0MIiyb+0L82YDCeGvZRfJe9clpfsgg7msoAAAAAAAAAAAAAAAAAAAAUAAAAABLqS2KOWKN+7Y5OSiKhKisiw6t/h2ovvR3WbapyAtrdctwupwA3v8AIN0gggFMl7ohggEznLqxn3Gw7UTQ0x/THzHXC//jBOCk8mCDCNcYINMf0x/TH/gjE7vyY+1E0NMf0x/T/9FRMrryoVFEuvKiBPkBVBBV+RDyo/gAkyDXSpbTB9QC+wDo0QGkyMsfyx/L/8ntVA==");
}

TEST(EverscaleSigner, Transfer1) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_behavior(Proto::MessageBehavior::SimpleTransfer);
    transfer.set_amount(100000000);
    transfer.set_expired_at(1680770631);
    transfer.set_to("0:db18a67f4626f15ac0537a18445937f685f9b30184f0d7b28be4bdeb92d2fd90");

    transfer.set_encoded_contract_data("te6ccgEBAQEAKgAAUAAAAAFLqS2KOWKN+7Y5OSiKhKisiw6t/h2ovvR3WbapyAtrdctwupw=");

    auto privateKey = parse_hex("542bd4288352f1c6b270046f153d406aec054a0a06000ab9b36b5c6dd3050ad4");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(Cell::fromBase64(output.encoded())->hash), "73807b0a3ca2d8564c023dccd5b9da222a270f68338c6fc2c064dda376a2c59d");

    // Link to the message: https://everscan.io/messages/73807b0a3ca2d8564c023dccd5b9da222a270f68338c6fc2c064dda376a2c59d
    ASSERT_EQ(output.encoded(), "te6ccgICAAIAAQAAAKoAAAHfiAG+Ilaz1wTyTEauoymMGl6o+NGqhszIlHS8BXAmXniYrAImASIQKH2jIwoA65IGC6aua4gAA4fFo/Nuxgb3sIRELhZnSXIS7IsE2E4D+8hk3EWGVZX+ICqlN/ka9DvXduhaXUlsUyF0MjgAAAAIHAABAGhCAG2MUz+jE3itYCm9DCIsm/tC/NmAwnhr2UXyXvXJaX7IIC+vCAAAAAAAAAAAAAAAAAAA");
}

TEST(EverscaleSigner, Transfer2) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(true);
    transfer.set_behavior(Proto::MessageBehavior::SendAllBalance);
    transfer.set_amount(200000000);
    transfer.set_expired_at(1680770631);
    transfer.set_to("0:df112b59eb82792623575194c60d2f547c68d54366644a3a5e02b8132f3c4c56");

    transfer.set_encoded_contract_data("te6ccgEBAQEAKgAAUAAAAAJLqS2KOWKN+7Y5OSiKhKisiw6t/h2ovvR3WbapyAtrdctwupw=");

    auto privateKey = parse_hex("542bd4288352f1c6b270046f153d406aec054a0a06000ab9b36b5c6dd3050ad4");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(Cell::fromBase64(output.encoded())->hash), "e35616cfa88e115580f07c6b41ae3ded1902d2bab1efefb74f677b4aececef24");

    // Link to the message: https://everscan.io/messages/e35616cfa88e115580f07c6b41ae3ded1902d2bab1efefb74f677b4aececef24
    ASSERT_EQ(output.encoded(), "te6ccgICAAIAAQAAAKoAAAHfiAG+Ilaz1wTyTEauoymMGl6o+NGqhszIlHS8BXAmXniYrANrT0ivIEpuMGjKoyS9J03Wbl24jowXvdzQdLD6L3USLETUyRGbbmbUfBcNtF1FwKtmIQd0lNR1qIX9K/eloMgaXUlsUyF0MjgAAAAUFAABAGhiAG+Ilaz1wTyTEauoymMGl6o+NGqhszIlHS8BXAmXniYrIF9eEAAAAAAAAAAAAAAAAAAA");
}

} // namespace TW::Everscale
