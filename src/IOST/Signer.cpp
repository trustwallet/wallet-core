// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Account.h"
#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include <google/protobuf/util/json_util.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

using namespace TW;
using namespace TW::IOST;

class IOSTEncoder {
  public:
    IOSTEncoder() = default;
    void WriteByte(uint8_t b) { buffer << b; }
    void WriteInt32(uint32_t i) {
        std::vector<uint8_t> data;
        encode32BE(i, data);
        for (auto b : data) {
            buffer << b;
        }
    }

    void WriteInt64(uint64_t i) {
        std::vector<uint8_t> data;
        encode64BE(i, data);
        for (auto b : data) {
            buffer << b;
        }
    }

    void WriteString(const std::string& s) {
        WriteInt32(static_cast<uint32_t>(s.size()));
        buffer << s;
    }

    void WriteStringSlice(const std::vector<std::string> v) {
        WriteInt32(static_cast<uint32_t>(v.size()));
        for (auto& s : v) {
            WriteString(s);
        }
    }

    std::string AsString() { return buffer.str(); }

  private:
    std::stringstream buffer;
};

std::string Signer::encodeTransaction(const Proto::Transaction& t) noexcept {
    IOSTEncoder se;
    se.WriteInt64(t.time());
    se.WriteInt64(t.expiration());
    se.WriteInt64(int64_t(t.gas_ratio() * 100));
    se.WriteInt64(int64_t(t.gas_limit() * 100));
    se.WriteInt64(t.delay());
    se.WriteInt32(int32_t(t.chain_id()));
    se.WriteString("");

    std::vector<std::string> svec;
    for (auto& item : t.signers()) {
        svec.push_back(item);
    }
    se.WriteStringSlice(svec);

    se.WriteInt32(t.actions_size());
    for (auto& a : t.actions()) {
        IOSTEncoder s;
        s.WriteString(a.contract());
        s.WriteString(a.action_name());
        s.WriteString(a.data());
        se.WriteString(s.AsString());
    }

    se.WriteInt32(t.amount_limit_size());
    for (auto& a : t.amount_limit()) {
        IOSTEncoder s;
        s.WriteString(a.token());
        s.WriteString(a.value());
        se.WriteString(s.AsString());
    }

    se.WriteInt32(t.signatures_size());
    for (auto& sig : t.signatures()) {
        IOSTEncoder s;
        s.WriteByte(static_cast<uint8_t>(sig.algorithm()));
        s.WriteString(sig.signature());
        s.WriteString(sig.public_key());
        se.WriteString(s.AsString());
    }

    return se.AsString();
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto t = input.transaction_template();

    if (t.actions_size() == 0) {
        t.add_actions();
        auto* action = t.mutable_actions(0);
        action->set_contract("token.iost");
        action->set_action_name("transfer");

        std::string token = "iost";
        std::string src = input.account().name();
        std::string dst = input.transfer_destination();
        std::string amount = input.transfer_amount();
        std::string memo = input.transfer_memo();

        nlohmann::json j;
        j.push_back(token);
        j.push_back(src);
        j.push_back(dst);
        j.push_back(amount);
        j.push_back(memo);
        std::string data = j.dump();
        action->set_data(data);
    }

    Account acc(input.account());
    auto pubkey = acc.publicActiveKey();
    std::string pubkeyStr(pubkey.begin(), pubkey.end());

    t.add_publisher_sigs();
    auto* sig = t.mutable_publisher_sigs(0);
    sig->set_algorithm(Proto::Algorithm::ED25519);
    sig->set_public_key(pubkeyStr);
    auto signature = acc.sign(Hash::sha3_256(encodeTransaction(t)), TWCurveED25519);
    std::string signatureStr(signature.begin(), signature.end());
    sig->set_signature(signatureStr);

    Proto::SigningOutput protoOutput;
    protoOutput.mutable_transaction()->CopyFrom(t);
    std::string transactionJson;
    google::protobuf::util::JsonOptions jsonOptions;
    jsonOptions.always_print_enums_as_ints = true;
    jsonOptions.preserve_proto_field_names = true;
    google::protobuf::util::MessageToJsonString(t, &transactionJson, jsonOptions);
    protoOutput.set_encoded(transactionJson);
    return protoOutput;
}

Data Signer::signaturePreimage() const {
    return data(encodeTransaction(input.transaction_template()));
}

Proto::SigningOutput Signer::compile(const Data& signature, const PublicKey& publicKey) const {
    Proto::SigningOutput output;
    // validate public key
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("Invalid public key");
    }
    {
        // validate correctness of signature
        const auto hash = Hash::sha3_256(this->signaturePreimage());
        if (!publicKey.verify(signature, hash)) {
            throw std::invalid_argument("Invalid signature/hash/publickey combination");
        }
    }
    auto t = input.transaction_template();

    std::string pubkeyStr(publicKey.bytes.begin(), publicKey.bytes.end());

    t.add_publisher_sigs();
    auto* sig = t.mutable_publisher_sigs(0);
    sig->set_algorithm(Proto::Algorithm(Proto::ED25519));
    sig->set_public_key(pubkeyStr);
    std::string signatureStr(signature.begin(), signature.end());
    sig->set_signature(signatureStr);

    std::string transactionJson;
    google::protobuf::util::JsonOptions jsonOptions;
    jsonOptions.always_print_enums_as_ints = true;
    jsonOptions.preserve_proto_field_names = true;
    google::protobuf::util::MessageToJsonString(t, &transactionJson, jsonOptions);

    output.mutable_transaction()->CopyFrom(t);
    output.set_encoded(transactionJson);
    return output;
}
