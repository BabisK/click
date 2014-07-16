#include <click/config.h>
#include "diameterfliprequest.hh"
#include <click/args.hh>
CLICK_DECLS

DiameterFlipRequest::DiameterFlipRequest(){}

Packet* DiameterFlipRequest::simple_action(Packet * p_in)
{
	WritablePacket * wp = p_in->put(0);
	DiameterHeader dh;
	dh.decode(wp->data());
	dh.isRequest() ? dh.setRequest(false) : dh.setRequest(true);
	dh.encode(wp->data());
	return wp;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterFlipRequest)
