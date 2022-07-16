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
#include <nlohmann/json.hpp>

#include <string>
#include <vector>

using json = nlohmann::json;

namespace TW::Nervos {

struct Script {
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

    friend bool operator==(const Script& lhs, const Script& rhs) {
        return (lhs.codeHash == rhs.codeHash) && (lhs.hashType == rhs.hashType) &&
               (lhs.args == rhs.args);
    }

    friend bool operator!=(const Script& lhs, const Script& rhs) { return !(lhs == rhs); }

    /// Returns the script's script hash.
    Data hash() const;

    /// Whether the script is empty.
    [[nodiscard]] bool empty() const;

    /// Initializes an script from a Protobuf script.
    Script(const Proto::Script& script) {
        auto&& scriptCodeHash = script.code_hash();
        codeHash.insert(codeHash.end(), scriptCodeHash.begin(), scriptCodeHash.end());
        auto&& hashTypeString = script.hash_type();
        auto&& hashTypeRegistry = Constants::gHashTypeRegistry;
        std::for_each(hashTypeRegistry.begin(), hashTypeRegistry.end(),
                      [&](const std::pair<HashType, std::string>& p) {
                          if (p.second == hashTypeString) {
                              hashType = p.first;
                          }
                      });
        auto&& scriptArgs = script.args();
        args.insert(args.end(), scriptArgs.begin(), scriptArgs.end());
    }

    /// Encodes the script.
    void encode(Data& data) const;

    /// Encodes the script into JSON format.
    json JSON() const;

    Proto::Script proto() const {
        auto script = Proto::Script();
        script.set_code_hash(std::string(codeHash.begin(), codeHash.end()));
        script.set_hash_type(Constants::gHashTypeRegistry.at(hashType));
        script.set_args(std::string(args.begin(), args.end()));
        return script;
    }
};

} // namespace TW::Nervos
