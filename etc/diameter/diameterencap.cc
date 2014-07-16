#include <click/config.h>
#include "diameterencap.hh"
#include <click/args.hh>
CLICK_DECLS

DiameterEncap::DiameterEncap(){}

int DiameterEncap::configure(Vector<String> &conf, ErrorHandler *errh)
{
	srand(time(NULL));

	uint32_t code;
	bool request;
	bool error = false;
	bool proxiable = false;
	bool retransmited = false;
	uint32_t appid = 0;

	if (Args(conf, this, errh)
	.read_m("CODE", code)
	.read_m("REQUEST", request)
	.read("ERROR", error)
	.read("PROXIABLE", proxiable)
	.read("RETRANSMITED", retransmited)
	.read("APPID", appid)
	.complete() < 0)
	return -1;

	dh.setCode(code);
	dh.setRequest(request);
	dh.setError(error);
	dh.setProxiable(proxiable);
	dh.setRetransmitted(retransmited);
	dh.setAppId(appid);

	return 0;
}

Packet* DiameterEncap::simple_action(Packet * p_in)
{
    dh.setHbh(rand());
    dh.setE2e(rand());
    dh.setLength(p_in->length());

	WritablePacket* wp = p_in->push(20);
	dh.encode(wp->data());

	return wp;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterEncap)
