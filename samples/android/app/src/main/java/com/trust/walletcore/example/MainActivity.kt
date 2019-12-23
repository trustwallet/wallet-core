package com.trust.walletcore.example

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.google.protobuf.ByteString
import kotlinx.android.synthetic.main.activity_main.*
import wallet.core.jni.CoinType
import wallet.core.jni.HDWallet
import wallet.core.jni.EthereumSigner
import wallet.core.jni.proto.Ethereum
import wallet.core.jni.BitcoinTransactionSigner
import wallet.core.jni.BitcoinScript
import wallet.core.jni.BitcoinSigHashType
import wallet.core.jni.proto.Bitcoin
import wallet.core.jni.AnySigner
import wallet.core.jni.proto.Any
import java.math.BigInteger
import kotlin.experimental.and

class MainActivity : AppCompatActivity() {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    private val seedPhrase = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal"
    private val passphrase = ""

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // 'Import' a wallet
        val wallet = HDWallet(seedPhrase, passphrase)
        showLog("Mnemonic: \n${wallet.mnemonic()}")

        // Ethereum example
        val coinEth: CoinType = CoinType.ETHEREUM
        // Get the default address
        val addressEth = wallet.getAddressForCoin(coinEth)
        showLog("Default ETH address: \n$addressEth")

        // Signing a transaction (using EthereumSigner)
        val secretPrivateKey = wallet.getKeyForCoin(coinEth)
        val dummyReceiverAddress = "0xC37054b3b48C3317082E7ba872d7753D13da4986"

        val signerInput = Ethereum.SigningInput.newBuilder().apply {
            this.chainId = ByteString.copyFrom(BigInteger("01").toByteArray())
            this.gasPrice = BigInteger("d693a400", 16).toByteString() // decimal 3600000000
            this.gasLimit = BigInteger("5208", 16).toByteString()     // decimal 21000
            this.toAddress = dummyReceiverAddress
            this.amount = BigInteger("0348bca5a16000", 16).toByteString()
            this.privateKey = ByteString.copyFrom(secretPrivateKey.data())
        }.build()
        val signerOutput = EthereumSigner.sign(signerInput)
        showLog("Signed transaction: \n${signerOutput.encoded.toByteArray().toHexString(false)}")

        // Signing a transaction (using AnySigner)
        val secretPrivateKeyHex = secretPrivateKey.data().toHexString()
                val chainIdB64 = "AQ=="  // base64(parse_hex("01"))
        val gasPriceB64 = "1pOkAA=="  // base64(parse_hex("d693a4")) decimal 3600000000
        val gasLimitB64 = "Ugg="  // base64(parse_hex("5208")) decimal 21000
        val amountB64 = "A0i8paFgAA=="  // base64(parse_hex("0348bca5a160"))  924400000000000

        val transaction = """
        {
            "chainId": "$chainIdB64",
            "gasPrice": "$gasPriceB64",
            "gasLimit": "$gasLimitB64",
            "toAddress": "$dummyReceiverAddress",
            "amount": "$amountB64"
        }
        """
        showLog("transaction: $transaction")

        val anySignerInput = Any.SigningInput.newBuilder().apply {
            this.coinType = coinEth.value()
            this.transaction = transaction
            this.privateKey = secretPrivateKeyHex
        }.build()
        val anySignerOutput = AnySigner.sign(anySignerInput)
        if (!anySignerOutput.success) {
            showLog("Error: \n${anySignerOutput.error}")
        } else {
            showLog("Signed transaction data: \n${anySignerOutput.output}")
        }

        // Bitcoin example
        val coinBtc: CoinType = CoinType.BITCOIN
        // Get the default address
        val addressBtc = wallet.getAddressForCoin(coinBtc)
        showLog("Default BTC address: \n$addressBtc")

        // Build a transaction
        val utxoTxId = "050d00e2e18ef13969606f1ceee290d3f49bd940684ce39898159352952b8ce2".hexStringToByteArray();
        val secretPrivateKeyBtc = wallet.getKeyForCoin(coinBtc)
        val toAddress = "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx"
        val changeAddress = "1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU"
        val script = BitcoinScript.buildForAddress(addressBtc, coinBtc).data()

        val outPoint = Bitcoin.OutPoint.newBuilder().apply {
            this.hash = ByteString.copyFrom(utxoTxId)
            this.index = 2
        }.build()
        val utxo = Bitcoin.UnspentTransaction.newBuilder().apply {
            this.amount = 5151
            this.outPoint = outPoint
            this.script = ByteString.copyFrom(script)
        }.build()
        val input = Bitcoin.SigningInput.newBuilder().apply {
            this.amount = 600
            this.hashType = BitcoinSigHashType.ALL.value().or(BitcoinSigHashType.FORK.value())
            this.toAddress = toAddress
            this.changeAddress = changeAddress
            this.byteFee = 1
            this.coinType = coinBtc.value()
            this.addUtxo(utxo)
            this.addPrivateKey(ByteString.copyFrom(secretPrivateKeyBtc.data()))
        }.build()

        val signer = BitcoinTransactionSigner(input)
        val result = signer.sign()

        assert(result.success)
        assert(result.error.isEmpty())
        assert(result.objectsCount > 0)

        val output = result.getObjects(0).unpack(Bitcoin.SigningOutput::class.java)
        val signedTransaction = output?.encoded?.toByteArray()
        showLog("Signed BTC transaction: \n${signedTransaction?.toHexString()}")
    }

    private fun ByteArray.toHexString(withPrefix: Boolean = true): String {
        val stringBuilder = StringBuilder()
        if(withPrefix) {
            stringBuilder.append("0x")
        }
        for (element in this) {
            stringBuilder.append(String.format("%02x", element and 0xFF.toByte()))
        }
        return stringBuilder.toString()
    }

    private fun BigInteger.toByteString(): ByteString {
        return ByteString.copyFrom(this.toByteArray())
    }

    private fun String.hexStringToByteArray() : ByteArray {
        val HEX_CHARS = "0123456789ABCDEF"
        val result = ByteArray(length / 2)
        for (i in 0 until length step 2) {
            val firstIndex = HEX_CHARS.indexOf(this[i].toUpperCase());
            val secondIndex = HEX_CHARS.indexOf(this[i + 1].toUpperCase());
            val octet = firstIndex.shl(4).or(secondIndex)
            result.set(i.shr(1), octet.toByte())
        }
        return result
    }

    private fun showLog(log: String) {
        val tv =  TextView(this)
        tv.text = log
        logView.addView(tv)
        println(log)
    }
}
