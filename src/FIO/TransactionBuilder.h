// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "Address.h"
#include "../Data.h"
#include "../PrivateKey.h"

#include <string>
#include <vector>

namespace TW::FIO {

/// Represents current parameters of the FIO blockchain
/// @chanId: constant chainId (32 bytes), obtained from get_info API
/// @headBlockNumber: the last block number, obtained from get_info API
/// @refBlockPrefix: Block prefix of last block, obtained from get_block API
class ChainParams {
public:
    Data chainId;
    uint64_t headBlockNumber;
    uint32_t refBlockPrefix;
};

class TransactionBuilder {
public:
    static constexpr auto DummyWalletFioName = "rewards@wallet";
    static constexpr auto WalletFioName = DummyWalletFioName;
    static constexpr auto ApiAccountAddress = "fio.address";
    static constexpr auto AuthrizationActive = "active";
    static const auto ExpirySeconds = 3600;
 
    /// Create a signed AddAddress transaction, returned as json string (double quote delimited), suitable for add_pub_address RPC call
    /// @address The owners' FIO address
    /// @privateKey The private key matching the address, needed for signing.
    /// @fioName The FIO name already registered to the owner. Ex.: "alice:trust"
    /// @addressess List of public addresses to be registered, ex. {{"BTC", "bc1qv...7v"},{"BNB", "bnb1ts3...9s"}}
    /// @chainParams Current parameters from the FIO chain, must be obtained recently using get_info and get_block calls.
    /// @walletFioName The FIO name of the originating wallet, use TransactionBuilder::WalletFioName.
    /// @expiryTime Expiry for this message, can be 0, then it is taken from current time with default expiry
    /// Note: fee is 0 for add_pub_address.
    static std::string createTx(const Address& address, const PrivateKey& privateKey, const std::string& fioName,
        const std::vector<std::pair<std::string, std::string>>& pubAddresses,
        const ChainParams& chainParams, const std::string& walletFioName, uint32_t expiryTime);
};

} // namespace TW::FIO
