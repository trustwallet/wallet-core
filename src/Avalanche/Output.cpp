// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Output.h"
#include "../BinaryCoding.h"

using namespace TW::Avalanche;


bool compareOutputForSort(Output lhs, Output rhs) {
    if (std::get<0>(lhs) != std::get<0>(rhs)) {
        return std::get<0>(lhs) < std::get<0>(rhs);
    }
    if (std::get<1>(lhs) != std::get<1>(rhs)) {
        return std::get<1>(lhs) < std::get<1>(rhs);
    }
    if (std::get<2>(lhs) != std::get<2>(rhs)) {
        for (auto i = 0; i < std::get<2>(lhs).size(); ++i) {
            if (std::get<2>(lhs)[i] != std::get<2>(rhs)[i]) {
                return std::get<2>(lhs)[i] < std::get<2>(rhs)[i];
            }
        }
    }
    return false;
}

void EncodeOutputs(std::vector<Output> outputs, TW::Data &data) {
    // go through each output and sort its list of addresses 
    // then, sort the list of outputs 
    for (auto &output : outputs) {
        std::sort(std::get<2>(output).begin(), std::get<2>(output).begin());
    }
    std::sort(outputs.begin(), outputs.end(), compareOutputForSort);
    // end sort 
    TW::encode32LE(outputs.size(), data);
    for (auto output : outputs) {
        TW::encode64LE(std::get<0>(output), data);
        TW::encode32LE(std::get<1>(output), data);
        std::vector<Address> addrs = std::get<2>(output);
        TW::encode32LE(addrs.size(), data);
        for (auto addr : addrs) {
            for (auto byte : addr.bytes) {
                data.push_back(byte);
            }
        }
    }
}
