#pragma once
#include <string>

#define RD_PROTOCAL_VERSION "1.0"

#define CMD_ITEM_SIZE_MAX 10485760 // 10M

typedef enum RD_CMD_CODE
{
	RDCC_INVALID					= 0x0,
	RDCC_PROTOCOL_VERSION			= 0x110,
	RDCC_LOGON						= 0x120,
	RDCC_LOGON_RESULT				= 0x121,
	RDCC_COMPRESS_MODE				= 0x130,
	RDCC_TRANSFER_BITMAP			= 0x140,
	RDCC_TRANSFER_MODIFY_BITMAP		= 0x141,
	RDCC_TRANSFER_MOUSE_EVENT		= 0x142,
	RDCC_TRANSFER_KEYBOARD_EVENT	= 0x143,
	RDCC_TRANSFER_CLIPBOARD			= 0x144,
	RDCC_TRANSFER_FILE				= 0x145,
	RDCC_TRANSFER_DIRECTORY			= 0x146,
	RDCC_TRANSFER_BITMAP_RESPONSE	= 0x147,
	RDCC_DISCONNECT					= 0x200,
} RD_CMD_CODE;

typedef struct BitmapInfo {
	BITMAPINFOHEADER bmiHeader;
	union {
		struct {
			DWORD red;
			DWORD green;
			DWORD blue;
		} mask;
		RGBQUAD color[256];
	};
} BitmapInfo;

typedef enum RD_CONNECTION_STATE
{
	RDCS_INVALID			= 0x0,		// ��ʼ״̬��δ����ʱ
	RDCS_CONNECTED			= 0x100,	// TCP�������ˣ���ûͨ����֤
	RDCS_PROTOCOL_VERSION	= 0x110,	// ��֤�汾
	RDCS_LOGON				= 0x120,	// �û��������¼
	RDCS_LOGON_RESULT		= 0x121,	// ��¼���
	RDCS_COMPRESS_MODE		= 0x130,	// ȷ��ѹ����ʽ
	RDCS_TRANSFER			= 0x140,	// ��֤��ɣ�����ͼ��
	RDCS_DISCONNECTED		= 0x200,	// �Ͽ�����
} RD_CONNECTION_STATE;

typedef enum RD_COMPRESS_MODE
{
	RDCM_RAW	= 0x0,	// ԭʼ���ݣ���ѹ��
	RDCM_ZIP	= 0x1,	// zipѹ��
} RD_COMPRESS_MODE;

typedef enum RD_ERROR_CODE
{
	RDEC_SUCCEEDED							= 0x0,
	RDEC_CONNECT_FAILED						= 0x100,
	RDEC_PROTOCOL_VERSION_FAILED			= 0x110,
	RDEC_LOGON_FAILED						= 0x120,
	RDEC_COMPRESS_MODE_NOT_SUPPORT			= 0x130,
	RDEC_TRANSFER_BITMAP_FAILED				= 0x140,
	RDEC_TRANSFER_BITMAP_RESPONSE_FAILED	= 0x150,
	RDEC_CLIENT_DISCONNECT					= 0x160,
} RD_ERROR_CODE;

typedef struct CommandData
{
	RD_CMD_CODE		code;
	std::string		data;
	unsigned int	length;
} CommandData;

#define BITMAP_SUFFIX_BMP ".bmp"
#define BITMAP_SUFFIX_JPEG ".jpg"

#define SCAN_MODIFY_RECT_UNIT		16	// ÿ32������ɨ��
#define CAPTURE_DESKTOP_FRAME		200