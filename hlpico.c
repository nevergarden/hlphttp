#include "picohttpparser/picohttpparser.h"

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define HL_NAME(n) picohttpparser_##n

#include <hl.h>

typedef struct {
	hl_type * t;
	vbyte * name;
	vbyte * value;
} hlphr_header;

typedef struct {
	hl_type * t;
	hlphr_header * headers;
	int header_count;
} hlphr_headers;

typedef struct {
	hl_type * t;
	vbyte * method;
	vbyte * path;
	int request_size;
	int minor_version;
	hlphr_headers *headers;
} hlphr_data;

int pret, minor_version;
const char * method, *path;
size_t method_len = 0, path_len = 0, num_headers = 0;

HL_PRIM hlphr_data* HL_NAME(parse_request)(const vbyte * buf, int max_header) {
	struct phr_header headers[max_header];
	const char* cb = (char*) buf;
	pret = phr_parse_request(cb, strlen(cb), &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);

	hlphr_data * ret = malloc(sizeof(hlphr_data));
	char * method_s = (char*) hl_alloc_bytes(method_len);
	strncpy(method_s, method, method_len);
	char * path_s = (char*) hl_alloc_bytes(path_len);
	strncpy(path_s, path, path_len);

	ret->method = (vbyte*) method_s;
	ret->path = (vbyte*) path_s;
	ret->minor_version = minor_version;
	ret->request_size = pret;

	/*
	hlphr_header headers_s[num_headers];

	int i = 0;
	for(i = 0; i != num_headers; ++i) {
		headers_s[i].name = (vbyte*) headers[i].name;
		headers_s[i].value = (vbyte*) headers[i].value;
	}
	

	ret->headers->headers = headers_s;
	ret->headers->header_count = num_headers;
	*/

	return ret;
}

HL_PRIM void HL_NAME(free_pico_data)(hlphr_data * data) {
	// free(data->headers);
	free(data);
}

#define PHR_DATA _OBJ(_BYTES _BYTES _I32 _I32 _DYN)
DEFINE_PRIM(PHR_DATA, parse_request, _BYTES _I32);
DEFINE_PRIM(_VOID, free_pico_data, PHR_DATA);
