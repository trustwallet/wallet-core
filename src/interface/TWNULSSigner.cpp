// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNULSSigner.h>

#include "../PrivateKey.h"
#include "../NULS/Signer.h"
#include "../proto/NULS.pb.h"

#include <sys/time.h>

int64_t getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000+tv.tv_usec/1000;
}

using namespace TW;
using namespace TW::NULS;

TW_NULS_Proto_SigningOutput TWNULSSignerSign(TW_NULS_Proto_Transaction data)
{
    Proto::Transaction tx;
    tx.ParseFromArray(TWDataBytes(data), TWDataSize(data));

    const auto signer = Signer(tx);
    const auto output = signer.sign(getCurrentTime());

    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(output.data()), output.size());
}
