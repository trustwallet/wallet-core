// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../HexCoding.h"

#include <iostream>

using namespace std;
using namespace TW::Telegram;

bool Workchain::isValid(WorkchainId_t workchainId)
{
    return (workchainId == -1 || workchainId == 0); 
}

bool Address::isValid(const std::string& address)
{
    Address addr;

    bool isValidRaw = parseRawAddress(address, addr);
    if (isValidRaw) return true;

    bool isValidUser = parseUserAddress(address, addr);
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
    string workchainStr = addressStr_in.substr(0, colidx);
    string addressStr = addressStr_in.substr(colidx + 1, addressStr_in.length() - colidx - 1);

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

    addr_inout.addrBytes.fill(parse_hex(addressStr).front());

    return true;
}

bool Address::parseUserAddress(const std::string& addressStr_in, Address& addr_inout)
{
    // TODO
    return false;
}
