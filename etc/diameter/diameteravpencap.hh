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
