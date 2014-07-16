#ifndef DIAMETERDECAP_HH_
#define DIAMETERDECAP_HH_
#include <click/element.hh>
#include "diameter.hh"
CLICK_DECLS

class DiameterDecap : public Element
{
public:
	DiameterDecap() CLICK_COLD;

	const char *class_name() const		{ return "DiameterDecap"; }
    const char *port_count() const		{ return PORTS_1_1; }

    Packet *simple_action(Packet * p_in);
};

CLICK_ENDDECLS
#endif /* DIAMETERDECAP_HH_ */
