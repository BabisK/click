/*
 * diameterpopstorepushavp.{cc,hh} -- element that removes the last avp from the packet
 * and stores it. When the packet passes from the second port the avp is pushed on it
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
#include "diameterpopstorepushavp.hh"
#include <click/args.hh>
#include <click/confparse.hh>
#include <click/glue.hh>
#include <iostream>
CLICK_DECLS

DiameterPopStorePushAVP::DiameterPopStorePushAVP()
: data(NULL) {}

DiameterPopStorePushAVP::~DiameterPopStorePushAVP()
{
	if(data != NULL)
	{
		delete[] data;
	}
}

void DiameterPopStorePushAVP::push(int, Packet* p)
{
	if(data == NULL)
	{
		if ((p = simple_action(p)))
		{
			output(0).push(p);
		}
	}
	else
	{
		if ((p = simple_action(p)))
		{
			output(1).push(p);
		}
	}
}

Packet* DiameterPopStorePushAVP::simple_action(Packet * p_in)
{
	DiameterHeader dh;
	AvpHeader tah;
	dh.decode(p_in->data());

	const uint8_t* nextAvp = p_in->data() + 20;
	while(nextAvp < p_in->data() + dh.getLength())
	{
		tah.decode(nextAvp);
		nextAvp += tah.getLengthPadded();
	}

	WritablePacket* wp;

	if(data == NULL)
	{
		nextAvp -= tah.getLengthPadded();
		ah.decode(nextAvp);
		uint32_t headerLength = ah.getVendorSpecific() ? 12 : 8;
		data = new uint8_t[ah.getLength() - headerLength];
		memcpy(data, nextAvp + headerLength, ah.getLengthPadded() - headerLength);
		dh.setLength(dh.getLength() - ah.getLengthPadded());
		p_in->take(ah.getLengthPadded());
		wp = p_in->put(0);
		dh.encode(wp->data());
	}
	else
	{
		wp = p_in->put(ah.getLengthPadded());
		memset(wp->data() + dh.getLength(), 0, ah.getLengthPadded());
		ah.encode(wp->data() + dh.getLength());
		uint32_t headerLength = ah.getVendorSpecific() ? 12 : 8;
		memcpy(wp->data() + dh.getLength() + headerLength, data, ah.getLength() - headerLength);
		dh.setLength(dh.getLength() + ah.getLengthPadded());
		dh.encode(wp->data());
		delete[] data;
		data = NULL;
	}
	return wp;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterPopStorePushAVP)
