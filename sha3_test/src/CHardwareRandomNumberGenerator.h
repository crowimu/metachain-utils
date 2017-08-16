//This class uses the Intel RdRand CPU instruction for 
//the random number generator that is compliant with security 
//and cryptographic standards:
//
//  http://en.wikipedia.org/wiki/RdRand
//
#pragma once

#include <Windows.h>
#include <assert.h>

class CHardwareRandomNumberGenerator
{
public:
	CHardwareRandomNumberGenerator(void);
	~CHardwareRandomNumberGenerator(void);
	int GetHardwareRandomBytes(BYTE* pOutRndVals = NULL, UINT* pncbInOutSzRndVals = NULL, DWORD dwmsMaxWait = 5 * 1000);
private:
	BOOL bRdRandSupported;
	static BOOL __is_cpuid_supported(void);
	static BOOL __cpuid(int data[4], int nID);
	int __fillHardwareRandomBytes(BYTE* pOutRndVals, UINT* pncbInOutSzRndVals, UINT& ncbOutSzWritten, DWORD dwmsMaxWait);
};