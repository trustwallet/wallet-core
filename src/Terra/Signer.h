

#pragma once

#include "../proto/Terra.pb.h"
#include "../Data.h"
#include <nlohmann/json.hpp>
#include <stdint.h>


namespace TW::TERRA{
    // Helper class that performs Terra transaction signing.
    class Signer {
    public:
        Terra::Proto::SigningInput input;

        /// Initializes a transaction signer.
        Signer(Terra::Proto::SigningInput&& input) : input(input) {}

        /// Signs the transaction.
        /// \returns the transaction signature or an empty vector if there is an error.
        std::vector<uint8_t> sign() const;

        /// Builds the signed transaction.
        /// \returns the signed transaction.
        Terra::Proto::SigningOutput build() const;

    private:
        std::string signaturePreimage() const;
        nlohmann::json buildTransactionJSON(const Data& signature) const;
    };


} //namespace TW::TERRA

/// Wrapper for C interface.
struct TWTerraSigner {
    TW::TERRA::Signer impl;
};