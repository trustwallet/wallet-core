#include "proto/Terra.pb.h"
#include <gtest/gtest.h>
#include <HexCoding.h>
#include <Terra/Signer.h>

namespace TW::TERRA{

    TEST(TerraStaking, Staking) {
        auto input = Terra::Proto::SigningInput();
        input.set_chain_id("columbus-1");
        input.set_memo("");
        input.set_sequence(7);

        auto& message = *input.mutable_stake_message();
        message.set_delegator_address("terra1jyjg55hzsh0f4xymy0kuuan30pp4q75ru0h35c");
        message.set_validator_address("terravaloper1jyjg55hzsh0f4xymy0kuuan30pp4q75ruqmvyt");
        auto& amountOfTx = *message.mutable_amount();
        amountOfTx.set_denom("uluna");
        amountOfTx.set_amount(9000000);

        auto &fee = *input.mutable_fee();
        fee.set_gas(102459);
        auto amountOfFee = fee.add_amounts();
        amountOfFee->set_denom("uluna");
        amountOfFee->set_amount(3000);

        auto privateKey = parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646");
        input.set_private_key(privateKey.data(), privateKey.size());

        auto signer = TERRA::Signer(std::move(input));

        auto output = signer.build();

        ASSERT_EQ( "{\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"3000\",\"denom\":\"uluna\"}],\"gas\":\"102459\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgDelegate\",\"value\":{\"amount\":{\"amount\":\"9000000\",\"denom\":\"uluna\"},\"delegator_address\":\"terra1jyjg55hzsh0f4xymy0kuuan30pp4q75ru0h35c\",\"validator_address\":\"terravaloper1jyjg55hzsh0f4xymy0kuuan30pp4q75ruqmvyt\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AkvCoxJlFT8H5w4LqwhyTmuF4hf4zWKM62KXQke7STOC\"},\"signature\":\"o8hRdmRzcJhshXfCDVR+8o7PskZOxX8JHItxtPWd6HNIN551zOylvzc14hkbzrRB5S82+22MmCrgK2RwQlDgvA==\"}],\"type\":\"cosmos-sdk/MsgSend\"}}", output.json());
        ASSERT_EQ("a1627478a563666565a266616d6f756e7481a266616d6f756e7464333030306564656e6f6d65756c756e616367617366313032343539646d656d6f60636d736781a2647479706576636f736d6f732d73646b2f4d736744656c65676174656576616c7565a366616d6f756e74a266616d6f756e7467393030303030306564656e6f6d65756c756e617164656c656761746f725f61646472657373782c7465727261316a796a673535687a736830663478796d79306b7575616e3330707034713735727530683335637176616c696461746f725f616464726573737833746572726176616c6f706572316a796a673535687a736830663478796d79306b7575616e33307070347137357275716d7679746a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416b76436f784a6c465438483577344c71776879546d7546346866347a574b4d36324b58516b653753544f43697369676e617475726578586f386852646d527a634a6873685866434456522b386f3750736b5a4f7858384a484974787450576436484e494e3535317a4f796c767a633134686b627a725242355338322b32324d6d4372674b325277516c446776413d3d647479706572636f736d6f732d73646b2f4d736753656e64", hex(output.encoded()));
    }


    TEST(TerraSigner, Sign){
        auto input = Terra::Proto::SigningInput();
        input.set_chain_id("columbus-1");
        input.set_memo("sendtocold");
        input.set_sequence(8);

        auto &message = *input.mutable_send_coins_message();
        message.set_from_address("terra1v9ku44wycfnsucez6fp085f5fsksp47u9x8jr4");
        message.set_to_address("terra1al546tmrj0wmwfctcqkch47h57h23mch7d8xqa");
        auto amountOfTx = message.mutable_amounts();
        amountOfTx->set_denom("uluna");
        amountOfTx->set_amount(141964648400);

        auto &fee = *input.mutable_fee();
        fee.set_gas(200000);
        auto amountOfFee = fee.add_amounts();
        amountOfFee->set_denom("uluna");
        amountOfFee->set_amount(1000);

        auto privateKey = parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646");
        input.set_private_key(privateKey.data(), privateKey.size());

        auto signer = TERRA::Signer(std::move(input));
        auto output = signer.build();

        ASSERT_EQ("{\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"1000\",\"denom\":\"uluna\"}],\"gas\":\"200000\"},\"memo\":\"sendtocold\",\"msg\":[{\"type\":\"cosmos-sdk/MsgSend\",\"value\":{\"amount\":{\"amount\":\"141964648400\",\"denom\":\"uluna\"},\"from_address\":\"terra1v9ku44wycfnsucez6fp085f5fsksp47u9x8jr4\",\"to_address\":\"terra1al546tmrj0wmwfctcqkch47h57h23mch7d8xqa\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AkvCoxJlFT8H5w4LqwhyTmuF4hf4zWKM62KXQke7STOC\"},\"signature\":\"Yb1kif/oOx09gZGsIL328V5GoDAMeUCCOptzY7WBqNJdK2YmWHrSh9y7He6sZ4CAdw3FPo4BSbnQOfse4fRi6g==\"}],\"type\":\"cosmos-sdk/MsgSend\"}}", output.json());

        ASSERT_EQ("a1627478a563666565a266616d6f756e7481a266616d6f756e7464313030306564656e6f6d65756c756e616367617366323030303030646d656d6f6a73656e64746f636f6c64636d736781a2647479706572636f736d6f732d73646b2f4d736753656e646576616c7565a366616d6f756e74a266616d6f756e746c3134313936343634383430306564656e6f6d65756c756e616c66726f6d5f61646472657373782c74657272613176396b753434777963666e737563657a366670303835663566736b73703437753978386a72346a746f5f61646472657373782c746572726131616c353436746d726a30776d7766637463716b636834376835376832336d63683764387871616a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416b76436f784a6c465438483577344c71776879546d7546346866347a574b4d36324b58516b653753544f43697369676e617475726578585962316b69662f6f4f783039675a4773494c3332385635476f44414d655543434f70747a59375742714e4a644b32596d5748725368397937486536735a34434164773346506f344253626e514f6673653466526936673d3d647479706572636f736d6f732d73646b2f4d736753656e64", hex(output.encoded()));
    }
} //namespace TW::TERRA