/*
 * diameterratedsource.{cc,hh} -- element that creates packets containing a diameter header,
 * much like the RatedSource() element of the standard package
 * Charalampos "Babis" Kaidos
 *
 * Copyright (c) 2014 Intracom Telecom
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#ifndef DIAMETERRATEDSOURCE_HH_
#define DIAMETERRATEDSOURCE_HH_
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
