#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "ocf_mylight.h"

static OCPlatformInfo platformInfo = {
	.platformID = "C0FFEE00-BAB0-FACE-B00C-00BA5EBA1100",
	.manufacturerName = "GeekRedLed",
	.manufacturerUrl = "http://geekredled.com",
	.modelNumber = "A053LIGHT0",
	.dateOfManufacture = "2017-07-11",
	.platformVersion = "BadIotivity1.2",
	.operatingSystemVersion = "fakerOS",
	.hardwareVersion = "ARTIK053",
	.firmwareVersion = "TEST_1.0",
	.supportUrl = "http://geekredled.com",
	.systemTime = "2017-07-11T16:06:00+09:00"
};

int ocf_mylight_playform_init()
{
	OCStackResult ret;

	ret = OCSetPlatformInfo(platformInfo);
	if (ret != OC_STACK_OK) {
		DBG("Platform Registration failed! (ret=%d)", ret);
		return -1;
	}

	return 0;
}
