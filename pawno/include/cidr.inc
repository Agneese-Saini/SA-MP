stock cidr_ip2long(const ip[]) {
	new parts[4];
	
	#if defined sscanf
		if (sscanf(address, "p<.>a<i>[4]", parts) == SSCANF_FALSE_RETURN) {
		    return 0;
		}
	#else
	    new pos;
	    parts[0] = strval(ip);

		if ((pos = strfind(ip, ".", false, 0)) == -1) {
	        return 0;
		}
		parts[1] = strval(ip[pos + 1]);

		if ((pos = strfind(ip, ".", false, pos)) == -1) {
	        return 0;
		}
		parts[2] = strval(ip[pos + 1]);

		if ((pos = strfind(ip, ".", false, pos)) == -1) {
	        return 0;
		}
		parts[3] = strval(ip[pos + 1]);
	#endif
	
	return ((parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3]);
}

stock cidr_match(const ip[], const range[]) {
	new pos, rangeip[18], bits, mask;
	
	if ((pos = strfind(range, "/", true, 0)) == -1) {
		bits = 26; // default bits
	} else {
		bits = strval(range[pos + 1]);
	}
	
	strmid(rangeip, range, 0, pos, sizeof(rangeip));
	
	mask = -1 << (32 - bits);
	
	return ((cidr_ip2long(ip) & mask) == (cidr_ip2long(rangeip) & mask));
}
