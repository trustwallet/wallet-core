// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "HexCoding.h"
#include "uint256.h"
#include "proto/Ethereum.pb.h"
#include "Ethereum/Address.h"
#include "Ethereum/ABI/Function.h"
#include "Ethereum/ABI/ParamBase.h"
#include "Ethereum/ABI/ParamAddress.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

namespace TW::Ethereum {

TEST(TWEthereumSigner, EmptyValue) {
    auto str = std::string("");
    uint256_t zero = load(str);

    ASSERT_EQ(zero, uint256_t(0));
}

TEST(TWEthereumSigner, BigInt) {
    // Check uint256_t loading
    Data expectedData = {0x52, 0x08};
    auto value = uint256_t(21000);
    auto loaded = load(expectedData);
    ASSERT_EQ(loaded, value);

    // Check proto storing
    Proto::SigningInput input;
    auto storedData = store(value);
    input.set_gas_limit(storedData.data(), storedData.size());
    ASSERT_EQ(hex(input.gas_limit()), hex(expectedData));

    // Check proto loading
    auto protoLoaded = load(input.gas_limit());
    ASSERT_EQ(protoLoaded, value);
}

TEST(TWAnySignerEthereum, Sign) {
    // from http://thetokenfactory.com/#/factory
    // https://etherscan.io/tx/0x63879f20909a315bcffe629bc03b20e5bc65ba2a377bd7152e3b69c4bd4cd6cc
    Proto::SigningInput input;
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(11));
    auto gasPrice = store(uint256_t(20000000000));
    auto gasLimit = store(uint256_t(1000000));
    auto data = parse_hex("0x60a060405260046060527f48302e31000000000000000000000000000000000000000000000000000000006080526006805460008290527f48302e310000000000000000000000000000000000000000000000000000000882556100b5907ff652222313e28459528d920b65115c16c04f3efc82aaedc97be59f3f377c0d3f602060026001841615610100026000190190931692909204601f01919091048101905b8082111561017957600081556001016100a1565b505060405161094b38038061094b83398101604052808051906020019091908051820191906020018051906020019091908051820191906020015050836000600050600033600160a060020a0316815260200190815260200160002060005081905550836002600050819055508260036000509080519060200190828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f1061017d57805160ff19168380011785555b506101ad9291506100a1565b5090565b8280016001018555821561016d579182015b8281111561016d57825182600050559160200191906001019061018f565b50506004805460ff19168317905560058054825160008390527f036b6384b5eca791c62761152d0c79bb0604c104a5fb6f4eb0703f3154bb3db0602060026001851615610100026000190190941693909304601f90810184900482019386019083901061022d57805160ff19168380011785555b5061025d9291506100a1565b82800160010185558215610221579182015b8281111561022157825182600050559160200191906001019061023f565b5050505050506106da806102716000396000f36060604052361561008d5760e060020a600035046306fdde038114610095578063095ea7b3146100f357806318160ddd1461016857806323b872dd14610171578063313ce5671461025c57806354fd4d501461026857806370a08231146102c657806395d89b41146102f4578063a9059cbb14610352578063cae9ca51146103f7578063dd62ed3e146105be575b6105f2610002565b6040805160038054602060026001831615610100026000190190921691909104601f81018290048202840182019094528383526105f493908301828280156106b75780601f1061068c576101008083540402835291602001916106b7565b61066260043560243533600160a060020a03908116600081815260016020908152604080832094871680845294825280832086905580518681529051929493927f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925929181900390910190a35060015b92915050565b6102e260025481565b610662600435602435604435600160a060020a0383166000908152602081905260408120548290108015906101c4575060016020908152604080832033600160a060020a03168452909152812054829010155b80156101d05750600082115b156106bf57600160a060020a0383811660008181526020818152604080832080548801905588851680845281842080548990039055600183528184203390961684529482529182902080548790039055815186815291519293927fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef9281900390910190a35060016106c3565b61067660045460ff1681565b6040805160068054602060026001831615610100026000190190921691909104601f81018290048202840182019094528383526105f493908301828280156106b75780601f1061068c576101008083540402835291602001916106b7565b600160a060020a03600435166000908152602081905260409020545b60408051918252519081900360200190f35b6105f46005805460408051602060026001851615610100026000190190941693909304601f810184900484028201840190925281815292918301828280156106b75780601f1061068c576101008083540402835291602001916106b7565b61066260043560243533600160a060020a03166000908152602081905260408120548290108015906103845750600082115b156106ca5733600160a060020a0390811660008181526020818152604080832080548890039055938716808352918490208054870190558351868152935191937fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef929081900390910190a3506001610162565b604080516020604435600481810135601f810184900484028501840190955284845261066294813594602480359593946064949293910191819084018382808284375094965050505050505033600160a060020a03908116600081815260016020908152604080832094881680845294825280832087905580518781529051929493927f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925929181900390910190a383600160a060020a031660405180807f72656365697665417070726f76616c28616464726573732c75696e743235362c81526020017f616464726573732c627974657329000000000000000000000000000000000000815260200150602e019050604051809103902060e060020a9004338530866040518560e060020a0281526004018085600160a060020a0316815260200184815260200183600160a060020a031681526020018280519060200190808383829060006004602084601f0104600f02600301f150905090810190601f1680156105965780820380516001836020036101000a031916815260200191505b509450505050506000604051808303816000876161da5a03f19250505015156106d257610002565b6102e2600435602435600160a060020a03828116600090815260016020908152604080832093851683529290522054610162565b005b60405180806020018281038252838181518152602001915080519060200190808383829060006004602084601f0104600f02600301f150905090810190601f1680156106545780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b604080519115158252519081900360200190f35b6040805160ff9092168252519081900360200190f35b820191906000526020600020905b81548152906001019060200180831161069a57829003601f168201915b505050505081565b5060005b9392505050565b506000610162565b5060016106c35600000000000000000000000000000000000000000000000000000000000003e80000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000c0000000000000000000000000000000000000000000000000000000000000000754204275636b73000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003544f540000000000000000000000000000000000000000000000000000000000");
    auto key = parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    // tx_mode not set, Legacy is the default
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_private_key(key.data(), key.size());
    auto& transfer = *input.mutable_transaction()->mutable_contract_generic();
    transfer.set_data(data.data(), data.size());

    std::string expected = "f90a9e0b8504a817c800830f42408080b90a4b60a060405260046060527f48302e31000000000000000000000000000000000000000000000000000000006080526006805460008290527f48302e310000000000000000000000000000000000000000000000000000000882556100b5907ff652222313e28459528d920b65115c16c04f3efc82aaedc97be59f3f377c0d3f602060026001841615610100026000190190931692909204601f01919091048101905b8082111561017957600081556001016100a1565b505060405161094b38038061094b83398101604052808051906020019091908051820191906020018051906020019091908051820191906020015050836000600050600033600160a060020a0316815260200190815260200160002060005081905550836002600050819055508260036000509080519060200190828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f1061017d57805160ff19168380011785555b506101ad9291506100a1565b5090565b8280016001018555821561016d579182015b8281111561016d57825182600050559160200191906001019061018f565b50506004805460ff19168317905560058054825160008390527f036b6384b5eca791c62761152d0c79bb0604c104a5fb6f4eb0703f3154bb3db0602060026001851615610100026000190190941693909304601f90810184900482019386019083901061022d57805160ff19168380011785555b5061025d9291506100a1565b82800160010185558215610221579182015b8281111561022157825182600050559160200191906001019061023f565b5050505050506106da806102716000396000f36060604052361561008d5760e060020a600035046306fdde038114610095578063095ea7b3146100f357806318160ddd1461016857806323b872dd14610171578063313ce5671461025c57806354fd4d501461026857806370a08231146102c657806395d89b41146102f4578063a9059cbb14610352578063cae9ca51146103f7578063dd62ed3e146105be575b6105f2610002565b6040805160038054602060026001831615610100026000190190921691909104601f81018290048202840182019094528383526105f493908301828280156106b75780601f1061068c576101008083540402835291602001916106b7565b61066260043560243533600160a060020a03908116600081815260016020908152604080832094871680845294825280832086905580518681529051929493927f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925929181900390910190a35060015b92915050565b6102e260025481565b610662600435602435604435600160a060020a0383166000908152602081905260408120548290108015906101c4575060016020908152604080832033600160a060020a03168452909152812054829010155b80156101d05750600082115b156106bf57600160a060020a0383811660008181526020818152604080832080548801905588851680845281842080548990039055600183528184203390961684529482529182902080548790039055815186815291519293927fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef9281900390910190a35060016106c3565b61067660045460ff1681565b6040805160068054602060026001831615610100026000190190921691909104601f81018290048202840182019094528383526105f493908301828280156106b75780601f1061068c576101008083540402835291602001916106b7565b600160a060020a03600435166000908152602081905260409020545b60408051918252519081900360200190f35b6105f46005805460408051602060026001851615610100026000190190941693909304601f810184900484028201840190925281815292918301828280156106b75780601f1061068c576101008083540402835291602001916106b7565b61066260043560243533600160a060020a03166000908152602081905260408120548290108015906103845750600082115b156106ca5733600160a060020a0390811660008181526020818152604080832080548890039055938716808352918490208054870190558351868152935191937fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef929081900390910190a3506001610162565b604080516020604435600481810135601f810184900484028501840190955284845261066294813594602480359593946064949293910191819084018382808284375094965050505050505033600160a060020a03908116600081815260016020908152604080832094881680845294825280832087905580518781529051929493927f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925929181900390910190a383600160a060020a031660405180807f72656365697665417070726f76616c28616464726573732c75696e743235362c81526020017f616464726573732c627974657329000000000000000000000000000000000000815260200150602e019050604051809103902060e060020a9004338530866040518560e060020a0281526004018085600160a060020a0316815260200184815260200183600160a060020a031681526020018280519060200190808383829060006004602084601f0104600f02600301f150905090810190601f1680156105965780820380516001836020036101000a031916815260200191505b509450505050506000604051808303816000876161da5a03f19250505015156106d257610002565b6102e2600435602435600160a060020a03828116600090815260016020908152604080832093851683529290522054610162565b005b60405180806020018281038252838181518152602001915080519060200190808383829060006004602084601f0104600f02600301f150905090810190601f1680156106545780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b604080519115158252519081900360200190f35b6040805160ff9092168252519081900360200190f35b820191906000526020600020905b81548152906001019060200180831161069a57829003601f168201915b505050505081565b5060005b9392505050565b506000610162565b5060016106c35600000000000000000000000000000000000000000000000000000000000003e80000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000c0000000000000000000000000000000000000000000000000000000000000000754204275636b73000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003544f54000000000000000000000000000000000000000000000000000000000026a042556c4f2a3f4e4e639cca524d1da70e60881417d4643e5382ed110a52719eafa0172f591a2a763d0bd6b13d042d8c5eb66e87f129c9dc77ada66b6041012db2b3";

    {
        // sign test
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEthereum);

        ASSERT_EQ(hex(output.encoded()), expected);
        ASSERT_EQ(hex(output.data()), hex(data));
    }
}

TEST(TWAnySignerEthereum, SignERC20TransferAsERC20) {
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(0));
    auto gasPrice = store(uint256_t(42000000000)); // 0x09c7652400
    auto gasLimit = store(uint256_t(78009));       // 130B9
    auto toAddress = "0x5322b34c88ed0691971bf52a7047448f0f4efc84";
    auto token = "0x6b175474e89094c44da98b954eedeac495271d0f"; // DAI
    auto amount = uint256_t(2000000000000000000);
    auto amountData = store(amount);
    auto key = parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151");

    Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    // tx_mode not set, Legacy is the default
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_to_address(token);
    input.set_private_key(key.data(), key.size());
    auto& erc20 = *input.mutable_transaction()->mutable_erc20_transfer();
    erc20.set_to(toAddress);
    erc20.set_amount(amountData.data(), amountData.size());

    // https://etherscan.io/tx/0x199a7829fc5149e49b452c2cab76d8fa5a9682fee6e4891b8acb697ac142513e
    std::string expected = "f8aa808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec8000025a0724c62ad4fbf47346b02de06e603e013f26f26b56fdc0be7ba3d6273401d98cea0032131cae15da7ddcda66963e8bef51ca0d9962bfef0547d3f02597a4a58c931";

    // sign test
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    ASSERT_EQ(hex(output.encoded()), expected);

    // expected payload
    Data payload;
    {
        auto func = ABI::Function("transfer", std::vector<std::shared_ptr<ABI::ParamBase>>{
                                             std::make_shared<ABI::ParamAddress>(parse_hex(toAddress)),
                                             std::make_shared<ABI::ParamUInt256>(amount)});
        func.encode(payload);
    }
    ASSERT_EQ(hex(output.data()), hex(payload));
    ASSERT_EQ(hex(output.data()), "a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000");
}

TEST(TWAnySignerEthereum, SignERC20TransferAsGenericContract) {
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(0));
    auto gasPrice = store(uint256_t(42000000000));                 // 0x09c7652400
    auto gasLimit = store(uint256_t(78009));                       // 130B9
    auto toAddress = "0x6b175474e89094c44da98b954eedeac495271d0f"; // DAI
    // payload: transfer(0x5322b34c88ed0691971bf52a7047448f0f4efc84, 2000000000000000000)
    auto data = parse_hex("0xa9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000");
    auto key = parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151");

    Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    // tx_mode not set, Legacy is the default
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_to_address(toAddress);
    input.set_private_key(key.data(), key.size());
    auto& transfer = *input.mutable_transaction()->mutable_contract_generic();
    transfer.set_data(data.data(), data.size());

    // https://etherscan.io/tx/0x199a7829fc5149e49b452c2cab76d8fa5a9682fee6e4891b8acb697ac142513e
    std::string expected = "f8aa808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec8000025a0724c62ad4fbf47346b02de06e603e013f26f26b56fdc0be7ba3d6273401d98cea0032131cae15da7ddcda66963e8bef51ca0d9962bfef0547d3f02597a4a58c931";

    // sign test
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    ASSERT_EQ(hex(output.encoded()), expected);
    ASSERT_EQ(hex(output.data()), hex(data));
}

TEST(TWAnySignerEthereum, SignERC20TransferInvalidAddress) {
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(0));
    auto gasPrice = store(uint256_t(42000000000)); // 0x09c7652400
    auto gasLimit = store(uint256_t(78009));       // 130B9
    auto invalidAddress = "0xdeadbeef";
    auto amount = store(uint256_t(2000000000000000000));
    auto key = parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151");

    Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    // tx_mode not set, Legacy is the default
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_to_address(invalidAddress);
    input.set_private_key(key.data(), key.size());
    auto& erc20 = *input.mutable_transaction()->mutable_erc20_transfer();
    erc20.set_to(invalidAddress);
    erc20.set_amount(amount.data(), amount.size());

    // sign test
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    ASSERT_EQ(hex(output.encoded()), "");
}

TEST(TWAnySignerEthereum, SignERC20Approve) {
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(0));
    auto gasPrice = store(uint256_t(42000000000)); // 0x09c7652400
    auto gasLimit = store(uint256_t(78009));       // 130B9
    auto spenderAddress = "0x5322b34c88ed0691971bf52a7047448f0f4efc84";
    auto token = "0x6b175474e89094c44da98b954eedeac495271d0f"; // DAI
    auto amount = store(uint256_t(2000000000000000000));
    auto key = parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151");

    Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    // tx_mode not set, Legacy is the default
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_to_address(token);
    input.set_private_key(key.data(), key.size());
    auto& erc20 = *input.mutable_transaction()->mutable_erc20_approve();
    erc20.set_spender(spenderAddress);
    erc20.set_amount(amount.data(), amount.size());

    std::string expected = "f8aa808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844095ea7b30000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec8000025a0d8136d66da1e0ba8c7208d5c4f143167f54b89a0fe2e23440653bcca28b34dc1a049222a79339f1a9e4641cb4ad805c49c225ae704299ffc10627bf41c035c464a";

    // sign test
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    ASSERT_EQ(hex(output.encoded()), expected);
}

TEST(TWAnySignerEthereum, SignERC721Transfer) {
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(0));
    auto gasPrice = store(uint256_t(42000000000));
    auto gasLimit = store(uint256_t(78009));
    auto tokenContract = "0x4e45e92ed38f885d39a733c14f1817217a89d425";
    auto fromAddress = "0x718046867b5b1782379a14eA4fc0c9b724DA94Fc";
    auto toAddress = "0x5322b34c88ed0691971bf52a7047448f0f4efc84";
    auto tokenId = parse_hex("23c47ee5");
    auto key = parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151");

    Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    // tx_mode not set, Legacy is the default
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_to_address(tokenContract);
    input.set_private_key(key.data(), key.size());
    auto& erc721 = *input.mutable_transaction()->mutable_erc721_transfer();
    erc721.set_from(fromAddress);
    erc721.set_to(toAddress);
    erc721.set_token_id(tokenId.data(), tokenId.size());

    std::string expected = "f8ca808509c7652400830130b9944e45e92ed38f885d39a733c14f1817217a89d42580b86423b872dd000000000000000000000000718046867b5b1782379a14ea4fc0c9b724da94fc0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000000000023c47ee526a0d38440a4dc140a4100d301eb49fcc35b64439e27d1d8dd9b55823dca04e6e659a03b5f56a57feabc3406f123d6f8198cd7d7e2ced7e2d58d375f076952ecd9ce88";

    // sign test
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    ASSERT_EQ(hex(output.encoded()), expected);
    ASSERT_EQ(hex(output.data()), "23b872dd000000000000000000000000718046867b5b1782379a14ea4fc0c9b724da94fc0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000000000023c47ee5");
}

TEST(TWAnySignerEthereum, SignERC1155Transfer) {
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(0));
    auto gasPrice = store(uint256_t(42000000000));
    auto gasLimit = store(uint256_t(78009));
    auto tokenContract = "0x4e45e92ed38f885d39a733c14f1817217a89d425";
    auto fromAddress = "0x718046867b5b1782379a14eA4fc0c9b724DA94Fc";
    auto toAddress = "0x5322b34c88ed0691971bf52a7047448f0f4efc84";
    auto tokenId = parse_hex("23c47ee5");
    auto value = uint256_t(2000000000000000000);
    auto valueData = store(value);
    auto data = parse_hex("01020304");
    auto key = parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151");

    Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    // tx_mode not set, Legacy is the default
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_to_address(tokenContract);
    input.set_private_key(key.data(), key.size());
    auto& erc1155 = *input.mutable_transaction()->mutable_erc1155_transfer();
    erc1155.set_from(fromAddress);
    erc1155.set_to(toAddress);
    erc1155.set_token_id(tokenId.data(), tokenId.size());
    erc1155.set_value(valueData.data(), valueData.size());
    erc1155.set_data(data.data(), data.size());

    std::string expected = "f9014a808509c7652400830130b9944e45e92ed38f885d39a733c14f1817217a89d42580b8e4f242432a000000000000000000000000718046867b5b1782379a14ea4fc0c9b724da94fc0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000000000023c47ee50000000000000000000000000000000000000000000000001bc16d674ec8000000000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000000000004010203040000000000000000000000000000000000000000000000000000000026a010315488201ac801ce346bffd1570de147615462d7e7db3cf08cf558465c6b79a06643943b24593bc3904a9fda63bb169881730994c973ab80f07d66a698064573";

    // sign test
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    ASSERT_EQ(hex(output.encoded()), expected);

    // expected payload
    Data payload;
    {
        auto func = ABI::Function("safeTransferFrom", std::vector<std::shared_ptr<ABI::ParamBase>>{
                                                     std::make_shared<ABI::ParamAddress>(parse_hex(fromAddress)),
                                                     std::make_shared<ABI::ParamAddress>(parse_hex(toAddress)),
                                                     std::make_shared<ABI::ParamUInt256>(uint256_t(0x23c47ee5)),
                                                     std::make_shared<ABI::ParamUInt256>(value),
                                                     std::make_shared<ABI::ParamByteArray>(data)});
        func.encode(payload);
    }
    ASSERT_EQ(hex(output.data()), hex(payload));
    ASSERT_EQ(hex(output.data()), "f242432a000000000000000000000000718046867b5b1782379a14ea4fc0c9b724da94fc0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000000000023c47ee50000000000000000000000000000000000000000000000001bc16d674ec8000000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000040102030400000000000000000000000000000000000000000000000000000000");
}

// EIP4337

TEST(TWAnySignerEthereum, EIP4337_SignTransferAccountNotDeployed) {
    // https://goerli.etherscan.io/tx/0xf4e9c9899da7d083f260fd8d0d326a6a0e965f03444a32c73e30cd30ccc609f7
    Proto::SigningInput input;
    auto chainId = store(uint256_t(5));
    auto nonce = store(uint256_t(0));
    auto amount = store(uint256_t(0x2386f26fc10000));
    auto gasLimit = store(uint256_t(0x5580));
    auto verificationGasLimit = store(uint256_t(0x073272));
    auto maxFeePerGas = store(uint256_t(0x01952f1f85));
    auto maxInclusionFeePerGas = store(uint256_t(0x0f));
    auto preVerificationGas = store(uint256_t(0xbc18));
    auto entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53";
    auto factory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92";
    auto logic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620";
    auto owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238";
    auto to = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a";

    auto key = parse_hex("0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_tx_mode(Proto::TransactionMode::UserOp);
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    input.set_to_address(to);

    auto& user_operation = *input.mutable_user_operation();
    user_operation.set_verification_gas_limit(verificationGasLimit.data(), verificationGasLimit.size());
    user_operation.set_pre_verification_gas(preVerificationGas.data(), preVerificationGas.size());
    user_operation.set_is_account_deployed(false);
    user_operation.set_entry_point(entryPoint);
    user_operation.set_account_factory(factory);
    user_operation.set_account_logic(logic);
    user_operation.set_owner(owner);

    input.set_private_key(key.data(), key.size());
    auto& transfer = *input.mutable_transaction()->mutable_transfer();
    transfer.set_amount(amount.data(), amount.size());

    std::string expected = "{\"callData\":\"0xb61d27f6000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a000000000000000000000000000000000000000000000000002386f26fc1000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"21888\",\"initCode\":\"0x5a87209b755781cf65feeedd3855ade0317f4a925fbfb9cf00000000000000000000000078d9c32b96bb872d66d51818227563f44e67e2380000000000000000000000000000000000000000000000000000000000000000\",\"maxFeePerGas\":\"6797860741\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"0\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"48152\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0x1560b19d17613ec8580cb0feaf7ac2953771404c5bd7830f585e5062e6ddd4b82ae3bb8dbddb659c0300e8009857b5c77501e1cfd5bbab48d03de0ea7207d07c1b\",\"verificationGasLimit\":\"471666\"}";
    {
        // sign test
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEthereum);

        ASSERT_EQ(std::string(output.encoded()), expected);
    }
}

TEST(TWAnySignerEthereum, EIP4337_SignTransferAccountDeployed) {
    // https://goerli.etherscan.io/tx/0x707ee622b87a35eb2ffc3762553db8ba0efc5053cfdbeb44a841562df2a7c2bf
    Proto::SigningInput input;
    auto chainId = store(uint256_t(5));
    auto nonce = store(uint256_t(1));
    auto amount = store(uint256_t(0x2386f26fc10000));
    auto gasLimit = store(uint256_t(0x9d55));
    auto verificationGasLimit = store(uint256_t(0x186a0));
    auto maxFeePerGas = store(uint256_t(0x1a339c9e9));
    auto maxInclusionFeePerGas = store(uint256_t(0xf));
    auto preVerificationGas = store(uint256_t(0xb708));
    auto entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53";
    auto factory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92";
    auto logic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620";
    auto owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238";
    auto to = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a";

    auto key = parse_hex("0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_tx_mode(Proto::TransactionMode::UserOp);
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    input.set_to_address(to);

    auto& user_operation = *input.mutable_user_operation();
    user_operation.set_verification_gas_limit(verificationGasLimit.data(), verificationGasLimit.size());
    user_operation.set_pre_verification_gas(preVerificationGas.data(), preVerificationGas.size());
    user_operation.set_is_account_deployed(true);
    user_operation.set_entry_point(entryPoint);
    user_operation.set_account_factory(factory);
    user_operation.set_account_logic(logic);
    user_operation.set_owner(owner);

    input.set_private_key(key.data(), key.size());
    auto& transfer = *input.mutable_transaction()->mutable_transfer();
    transfer.set_amount(amount.data(), amount.size());

    std::string expected = "{\"callData\":\"0xb61d27f6000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a000000000000000000000000000000000000000000000000002386f26fc1000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"40277\",\"initCode\":\"0x\",\"maxFeePerGas\":\"7033440745\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"1\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"46856\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0xaed2011e5cf267de495b38ecf86ad6f1d4c05217a99e59f47e8d52ba3d41c10144785893fa3e7c116a054999e3902fc2771064d0545148bc49f6d7c827fc7a9a1c\",\"verificationGasLimit\":\"100000\"}";

    {
        // sign test
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEthereum);

        ASSERT_EQ(std::string(output.encoded()), expected);
    }
}

TEST(TWAnySignerEthereum, EIP4337_SignERC20TransferAccountDeployed) {
    // https://goerli.etherscan.io/tx/0xe1c45f163cec0b9b2c3fd2307f75e48337d35c7c336ad96675c0adb6ce6fc58f
    Proto::SigningInput input;
    auto chainId = store(uint256_t(5));
    auto nonce = store(uint256_t(6));
    auto amount = store(uint256_t(0x186a0));
    auto gasLimit = store(uint256_t(0xf78e));
    auto verificationGasLimit = store(uint256_t(0x186a0));
    auto maxFeePerGas = store(uint256_t(0x168ad5950f));
    auto maxInclusionFeePerGas = store(uint256_t(0xf));
    auto preVerificationGas = store(uint256_t(0xbb10));
    auto entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53";
    auto factory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92";
    auto logic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620";
    auto owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238";
    auto tokenContract = "0x98339d8c260052b7ad81c28c16c0b98420f2b46a";
    auto to = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a";

    auto key = parse_hex("0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_tx_mode(Proto::TransactionMode::UserOp);
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    input.set_to_address(tokenContract);

    auto& user_operation = *input.mutable_user_operation();
    user_operation.set_verification_gas_limit(verificationGasLimit.data(), verificationGasLimit.size());
    user_operation.set_pre_verification_gas(preVerificationGas.data(), preVerificationGas.size());
    user_operation.set_is_account_deployed(true);
    user_operation.set_entry_point(entryPoint);
    user_operation.set_account_factory(factory);
    user_operation.set_account_logic(logic);
    user_operation.set_owner(owner);

    input.set_private_key(key.data(), key.size());
    auto& transfer = *input.mutable_transaction()->mutable_erc20_transfer();
    transfer.set_amount(amount.data(), amount.size());
    transfer.set_to(to);

    std::string expected = "{\"callData\":\"0xb61d27f600000000000000000000000098339d8c260052b7ad81c28c16c0b98420f2b46a000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044a9059cbb000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a00000000000000000000000000000000000000000000000000000000000186a000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"63374\",\"initCode\":\"0x\",\"maxFeePerGas\":\"96818533647\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"6\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"47888\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0xd006c93d6a8753b5e7c1e6349de0dea34eab2e7a533106e0f2e1a3a3b013c8e97b007546dab9d7b8fc471ad14ff2e8aa351dc4f1ecb63bf20f33858dc7366cbe1c\",\"verificationGasLimit\":\"100000\"}";
    {
        // sign test
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEthereum);

        ASSERT_EQ(std::string(output.encoded()), expected);
    }
}

TEST(TWAnySignerEthereum, EIP4337_SignERC20ApproveAccountDeployed) {
    // https://goerli.etherscan.io/tx/0xe1c45f163cec0b9b2c3fd2307f75e48337d35c7c336ad96675c0adb6ce6fc58f
    Proto::SigningInput input;
    auto chainId = store(uint256_t(5));
    auto nonce = store(uint256_t(9));
    auto amount = store(uint256_t(0x186a0));
    auto gasLimit = store(uint256_t(0xf78e));
    auto verificationGasLimit = store(uint256_t(0x186a0));
    auto maxFeePerGas = store(uint256_t(0x168ad5950f));
    auto maxInclusionFeePerGas = store(uint256_t(0xf));
    auto preVerificationGas = store(uint256_t(0xbb10));
    auto entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53";
    auto factory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92";
    auto logic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620";
    auto owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238";
    auto tokenContract = "0x98339d8c260052b7ad81c28c16c0b98420f2b46a";
    auto to = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a";

    auto key = parse_hex("0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_tx_mode(Proto::TransactionMode::UserOp);
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    input.set_to_address(tokenContract);

    auto& user_operation = *input.mutable_user_operation();
    user_operation.set_verification_gas_limit(verificationGasLimit.data(), verificationGasLimit.size());
    user_operation.set_pre_verification_gas(preVerificationGas.data(), preVerificationGas.size());
    user_operation.set_is_account_deployed(true);
    user_operation.set_entry_point(entryPoint);
    user_operation.set_account_factory(factory);
    user_operation.set_account_logic(logic);
    user_operation.set_owner(owner);

    input.set_private_key(key.data(), key.size());
    auto& transfer = *input.mutable_transaction()->mutable_erc20_approve();
    transfer.set_amount(amount.data(), amount.size());
    transfer.set_spender(to);

    std::string expected = "{\"callData\":\"0xb61d27f600000000000000000000000098339d8c260052b7ad81c28c16c0b98420f2b46a000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044095ea7b3000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a00000000000000000000000000000000000000000000000000000000000186a000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"63374\",\"initCode\":\"0x\",\"maxFeePerGas\":\"96818533647\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"9\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"47888\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0x262a67dd8cf3d16a72b7809b3b5ed55e9f4c2b93eedd5a3c6be035fbbd7111164464ec933d0fdfa359e266e318f3ac22702ae428ce14fc142e4475603e6ec15e1c\",\"verificationGasLimit\":\"100000\"}";
    {
        // sign test
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEthereum);

        ASSERT_EQ(std::string(output.encoded()), expected);
    }
}

TEST(TWAnySignerEthereum, EIP4337_SignERC721TransferAccountDeployed) {
    // https://goerli.etherscan.io/tx/0x043f7cf4858c8480c8abbd5a5485bee11c8e70e7c6b87825a78eef00a42b662d
    Proto::SigningInput input;
    auto chainId = store(uint256_t(5));
    auto nonce = store(uint256_t(12));
    auto tokenId = store(uint256_t(0x2A8E57));
    auto gasLimit = store(uint256_t(6337400));
    auto verificationGasLimit = store(uint256_t(1500000));
    auto maxFeePerGas = store(uint256_t(0x168ad5950f));
    auto maxInclusionFeePerGas = store(uint256_t(0xf));
    auto preVerificationGas = store(uint256_t(49999));
    auto entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53";
    auto factory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92";
    auto logic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620";
    auto owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238";
    auto tokenContract = "0xf5de760f2e916647fd766b4ad9e85ff943ce3a2b";
    auto to = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a";
    auto from = "0x8cE23B8769ac01d0df0d5f47Be1A38FeA97F3879";

    auto key = parse_hex("0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_tx_mode(Proto::TransactionMode::UserOp);
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    input.set_to_address(tokenContract);

    auto& user_operation = *input.mutable_user_operation();
    user_operation.set_verification_gas_limit(verificationGasLimit.data(), verificationGasLimit.size());
    user_operation.set_pre_verification_gas(preVerificationGas.data(), preVerificationGas.size());
    user_operation.set_is_account_deployed(true);
    user_operation.set_entry_point(entryPoint);
    user_operation.set_account_factory(factory);
    user_operation.set_account_logic(logic);
    user_operation.set_owner(owner);

    input.set_private_key(key.data(), key.size());
    auto& transfer = *input.mutable_transaction()->mutable_erc721_transfer();
    transfer.set_token_id(tokenId.data(), tokenId.size());
    transfer.set_to(to);
    transfer.set_from(from);

    std::string expected = "{\"callData\":\"0xb61d27f6000000000000000000000000f5de760f2e916647fd766b4ad9e85ff943ce3a2b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000006423b872dd0000000000000000000000008ce23b8769ac01d0df0d5f47be1a38fea97f3879000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a00000000000000000000000000000000000000000000000000000000002a8e5700000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"6337400\",\"initCode\":\"0x\",\"maxFeePerGas\":\"96818533647\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"12\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"49999\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0x5951cc161a4d60d6b59503efb93e446f5d1a2e3a41d4503ba6393bcf2a2637340d0a865ed5d4d7650a68cbb95915eaa7ed54fd2c42b4bf7c83376f5c5d70691d1b\",\"verificationGasLimit\":\"1500000\"}";
    {
        // sign test
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEthereum);

        ASSERT_EQ(std::string(output.encoded()), expected);
    }
}

TEST(TWAnySignerEthereum, EIP4337_SignERC1155TransferAccountDeployed) {
    // https://goerli.etherscan.io/tx/0x6e9875715f2e46dfb45f6d7ba15dc8bd1561abb3ae7d19e549929835fca5f6af
    Proto::SigningInput input;
    auto chainId = store(uint256_t(5));
    auto nonce = store(uint256_t(0));
    auto tokenId = store(uint256_t(0x01));
    auto gasLimit = store(uint256_t(6337400));
    auto verificationGasLimit = store(uint256_t(1500000));
    auto maxFeePerGas = store(uint256_t(0x168ad5950f));
    auto maxInclusionFeePerGas = store(uint256_t(0xf));
    auto preVerificationGas = store(uint256_t(51000));
    auto entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53";
    auto factory = "0x76627b8D1E01fAF0C73B69625BC1fCb8FA19a2AD";
    auto logic = "0x510ab68bd111ce7115df797118b0334d727d564b";
    auto owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238";
    auto tokenContract = "0x428ce4b916332e1afccfddce08baecc97cb40b12";
    auto to = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a";
    auto from = "0x8c560E00680b973645900528EDe71a99b8d4dca8";

    auto key = parse_hex("0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_tx_mode(Proto::TransactionMode::UserOp);
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    input.set_to_address(tokenContract);

    auto& user_operation = *input.mutable_user_operation();
    user_operation.set_verification_gas_limit(verificationGasLimit.data(), verificationGasLimit.size());
    user_operation.set_pre_verification_gas(preVerificationGas.data(), preVerificationGas.size());
    user_operation.set_is_account_deployed(true);
    user_operation.set_entry_point(entryPoint);
    user_operation.set_account_factory(factory);
    user_operation.set_account_logic(logic);
    user_operation.set_owner(owner);

    input.set_private_key(key.data(), key.size());
    auto& transfer = *input.mutable_transaction()->mutable_erc1155_transfer();
    transfer.set_token_id(tokenId.data(), tokenId.size());
    transfer.set_to(to);
    transfer.set_from(from);

    std::string expected = "{\"callData\":\"0xb61d27f6000000000000000000000000428ce4b916332e1afccfddce08baecc97cb40b120000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000c4f242432a0000000000000000000000008c560e00680b973645900528ede71a99b8d4dca8000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a0000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"6337400\",\"initCode\":\"0x\",\"maxFeePerGas\":\"96818533647\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"0\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"51000\",\"sender\":\"0x8c560e00680b973645900528ede71a99b8d4dca8\",\"signature\":\"0xaae38bcf9f946921541b44c2a66596968beecb9420471e2c9c531f758a2d652930ffdeeab95742e57e8520fb5c8ca4fee6a8e47e37336d4201fe104103f85e111c\",\"verificationGasLimit\":\"1500000\"}";
    {
        // sign test
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEthereum);

        ASSERT_EQ(std::string(output.encoded()), expected);
    }
}

TEST(TWAnySignerEthereum, SignJSON) {
    auto json = STRING(R"({"chainId":"AQ==","gasPrice":"1pOkAA==","gasLimit":"Ugg=","toAddress":"0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1","transaction":{"transfer":{"amount":"A0i8paFgAA=="}}})");
    auto key = DATA("17209af590a86462395d5881e60d11c7fa7d482cfb02b5a01b93c2eeef243543");
    auto result = WRAPS(TWAnySignerSignJSON(json.get(), key.get(), TWCoinTypeEthereum));

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeEthereum));
    assertStringsEqual(result, "f86a8084d693a400825208947d8bf18c7ce84b3e175b339c4ca93aed1dd166f1870348bca5a160008025a0fe5802b49e04c6b1705088310e133605ed8b549811a18968ad409ea02ad79f21a05bf845646fb1e1b9365f63a7fd5eb5e984094e3ed35c3bed7361aebbcbf41f10");
}

TEST(TWAnySignerEthereum, PlanNotSupported) {
    // Ethereum does not use plan(), call it nonetheless
    Proto::SigningInput input;
    auto inputData = input.SerializeAsString();
    auto inputTWData = WRAPD(TWDataCreateWithBytes((const uint8_t*)inputData.data(), inputData.size()));
    auto outputTWData = WRAPD(TWAnySignerPlan(inputTWData.get(), TWCoinTypeEthereum));
    EXPECT_EQ(TWDataSize(outputTWData.get()), 0ul);
}

TEST(TWAnySignerEthereum, SignERC1559Transfer_1442) {
    auto chainId = store(uint256_t(3));
    auto nonce = store(uint256_t(6));
    auto gasLimit = store(uint256_t(21100));
    auto maxInclusionFeePerGas = store(uint256_t(2000000000));
    auto maxFeePerGas = store(uint256_t(3000000000));
    auto toAddress = "0xB9F5771C27664bF2282D98E09D7F50cEc7cB01a7";
    auto value = uint256_t(543210987654321);
    auto valueData = store(value);
    auto key = parse_hex("4f96ed80e9a7555a6f74b3d658afdd9c756b0a40d4ca30c42c2039eb449bb904");

    Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_tx_mode(Proto::TransactionMode::Enveloped); // EIP1559
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_to_address(toAddress);
    input.set_private_key(key.data(), key.size());
    auto& transfer = *input.mutable_transaction()->mutable_transfer();
    transfer.set_amount(valueData.data(), valueData.size());
    transfer.set_data(Data().data(), 0);

    // https://ropsten.etherscan.io/tx/0x14429509307efebfdaa05227d84c147450d168c68539351fbc01ed87c916ab2e
    std::string expected = "02f8710306847735940084b2d05e0082526c94b9f5771c27664bf2282d98e09d7f50cec7cb01a78701ee0c29f50cb180c080a092c336138f7d0231fe9422bb30ee9ef10bf222761fe9e04442e3a11e88880c64a06487026011dae03dc281bc21c7d7ede5c2226d197befb813a4ecad686b559e58";

    // sign test
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    EXPECT_EQ(hex(output.encoded()), expected);
    EXPECT_EQ(hex(output.v()), "00");
    EXPECT_EQ(hex(output.r()), "92c336138f7d0231fe9422bb30ee9ef10bf222761fe9e04442e3a11e88880c64");
    EXPECT_EQ(hex(output.s()), "6487026011dae03dc281bc21c7d7ede5c2226d197befb813a4ecad686b559e58");
    EXPECT_EQ(hex(output.data()), "");
}

TEST(TWAnySignerEthereum, SignERC20Transfer_1559) {
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(0));
    auto gasLimit = store(uint256_t(78009));                   // 130B9
    auto maxInclusionFeePerGas = store(uint256_t(2000000000)); // 77359400
    auto maxFeePerGas = store(uint256_t(3000000000));          // B2D05E00
    auto toAddress = "0x5322b34c88ed0691971bf52a7047448f0f4efc84";
    auto token = "0x6b175474e89094c44da98b954eedeac495271d0f"; // DAI
    auto amount = uint256_t(2000000000000000000);
    auto amountData = store(amount);
    auto key = parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151");

    Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_tx_mode(Proto::TransactionMode::Enveloped);
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_to_address(token);
    input.set_private_key(key.data(), key.size());
    auto& erc20 = *input.mutable_transaction()->mutable_erc20_transfer();
    erc20.set_to(toAddress);
    erc20.set_amount(amountData.data(), amountData.size());

    // sign test
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    ASSERT_EQ(hex(output.encoded()), "02f8b00180847735940084b2d05e00830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000c080a0adfcfdf98d4ed35a8967a0c1d78b42adb7c5d831cf5a3272654ec8f8bcd7be2ea011641e065684f6aa476f4fd250aa46cd0b44eccdb0a6e1650d658d1998684cdf");
}

TEST(TWAnySignerEthereum, SignERC20Approve_1559) {
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(0));
    auto gasLimit = store(uint256_t(78009)); // 130B9
    auto maxInclusionFeePerGas = store(uint256_t(2000000000));
    auto maxFeePerGas = store(uint256_t(3000000000));
    auto spenderAddress = "0x5322b34c88ed0691971bf52a7047448f0f4efc84";
    auto token = "0x6b175474e89094c44da98b954eedeac495271d0f"; // DAI
    auto amount = store(uint256_t(2000000000000000000));
    auto key = parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151");

    Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_tx_mode(Proto::TransactionMode::Enveloped);
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_to_address(token);
    input.set_private_key(key.data(), key.size());
    auto& erc20 = *input.mutable_transaction()->mutable_erc20_approve();
    erc20.set_spender(spenderAddress);
    erc20.set_amount(amount.data(), amount.size());

    // sign test
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    ASSERT_EQ(hex(output.encoded()), "02f8b00180847735940084b2d05e00830130b9946b175474e89094c44da98b954eedeac495271d0f80b844095ea7b30000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000c080a05a43dda3dc193480ee532a5ed67ba8fbd2e3afb9eee218f4fb955b415d592925a01300e5b5f51c8cd5bf80f018cea3fb347fae589e65355068ac44ffc996313c60");
}

TEST(TWAnySignerEthereum, SignERC721Transfer_1559) {
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(0));
    auto gasLimit = store(uint256_t(78009));
    auto maxInclusionFeePerGas = store(uint256_t(2000000000));
    auto maxFeePerGas = store(uint256_t(3000000000));
    auto tokenContract = "0x4e45e92ed38f885d39a733c14f1817217a89d425";
    auto fromAddress = "0x718046867b5b1782379a14eA4fc0c9b724DA94Fc";
    auto toAddress = "0x5322b34c88ed0691971bf52a7047448f0f4efc84";
    auto tokenId = parse_hex("23c47ee5");
    auto key = parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151");

    Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_tx_mode(Proto::TransactionMode::Enveloped);
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_to_address(tokenContract);
    input.set_private_key(key.data(), key.size());
    auto& erc721 = *input.mutable_transaction()->mutable_erc721_transfer();
    erc721.set_from(fromAddress);
    erc721.set_to(toAddress);
    erc721.set_token_id(tokenId.data(), tokenId.size());

    // sign test
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    ASSERT_EQ(hex(output.encoded()), "02f8d00180847735940084b2d05e00830130b9944e45e92ed38f885d39a733c14f1817217a89d42580b86423b872dd000000000000000000000000718046867b5b1782379a14ea4fc0c9b724da94fc0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000000000023c47ee5c080a0dbd591d1eac39bad62d7c158d5e1d55e7014d2218998f8980462e2f283f42d4aa05acadb904484a0fb5526a4c64b8addb8aac4f6548f90199e40eb787b79faed4a");
}

TEST(TWAnySignerEthereum, SignERC1155Transfer_1559) {
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(0));
    auto gasLimit = store(uint256_t(78009));
    auto maxInclusionFeePerGas = store(uint256_t(2000000000));
    auto maxFeePerGas = store(uint256_t(3000000000));
    auto tokenContract = "0x4e45e92ed38f885d39a733c14f1817217a89d425";
    auto fromAddress = "0x718046867b5b1782379a14eA4fc0c9b724DA94Fc";
    auto toAddress = "0x5322b34c88ed0691971bf52a7047448f0f4efc84";
    auto tokenId = parse_hex("23c47ee5");
    auto value = uint256_t(2000000000000000000);
    auto valueData = store(value);
    auto data = parse_hex("01020304");
    auto key = parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151");

    Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_tx_mode(Proto::TransactionMode::Enveloped);
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_to_address(tokenContract);
    input.set_private_key(key.data(), key.size());
    auto& erc1155 = *input.mutable_transaction()->mutable_erc1155_transfer();
    erc1155.set_from(fromAddress);
    erc1155.set_to(toAddress);
    erc1155.set_token_id(tokenId.data(), tokenId.size());
    erc1155.set_value(valueData.data(), valueData.size());
    erc1155.set_data(data.data(), data.size());

    // sign test
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    ASSERT_EQ(hex(output.encoded()), "02f901500180847735940084b2d05e00830130b9944e45e92ed38f885d39a733c14f1817217a89d42580b8e4f242432a000000000000000000000000718046867b5b1782379a14ea4fc0c9b724da94fc0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000000000023c47ee50000000000000000000000000000000000000000000000001bc16d674ec8000000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000040102030400000000000000000000000000000000000000000000000000000000c080a0533df41dda5540c57257b7fe89c29cefff0155c333e063220df2bf9680fcc15aa036a844fd20de5a51de96ceaaf078558e87d86426a4a5d4b215ee1fd0fa397f8a");
}

TEST(TWAnySignerEthereum, StakeRocketPool) {
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(1));
    auto gasPrice = store(uint256_t(2002000000));
    auto gasLimit = store(uint256_t(205000));
    auto maxFeePerGas = store(uint256_t(27900000000));
    auto maxInclusionFeePerGas = store(uint256_t(1000000000));
    auto toAddress = "0x2cac916b2a963bf162f076c0a8a4a8200bcfbfb4";
    auto key = parse_hex("9f56448d33de406db1561aae15fce64bdf0e9706ff15c45d4409e8fcbfd1a498");
    const auto pk = PrivateKey(key);

    // 0.01 ETH
    auto valueData = store(uint256_t(10000000000000000));

    Data payload;
    {
        auto func = ABI::Function("deposit", std::vector<std::shared_ptr<ABI::ParamBase>>{ });
        func.encode(payload);
    }


    Proto::SigningInput input;
    input.set_tx_mode(Proto::TransactionMode::Enveloped);

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());

    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());

    input.set_to_address(toAddress);
    input.set_private_key(key.data(), key.size());

    auto& transfer = *input.mutable_transaction()->mutable_transfer();
    transfer.set_amount(valueData.data(), valueData.size());
    transfer.set_data(payload.data(), payload.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    // https://etherscan.io/tx/0xfeba0c579f3e964fbc4eafa500e86891b9f4113735b1364edd4433d765506f1e
    EXPECT_EQ(hex(output.r()), "fb39e5079d7a0598ec45785d73a06b91fe1db707b9c6a150c87ffce2492c66d6");
    EXPECT_EQ(hex(output.v()), "00");
    EXPECT_EQ(hex(output.s()), "7fbd43a6f4733b2b4f98ad1bc4678ea2615f5edf56ad91408337adec2f07c0ac");
    EXPECT_EQ(hex(output.encoded()), "02f8770101843b9aca0085067ef83700830320c8942cac916b2a963bf162f076c0a8a4a8200bcfbfb4872386f26fc1000084d0e30db0c080a0fb39e5079d7a0598ec45785d73a06b91fe1db707b9c6a150c87ffce2492c66d6a07fbd43a6f4733b2b4f98ad1bc4678ea2615f5edf56ad91408337adec2f07c0ac");
}

TEST(TWAnySignerEthereum, UnstakeRocketPool) {
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(3));
    auto gasPrice = store(uint256_t(2002000000));
    auto gasLimit = store(uint256_t(350000));
    auto maxFeePerGas = store(uint256_t(27900000000));
    auto maxInclusionFeePerGas = store(uint256_t(1000000000));
    auto toAddress = "0xae78736Cd615f374D3085123A210448E74Fc6393";
    auto key = parse_hex("9f56448d33de406db1561aae15fce64bdf0e9706ff15c45d4409e8fcbfd1a498");
    const auto pk = PrivateKey(key);

    auto valueData = store(uint256_t(0));

    Data payload;
    {
        auto func = ABI::Function("burn", std::vector<std::shared_ptr<ABI::ParamBase>>{
                                                          std::make_shared<ABI::ParamUInt256>(uint256_t(0x21faa32ab2502b))});
        func.encode(payload);
    }

    Proto::SigningInput input;
    input.set_tx_mode(Proto::TransactionMode::Enveloped);

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());

    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());

    input.set_to_address(toAddress);
    input.set_private_key(key.data(), key.size());

    auto& transfer = *input.mutable_transaction()->mutable_contract_generic();
    transfer.set_amount(valueData.data(), valueData.size());
    transfer.set_data(payload.data(), payload.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEthereum);

    // https://etherscan.io/tx/0x7fd3c0e9b8b309b4258baa7677c60f5e00e8db7b647fbe3a52adda25058a4b37
    EXPECT_EQ(hex(output.r()), "1fc6e94908107584357799e952b4e3fb87f088aeb66d7930a7015643f19c9e7f");
    EXPECT_EQ(hex(output.v()), "00");
    EXPECT_EQ(hex(output.s()), "2c56a0b70ff2e52bf374a3dcd404bc42317d5ca15d319f5e33665352eb48f06f");
    EXPECT_EQ(hex(output.encoded()), "02f8900103843b9aca0085067ef837008305573094ae78736cd615f374d3085123a210448e74fc639380a442966c680000000000000000000000000000000000000000000000000021faa32ab2502bc080a01fc6e94908107584357799e952b4e3fb87f088aeb66d7930a7015643f19c9e7fa02c56a0b70ff2e52bf374a3dcd404bc42317d5ca15d319f5e33665352eb48f06f");
}


} // namespace TW::Ethereum
