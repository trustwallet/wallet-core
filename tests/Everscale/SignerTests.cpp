// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Everscale/Address.h"
#include "Everscale/Messages.h"
#include "Everscale/Signer.h"

#include "Base64.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Everscale {

TEST(EverscaleSigner, Deploy) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_flags(3);
    transfer.set_amount(500000000);
    transfer.set_expired_at(1660261731);

    auto privateKey = parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(output.encoded(), "te6ccgICAAQAAQAAAUoAAAPhiABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYBGTp6HO3zHcMkuRWEKShVeKcgHJt5kYQQy+Qn296nFFjD0XqbeVvMtTL3N7ud7Ad8aFTFWSgEKVN2n4NzfUBTugCXUlsUxesqxgAAAAAHAAAwACAAEAaEIAE0/3EhdaCDw1Xwu70KPC5F31jxoIBjq4cLtMWbVrN9gg7msoAAAAAAAAAAAAAAAAAAAAUAAAAABLqS2K5JJfmTLfjX/QBC7/8+IXipcgKLZE3tOjtm9tBXf4LngA3v8AIN0gggFMl7ohggEznLqxn3Gw7UTQ0x/THzHXC//jBOCk8mCDCNcYINMf0x/TH/gjE7vyY+1E0NMf0x/T/9FRMrryoVFEuvKiBPkBVBBV+RDyo/gAkyDXSpbTB9QC+wDo0QGkyMsfyx/L/8ntVA==");
}

TEST(EverscaleSigner, Transfer1) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_flags(3);
    transfer.set_amount(1000000000);
    transfer.set_expired_at(1659026078);

    auto dst = Address("0:6dc497fb5cdb013fb1b807d8a2dbf7653d3e112cdb6ca95054bd2c74408d557d");
    transfer.set_address(dst.string().c_str(), dst.string().size());

    auto stateInit = TW::Base64::decode("te6ccgEBAQEAKgAAUAAAAi9LqS2K8drjTwv63I+aPTBLtMULU+zuEMSAmO8j5A00qizUXzU=");
    transfer.set_state_init(stateInit.data(), stateInit.size());

    auto privateKey = parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(output.encoded(), "te6ccgICAAIAAQAAAKoAAAHfiABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYAC+o+gbLqcs2fFi5y5dyYccszRvGc4IBmWqCU/mWGub29BSl+RG2pv6zkYPUVwox/ExjTZfBXR9U+vUBKQ1VgBSXUlsUxcV1PAAABF4HAABAGhCADbiS/2ubYCf2NwD7FFt+7KenwiWbbZUqCpeljogRqq+odzWUAAAAAAAAAAAAAAAAAAA");
}

TEST(EverscaleSigner, Transfer2) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_flags(3);
    transfer.set_amount(1000000000);
    transfer.set_expired_at(1659026078);

    auto dst = Address("0:ab91c8cec44aa6e3b2c31443202a63241394491f0e41215a30620b57ef28b69b");
    transfer.set_address(dst.string().c_str(), dst.string().size());

    auto stateInit = TW::Base64::decode("te6ccgEBAQEAKgAAUAAAATZLqS2Kc7K5TorVBJDDavbyxCCgDW/rqoGMkiZpeu4aVpmmWvE=");
    transfer.set_state_init(stateInit.data(), stateInit.size());

    auto privateKey = parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);
    auto boc = output.encoded();

    ASSERT_EQ(output.encoded(), "te6ccgICAAIAAQAAAKoAAAHfiABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYAFRS7u+U3g33tkvU5mOIulqQY3bE292gaWV7+cPYFxLXr/KwApb9WXBy73l6ZjBMneTnpTaDu9oeTDxl5QLVfgyXUlsUxcV1PAAAAmwHAABAGhCAFXI5GdiJVNx2WGKIZAVMZIJyiSPhyCQrRgxBav3lFtNodzWUAAAAAAAAAAAAAAAAAAA");
}

TEST(EverscaleSigner, Transfer3) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_flags(3);
    transfer.set_amount(800000000);
    transfer.set_expired_at(1660261731);

    auto dst = Address("0:ab91c8cec44aa6e3b2c31443202a63241394491f0e41215a30620b57ef28b69b");
    transfer.set_address(dst.string().c_str(), dst.string().size());

    auto stateInit = TW::Base64::decode("te6ccgEBAQEAKgAAUAAAAAFLqS2KMfSrLtRKmdm2qHVPEU4LUxtqAl+/Ym+ucHTFtP57Dxg=");
    transfer.set_state_init(stateInit.data(), stateInit.size());

    auto privateKey = parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(output.encoded(), "te6ccgICAAIAAQAAAKoAAAHfiABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYAE5ztlfTQRa3lf9wnyBo6oIaFqg9TK+bTYjIQQytdpDhCmzL1NlRG/F2LjxmAdhPqv6NMmc7gRkjrNO1ho645BiXUlsUxesqxgAAAAIHAABAGhCAFXI5GdiJVNx2WGKIZAVMZIJyiSPhyCQrRgxBav3lFtNoX14QAAAAAAAAAAAAAAAAAAA");
}

TEST(EverscaleSigner, Transfer4) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_flags(3);
    transfer.set_amount(500000000);
    transfer.set_expired_at(1660261731);

    auto dst = Address("0:ab91c8cec44aa6e3b2c31443202a63241394491f0e41215a30620b57ef28b69b");
    transfer.set_address(dst.string().c_str(), dst.string().size());

    auto stateInit = TW::Base64::decode("te6ccgEBAQEAKgAAUAAAAAFLqS2K5JJfmTLfjX/QBC7/8+IXipcgKLZE3tOjtm9tBXf4Lng=");
    transfer.set_state_init(stateInit.data(), stateInit.size());

    auto privateKey = parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(output.encoded(), "te6ccgICAAIAAQAAAKoAAAHfiABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYAPe+XYmEUX0LtwyLBLK0ym7oUCCv4vnH8fiX65/43djaTtUGvv9WkulWXdTzq5PqIK+TdKgl1ljb/3ommv2r+BqXUlsUxesqxgAAAAIHAABAGhCAFXI5GdiJVNx2WGKIZAVMZIJyiSPhyCQrRgxBav3lFtNoO5rKAAAAAAAAAAAAAAAAAAA");
}

} // namespace TW::Everscale
