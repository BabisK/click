#ifndef DIAMETERAVPDECAP_HH_
#define DIAMETERAVPDECAP_HH_
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
#endif /* DIAMETERAVPDECAP_HH_ */
