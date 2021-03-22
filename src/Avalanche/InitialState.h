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
        enum FeatureExtension {SECP256k1 = 0, NFT = 1};
        FeatureExtension FxID;
        std::vector<TransactionOutput*> Outputs;
        
        InitialState(FeatureExtension fxid, std::vector<TransactionOutput*> outputs)
         : FxID(fxid) {
          Outputs = outputs;
          std::sort(Outputs.begin(), Outputs.end());
         }

        InitialState(const InitialState &other) {
          FxID = other.FxID;
          std::vector<TransactionOutput*> outputs;
          for (auto output : other.Outputs) {
            outputs.push_back(output->duplicate());
          }
          Outputs = outputs;
          std::sort(Outputs.begin(), Outputs.end());          
        }

        InitialState& operator=(const InitialState &other);

        /// Encodes the InitialState into the provided buffer.
        void encode(Data& data) const;

        bool operator<(const InitialState& other) const;

        ~InitialState();
    };
} // namespace TW::Avalanche