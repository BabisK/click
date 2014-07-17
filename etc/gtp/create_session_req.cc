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

#include "create_session_req.hh"
#include <click/error.hh>

CLICK_DECLS

#include "gtp.h"
#include <sys/types.h>
#include <string.h>
#include "gtpTypes.h"
#include "tli.h"

static int encodeCreateSessionReq(u_int8_t version, u_int8_t seq, char * imsi, u_int8_t epsBearerId, u_int8_t * index_buf);

CreateSessionRequestFromImsi::CreateSessionRequestFromImsi()
{
}
CreateSessionRequestFromImsi::~CreateSessionRequestFromImsi()
{
}

int
CreateSessionRequestFromImsi::initialize(ErrorHandler *errh)
{
    errh->message("Successfully linked with package!");
    printf("GTP_ELEMENT initialized\n");
    return 0;
}

Packet * 
CreateSessionRequestFromImsi::simple_action(Packet * p){
    printf("Inside simple_action\n");
    click_chatter("Got a packet of size");
    WritablePacket * po_output = p->put(16);
    u_int8_t buf[1000];
    memset(buf, '\0', 1000);
    //encodeCreateSessionReq(2, 1, (char *)p->data(), 5, (u_int8_t *)po_output->data());
    encodeCreateSessionReq(2, 1, (char *)p->data(), 5, buf);
    memcpy((u_int8_t *)po_output->data(), buf, 1000);
    return po_output;
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

static int encodeCreateSessionReq(u_int8_t version, u_int8_t seq, char * imsi, u_int8_t epsBearerId, u_int8_t * index_buf){
    u_int8_t * initialIndex = index_buf;
    index_buf += encodeGtpHeader(version, CREATE_SESSION_REQ_MSG_TYPE, seq, index_buf);
    index_buf += encodeImsi(imsi, index_buf);
    index_buf += encodeBearerId(epsBearerId, index_buf);
    int totalMsgBytes = (int)(index_buf - initialIndex) - sizeof(gtpMsgHeader);
    //at the end set the actual message length at fixed protocol positions 2 and 3
    u_int16_t msgLen = htons(totalMsgBytes);
    memcpy(initialIndex + 2, &msgLen, sizeof(u_int16_t));
    printf("Total bytes ... %d\n", totalMsgBytes);
    printf("Encoded Msg ...\n");
    for(int i =0; i < totalMsgBytes + sizeof(gtpMsgHeader); i++){
        printf(" %.2X", initialIndex[i]);
    }
    printf("\n");

    return ENC_OK;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(CreateSessionRequestFromImsi)
