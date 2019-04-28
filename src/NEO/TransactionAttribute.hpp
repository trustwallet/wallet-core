#pragma once

#include "TransactionAttributeUsage.h"
#include "ISerializable.hpp"
#include "../Data.h"

namespace TW::NEO {
    class TransactionAttribute : public ISerializable {
    public:
        TransactionAttributeUsage usage;
        Data data;

        int64_t size() const {
            return sizeof(usage) + data.size();
        }

        void deserialize(const Data &data) {
            if (data.size() < 1) {
                throw std::invalid_argument("Invalid data for deserialization");
            }
            if (Usage == TransactionAttributeUsage::TAU_ContractHash || Usage == TransactionAttributeUsage::TAU_Vote ||
                (Usage >= TransactionAttributeUsage::TAU_Hash1 && Usage <= TransactionAttributeUsage::TAU_Hash15)) {
                Data = readBytes(data, 32);
            } else if (Usage == TransactionAttributeUsage::TAU_ECDH02 ||
                       Usage == TransactionAttributeUsage::TAU_ECDH03) {
                Data = concat(Data(1, (byte) Usage), readBytes(from, 32));
            } else if (Usage == TransactionAttributeUsage::TAU_Script) {
                Data = readBytes(data, 20);
            } else if (Usage == TransactionAttributeUsage::TAU_DescriptionUrl) {
                Data = reader.ReadBytes(reader.ReadByte());
            } else if (Usage == TransactionAttributeUsage::TAU_Description ||
                       Usage >= TransactionAttributeUsage::TAU_Remark) {
                Data = readVarBytes(types::MaxValue<ushort>());
            } else {
                throw std::invalid_argument("TransactionAttribute Deserialize FormatException");
            }
        }

        Data serialize() const {
            return concat(Data({usage}), data);
        }
    };
}