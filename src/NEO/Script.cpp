#include "Script.h"

namespace TW::NEO {

Data Script::CreateSignatureRedeemScript(Data publicKey) {
  Data result;
  result.push_back((byte)0x21); // size
  result.insert(result.end(), publicKey.begin(), publicKey.end());
  result.push_back((byte)0xAC); //CHECKSIG
  return result;
}

Data Script::CreateInvocationScript(Data signature) {
  Data result;
  result.push_back((byte)0x40); // size
  result.insert(result.end(), signature.begin(), signature.end());
  return result;
}

} // namespace TW::NEO
