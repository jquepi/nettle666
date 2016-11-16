/* gost28147.h

   The GOST 28147-89 cipher function, described in RFC 5831.

   Copyright (C) 2019 Dmitry Eremin-Solenikov

   This file is part of GNU Nettle.

   GNU Nettle is free software: you can redistribute it and/or
   modify it under the terms of either:

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at your
       option) any later version.

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at your
       option) any later version.

   or both in parallel, as here.

   GNU Nettle is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see http://www.gnu.org/licenses/.
*/

#ifndef NETTLE_GOST28147_H_INCLUDED
#define NETTLE_GOST28147_H_INCLUDED

#include "nettle-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* S-Boxes & parameters */
#define gost28147_param_test_3411 nettle_gost28147_param_test_3411
#define gost28147_param_CryptoPro_3411 nettle_gost28147_param_CryptoPro_3411
#define gost28147_param_Test_89 nettle_gost28147_param_Test_89
#define gost28147_param_CryptoPro_A nettle_gost28147_param_CryptoPro_A
#define gost28147_param_CryptoPro_B nettle_gost28147_param_CryptoPro_B
#define gost28147_param_CryptoPro_C nettle_gost28147_param_CryptoPro_C
#define gost28147_param_CryptoPro_D nettle_gost28147_param_CryptoPro_D
#define gost28147_param_TC26_Z nettle_gost28147_param_TC26_Z

#define gost28147_set_key nettle_gost28147_set_key
#define gost28147_set_param nettle_gost28147_set_param
#define gost28147_encrypt nettle_gost28147_encrypt
#define gost28147_encrypt_for_cfb nettle_gost28147_encrypt_for_cfb
#define gost28147_decrypt nettle_gost28147_decrypt

#define gost28147_cnt_init nettle_gost28147_cnt_init
#define gost28147_cnt_set_iv nettle_gost28147_cnt_set_iv
#define gost28147_cnt_crypt nettle_gost28147_cnt_crypt

#define gost28147_imit_init nettle_gost28147_imit_init
#define gost28147_imit_set_key nettle_gost28147_imit_set_key
#define gost28147_imit_set_nonce nettle_gost28147_imit_set_nonce
#define gost28147_imit_set_param nettle_gost28147_imit_set_param
#define gost28147_imit_update nettle_gost28147_imit_update
#define gost28147_imit_digest nettle_gost28147_imit_digest

#define GOST28147_KEY_SIZE 32
#define GOST28147_BLOCK_SIZE 8

struct gost28147_ctx
{
  uint32_t key[GOST28147_KEY_SIZE/4];
  const uint32_t (*sbox)[256];
  int key_meshing;
  int key_count; /* Used for key meshing */
};

struct gost28147_param
{
  uint32_t sbox[4][256];
  int key_meshing;
};

extern const struct gost28147_param gost28147_param_test_3411;
extern const struct gost28147_param gost28147_param_CryptoPro_3411;
extern const struct gost28147_param gost28147_param_Test_89;
extern const struct gost28147_param gost28147_param_CryptoPro_A;
extern const struct gost28147_param gost28147_param_CryptoPro_B;
extern const struct gost28147_param gost28147_param_CryptoPro_C;
extern const struct gost28147_param gost28147_param_CryptoPro_D;
extern const struct gost28147_param gost28147_param_TC26_Z;

void
gost28147_set_key(struct gost28147_ctx *ctx, const uint8_t *key);

void
gost28147_set_param(struct gost28147_ctx *ctx,
		    const struct gost28147_param *param);

void
gost28147_encrypt(const struct gost28147_ctx *ctx,
		  size_t length, uint8_t *dst,
		  const uint8_t *src);
void
gost28147_decrypt(const struct gost28147_ctx *ctx,
		  size_t length, uint8_t *dst,
		  const uint8_t *src);
void
gost28147_encrypt_for_cfb(struct gost28147_ctx *ctx,
			  size_t length, uint8_t *dst,
			  const uint8_t *src);

struct gost28147_cnt_ctx {
  struct gost28147_ctx ctx;
  size_t bytes;
  uint32_t iv[2];
  uint8_t buffer[GOST28147_BLOCK_SIZE];
};

void
gost28147_cnt_init(struct gost28147_cnt_ctx *ctx,
		   const uint8_t *key,
		   const struct gost28147_param *param);

void
gost28147_cnt_set_iv(struct gost28147_cnt_ctx *ctx,
		     const uint8_t *iv);

void
gost28147_cnt_crypt(struct gost28147_cnt_ctx *ctx,
		    size_t length, uint8_t *dst,
		    const uint8_t *src);

#define GOST28147_IMIT_DIGEST_SIZE 4
#define GOST28147_IMIT_BLOCK_SIZE GOST28147_BLOCK_SIZE
#define GOST28147_IMIT_KEY_SIZE GOST28147_KEY_SIZE

struct gost28147_imit_ctx
{
  struct gost28147_ctx cctx;
  uint64_t count;		/* Block count */
  uint8_t block[GOST28147_IMIT_BLOCK_SIZE]; /* Block buffer */
  unsigned index;               /* Into buffer */
  uint32_t state[GOST28147_IMIT_BLOCK_SIZE/4];
};

void
gost28147_imit_init(struct gost28147_imit_ctx *ctx);

void
gost28147_imit_set_key(struct gost28147_imit_ctx *ctx,
		       size_t length,
		       const uint8_t *key);

void
gost28147_imit_set_nonce(struct gost28147_imit_ctx *ctx,
		         const uint8_t *nonce);

void
gost28147_imit_set_param(struct gost28147_imit_ctx *ctx,
			 const struct gost28147_param *param);

void
gost28147_imit_update(struct gost28147_imit_ctx *ctx,
		      size_t length,
		      const uint8_t *data);

void
gost28147_imit_digest(struct gost28147_imit_ctx *ctx,
		      size_t length,
		      uint8_t *digest);

#ifdef __cplusplus
}
#endif

#endif /* NETTLE_GOST28147_H_INCLUDED */
