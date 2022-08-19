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
    Data signature;
    LNNetwork network;
    std::string unparsedAmnt; // TODO change to numeric type, parse  // TODO check range
    uint64_t timestamp; // 35 bits
    Data nodeId;
    Data paymentHash;
    Data secret;
    std::string description;
    std::vector<RoutingInfo> routing;
    Data unparsedFeatures; // TODO
    Data unparsedExpiry; // TODO
    uint16_t minFinalCltvExpiry;
    // internal, redundant fields
    std::string intPrefix;
    Data intDataRaw;
};

class InvoiceDecoder {
public:
    static struct LNInvoice decodeInvoice(const std::string& invstr);
    static std::string encodeInvoice(const LNInvoice& inv);
};

} // namespace TW::Lightning
