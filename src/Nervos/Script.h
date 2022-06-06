// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../proto/Nervos.pb.h"
#include "Address.h"

#include <string>
#include <vector>

namespace TW::Nervos {

class Script {
  public:
    Data codeHash;
    HashType hashType;
    Data args;

    /// Initializes an empty script.
    Script() = default;

    /// Initializes a script from another script
    Script(const Script &script) : codeHash(script.codeHash), hashType(script.hashType), args(script.args) {}

    /// Initializes a script with codeHash, args and hashType.
    Script(const Data& codeHash, const HashType hashType, const Data &args) : codeHash(codeHash), hashType(hashType), args(args) {}

    /// Initializes a script with the given address.
    Script(const Address& address) : codeHash(address.codeHash), hashType(address.hashType), args(address.args) {}

    Script(const Data& data);

    /// Returns the script's script hash.
    Data hash() const;

    /// Whether the script is empty.
    bool empty() const;

    /// Initializes an script from a Protobuf script.
    Script(const Proto::Script& script) {
        std::copy(script.code_hash().begin(), script.code_hash().end(), std::back_inserter(codeHash));
        if (script.hash_type() == "data") {
            hashType = HashType::Data0;
        } else if (script.hash_type() == "type") {
            hashType = HashType::Type1;
        } else if (script.hash_type() == "data1") {
            hashType = HashType::Data1;
        }
        std::copy(script.args().begin(), script.args().end(), std::back_inserter(args));
    }

    /// Encodes the script.
    void encode(Data& data) const;

    Proto::Script proto() const {
        auto script = Proto::Script();
        script.set_code_hash(std::string(codeHash.begin(), codeHash.end()));
        if (hashType == HashType::Data0) {
            script.set_hash_type("data");
        } else if (hashType == HashType::Type1) {
            script.set_hash_type("type");
        } else if (hashType == HashType::Data1) {
            script.set_hash_type("data1");
        }
        script.set_args(std::string(args.begin(), args.end()));
        return script;
    }
};

inline bool operator==(const Script& lhs, const Script& rhs) {
    return (lhs.codeHash == rhs.codeHash) && (lhs.hashType == rhs.hashType) && (lhs.args == rhs.args);
}
inline bool operator!=(const Script& lhs, const Script& rhs) {
    return !(lhs == rhs);
}

} // namespace TW::Nervos

/// Wrapper for C interface.
struct TWNervosScript {
    TW::Nervos::Script impl;
};
