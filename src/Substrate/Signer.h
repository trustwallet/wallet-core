//
// Created by Fitz on 2022/2/7.
//

#pragma once

#include "../Data.h"
#include "../proto/Substrate.pb.h"

namespace TW::Substrate {

class Signer {
  public:
    explicit Signer();

    static Data signaturePreImage(const Proto::SigningInput &input);
    static Data encodeTransaction(const Proto::SigningInput &input, const Data &publicKey, const Data &signature);
};

}