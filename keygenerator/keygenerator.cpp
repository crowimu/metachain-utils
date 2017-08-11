/*********************************************************************
* Copyright (c) 2017 TCT DEVs	                                     *
* Distributed under the GPLv3.0 software license					 *
* contact us before using our code									 *
**********************************************************************/


#include "../common/ArgsManager.h"
#include "../common/logger.h"

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
	if (IsArgSet("-?") || IsArgSet("-h") || IsArgSet("--help") || argc==1)
	{
		LOGS("Arguments:");
		LOGS("-?, -h, --help: print this help and exit");
		getchar();
		return 1;
	}
	/*
	// load our ini file and create our iniFile object
	std::string strIni;
	if (IsArgSet("-c"))
		strIni = GetArg("-c", "node.ini");
	else if (IsArgSet("--conf"))
		strIni = GetArg("--conf", "node.ini");
	else
		strIni = "node.ini";
		*/
	getchar();
	return 1;
}
