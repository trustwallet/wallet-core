package wallet.core.java;

import wallet.core.jni.CoinType;
import wallet.core.jni.PrivateKey;
import wallet.core.jni.PublicKey;

// This is used only in TrustWallet auto-tests
public class PrivateKeySigner implements Signer {

    private final PublicKey publicKey;
    private final PrivateKey privateKey;
    private final CoinType coinType;

    public PrivateKeySigner(PrivateKey privateKey, CoinType coinType) {
        this.privateKey = privateKey;
        this.coinType = coinType;
        this.publicKey = privateKey.getPublicKey(coinType);
    }

    @Override
    public byte[] sign(byte[] data) {
        return privateKey.sign(data, coinType.curve());
    }

    @Override
    public PublicKey getPublicKey() {
        return publicKey;
    }
}
