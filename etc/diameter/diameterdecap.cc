#include <click/config.h>
#include "diameterdecap.hh"
#include <click/args.hh>
CLICK_DECLS

DiameterDecap::DiameterDecap(){}

Packet* DiameterDecap::simple_action(Packet * p_in)
{
	p_in->pull(20);
	return p_in;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterDecap)
