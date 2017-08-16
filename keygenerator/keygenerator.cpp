/*********************************************************************
* Copyright (c) 2017 TCT DEVs	                                     *
* Distributed under the GPLv3.0 software license					 *
* contact us before using our code									 *
**********************************************************************/


#include "../common/ArgsManager.h"
#include "../common/logger.h"
#include "src/BIP39/BIP39.h"
#include "src/BIP39/dictionairy.h"
#include "boost/algorithm/string.hpp"
#include "src/base16.h"
#include <sstream>

#ifdef _WIN32
	#include <SDKDDKVer.h>
	#ifdef _MSC_VER
		#include <basetsd.h>
	#endif
#endif

// argument manager
ArgsManager gArgs;

// main entry point
int main(int argc, char* argv[])
{
	// borrowed from the bitcoin core we initialize the argmanager and parse the arguments if provided
	ParseParameters(argc, argv);

	LOGS("MetaChain-Utils: keygenerator");
	LOGS("=============================");

	// Process help and version
	/*if (IsArgSet("-?") || IsArgSet("-h") || IsArgSet("--help") || argc==1)
	{
		LOGS("Arguments:");
		LOGS("-?, -h, --help: print this help and exit");
		getchar();
		return 1;
	}*/

	// Bound parameters.
	BIP39::dictionary_list tmp { &BIP39::language::en };

	BIP39::base16 test("7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fdedefefe");
	const BIP39::dictionary_list& language = tmp;
	const BIP39::data_chunk& entropy (test);

	const auto entropy_size = entropy.size();

	if ((entropy_size % BIP39::mnemonic_seed_multiple) != 0)
	{
		LOG_ERROR("Mnemonic invalid entropy", "KEYGEN");
		getchar();
		return 1;
	}

	// If 'any' default to first ('en'), otherwise the one specified.
	const auto dictionary = language.front();
	BIP39::Mnemonic mnem;
	const auto words = mnem.create(entropy, *dictionary);

	LOGS( "Mnemonic: " + boost::join(words, " ") );

	LOGS("Is Mnemonic valid: " + std::to_string(mnem.isValid(words, *dictionary)));

	LOGS("Decode: " + BIP39::encode_base16(mnem.decode(words)));

	getchar();
	return 1;
}
