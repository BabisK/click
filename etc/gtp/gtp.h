#ifndef GTP_MSG_H
#define GTP_MSG_H
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include<arpa/inet.h>
#include "tli.h"
#include "gtpTypes.h"

//extern int encodeCreateSessionReq(u_int8_t version, gtpMsgType_t msgtype, u_int8_t seq, char * imsi, u_int8_t epsBearerId, u_int8_t * outmsg_buf);
//extern int decodeIncomingMessage(u_int8_t * index_buf);
//dummy response
//extern int encodeCreateSessionResp(char * cause);

class gtpMsgHeader{
public:
    gtpMsgHeader() : 
    m_version(0), 
    m_msgType((gtpMsgType_t)0),
    m_len(0)
    {
        printf("--Constructing gtpMsgHeader\n");
        memset(m_seq, '\0', 3);
    }

    //inline functions
    u_int8_t version()const;
    void version(u_int8_t version);
    gtpMsgType_t msgType()const;
    void msgType(gtpMsgType_t msgType);
    u_int16_t len()const;
    void len(u_int16_t len);
    u_int8_t seq()const;
    void seq(u_int8_t seq);

    //int encCreateSessionReq(u_int8_t version, gtpMsgType_t msgtype, u_int8_t seq, char * imsi, u_int8_t * outmsg_buf);

private:
    u_int8_t           m_version;
    gtpMsgType_t       m_msgType;
    /*teid not supported T should be 0 in first octed*/
    u_int16_t          m_len;
    //only the LSB is supposed to be used for our needs
    u_int8_t           m_seq[3];

}__attribute__((packed));



#include "gtp.inl"
#endif /*GTP_MSG_H*/
