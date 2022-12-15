// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Everscale/Messages.h"

using namespace TW::Everscale;

namespace TW::TheOpenNetwork {

class Message : public Everscale::Message {
public:
    explicit Message(HeaderRef header) : Everscale::Message(std::move(header)) { }
    [[nodiscard]] Cell::Ref intoCell() const override;
};

} // namespace TW::TheOpenNetwork
