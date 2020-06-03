#include <service.h>

#include <iostream>

#include <boost/algorithm/string.hpp>

#include "parseHttp.h"
#include "setRedis.h"
#include "setMysql.h"

std::string translate(std::string query){
    char resTmp[1024];
    if (query.size() == 0) return "";

    std::vector<std::string> res;
    std::string cmd = "get " + ParseHttpRequest::base64Encode(query);
    execRedisCommand(cmd, res);
    if (res.size() != 0) {
        strncpy(resTmp, (char*)(ParseHttpRequest::base64Decode(res[0])).c_str(), 256);
        return resTmp;
    }
    
    cmd = "python3 other/baidufanyi.py \"" + boost::algorithm::replace_all_copy(query, "\"", "\\\"") + "\"";
    FILE* p = popen((char*)cmd.data(),"r");
    fgets(resTmp, 512, p);
    
    pclose(p);
    cmd = "set " + ParseHttpRequest::base64Encode(query) + " " + ParseHttpRequest::base64Encode(resTmp);
    execRedisCommand(cmd, res);
    cmd = "EXPIRE " + ParseHttpRequest::base64Encode(query) + " 10800";
    execRedisCommand(cmd, res);
    return resTmp;
}

std::string getProblem(std::string id, std::string difficulty){
    if (id.size() == 0 || difficulty.size() == 0) return "";
    
    id = boost::algorithm::replace_all_copy(id, "\"", "\\\"");
    difficulty = boost::algorithm::replace_all_copy(difficulty, "\"", "\\\"");
    if (difficulty != "easy" && difficulty != "normal" && difficulty != "hard") return "difficulty error";

    std::string res;
    std::string query;
    MYSQL connect;
    MYSQL_RES* result;
    MYSQL_FIELD* fd;
    MYSQL_ROW sqlRow;
    int rowNum, colNum;

    mysql_init(&connect);
    if (mysql_real_connect(&connect, "0.0.0.0", "su", "tF#262420228", "edu_project", 0, NULL, 0) != NULL){
        mysql_query(&connect,"set names 'utf8'");
        mysql_query(&connect,"set character_set_server=utf8");
        mysql_set_character_set(&connect,"utf8");
        query = "select * from eng_word_" + difficulty + " where id = \"" + id + "\"";
    } else return "error";

    std::cout << query << std::endl;
    if (!mysql_query(&connect, (char*)query.c_str())){
        result = mysql_store_result(&connect);
        if (result){
            rowNum = mysql_num_rows(result);
            colNum = mysql_num_fields(result);
            if (rowNum == 0) {
                mysql_close(&connect);
                return "id error";
            }
            std::string fields[colNum], cols[colNum];
            for(int i = 0; fd = mysql_fetch_field(result); i++){
                fields[i] = fd->name;
            }
            while (sqlRow = mysql_fetch_row(result)){
                for(int i = 0; i < colNum; i++){
                    cols[i] = sqlRow[i];
                }
            }
            res += "{";
            for(int i = 0; i < colNum; i++){
                cols[i] = boost::algorithm::replace_all_copy(cols[i], "\"", "\\\"");
                res += "\"" + fields[i] + "\"" + ":\"" + cols[i] + "\"";
                if (i != colNum - 1) res += ",";
            }
            res += "}";
        } else {
            mysql_close(&connect);
            return "error";
        }
    } else {
        mysql_close(&connect);
        return "error";
    }
    
    return res;
}
