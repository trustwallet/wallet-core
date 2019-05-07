// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "TWTestUtilities.h"
#include "proto/Semux.pb.h"

#include <TrustWalletCore/TWSemuxAddress.h>
#include <TrustWalletCore/TWSemuxSigner.h>
#include <TrustWalletCore/TWSemuxProto.h>
#include <TrustWalletCore/TWHDWallet.h>

#include <gtest/gtest.h>

using namespace TW;

TEST(TWSemuxSigner, Sign) {
    auto privateKey = parse_hex("7ea3e3e2ce1e2c4e7696f09a252a1b9d58948bc942c0b42092080a896c43649f");
    auto input = Semux::Proto::SigningInput();
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_destination("0xdb7cadb25fdcdd546fb0268524107582c3f8999c");
    input.set_value((uint64_t) 123456789L);
    input.set_fee((uint64_t) 5000000L);
    input.set_nonce((uint64_t) 123L);
    input.set_timestamp((uint64_t) 1523028482000L);

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte*)inputData.data(), inputData.size());
    auto outputTWData = TWSemuxSignerSign(inputTWData);
    auto output = Semux::Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    ASSERT_EQ(
            hex(output.encoded()),
            "20e3b076d3d634b9c88b4b2ab281ffd7c440e0eeccf157f2a7cc09c3b7885958c738000114db7cadb25fdcdd546fb0268524107582c3f8999c00000000075bcd1500000000004c4b40000000000000007b000001629b9257d0006064f22068b0e9d103aaae81c099d1d59a44c7ec022550ab8dcccd28104a2a79d27c9dc9a277da765bd5bde2667af78a67a99aa33bf6e352e36546d0285526210e057f987e38f88037e8019cbb774dda106fc051fc4a6320a00294fe1866d08442"
            );

}

TEST(TWSemuxAddress, HDWallet) {
    auto mnemonic = "shoot island position soft burden budget tooth cruel issue economy destroy above";
    auto passphrase = "";

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
            STRING(mnemonic).get(),
            STRING(passphrase).get()
    ));

    auto privateKey = TWHDWalletGetKey(wallet.get(), TWCoinTypeDerivationPath(TWCoinTypeSemux));
    auto publicKey = TWPrivateKeyGetPublicKeyEd25519(privateKey);
    auto address = TWSemuxAddressCreateWithPublicKey(publicKey);
    auto addressStr = WRAPS(TWSemuxAddressDescription(address));

    assertStringsEqual(addressStr, "0xfe604170382452f77bc922bc19eb4b53504b09c2");
}