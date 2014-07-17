/*
 * diameterfliprequest.{cc,hh} -- element that changes the request flag of the diameter message
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
#include "diameterfliprequest.hh"
#include <click/args.hh>
CLICK_DECLS

DiameterFlipRequest::DiameterFlipRequest(){}

Packet* DiameterFlipRequest::simple_action(Packet * p_in)
{
	WritablePacket * wp = p_in->put(0);
	DiameterHeader dh;
	dh.decode(wp->data());
	dh.isRequest() ? dh.setRequest(false) : dh.setRequest(true);
	dh.encode(wp->data());
	return wp;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterFlipRequest)
