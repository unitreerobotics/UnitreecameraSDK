/**
  * @file SystemLog.hpp
  * @brief This file is part of UnitreeCameraSDK, which declare the APIs of log system.  
  * @details The log system is used for unified management of the output information of the special system.
  * @author ZhangChunyang
  * @date  2021.07.31
  * @version 1.0.1
  * @copyright Copyright (c)2020-2021, Hangzhou Yushu Technology Stock CO.LTD. All Rights Reserved.
  */

#ifndef __SYSTEMLOG_HPP__
#define __SYSTEMLOG_HPP__

#include <stdarg.h>
#include <stdio.h>
#include <string>

class SystemLog
{
private:
    int m_logLevel;
    bool m_isSave;
    std::string m_logName;
    std::string m_logFileName;
    
public:
    /**
      * @fn SystemLog
      * @brief SystemLog constructor
      * @details 
      * @param[in] logName the name of special system
      * @param[out] None
      * @return None
      * @note 
      * @attention output information starts with "logName"
      * @code
      * @endcode
      */
    SystemLog(std::string logName);
    
public:    
    /**
      * @fn setLogLevel
      * @brief set system ouput log level 
      * @details log level means that different kind of information will be output
      * @param[in] level 1 running information output, 2 running and debug infomation output
      * @param[out] None
      * @return None
      * @note 
      * @attention 
      * @code
      * @endcode
      */
    void setLogLevel(int level);
    /**
      * @fn runTimeError
      * @brief output running error infomation, color: red
      * @details output format [logName][ERROR] info
      * @param[in] format 
      * @param[in] ...
      * @param[out] None
      * @return None
      * @note 
      * @attention use it like printf()
      * @code
      * @endcode
      */
    void runTimeError(const char *format,...);
    /**
      * @fn runTimeInfo
      * @brief output running infomation, color: green
      * @details output format [logName][INFO] info
      * @param[in] format 
      * @param[in] ...
      * @param[out] None
      * @return None
      * @note 
      * @attention use it like printf()
      * @code
      * @endcode
      */
    void runTimeInfo(const char *format,...);
    /**
      * @fn runTimeWarning
      * @brief output running warning infomation, color: yellow
      * @details output format [logName][WARNING] info
      * @param[in] format 
      * @param[in] ...
      * @param[out] None
      * @return None
      * @note 
      * @attention use it like printf()
      * @code
      * @endcode
      */
    void runTimeWarning(const char *format,...);
    /**
      * @fn debugTimeWarning
      * @brief output debug warning infomation, color: cyan
      * @details output format [logName][DEBUG_WARNING] info
      * @param[in] format 
      * @param[in] ...
      * @param[out] None
      * @return None
      * @note 
      * @attention use it like printf()
      * @code
      * @endcode
      */
    void debugTimeWarning(const char *format,...);
    /**
      * @fn debugTimeInfo
      * @brief output debug infomation, color: white
      * @details output format [logName][DEBUG_INFO] info
      * @param[in] format 
      * @param[in] ...
      * @param[out] None
      * @return None
      * @note 
      * @attention use it like printf()
      * @code
      * @endcode
      */
    void debugTimeInfo(const char *format,...);
    /**
      * @fn debugTimeError
      * @brief output debug error infomation, color: magenta
      * @details output format [logName][DEBUG_ERROR] info
      * @param[in] format 
      * @param[in] ...
      * @param[out] None
      * @return None
      * @note 
      * @attention use it like printf()
      * @code
      * @endcode
      */
    void debugTimeError(const char *format,...);
    /**
      * @fn saveLog
      * @brief save system log to a file, default file name: Running.txt
      * @details 
      * @param[in] none
      * @param[out] None
      * @return None
      * @note 
      * @attention not implement at this version
      * @code
      * @endcode
      */    
    void saveLog(void);
    /**
      * @fn saveLogToFile
      * @brief save system log to a designated file
      * @details 
      * @param[in] fileName
      * @param[out] None
      * @return None
      * @note 
      * @attention not implement at this version
      * @code
      * @endcode
      */    
    void saveLogToFile(std::string fileName) ;
};


#endif //__SYSTEMLOG_HPP__
