// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include <TrustWalletCore/TWARKSigner.h>
#include "../ARK/Signer.h"
#include "../proto/ARK.pb.h"
#include "../ARK/Transaction.h"
#include "../ARK/Address.h"
#include "../uint256.h"
#include <boost/multiprecision/cpp_int.hpp>

using namespace TW;
using namespace TW::ARK;

TW_ARK_Proto_SigningOutput TWARKSignerSign(TW_ARK_Proto_SigningInput data){
    
    Proto::SigningInput input;
    input.ParseFromArray( TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    
    PrivateKey key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));

    Transaction tx = Transaction();
    
    uint32_t t = static_cast<uint32_t>(input.timestamp());
    tx.setTimestamp(t);
    tx.recipientId = input.recipientid();
    tx.amount = static_cast<uint64_t>(input.amount());
    
    uint64_t fee = static_cast<uint64_t>(input.fee());
    if ( fee > 0  ){
        tx.fee = static_cast<uint64_t>(input.fee());
    }
    
    Signer::sign(key, tx);

    Proto::SigningOutput output;

    output.set_json(tx.toJson());
    auto serialized = output.SerializeAsString();
    
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
