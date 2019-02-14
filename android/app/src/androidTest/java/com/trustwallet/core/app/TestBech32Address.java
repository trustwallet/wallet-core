package com.trustwallet.core.app;

import com.trustwallet.core.app.utils.Numeric;
import com.wallet.crypto.trustapp.jni.Bech32Address;
import com.wallet.crypto.trustapp.jni.HRP;
import com.wallet.crypto.trustapp.jni.PublicKey;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class TestBech32Address {

    static {
        System.loadLibrary("TrustWalletCore");
    }

    @Test
    public void testFromPublic() {
        byte[] data = Numeric.INSTANCE.hexStringToByteArray("0x02f1e733ed6030cc569c4323a34b17e192d58107d9ffbce71c8420b779f484dba1");
        PublicKey publicKey = new PublicKey(data);
        Bech32Address address = new Bech32Address(HRP.BITCOIN, publicKey);
        assertEquals(address.description(), "bc1qrq6gs660qewd282en83n6em9s4rlslj3cd2wmg");
    }

    @Test
    public void testBadHrp() {
        byte[] data = Numeric.INSTANCE.hexStringToByteArray("0x02f1e733ed6030cc569c4323a34b17e192d58107d9ffbce71c8420b779f484dba1");
        PublicKey publicKey = new PublicKey(data);
        Bech32Address address = new Bech32Address(HRP.BITCOIN, publicKey);
        assertEquals(address.description(), "bc1qrq6gs660qewd282en83n6em9s4rlslj3cd2wmg");
    }
}
