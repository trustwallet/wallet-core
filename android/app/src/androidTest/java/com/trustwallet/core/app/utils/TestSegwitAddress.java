package com.trustwallet.core.app.utils;

import org.junit.Test;
import wallet.core.jni.HRP;
import wallet.core.jni.PublicKey;
import wallet.core.jni.PublicKeyType;
import wallet.core.jni.SegwitAddress;

import static org.junit.Assert.assertEquals;

public class TestSegwitAddress {

    static {
        System.loadLibrary("TrustWalletCore");
    }

    @Test
    public void testFromPublic() {
        byte[] data = Numeric.INSTANCE.hexStringToByteArray("0x02f1e733ed6030cc569c4323a34b17e192d58107d9ffbce71c8420b779f484dba1");
        PublicKey publicKey = new PublicKey(data, PublicKeyType.SECP256K1);
        SegwitAddress address = new SegwitAddress(HRP.BITCOIN, publicKey);
        assertEquals(address.description(), "bc1qrq6gs660qewd282en83n6em9s4rlslj3cd2wmg");
    }
}
