#include "picohttpparser/picohttpparser.h"

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define HL_NAME(n) picohttpparser_##n

#include <hl.h>

typedef struct {
	hl_type * t;
	vbyte * method;
	vbyte * path;
	int request_size;
	int minor_version;
	int header_count;
	varray *headers;
} hlphr_data;

int pret, minor_version;
const char * method, *path;
size_t method_len = 0, path_len = 0, num_headers;

HL_PRIM hlphr_data* HL_NAME(parse_request)(const vbyte * buf, int max_header) {
	struct phr_header headers[max_header];
	const char* cb = (char*) buf;
	num_headers = sizeof(headers) / sizeof(headers[0]);
	pret = phr_parse_request(cb, strlen(cb), &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);

	hlphr_data * ret = malloc(sizeof(hlphr_data));
	char * method_s = (char*) hl_alloc_bytes(method_len+1);
	strncpy(method_s, method, method_len);
	char * path_s = (char*) hl_alloc_bytes(path_len+1);
	strncpy(path_s, path, path_len);
	
	ret->method = (vbyte*) method_s;
	ret->path = (vbyte*) path_s;
	ret->minor_version = minor_version;
	ret->request_size = pret;
	ret->header_count = num_headers;

	ret->headers = hl_alloc_array(&hlt_dynobj, (int)num_headers);
	
	for(int i = 0; i < num_headers; i++) {
		vdynamic *header = (vdynamic*) hl_alloc_dynobj();
		hl_dyn_setp(header, hl_hash_utf8("name"), &hlt_bytes, hl_copy_bytes(headers[i].name, headers[i].name_len) );
		hl_dyn_setp(header, hl_hash_utf8("value"), &hlt_bytes, hl_copy_bytes(headers[i].value, headers[i].value_len) );

		hl_aptr(ret->headers, vdynamic*)[i] = header;
	}

	return ret;
}

HL_PRIM void HL_NAME(free_pico_data)(hlphr_data * data) {
	free(data);
}

#define PHR_DATA _OBJ(_BYTES _BYTES _I32 _I32 _I32 _ARR)
DEFINE_PRIM(PHR_DATA, parse_request, _BYTES _I32);
DEFINE_PRIM(_VOID, free_pico_data, PHR_DATA);
