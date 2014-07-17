#ifndef GTP_TYPES_H
#define GTP_TYPES_H

enum error_e {
    ENC_OK                      = 1,
    ENC_NOK                     = 2,
    DEC_OK                      = 3,
    DEC_NOK                     = 4
}__attribute__((packed));

enum gtpMsgType_e {
    ECHO_REQ_MSG_TYPE           = 1,
    ECHO_RSP_MSG_TYPE           = 2,
    CREATE_SESSION_REQ_MSG_TYPE = 32,
    CREATE_SESSION_RSP_MSG_TYPE = 33
}__attribute__((packed));

typedef enum gtpMsgType_e gtpMsgType_t;


enum informationElement_e{
    IMSI_TYPE                   = 1,
    EPS_BEARER_ID_TYPE          = 73
}__attribute__((packed));

typedef enum informationElement_e informationElement_t;

#endif //GTP_TYPES
