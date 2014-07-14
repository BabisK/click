#ifndef DIAMETERINFINITESOURCE_HH_
#define DIAMETERINFINITESOURCE_HH_
#include <click/element.hh>
#include <click/tokenbucket.hh>
#include <click/task.hh>
#include "diameter.hh"
CLICK_DECLS

class DiameterRatedSource : public Element
{
public:
	DiameterRatedSource() CLICK_COLD;

	const char *class_name() const		{ return "DiameterRatedSource"; }
    const char *port_count() const		{ return PORTS_0_1; }
    void add_handlers() CLICK_COLD;

    int configure(Vector<String> &conf, ErrorHandler *errh) CLICK_COLD;
    int initialize(ErrorHandler *errh) CLICK_COLD;
    void cleanup(CleanupStage) CLICK_COLD;

    bool run_task(Task *task);
    Packet *pull(int);

  protected:

    static const unsigned NO_LIMIT = 0xFFFFFFFFU;

    TokenBucket _tb;
    unsigned _count;
    unsigned _limit;
    bool _active;
    bool _stop;
    uint32_t _code;
    uint32_t _appid;
    Packet *_packet;
    Task _task;
    Timer _timer;
    DiameterHeader dh;

    void setup_packet();

    static String read_param(Element *, void *) CLICK_COLD;
    static int change_param(const String &, Element *, void *, ErrorHandler *);

};



CLICK_ENDDECLS
#endif /* DIAMETERINFINITESOURCE_HH_ */
