#pragma once

#include "TransactionAttributeUsage.h"
#include "ISerializable.h"
#include "../Data.h"
#include "../ReadData.h"

namespace TW::NEO {
    class TransactionAttribute : public ISerializable {
    public:
        TransactionAttributeUsage usage;
        Data data;

        virtual ~TransactionAttribute() {}

        int64_t size() const override {
            return sizeof(usage) + data.size();
        }

        void deserialize(const Data &data, int initial_pos = 0) override {
            usage = (TransactionAttributeUsage) readBytes(data, 1, initial_pos)[0];
            if (data.size() + initial_pos < 1) {
                throw std::invalid_argument("Invalid data for deserialization");
            }
            if (usage == TransactionAttributeUsage::TAU_ContractHash || usage == TransactionAttributeUsage::TAU_Vote ||
                (usage >= TransactionAttributeUsage::TAU_Hash1 && usage <= TransactionAttributeUsage::TAU_Hash15)) {
                this->data = readBytes(data, 32, initial_pos + 1);
            } else if (usage == TransactionAttributeUsage::TAU_ECDH02 ||
                       usage == TransactionAttributeUsage::TAU_ECDH03) {
                this->data = readBytes(data, 32, initial_pos + 1);
            } else if (usage == TransactionAttributeUsage::TAU_Script) {
                this->data = readBytes(data, 20, initial_pos + 1);
            } else if (usage == TransactionAttributeUsage::TAU_DescriptionUrl) {
                this->data = readBytes(data, 1, initial_pos + 1);
            } else if (usage == TransactionAttributeUsage::TAU_Description ||
                       usage >= TransactionAttributeUsage::TAU_Remark) {
//                this->data = readBytes(data, types::MaxValue<ushort>(), initial_pos + 1);
                this->data = readBytes(data, data.size() - 1 - initial_pos, initial_pos + 1);
            } else {
                throw std::invalid_argument("TransactionAttribute Deserialize FormatException");
            }
        }

        Data serialize() const override {
            return concat(Data({static_cast<byte>(usage)}), data);
        }
    };
}