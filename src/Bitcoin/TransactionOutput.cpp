// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionOutput.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"
#include "rust/Wrapper.h"

#include "../BinaryCoding.h"

namespace TW::Bitcoin {

void TransactionOutput::encode(Data& data) const {
    encode64LE(value, data);
    script.encode(data);
}

Proto::TransactionOutput buildP2wpkhScript(Amount value, const PublicKey& publicKey) {
    TW::Bitcoin::Proto::TransactionOutput out;

    Rust::CByteArrayWrapper res = TW::Rust::tw_build_p2wpkh_script(value, publicKey.bytes.data(), publicKey.bytes.size());
    auto result = res.data;
    out.ParseFromArray(result.data(), static_cast<int>(result.size()));
    return out;
}

Proto::TransactionOutput buildBRC20InscribeTransfer(const std::string& ticker, uint64_t brcAmount, Amount brcInscribeAmount, const PublicKey& publicKey) {
    TW::Bitcoin::Proto::TransactionOutput out;
    auto tickerBytes = data(ticker);
    Rust::CByteArrayWrapper res = TW::Rust::tw_build_brc20_inscribe_transfer(tickerBytes.data(), brcAmount, brcInscribeAmount, publicKey.bytes.data(), publicKey.bytes.size());
    auto result = res.data;
    out.ParseFromArray(result.data(), static_cast<int>(result.size()));
    return out;
}

} // namespace TW::Bitcoin
