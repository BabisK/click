/*
 * DiameterAppendAVP.hh
 *
 *  Created on: Jul 14, 2014
 *      Author: ckaidos
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
};

CLICK_ENDDECLS
#endif /* DIAMETERAPPENDAVP_HH_ */
