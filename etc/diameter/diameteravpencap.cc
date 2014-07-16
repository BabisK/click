#include <click/config.h>
#include "diameteravpencap.hh"
#include <click/args.hh>
CLICK_DECLS

DiameterAvpEncap::DiameterAvpEncap(){}

int DiameterAvpEncap::configure(Vector<String> &conf, ErrorHandler *errh)
{
	uint32_t code;
	uint32_t vendorId = 0;
	bool mandatory = false;

	if (Args(conf, this, errh)
	.read_m("CODE", code)
	.read("VENDORID", vendorId)
	.read("MANDATORY", mandatory)
	.complete() < 0)
	return -1;

	ah.setCode(code);
	ah.setMandatory(mandatory);
	if(vendorId != 0)
	{
		ah.setVendorSpecific(true);
		ah.setVendorId(vendorId);
	}
	else
	{
		ah.setVendorSpecific(false);
		ah.setVendorId(0);
	}

	return 0;
}

Packet* DiameterAvpEncap::simple_action(Packet * p_in)
{
	uint32_t avpHeaderLength = ah.getVendorSpecific() ? 12 : 8;
	ah.setLength(avpHeaderLength + p_in->length());
	WritablePacket* wp = p_in->push(avpHeaderLength);
	ah.encode(wp->data());

	return wp;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterAvpEncap)
