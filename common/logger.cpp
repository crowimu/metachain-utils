/*********************************************************************
* Copyright (c) 2017 TCT DEVs	                                     *
* Distributed under the GPLv3.0 software license					 *
* contact us before using our code									 *
**********************************************************************/

#include "logger.h"

#define __STDC_WANT_LIB_EXT1__ 1
#include <time.h>
#include <iostream>

Logger::Logger()
{
	this->log("testing logging facility");
}

Logger& Logger::getInstance()
{
	static Logger instance;
	return instance;
}

Logger::~Logger()
{
}

void Logger::log( std::string strLogLine, facility logFacility, std::string strModule)
{
	/*
		Time processing
	*/
		// get the current time
		this->m_timeCur = time(0);

		// clear the buffer
		memset(this->m_caTimeBuf, '\0', sizeof(this->m_caTimeBuf));

		// get the structure
		#ifdef _WIN32
			localtime_s(&this->m_timeStruct, &this->m_timeCur);
		#else
			localtime_r(&this->m_timeCur, &this->m_timeStruct);
		#endif

		// format the time
		strftime(this->m_caTimeBuf, sizeof(this->m_caTimeBuf), "%Y-%m-%d %X:", &this->m_timeStruct);

	/*
		Facility processing
	*/
		m_strFacility = " [";
	switch (logFacility)
	{
		case facility::debug:		m_strFacility += "DEBUG";		break;
		case facility::info:		m_strFacility += "INFO";		break;
		case facility::warning:		m_strFacility += "WARNING";	break;
		case facility::error:		m_strFacility += "ERROR";		break;
		default:					m_strFacility += "UNKNOWN";
	}
	m_strFacility += "] ";

	// make the strModule more fancy if set
	if (strModule != "")
		strModule = " [" + strModule + "] ";	
	
	// output to stdout
	std::cout << this->m_caTimeBuf << m_strFacility << strModule << strLogLine << std::endl;
}