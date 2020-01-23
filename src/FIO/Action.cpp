// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Action.h"
#include "EOS/Name.h" // Name is reused
#include "../Data.h"
#include "../BinaryCoding.h"

namespace TW::FIO {

void Action::serialize(Data& out) const {
    EOS::Name(account).serialize(out);
    EOS::Name(name).serialize(out);
    auth.serialize(out);
    encodeVarInt(actionDataSer.size(), out);
    append(out, 1); // 01
    append(out, actionDataSer);
    append(out, 0); // 00
}

void AddAddressData::serialize(Data& out) const {
    encodeString(fioAddress, out);
    addresses.serialize(out);
    encode64LE(maxFee, out);
    EOS::Name(actor).serialize(out);
    encodeString(tpid, out);
}

} // namespace TW::FIO
