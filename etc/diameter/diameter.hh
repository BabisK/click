/*
 * diameter.hh
 *
 *  Created on: Jul 12, 2014
 *      Author: babis
 */

#ifndef DIAMETER_HH
#define DIAMETER_HH

#include <stdint.h>
#include <arpa/inet.h>

class DiameterHeader
{
private:
	uint8_t version;
	uint32_t length;
	bool request;
	bool proxiable;
	bool error;
	uint32_t retransmitted;
	uint32_t code;
	uint32_t appId;
	uint32_t hbh;
	uint32_t e2e;

public:
	void setVersion(uint8_t version)
	{
		this->version = version;
	}

	uint8_t getVersion() const {
		return version;
	}

	uint32_t getAppId() const {
		return appId;
	}

	void setAppId(uint32_t appId) {
		this->appId = appId;
	}

	uint32_t getCode() const {
		return code;
	}

	void setCode(uint32_t code) {
		this->code = code;
	}

	uint32_t getE2e() const {
		return e2e;
	}

	void setE2e(uint32_t e2e) {
		this->e2e = e2e;
	}

	bool isError() const {
		return error;
	}

	void setError(bool error) {
		this->error = error;
	}

	uint32_t getHbh() const {
		return hbh;
	}

	void setHbh(uint32_t hbh) {
		this->hbh = hbh;
	}

	uint32_t getLength() const {
		return length;
	}

	void setLength(uint32_t length) {
		this->length = length;
	}

	bool isProxiable() const {
		return proxiable;
	}

	void setProxiable(bool proxiable) {
		this->proxiable = proxiable;
	}

	bool isRequest() const {
		return request;
	}

	void setRequest(bool request) {
		this->request = request;
	}

	uint32_t getRetransmitted() const {
		return retransmitted;
	}

	void setRetransmitted(uint32_t retransmitted) {
		this->retransmitted = retransmitted;
	}

	void decode(uint8_t* buffer)
	{
		version = buffer[0];
		length = ((buffer[1] << 16) & 0x00FF0000) | ((buffer[2] << 8) & 0x0000FF00) | (buffer[3] & 0x000000FF);
		request = buffer[4] >> 7;
		proxiable = (buffer[4] >> 6) & 0x1;
		error = (buffer[4] >> 5) & 0x1;
		retransmitted = (buffer[4] >> 4) & 0x1;
		code = ((buffer[5] << 16) & 0x00FF0000) | ((buffer[6] << 8) & 0x0000FF00) | (buffer[7] & 0x000000FF);
		appId = ((buffer[8] << 24) & 0xFF000000) | ((buffer[9] << 16) & 0x00FF0000) | ((buffer[10] << 8) & 0x0000FF00) | (buffer[11] & 0x000000FF);
		hbh = ((buffer[12] << 24) & 0xFF000000) | ((buffer[13] << 16) & 0x00FF0000) | ((buffer[14] << 8) & 0x0000FF00) | (buffer[15] & 0x000000FF);
		e2e = ((buffer[16] << 24) & 0xFF000000) | ((buffer[17] << 16) & 0x00FF0000) | ((buffer[18] << 8) & 0x0000FF00) | (buffer[19] & 0x000000FF);
	}

	void encode(uint8_t* buffer)
	{
		buffer[0] = version;
		buffer[1] = (length >> 16) & 0xFF;
		buffer[2] = (length >> 8) & 0xFF;
		buffer[3] = length & 0xFF;
		buffer[4] = (request << 7) | (proxiable << 6) | (error << 5) | (retransmitted << 4);
		buffer[5] = (code >> 16) & 0xFF;
		buffer[6] = (code >> 8) & 0xFF;
		buffer[7] = code & 0xFF;
		buffer[8] = (appId >> 24) & 0xFF;
		buffer[9] = (appId >> 16) & 0xFF;
		buffer[10] = (appId >> 8) & 0xFF;
		buffer[11] = appId & 0xFF;
		buffer[12] = (hbh >> 24) & 0xFF;
		buffer[13] = (hbh >> 16) & 0xFF;
		buffer[14] = (hbh >> 8) & 0xFF;
		buffer[15] = hbh & 0xFF;
		buffer[16] = (e2e >> 24) & 0xFF;
		buffer[17] = (e2e >> 16) & 0xFF;
		buffer[18] = (e2e >> 8) & 0xFF;
		buffer[19] = e2e & 0xFF;
	}
};

class AvpHeader
{
private:
	uint32_t code;
	bool vendorSpecific;
	bool mandatory;
	uint32_t length;
	uint32_t vendorId;

public:
	uint32_t getCode() const {
		return code;
	}

	void setCode(uint32_t code) {
		this->code = code;
	}

	uint32_t getLength() const {
		return length;
	}

	void setLength(uint32_t length) {
		this->length = length;
	}

	bool getMandatory() const {
		return mandatory;
	}

	void setMandatory(bool mandatory) {
		this->mandatory = mandatory;
	}

	uint32_t getVendorId() const {
		return vendorId;
	}

	void setVendorId(uint32_t vendorId) {
		this->vendorId = vendorId;
	}

	bool getVendorSpecific() const {
		return vendorSpecific;
	}

	void setVendorSpecific(bool vendorSpecific) {
		this->vendorSpecific = vendorSpecific;
	}

	void decode(uint8_t* buffer)
	{
		code = ((buffer[0] << 24) & 0xFF000000) | ((buffer[1] << 16) & 0x00FF0000) | ((buffer[2] << 8) & 0x0000FF00) | (buffer[3] & 0x000000FF);
		vendorSpecific = (buffer[4] >> 7) & 0x1;
		mandatory = (buffer[4] >> 6) & 0x1;
		length = ((buffer[5] << 16) & 0x00FF0000) | ((buffer[6] << 8) & 0x0000FF00) | (buffer[7] & 0x000000FF);
		if(vendorSpecific)
		{
			vendorId = ((buffer[8] << 24) & 0xFF000000) | ((buffer[9] << 16) & 0x00FF0000) | ((buffer[10] << 8) & 0x0000FF00) | (buffer[11] & 0x000000FF);
		}
		else
		{
			vendorId = 0;
		}
	}

	void encode(uint8_t* buffer)
	{
		buffer[0] = (code >> 24) & 0xFF;
		buffer[1] = (code >> 16) & 0xFF;
		buffer[2] = (code >> 8) & 0xFF;
		buffer[3] = code & 0xFF;
		buffer[4] = (vendorSpecific << 7) | (mandatory << 6);
		buffer[5] = (length >> 16) & 0xFF;
		buffer[6] = (length >> 8) & 0xFF;
		buffer[7] = length & 0xFF;
		if(vendorSpecific)
		{
			buffer[8] = (vendorId >> 24) & 0xFF;
			buffer[9] = (vendorId >> 16) & 0xFF;
			buffer[10] = (vendorId >> 8) & 0xFF;
			buffer[11] = vendorId & 0xFF;
		}
	}
};

/*struct diameterHeader
{
	uint8_t version : 8;
	uint32_t length : 24;
	uint32_t request : 1;
	uint32_t proxiable : 1;
	uint32_t error : 1;
	uint32_t retransmitted : 1;
	uint32_t : 4;
	uint32_t code : 24;
	uint32_t appId : 32;
	uint32_t hbh : 32;
	uint32_t e2e : 32;
};*/

/*struct avpHeader
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
};*/



#endif /* DIAMETER_HH_ */
