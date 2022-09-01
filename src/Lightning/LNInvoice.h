// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

namespace TW::Lightning {

struct RoutingInfo {
    Data pubkey;
    Data shortChannelId;
    Data unparsedFeeBaseMsat; // TODO uint32_t feeBaseMsat;
    Data unparsedFeePPM; // TODO uint32_t feePPM;
    Data unparsedCltvExpiryDelta; // TODO uint16_t cltvExpiryDelta;
};

enum LNNetwork {
    Mainnet = 1,
    Testnet,
    Signet,
    Regtest,
};

struct LNInvoice {
    LNNetwork network;
    bool amountPresent;
    std::string unparsedAmount; // TODO change to numeric type, parse  // TODO check range
    uint64_t timestamp; // 35 bits
    Data signature;
    /// Set to true if there is a mismatch between the nodeId (public key) and signature
    bool mismatchNodeidSignature;
    Data nodeId;
    Data paymentHash;
    Data secret;
    std::string description;
    std::vector<RoutingInfo> routing;
    Data unparsedFeatures; // TODO
    Data unparsedExpiry; // TODO
    uint16_t minFinalCltvExpiry;
};

/// Lightning Invoice decoding and encoding.
/// Specs: https://github.com/lightning/bolts/blob/master/11-payment-encoding.md
class InvoiceDecoder {
public:
    static struct LNInvoice decodeInvoice(const std::string& invstr);
    /// Encode an invoice, from the parts provided.
    /// If optional private key is provided, signature is recreated (and provided one is ignored). Otherwise the provided signature is used.
    static std::string encodeInvoice(const LNInvoice& inv, const Data& optionalPrivateKey = Data());
    /// Verify signature inside the invoice. Normally no public key is provided, but the included nodeId public key is used for verification.
    /// If public key is provided, it is used for verification.
    static bool verifySignature(const std::string& invstr, const Data& extPublicKey = Data());
    // Create signature of the invoice data, using private key
    static Data buildSignature(const std::string& invstr, const Data& privateKey);

private:
    static void decodeInternal(const std::string& invstr, std::string& prefix, LNNetwork& net, std::string& prefixPrefix, Data& dataRaw5bit, Data& data5bit, Data& data8bit, Data& signature);
    // build the prefix
    static std::string buildPrefix(const LNInvoice& inv);
    // build up the data up to the signature
    static Data buildUptosig(const LNInvoice& inv);
};

} // namespace TW::Lightning
