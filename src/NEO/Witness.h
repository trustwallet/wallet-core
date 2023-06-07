// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "ISerializable.h"
#include "Serializable.h"

namespace TW::NEO {

class Witness final: public Serializable {
  public:
    Data invocationScript;
    Data verificationScript;

    ~Witness() override = default;

    size_t size() const override {
        return invocationScript.size() + verificationScript.size();
    }

    void deserialize(const Data& data, size_t initial_pos = 0) override {
        uint32_t size;
        invocationScript = readVarBytes(data, initial_pos, &size);
        verificationScript = readVarBytes(data, initial_pos + static_cast<size_t>(size));
    }

    Data serialize() const override {
        return concat(writeVarBytes(invocationScript), writeVarBytes(verificationScript));
    }

    bool operator==(const Witness &other) const {
        return this->invocationScript.size() == other.invocationScript.size()
            && this->verificationScript.size() == other.verificationScript.size()
            && this->invocationScript == other.invocationScript
            && this->verificationScript == other.verificationScript;
    }
};

}