// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include "HexCoding.h"
#include "proto/Everscale.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Everscale {

TEST(TWAnySignerEverscale, SignMessageToDeployWallet) {
    Proto::SigningInput input;

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_behavior(Proto::MessageBehavior::SimpleTransfer);
    transfer.set_amount(500000000);
    transfer.set_expired_at(1680770631);
    transfer.set_to("0:db18a67f4626f15ac0537a18445937f685f9b30184f0d7b28be4bdeb92d2fd90");

    // NOTE: There is `set_encoded_contract_data` because contract was not deployed yet

    auto privateKey = parse_hex("542bd4288352f1c6b270046f153d406aec054a0a06000ab9b36b5c6dd3050ad4");
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEverscale);

    ASSERT_EQ(output.encoded(), "te6ccgICAAQAAQAAAUoAAAPhiAG+Ilaz1wTyTEauoymMGl6o+NGqhszIlHS8BXAmXniYrBGMBTen55/RbfcIBoeCrPB1cxPMcHRx7xyBzJmdtewBPaTu/WuHgnqg09jQaxTEcii+Nuqm7p3b6iMq+/6598ggCXUlsUyF0MjgAAAAAHAAAwACAAEAaEIAbYxTP6MTeK1gKb0MIiyb+0L82YDCeGvZRfJe9clpfsgg7msoAAAAAAAAAAAAAAAAAAAAUAAAAABLqS2KOWKN+7Y5OSiKhKisiw6t/h2ovvR3WbapyAtrdctwupwA3v8AIN0gggFMl7ohggEznLqxn3Gw7UTQ0x/THzHXC//jBOCk8mCDCNcYINMf0x/TH/gjE7vyY+1E0NMf0x/T/9FRMrryoVFEuvKiBPkBVBBV+RDyo/gAkyDXSpbTB9QC+wDo0QGkyMsfyx/L/8ntVA==");
}

} // namespace TW::Everscale
