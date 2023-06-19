// Copyright © 2019 Mart Roosmaa.
// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include <nlohmann/json.hpp>

#include <boost/multiprecision/cpp_int.hpp>
#include <google/protobuf/util/json_util.h>

using namespace TW;

using uint128_t = boost::multiprecision::uint128_t;
using json = nlohmann::json;

namespace TW::Nano {

const std::array<byte, 32> kBlockHashPreamble{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
};

std::array<byte, 16> store(const uint128_t& value) {
    using boost::multiprecision::cpp_int;

    Data buf;
    buf.reserve(16);
    export_bits(value, std::back_inserter(buf), 8);

    // Zero-pad the bytes
    while (buf.size() < 16) {
        buf.insert(buf.begin(), 0);
    }

    std::array<byte, 16> arr = {0};
    std::copy_n(buf.begin(), arr.size(), arr.begin());
    return arr;
}

std::array<byte, 32> previousFromInput(const Proto::SigningInput& input) {
    std::array<byte, 32> parentHash = {0};
    if (input.parent_block().size() != 0) {
        if (input.parent_block().size() != parentHash.size()) {
            throw std::invalid_argument("Invalid parent block hash");
        }
        std::copy_n(input.parent_block().begin(), parentHash.size(), parentHash.begin());
    }
    return parentHash;
}

std::array<byte, 32> linkFromInput(const Proto::SigningInput& input, bool emptyParentHash = false) {
    std::array<byte, 32> link = {0};
    switch (input.link_oneof_case()) {
        case Proto::SigningInput::kLinkBlock: {
            if (input.link_block().size() != link.size()) {
                throw std::invalid_argument("Invalid link block hash");
            }
            std::copy_n(input.link_block().begin(), link.size(), link.begin());
            break;
        }
        case Proto::SigningInput::kLinkRecipient: {
            if (!emptyParentHash) {
                auto toAddress = Address(input.link_recipient());
                std::copy_n(toAddress.bytes.begin(), link.size(), link.begin());
            }
            break;
        }
        case Proto::SigningInput::LINK_ONEOF_NOT_SET: break;
    }
    return link;
}

std::array<byte, 32> hashBlockData(const PublicKey& publicKey, const Proto::SigningInput& input) {
    std::array<byte, 32> parentHash = previousFromInput(input);
    bool emptyParentHash = std::all_of(parentHash.begin(), parentHash.end(), [](auto b) { return b == 0; });

    std::array<byte, 32> repPublicKey = {0};
    auto repAddress = Address(input.representative());
    std::copy_n(repAddress.bytes.begin(), repPublicKey.size(), repPublicKey.begin());

    uint128_t balance_uint;
    try {
        balance_uint = uint128_t(input.balance());
    } catch (const std::runtime_error&) {
        throw std::invalid_argument("Invalid balance");
    }
    bool zeroBalance = balance_uint == uint128_t(0);
    std::array<byte, 16> balance = store(balance_uint);
    if (emptyParentHash && zeroBalance) {
        throw std::invalid_argument("Invalid balance");
    }

    std::array<byte, 32> link = linkFromInput(input, emptyParentHash);
    bool emptyLink = std::all_of(link.begin(), link.end(), [](auto b) { return b == 0; });
    if (emptyParentHash && emptyLink) {
        throw std::invalid_argument("Missing link block hash");
    }

    auto msg = Data();
    msg.insert(msg.end(), kBlockHashPreamble.begin(), kBlockHashPreamble.end());
    msg.insert(msg.end(), publicKey.bytes.begin(), publicKey.bytes.end());
    msg.insert(msg.end(), parentHash.begin(), parentHash.end());
    msg.insert(msg.end(), repPublicKey.begin(), repPublicKey.end());
    msg.insert(msg.end(), balance.begin(), balance.end());
    msg.insert(msg.end(), link.begin(), link.end());

    std::array<byte, 32> blockHash = {0};
    auto digest = Hash::blake2b(msg, blockHash.size());
    std::copy_n(digest.begin(), blockHash.size(), blockHash.begin());

    return blockHash;
}

Signer::Signer(const Proto::SigningInput& input)
  : privateKey(Data(input.private_key().begin(), input.private_key().end())),
    publicKey(privateKey.getPublicKey(TWPublicKeyTypeED25519Blake2b)),
    input(input),
    previous{previousFromInput(input)},
    link{linkFromInput(input)},
    blockHash(hashBlockData(publicKey, input)) {}


Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    Proto::SigningOutput output;
    try {
        auto signer = Signer(input);
        output = signer.build();
    }
    catch (...) {}
    return output;
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = Signer::sign(input);
    return output.json();
}

std::array<byte, 64> Signer::sign() const noexcept {
    auto digest = Data(blockHash.begin(), blockHash.end());
    auto sig = privateKey.sign(digest, TWCurveED25519Blake2bNano);

    std::array<byte, 64> signature = {0};
    std::copy_n(sig.begin(), signature.size(), signature.begin());
    return signature;
}

Proto::SigningOutput Signer::build() const {
    auto output = Proto::SigningOutput();
    const auto signature = sign();
    output.set_signature(signature.data(), signature.size());
    output.set_block_hash(blockHash.data(), blockHash.size());

    // build json
    json json = {
        {"type", "state"},
        {"account", Address(publicKey).string()},
        {"previous", hex(previous)},
        {"representative", Address(input.representative()).string()},
        {"balance", input.balance()},
        {"link", hex(link)},
        {"link_as_account", Address(PublicKey(Data(link.begin(), link.end()), TWPublicKeyTypeED25519Blake2b)).string()},
        {"signature", hex(signature)},
    };

    if (input.work().size() > 0) {
        json["work"] = input.work();
    }

    output.set_json(json.dump());
    return output;
}

Data Signer::buildUnsignedTxBytes(const Proto::SigningInput& input) {
    const auto pubKey = PublicKey(Data(input.public_key().begin(), input.public_key().end()), TWPublicKeyTypeED25519Blake2b);
    auto block = hashBlockData(pubKey, input);
    return Data(block.begin(), block.end());
}

Proto::SigningOutput Signer::buildSigningOutput(const Proto::SigningInput& input, const Data& signature) {
    auto output = Proto::SigningOutput();
    const auto pubKey = PublicKey(Data(input.public_key().begin(), input.public_key().end()), TWPublicKeyTypeED25519Blake2b);
    auto block = hashBlockData(pubKey, input);
    output.set_signature(signature.data(), signature.size());
    output.set_block_hash(block.data(), block.size());

    auto prev = previousFromInput(input);
    auto li = linkFromInput(input);

    // build json
    json json = {
        {"type", "state"},
        {"account", Address(pubKey).string()},
        {"previous", hex(prev)},
        {"representative", Address(input.representative()).string()},
        {"balance", input.balance()},
        {"link", hex(li)},
        {"link_as_account", Address(PublicKey(Data(li.begin(), li.end()), TWPublicKeyTypeED25519Blake2b)).string()},
        {"signature", hex(signature)},
    };

    if (input.work().size() > 0) {
        json["work"] = input.work();
    }

    output.set_json(json.dump());
    return output;
}

} // namespace TW::Nano
