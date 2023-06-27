// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Oep4.h"

#include "../proto/Ontology.pb.h"

#include <TrustWalletCore/TWOntologyProto.h>

namespace TW::Ontology {

class Oep4TxBuilder {

public:
    static Data decimals(const Ontology::Proto::SigningInput& input);

    static Data balanceOf(const Ontology::Proto::SigningInput& input);

    static Data transfer(const Ontology::Proto::SigningInput& input);

    static Data build(const Ontology::Proto::SigningInput& input);

    static Transaction buildTx(const Ontology::Proto::SigningInput& input);
};

} // namespace TW::Ontology
