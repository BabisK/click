/*
 * diameteravpencap.{cc,hh} -- element that puts a Diameter AVP header on the packet
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
#include "diameteravpencap.hh"
#include <click/args.hh>
CLICK_DECLS

DiameterAvpEncap::DiameterAvpEncap(){}

int DiameterAvpEncap::configure(Vector<String> &conf, ErrorHandler *errh)
{
	uint32_t code;
	uint32_t vendorId = 0;
	bool mandatory = false;

	if (Args(conf, this, errh)
	.read_m("CODE", code)
	.read("VENDORID", vendorId)
	.read("MANDATORY", mandatory)
	.complete() < 0)
	return -1;

	ah.setCode(code);
	ah.setMandatory(mandatory);
	if(vendorId != 0)
	{
		ah.setVendorSpecific(true);
		ah.setVendorId(vendorId);
	}
	else
	{
		ah.setVendorSpecific(false);
		ah.setVendorId(0);
	}

	return 0;
}

Packet* DiameterAvpEncap::simple_action(Packet * p_in)
{
	uint32_t avpHeaderLength = ah.getVendorSpecific() ? 12 : 8;
	ah.setLength(avpHeaderLength + p_in->length());
	WritablePacket* wp = p_in->push(avpHeaderLength);
	ah.encode(wp->data());

	return wp;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterAvpEncap)
