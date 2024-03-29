#include"Logger.h"
#include<string.h>
#include<time.h>
#include<stdexcept>
#include<iostream>
#include<stdarg.h>

const char * Logger::s_level[LEVEL_COUNT]={
    "DEBUG",
    "INFO",
    "MARN",
    "ERROR",
    "FATAL"
};

Logger * Logger::m_instance = NULL;

Logger::Logger():m_level(DEBUG), m_len(0), m_max(0){}

Logger::~Logger(){
    close();
}

Logger * Logger::instance(){
    if(m_instance == NULL){
        m_instance = new Logger();
    }
    return m_instance;
}

void Logger::open(const string & filename){
    m_filename = filename;
    m_fout.open(filename, ios::app);
    if(m_fout.fail()){
        throw std::logic_error("open file failed" + filename);
    }
    m_fout.seekp(0,ios::end);
    m_len=m_fout.tellp();
}

void Logger::close(){
    m_fout.close();
}

void Logger::log(Level level, const char * file, int line, const char * format, ...){

    if(m_level>level){
        return;
    }

    if(m_fout.fail()){
        throw std::logic_error("open file failed" + m_filename);
    }

    time_t ticks = time(NULL);
    struct tm * ptm = localtime(&ticks);
    char timestamp[32];
    memset(timestamp,0,sizeof(timestamp));
    strftime(timestamp,sizeof(timestamp),"%Y-%m-%d %H:%M:%S",ptm);

    const char * fmt = "%s %s %s:%d ";
    int size = snprintf(NULL, 0, fmt, timestamp, s_level[level], file, line);
    if (size>0)
    {
        char * buffer = new char[size + 1];
        snprintf(buffer, size + 1, fmt, timestamp, s_level[level], file, line);
        buffer[size]='\0';
        std::cout<<buffer<<'\n';
        m_fout<<buffer;
        m_len += size;
        delete[] buffer;
    }
    va_list arg_ptr;
    va_start(arg_ptr, format);
    size = vsnprintf(NULL, 0, format, arg_ptr);
    va_end(arg_ptr);
    if(size>0){
        char * content = new char[size + 1];
        va_start(arg_ptr, format);
        vsnprintf(content, size + 1, format, arg_ptr);
        va_end(arg_ptr);
        std::cout<<content<<'\n';
        m_fout<< content;
        m_len += size;
        delete[] content;
    }
    m_fout<<"\n";
    m_fout.flush();

    if(m_len >= m_max && m_max > 0){
        rotate();
    }
}

void Logger::rotate(){
    close();
    time_t ticks = time(NULL);
    struct  tm* ptm = localtime(&ticks);
    char timestamp[32];
    memset(timestamp , 0 , sizeof(timestamp));
    strftime(timestamp,sizeof(timestamp),"%Y-%m-%d %H-%M-%S",ptm);
    string filename = m_filename + timestamp;
    if(rename(m_filename.c_str(),filename.c_str()) != 0){
        throw std::logic_error("rename log file failed: " + string(strerror(errno)));
    }
    open(m_filename);
}