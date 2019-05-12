package com.trustwallet.core.app.blockchains;

import wallet.core.jni.SegwitAddress;
import wallet.core.jni.HRP;
import wallet.core.jni.PublicKey;
import wallet.core.jni.PublicKeyType;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class TestSegwitAddress {

    static {
        System.loadLibrary("TrustWalletCore");
    }

    @Test
    public void testFromPublic() {
        byte[] data = {0x02,
                (byte) 0xf1, (byte) 0xe7, (byte) 0x33, (byte) 0xed, (byte) 0x60, (byte) 0x30, (byte) 0xcc, (byte) 0x56,
                (byte) 0x9c, (byte) 0x43, (byte) 0x23, (byte) 0xa3, (byte) 0x4b, (byte) 0x17, (byte) 0xe1, (byte) 0x92,
                (byte) 0xd5, (byte) 0x81, (byte) 0x07, (byte) 0xd9, (byte) 0xff, (byte) 0xbc, (byte) 0xe7, (byte) 0x1c,
                (byte) 0x84, (byte) 0x20, (byte) 0xb7, (byte) 0x79, (byte) 0xf4, (byte) 0x84, (byte) 0xdb, (byte) 0xa1
        };
        PublicKey publicKey = new PublicKey(data, PublicKeyType.SECP256K1);
        SegwitAddress address = new SegwitAddress(HRP.BITCOIN, publicKey);
        assertEquals(address.description(), "bc1qrq6gs660qewd282en83n6em9s4rlslj3cd2wmg");
    }

    @Test
    public void testBadHrp() {
        byte[] data = {0x02,
                (byte) 0xf1, (byte) 0xe7, (byte) 0x33, (byte) 0xed, (byte) 0x60, (byte) 0x30, (byte) 0xcc, (byte) 0x56,
                (byte) 0x9c, (byte) 0x43, (byte) 0x23, (byte) 0xa3, (byte) 0x4b, (byte) 0x17, (byte) 0xe1, (byte) 0x92,
                (byte) 0xd5, (byte) 0x81, (byte) 0x07, (byte) 0xd9, (byte) 0xff, (byte) 0xbc, (byte) 0xe7, (byte) 0x1c,
                (byte) 0x84, (byte) 0x20, (byte) 0xb7, (byte) 0x79, (byte) 0xf4, (byte) 0x84, (byte) 0xdb, (byte) 0xa1
        };
        PublicKey publicKey = new PublicKey(data, PublicKeyType.SECP256K1);
        SegwitAddress address = new SegwitAddress(HRP.BITCOIN, publicKey);
        assertEquals(address.description(), "bc1qrq6gs660qewd282en83n6em9s4rlslj3cd2wmg");
    }
}
