#include "Signer.h"
#include "../proto/Binance.pb.h"

using namespace TW;
using namespace TW::Binance;
using namespace std;

int main(int argc, char* argv[]) {
    auto input = Proto::SigningInput();
    input.set_account_number(696296);
    input.set_chain_id("Binance-Chain-Nile");
    input.set_sequence(0);
    // prepare send message
    auto send = Proto::SendOrder();
    auto inputs = send.add_inputs();
    inputs->set_address("tbnb1pjhqz6pfp7zre7xpj00rmr0ph276rmdsxjyuv2");
    auto incoins = inputs->add_coins();
    incoins->set_amount(1000);
    incoins->set_denom("BNB");
    auto outputs = send.add_outputs();
    outputs->set_address("tbnb1mh3w2kxmdmnvctt7t5nu7hhz9jnp422edqdw2d");
    auto outcoins = outputs->add_coins();
    outcoins->set_amount(1000);
    outcoins->set_denom("BNB");
    input.set_allocated_send_order(&send);
    Signer signer(move(input));

    signer.signWithTss();
    return 0;
}