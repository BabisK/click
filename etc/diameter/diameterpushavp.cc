/*
 * diameterpushavp.{cc,hh} -- element that puts an AVP at the end of the packet
 * Charalampos "Babis" Kaidos
 *
 * Copyright (c) 2014 Intracom Telecom
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

#include <click/config.h>
#include "diameterpushavp.hh"
#include <click/args.hh>
#include <click/confparse.hh>
#include <click/glue.hh>
#include <iostream>
CLICK_DECLS

enum randStringType{
	AlphaNum,
	Num,
	Alpha,
	Hex
};

static String randString(randStringType type, size_t size)
{
	static const char alphaNum[] =
	        "0123456789"
	        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	        "abcdefghijklmnopqrstuvwxyz";

	static const char num[] =
	        "0123456789";

	static const char alpha[] =
	        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	        "abcdefghijklmnopqrstuvwxyz";

	static const char hex[] =
	        "0123456789"
	        "ABCDEFG"
	        "abcdefg";

	String ret;
	switch(type)
	{
	case AlphaNum:
		while(size-- > 0)
		{
			ret.append(alphaNum[rand() % (sizeof(alphaNum)-1)]);
		}
		break;
	case Num:
		while(size-- > 0)
		{
			ret.append(num[rand() % (sizeof(num)-1)]);
		}
		break;
	case Alpha:
		while(size-- > 0)
		{
			ret.append(alpha[rand() % (sizeof(alpha)-1)]);
		}
		break;
	case Hex:
		while(size-- > 0)
		{
			ret.append(hex[rand() % (sizeof(hex)-1)]);
		}
		break;
	}

	return ret;
}

DiameterPushAVP::DiameterPushAVP()
: datastring(), datauint(0), randUInt32(false), randAlphaNumString(0), randNumString(0), randAlphaString(0), randHexString(0) {}

int DiameterPushAVP::configure(Vector<String> &conf, ErrorHandler *errh)
{
	srand(time(NULL));

	uint32_t code;
	bool vendorSpec;
	uint32_t vendorId;

	if (Args(conf, this, errh)
		.read_m("CODE", code)
		.read("VENDORSPEC", vendorSpec)
		.read("VENDOR", vendorId)
		.read("OCTETSTRING", datastring)
		.read("UINT32", datauint)
		.read("RANDUINT32", randUInt32)
		.read("RANDALPHANUMSTRING", randAlphaNumString)
		.read("RANDNUMSTRING", randNumString)
		.read("RANDALPHASTRING", randAlphaString)
		.read("RANDHEXSTRING", randHexString)
		.complete() < 0)
		return -1;

	ah.setCode(code);
	ah.setMandatory(false);
	ah.setVendorSpecific(vendorSpec);
	if(vendorSpec)
	{
		ah.setVendorId(vendorId);
	}
	else
	{
		ah.setVendorId(0);
	}

	return 0;
}

Packet* DiameterPushAVP::simple_action(Packet * p_in)
{
	if(randUInt32 == true)
	{
		datauint = rand();
	}
	else if(randAlphaNumString > true)
	{
		datastring = randString(AlphaNum, randAlphaNumString);
	}
	else if(randNumString > 0)
	{
		datastring = randString(Num, randNumString);
	}
	else if(randAlphaString > 0)
	{
		datastring = randString(Alpha, randAlphaString);
	}
	else if(randHexString > 0)
	{
		datastring = randString(Hex, randHexString);
	}

	uint32_t avpHeaderLength = ah.getVendorSpecific() ? 12 : 8;
	uint32_t avpDataLength = datastring.length() > 0 ? datastring.length() : (datauint > 0 ? 4 : 0);
	uint32_t avpLength = avpHeaderLength + avpDataLength;
	uint32_t avpLengthPadded = ((avpLength + 3) / 4) * 4;
	WritablePacket *p = p_in->put(avpLengthPadded);

	DiameterHeader dh;
	dh.decode(p->data());

	memset(p->data() + dh.getLength(), 0, avpLengthPadded);

	ah.setLength(avpLength);
	ah.encode(p->data() + dh.getLength());
	if(datastring.length() > 0)
	{
		memcpy(p->data() + dh.getLength() + avpHeaderLength, datastring.data(), avpDataLength);
	}
	else
	{
		uint32_t hton = htonl(datauint);
		memcpy(p->data() + dh.getLength() + avpHeaderLength, &hton, avpDataLength);
	}

	dh.setLength(dh.getLength() + ah.getLengthPadded());
	dh.encode(p->data());

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterPushAVP)
