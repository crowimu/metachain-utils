#pragma once

#include "BIP39\BIP39.h"
#include <mutex>

namespace BIP39
{
	long_hash pkcs5_pbkdf2_hmac_sha512(data_slice passphrase, data_slice salt, size_t iterations);

	/* Password-Based Key Derivation Function 2 (PKCS #5 v2.0). */
	/* Code based on IEEE Std 802.11-2007, Annex H.4.2. */
	/* returns 0 if successful. */
	int pkcs5_pbkdf2(const uint8_t* passphrase, size_t passphrase_length,
		const uint8_t* salt, size_t salt_length, uint8_t* key, size_t key_length,
		size_t iterations);

	inline data_chunk to_chunk(uint8_t byte);

	// Ensure validate_localization is called only once.
	static std::once_flag icu_mutex;
	std::string to_normal_nfkd_form(const std::string& value);

	/**
	* Create a data chunk from an iterable object.
	*/
	template <typename Source>
	data_chunk to_chunk(const Source& bytes);

	template <typename Source>
	data_chunk to_chunk(const Source& bytes)
	{
		return data_chunk(std::begin(bytes), std::end(bytes));
	}
}