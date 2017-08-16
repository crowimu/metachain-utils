#pragma once

#ifndef LIBBITCOIN_CONFIG_BASE16_HPP
#define LIBBITCOIN_CONFIG_BASE16_HPP

#include <array>
#include <iostream>
#include <string>
#include <cstdint>
#include "BIP39/BIP39.h"
namespace BIP39
{
	/**
	* Serialization helper for base16 encoded data.
	*/
	class base16
	{
	public:

		/**
		* Default constructor.
		*/
		base16();

		/**
		* Initialization constructor.
		* @param[in]  hexcode  The value to initialize with.
		*/
		base16(const std::string& hexcode);

		/**
		* Initialization constructor.
		* @param[in]  value  The value to initialize with.
		*/
		base16(const data_chunk& value);

		/**
		* Initialization constructor.
		* @param[in]  value  The value to initialize with.
		*/
		template<size_t Size>
		base16(const byte_array<Size>& value)
			: value_(value.begin(), value.end())
		{
		}

		/**
		* Copy constructor.
		* @param[in]  other  The object to copy into self on construct.
		*/
		base16(const base16& other);

		/**
		* Overload cast to internal type.
		* @return  This object's value cast to internal type reference.
		*/
		operator const data_chunk&() const;

		/**
		* Overload cast to generic data reference.
		* @return  This object's value cast to a generic data.
		*/
		operator data_slice() const;

		/**
		* Overload stream in. If input is invalid sets no bytes in argument.
		* @param[in]   input     The input stream to read the value from.
		* @param[out]  argument  The object to receive the read value.
		* @return                The input stream reference.
		*/
		friend std::istream& operator>>(std::istream& input,
			base16& argument);

		/**
		* Overload stream out.
		* @param[in]   output    The output stream to write the value to.
		* @param[out]  argument  The object from which to obtain the value.
		* @return                The output stream reference.
		*/
		friend std::ostream& operator<<(std::ostream& output,
			const base16& argument);

	private:

		/**
		* The state of this object.
		*/
		data_chunk value_;

	};


	std::string encode_base16(data_slice data);
	bool is_base16(const char c);
	




	/**
	* Convert a hex string into bytes.
	* @return false if the input is malformed.
	*/
	bool decode_base16(data_chunk& out, const std::string &in);

	/**
	* Converts a hex string to a number of bytes.
	* @return false if the input is malformed, or the wrong length.
	*/
	template <size_t Size>
	bool decode_base16(byte_array<Size>& out, const std::string &in);



	// For template implementation only, do not call directly.
	bool decode_base16_private(uint8_t* out, size_t out_size,
		const char* in);

	template <size_t Size>
	bool decode_base16(byte_array<Size>& out, const std::string &in)
	{
		if (in.size() != 2 * Size)
			return false;

		byte_array<Size> result;
		if (!decode_base16_private(result.data(), result.size(), in.data()))
			return false;

		out = result;
		return true;
	}
}
#endif
