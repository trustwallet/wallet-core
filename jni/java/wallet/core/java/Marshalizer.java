// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package wallet.core.java;

import com.google.protobuf.Message;
import com.google.protobuf.Parser;

public class Marshalizer {
    public static <T extends Message> T unmarshalProto(byte[] data, Parser<T> parser) throws Exception {
        T message = parser.parseFrom(data);
        return message;
    }
}
