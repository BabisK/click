#ifndef SAMPLEPACKAGEELEMENT_HH
#define SAMPLEPACKAGEELEMENT_HH
#include <click/element.hh>
CLICK_DECLS

/*
 * =c
 * SamplePackageElement()
 * =s debugging
 * demonstrates how to write a package
 * =d
 *
 * This is the only element in the `sample' package. It demonstrates how to
 * write an element that will be placed in a package. It does nothing except
 * report that the package was successfully loaded when it initializes. */

class GTP : public Element {
public:

	GTP() ;		// SEE sample.cc FOR CONSTRUCTOR
    ~GTP() ;		// SEE sample.cc FOR DESTRUCTOR

    const char * class_name() const	{ return "GTP"; }
    const char * port_count()const{ return "1/1";}
    const char * processing() const { return PUSH; }
    //void push(int, Packet *);

    int initialize(ErrorHandler *errh);
    
    //Packet * pull(int, Packet *);
    //const char *flow_code() const     { return "1/1"; }
    Packet * simple_action(Packet * p);
    
};

CLICK_ENDDECLS
#endif
