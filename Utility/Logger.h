#pragma once
#include<string>
#include<fstream>
using namespace std;

#define debug(format, ...)   \
    Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define info(format, ...)   \
    Logger::instance()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define warn(format, ...)   \
    Logger::instance()->log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define error(format, ...)   \
    Logger::instance()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define fatal(format, ...)   \
    Logger::instance()->log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__);



class Logger{

public:
    enum Level {
        DEBUG=0,
        INFO,
        WARN,
        ERROR,
        FATAL,
        LEVEL_COUNT
    };
    static Logger * instance();
    void open(const string & filename);
    void close();
    void log(Level level, const char * file, int line, const char * format, ...);
    inline void level(Level level){
        m_level = level;
    }
    inline void max(int bytes){
        m_max = bytes;
    }


private:
    Logger(); 
    ~Logger();

private:
    string m_filename;
    ofstream m_fout;
    Level m_level;
    int m_max;
    int m_len;
    static const char * s_level[LEVEL_COUNT];
    static Logger * m_instance;
    void rotate();
};
