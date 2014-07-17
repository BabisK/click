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

#ifndef DIAMETERAPPENDAVP_HH_
#define DIAMETERAPPENDAVP_HH_
#include <click/element.hh>
#include "diameter.hh"
CLICK_DECLS

class DiameterPushAVP : public Element
{
public:
	DiameterPushAVP() CLICK_COLD;
	const char *class_name() const		{ return "DiameterPushAVP"; }
	const char *port_count() const		{ return PORTS_1_1; }

	int configure(Vector<String> &conf, ErrorHandler *errh) CLICK_COLD;
	Packet *simple_action(Packet *);

private:
	AvpHeader ah;
	String datastring;
	uint32_t datauint;
	bool randUInt32;
	size_t randAlphaNumString;
	size_t randNumString;
	size_t randAlphaString;
	size_t randHexString;
};

CLICK_ENDDECLS
#endif /* DIAMETERAPPENDAVP_HH_ */
