/*
 * diameter.hh
 *
 *  Created on: Jul 12, 2014
 *      Author: babis
 */

#ifndef DIAMETER_HH
#define DIAMETER_HH

struct diameterHeader
{
	uint8_t version : 8;
	uint32_t length : 24;
	uint8_t request : 1;
	uint8_t proxiable : 1;
	uint8_t error : 1;
	uint8_t retransmitted : 1;
	uint8_t : 4;
	uint32_t code : 32;
	uint32_t appId : 32;
	uint32_t hbh : 32;
	uint32_t e2e : 32;
};

struct avpHeader
{
	uint32_t code : 32;
	uint8_t vendorSpecific : 1;
	uint8_t mandatory : 2;
	uint8_t : 6;
	uint32_t length : 24;
};

struct avpHeaderVendor
{
	uint32_t code : 32;
	uint8_t vendorSpecific : 1;
	uint8_t mandatory : 2;
	uint8_t : 6;
	uint32_t length : 24;
	uint32_t vendor : 32;
};



#endif /* DIAMETER_HH_ */
