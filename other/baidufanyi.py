#! /usr/bin/env python
# -*- coding:utf-8 -*-

import urllib.request
import urllib.parse
import json
import sys
import execjs


class BaiduFanyi(object):
    def __init__(self):
        self.url = 'https://fanyi.baidu.com/v2transapi'
        
        self.header = {
            "accept": "*/*",
            "accept-language": "zh-CN,zh;q=0.9",
            "content-type": "application/x-www-form-urlencoded; charset=UTF-8",
            "cookie": "BAIDUID=CC1996183B06AC5DD987C80465B33C2D:FG=1; BIDUPSID=CC1996183B06AC5DD987C80465B33C2D; PSTM=1564044055; BDORZ=B490B5EBF6F3CD402E515D22BCDA1598; H_PS_PSSID=29591_1420_21095_29074_29523_29519_29099_29568_28838_29221_29071_28701; REALTIME_TRANS_SWITCH=1; FANYI_WORD_SWITCH=1; HISTORY_SWITCH=1; SOUND_SPD_SWITCH=1; SOUND_PREFER_SWITCH=1; locale=zh; Hm_lvt_64ecd82404c51e03dc91cb9e8c025574=1564654698,1564656254; to_lang_often=%5B%7B%22value%22%3A%22zh%22%2C%22text%22%3A%22%u4E2D%u6587%22%7D%2C%7B%22value%22%3A%22en%22%2C%22text%22%3A%22%u82F1%u8BED%22%7D%5D; from_lang_often=%5B%7B%22value%22%3A%22en%22%2C%22text%22%3A%22%u82F1%u8BED%22%7D%2C%7B%22value%22%3A%22zh%22%2C%22text%22%3A%22%u4E2D%u6587%22%7D%5D; delPer=0; PSINO=7; Hm_lpvt_64ecd82404c51e03dc91cb9e8c025574=1564656596; yjs_js_security_passport=a816a52f8c21bfa88eb17bf6d131c9c649fdec79_1564656596_js",
            "origin": "https://fanyi.baidu.com",
            "referer": "https://fanyi.baidu.com/",
            "user-agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.142 Safari/537.36",
            "x-requested-with": "XMLHttpRequest",
        }
        
        self.data = {
            "from": "",  #输入的语言类型待检测
            "to": "",
            "query": '',  # 要查询的数据待输入
            "transtype": "translang",
            "simple_means_flag": "3",
            "sign": "",  # 要运算的sign值
            "token": "b670bbc1562d679045dbea34270af2bc"  
        }
        
        with open('other/baidufanyi.js', 'r') as f:  # 导入javascript代码
            self.js_func = execjs.compile(f.read())
            
    
    def langdetect(self, q, lang_from, lang_to):
        if lang_from:
            self.data['from'] = lang_from
        else:
            detect_url = "https://fanyi.baidu.com/langdetect"
            formdata = urllib.parse.urlencode({"query": q}).encode('utf-8')
            req = urllib.request.Request(url=detect_url, data=formdata, headers=self.header)
            try:
                self.data['from'] = json.loads(urllib.request.urlopen(req).read().decode("utf-8"))['lan']
            except:
                self.data['from'] = 'zh'
                
        if lang_to:
            self.data['to'] = lang_to
        else:
            if self.data['from'] != 'en':
                self.data['to'] = 'en'
            else:
                self.data['to'] = 'zh'
    
    
    def query(self, q, lang_from = '', lang_to = ''):
        self.data['query'] = q
        self.data['sign'] = self.js_func.call('e', q)  # 填入sign值
        self.langdetect(q, lang_from, lang_to)  # 确定翻译的语种
        
        formdata = urllib.parse.urlencode(self.data).encode('utf-8')
        req = urllib.request.Request(url=self.url, data=formdata, headers=self.header)
        response = json.loads(urllib.request.urlopen(req).read().decode("utf-8"))
        try:
            print("{\"res\":\"" + response['trans_result']['data'][0]['dst'] + "\",\"ph_en\":\"" + response['dict_result']['simple_means']['symbols'][0]['ph_en'] + "\",\"ph_am\":\"" + response['dict_result']['simple_means']['symbols'][0]['ph_am'] + "\"}", end="")
        except:
            try:
                print(response['trans_result']['data'][0]['dst'], end="")
            except:
                print("", end="")
if __name__ == "__main__":
    b = BaiduFanyi()
    try:
        b.query(sys.argv[1])
    except:
        print("", end="")
