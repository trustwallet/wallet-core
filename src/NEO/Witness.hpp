#pragma once

#include "ISerializable.h"
#include "../Data.h"

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
            invocationScript = readVarBytes(data, initial_pos);
            verificationScript = readVarBytes(data, initial_pos + 4 + invocationScript.size());
        }

        Data serialize() const override {
            return concat(writeVarBytes(invocationScript), writeVarBytes(verificationScript));
        }
    };
}