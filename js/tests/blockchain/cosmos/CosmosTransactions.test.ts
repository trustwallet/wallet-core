import { expect } from 'chai';
import 'mocha';

import * as Long from 'long';

import { fromHexString } from '../../Utils';
import { TW, PrivateKey, CosmosAddress, CosmosSigner, HRP } from '../../../lib';

describe('CosmosTransactions', () => {

    it('test signing transaction', () => {
        const privateKey = PrivateKey.createWithData(fromHexString('80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005'));
        const publicKey = privateKey.getPublicKeySecp256k1(true)
        const from = CosmosAddress.createWithPublicKey(HRP.COSMOS, publicKey).description();

        const txAmount = TW.Cosmos.Proto.Amount.create({
            amount: Long.fromInt(1),
            denom: 'muon'
        });

        const sendCoinsMsg = TW.Cosmos.Proto.SendCoinsMessage.create({
            fromAddress: from,
            toAddress: 'cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573',
            amounts: [txAmount]
        });


        const feeAmount = TW.Cosmos.Proto.Amount.create({
            amount: Long.fromInt(200),
            denom: 'muon'
        });

        const cosmosFee = TW.Cosmos.Proto.Fee.create({
            gas: Long.fromInt(200000),
            amounts: [feeAmount]
        });

        const signingInput = TW.Cosmos.Proto.SigningInput.create({
            accountNumber: Long.fromInt(1037),
            chainId: 'gaia-13003',
            memo: '',
            sequence: Long.fromInt(8),
            sendCoinsMessage: sendCoinsMsg,
            fee: cosmosFee,
            privateKey: privateKey.data()
        });

        const jsonPayload = CosmosSigner.sign(signingInput).json;

        const expectedJsonPayload = {'tx':{'fee':{'amount':[{'amount':'200','denom':'muon'}],'gas':'200000'},'memo':'','msg':[{'type':'cosmos-sdk/MsgSend','value':{'amount':[{'amount':'1','denom':'muon'}],'from_address':'cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02','to_address':'cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573'}}],'signatures':[{'pub_key':{'type':'tendermint/PubKeySecp256k1','value':'AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F'},'signature':'/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=='}],'type':'cosmos-sdk/MsgSend'}};
        expect(jsonPayload).to.equal(JSON.stringify(expectedJsonPayload));
    });

});
