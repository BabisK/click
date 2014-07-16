#ifndef DIAMETERFLIPREQUEST_HH_
#define DIAMETERFLIPREQUEST_HH_
#include <click/element.hh>
#include "diameter.hh"
CLICK_DECLS

class DiameterFlipRequest : public Element
{
public:
	DiameterFlipRequest() CLICK_COLD;

	const char *class_name() const		{ return "DiameterFlipRequest"; }
	const char *port_count() const		{ return PORTS_1_1; }

	Packet *simple_action(Packet * p_in);
};

CLICK_ENDDECLS
#endif /* DIAMETERFLIPREQUEST_HH_ */
