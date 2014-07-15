#include <click/config.h>
#include "diameterpopavp.hh"
#include <click/args.hh>
#include <click/confparse.hh>
#include <click/glue.hh>
#include <iostream>
CLICK_DECLS

DiameterPopAVP::DiameterPopAVP(){}

int
DiameterPopAVP::configure(Vector<String> &conf, ErrorHandler *errh)
{
	return 0;
}

Packet* DiameterPopAVP::simple_action(Packet * p_in)
{
	DiameterHeader dh;
	dh.decode(p_in->data());

	const uint8_t* nextAvp = p_in->data() + 20;
	AvpHeader ah;
	while(nextAvp < p_in->data() + dh.getLength())
	{
		ah.decode(nextAvp);
		nextAvp += ah.getLength();
	}
	p_in->take(ah.getLength());
	WritablePacket* wp = p_in->put(0);
	dh.setLength(dh.getLength() - ah.getLength());
	dh.encode(wp->data());

	return wp;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterPopAVP)
