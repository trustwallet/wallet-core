// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWJsonSigner.h>
#include <string>

#include "../proto/Json.pb.h"

#include "Data.h"
#include "Coin.h"

using namespace TW;
using namespace TW::Json;
using namespace TW::Json::Proto;
using namespace google::protobuf::util;


TW_Json_Proto_SigningOutput TWJsonSignerSign(enum TWCoinType coin, TWString *_Nonnull transaction, TWString *_Nonnull privateKey)
{

}
