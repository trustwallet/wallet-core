package main

import (
	"encoding/hex"
	"fmt"
	"math"
	"math/big"
	"tw/core"
	"tw/protos/ethereum"
)

func main() {
	mn := "confirm bleak useless tail chalk destroy horn step bulb genuine attract split"
	// bitcoin wallet
	bw, err := core.CreateWalletWithMnemonic(mn, core.CoinTypeBitcoin)
	if err != nil {
		panic(err)
	}
	printWallet(bw)
	// ethereum wallet
	ew, err := core.CreateWalletWithMnemonic(mn, core.CoinTypeEthereum)
	if err != nil {
		panic(err)
	}
	printWallet(ew)
	// tron wallet
	tw, err := core.CreateWalletWithMnemonic(mn, core.CoinTypeTron)
	if err != nil {
		panic(err)
	}
	printWallet(tw)

	// ethereum transaction
	priKeyByte, _ := hex.DecodeString(ew.PriKey)

	ethSigningInput := ethereum.SigningInput{
		ChainId:    big.NewInt(4).Bytes(), // mainnet: 1, rinkeby: 4 https://chainlist.org/
		Nonce:      big.NewInt(0).Bytes(), // get nonce from network
		TxMode:     ethereum.TransactionMode_Legacy,
		GasPrice:   big.NewInt(100000000000).Bytes(), // 100 gwei
		GasLimit:   big.NewInt(21000).Bytes(),
		ToAddress:  "0xE9B511C0753649E5F3E78Ed8AdBEE92d0d2Db384",
		PrivateKey: priKeyByte,
		Transaction: &ethereum.Transaction{
			TransactionOneof: &ethereum.Transaction_Transfer_{
				Transfer: &ethereum.Transaction_Transfer{
					// eth amount should be in wei, eth * (10^decimals) = wei
					Amount: big.NewInt(int64(
						0.01 * math.Pow10(ew.CoinType.Decimals()),
					)).Bytes(),
					Data: []byte{},
				},
			},
		},
	}

	var ethSigningOutput ethereum.SigningOutput
	err = core.CreateSignedTx(&ethSigningInput, ew.CoinType, &ethSigningOutput)
	if err != nil {
		panic(err)
	}
	fmt.Println("Ethereum signed tx:\n\t", hex.EncodeToString(ethSigningOutput.GetEncoded()))
}

func printWallet(w *core.Wallet) {
	fmt.Printf("%s wallet: \n", w.CoinType.GetName())
	fmt.Printf("\t address: %s \n", w.Address)
	fmt.Printf("\t pri key: %s \n", w.PriKey)
	fmt.Printf("\t pub key: %s \n", w.PubKey)
	fmt.Println("")
}
