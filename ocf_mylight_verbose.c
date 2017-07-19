#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "ocf_mylight.h"

static void _method(OCMethod method)
{
	if (method & OC_REST_NOMETHOD)
		printf("NOMETHOD ");
	if (method & OC_REST_GET)
		printf("GET ");
	if (method & OC_REST_PUT)
		printf("PUT ");
	if (method & OC_REST_POST)
		printf("POST ");
	if (method & OC_REST_DELETE)
		printf("DELETE ");
	if (method & OC_REST_OBSERVE)
		printf("OBSERVE ");
	if (method & OC_REST_OBSERVE_ALL)
		printf("OBSERVE_ALL ");
	if (method & OC_REST_PRESENCE)
		printf("PRESENCE ");
	if (method & OC_REST_DISCOVER)
		printf("DISCOVER ");
}

static void _flag(OCEntityHandlerFlag flag)
{
	if (flag & OC_REQUEST_FLAG)
		printf("REQUEST ");
	if (flag & OC_OBSERVE_FLAG)
		printf("OBSERVE ");
}

static void _ll(OCStringLL *ll)
{
	OCStringLL *cur = ll;

	while (cur) {
		printf("%s ", cur->value);
		cur = cur->next;
	}
}

static void _ll_rep_value(OCRepPayloadValue *values)
{
	OCRepPayloadValue *cur = values;

	while (cur) {
		printf("{\"%s\": ", cur->name);

		switch (cur->type) {
		case OCREP_PROP_INT:
			printf("%zd} ", cur->i);
			break;
		case OCREP_PROP_DOUBLE:
			printf("%f} ", cur->d);
			break;
		case OCREP_PROP_BOOL:
			printf("%d} ", cur->b);
			break;
		case OCREP_PROP_STRING:
			printf("\"%s\"} ", cur->str);
			break;
		case OCREP_PROP_BYTE_STRING:
			printf("len=%zd [...]} ", cur->ocByteStr.len);
			break;
		case OCREP_PROP_OBJECT:
			printf("{0x%p}}", cur->obj);
			break;
		case OCREP_PROP_ARRAY:
			printf("[type=%d]}", cur->arr.type);
			break;
		default:
			printf("unknown} ");
			break;
		}

		cur = cur->next;
	}
}

static void _payload_representation(const char *indent, OCRepPayload *payload)
{
	printf("%s  - payload-type: %d (%s)\n", indent, payload->base.type,
			"Representation");
	printf("%s    - uri: %s\n", indent, payload->uri);

	if (payload->types) {
		printf("%s    - types: ", indent);
		_ll(payload->types);
		printf("\n");
	}

	if (payload->interfaces) {
		printf("%s    - interfaces: ", indent);
		_ll(payload->interfaces);
		printf("\n");
	}

	if (payload->values) {
		printf("%s    - values: ", indent);
		_ll_rep_value(payload->values);
		printf("\n");
	}
}

void ocf_mylight_verbose_payload(const char *indent, OCPayload *payload)
{
	if (!payload)
		return;

	if (payload->type == PAYLOAD_TYPE_REPRESENTATION)
		_payload_representation(indent, (OCRepPayload *) payload);
	else
		printf("%s  - payload-type: %d\n", indent, payload->type);
}

static void _header_options(OCHeaderOption *opt, uint8_t count)
{
	int i;

	printf("  - vendor specific header count: %d\n", count);
	for (i = 0; i < count; i++)
		printf("  - [%d] proto_id=%d, option_id=0x%x, option_len=%d\n",
				i, opt[i].protocolID, opt[i].optionID,
				opt[i].optionLength);
}

void ocf_mylight_verbose_request(OCEntityHandlerFlag flag,
		OCEntityHandlerRequest *req)
{
	MSG("IncomingRequest: (id: %d)", req->messageID);
	printf("  - resourceHandle: 0x%p\n", req->resource);
	printf("    - uri: '%s'\n", OCGetResourceUri(req->resource));
	printf("    - property: 0x%X\n",
			OCGetResourceProperties(req->resource));
	printf("  - flag: 0x%X ( ", flag);
	_flag(flag);
	printf(")\n");

	printf("  - method: %d ( ", req->method);
	_method(req->method);
	printf(")\n");

	printf("  - query: '%s'\n", req->query);
	printf("  - dev_addr: adapter:%d, flags:0x%X, port:%d\n",
			req->devAddr.adapter, req->devAddr.flags,
			req->devAddr.port);

	if (req->numRcvdVendorSpecificHeaderOptions > 0)
		_header_options(req->rcvdVendorSpecificHeaderOptions,
				req->numRcvdVendorSpecificHeaderOptions);

	ocf_mylight_verbose_payload("", req->payload);
}

void ocf_mylight_verbose_response(OCEntityHandlerResponse *resp)
{
	MSG("OutgoingResponse:");
	printf("  - resourceHandle: 0x%p\n", resp->resourceHandle);
	printf("  - result: %d\n", resp->ehResult);
	printf("  - resourceUri: '%s'\n", resp->resourceUri);
	printf("  - persistentBufferFlag: %d\n", resp->persistentBufferFlag);

	if (resp->numSendVendorSpecificHeaderOptions > 0)
		_header_options(resp->sendVendorSpecificHeaderOptions,
				resp->numSendVendorSpecificHeaderOptions);

	ocf_mylight_verbose_payload("", resp->payload);
}
