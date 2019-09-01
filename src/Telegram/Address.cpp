// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../HexCoding.h"
#include "../Base64.h"

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
    bool isValidRaw = parseRawAddress(addressStr_in, addr_inout);
    if (isValidRaw)
    {
        // raw format, stop
        return true;
    }
    // not raw format, must be user friendly format
    bool isValidUser = parseUserAddress(addressStr_in, addr_inout);
    return isValidUser;
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

    return true;
}

bool Address::parseUserAddress(const std::string& addressStr_in, Address& addr_inout)
{
    TW::Data base64decode;
    try
    {
        base64decode = TW::Base64::decode(addressStr_in);
    }
    catch(exception ex)
    {
        // not valid base64
        return false;
    }

    byte tagByte = base64decode[0];
    if (tagByte != 0x11 && tagByte != 0x51 && tagByte != 0x91 && tagByte != 0xd1)
    {
        return false;
    }

    byte chainId = base64decode[1];
    switch (chainId)
    {
    case 0x00:
        // basic workchain
        addr_inout.workchainId = Workchain::BasicChainId;
        break;

    case 0xff:
        // master chain
        addr_inout.workchainId = Workchain::MasterChainId;
        break;

    default:
        // invalid chain
        return false;
    }

    // 32 bytes address
    std::copy(base64decode.begin() + 2, base64decode.begin() + 2 + AddressLength, addr_inout.addrBytes.begin());

    // TODO check CRC

    return true;
}

std::string Address::string() const
{
    // TODO
    return "TODO";
}

std::string Address::stringRaw() const
{
    std::stringstream ss;
    ss << workchainId << ':' << hex(addrBytes);
    return ss.str();
}
