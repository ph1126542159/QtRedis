#include <QCoreApplication>
#include "RedisClient.h"
#include "RedisCommand.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Redis::RedisClient redis;
    redis.connect("127.0.0.1",6379);
    if(redis.isConnected()){
        Redis::RedisCommand setCommand = Redis::RedisCommand::set("mykey", "Hello");
        try{
            std::string result = redis.execute<std::string>(setCommand);
            qDebug()<<"succed:"<<result.c_str();
        }catch(Redis::Exception& exc){
            qDebug()<<"failed."<<exc.displayText().c_str();
        }
        catch(std::exception& exc){
            qDebug()<<"failed."<<exc.what();
        }

    }

    return a.exec();
}
