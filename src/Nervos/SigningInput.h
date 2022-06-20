// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Cell.h"
#include "Script.h"
#include "Serialization.h"
#include "../PrivateKey.h"
#include "../proto/Nervos.pb.h"
#include "../uint256.h"

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace TW::Nervos {

/// Input for signing, info of an unsigned transaction
class SigningInput {
public:
    // Amount to send.  Transaction created will have this amount in its output,
    // except when use_max_amount is set, in that case this amount is not relevant, maximum possible
    // amount will be used (max avail less fee). If amount is equal or more than the available
    // amount, also max amount will be used.
    uint256_t amount = 0;

    // Transaction fee per byte
    uint64_t byteFee = 0;

    // Recipient's address
    std::string toAddress;

    // Change address
    std::string changeAddress;

    // Available private keys
    std::vector<PrivateKey> privateKeys;

    // Available live cells
    Cells cells;

    // If sending max amount
    bool useMaxAmount = false;

    // SUDT (Simple User Defined Token) address, none for native token
    Data sudtAddress;

public:
    SigningInput() = default;

    SigningInput(const Proto::SigningInput& signingInput) {
        auto signingInputAmount = signingInput.amount();
        auto signingInputAmountData = Data();
        signingInputAmountData.insert(signingInputAmountData.begin(), signingInputAmount.begin(),
                                      signingInputAmount.end());
        amount = Serialization::decodeUint256(signingInputAmountData);
        byteFee = signingInput.byte_fee();
        toAddress = signingInput.to_address();
        changeAddress = signingInput.change_address();
        for (auto& privateKey : signingInput.private_key()) {
            privateKeys.emplace_back(privateKey);
        }
        for (auto& cell : signingInput.cell()) {
            cells.emplace_back(cell);
        }
        useMaxAmount = signingInput.use_max_amount();
        auto& signingInputSudtAddress = signingInput.sudt_address();
        sudtAddress.insert(sudtAddress.end(), signingInputSudtAddress.begin(),
                           signingInputSudtAddress.end());
    }
};

} // namespace TW::Nervos
