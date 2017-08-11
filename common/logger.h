#pragma once

/*********************************************************************
* Copyright (c) 2017 TCT DEVs	                                     *
* Distributed under the GPLv3.0 software license					 *
* contact us before using our code									 *
**********************************************************************/

#ifndef __LOGGER_H__
#define __LOGGER_H__ 1

#include <string>
#include <fstream>

/*
	stripped down singleton logger
*/
class Logger
{
	private:
		// variables for faster time processing
		time_t						m_timeCur;
		tm							m_timeStruct;
		char						m_caTimeBuf[80];

		// variables for faster facility processing
		std::string					m_strFacility;
		
		// constructor and operator
									Logger();
									Logger(Logger const& copy);					// not implemented
		Logger&						operator=(Logger const& copy);	// not implemented

	public:
		enum facility {
			debug,
			info,
			warning,
			error
		};

		static						Logger& getInstance();
									~Logger();
		void						log(std::string strLogLine, facility logFacility = facility::info, std::string strModule = "");
};

// logging macros for faster development, interchangeability and readability
#define LOG(logline, module) Logger::getInstance().log(logline, Logger::facility::info, module)
#define LOGS(logline) Logger::getInstance().log(logline, Logger::facility::info )
#define LOG_WARNING(logline, module) Logger::getInstance().log(logline, Logger::facility::warning, module)
#define LOG_DEBUG(logline, module) Logger::getInstance().log(logline, Logger::facility::debug, module)
#define LOG_ERROR(logline, module) Logger::getInstance().log(logline, Logger::facility::error, module)

#endif