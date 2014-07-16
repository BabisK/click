#ifndef SAMPLEPACKAGEELEMENT_HH
#define SAMPLEPACKAGEELEMENT_HH
#include <click/element.hh>
CLICK_DECLS

class TCPEncap : public Element
{
public:
	TCPEncap() CLICK_COLD;		// SEE sample.cc FOR CONSTRUCTOR

    const char *class_name() const	{ return "TCPEncap"; }
    const char *port_count() const		{ return PORTS_1_1; }

    int configure(Vector<String> &conf, ErrorHandler *errh) CLICK_COLD;
    Packet *simple_action(Packet * p_in);

private:
    uint16_t src;
    uint16_t dst;
    bool seqExists;
    bool ackExists;
    uint32_t seq;
    uint32_t ack;
};

CLICK_ENDDECLS
#endif
