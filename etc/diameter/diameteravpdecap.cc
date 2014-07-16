#include <click/config.h>
#include "diameteravpdecap.hh"
#include <click/args.hh>
CLICK_DECLS

DiameterAvpDecap::DiameterAvpDecap(){}

Packet* DiameterAvpDecap::simple_action(Packet * p_in)
{
	AvpHeader ah;
	ah.decode(p_in->data());
	p_in->pull(ah.getVendorSpecific() ? 12 : 8);

	return p_in;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterAvpDecap)
