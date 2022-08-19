// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Lightning/LNInvoice.h"
#include "Bech32.h"
#include "BinaryCoding.h"
//#include "HexCoding.h" // TODO is this needed?

//#include <iostream> // TODO remove

namespace TW::Lightning {

bool findKnownPrefix(const std::string& prefix, LNNetwork& net, std::string& prefixPrefix) {
    static const std::pair<std::string, LNNetwork> prefixes[] = {
        {"lnbcrt", Regtest},
        {"lntbs", Signet},
        {"lntb", Testnet},
        {"lnbc", Mainnet}
    };
    for(const auto& p: prefixes) {
        if (prefix.starts_with(std::get<0>(p))) {
            prefixPrefix = std::get<0>(p);
            net = std::get<1>(p);
            return true;
        }
    }
    return false;
}

struct LNInvoice InvoiceDecoder::decodeInvoice(const std::string& invstr) {
    // https://github.com/lightning/bolts/blob/master/11-payment-encoding.md

    LNInvoice inv;

    const auto dec = Bech32::decode(invstr, 3000);

    const auto prefix = std::get<0>(dec);
    const auto& data5 = std::get<1>(dec);
    const auto checkSum = std::get<2>(dec);
    inv.intPrefix = prefix;
    inv.intDataRaw = data5;

    assert(checkSum == Bech32::Bech32);
    if (prefix.length() < 4 || prefix.substr(0, 2) != "ln") {
        throw std::invalid_argument("Invalid prefix " + prefix);
    }

    //std::cout << "PREFIX " << prefix << "\n";
    std::string prefixPrefix;
    if (!findKnownPrefix(prefix, inv.network, prefixPrefix)) {
        throw std::invalid_argument("Invalid prefix " + prefix);
    }
    const auto prefix2 = prefix.substr(prefixPrefix.length());
    //std::cout << "PREFIX2 " << prefix2 << "\n";
    if (prefix2.length() < 1) {
        throw std::invalid_argument("Invalid prefix " + prefix + " " + prefix2);
    }
    inv.unparsedAmnt = prefix2;

    size_t idx = 0;
    Data timestamp5 = TW::subData(data5, idx, 7);
    idx += 7;
    {
        Data conv;
        Bech32::convertBits<5, 8, true>(conv, timestamp5);
        assert(conv.size() == 5);
        inv.timestamp = ((uint64_t)(conv[0] & 0x1F) << (32-5)) + ((uint64_t)(TW::decode32BE(conv.data() + 1)) >> 5);
    }

    const auto sigLen = 104;
    assert(data5.size() > sigLen);
    const auto lenWoSig = data5.size() - sigLen;
    const auto signature5 = TW::subData(data5, data5.size() - sigLen, sigLen);
    Bech32::convertBits<5, 8, false>(inv.signature, signature5);

    while (idx < lenWoSig) {
        const auto tag = data5[idx++];
        //std::cout << "TAG " << (int)tag << "  " << (int)idx << " " << lenWoSig << "\n";
        if (idx + 2 >= lenWoSig) {
            break;
        }
        auto tagLen = (data5[idx] << 5) + data5[idx + 1];
        idx += 2;
        //std::cout << "TAGLEN " << (int)tagLen <<  "\n";
        if (idx + tagLen >= data5.size()) {
            break;
        }
        const auto tagValue5 = TW::subData(data5, idx, tagLen);

        Data convPad;
        Bech32::convertBits<5, 8, true>(convPad, tagValue5);
        Data convUnpad;
        Bech32::convertBits<5, 8, false>(convUnpad, tagValue5);

        //std::cout << "TAG " << (int)tag << " LEN " << (int)tagLen << " " << (int)tagValue5.size() << " VAL " << hex(tagValue5) << "  " << (int)convPad.size() << " " << (int)convUnpad.size() << " " << hex(convPad) << "\n";

        switch (tag) {
            case 1: // p payment hash
                if (tagLen == 52) {
                    inv.paymentHash = convUnpad;
                }
                break;

            case 3: // r routing
                {
                    size_t cidx = 0;
                    while (cidx < convUnpad.size()) {
                        RoutingInfo ri;
                        ri.pubkey = TW::subData(convUnpad, cidx, 33);
                        ri.shortChannelId = TW::subData(convUnpad, cidx + 33, 8);
                        ri.unparsedFeeBaseMsat = TW::subData(convUnpad, cidx + 33 + 8, 4);
                        ri.unparsedFeePPM = TW::subData(convUnpad, cidx + 33 + 8 + 4, 4); // TODO
                        ri.unparsedCltvExpiryDelta = TW::subData(convUnpad, cidx + 33 + 8 + 4 + 4, 2); // TODO
                        cidx += 51;
                        inv.routing.push_back(ri);
                    }
                }
                break;

            case 5: // 9 features
                inv.unparsedFeatures = convUnpad;
                break;

            case 6: // x expiry
                inv.unparsedExpiry = convUnpad;
                break;

            case 13: // d description
                inv.description = std::string(std::string(convUnpad.begin(), convUnpad.end()).c_str());
                break;

            case 16: // s secret
                if (tagLen == 52) {
                    inv.secret = convUnpad;
                }
                break;

            case 19: // n payee nodeID
                if (tagLen == 53) {
                    inv.nodeId = convUnpad;
                }
                break;

            case 24: // c min_final_cltv_expiry
                if (tagValue5.size() == 1) {
                    inv.minFinalCltvExpiry = tagValue5[0];
                } else if (tagValue5.size() == 2) {
                    inv.minFinalCltvExpiry = decode16BE(convPad.data()) >> 6;
                }
                //std::cout << "minFinalCltvExpiry " << inv.minFinalCltvExpiry << "\n";
                break;

            default:
                std::cout << "TAG? " << (int)tag << " LEN " << (int)tagLen << " " << (int)tagValue5.size() << "\n";
                continue;
        }
        idx += tagLen;
    }

    /**
     * r (3): data_length variable. One or more entries containing extra routing information for a private route; there may be more than one r field
        pubkey (264 bits) 33 bytes
        short_channel_id (64 bits) 8 bytes
        fee_base_msat (32 bits, big-endian) 4 bytes
        fee_proportional_millionths (32 bits, big-endian) 4 bytes
        cltv_expiry_delta (16 bits, big-endian) 2 bytes
        */

    /*
    std::cout << "INVOICE \n";
    std::cout << "network " << (int)inv.network << "\n";
    std::cout << "amnt    " << inv.unparsedAmnt << "\n";
    std::cout << "timest  " << inv.timestamp << "\n";
    std::cout << "nodeId  " << hex(inv.nodeId) << "\n";
    std::cout << "payHash " << hex(inv.paymentHash) << "\n";
    std::cout << "secret  " << hex(inv.secret) << "\n";
    std::cout << "desc    " << inv.description << "\n";
    */

    return inv;
}

std::string networkToPrefix(LNNetwork net) {
    switch (net) {
        default:
        case Mainnet: return "bc";
        case Testnet: return "tb";
        case Signet: return "tbs";
        case Regtest: return "bcrt";
    }
}

void appendTag5(Data& data5, uint8_t tag, const Data& value5) {
    data5.push_back(tag);
    const auto tagLen = value5.size();
    data5.push_back((tagLen & 0xE0) >> 5);
    data5.push_back(tagLen & 0x1F);
    append(data5, value5);
}

void appendTag(Data& data5, uint8_t tag, const Data& value, bool pad = true) {
    Data value5;
    if (pad) {
        Bech32::convertBits<8, 5, true>(value5, value);
    } else {
        Bech32::convertBits<8, 5, false>(value5, value);
    }
    appendTag5(data5, tag, value5);
}

std::string InvoiceDecoder::encodeInvoice(const LNInvoice& inv) {
    const std::string prefix = "ln" + networkToPrefix(inv.network) + inv.unparsedAmnt;

    Data data5;
    Data timestamp2;
    encode64BE(inv.timestamp << 5, timestamp2);
    //std::cout << hex(timestamp2) << "\n";
    Data timestamp5;
    Bech32::convertBits<8, 5, true>(timestamp5, subData(timestamp2, 3, 5));
    //std::cout << hex(timestamp5) << "\n";
    TW::append(data5, TW::subData(timestamp5, 0, 7));

    if (inv.paymentHash.size() == 32) {
        appendTag(data5, 1, inv.paymentHash);
    }
    if (inv.routing.size() > 0) {
        Data rd;
        for(const auto& r: inv.routing) {
            append(rd, r.pubkey);
            append(rd, r.shortChannelId);
            append(rd, r.unparsedFeeBaseMsat);
            append(rd, r.unparsedFeePPM);
            append(rd, r.unparsedCltvExpiryDelta);
        }
        appendTag(data5, 3, rd);
    }
    if (inv.unparsedFeatures.size() > 0) {
        appendTag(data5, 5, inv.unparsedFeatures);
    }
    if (inv.unparsedExpiry.size() > 0) {
        appendTag(data5, 6, inv.unparsedExpiry);
    }
    // description: always
    appendTag(data5, 13, data(inv.description));
    if (inv.secret.size() == 32) {
        appendTag(data5, 16, inv.secret);
    }
    if (inv.nodeId.size() == 33) {
        appendTag(data5, 19, inv.nodeId);
    }
    if (inv.minFinalCltvExpiry > 0) {
        if (inv.minFinalCltvExpiry < 32) {
            appendTag5(data5, 24, {(byte)(inv.minFinalCltvExpiry)});
        } else if (inv.minFinalCltvExpiry < 1024) {
            appendTag5(data5, 24, {
                (byte)(((byte)(inv.minFinalCltvExpiry) & 0xE0) >> 5),
                (byte)((byte)(inv.minFinalCltvExpiry) & 0x1F),
            });
        }
    }

    Data signature5;
    Bech32::convertBits<8, 5, true>(signature5, inv.signature);
    TW::append(data5, signature5);

    const auto enc = Bech32::encode(prefix, data5, Bech32::Bech32);
    return enc;
}

} // namespace TW::Lightning
