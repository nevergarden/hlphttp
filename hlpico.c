#include "picohttpparser/picohttpparser.h"

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define HL_NAME(n) picohttpparser_##n

#include <hl.h>

typedef struct
{
	int name_len;
	int name_pos;
	int value_len;
	int value_pos;
} hlphr_header;

typedef struct
{
	hl_type *t;
	int method_len;
	char *method;
	int path_len;
	char *path;
	int request_size;
	int minor_version;
	int header_count;
	hlphr_header *headers;
} hlphr_data;

HL_PRIM hlphr_data *HL_NAME(parse_request)(const vbyte *buf, int max_header)
{
	int pret, minor_version;
	const char *method, *path;
	size_t method_len = 0, path_len = 0, num_headers;

	struct phr_header headers[max_header];
	const char *cb = (char *)buf;
	num_headers = sizeof(headers) / sizeof(headers[0]);
	pret = phr_parse_request(cb, strlen(cb), &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);

	hlphr_data *ret = malloc(sizeof(hlphr_data));
	char *method_s = malloc(method_len);
	strncpy(method_s, method, method_len);
	char *path_s = malloc(path_len);
	strncpy(path_s, path, path_len);

	ret->method_len = (int) method_len;
	ret->method = method_s;
	ret->path_len = (int) path_len;
	ret->path = path_s;
	ret->minor_version = minor_version;
	ret->request_size = pret;
	ret->header_count = num_headers;

	ret->headers = malloc(sizeof(hlphr_header)*ret->header_count);
	for(int i = 0; i < ret->header_count; i++) {
		ret->headers[i].name_len = headers[i].name_len;
		ret->headers[i].name_pos = ((const unsigned char *)headers[i].name) - buf;
		ret->headers[i].value_len = headers[i].value_len;
		ret->headers[i].value_pos = ((const unsigned char *)headers[i].value) - buf;
	}

	return ret;
}

HL_PRIM void HL_NAME(free_pico_data)(hlphr_data *data)
{
	free(data->headers);
	free(data->method);
	free(data->path);
	free(data);
}

#define PHR_DATA _OBJ(_I32 _BYTES _I32 _BYTES _I32 _I32 _I32 _ARR)
DEFINE_PRIM(PHR_DATA, parse_request, _BYTES _I32);
DEFINE_PRIM(_VOID, free_pico_data, PHR_DATA);
