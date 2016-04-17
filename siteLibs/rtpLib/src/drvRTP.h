
struct fpSDataReq
{
};

struct fpSDataRead
{
	unsigned char sync;
	unsigned char length[2];
	unsigned char node;
	unsigned char response;
	float fval;
	unsigned char fbit;
	unsigned char crc[2];
};

typedef struct
{
	char           *link_text;  /* Original text of INST_IO link  */
	char           *PRTHost;   /* PLC, */
	char           *string_tag; /* tag, */
	size_t         index;     /* array element parsed from that: 0, 1,... */

	IOSCANPVT      ioscanpvt;
}DEVRTPData;

