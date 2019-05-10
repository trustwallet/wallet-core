#pragma once

#include "ISerializable.h"
#include "../Data.h"
#include "../uint160.hpp"

namespace TW::NEO {
    class Witness : public ISerializable {
    public:
        Data invocationScript;
        Data verificationScript;

    public:
        int64_t size() const {
            return invocationScript.size() + verificationScript.size();
        }

        void deserialize(const Data &data) {
            invocationScript = readBytes(data, 65536);
            verificationScript = readBytes(data, 65536, invocationScript.size());
        }

        Data serialize() const {
            return concat(invocationScript, verificationScript);
        }
    };
}