// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include "HexCoding.h"
#include "proto/Everscale.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Everscale {

TEST(TWAnySignerEverscale, SignMessageToDeployWallet) {
    Proto::SigningInput input;

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_flags(3);
    transfer.set_amount(500000000);
    transfer.set_expired_at(1660261731);

    auto privateKey = parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f");
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEverscale);

    ASSERT_EQ(output.encoded(), "te6ccgICAAQAAQAAAUoAAAPhiABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYBGTp6HO3zHcMkuRWEKShVeKcgHJt5kYQQy+Qn296nFFjD0XqbeVvMtTL3N7ud7Ad8aFTFWSgEKVN2n4NzfUBTugCXUlsUxesqxgAAAAAHAAAwACAAEAaEIAE0/3EhdaCDw1Xwu70KPC5F31jxoIBjq4cLtMWbVrN9gg7msoAAAAAAAAAAAAAAAAAAAAUAAAAABLqS2K5JJfmTLfjX/QBC7/8+IXipcgKLZE3tOjtm9tBXf4LngA3v8AIN0gggFMl7ohggEznLqxn3Gw7UTQ0x/THzHXC//jBOCk8mCDCNcYINMf0x/TH/gjE7vyY+1E0NMf0x/T/9FRMrryoVFEuvKiBPkBVBBV+RDyo/gAkyDXSpbTB9QC+wDo0QGkyMsfyx/L/8ntVA==");
}

} // namespace TW::Everscale
