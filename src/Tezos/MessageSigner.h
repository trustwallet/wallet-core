// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "PrivateKey.h"
#include "Data.h"

namespace TW::Tezos {
    class MessageSigner {
    public:
        /// implement format input as described in https://tezostaquito.io/docs/signing/
        /// \param message message to format e.g: Hello, World
        /// \param dAppUrl the app url, e.g: testUrl
        /// \return the formatted message as a string
        static std::string formatMessage(const std::string& message, const std::string& dAppUrl);

        /// implement input to payload as described in: https://tezostaquito.io/docs/signing/
        ///
        /// \param input formatted input to be turned into an hex payload
        /// \return the hexpayload of the formated input as a hex string
        static std::string inputToPayload(const std::string& input);

        /// implement signing as described in https://tezostaquito.io/docs/signing/
        /// \param privateKey the private key to sign with
        /// \param message message to sign
        /// \return base58 signed message
        static std::string signMessage(const PrivateKey& privateKey, const std::string& message);

        /// implement verification as described in https://tezostaquito.io/docs/signing/
        /// \param publicKey publickey to verify the signed message
        /// \param message message to be verified as a string
        /// \param signature signature to verify the message against
        /// \return true if the message match the signature, false otherwise
        static bool verifyMessage(const PublicKey& publicKey, const std::string& message, const std::string& signature) noexcept;;
    };
}
