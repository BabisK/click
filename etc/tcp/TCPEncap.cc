#include <click/config.h>
#include "TCPEncap.hh"
#include <click/args.hh>
CLICK_DECLS

TCPEncap::TCPEncap()
: src(0), dst(0), seqExists(false), ackExists(false), seq(0), ack(0)
{
}

int TCPEncap::configure(Vector<String> &conf, ErrorHandler *errh)
{
	srand(time(NULL));

	if (Args(conf, this, errh)
	.read_m("SRC", src)
	.read_m("DST", dst)
	.read("SEQ", seq)
	.read("ACK", ack)
	.complete() < 0)
	return -1;

	src = htons(src);
	dst = htons(dst);

	if(seq != 0)
	{
		seqExists = true;
	}

	if(ack != 0)
	{
		ackExists = true;
	}

	return 0;
}

Packet* TCPEncap::simple_action(Packet * p_in)
{
	if(seqExists == false)
	{
		seq = rand();
	}

	if(ackExists == false)
	{
		ack = rand();
	}

	WritablePacket* wp = p_in->push(20);

	memcpy(wp->data(), &src, 2);
	memcpy(wp->data() + 2, &dst, 2);
	memset(wp->data() + 4, 0, 3);
	memset(wp->data() + 7, 1, 1);
	memset(wp->data() + 8, 0, 4);
	memset(wp->data() + 12, 0x50, 1);
	memset(wp->data() + 13, 0, 1);
	memset(wp->data() + 14, 0xFFFF, 2);
	memset(wp->data() + 16, 0, 4);

	return wp;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TCPEncap)
