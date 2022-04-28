// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Arweave/Random.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW::Arweave;
using namespace TW;

const auto Seed1 = "7d1fda7b2223009190fe30c9701ecf89feb0c775a2e4e3fcc19435e316c672075f807160ca538e8e0a62fd937c062d0ad5c02ab1e8a3b60fe70ab65cfd991576"; // 125  31 218 ...
const auto Random1 = "f5e1bb6bc4b9e265ce1dbd98ba928529f1426bbf4cb5dca03c5fcc23f5136842b985ff0f2dc9d7aa8152bfea346c11476212a6355073558dc51891edd5d9f9a23ef1035182c91f1b8cb1fd40663e27e78a8b3bf24a687fd50034afb164b430595f58b6da27164f4619786493e56ac1be42178b31317dbb0f42f8af325f0a0802906a449d1a63db8348fa1dc0aee3d4a22dcb5d841fe54ca969eea70abbca373889b5ccc98ce8d6fbbb3860a74c8363230718b7066575cc4b0e407025dddfe6a06bb7c70b1151588b32b4ff8a9e5228f050a402cf44688f4976d6a035223252a39f89a56711dcedc03baed24a6b09b6e540b766c5bc1c6188b57048cd3b92fa5546"; // 245 225 187 ...


TEST(ArweaveRandom, Generate1) {
    const auto len = 257;

    RandomGenerator rng = RandomGenerator(parse_hex(Seed1));
    const auto random1 = rng.nextBytes(len);
    EXPECT_EQ(random1.size(), len);
    EXPECT_EQ(hex(random1), Random1);

    const auto random2 = rng.nextBytes(len);
    EXPECT_EQ(hex(random2), "c2c84bc64b2be367b80ccd4a0c81a72c0d7e35534be5072529bd33a38b69b1fe0a133ade9daf0147ea2f50526525519fb3e8c7e77a7402627d1ac836ab705d00f85dabe8ddc91b2b248907f858ae782c53b6c44333e392f109ffdc188e9da17fbf2cebf7d13472a9d5ad73c46912258500f30735bbbffcefc5e2410d88898b7ba8436751fbf0c49b46d62d82990ad11378d5d59e9efee929d60c7bd26be33a10e38acaaca4b2e3f3c7efb354c92efc0a7ef553132e9bed6edbdbe22dd3aa70f0cd6d68873106a1b505a35c6f1d61afe14b62fe31cbfccca58a0b89e4dd1de4aad45ed91f4ce4dbfdc73db04ad622b2649665532bb22d37920d0c80737cbe35e21a");
}
