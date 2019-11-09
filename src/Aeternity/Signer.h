// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "../proto/Aeternity.pb.h"
#include <PrivateKey.h>

namespace TW::Aeternity {

class Signer {
  public:
    /// Signs the given transaction.
    static Proto::SigningOutput sign(const PrivateKey &privateKey, Transaction &transaction);

  private:
    static const uint8_t checkSumSize = 4;

    static Data buildRlpTxRaw(Data &txRaw, Data &sigRaw);

    static Data buildMessageToSign(Data &txRaw);

    static Proto::SigningOutput createProtoOutput(std::string &signature, const std::string &signedTx);

    /// Encode a byte array into base64 with prefix and a checksum
    static std::string encodeBase64WithChecksum(const std::string &prefix, const TW::Data &rawTx);
};

} // namespace TW::Aeternity

/// Wrapper for C interface.
struct TWAeternitySigner {
    TW::Aeternity::Signer impl;
};
