// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include "../Data.h"
#include "Address.h"

namespace TW::ARK {

  class Transaction {
    public:
      
      uint8_t type;
      Data publicKey;
      Address recipientId;
      uint64_t amount;
      uint64_t fee;    

      /// Transaction signature.
      Data signature;

      Transaction();

      /// Encodes the transaction.
      Data encoded();
      std::string toJson();
      void setTimestamp(uint32_t &timestamp);
      uint32_t getTimestamp();
    
    private:
      uint32_t timestamp;
  };
}