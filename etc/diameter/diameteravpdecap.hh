/*
 * diameteravpdecap.{cc,hh} -- element that removes a Diameter AVP header from the packet
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

#ifndef DIAMETERAVPDECAP_HH
#define DIAMETERAVPDECAP_HH
#include <click/element.hh>
#include "diameter.hh"
CLICK_DECLS

class DiameterAvpDecap : public Element
{
public:
	DiameterAvpDecap() CLICK_COLD;

	const char *class_name() const		{ return "DiameterAvpDecap"; }
    const char *port_count() const		{ return PORTS_1_1; }

    Packet *simple_action(Packet * p_in);
};

CLICK_ENDDECLS
#endif /* DIAMETERAVPDECAP_HH */
