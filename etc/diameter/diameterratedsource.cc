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

#include <click/config.h>
#include "diameterratedsource.hh"
#include <click/args.hh>
#include <click/error.hh>
#include <click/router.hh>
#include <click/straccum.hh>
#include <click/standard/scheduleinfo.hh>
#include <click/glue.hh>
CLICK_DECLS

const unsigned DiameterRatedSource::NO_LIMIT;

DiameterRatedSource::DiameterRatedSource()
  : _packet(0), _task(this), _timer(&_task)
{
}

int
DiameterRatedSource::configure(Vector<String> &conf, ErrorHandler *errh)
{
	srand(time(NULL));
    String data =
	"Random bullshit in a packet, at least 64 bytes long. Well, now it is.";
    unsigned rate = 10;
    int limit = -1;
    bool active = true, stop = false;
    uint32_t code = 0;
    uint32_t appid = 0;
    bool request;

    if (Args(conf, this, errh)
	.read_p("RATE", rate)
	.read_p("LIMIT", limit)
	.read_p("ACTIVE", active)
	.read_m("CODE", code)
	.read_m("REQUEST", request)
	.read("APPID", appid)
	.read("STOP", stop)
	.complete() < 0)
	return -1;

    int burst = rate < 200 ? 2 : rate / 100;
    _tb.assign(rate, burst);
    _limit = (limit >= 0 ? unsigned(limit) : NO_LIMIT);
    _active = active;
    _stop = stop;
    _code = code;
    _appid = appid;

    dh.setVersion(1);
    dh.setLength(20);
    dh.setRequest(request);
    dh.setCode(_code);
    dh.setAppId(_appid);

    setup_packet();

    return 0;
}

int
DiameterRatedSource::initialize(ErrorHandler *errh)
{
    _count = 0;
    if (output_is_push(0))
	ScheduleInfo::initialize_task(this, &_task, errh);
    _tb.set(1);
    _timer.initialize(this);
    return 0;
}

void
DiameterRatedSource::cleanup(CleanupStage)
{
    if (_packet)
	_packet->kill();
    _packet = 0;
}

bool
DiameterRatedSource::run_task(Task *)
{
    if (!_active)
	return false;
    if (_limit != NO_LIMIT && _count >= _limit) {
	if (_stop)
	    router()->please_stop_driver();
	return false;
    }

    dh.setHbh(rand());
    dh.setE2e(rand());

    _tb.refill();
    if (_tb.remove_if(1)) {
	Packet *p = _packet->clone();
	p->set_timestamp_anno(Timestamp::now());
	WritablePacket *wp = p->put(0);
	wp->set_timestamp_anno(Timestamp::now());
	dh.encode(wp->data());
	output(0).push(wp);
	_count++;
	_task.fast_reschedule();
	return true;
    } else {
	_timer.schedule_after(Timestamp::make_jiffies(_tb.time_until_contains(1)));
	return false;
    }
}

Packet *
DiameterRatedSource::pull(int)
{
    if (!_active)
	return 0;
    if (_limit != NO_LIMIT && _count >= _limit) {
	if (_stop)
	    router()->please_stop_driver();
	return 0;
    }

    dh.setHbh(rand());
    dh.setE2e(rand());

    _tb.refill();
    if (_tb.remove_if(1)) {
	_count++;
	Packet *p = _packet->clone();
	p->set_timestamp_anno(Timestamp::now());
	WritablePacket *wp = p->put(0);
	wp->set_timestamp_anno(Timestamp::now());
	dh.encode(wp->data());
	return wp;
    } else
	return 0;
}

void
DiameterRatedSource::setup_packet()
{
    if (_packet)
	_packet->kill();

    // note: if you change `headroom', change `click-align'
    unsigned int headroom = 16+20+24;

    uint8_t data[20] = {0};
    _packet = Packet::make(headroom, reinterpret_cast<unsigned char*>(data), 20, 0);
}

String
DiameterRatedSource::read_param(Element *e, void *vparam)
{
  DiameterRatedSource *rs = (DiameterRatedSource *)e;
  switch ((intptr_t)vparam) {
   case 1:			// rate
    return String(rs->_tb.rate());
   case 2:			// limit
    return (rs->_limit != NO_LIMIT ? String(rs->_limit) : String("-1"));
   default:
    return "";
  }
}

int
DiameterRatedSource::change_param(const String &s, Element *e, void *vparam,
			  ErrorHandler *errh)
{
	DiameterRatedSource *rs = (DiameterRatedSource *)e;
  switch ((intptr_t)vparam) {

  case 1: {			// rate
      unsigned rate;
      if (!IntArg().parse(s, rate))
	  return errh->error("syntax error");
      rs->_tb.assign_adjust(rate, rate < 200 ? 2 : rate / 100);
      break;
  }

   case 2: {			// limit
     int limit;
     if (!IntArg().parse(s, limit))
       return errh->error("syntax error");
     rs->_limit = (limit >= 0 ? unsigned(limit) : NO_LIMIT);
     break;
   }

  case 3: {			// active
      bool active;
      if (!BoolArg().parse(s, active))
	  return errh->error("syntax error");
      rs->_active = active;
      if (rs->output_is_push(0) && !rs->_task.scheduled() && active) {
	  rs->_tb.set(1);
	  rs->_task.reschedule();
      }
      break;
  }

  case 5: {			// reset
      rs->_count = 0;
      rs->_tb.set(1);
      if (rs->output_is_push(0) && !rs->_task.scheduled() && rs->_active)
	  rs->_task.reschedule();
      break;
  }

  }
  return 0;
}

void
DiameterRatedSource::add_handlers()
{
  add_read_handler("rate", read_param, 1);
  add_write_handler("rate", change_param, 1);
  add_read_handler("limit", read_param, 2, Handler::CALM);
  add_write_handler("limit", change_param, 2);
  add_data_handlers("active", Handler::OP_READ | Handler::CHECKBOX, &_active);
  add_write_handler("active", change_param, 3);
  add_data_handlers("count", Handler::OP_READ, &_count);
  add_write_handler("reset", change_param, 5, Handler::BUTTON);

  if (output_is_push(0))
    add_task_handlers(&_task);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DiameterRatedSource)
