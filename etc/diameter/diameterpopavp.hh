/*
 * diameterpopavp.hh
 *
 *  Created on: Jul 15, 2014
 *      Author: ckaidos
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

	int configure(Vector<String> &conf, ErrorHandler *errh) CLICK_COLD;
	Packet *simple_action(Packet *);
};

CLICK_ENDDECLS
#endif /* DIAMETERPOPAVP_HH_ */
