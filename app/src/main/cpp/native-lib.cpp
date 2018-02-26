#include <jni.h>
#include <string>
#include "Log.h"
#include <hmac.h>
#include<rsa.h>
#include<pem.h>
#include <md5.h>
#include <string.h>

#include <time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "rsa_openssl.h"

#define debug 1
#define TIMEOUT 3
#define JAN_1970 0x83AA7E80
#define NTP_SERVER_1 "202.120.2.101"    //cn.pool.ntp.org
#define NTP_SERVER_2 "223.113.97.99"  //cn.pool.ntp.org
#define NTP_SERVER_3 "202.112.31.197"   //cn.pool.ntp.org
extern "C" {


void construct_ntp_packet(char content[])
{
    long           timer;

    memset(content, 0, 48);
    content[0] = 0x1b; 			// LI = 0 ; VN = 3 ; Mode = 3 (client);

    time((time_t *)&timer);
    timer = htonl(timer + JAN_1970 );

    memcpy(content + 40, &timer, sizeof(timer));  //trans_timastamp
}

int get_ntp_time(int sockfd, struct sockaddr_in *server_addr, struct tm *net_tm)
{
    char           content[256];
    time_t         timet;
    long           temp;
    int            addr_len = 16;
    struct timeval block_time;
    fd_set         sockfd_set;

    FD_ZERO(&sockfd_set);
    FD_SET(sockfd, &sockfd_set);
    block_time.tv_sec = TIMEOUT;      //time out
    block_time.tv_usec = 0;

    construct_ntp_packet(content);
    if (sendto(sockfd, content, 48, 0, (struct sockaddr *)server_addr, addr_len) < 0) {
#if debug
        perror("sendto error");
#endif
        return (-1);
    }

    if(select(sockfd + 1, &sockfd_set, NULL, NULL, &block_time ) > 0) {
        if (recvfrom(sockfd, content, 256, 0, (struct sockaddr *)server_addr, (socklen_t *)&addr_len) < 0) {
#if debug
            LOGD("recvfrom error");
#endif
            return (-1);
        }
        else {
            memcpy(&temp, content + 40, 4);
            temp = (time_t)(ntohl(temp) - JAN_1970 );
            timet = (time_t)temp;
            memcpy(net_tm, gmtime(&timet), sizeof(struct tm));

            net_tm->tm_hour = net_tm->tm_hour + 8;  //beijing time zone
        }
    }
    else {
        return(-1);
    }
    return(0);
}
int get_time()
{
    int                  sockfd, i;
    struct tm            *net_tm;
    struct sockaddr_in   addr;
    char                 ip[4][16]= { { NTP_SERVER_1 } , { NTP_SERVER_2 }, { NTP_SERVER_3 } };
    char                 date_buf[50];

    net_tm = (struct tm *)malloc(sizeof(struct tm));

    for (i = 0 ; i < 3 ; i++ ) {
        memset(&addr, 0, sizeof(addr));
        addr.sin_addr.s_addr = inet_addr( ip[i] );
        addr.sin_port = htons(123);

        if((sockfd = socket(PF_INET, SOCK_DGRAM, 0/*IPPROTO_UDP*/)) == -1) {
#if debug
            LOGD ("socket error");
#endif
            return (-1);
        }

        if (get_ntp_time(sockfd, &addr, net_tm) == 0) {
            break;
        }

        //close(sockfd);
    }

    //strftime(date_buf, sizeof(date_buf), "date -s \"%F %T\"", net_tm);
    //system(date_buf);
    LOGI("%d/%d/%d %d:%d", net_tm->tm_year+1900, net_tm->tm_mon+1, net_tm->tm_mday, net_tm->tm_hour, net_tm->tm_min);
    return (int)mktime(net_tm);
//    return net_tm->tm_year+1900;
}
int standard_to_stamp(char *str_time)
{
    struct tm stm;
    int iY, iM, iD, iH, iMin, iS;

    memset(&stm,0,sizeof(stm));
    iY = atoi(str_time);
    iM = atoi(str_time+5);
    iD = atoi(str_time+8);
    iH = atoi(str_time+11);
    iMin = atoi(str_time+14);
    iS = atoi(str_time+17);

    stm.tm_year=iY-1900;
    stm.tm_mon=iM-1;
    stm.tm_mday=iD;
    stm.tm_hour=iH;
    stm.tm_min=iMin;
    stm.tm_sec=iS;

    printf("%d-%0d-%0d %0d:%0d:%0d\n", iY, iM, iD, iH, iMin, iS);   //标准时间格式例如：2016:08:02 12:12:30
    return (int)mktime(&stm);
}

JNIEXPORT jstring JNICALL
Java_com_example_liaoxiaomin_mycmakejnitest_JniUtil_SignByRSA(JNIEnv *env, jobject instance ,jstring msg_) {
    const char *msg = env->GetStringUTFChars(msg_, 0);
    std::string msgC;
    msgC.assign(msg);
    const char* base64=NULL;
    int SETtime=standard_to_stamp("2018:02:15 12:12:30");
//    LOGI("SETtime: %ld\n",SETtime);
    int nowtime = get_time();
//    LOGI("nowtime: %ld\n",nowtime);
    if(SETtime>nowtime){

        base64=RSA_OPENSSL::rsaSign(msgC);
//        LOGI("SignByRSA base64=%s",base64);
        env->ReleaseStringUTFChars(msg_, msg);
        return env->NewStringUTF(base64);
    }else{
        return env->NewStringUTF("timeOFsetting < nowtime");
    }
}

}