#ifndef HI_REDIS_H_
#define HI_REDIS_H_
#include <iostream>
#include <vector>
#include <hiredis/hiredis.h>
#endif // !HI_REDIS_H_

void execRedisCommand(std::string command, std::vector<std::string>& result);
