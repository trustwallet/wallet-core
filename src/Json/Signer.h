//
// Created by Leone Parise on 2019-05-24.
//
#pragma once

#include "../Coin.h"
#include "../PrivateKey.h"
#include "../proto/Json.pb.h"

enum JsonSignErrorCode {
    JsonSignErrorCodeNotSupported = 1,
    JsonSignErrorCodeJsonParseError = 2,
};

namespace TW::Json {
/// Helper class to perform json signing
class Signer {
public:
    Proto::SigningOutput sign(TWCoinType coinType, const std::string &transaction, const std::string &privateKey) const noexcept;
private:
    void parse(const std::string &transaction, google::protobuf::Message *message, Proto::SigningOutput &output) const noexcept;
};

}

/// Wrapper for C interface.
struct TWJsonSigner {
    TW::Json::Signer impl;
};

