// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Constants.h"
#include "ISerializable.h"
#include "Serializable.h"
#include "TransactionAttributeUsage.h"
#include "Data.h"

namespace TW::NEO {

class TransactionAttribute final: public Serializable {
public:
    TransactionAttributeUsage usage = TAU_ContractHash;
    Data _data;

    ~TransactionAttribute() override = default;

    size_t size() const override {
        switch (usage) {
        case TransactionAttributeUsage::TAU_ContractHash:
        case TransactionAttributeUsage::TAU_ECDH02:
        case TransactionAttributeUsage::TAU_ECDH03:
        case TransactionAttributeUsage::TAU_Vote:
            return 1UL + contractHashSize;
        case TransactionAttributeUsage::TAU_Script:
            return 1UL + scriptHashSize;
        default:
            if (usage >= TransactionAttributeUsage::TAU_Hash1 &&
                usage <= TransactionAttributeUsage::TAU_Hash15) {
                return 1 + contractHashSize;
            }
            return 1UL + static_cast<size_t>(varIntSize(_data.size())) + _data.size();
        }
    }

    void deserialize(const Data& data, size_t initial_pos = 0) override {
        if (data.size() < initial_pos + 1) {
            throw std::invalid_argument("Invalid data for deserialization");
        }

        // see: https://github.com/neo-project/neo/blob/v2.12.0/neo/Network/P2P/Payloads/TransactionAttribute.cs#L32
        usage = (TransactionAttributeUsage)data[initial_pos];
        switch (usage) {
        case TransactionAttributeUsage::TAU_ECDH02:
        case TransactionAttributeUsage::TAU_ECDH03: {
            this->_data = concat({(TW::byte)usage}, readBytes(data, contractHashSize, initial_pos + 1));
            break;
        }

        case TransactionAttributeUsage::TAU_Script: {
            this->_data = readBytes(data, scriptHashSize, initial_pos + 1);
            break;
        }

        case TransactionAttributeUsage::TAU_DescriptionUrl:
        case TransactionAttributeUsage::TAU_Description:
        case TransactionAttributeUsage::TAU_Remark: {
            this->_data = readVarBytes(data, initial_pos + 1);
            break;
        }

        default:
            if (usage == TransactionAttributeUsage::TAU_ContractHash ||
                usage == TransactionAttributeUsage::TAU_Vote ||
                (usage >= TransactionAttributeUsage::TAU_Hash1 && usage <= TransactionAttributeUsage::TAU_Hash15)) {
                this->_data = readBytes(data, contractHashSize, initial_pos + 1);
                break;
            }
            throw std::invalid_argument("TransactionAttribute Deserialize FormatException");
        }
    }

    Data serialize() const override {
        Data result;
        result.push_back((TW::byte)usage);

        // see: https://github.com/neo-project/neo/blob/v2.12.0/neo/Network/P2P/Payloads/TransactionAttribute.cs#L49
        if (usage == TransactionAttributeUsage::TAU_DescriptionUrl ||
            usage == TransactionAttributeUsage::TAU_Description ||
            usage >= TransactionAttributeUsage::TAU_Remark) {
            Data resp;
            encodeVarInt((uint64_t)_data.size(), resp);
            result.insert(result.end(), resp.begin(), resp.end());
        }
        if (usage == TransactionAttributeUsage::TAU_ECDH02 ||
            usage == TransactionAttributeUsage::TAU_ECDH03) {
            result.insert(result.end(), _data.begin() + 1, _data.begin() + 1 + contractHashSize);
        } else {
            result.insert(result.end(), _data.begin(), _data.end());
        }

        return result;
    }

    bool operator==(const TransactionAttribute &other) const {
        return this->usage == other.usage
            && _data.size() == other._data.size()
            && _data == other._data;
    }
};

} // namespace TW::NEO
