//
// Created by gzbd on 2016/12/7.
//

#ifndef RSA_OPENSSL_H
#define RSA_OPENSSL_H

#include <string>
#include <stdio.h>
#include <stdlib.h>


class RSA_OPENSSL {

public:
    /*
     * 有自定义密码的签名
    */
    static const char* rsaSign(std::string test);
};


#endif //RSA_OPENSSL_H
