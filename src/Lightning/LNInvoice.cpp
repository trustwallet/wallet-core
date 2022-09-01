// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Lightning/LNInvoice.h"
#include "Bech32.h"
#include "BinaryCoding.h"
#include "Hash.h"
#include "PrivateKey.h"
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

void InvoiceDecoder::decodeInternal(const std::string& invstr, std::string& prefix, LNNetwork& net, std::string& prefixPrefix, Data& dataRaw5bit, Data& data5bit, Data& data8bit, Data& signature) {
    const auto bech32decoded = Bech32::decode(invstr, 3000);

    prefix = std::get<0>(bech32decoded);
    dataRaw5bit = std::get<1>(bech32decoded);
    const auto checkSum = std::get<2>(bech32decoded);

    if (checkSum != Bech32::Bech32) {
        throw std::invalid_argument("Invalid Bech32 variant " + std::to_string(checkSum));
    }
    assert(checkSum == Bech32::Bech32);
    //std::cout << "PREFIX " << prefix << "\n";
    if (prefix.length() < 4 || prefix.substr(0, 2) != "ln") {
        throw std::invalid_argument("Invalid prefix " + prefix);
    }

    if (!findKnownPrefix(prefix, net, prefixPrefix)) {
        throw std::invalid_argument("Invalid prefix " + prefix);
    }

    const auto sigLen = 104;
    if (dataRaw5bit.size() <= sigLen) {
        throw std::invalid_argument("Too short or signature missing " + std::to_string(dataRaw5bit.size()));
    }
    assert(dataRaw5bit.size() > sigLen);
    const auto lenWoSig = dataRaw5bit.size() - sigLen;
    assert(lenWoSig > 0);
    const auto signature5 = TW::subData(dataRaw5bit, lenWoSig, sigLen);
    Bech32::convertBits<5, 8, false>(signature, signature5);

    data5bit = TW::subData(dataRaw5bit, 0, lenWoSig);
    Bech32::convertBits<5, 8, true>(data8bit, data5bit);
}

struct LNInvoice InvoiceDecoder::decodeInvoice(const std::string& invstr) {
    std::string prefix;
    LNNetwork net;
    std::string prefixPrefix;
    Data dataRaw5bit;
    Data data5bit;
    Data data8bit;
    Data signature;
    decodeInternal(invstr, prefix, net, prefixPrefix, dataRaw5bit, data5bit, data8bit, signature);

    LNInvoice inv;

    inv.network = net;
    inv.signature = signature;

    const auto prefix2 = prefix.substr(prefixPrefix.length());
    //std::cout << "PREFIX2 " << prefix2 << "\n";
    if (prefix2.length() == 0) {
        // no amount
        inv.amountPresent = false;
        inv.unparsedAmount = "";
    } else {
        // amount present
        inv.amountPresent = true;
        inv.unparsedAmount = prefix2;
    }

    size_t idx = 0;
    Data timestamp5 = TW::subData(data5bit, idx, 7);
    idx += 7;
    {
        Data conv;
        Bech32::convertBits<5, 8, true>(conv, timestamp5);
        assert(conv.size() == 5);
        inv.timestamp = ((uint64_t)(conv[0] & 0x1F) << (32-5)) + ((uint64_t)(TW::decode32BE(conv.data() + 1)) >> 5);
    }

    while (idx < data5bit.size()) {
        const auto tag = data5bit[idx++];
        //std::cout << "TAG " << (int)tag << "  " << (int)idx << " " << lenWoSig << "\n";
        if (idx + 2 > data5bit.size()) {
            break;
        }
        auto tagLen = (data5bit[idx] << 5) + data5bit[idx + 1];
        idx += 2;
        //std::cout << "TAGLEN " << (int)tagLen <<  "\n";
        if (idx + tagLen > data5bit.size()) {
            break;
        }
        const auto tagValue5 = TW::subData(data5bit, idx, tagLen);

        Data conv;
        Bech32::convertBits<5, 8, false>(conv, tagValue5);

        //std::cout << "TAG " << (int)tag << " LEN " << (int)tagLen << " " << (int)tagValue5.size() << " VAL " << hex(tagValue5) << "  " << (int)conv.size() << " " << hex(conv) << "\n";

        switch (tag) {
            case 1: // p payment hash
                if (tagLen == 52) {
                    inv.paymentHash = conv;
                }
                break;

            case 3: // r routing
                {
                    size_t cidx = 0;
                    while (cidx < conv.size()) {
                        RoutingInfo ri;
                        ri.pubkey = TW::subData(conv, cidx, 33);
                        ri.shortChannelId = TW::subData(conv, cidx + 33, 8);
                        ri.unparsedFeeBaseMsat = TW::subData(conv, cidx + 33 + 8, 4);
                        ri.unparsedFeePPM = TW::subData(conv, cidx + 33 + 8 + 4, 4); // TODO
                        ri.unparsedCltvExpiryDelta = TW::subData(conv, cidx + 33 + 8 + 4 + 4, 2); // TODO
                        cidx += 51;
                        inv.routing.push_back(ri);
                    }
                }
                break;

            case 5: // 9 features
                inv.unparsedFeatures = conv;
                break;

            case 6: // x expiry
                inv.unparsedExpiry = conv;
                break;

            case 13: // d description
                inv.description = std::string(std::string(conv.begin(), conv.end()).c_str());
                break;

            case 16: // s secret
                if (tagLen == 52) {
                    inv.secret = conv;
                }
                break;

            case 19: // n payee nodeID
                if (tagLen == 53) {
                    inv.nodeId = conv;
                }
                break;

            case 24: // c min_final_cltv_expiry
                if (tagValue5.size() == 1) {
                    inv.minFinalCltvExpiry = tagValue5[0];
                } else if (tagValue5.size() == 2) {
                    inv.minFinalCltvExpiry = decode16BE(conv.data()) >> 6;
                }
                //std::cout << "minFinalCltvExpiry " << inv.minFinalCltvExpiry << "\n";
                break;

            default:
                //std::cout << "TAG? " << (int)tag << " LEN " << (int)tagLen << " " << (int)tagValue5.size() << "\n";
                continue;
        }
        idx += tagLen;
    }

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

std::string InvoiceDecoder::buildPrefix(const LNInvoice& inv) {
    std::string prefix = "ln" + networkToPrefix(inv.network);
    if (inv.amountPresent) {
        prefix += inv.unparsedAmount;
    }
    return prefix;
}

void appendTag5(Data& data5, uint8_t tag, const Data& value5) {
    data5.push_back(tag);
    const auto tagLen = value5.size();
    data5.push_back((tagLen & 0xE0) >> 5);
    data5.push_back(tagLen & 0x1F);
    append(data5, value5);
}

void appendTag(Data& data5, uint8_t tag, const Data& value) {
    Data value5;
    Bech32::convertBits<8, 5, true>(value5, value);
    appendTag5(data5, tag, value5);
}

Data InvoiceDecoder::buildUptosig(const LNInvoice& inv) {
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
    return data5;
}

std::string InvoiceDecoder::encodeInvoice(const LNInvoice& inv) {
    const std::string prefix = buildPrefix(inv);

    Data data5 = buildUptosig(inv);

    Data signature5;
    Bech32::convertBits<8, 5, true>(signature5, inv.signature);
    TW::append(data5, signature5);

    const auto enc = Bech32::encode(prefix, data5, Bech32::Bech32);
    return enc;
}

bool InvoiceDecoder::verifySignature(const std::string& invstr, const Data& extPublicKey) {
    try {    
        std::string prefix;
        LNNetwork net;
        std::string prefixPrefix;
        Data dataRaw5bit;
        Data data5bit;
        Data data8bit;
        Data signature;
        decodeInternal(invstr, prefix, net, prefixPrefix, dataRaw5bit, data5bit, data8bit, signature);

        Data pubKeyData;
        if (extPublicKey.size() == 0) {
            // no public key provided, take it from the invoice
            const auto decoded = decodeInvoice(invstr);
            pubKeyData = decoded.nodeId;
        } else {
            // use pub key provided
            pubKeyData = extPublicKey;
        }
        if (pubKeyData.size() == 0) {
            throw std::invalid_argument("Missing public key");
        }
        //std::cout << "pubKey " << hex(pubKeyData) << "\n";

        auto dataToSign = TW::data(prefix);
        TW::append(dataToSign, data8bit);
        //std::cout << "dataToSign " << hex(dataToSign) << "\n";
        const auto hashToSign = Hash::sha256(dataToSign);
        //std::cout << "hashToSign " << hex(hashToSign) << "\n";
        const auto publicKey = PublicKey(pubKeyData, TWPublicKeyTypeSECP256k1);
        return publicKey.verify(signature, hashToSign);
    } catch (std::exception& ex) {
        //std::cout << "ex " << ex.what() << "\n";
    } catch (...) {
    }
    return false;
}

} // namespace TW::Lightning
