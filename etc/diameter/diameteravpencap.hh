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

#ifndef DIAMETERAVPENCAP_HH_
#define DIAMETERAVPENCAP_HH_
#include <click/element.hh>
#include "diameter.hh"
CLICK_DECLS

class DiameterAvpEncap : public Element
{
public:
	DiameterAvpEncap() CLICK_COLD;

	const char *class_name() const		{ return "DiameterAvpEncap"; }
    const char *port_count() const		{ return PORTS_1_1; }

    int configure(Vector<String> &conf, ErrorHandler *errh) CLICK_COLD;
    Packet *simple_action(Packet * p_in);

private:
    AvpHeader ah;
};

CLICK_ENDDECLS
#endif /* DIAMETERAVPENCAP_HH_ */
