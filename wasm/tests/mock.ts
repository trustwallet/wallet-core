// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import { Events, Storage } from "webextension-polyfill";

export class ChromeStorageMock implements Storage.StorageArea {
  object = {};

  get(
    keys?: string | string[] | Record<string, any> | null | undefined
  ): Promise<Record<string, any>> {
    var ids: string[] = [];
    if (typeof keys === "string") {
      ids.push(keys);
    } else if (keys instanceof Array) {
      ids = ids.concat(keys);
    }

    var result: Record<string, any> = {};
    ids.forEach((id) => {
      result[id] = this.object[id];
    });
    return Promise.resolve(result);
  }

  set(items: Record<string, any>): Promise<void> {
    Object.keys(items).forEach((key) => {
      this.object[key] = items[key];
    });
    return Promise.resolve();
  }

  remove(keys: string | string[]): Promise<void> {
    var ids: string[] = [];
    if (typeof keys === "string") {
      ids.push(keys);
    }
    ids = ids.concat(keys);
    ids.forEach((id) => delete this.object[id]);
    return Promise.resolve();
  }

  clear(): Promise<void> {
    throw new Error("Method not implemented.");
  }

  onChanged: Events.Event<
    (changes: Storage.StorageAreaOnChangedChangesType) => void
  > = {} as any;
}
