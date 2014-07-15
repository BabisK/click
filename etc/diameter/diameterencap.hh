#ifndef DIAMETERENCAP_HH_
#define DIAMETERENCAP_HH_
#include <click/element.hh>
#include "diameter.hh"
CLICK_DECLS

class DiameterEncap : public Element
{
public:
	DiameterEncap() CLICK_COLD;

	const char *class_name() const		{ return "DiameterEncap"; }
    const char *port_count() const		{ return PORTS_1_1; }

    int configure(Vector<String> &conf, ErrorHandler *errh) CLICK_COLD;
    Packet *simple_action(Packet * p_in);

private:
    DiameterHeader dh;
};

CLICK_ENDDECLS
#endif /* DIAMETERENCAP_HH_ */
