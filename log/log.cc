/**
 * @file log.cc
 * @author breezeTuT (y_q_email@163.com)
 * @brief
 * @date 2023-03-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "log.h"
#include "iostream"

using namespace std;

Log::Log()
{
    lineCount_ = 0;
    isAsync_ = false;
    writeThread_ = nullptr;
    deque_ = nullptr;
    toDay_ = 0;
    fp_ = nullptr;
}

Log::~Log()
{
    if (writeThread_ && writeThread_->joinable())
    {
        while (!deque_->empty())
        {
            deque_->flush();
        };
        deque_->Close();
        writeThread_->join();
    }
    if (fp_)
    {
        lock_guard<mutex> locker(mtx_);
        flush();
        fclose(fp_);
    }
}

int Log::GetLevel()
{
    lock_guard<mutex> locker(mtx_);
    return level_;
}

void Log::SetLevel(int level)
{
    lock_guard<mutex> locker(mtx_);
    level_ = level;
}

void Log::init(int level = 1, const char *path, const char *suffix,
               int maxQueueSize)
{
    isOpen_ = true;
    level_ = level;
    if (maxQueueSize > 0)
    {
        isAsync_ = true;
        if (!deque_)
        {
            //c++ 11,in c++17 can use std::make_unique
            unique_ptr<BlockDeque<std::string>> newDeque(new BlockDeque<std::string>);
            deque_ = move(newDeque);

            //c++ 11,in c++17 can use std::make_unique
            std::unique_ptr<std::thread> NewThread(new thread(FlushLogThread));
            writeThread_ = move(NewThread);
        }
    }
    else
    {
        isAsync_ = false;
    }

    lineCount_ = 0;

    time_t timer = time(nullptr);
    struct tm *sysTime = localtime(&timer);
    struct tm t = *sysTime;
    path_ = path;
    suffix_ = suffix;
    char fileName[LOG_NAME_LEN] = {0};
    snprintf(fileName, LOG_NAME_LEN - 1, "%s/%04d_%02d_%02d%s",
             path_, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, suffix_);
    toDay_ = t.tm_mday;

    {
        lock_guard<mutex> locker(mtx_);

        // init buffer
        buff_.resize(BUFFER_SIZE);
        buff_.assign(buff_.size(), 0); // set zero
        buffToWrite_ = 0;
        buffToRead_ = 0;

        if (fp_)
        {
            flush();
            fclose(fp_);
        }

        fp_ = fopen(fileName, "a");
        if (fp_ == nullptr)
        {
            mkdir(path_, 0777);
            fp_ = fopen(fileName, "a");
        }
        assert(fp_ != nullptr);
    }
}

void Log::write(int level, const char *format, ...)
{
    struct timeval now = {0, 0};
    gettimeofday(&now, nullptr);
    time_t tSec = now.tv_sec;
    struct tm *sysTime = localtime(&tSec);
    struct tm t = *sysTime;
    va_list vaList;

    /* time and lines */
    if (toDay_ != t.tm_mday || (lineCount_ && (lineCount_ % MAX_LINES == 0)))
    {
        unique_lock<mutex> locker(mtx_);
        locker.unlock();

        char newFile[LOG_NAME_LEN];
        char tail[36] = {0};
        snprintf(tail, 36, "%04d_%02d_%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);

        if (toDay_ != t.tm_mday)
        {
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s%s", path_, tail, suffix_);
            toDay_ = t.tm_mday;
            lineCount_ = 0;
        }
        else
        {
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s-%d%s", path_, tail, (lineCount_ / MAX_LINES), suffix_);
        }
        locker.lock();
        flush();
        fclose(fp_);
        fp_ = fopen(newFile, "a");
        assert(fp_ != nullptr);
    }

    {
        unique_lock<mutex> locker(mtx_);
        lineCount_++;

        //write time to buffer
        int n = snprintf(&(*buff_.begin()) + buffToWrite_, 128, "%d-%02d-%02d %02d:%02d:%02d.%06ld ",
                         t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                         t.tm_hour, t.tm_min, t.tm_sec, now.tv_usec);
        buffToWrite_ += n;

        //write log level to buffer
        AppendLogLevelTitle_(level);

        //write log content to buffer
        va_start(vaList, format);
        int m = vsnprintf(buff_.begin().base() + buffToWrite_, buff_.size() - buffToWrite_, format, vaList);
        va_end(vaList);

        buffToWrite_ += m;


        //add Wrap delimiter
        std::copy("\n\0", "\n\0" + 2, buff_.begin() + buffToWrite_);
        buffToWrite_ += 2;


        /*-------------write buffer to file-----------------*/

        //if async, push buffer to deque
        if (isAsync_ && deque_ && !deque_->full())
        {
            // deque_->push_back(buff_.RetrieveAllToStr());
            std::string str(buff_.begin().base() + buffToRead_, buffToWrite_ - buffToRead_);
            deque_->push_back(str);

            buff_.assign(0, buff_.size());
            buffToWrite_ = 0;
            buffToRead_ = 0;
        }

        //if not async, write buffer to file
        else
        {
            // write buffer into fp_,from readpos to end
            fputs(buff_.begin().base() + buffToRead_, fp_);
        }

        // buff_.RetrieveAll();
        buff_.assign(0, buff_.size());
        buffToWrite_ = 0;
        buffToRead_ = 0;
    }
}

void Log::AppendLogLevelTitle_(int level)
{
    switch (level)
    {
    case 0:
        std::copy("[debug]: ", "[debug]: " + 9, buff_.begin() + buffToWrite_);//add level
        buffToWrite_ += 8;//move buffToWrite_
        break;
    case 1:
        std::copy("[info]: ", "[info]: " + 8, buff_.begin() + buffToWrite_);
        buffToWrite_ += 8;
        break;
    case 2:
        std::copy("[warn]: ", "[warn]: " + 8, buff_.begin() + buffToWrite_);
        buffToWrite_ += 8;
        break;
    case 3:
        std::copy("[error]: ", "[error]: " + 9, buff_.begin() + buffToWrite_);
        buffToWrite_ += 8;
        break;

    default:
        std::copy("[info]: ", "[info]: " + 8, buff_.begin() + buffToWrite_);
        buffToWrite_ += 8;
        break;
    }
}

void Log::flush()
{
    if (isAsync_)
    {
        deque_->flush();
    }
    fflush(fp_);
}

void Log::AsyncWrite_()
{
    string str = "";
    while (deque_->pop(str))
    {        
        lock_guard<mutex> locker(mtx_);
        fputs(str.c_str(), fp_);
        //TODO:if not flush, the log will not be written into file when async write,why?
        //and LOG_BASE also called log->flush(),but not work,need settle.
        flush();
    }
}

Log *Log::Instance()
{
    static Log inst;
    return &inst;
}

void Log::FlushLogThread()
{
    Log::Instance()->AsyncWrite_();
}