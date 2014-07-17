/*
 * diameterpopavp.{cc,hh} -- element that removes the last avp from the packet
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
#include "diameterpopavp.hh"
#include <click/args.hh>
#include <click/confparse.hh>
#include <click/glue.hh>
#include <iostream>
CLICK_DECLS

DiameterPopAVP::DiameterPopAVP(){}

Packet* DiameterPopAVP::simple_action(Packet * p_in)
{
	DiameterHeader dh;
	dh.decode(p_in->data());

	const uint8_t* nextAvp = p_in->data() + 20;
	AvpHeader ah;
	while(nextAvp < p_in->data() + dh.getLength())
	{
		ah.decode(nextAvp);
		nextAvp += ah.getLengthPadded();
	}
	p_in->take(ah.getLengthPadded());
	WritablePacket* wp = p_in->put(0);
	dh.setLength(dh.getLength() - ah.getLengthPadded());
	dh.encode(wp->data());

	return wp;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterPopAVP)
