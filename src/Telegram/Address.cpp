// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../HexCoding.h"
#include "../Base64.h"
#include "../Crc.h"

#include <iostream>

using namespace std;
using namespace TW::Telegram;

bool Workchain::isValid(WorkchainId_t workchainId)
{
    return (workchainId == MasterChainId || workchainId == BasicChainId);
}

Address::Address(const std::string& address)
{
    bool valid = parseAddress(address, *this);

    // Ensure address is valid
    if (!valid) {
        throw std::invalid_argument("Invalid address data");
    }
}

Address::Address(const PublicKey& publicKey)
{
    // TODO
    // Steps: a StateInit account state is created (containing code and data), its hash is taken, and new address is derived from the hash
    throw std::logic_error("TODO: Not yet implemented");
}

bool Address::isValid(const std::string& address)
{
    Address addr;
    bool isValid = parseAddress(address, addr);
    return isValid;
}

bool Address::parseAddress(const std::string& addressStr_in, Address& addr_inout)
{
    // try several formats, start with the common one, stop if one matches
    bool isValidUser = parseUserAddress(addressStr_in, addr_inout);
    if (isValidUser)
    {
        return true;
    }
    bool isValidRaw = parseRawAddress(addressStr_in, addr_inout);
    return isValidRaw;
}

bool Address::parseRawAddress(const std::string& addressStr_in, Address& addr_inout)
{
    // split by colon ':'
    auto colidx = addressStr_in.find(':');

    if (colidx == string::npos)
    {
        // no colon, invalid
        return false;
    }
    if (colidx < 1 || colidx >= addressStr_in.length() - 1)
    {
        // colon in wrong position
        return false;
    }
    std::string workchainStr = addressStr_in.substr(0, colidx);
    std::string addressStr = addressStr_in.substr(colidx + 1, addressStr_in.length() - colidx - 1);

    WorkchainId_t workchainId;
    try
    {
        workchainId = std::stoi(workchainStr);
    }
    catch(const std::exception& e)
    {
        // workchain ID is invalid (not a decimal number)
        return false;
    }
    if (!Workchain::isValid(workchainId))
    {
        // invalid workchain ID
        return false;
    }
    addr_inout.workchainId = workchainId;

    if (addressStr.length() != AddressLength * 2)
    {
        // wrong length of address part
        return false;
    }

    auto hexParse = parse_hex(addressStr);
    std::copy(hexParse.begin(), hexParse.end(), addr_inout.addrBytes.begin());

    addr_inout.isBounceable = true;
    addr_inout.isTestOnly = false;

    return true;
}

bool Address::convertBase64Url(const std::string& base64Url_in, std::string& base64Std_out)
{
    // Base64URL format ('+' and '/' are replaced by '-' and '_')
    if (base64Url_in.find('-') == string::npos && base64Url_in.find('_') == string::npos)
    {
        // no Base64Url-specific characters
        return false;
    }
    // make copy, replace
    base64Std_out = std::string(base64Url_in.c_str());
    for(auto i = base64Std_out.begin(), n = base64Std_out.end(); i != n; ++i)
    {
        if (*i == '-') *i = '+';
        else if (*i == '_') *i = '/';
    }
    return true;
}

bool Address::parseUserAddress(const std::string& addressStr_in, Address& addr_inout)
{
    TW::Data bytes;
    try
    {
        bytes = TW::Base64::decode(addressStr_in);
    }
    catch(const exception& ex)
    {
        // 2nd try: Base64URL format ('+' and '/' are replaced by '-' and '_')
        std::string base64Url;
        if (!convertBase64Url(addressStr_in, base64Url))
        {
            // not valid base64url
            return false;
        }
        // looks to be URL format
        try
        {
            bytes = TW::Base64::decode(base64Url);
        }
        catch(const std::exception& ex)
        {
            // not valid base64url
            return false;
        }
    }

    // check length
    if (bytes.size() != 36)
    {
        return false;
    }
    assert(bytes.size() >= 36);

    addr_inout.isBounceable = true;
    addr_inout.isTestOnly = false;
    byte tagByte = bytes[0];
    if (tagByte >= 0x80)
    {
        // test-only
        tagByte -= 0x80;
        addr_inout.isTestOnly = true;
    }
    if (tagByte >= 0x40)
    {
        // not bounceable
        tagByte -= 0x40;
        addr_inout.isBounceable = false;
    }
    if (tagByte != 0x11)
    {
        // invalid tag
        return false;
    }

    byte chainId = bytes[1];
    switch (chainId)
    {
        case 0x00: addr_inout.workchainId = Workchain::BasicChainId; break;
        case 0xff: addr_inout.workchainId = Workchain::MasterChainId; break;
        default: return false; // invalid chain
    }

    // 32 bytes address
    std::copy(bytes.begin() + 2, bytes.begin() + 2 + AddressLength, addr_inout.addrBytes.begin());

    // check CRC
    uint16_t crcGiven = ((uint16_t)bytes[34] << 8) + bytes[35];
    uint16_t crcComputed = TW::Crc::crc16(bytes.data(), 2 + AddressLength);
    if (crcGiven != crcComputed)
    {
        // CRC mismatch
        return false;
    }

    return true;
}

std::string Address::string() const
{
    TW::Data bytes;
    // tag
    byte tag = 0x11 +
        0x40 * (isBounceable ? 0 : 1) +
        0x80 * (isTestOnly ? 1 : 0);
    bytes.push_back(tag);
    byte chainId = 0;
    switch (workchainId)
    {
        case Workchain::BasicChainId: chainId = 0x00; break;
        case Workchain::MasterChainId: chainId = 0xff; break;
        default: chainId = 0x01; break; // invalid
    }
    bytes.push_back(chainId);
    for(auto i = addrBytes.begin(), n = addrBytes.end(); i != n; ++i) bytes.push_back(*i);
    // add crc checksumS
    uint16_t crc = TW::Crc::crc16(bytes.data(), bytes.size());
    bytes.push_back(crc >> 8);
    bytes.push_back(crc & 0xff);

    // base64 encode
    std::string addrEnc = TW::Base64::encode(bytes);
    return addrEnc;
}

std::string Address::stringRaw() const
{
    std::stringstream ss;
    ss << workchainId << ':' << hex(addrBytes);
    return ss.str();
}

bool operator==(const Address& lhs, const Address& rhs)
{
    return (
        lhs.workchainId == rhs.workchainId &&
        lhs.addrBytes == rhs.addrBytes &&
        lhs.isBounceable == rhs.isBounceable &&
        lhs.isTestOnly == rhs.isTestOnly);
}
