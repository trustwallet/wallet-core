// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "InternetComputer/Signer.h"
#include "InternetComputer/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TestUtilities.h"
#include "TransactionCompiler.h"

#include "proto/InternetComputer.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

using namespace TW;

namespace TW::InternetComputer {

TEST(InternetComputerCompiler, CompileWithSignatures) {
    // TODO: Finalize test implementation
}

} // namespace TW::InternetComputer
