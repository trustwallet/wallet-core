#pragma once

#include "ISerializable.h"
#include "../Data.h"
#include "../UInt.hpp"

namespace TW::NEO {
    class Witness : public ISerializable {
    public:
        Data invocationScript;
        Data verificationScript;
    public:
        virtual ~Witness() {}

        int64_t size() const override {
            return invocationScript.size() + verificationScript.size();
        }

        void deserialize(const Data &data, int initial_pos = 0) override {
            invocationScript = readBytes(data, 65536, initial_pos);
            verificationScript = readBytes(data, 65536, initial_pos + invocationScript.size());
        }

        Data serialize() const override {
            return concat(invocationScript, verificationScript);
        }
    };
}