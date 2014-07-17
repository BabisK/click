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

#ifndef DIAMETERPOPAVP_HH_
#define DIAMETERPOPAVP_HH_
#include <click/element.hh>
#include "diameter.hh"
CLICK_DECLS

class DiameterPopAVP : public Element
{
public:
	DiameterPopAVP() CLICK_COLD;
	const char *class_name() const		{ return "DiameterPopAVP"; }
	const char *port_count() const		{ return PORTS_1_1; }

	Packet *simple_action(Packet *);
};

CLICK_ENDDECLS
#endif /* DIAMETERPOPAVP_HH_ */
