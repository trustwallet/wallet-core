// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "Constants.h"
#include "../Data.h"
#include "../proto/Nervos.pb.h"

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

    /// Copy constructor
    Script(const Script& script)
        : codeHash(script.codeHash), hashType(script.hashType), args(script.args) {}

    /// Move constructor
    Script(Script&& script)
        : codeHash(std::move(script.codeHash))
        , hashType(script.hashType)
        , args(std::move(script.args)) {}

    /// Initializes a script with codeHash, args and hashType.
    Script(const Data& codeHash, const HashType hashType, const Data& args)
        : codeHash(codeHash), hashType(hashType), args(args) {}

    /// Initializes a script with the given address.
    Script(const Address& address)
        : codeHash(address.codeHash), hashType(address.hashType), args(address.args) {}

    // Copy assignment operator
    Script& operator=(const Script& script) {
        codeHash = script.codeHash;
        hashType = script.hashType;
        args = script.args;
        return *this;
    }

    // Move assignment operator
    Script& operator=(Script&& script) {
        codeHash = std::move(script.codeHash);
        hashType = script.hashType;
        args = std::move(script.args);
        return *this;
    }

    /// Returns the script's script hash.
    Data hash() const;

    /// Whether the script is empty.
    bool empty() const;

    /// Initializes an script from a Protobuf script.
    Script(const Proto::Script& script) {
        auto& scriptCodeHash = script.code_hash();
        codeHash.insert(codeHash.end(), scriptCodeHash.begin(), scriptCodeHash.end());
        hashType = Constants::stringToHashType(script.hash_type());
        auto& scriptArgs = script.args();
        args.insert(args.end(), scriptArgs.begin(), scriptArgs.end());
    }

    /// Encodes the script.
    void encode(Data& data) const;

    Proto::Script proto() const {
        auto script = Proto::Script();
        script.set_code_hash(std::string(codeHash.begin(), codeHash.end()));
        script.set_hash_type(Constants::hashTypeToString(hashType));
        script.set_args(std::string(args.begin(), args.end()));
        return script;
    }
};

inline bool operator==(const Script& lhs, const Script& rhs) {
    return (lhs.codeHash == rhs.codeHash) && (lhs.hashType == rhs.hashType) &&
           (lhs.args == rhs.args);
}
inline bool operator!=(const Script& lhs, const Script& rhs) {
    return !(lhs == rhs);
}

} // namespace TW::Nervos

/// Wrapper for C interface.
struct TWNervosScript {
    TW::Nervos::Script impl;
};
