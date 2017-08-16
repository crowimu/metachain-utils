//This class uses the Intel RdRand CPU instruction for 
//the random number generator that is compliant with security 
//and cryptographic standards:
//
//  http://en.wikipedia.org/wiki/RdRand
//
//[32-bit Intel-only implementation]
//
#include "CHardwareRandomNumberGenerator.h"

CHardwareRandomNumberGenerator::CHardwareRandomNumberGenerator(void) :
	bRdRandSupported(FALSE)
{
	//Check that RdRand instruction is supported
	if (__is_cpuid_supported())
	{
		//It must be Intel CPU
		int name[4] = { 0 };
		if (__cpuid(name, 0))
		{
			if (name[1] == 0x756e6547 &&         //uneG
				name[2] == 0x6c65746e &&        //letn
				name[3] == 0x49656e69)          //Ieni
			{
				//Get flag itself
				int data[4] = { 0 };
				if (__cpuid(data, 1))
				{
					//Check bit 30 on the 2nd index (ECX register)
					if (data[2] & (0x1 << 30))
					{
						//Supported!
						bRdRandSupported = TRUE;
					}
				}
			}
		}
	}
}

CHardwareRandomNumberGenerator::~CHardwareRandomNumberGenerator(void)
{
}


int CHardwareRandomNumberGenerator::GetHardwareRandomBytes(BYTE* pOutRndVals, UINT* pncbInOutSzRndVals, DWORD dwmsMaxWait)
{
	//Generate random numbers into the 'pOutRndVals' buffer
	//INFO: This function uses CPU/hardware to generate a set of
	//      random numbers that are cryptographically strong.
	//INFO: For more details refer to:
	//       http://electronicdesign.com/learning-resources/understanding-intels-ivy-bridge-random-number-generator
	//INFO: To review the "ANALYSIS OF INTEL’S IVY BRIDGE DIGITAL RANDOM NUMBER GENERATOR" check:
	//       http://www.cryptography.com/public/pdf/Intel_TRNG_Report_20120312.pdf
	//'pOutRndVals' = if not NULL, points to the buffer that receives random bytes
	//'pncbInOutSzRndVals' = if not NULL, on the input must contain the number of BYTEs to write into the 'pOutRndVals' buffer
	//                                    on the output will contain the number of BYTEs actually written into the 'pOutRndVals' buffer
	//'dwmsMaxWait' = timeout for this method, expressed in milliseconds
	//RETURN:
	//      = 1 if hardware random number generator is supported & the buffer in 'pOutRndVals' was successfully filled out with random numbers
	//      = 0 if hardware random number generator is supported, but timed out while filling out the buffer in 'pOutRndVals'
	//          INFO: Check 'pncbInOutSzRndVals', it will contain the number of BYTEs actually written into the 'pOutRndVals' array
	//      = -1 if general error
	//      = -2 if hardware random number generator is not supported on this hardware
	//          INFO: Requires Intel Ivy Bridge, or later chipset.

	UINT ncbSzWritten = 0;
	int nRes = __fillHardwareRandomBytes(pOutRndVals, pncbInOutSzRndVals, ncbSzWritten, dwmsMaxWait);

	if (pncbInOutSzRndVals)
		*pncbInOutSzRndVals = ncbSzWritten;

	return nRes;
}

int CHardwareRandomNumberGenerator::__fillHardwareRandomBytes(BYTE* pOutRndVals, UINT* pncbInOutSzRndVals, UINT& ncbOutSzWritten, DWORD dwmsMaxWait)
{
	//INTERNAL METHOD

	ncbOutSzWritten = 0;

	//Check support
	if (!bRdRandSupported)
		return -2;

	__try
	{
		//We must have a buffer to fill out
		if (pOutRndVals &&
			pncbInOutSzRndVals &&
			(int*)*pncbInOutSzRndVals > 0)
		{
			//Begin timing ticks in ms
			DWORD dwmsIniTicks = ::GetTickCount();

			UINT ncbSzRndVals = *pncbInOutSzRndVals;

			//Fill in data array
			for (UINT i = 0; i < ncbSzRndVals; i += sizeof(DWORD))
			{
				DWORD random_value;
				int got_value;

				int nFailureCount = 0;

				//Since RdRand instruction may not have enough random numbers
				//in its buffer, we may need to "loop" while waiting for it to
				//generate more results...
				//For the first 10 failures we'll simply loop around, after which we
				//will wait for 1 ms per each failed iteration to save on the overall
				//CPU cycles that this method may consume.
				for (;; nFailureCount++ < 10 ? 1 : ::Sleep(1))
				{
					__asm
					{
						push eax
						push edx
						xor eax, eax

						; RDRAND instruction = Set random value into EAX.Will set overflow[C] flag if success
						_emit 0x0F
						_emit 0xC7
						_emit 0xF0

						mov edx, 1

						; Check if the value was available in the RNG buffer
						jc lbl_set_it

						; It wasn't available
						xor edx, edx
						xor eax, eax
						lbl_set_it :
						mov dword ptr[got_value], edx
							mov dword ptr[random_value], eax

							pop edx
							pop eax
					}

					if (got_value)
					{
						//Got random value OK
						break;
					}

					//Otherwise RdRand instruction failed to produce a random value

					//See if we timed out?
					if (::GetTickCount() - dwmsIniTicks > dwmsMaxWait)
					{
						//Timed out
						return 0;
					}

					//Try again
				}

				//We now have a 4-byte, or DWORD, random value
				//So let's put it into our array
				if (i + sizeof(DWORD) <= ncbSzRndVals)
				{
					*(DWORD*)(pOutRndVals + i) = random_value;
					ncbOutSzWritten += sizeof(DWORD);
				}
				else if (i + sizeof(WORD) + sizeof(BYTE) <= ncbSzRndVals)
				{
					*(WORD*)(pOutRndVals + i) = (WORD)random_value;
					*(BYTE*)(pOutRndVals + i + sizeof(WORD)) = (BYTE)(random_value >> 16);
					ncbOutSzWritten += sizeof(WORD) + sizeof(BYTE);
				}
				else if (i + sizeof(WORD) <= ncbSzRndVals)
				{
					*(WORD*)(pOutRndVals + i) = (WORD)random_value;
					ncbOutSzWritten += sizeof(WORD);
				}
				else if (i + sizeof(BYTE) <= ncbSzRndVals)
				{
					*(BYTE*)(pOutRndVals + i) = (BYTE)random_value;
					ncbOutSzWritten += sizeof(BYTE);
				}
				else
				{
					//Shouldn't even be here
					assert(NULL);
					return -1;
				}
			}
		}
	}
	__except (1)
	{
		//A generic catch-all just to be sure...
		return -1;
	}

	return 1;
}


BOOL CHardwareRandomNumberGenerator::__is_cpuid_supported(void)
{
	//See if CPUID command is supported
	//INFO: Some really old CPUs may not support it!
	//RETURN: = TRUE if yes, and __cpuid() can be called
	BOOL bSupported;
	DWORD nEFlags = 0;

	__try
	{
#define FLAG_VALUE (0x1 << 21)

		_asm
		{
			//remember EFLAGS & EAX
			pushfd
			push eax

			//Set bit 21 in EFLAGS
			pushfd
			pop eax
			or eax, FLAG_VALUE
			push eax
			popfd

			//Check if bit 21 in EFLAGS was set
			pushfd
			pop eax
			mov nEFlags, eax

			//Restore EFLAGS & EAX
			pop eax
			popfd
		}

		bSupported = (nEFlags & FLAG_VALUE) ? TRUE : FALSE;
	}
	__except (1)
	{
		//A generic catch-all just to be sure...
		bSupported = FALSE;
	}

	return bSupported;
}

BOOL CHardwareRandomNumberGenerator::__cpuid(int data[4], int nID)
{
	//INFO: Call __is_cpuid_supported() first to see if this function is supported
	//RETURN:
	//      = TRUE if success, check 'data' for results
	BOOL bRes = TRUE;

	__try
	{
		_asm
		{
			push eax
			push ebx
			push ecx
			push edx
			push esi

			//Call CPUID
			mov eax, nID
			_emit 0x0f; CPUID
			_emit 0xa2

			//Save 4 registers
			mov esi, data
			mov dword ptr[esi], eax
			mov dword ptr[esi + 4], ebx
			mov dword ptr[esi + 8], ecx
			mov dword ptr[esi + 12], edx

			pop esi
			pop edx
			pop ecx
			pop ebx
			pop eax
		}

	}
	__except (1)
	{
		//A generic catch-all just to be sure...
		bRes = FALSE;
	}

	return bRes;
}