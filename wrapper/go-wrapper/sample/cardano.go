package sample

import (
	"encoding/hex"
	"errors"

	"github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/core"
	"github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/cardano"
	"github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/transactioncompiler"
	"github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/types"
	"google.golang.org/protobuf/proto"
)

// Using real succesfull transation:
func TestCardano() {
	txHash1, _ := hex.DecodeString("8316e5007d61fb90652cabb41141972a38b5bc60954d602cf843476aa3f67f63")
	utxo1 := cardano.TxInput{
		OutPoint: &cardano.OutPoint{
			TxHash:      txHash1,
			OutputIndex: 0,
		},
		Address: "Ae2tdPwUPEZ6vkqxSjJxaQYmDxHf5DTnxtZ67pFLJGTb9LTnCGkDP6ca3f8",
		Amount:  2500000,
	}

	txHash2, _ := hex.DecodeString("e29392c59c903fefb905730587d22cae8bda30bd8d9aeec3eca082ae77675946")
	utxo2 := cardano.TxInput{
		OutPoint: &cardano.OutPoint{
			TxHash:      txHash2,
			OutputIndex: 0,
		},
		Address: "Ae2tdPwUPEZ6vkqxSjJxaQYmDxHf5DTnxtZ67pFLJGTb9LTnCGkDP6ca3f8",
		Amount:  1700000,
	}

	transferMessage := cardano.Transfer{
		ToAddress:     "addr1q90uh2eawrdc9vaemftgd50l28yrh9lqxtjjh4z6dnn0u7ggasexxdyyk9f05atygnjlccsjsggtc87hhqjna32fpv5qeq96ls",
		ChangeAddress: "addr1qx55ymlqemndq8gluv40v58pu76a2tp4mzjnyx8n6zrp2vtzrs43a0057y0edkn8lh9su8vh5lnhs4npv6l9tuvncv8swc7t08",
		Amount:        3000000,
		UseMaxAmount:  false,
	}
	var privateKeys [][]byte
	privateKeyData, _ := hex.DecodeString("98f266d1aac660179bc2f456033941238ee6b2beb8ed0f9f34c9902816781f5a9903d1d395d6ab887b65ea5e344ef09b449507c21a75f0ce8c59d0ed1c6764eba7f484aa383806735c46fd769c679ee41f8952952036a6e2338ada940b8a91f4e890ca4eb6bec44bf751b5a843174534af64d6ad1f44e0613db78a7018781f5aa151d2997f52059466b715d8eefab30a78b874ae6ef4931fa58bb21ef8ce2423d46f19d0fbf75afb0b9a24e31d533f4fd74cee3b56e162568e8defe37123afc4")
	privateKeys = append(privateKeys, privateKeyData)

	input := cardano.SigningInput{
		Utxos:           []*cardano.TxInput{&utxo1, &utxo2},
		PrivateKey:      privateKeys,
		TransferMessage: &transferMessage,
		Ttl:             190000000,
	}

	txInputData, _ := proto.Marshal(&input)
	msgForSign := core.PreImageHashes(core.CoinTypeCardano, txInputData)

	var preSigningOutput transactioncompiler.PreSigningOutput
	proto.Unmarshal(msgForSign, &preSigningOutput)

	var output cardano.SigningOutput
	err := core.CreateSignedTx(&input, core.CoinTypeCardano, &output)
	if err != nil {
		panic(err)
	}

	twPrivateKey, err := core.PrivateKeyCreateWithData(privateKeyData)
	if err != nil {
		panic(err)
	}

	dataForSign := types.TWDataCreateWithGoBytes(preSigningOutput.DataHash)
	signature := core.PrivateKeySign(twPrivateKey, dataForSign, core.CurveED25519ExtendedCardano)
	if signature == nil {
		panic(errors.New("signature is nil"))
	}

	pub := core.TWPrivateKeyGetPublicKeyEd25519Cardano(twPrivateKey)
	if pub == nil {
		panic(errors.New("publickey pointer is nil"))
	}

	pubValue := core.PublicKeyDescription(pub)
	if pubValue == nil {
		panic(errors.New("no value for publickey"))
	}

	pubKeyBytes, _ := hex.DecodeString(types.TWStringGoString(pubValue))
	if pubKeyBytes == nil {
		panic(errors.New("no value for publickey"))
	}

	valid := core.PublicKeyVerify(pubKeyBytes, core.PublicKeyTypeED25519Cardano, types.TWDataGoBytes(signature), preSigningOutput.DataHash)
	if !valid {
		panic(errors.New("verification failed"))
	}
	core.PrivateKeyDelete(twPrivateKey)
}
