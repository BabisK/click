/*
 * sampleelt.{cc,hh} -- sample package element
 * Eddie Kohler
 *
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Mazu Networks, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

// ALWAYS INCLUDE <click/config.h> FIRST
#include <click/config.h>

#include "gtp.hh"
#include <click/error.hh>

CLICK_DECLS

#include "gtp.h"
#include <sys/types.h>
#include <string.h>
#include "gtpTypes.h"
#include "tli.h"


static int decodeIncomingMessage(u_int8_t * index_buf);
static int encodeCreateSessionResp(u_int8_t * index_buf);
static u_int8_t bearerid = 0;
static char imsi[16];
static int create_session_resp_len = 0;

GTP::GTP()
{
}
GTP::~GTP()
{
}

int
GTP::initialize(ErrorHandler *errh)
{
    errh->message("Successfully linked with package!");
    printf("GTP_ELEMENT initialized\n");
    return 0;
}
//void SamplePackageElement::push(int, Packet *p){
  //  printf("Inside push\nPushing to out(0)\n");
  //  output(0).push(p);
//}

Packet * 
GTP::simple_action(Packet * p){
    printf("Inside simple_action\n");
    click_chatter("Got a packet of size");
    decodeIncomingMessage((u_int8_t *)p->data());
//    WritablePacket * po_output = p->put(0);
    u_int8_t buf[1000];
    memset(buf, '\0', 1000);
    encodeCreateSessionResp(buf);
    printf("create session response...\n");
    for(int i =0; i <100; i++){
        printf(" %.2X ", buf[i]);
    }
    printf("\n");
  //  memcpy((u_int8_t *)po_output->data(), buf, create_session_resp_len + sizeof(msgGtpMsgHeader));
    //            return po_output;
    //
    WritablePacket *p_out = p->make(buf, create_session_resp_len + sizeof(gtpMsgHeader));
    return p_out;
}


static int encodeGtpHeader(u_int8_t version, gtpMsgType_t msgtype, u_int8_t seq, u_int8_t * index_buf){
    gtpMsgHeader *po_gtpMsgHeader = (gtpMsgHeader *)index_buf;
    po_gtpMsgHeader->version(version);
    po_gtpMsgHeader->msgType(msgtype);
    po_gtpMsgHeader->len(0);//for now, this will be written after gtp body has been encoded
    po_gtpMsgHeader->seq(seq);
    
    return sizeof(gtpMsgHeader);
} 

static int encodeTLI(u_int8_t type, u_int16_t len, u_int8_t inst, u_int8_t * index_buf){
    class TLI * po_TLI = (TLI *)index_buf;
    po_TLI->type(type);
    po_TLI->len(len);
    po_TLI->inst(inst);
    return sizeof(type) + sizeof(len) + sizeof(inst);
}

static int encodeImsi(char * imsi, u_int8_t * index_buf){
    encodeTLI(IMSI_TYPE, strlen(imsi), 0, index_buf);
    memcpy(index_buf + sizeof(TLI), imsi, strlen(imsi));
    return strlen(imsi) + sizeof(TLI);    
}

static int encodeBearerId(u_int8_t bearerid, u_int8_t * index_buf){
    encodeTLI(EPS_BEARER_ID_TYPE, sizeof(u_int8_t), 0, index_buf);
    memcpy(index_buf + sizeof(TLI), &bearerid, sizeof(u_int8_t));;
    return sizeof(bearerid) + sizeof(TLI);
}

static int encodeCreateSessionReq(u_int8_t version, gtpMsgType_t msgtype, u_int8_t seq, char * imsi, u_int8_t epsBearerId, u_int8_t * index_buf){
    u_int8_t * initialIndex = index_buf;
    index_buf += encodeGtpHeader(version, msgtype, seq, index_buf);
    index_buf += encodeImsi(imsi, index_buf);
    index_buf += encodeBearerId(epsBearerId, index_buf);
    int totalMsgBytes = (int)(index_buf - initialIndex) - sizeof(gtpMsgHeader);
    //at the end set the actual message length at fixed protocol positions 2 and 3
    u_int16_t msgLen = htons(totalMsgBytes);
    memcpy(initialIndex + 2, &msgLen, sizeof(u_int16_t));
    printf("Total bytes ... %d\n", totalMsgBytes);

    return ENC_OK;
}


static int decodeCreateSessionReq(u_int8_t * index_buf){
    int ret = 0;
    printf("Decoding CreateSessionRequest...\n");
    gtpMsgHeader * po_header = (gtpMsgHeader *)index_buf;
    printf("GTP version : %d\nMessage length : %d\nSequence number : %d\n",
        po_header->version(), po_header->len(), po_header->seq());
    index_buf += sizeof(gtpMsgHeader); 
    //!entering loop to :decode tli-value information elements 
    int messageLength = po_header->len();
    u_int8_t * po_tli = index_buf;
    while(0 < messageLength){
        switch(((TLI *)(po_tli))->type()){
        case IMSI_TYPE:
            printf("Imsi found in message...\nimsi instance : %d\n",
                ((TLI *)po_tli)->inst());
            memcpy(imsi, (u_int8_t *)po_tli + sizeof(TLI), ((TLI *)po_tli)->len());
            imsi[15] = '\0';
            printf("Imsi : %s\n", imsi);
            messageLength -= (sizeof(TLI) + ((TLI *)po_tli)->len());
            po_tli = po_tli + (sizeof(TLI) + ((TLI *)po_tli)->len());
            continue;
        case EPS_BEARER_ID_TYPE:
            printf("bearer id found in message...\nbearer id instance : %d\n",
                ((TLI *)po_tli)->inst());
            bearerid = *((u_int8_t *)po_tli + sizeof(TLI));
            printf("Bearer id : %d\n", bearerid);
            messageLength -= (sizeof(TLI) + ((TLI *)po_tli)->len());
            po_tli += (sizeof(TLI) + ((TLI *)po_tli)->len());
            continue;
        default:
            printf("Not supported information element type in message...\n");
            ret = DEC_NOK;
            exit(1);
            break;
        }
    }
    ret = DEC_OK;

    return ret;
}    

static int decodeIncomingMessage(u_int8_t * index_buf){
    int ret = 0;
    printf("Decoding incoming message\n");
    //!protocol message type at protocol position 1
    switch(index_buf[1]){
    
    case CREATE_SESSION_REQ_MSG_TYPE:
        ret = decodeCreateSessionReq(index_buf);
        break;
    default:
        printf("Unsupported message type\n");
        ret = DEC_NOK;    

    }
    return ret;
}


static int encodeCreateSessionResp(u_int8_t * index_buf){
    u_int8_t * initialIndex = index_buf;
    index_buf += encodeGtpHeader(2, CREATE_SESSION_RSP_MSG_TYPE, 3, index_buf);
    printf("create session response...header\n");
    char response[100];
    create_session_resp_len = sprintf((char *)response, "SESSION CREATED SUCCESFULLY\nImsi : %s\nBearer Identifier : %d\n", imsi, bearerid);
 
    strcat((char *)initialIndex, response);
    //int totalMsgBytes = (int)(index_buf - initialIndex) - sizeof(gtpMsgHeader);
    //at the end set the actual message length at fixed protocol positions 2 and 3
    //u_int16_t msgLen = htons(totalMsgBytes);
    //memcpy(initialIndex + 2, &msgLen, sizeof(u_int16_t));
    //printf("Total bytes ... %d\n", totalMsgBytes);
    return ENC_OK;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(GTP)
