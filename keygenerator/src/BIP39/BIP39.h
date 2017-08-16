#pragma once

#ifndef __BIP39_H__
#define __BIP39_H__

#include <cstdint>
#include "../array_slice.h"
#include "dictionairy.h"

namespace BIP39
{
	typedef std::vector<std::string> string_list;
	typedef array_slice<uint8_t> data_slice;
	typedef std::vector<uint8_t> data_chunk;
	typedef std::initializer_list<data_slice> loaf;

	const size_t hash_size = 32;
	const size_t half_hash_size = hash_size / 2;
	const size_t quarter_hash_size = half_hash_size / 2;
	const size_t long_hash_size = 2 * hash_size;
	const size_t short_hash_size = 20;
	const size_t mini_hash_size = 6;

	template <size_t Size>
	using byte_array = std::array<uint8_t, Size>;

	typedef byte_array<hash_size> hash_digest;
	typedef byte_array<half_hash_size> half_hash;
	typedef byte_array<quarter_hash_size> quarter_hash;
	typedef byte_array<long_hash_size> long_hash;
	typedef byte_array<short_hash_size> short_hash;
	typedef byte_array<mini_hash_size> mini_hash;

	static constexpr size_t bits_per_word = 11;
	static constexpr size_t entropy_bit_divisor = 32;
	static constexpr size_t hmac_iterations = 2048;
	static const char* passphrase_prefix = "mnemonic";
	static size_t mnemonic_word_multiple = 3;
	static size_t mnemonic_seed_multiple = 4;
	const size_t byte_bits = 8;

	class Mnemonic
	{
	private:
		inline uint8_t shift(size_t bit);
		inline data_chunk build_chunk(loaf slices, size_t extra_reserve = 0);

	public:
		string_list create(data_slice entropy, const dictionary &lexicon = language::en);
		bool isValid(const string_list& mnemonic, const dictionary &lexicon);
		bool isValid(const string_list& mnemonic, const dictionary_list& lexicons = language::all);
		long_hash decode(const string_list& mnemonic);
#ifdef WITH_ICU
		long_hash decode(const string_list& mnemonic, const std::string& passphrase);
#endif
	};
}
#endif
