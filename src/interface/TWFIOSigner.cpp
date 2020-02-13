// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWFIOSigner.h>

#include "../FIO/TransactionBuilder.h"

#include "../Data.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"
#include "../proto/FIO.pb.h"

#include <string>

using namespace TW;
using namespace TW::FIO;
using namespace std;


TW_FIO_Proto_SigningOutput TWFIOSignerSign(TW_FIO_Proto_SigningInput input) {
    FIO::Proto::SigningOutput out;
    try
    {    
        FIO::Proto::SigningInput in;
        if (!in.ParseFromArray(TWDataBytes(input), static_cast<int>(TWDataSize(input)))) {
            out.set_error("Error: could not parse input");
        } else {
            const string json = TransactionBuilder::sign(in);
            out.set_json(json);
        }
    }
    catch(const std::exception& e)
    {
        out.set_error("Internal error");
    }
    auto serialized = out.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()), serialized.size());
}
