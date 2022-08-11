#pragma once

#include <iterator>
#include <algorithm>

#include "HexCoding.h"
#include "Address.h"
#include "Asset.h"
#include "../Data.h"

namespace TW::Ontology {

class Oep : public Asset {
  private:
    static const uint8_t version = 0x00;

    Data contract;

  public:
    explicit Oep(const std::string& contract_addr) {
      contract = parse_hex(contract_addr);
      std::reverse(std::begin(contract), std::end(contract));
    };

    Data contractAddress() override { return contract; }

    Transaction decimals(uint32_t nonce) override;

    Transaction balanceOf(const Address &address, uint32_t nonce) override;

    Transaction transfer(const Signer &from, const Address &to, uint64_t amount,
                         const Signer &payer, uint64_t gasPrice, uint64_t gasLimit,
                         uint32_t nonce) override;

    Transaction unsignedTransfer(const Address &from, const Address &to, uint64_t amount,
                         uint64_t gasPrice, uint64_t gasLimit, uint32_t nonce) override;
};

} // namespace TW::Ontology