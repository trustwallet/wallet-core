// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PublicKey.h"

#include "Everscale/BuilderData.h"
#include "Everscale/Cell.h"
#include "Everscale/Wallet.h"

#include <gtest/gtest.h>
#include <memory>

using namespace TW;
using namespace TW::Everscale;

TEST(EverscaleCell, ComputeAddr) {
    const auto seqno = 0;
    const auto walletId = WALLET_ID;
    const auto publicKey = PublicKey(parse_hex("7dbe83e9b223157e85bed2628430e2cdb531d5c99ab428618b7dd29b567a0369"), TWPublicKeyTypeED25519);

    BuilderData builder_data;
    builder_data.appendU32(seqno);
    builder_data.appendU32(walletId);
    builder_data.appendRaw(publicKey.bytes, 256);

    const auto data = std::make_shared<Cell>(builder_data.intoCell());
    const auto code = Cell::deserialize(Wallet::code, sizeof(Wallet::code));

    BuilderData builder_state_init;
    builder_state_init.appendBitZero(); // split_depth
    builder_state_init.appendBitZero(); // special
    builder_state_init.appendBitOne();  // code
    builder_state_init.appendReferenceCell(code);
    builder_state_init.appendBitOne();  // data
    builder_state_init.appendReferenceCell(data);
    builder_state_init.appendBitZero(); // library

    auto init_state = builder_state_init.intoCell();
    auto hash = init_state.hash;

    ASSERT_EQ(hex(hash), "5a0f742c28067da91e05830f0b072a2069f0617a5f6529d295f6c517d63d67c6");
}
