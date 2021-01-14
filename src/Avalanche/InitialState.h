// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TransferableOutput.h"

namespace TW::Avalanche {

    class InitialState {
      public: 
        uint32_t FxID;
        std::vector<TransferableOutput> Outputs;

        enum FeatureExtension {SECP256k1 = 0, NFT = 1};
        
        InitialState(FeatureExtension fxid, std::vector<TransferableOutput> &outputs)
         : FxID(fxid), Outputs(outputs) {}

        /// Encodes the input into the provided buffer.
        void encode(Data& data) const;
    };

} // namespace TW::Avalanche