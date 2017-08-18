#include "BIP39.h"
#include "../collection.h"
#include "../hash.h"
#include "../../sha3_test/src/sha3.h"
#include <cstdint>
#include "boost/algorithm/string.hpp"

namespace BIP39
{
	uint8_t Mnemonic::shift(size_t bit)
	{
		return (1 << (byte_bits - (bit % byte_bits) - 1));
	}

	data_chunk Mnemonic::build_chunk(loaf slices, size_t extra_reserve)
	{
		size_t size = 0;
		for (const auto slice : slices)
			size += slice.size();

		data_chunk out;
		out.reserve(size + extra_reserve);
		for (const auto slice : slices)
			out.insert(out.end(), slice.begin(), slice.end());

		return out;
	}

	string_list Mnemonic::create(data_slice entropy, const dictionary &lexicon)
	{
		if ((entropy.size() % mnemonic_seed_multiple) != 0)
			return string_list();

		const size_t entropy_bits = (entropy.size() * byte_bits);
		const size_t check_bits = (entropy_bits / entropy_bit_divisor);
		const size_t total_bits = (entropy_bits + check_bits);
		const size_t word_count = (total_bits / bits_per_word);

		hash_digest hash;
		SHA3 crypto;
		memcpy(hash.data(), crypto.hash(SHA3::HashType::DEFAULT, SHA3::HashSize::SHA3_256, entropy.data(), entropy.size()), 32);

		const auto data = build_chunk({ entropy, hash });

		size_t bit = 0;
		string_list words;

		for (size_t word = 0; word < word_count; word++)
		{
			size_t position = 0;
			for (size_t loop = 0; loop < bits_per_word; loop++)
			{
				bit = (word * bits_per_word + loop);
				position <<= 1;

				const auto byte = bit / byte_bits;

				if ((data[byte] & shift(bit)) > 0)
					position++;
			}

			words.push_back(lexicon[position]);
		}

		return words;
	}

	bool Mnemonic::isValid(const string_list& words, const dictionary& lexicon)
	{
		const auto word_count = words.size();
		if ((word_count % mnemonic_word_multiple) != 0)
			return false;

		const auto total_bits = bits_per_word * word_count;
		const auto check_bits = total_bits / (entropy_bit_divisor + 1);
		const auto entropy_bits = total_bits - check_bits;

		size_t bit = 0;
		data_chunk data((total_bits + byte_bits - 1) / byte_bits, 0);

		for (const auto& word : words)
		{
			const auto position = find_position(lexicon, word);
			if (position == -1)
				return false;

			for (size_t loop = 0; loop < bits_per_word; loop++, bit++)
			{
				if (position & (1 << (bits_per_word - loop - 1)))
				{
					const auto byte = bit / byte_bits;
					data[byte] |= shift(bit);
				}
			}
		}

		data.resize(entropy_bits / byte_bits);

		const auto mnemonic = create(data, lexicon);
		return std::equal(mnemonic.begin(), mnemonic.end(), words.begin());
	}

	bool Mnemonic::isValid(const string_list& mnemonic,	const dictionary_list& lexicons)
	{
		for (const auto& lexicon : lexicons)
			if (isValid(mnemonic, *lexicon))
				return true;

		return false;
	}

	long_hash Mnemonic::decode(const string_list& mnemonic)
	{
		const auto sentence = boost::join(mnemonic, " ");
		const std::string salt(passphrase_prefix);
		return pkcs5_pbkdf2_hmac_sha512(to_chunk(sentence), to_chunk(salt),	hmac_iterations);
	}

#ifdef WITH_ICU

	long_hash decode_mnemonic(const string_list& mnemonic, const std::string& passphrase)
	{
		const auto sentence = boost::join(mnemonic, " " );
		const std::string prefix(passphrase_prefix);
		const auto salt = to_normal_nfkd_form(prefix + passphrase);
		return pkcs5_pbkdf2_hmac_sha512(to_chunk(sentence), to_chunk(salt),	hmac_iterations);
	}

#endif
}