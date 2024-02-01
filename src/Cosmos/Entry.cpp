// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"
#include "Address.h"

#include <proto/Cosmos.pb.h>
#include <proto/TransactionCompiler.pb.h>
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace std;

namespace TW::Cosmos {

string Entry::signJSON(TWCoinType coin, const std::string& json, const Data& key) const {
    return signJSONHelper<Proto::SigningInput, Proto::SigningOutput>(
        coin,
        json,
        key,
        [](const Proto::SigningOutput& output) { return output.json(); }
    );
}

} // namespace TW::Cosmos
