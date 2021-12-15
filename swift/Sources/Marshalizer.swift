// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation
import SwiftProtobuf

public final class Marshalizer {
    public static func unmarshalProto<T: Message>(_ data: Data) -> T {
        do {
            return try T(serializedData: data)
        } catch let error {
            fatalError("Marshalizer.unmarshalProto() error: " + error.localizedDescription)
        }
    }
}
