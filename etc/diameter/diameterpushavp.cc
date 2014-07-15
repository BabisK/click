#include <click/config.h>
#include "diameterpushavp.hh"
#include <click/args.hh>
#include <click/confparse.hh>
#include <click/glue.hh>
#include <iostream>
CLICK_DECLS

DiameterPushAVP::DiameterPushAVP()
: datastring(), datauint(0) {}

int
DiameterPushAVP::configure(Vector<String> &conf, ErrorHandler *errh)
{
	srand(time(NULL));

	uint32_t code;
	bool vendorSpec;
	uint32_t vendorId;


	if (Args(conf, this, errh)
		.read_m("CODE", code)
		.read("VENDORSPEC", vendorSpec)
		.read("VENDOR", vendorId)
		.read("OCTETSTRING", datastring)
		.read("UINT32", datauint)
		.complete() < 0)
		return -1;

	ah.setCode(code);
	ah.setMandatory(false);
	ah.setVendorSpecific(vendorSpec);
	if(vendorSpec)
	{
		ah.setVendorId(vendorId);
	}
	else
	{
		ah.setVendorId(0);
	}

	return 0;
}

Packet* DiameterPushAVP::simple_action(Packet * p_in)
{
	uint32_t avpHeaderLength = ah.getVendorSpecific() ? 12 : 8;
	uint32_t avpDataLength = datastring.length() > 0 ? datastring.length() : (datauint > 0 ? 4 : 0);
	uint32_t avpLength = avpHeaderLength + (((avpDataLength + 3)/4)*4);
	WritablePacket *p = p_in->put(avpLength);

	DiameterHeader dh;
	dh.decode(p->data());

	memset(p->data() + dh.getLength(), 0, avpLength);

	ah.setLength(avpLength);
	ah.encode(p->data() + dh.getLength());
	if(datastring.length() > 0)
	{
		memcpy(p->data() + dh.getLength() + avpHeaderLength, datastring.data(), avpDataLength);
	}
	else
	{
		uint32_t hton = htonl(datauint);
		memcpy(p->data() + dh.getLength() + avpHeaderLength, &hton, avpDataLength);
	}

	dh.setLength(dh.getLength() + ah.getLength());
	dh.encode(p->data());

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterPushAVP)
