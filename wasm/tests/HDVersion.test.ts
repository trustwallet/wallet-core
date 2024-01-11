// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";

describe("HDVersion", () => {
  it("test isPublic", () => {
    const { HDVersion, HDVersionExt } = globalThis.core;

    assert.isFalse(HDVersionExt.isPublic(HDVersion.none));
    assert.isTrue(HDVersionExt.isPublic(HDVersion.xpub));
    assert.isFalse(HDVersionExt.isPublic(HDVersion.xprv));
    assert.isTrue(HDVersionExt.isPublic(HDVersion.ypub));
    assert.isFalse(HDVersionExt.isPublic(HDVersion.yprv));
    assert.isTrue(HDVersionExt.isPublic(HDVersion.zpub));
    assert.isFalse(HDVersionExt.isPublic(HDVersion.zprv));
    assert.isTrue(HDVersionExt.isPublic(HDVersion.ltub));
    assert.isFalse(HDVersionExt.isPublic(HDVersion.ltpv));
    assert.isTrue(HDVersionExt.isPublic(HDVersion.mtub));
    assert.isFalse(HDVersionExt.isPublic(HDVersion.mtpv));
    assert.isTrue(HDVersionExt.isPublic(HDVersion.dpub));
    assert.isFalse(HDVersionExt.isPublic(HDVersion.dprv));
    assert.isTrue(HDVersionExt.isPublic(HDVersion.dgub));
    assert.isFalse(HDVersionExt.isPublic(HDVersion.dgpv));
  });

  it("test isPrivate", () => {
    const { HDVersion, HDVersionExt } = globalThis.core;

    assert.isFalse(HDVersionExt.isPrivate(HDVersion.none));
    assert.isFalse(HDVersionExt.isPrivate(HDVersion.xpub));
    assert.isTrue(HDVersionExt.isPrivate(HDVersion.xprv));
    assert.isFalse(HDVersionExt.isPrivate(HDVersion.ypub));
    assert.isTrue(HDVersionExt.isPrivate(HDVersion.yprv));
    assert.isFalse(HDVersionExt.isPrivate(HDVersion.zpub));
    assert.isTrue(HDVersionExt.isPrivate(HDVersion.zprv));
    assert.isFalse(HDVersionExt.isPrivate(HDVersion.ltub));
    assert.isTrue(HDVersionExt.isPrivate(HDVersion.ltpv));
    assert.isFalse(HDVersionExt.isPrivate(HDVersion.mtub));
    assert.isTrue(HDVersionExt.isPrivate(HDVersion.mtpv));
    assert.isFalse(HDVersionExt.isPrivate(HDVersion.dpub));
    assert.isTrue(HDVersionExt.isPrivate(HDVersion.dprv));
    assert.isFalse(HDVersionExt.isPrivate(HDVersion.dgub));
    assert.isTrue(HDVersionExt.isPrivate(HDVersion.dgpv));
  });
});
