#include <click/config.h>
#include "diameterpopavp.hh"
#include <click/args.hh>
#include <click/confparse.hh>
#include <click/glue.hh>
#include <iostream>
CLICK_DECLS

DiameterPopAVP::DiameterPopAVP(){}

Packet* DiameterPopAVP::simple_action(Packet * p_in)
{
	DiameterHeader dh;
	dh.decode(p_in->data());

	const uint8_t* nextAvp = p_in->data() + 20;
	AvpHeader ah;
	while(nextAvp < p_in->data() + dh.getLength())
	{
		ah.decode(nextAvp);
		nextAvp += ah.getLengthPadded();
	}
	p_in->take(ah.getLengthPadded());
	WritablePacket* wp = p_in->put(0);
	dh.setLength(dh.getLength() - ah.getLengthPadded());
	dh.encode(wp->data());

	return wp;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterPopAVP)
