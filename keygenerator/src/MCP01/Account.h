#pragma once

/*********************************************************************
* Copyright (c) 2017 TCT DEVs	                                     *
* Distributed under the GPLv3.0 software license					 *
* contact us before using our code									 *
**********************************************************************/

// MCP01 Account functions: Wallet Address Generation, Checking

#ifndef __MCP01_ACCOUNT_H__
#define __MCP01_ACCOUNT_H__

#include <string>
#include "../MCP39/Mnemonic.h"
#include "../base16.h"

namespace MCP01
{
#define WALLET_ADDRESS_STD_PREFIX "MC"
#define WALLET_ADDRESS_VERSION 1

	class Account
	{
		public:
			enum ECDSA {
				SECP256k1,
				SECP256r1,
				not_calculated
			};
										Account(uint8_t *keyPriv);
										Account(MCP39::long_hash keyPriv);
										Account(uint8_t *keyPriv, uint8_t *keyPub, ECDSA ecdsaPubKey);
										~Account();

			bool						calcPubKey(ECDSA type);
			bool						calcWalletAddress(uint8_t* walletAddress, uint16_t uiChainIdentifier, bool bTestNet = false);

			uint8_t*					getPrivKey() { return m_keyPriv; };
			std::string					getPrivKeyStr() { return encode_base16(m_keyPriv, 64); };
			uint8_t*					getPubKey() { return m_keyPub; };
			std::string					getPubKeyStr() { return encode_base16(m_keyPub, 64); };

			std::string					getWalletAddress(std::string strChainIdentifier, bool bTestNet = false);
			bool						verifyWalletAddress(std::string);

		private:
			uint8_t						m_keyPriv[64];
			uint8_t						m_keyPub[64];
			ECDSA						m_ecdsaPubKey;

			std::string					tokenize(std::string strWalletAddress);
			std::string					untokenize(std::string strWalletAddress);
	};
}
#endif