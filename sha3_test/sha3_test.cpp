/*********************************************************************
* Copyright (c) 2017 TCT DEVs	                                     *
* Distributed under the GPLv3.0 software license					 *
* contact us before using our code									 *
**********************************************************************/


#include "../common/ArgsManager.h"
#include "../common/logger.h"
#include "src/sha3.h"
#include <chrono>

#ifdef _WIN32
	#include "src\CHardwareRandomNumberGenerator.h"
	#include <SDKDDKVer.h>
	#ifdef _MSC_VER
		#include <basetsd.h>
	#endif
#endif

// small inline function for easier processing of the tested output
void __inline TestHash(SHA3::HashType HashType, SHA3::HashSize HashSize, char *pcBuffer, unsigned int uiLength, unsigned int uiDigestLength = 0 )
{
	SHA3 crypto;
	std::chrono::high_resolution_clock::time_point t1, t2;
	std::string strResult, strFacility = "";

	t1 = std::chrono::high_resolution_clock::now();
	strResult = crypto.to_string(crypto.hash(HashType, HashSize, (uint8_t*)pcBuffer, uiLength, uiDigestLength), HashSize);
	t2 = std::chrono::high_resolution_clock::now();

	switch (HashType)
	{
	case SHA3::HashType::DEFAULT:
		strFacility = "SHA3";
		break;
	case SHA3::HashType::KECCAK:
		strFacility = "KECCAK";
		break;
	case SHA3::HashType::SHAKE:
		strFacility = "SHAKE";
		break;
	}

	strFacility += "-" + std::to_string(HashSize) + " (" + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());

#ifdef _WIN32
	strFacility += " \xE6s)";
#else
	strFacility += " \xC2\xB5s)";
#endif

	LOG("Result: " + strResult, strFacility);
};

// argument manager
ArgsManager gArgs;

// main entry point
int main(int argc, char* argv[])
{
	// borrowed from the bitcoin core we initialize the argmanager and parse the arguments if provided
	ParseParameters(argc, argv);

	LOGS("MetaChain-Utils: sha3_test");
	LOGS("=========================================================");

	// Process help and version
	if (IsArgSet("-?") || IsArgSet("-h") || IsArgSet("--help") || argc==1)
	{
		LOGS("Arguments:");
		LOGS("-?, -h, --help: print this help and exit");
		LOGS("--test-cshake: test cshake implementation. requirements:");
		LOGS("            -x=<string>: message to be hashed, randomly created when empty");
		LOGS("            -s=<string>: customization string, may be empty");
		LOGS("            -n=<string>: name string, may be empty");
		LOGS("--test-kmac: test kmac implementation. requirements:");
		LOGS("            -x=<string>: message to be hashed, randomly created when empty");
		LOGS("            -k=<string> key to encrypt the message, may be empty");
		LOGS("--test: test different parameters and check the SHA3 algorithm");
		getchar();
		return 1;
	}

	if (IsArgSet("--test-cshake"))
	{
		LOGS("Testing cShake");
		LOGS("=========================================================");

		std::string strInput = GetArg("-x", "");
		std::string strCustomization = GetArg("-s", "");
		std::string strName = GetArg("-n", "");

		if (strInput == "")
		{
			unsigned int uiBufferSize = 256;
			char *pcBuffer = (char*)malloc(uiBufferSize);
			memset(pcBuffer, '\0', uiBufferSize);

#ifdef _WIN32
			CHardwareRandomNumberGenerator h;
			h.GetHardwareRandomBytes((BYTE*)pcBuffer, &uiBufferSize);
#else
			int fd = open("/dev/random", O_RDONLY);
			read(fd, pcBuffer, uiBufferSize);
#endif
			strInput = pcBuffer;
		}
		LOGS("Parameters:");
		LOGS("Input: " + strInput);
		LOGS("Customization: " + strCustomization);
		LOGS("Function Name: " + strName);
		LOGS("=========================================================");

		SHA3 crypto;
		std::chrono::high_resolution_clock::time_point t1, t2;
		std::string strResult, strFacility = "";

		// 128 bit
		t1 = std::chrono::high_resolution_clock::now();
		strResult = crypto.to_string(crypto.cShake(SHA3::HashSize::SHA3_128, (const uint8_t*)strInput.c_str(), strInput.length(), SHA3::HashSize::SHA3_128*2, strName, strCustomization), SHA3::HashSize::SHA3_128*2);
		t2 = std::chrono::high_resolution_clock::now();
		
		strFacility += "cSHAKE-128 (" + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
#ifdef _WIN32
		strFacility += " \xE6s)";
#else
		strFacility += " \xC2\xB5s)";
#endif
		LOG("Result: " + strResult, strFacility);

		// 256 bit
		strResult, strFacility = "";
		t1 = std::chrono::high_resolution_clock::now();
		strResult = crypto.to_string(crypto.cShake(SHA3::HashSize::SHA3_256, (const uint8_t*)strInput.c_str(), strInput.length(), SHA3::HashSize::SHA3_256 * 2, strName, strCustomization), SHA3::HashSize::SHA3_256 * 2);
		t2 = std::chrono::high_resolution_clock::now();

		strFacility += "cSHAKE-256 (" + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());

#ifdef _WIN32
		strFacility += " \xE6s)";
#else
		strFacility += " \xC2\xB5s)";
#endif

		LOG("Result: " + strResult, strFacility);
	}
	else if (IsArgSet("--test-kmac"))
	{
		LOGS("Testing KMAC");
		LOGS("=========================================================");

		std::string strInput = GetArg("-x", "");
		std::string strCustomization = GetArg("-s", "");
		std::string strKey = GetArg("-k", "");

		if (strInput == "")
		{
			unsigned int uiBufferSize = 256;
			char *pcBuffer = (char*)malloc(uiBufferSize);
			memset(pcBuffer, '\0', uiBufferSize);

#ifdef _WIN32
			CHardwareRandomNumberGenerator h;
			h.GetHardwareRandomBytes((BYTE*)pcBuffer, &uiBufferSize);
#else
			int fd = open("/dev/random", O_RDONLY);
			read(fd, pcBuffer, uiBufferSize);
#endif
			strInput = pcBuffer;
		}
		LOGS("Parameters:");
		LOGS("Input: " + strInput);
		LOGS("Customization: " + strCustomization);
		LOGS("Key: " + strKey);
		LOGS("=========================================================");

		SHA3 crypto;
		std::chrono::high_resolution_clock::time_point t1, t2;
		std::string strResult, strFacility = "";

		// 128 bit
		t1 = std::chrono::high_resolution_clock::now();
		strResult = crypto.to_string(crypto.kmac(SHA3::HashSize::SHA3_128, (const uint8_t*)strInput.c_str(), strInput.length(), SHA3::HashSize::SHA3_128 * 2, strKey, strCustomization), SHA3::HashSize::SHA3_128 * 2);
		t2 = std::chrono::high_resolution_clock::now();

		strFacility += "KMAC-128 (" + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
#ifdef _WIN32
		strFacility += " \xE6s)";
#else
		strFacility += " \xC2\xB5s)";
#endif
		LOG("Result: " + strResult, strFacility);

		// 256 bit
		strResult, strFacility = "";
		t1 = std::chrono::high_resolution_clock::now();
		strResult = crypto.to_string(crypto.kmac(SHA3::HashSize::SHA3_256, (const uint8_t*)strInput.c_str(), strInput.length(), SHA3::HashSize::SHA3_256 * 2, strKey, strCustomization), SHA3::HashSize::SHA3_256 * 2);
		t2 = std::chrono::high_resolution_clock::now();

		strFacility += "KMAC-256 (" + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());

#ifdef _WIN32
		strFacility += " \xE6s)";
#else
		strFacility += " \xC2\xB5s)";
#endif

		LOG("Result: " + strResult, strFacility);
	}
	else if (IsArgSet("--test"))
	{
		LOG("Testing small hardcoded string", "HashSize [bit]: 224");
		TestHash(SHA3::HashType::DEFAULT, SHA3::HashSize::SHA3_224, "test", 32 );
		TestHash(SHA3::HashType::KECCAK, SHA3::HashSize::SHA3_224, "test", 32 );
		TestHash(SHA3::HashType::SHAKE, SHA3::HashSize::SHA3_224, "test", 32, 224 );
		LOG("Testing small hardcoded string", "HashSize [bit]: 256");
		TestHash(SHA3::HashType::DEFAULT, SHA3::HashSize::SHA3_256, "test", 32 );
		TestHash(SHA3::HashType::KECCAK, SHA3::HashSize::SHA3_256, "test", 32 );
		TestHash(SHA3::HashType::SHAKE, SHA3::HashSize::SHA3_256, "test", 32, 256 );
		LOG("Testing small hardcoded string", "HashSize [bit]: 384");
		TestHash(SHA3::HashType::DEFAULT, SHA3::HashSize::SHA3_384, "test", 32 );
		TestHash(SHA3::HashType::KECCAK, SHA3::HashSize::SHA3_384, "test", 32 );
		TestHash(SHA3::HashType::SHAKE, SHA3::HashSize::SHA3_384, "test", 32, 384 );
		LOG("Testing small hardcoded string", "HashSize [bit]: 512");
		TestHash(SHA3::HashType::DEFAULT, SHA3::HashSize::SHA3_512, "test", 32 );
		TestHash(SHA3::HashType::KECCAK, SHA3::HashSize::SHA3_512, "test", 32 );
		TestHash(SHA3::HashType::SHAKE, SHA3::HashSize::SHA3_512, "test", 32, 512 );

		LOGS("=========================================================");

		unsigned int uiBufferSize = 256;
		for (int i = 1; i < 6; i++)
		{
			uiBufferSize *= 2;
			LOGS("Testing random buffer size: " + std::to_string(uiBufferSize));

			char *pcBuffer = (char*)malloc(uiBufferSize);
			memset(pcBuffer, '\0', uiBufferSize);

#ifdef _WIN32
			CHardwareRandomNumberGenerator h;
			h.GetHardwareRandomBytes((BYTE*)pcBuffer, &uiBufferSize);
#else
			int fd = open("/dev/random", O_RDONLY);
			read(fd, pcBuffer, uiBufferSize);
#endif

			TestHash(SHA3::HashType::DEFAULT, SHA3::HashSize::SHA3_224, pcBuffer, uiBufferSize);
			TestHash(SHA3::HashType::KECCAK, SHA3::HashSize::SHA3_224, pcBuffer, uiBufferSize);
			TestHash(SHA3::HashType::SHAKE, SHA3::HashSize::SHA3_224, pcBuffer, uiBufferSize, 224);

			TestHash(SHA3::HashType::DEFAULT, SHA3::HashSize::SHA3_256, pcBuffer, uiBufferSize);
			TestHash(SHA3::HashType::KECCAK, SHA3::HashSize::SHA3_256, pcBuffer, uiBufferSize);
			TestHash(SHA3::HashType::SHAKE, SHA3::HashSize::SHA3_256, pcBuffer, uiBufferSize, 256);

			TestHash(SHA3::HashType::DEFAULT, SHA3::HashSize::SHA3_384, pcBuffer, uiBufferSize);
			TestHash(SHA3::HashType::KECCAK, SHA3::HashSize::SHA3_384, pcBuffer, uiBufferSize);
			TestHash(SHA3::HashType::SHAKE, SHA3::HashSize::SHA3_384, pcBuffer, uiBufferSize, 384);

			TestHash(SHA3::HashType::DEFAULT, SHA3::HashSize::SHA3_512, pcBuffer, uiBufferSize);
			TestHash(SHA3::HashType::KECCAK, SHA3::HashSize::SHA3_512, pcBuffer, uiBufferSize);
			TestHash(SHA3::HashType::SHAKE, SHA3::HashSize::SHA3_512, pcBuffer, uiBufferSize, 512);


			LOGS("=========================================================");
			delete pcBuffer;
		}
	}
	
	getchar();
	return 1;
}
