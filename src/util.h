// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

/**
 * Server/client environment: argument handling, config file parsing,
 * logging, thread wrappers
 */
#ifndef SEQUENCE_UTIL_H
#define SEQUENCE_UTIL_H

#if defined(HAVE_CONFIG_H)
#include "config/sequence-config.h"
#endif

#include "amount.h"
#include "compat.h"
#include "tinyformat.h"
#include "utiltime.h"

#include <exception>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread/exceptions.hpp>


// Debugging macros

// Uncomment the following line to enable debugging messages
// or enable on a per file basis prior to inclusion of util.h
//#define ENABLE_SEQUENCE_DEBUG
#ifdef ENABLE_SEQUENCE_DEBUG
#define DBG( x ) x
#else
#define DBG( x ) 
#endif

extern int nWalletBackups;

/** Signals for translation. */
class CTranslationInterface
{
public:
    /** Translate a message to the native language of the user. */
    boost::signals2::signal<std::string (const char* psz)> Translate;
};

extern std::map<std::string, std::string> mapArgs;
extern std::map<std::string, std::vector<std::string> > mapMultiArgs;
extern bool fDebug;
extern bool fPrintToConsole;
extern bool fPrintToDebugLog;
extern bool fServer;
extern std::string strMiscWarning;
extern bool fLogTimestamps;
extern bool fLogIPs;
extern volatile bool fReopenDebugLog;
extern CTranslationInterface translationInterface;

/**
 * Translation function: Call Translate signal on UI interface, which returns a boost::optional result.
 * If no translation slot is registered, nothing is returned, and simply return the input.
 */
inline std::string _(const char* psz)
{
    boost::optional<std::string> rv = translationInterface.Translate(psz);
    return rv ? (*rv) : psz;
}

void SetupEnvironment();

/** Return true if log accepts specified category */
bool LogAcceptCategory(const char* category);
/** Send a string to the log output */
int LogPrintStr(const std::string &str);

#define LogPrintf(...) LogPrint(NULL, __VA_ARGS__)

/**
 * When we switch to C++11, this can be switched to variadic templates instead
 * of this macro-based construction (see tinyformat.h).
 */
#define MAKE_ERROR_AND_LOG_FUNC(n)                                        \
    /**   Print to debug.log if -debug=category switch is given OR category is NULL. */ \
    template<TINYFORMAT_ARGTYPES(n)>                                          \
    static inline int LogPrint(const char* category, const char* format, TINYFORMAT_VARARGS(n))  \
    {                                                                         \
        if(!LogAcceptCategory(category)) return 0;                            \
        return LogPrintStr(tfm::format(format, TINYFORMAT_PASSARGS(n))); \
    }                                                                         \
    /**   Log error and return false */                                        \
    template<TINYFORMAT_ARGTYPES(n)>                                          \
    static inline bool error(const char* format, TINYFORMAT_VARARGS(n))                     \
    {                                                                         \
        LogPrintStr("ERROR: " + tfm::format(format, TINYFORMAT_PASSARGS(n)) + "\n"); \
        return false;                                                         \
    }

TINYFORMAT_FOREACH_ARGNUM(MAKE_ERROR_AND_LOG_FUNC)

/**
 * Zero-arg versions of logging and error, these are not covered by
 * TINYFORMAT_FOREACH_ARGNUM
 */
static inline int LogPrint(const char* category, const char* format)
{
    if(!LogAcceptCategory(category)) return 0;
    return LogPrintStr(format);
}
static inline bool error(const char* format)
{
    LogPrintStr(std::string("ERROR: ") + format + "\n");
    return false;
}

void PrintExceptionContinue(const std::exception* pex, const char* pszThread);
void ParseParameters(int argc, const char*const argv[]);
void FileCommit(FILE *file);
bool TruncateFile(FILE *file, unsigned int length);
int RaiseFileDescriptorLimit(int nMinFD);
void AllocateFileRange(FILE *file, unsigned int offset, unsigned int length);
bool RenameOver(boost::filesystem::path src, boost::filesystem::path dest);
bool TryCreateDirectory(const boost::filesystem::path& p);
boost::filesystem::path GetDefaultDataDir();
const boost::filesystem::path &GetDataDir(bool fNetSpecific = true);
const boost::filesystem::path &GetBackupsDir();
boost::filesystem::path GetConfigFile();
#ifndef WIN32
boost::filesystem::path GetPidFile();
void CreatePidFile(const boost::filesystem::path &path, pid_t pid);
#endif
void ReadConfigFile(std::map<std::string, std::string>& mapSettingsRet, std::map<std::string, std::vector<std::string> >& mapMultiSettingsRet);
#ifdef WIN32
boost::filesystem::path GetSpecialFolderPath(int nFolder, bool fCreate = true);
#endif
boost::filesystem::path GetTempPath();
void ShrinkDebugFile();
void runCommand(std::string strCommand);

inline bool IsSwitchChar(char c)
{
#ifdef WIN32
    return c == '-' || c == '/';
#else
    return c == '-';
#endif
}

/**
 * Return string argument or default value
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @param default (e.g. "1")
 * @return command-line argument or default value
 */
std::string GetArg(const std::string& strArg, const std::string& strDefault);

/**
 * Return integer argument or default value
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @param default (e.g. 1)
 * @return command-line argument (0 if invalid number) or default value
 */
int64_t GetArg(const std::string& strArg, int64_t nDefault);

/**
 * Return boolean argument or default value
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @param default (true or false)
 * @return command-line argument or default value
 */
bool GetBoolArg(const std::string& strArg, bool fDefault);

/**
 * Set an argument if it doesn't already have a value
 *
 * @param strArg Argument to set (e.g. "-foo")
 * @param strValue Value (e.g. "1")
 * @return true if argument gets set, false if it already had a value
 */
bool SoftSetArg(const std::string& strArg, const std::string& strValue);

/**
 * Set a boolean argument if it doesn't already have a value
 *
 * @param strArg Argument to set (e.g. "-foo")
 * @param fValue Value (e.g. false)
 * @return true if argument gets set, false if it already had a value
 */
bool SoftSetBoolArg(const std::string& strArg, bool fValue);

/**
 * Return the number of physical cores available on the current system.
 * @note This does not count virtual cores, such as those provided by HyperThreading
 * when boost is newer than 1.56.
 */
int GetNumCores();

void SetThreadPriority(int nPriority);
void RenameThread(const char* name);
std::string GetThreadName();

/**
 * Standard wrapper for do-something-forever thread functions.
 * "Forever" really means until the thread is interrupted.
 * Use it like:
 *   new boost::thread(boost::bind(&LoopForever<void (*)()>, "dumpaddr", &DumpAddresses, 900000));
 * or maybe:
 *    boost::function<void()> f = boost::bind(&FunctionWithArg, argument);
 *    threadGroup.create_thread(boost::bind(&LoopForever<boost::function<void()> >, "nothing", f, milliseconds));
 */
template <typename Callable> void LoopForever(const char* name,  Callable func, int64_t msecs)
{
    std::string s = strprintf("sequence-%s", name);
    RenameThread(s.c_str());
    LogPrintf("%s thread start\n", name);
    try
    {
        while (1)
        {
            MilliSleep(msecs);
            func();
        }
    }
    catch (boost::thread_interrupted)
    {
        LogPrintf("%s thread stop\n", name);
        throw;
    }
    catch (const std::exception& e) {
        PrintExceptionContinue(&e, name);
        throw;
    }
    catch (...) {
        PrintExceptionContinue(NULL, name);
        throw;
    }
}

/**
 * .. and a wrapper that just calls func once
 */
template <typename Callable> void TraceThread(const char* name,  Callable func)
{
    std::string s = strprintf("sequence-%s", name);
    RenameThread(s.c_str());
    try
    {
        LogPrintf("%s thread start\n", name);
        func();
        LogPrintf("%s thread exit\n", name);
    }
    catch (boost::thread_interrupted)
    {
        LogPrintf("%s thread interrupt\n", name);
        throw;
    }
    catch (const std::exception& e) {
        PrintExceptionContinue(&e, name);
        throw;
    }
    catch (...) {
        PrintExceptionContinue(NULL, name);
        throw;
    }
}

inline uint32_t ByteReverse(uint32_t value)
{
    value = ((value & 0xFF00FF00) >> 8) | ((value & 0x00FF00FF) << 8);
    return (value<<16) | (value>>16);
}

#endif // SEQUENCE_UTIL_H
