// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <vector>

namespace TW {
namespace Bitcoin {

class Script {
public:
    /// Script raw bytes.
    std::vector<uint8_t> bytes;

    /// Initializes an empty script.
    Script() = default;

    /// Initializes a script with a collection of raw bytes.
    template<typename It>
    Script(It begin, It end) : bytes(begin, end) {}

    /// Initializaes a script with a collection of raw bytes by moving.
    Script(std::vector<uint8_t>&& bytes) : bytes(bytes) {}

    /// Whether the script is empty.
    bool empty() const {
        return bytes.empty();
    }

    /// Returns the script's script hash.
    std::vector<uint8_t> hash() const;

    /// Determines whether this is a pay-to-script-hash (P2SH) script.
    bool isPayToScriptHash() const;

    /// Determines whether this is a pay-to-witness-script-hash (P2WSH) script.
    bool isPayToWitnessScriptHash() const;

    /// Determines whether this is a witness programm script.
    bool isWitnessProgram() const;

    /// Matches the script to a pay-to-public-key (P2PK) script.
    bool matchPayToPubkey(std::vector<uint8_t>& publicKey) const;

    /// Matches the script to a pay-to-public-key-hash (P2PKH).
    bool matchPayToPubkeyHash(std::vector<uint8_t>& keyHash) const;

    /// Matches the script to a pay-to-script-hash (P2SH).
    bool matchPayToScriptHash(std::vector<uint8_t>& scriptHash) const;

    /// Matches the script to a pay-to-witness-public-key-hash (P2WPKH).
    bool matchPayToWitnessPublicKeyHash(std::vector<uint8_t>& keyHash) const;

    /// Matches the script to a pay-to-witness-script-hash (P2WSH).
    bool matchPayToWitnessScriptHash(std::vector<uint8_t>& scriptHash) const;

    /// Matches the script to a multisig script.
    bool matchMultisig(std::vector<std::vector<uint8_t>>& publicKeys, int& required) const;

    /// Builds a pay-to-public-key-hash (P2PKH) script from a public key hash.
    static Script buildPayToPublicKeyHash(const std::vector<uint8_t>& hash);

    /// Builds a pay-to-script-hash (P2SH) script from a script hash.
    static Script buildPayToScriptHash(const std::vector<uint8_t>& scriptHash);

    /// Builds a pay-to-witness-public-key-hash (P2WPKH) script from a public key hash.
    static Script buildPayToWitnessPubkeyHash(const std::vector<uint8_t>& hash);

    /// Builds a pay-to-witness-script-hash (P2WSH) script from a script hash.
    static Script buildPayToWitnessScriptHash(const std::vector<uint8_t>& scriptHash);

    /// Builds a pay-to-public-key-hash (P2PKH) script appropriate for the given address.
    static Script buildForAddress(const std::string& address);

    /// Encodes the script.
    void encode(std::vector<uint8_t>& data) const;

private:

    /// Extracts a single opcode at the given index including its operand.
    ///
    /// \param index [in/out] index where the operation starts, on return the index of the next operation.
    /// \param opcode [out] the opcode.
    /// \param operand [out] the opcode's operand.
    /// \returns whether an opcode was available.
    bool getScriptOp(size_t& index, uint8_t& opcode, std::vector<uint8_t>& operand) const;
};

inline bool operator==(const Script& lhs, const Script& rhs) { return lhs.bytes == rhs.bytes; }
inline bool operator!=(const Script& lhs, const Script& rhs) { return !(lhs == rhs); }

}} // namespace

/// Wrapper for C interface.
struct TWBitcoinScript {
    TW::Bitcoin::Script impl;
};
