/*
 * This file is part of the Trezor project, https://trezor.io/
 *
 * Copyright (c) SatoshiLabs
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "chacha_drbg.h"

#include <stdint.h>

#define MAX(a, b) (a) > (b) ? (a) : (b)

void chacha_drbg_init(CHACHA_DRBG_CTX *ctx,
                      const uint8_t entropy[CHACHA_DRBG_SEED_LENGTH]) {
  uint8_t buffer[MAX(CHACHA_DRBG_KEY_LENGTH, CHACHA_DRBG_IV_LENGTH)] = {0};
  ECRYPT_keysetup(&ctx->chacha_ctx, buffer, CHACHA_DRBG_KEY_LENGTH * 8,
                  CHACHA_DRBG_IV_LENGTH * 8);
  ECRYPT_ivsetup(&ctx->chacha_ctx, buffer);

  chacha_drbg_reseed(ctx, entropy);
}

static void chacha_drbg_update(CHACHA_DRBG_CTX *ctx,
                               const uint8_t data[CHACHA_DRBG_SEED_LENGTH]) {
  uint8_t buffer[CHACHA_DRBG_SEED_LENGTH] = {0};

  if (data)
    ECRYPT_encrypt_bytes(&ctx->chacha_ctx, data, buffer,
                         CHACHA_DRBG_SEED_LENGTH);
  else
    ECRYPT_keystream_bytes(&ctx->chacha_ctx, buffer, CHACHA_DRBG_SEED_LENGTH);

  ECRYPT_keysetup(&ctx->chacha_ctx, buffer, CHACHA_DRBG_KEY_LENGTH * 8,
                  CHACHA_DRBG_IV_LENGTH * 8);
  ECRYPT_ivsetup(&ctx->chacha_ctx, buffer + CHACHA_DRBG_KEY_LENGTH);
}

void chacha_drbg_generate(CHACHA_DRBG_CTX *ctx, uint8_t *output,
                          uint8_t output_length) {
  ECRYPT_keystream_bytes(&ctx->chacha_ctx, output, output_length);
  chacha_drbg_update(ctx, NULL);
  ctx->reseed_counter++;
}

void chacha_drbg_reseed(CHACHA_DRBG_CTX *ctx,
                        const uint8_t entropy[CHACHA_DRBG_SEED_LENGTH]) {
  chacha_drbg_update(ctx, entropy);
  ctx->reseed_counter = 1;
}
