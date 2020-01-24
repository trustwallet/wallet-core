#pragma once

#include "../Data.h"

namespace TW::NEO {

class Script {
public:
  static Data CreateSignatureRedeemScript(Data publicKey);
  static Data CreateInvocationScript(Data signature);

};

} // namespace TW::NEO
