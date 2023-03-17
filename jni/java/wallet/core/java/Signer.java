package wallet.core.java;

import wallet.core.jni.PublicKey;

public interface Signer {

    PublicKey getPublicKey();
    byte[] sign(byte[] data);
}
