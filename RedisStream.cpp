#include "RedisStream.h"

namespace Redis {
RedisStream::RedisStream(QTcpSocket &redis):
    BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::in | std::ios::out),
    _redis(redis)
{
}

RedisStream::~RedisStream()
{

}
int RedisStream::readFromDevice(char* buffer, std::streamsize len)
{
    _redis.waitForReadyRead(1);
    return _redis.read(buffer, static_cast<int>(len));
}


int RedisStream::writeToDevice(const char* buffer, std::streamsize length)
{
    return _redis.write(buffer, static_cast<int>(length));
}

RedisIOS::RedisIOS(QTcpSocket &redis):
    _buf(redis)
{
    ph_ios_init(&_buf);
}

RedisIOS::~RedisIOS()
{
    try
    {
        _buf.sync();
    }
    catch (...)
    {
    }
}
RedisStream* RedisIOS::rdbuf()
{
    return &_buf;
}


void RedisIOS::close()
{
    _buf.sync();
}

RedisOutputStream::RedisOutputStream(QTcpSocket &redis):
    RedisIOS(redis),
    std::ostream(&_buf)
{
}
RedisOutputStream::~RedisOutputStream()
{
}


RedisInputStream::RedisInputStream(QTcpSocket &redis):
    RedisIOS(redis),
    std::istream(&_buf)
{
}
RedisInputStream::~RedisInputStream()
{
}


std::string RedisInputStream::getline()
{
    std::string line;
    std::getline(*this, line);
    if ( line.size() > 0 ) line.erase(line.end() - 1);
    return line;
}

}
