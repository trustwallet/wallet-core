#pragma once

#include "TransactionAttributeUsage.h"
#include "ISerializable.h"
#include "../Data.h"

namespace TW::NEO {
    class TransactionAttribute : public ISerializable {
    public:
        TransactionAttributeUsage usage;
        Data data;

        int64_t size() const override {
            return sizeof(usage) + data.size();
        }

        void deserialize(const Data &data) override {
            if (data.size() < 1) {
                throw std::invalid_argument("Invalid data for deserialization");
            }
            if (usage == TransactionAttributeUsage::TAU_ContractHash || usage == TransactionAttributeUsage::TAU_Vote ||
                (usage >= TransactionAttributeUsage::TAU_Hash1 && usage <= TransactionAttributeUsage::TAU_Hash15)) {
                this->data = readBytes(data, 32);
            } else if (usage == TransactionAttributeUsage::TAU_ECDH02 ||
                       usage == TransactionAttributeUsage::TAU_ECDH03) {
                this->data = concat(Data(1, (byte) usage), readBytes(data, 32));
            } else if (usage == TransactionAttributeUsage::TAU_Script) {
                this->data = readBytes(data, 20);
            } else if (usage == TransactionAttributeUsage::TAU_DescriptionUrl) {
                this->data =readBytes(data, 1);;
            } else if (usage == TransactionAttributeUsage::TAU_Description ||
                       usage >= TransactionAttributeUsage::TAU_Remark) {
                this->data = readBytes(data, types::MaxValue<ushort>());
            } else {
                throw std::invalid_argument("TransactionAttribute Deserialize FormatException");
            }
        }

        Data serialize() const override {
            return concat(Data({static_cast<unsigned char>(usage)}), data);
        }
    };
}