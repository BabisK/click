/*
 * diameter.{cc,hh} -- header for basic Diameter (IETF RFC 6733) structures
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

#ifndef DIAMETER_HH
#define DIAMETER_HH

#include <stdint.h>

class DiameterHeader {
private:
	uint8_t version;
	uint32_t length;
	bool request;
	bool proxiable;
	bool error;
	bool retransmitted;
	uint32_t code;
	uint32_t appId;
	uint32_t hbh;
	uint32_t e2e;

public:
	void setVersion(uint8_t version) {
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

	bool isRetransmitted() const {
		return retransmitted;
	}

	void setRetransmitted(bool retransmitted) {
		this->retransmitted = retransmitted;
	}

	void decode(const uint8_t* buffer) {
		version = buffer[0];
		length = ((buffer[1] << 16) & 0x00FF0000)
				| ((buffer[2] << 8) & 0x0000FF00) | (buffer[3] & 0x000000FF);
		request = buffer[4] >> 7;
		proxiable = (buffer[4] >> 6) & 0x1;
		error = (buffer[4] >> 5) & 0x1;
		retransmitted = (buffer[4] >> 4) & 0x1;
		code = ((buffer[5] << 16) & 0x00FF0000)
				| ((buffer[6] << 8) & 0x0000FF00) | (buffer[7] & 0x000000FF);
		appId = ((buffer[8] << 24) & 0xFF000000)
				| ((buffer[9] << 16) & 0x00FF0000)
				| ((buffer[10] << 8) & 0x0000FF00) | (buffer[11] & 0x000000FF);
		hbh = ((buffer[12] << 24) & 0xFF000000)
				| ((buffer[13] << 16) & 0x00FF0000)
				| ((buffer[14] << 8) & 0x0000FF00) | (buffer[15] & 0x000000FF);
		e2e = ((buffer[16] << 24) & 0xFF000000)
				| ((buffer[17] << 16) & 0x00FF0000)
				| ((buffer[18] << 8) & 0x0000FF00) | (buffer[19] & 0x000000FF);
	}

	void encode(uint8_t* buffer) {
		memset(buffer, 0, 20);
		buffer[0] = version;
		buffer[1] = (length >> 16) & 0xFF;
		buffer[2] = (length >> 8) & 0xFF;
		buffer[3] = length & 0xFF;
		buffer[4] = (request << 7) | (proxiable << 6) | (error << 5)
				| (retransmitted << 4);
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

class AvpHeader {
private:
	uint32_t code;
	bool vendorSpecific;
	bool mandatory;
	uint32_t length;
	uint32_t vendorId;

public:
	AvpHeader() :
			code(0), vendorSpecific(false), mandatory(false), length(0), vendorId(
					0) {
	}

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

	uint32_t getLengthPadded() const {
		return (((this->length + 3) / 4) * 4);
	}

	void decode(const uint8_t* buffer) {
		code = ((buffer[0] << 24) & 0xFF000000)
				| ((buffer[1] << 16) & 0x00FF0000)
				| ((buffer[2] << 8) & 0x0000FF00) | (buffer[3] & 0x000000FF);
		vendorSpecific = (buffer[4] >> 7) & 0x1;
		mandatory = (buffer[4] >> 6) & 0x1;
		length = ((buffer[5] << 16) & 0x00FF0000)
				| ((buffer[6] << 8) & 0x0000FF00) | (buffer[7] & 0x000000FF);
		if (vendorSpecific) {
			vendorId = ((buffer[8] << 24) & 0xFF000000)
					| ((buffer[9] << 16) & 0x00FF0000)
					| ((buffer[10] << 8) & 0x0000FF00)
					| (buffer[11] & 0x000000FF);
		} else {
			vendorId = 0;
		}
	}

	void encode(uint8_t* buffer) {
		if (vendorSpecific) {
			memset(buffer, 0, 12);
		} else {
			memset(buffer, 0, 8);
		}

		buffer[0] = (code >> 24) & 0xFF;
		buffer[1] = (code >> 16) & 0xFF;
		buffer[2] = (code >> 8) & 0xFF;
		buffer[3] = code & 0xFF;
		buffer[4] = (vendorSpecific << 7) | (mandatory << 6);
		buffer[5] = (length >> 16) & 0xFF;
		buffer[6] = (length >> 8) & 0xFF;
		buffer[7] = length & 0xFF;
		if (vendorSpecific) {
			buffer[8] = (vendorId >> 24) & 0xFF;
			buffer[9] = (vendorId >> 16) & 0xFF;
			buffer[10] = (vendorId >> 8) & 0xFF;
			buffer[11] = vendorId & 0xFF;
		}
	}
};

#endif /* DIAMETER_HH_ */
