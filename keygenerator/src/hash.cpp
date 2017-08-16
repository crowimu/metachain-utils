#include "hash.h"
#include "hmac_sha512.h"
#include "zeroize.h"

namespace BIP39
{
	hash_digest sha256_hash(data_slice data)
	{
		hash_digest hash;
		SHA256_(data.data(), data.size(), hash.data());
		return hash;
	}

	long_hash pkcs5_pbkdf2_hmac_sha512(data_slice passphrase, data_slice salt, size_t iterations)
	{
		long_hash hash;
		const auto result = pkcs5_pbkdf2(passphrase.data(), passphrase.size(),
			salt.data(), salt.size(), hash.data(), hash.size(), iterations);

		if (result != 0)
			throw std::bad_alloc();

		return hash;
	}

	int pkcs5_pbkdf2(const uint8_t* passphrase, size_t passphrase_length,
		const uint8_t* salt, size_t salt_length, uint8_t* key, size_t key_length,
		size_t iterations)
	{
		uint8_t* asalt;
		size_t asalt_size;
		size_t count, index, iteration, length;
		uint8_t buffer[HMACSHA512_DIGEST_LENGTH];
		uint8_t digest1[HMACSHA512_DIGEST_LENGTH];
		uint8_t digest2[HMACSHA512_DIGEST_LENGTH];

		/* An iteration count of 0 is equivalent to a count of 1. */
		/* A key_length of 0 is a no-op. */
		/* A salt_length of 0 is perfectly valid. */

		if (salt_length > SIZE_MAX - 4)
			return -1;
		asalt_size = salt_length + 4;
		asalt = (uint8_t*)malloc(asalt_size);
		if (asalt == NULL)
			return -1;

		memcpy(asalt, salt, salt_length);
		for (count = 1; key_length > 0; count++)
		{
			asalt[salt_length + 0] = (count >> 24) & 0xff;
			asalt[salt_length + 1] = (count >> 16) & 0xff;
			asalt[salt_length + 2] = (count >> 8) & 0xff;
			asalt[salt_length + 3] = (count >> 0) & 0xff;
			HMACSHA512(asalt, asalt_size, passphrase, passphrase_length, digest1);
			memcpy(buffer, digest1, sizeof(buffer));

			for (iteration = 1; iteration < iterations; iteration++)
			{
				HMACSHA512(digest1, sizeof(digest1), passphrase, passphrase_length,
					digest2);
				memcpy(digest1, digest2, sizeof(digest1));
				for (index = 0; index < sizeof(buffer); index++)
					buffer[index] ^= digest1[index];
			}

			length = (key_length < sizeof(buffer) ? key_length : sizeof(buffer));
			memcpy(key, buffer, length);
			key += length;
			key_length -= length;
		};

		zeroize(digest1, sizeof(digest1));
		zeroize(digest2, sizeof(digest2));
		zeroize(buffer, sizeof(buffer));
		zeroize(asalt, asalt_size);
		free(asalt);

		return 0;
	}

	inline data_chunk to_chunk(uint8_t byte)
	{
		return data_chunk{ byte };
	}
}
