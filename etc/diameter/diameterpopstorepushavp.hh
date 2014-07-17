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

#ifndef DIAMETERPOPSTOREUSHAVP_HH_
#define DIAMETERPOPSTOREUSHAVP_HH_
#include <click/element.hh>
#include "diameter.hh"
CLICK_DECLS

class DiameterPopStorePushAVP : public Element
{
public:
	DiameterPopStorePushAVP() CLICK_COLD;
	~DiameterPopStorePushAVP() CLICK_COLD;

	const char *class_name() const		{ return "DiameterPopStorePushAVP"; }
	const char *port_count() const		{ return "1/2"; }

	void push(int, Packet* p);
	Packet *simple_action(Packet *);
private:
	AvpHeader ah;
	uint8_t* data;
};

CLICK_ENDDECLS
#endif /* DIAMETERPOPSTOREUSHAVP_HH_ */
