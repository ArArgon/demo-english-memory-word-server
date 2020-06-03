#include <setRedis.h>

void execRedisCommand(std::string command, std::vector<std::string>& result){
    redisContext* c = redisConnect("127.0.0.1", 6379);
    if (c->err) {
        redisFree(c);
        std::cout << "connect to redis failed" << std::endl;
        return;
    }
    redisReply* r = (redisReply*)redisCommand(c, (char*)command.c_str());
    if (r->type == REDIS_REPLY_INTEGER) {
        int len = r->integer;
        result.push_back(std::to_string(len));
    } else if (r->type == REDIS_REPLY_STRING) {
        result.push_back(r->str);
    } else if (r->type == REDIS_REPLY_STATUS) {
        result.push_back(r->str);
    } else if (r->type == REDIS_REPLY_ARRAY) {
        for(int i = 0; i < r->elements; i++){
            result.push_back(r->element[i]->str);
        }
    } else if (r->type == REDIS_REPLY_NIL) {

    } else {
        std::cout << r->str << std::endl;
        std::cout << "exec failed." << std::endl;
    }
    freeReplyObject(r);
    redisFree(c);
    return;
}
