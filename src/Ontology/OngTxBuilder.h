// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Ong.h"

#include "../proto/Ontology.pb.h"

#include <TrustWalletCore/TWOntologyProto.h>

namespace TW::Ontology {

class OngTxBuilder {

  public:
    static TW_Ontology_Proto_SigningOutput decimals();

    static TW_Ontology_Proto_SigningOutput balanceOf(const Ontology::Proto::SigningInput& input);

    static TW_Ontology_Proto_SigningOutput transfer(const Ontology::Proto::SigningInput& input);

    static TW_Ontology_Proto_SigningOutput withdraw(const Ontology::Proto::SigningInput& input);

    static TW_Ontology_Proto_SigningOutput build(const Ontology::Proto::SigningInput& input);
};

} // namespace TW::Ontology
