#ifndef __protocmn_h__
#define __protocmn_h__

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_ENC_PARAMS_SIZE 1024

#define BECAUSE_OF_TIME		1
#define BECAUSE_OF_RANGE	2

#define STR_STATUS_WRONG_PARAMS		0	//invalid parameters
#define STR_STATUS_NOT_VALID_MSG	1	//invalid message in stream;
#define STR_STATUS_NO_MSG			2	//empty stream (no message);
#define STR_STATUS_NOT_WHOLE_MSG	3	//not whole message in stream;
#define STR_STATUS_VALID_MSG		4	//valid message in stream

#define CODE_NOT_EXIST (-1)

int find_code_offset(unsigned char* stream,int stream_length,unsigned char code);

void printf_buf_hex(unsigned char* buf,int size,char* header);
void printf_buf_dec(unsigned char* buf,int size,char* header);
void printf_buf_hex_16(unsigned char* buf,int size,char* header);
void printf_buf_hex_16c(unsigned char* buf,int size,char* header);
void printf_buf_hex_N(unsigned char* buf,int size,char* header,int N);
void printf_buf_ascii_16(unsigned char* buf,int size,char* header);

void printf_buf_ascii_16fx(unsigned char* buf,int size,char* header);
void printf_buf_ascii_32fx(unsigned char* buf,int size,char* header);

int is_pascal_string(unsigned char* stream,char* dstbuf,int* plength);
int enc_pascal_string(char* text,int textsize,unsigned char* stream);

#define RX_RESULT_OK			0
#define RX_RESULT_PORTERROR		-1
#define RX_RESULT_MSGFROMUPPER	-2
#define RX_RESULT_TIMEOUT		-3
#define RX_RESULT_UNKNOWNERR	-4

typedef int (*method_recv)(	void* stuff,unsigned char* buffer,int* psize,int timeout,void* exit_event);
typedef int (*method_send)(	void* stuff,unsigned char* data,int size,int *pwrite,int timeout);
typedef int (*method_setup_port)(	void* staff,int bps,int parity,int databits,int stopbits);
typedef int (*method_purge_port)(	void* staff);
typedef int (*method_open_port)(	void** pstaff);
typedef int (*method_close_port)(	void** pstaff);

#ifdef __cplusplus
}
#endif

#endif /* __protocmn_h__ */
