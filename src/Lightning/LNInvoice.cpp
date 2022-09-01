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
#include "HexCoding.h"

//#include <iostream> // TODO remove

namespace TW::Lightning {

constexpr auto SignatureLength8bit = 65;
constexpr auto SignatureLength5bit = 104;

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

    if (dataRaw5bit.size() <= SignatureLength5bit) {
        throw std::invalid_argument("Too short or signature missing " + std::to_string(dataRaw5bit.size()));
    }
    assert(dataRaw5bit.size() > SignatureLength5bit);
    const auto lenWoSig = dataRaw5bit.size() - SignatureLength5bit;
    assert(lenWoSig > 0);
    const auto signature5 = TW::subData(dataRaw5bit, lenWoSig, SignatureLength5bit);
    Bech32::convertBits<5, 8, false>(signature, signature5);

    data5bit = TW::subData(dataRaw5bit, 0, lenWoSig);
    Bech32::convertBits<5, 8, true>(data8bit, data5bit);
}

bool verifySignatureInternal(const std::string& prefix, const Data& data8bit, const Data& publicKey, const Data& signature) {
    //std::cout << "pubKey " << hex(publicKey) << "\n";
    if (publicKey.size() == 0) {
        throw std::invalid_argument("Missing public key");
    }
    if (publicKey.size() != 33) {
        throw std::invalid_argument("Wrong public key size " + std::to_string(publicKey.size()));
    }

    auto dataToSign = TW::data(prefix);
    TW::append(dataToSign, data8bit);
    //std::cout << "dataToSign " << hex(dataToSign) << "\n";
    const auto hashToSign = Hash::sha256(dataToSign);
    //std::cout << "hashToSign " << hex(hashToSign) << "\n";
    return PublicKey(publicKey, TWPublicKeyTypeSECP256k1).verify(signature, hashToSign);
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

    // check signature (as extra info)
    inv.mismatchNodeidSignature = false;
    if (inv.nodeId.size() > 0) {
        inv.mismatchNodeidSignature = !verifySignatureInternal(prefix, data8bit, inv.nodeId, inv.signature);
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

    if (inv.paymentHash.size() > 0) {
        if (inv.paymentHash.size() != 32) {
            throw std::invalid_argument("Payment hash invalid size " + std::to_string(inv.paymentHash.size()));
        }
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
    if (inv.secret.size() > 0) {
        if (inv.secret.size() != 32) {
            throw std::invalid_argument("Secret invalid size " + std::to_string(inv.secret.size()));
        }
        appendTag(data5, 16, inv.secret);
    }
    if (inv.nodeId.size() > 0) {
        if (inv.nodeId.size() != 33) {
            throw std::invalid_argument("NodeId invalid size " + std::to_string(inv.nodeId.size()));
        }
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

std::string InvoiceDecoder::encodeInvoice(const LNInvoice& inv, const Data& optionalPrivateKey) {
    const std::string prefix = buildPrefix(inv);

    Data data5 = buildUptosig(inv);

    Data signature;
    if (optionalPrivateKey.size() > 0) {
        // private key provided, use that to create signature
        if (optionalPrivateKey.size() != 32) {
            throw std::invalid_argument("optionalPrivateKey worng size " + std::to_string(optionalPrivateKey.size()));
        }

        // verify that private key and nodeId matches (if nodeId is present)
        if (inv.nodeId.size() > 0) {
            const auto publicKey = PrivateKey(optionalPrivateKey).getPublicKey(TWPublicKeyTypeSECP256k1);
            if (inv.nodeId != publicKey.bytes) {
                throw std::invalid_argument("Wrong private key provided, public keys do not match " + hex(inv.nodeId) + " " + hex(publicKey.bytes));
            }
        }

        // set placeholder signature input
        auto inv2 = inv;
        inv2.signature = Data(SignatureLength8bit);
        const auto encodedWithNosig = encodeInvoice(inv2, Data());
        //std::cout << "encodedWithNosig " << hex(encodedWithNosig) << "\n";
        // create signature
        signature = buildSignature(encodedWithNosig, optionalPrivateKey);
        //std::cout << "signature " << hex(signature) << "\n";
        assert(signature.size() == SignatureLength8bit);
    } else {
        // no private key, use provided signature
        signature = inv.signature;
        if (signature.size() != SignatureLength8bit) {
            throw std::invalid_argument("Provided signature wrong size " + std::to_string(signature.size()));
        }
        assert(signature.size() == SignatureLength8bit);
    }

    Data signature5;
    Bech32::convertBits<8, 5, true>(signature5, signature);
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

        return verifySignatureInternal(prefix, data8bit, pubKeyData, signature);
    } catch (std::exception& ex) {
        //std::cout << "ex " << ex.what() << "\n";
    } catch (...) {
    }
    return false;
}

Data InvoiceDecoder::buildSignature(const std::string& invstr, const Data& privateKey) {
    std::string prefix;
    LNNetwork net;
    std::string prefixPrefix;
    Data dataRaw5bit;
    Data data5bit;
    Data data8bit;
    Data signature;
    decodeInternal(invstr, prefix, net, prefixPrefix, dataRaw5bit, data5bit, data8bit, signature);

    auto dataToSign = TW::data(prefix);
    TW::append(dataToSign, data8bit);
    //std::cout << "dataToSign " << hex(dataToSign) << "\n";
    const auto hashToSign = Hash::sha256(dataToSign);
    //std::cout << "hashToSign " << hex(hashToSign) << "\n";

    const auto pk = PrivateKey(privateKey);
    return pk.sign(hashToSign, TWCurveSECP256k1);
}

} // namespace TW::Lightning
