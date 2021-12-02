// ref: CCI/CSI Protocol Interface standard for beveridge dispensers and payment systems
// ver: Version 3.5
// Author: Mario.Ge (mylover433@hotmail.com)

#ifndef _CCI_CSI_PROTOCOL_H_
#define _CCI_CSI_PROTOOCL_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

typedef struct
{
    const char    *name;                    // 指令名称
    const uint8_t level;                    // 指令等级
    const char    type;                     // 指令类型
    const int8_t  master_data_len;          // 主端发送数据长度
    const int8_t  slave_data_len;           // 从端响应数据长度
    uint8_t       (*rpl_handle)(void *ptr); // 从端响应处理函数
    const char    *description;             // 指令功能描述
} command_item_t;

#define BUFFER_SIZE  (32)

static uint16_t bcc_calc(uint8_t *p, uint8_t len)
{
    uint8_t bcch, bccl;
    uint8_t i, bcc = 0x00;

    for(i = 0; i < len; i++) {
        bcc ^= p[i];
    }

    bcch = (bcc & 0xF0) >> 4;
    bccl = (bcc & 0x0F);
    bcch += ((bcch >= 0) && (bcch <= 9)) ? 0x30 : 0x37;
    bccl += ((bccl >= 0) && (bccl <= 9)) ? 0x30 : 0x37;

    return bccl * 256 + bcch;
}

/*
  ====== 3.4 Telegram structure ======
+----------------+----------+-----------------+----------------------+---------------+----------------------+
|   Start(STX)   |   Type   |   Data[0...n]   |   End of text(ETX)   |   Bcc Check   |   End of block(ETB)  |
+----------------+----------+-----------------+----------------------+---------------+----------------------+
|  1Byte(0x02)   |  1Byte   |      nByte      |      1Byte(0x03)     |     2Byte     |      1Byte(0x17)     |
+----------------+----------+-----------------+----------------------+---------------+----------------------+
*/

#define TELEGRAM_STX    (0x02)
#define TELEGRAM_ETX    (0x03)
#define TELEGRAM_ETB    (0x17)

// telegram structure defination.
struct TELEGRAM
{
    TELEGRAM(const command_item_t *cmd) {
        assert(cmd->master_data_len <= 13);   // maximum master data length is 13.
        memset(data, 0x00, sizeof(data));
        data_len           = cmd->master_data_len;
        data[0]            = TELEGRAM_STX;
        data[1]            = cmd->type;
        data[data_len + 2] = TELEGRAM_ETX;
        data[data_len + 5] = TELEGRAM_ETB;
    }
    void set_data(uint8_t *p)  {
        memcpy(data + 2, p, data_len);
        *(uint16_t *)&data[data_len + 3] = bcc_calc(&data[1], data_len + 2);
    }

    uint8_t length(void) {
        return data_len + 6;
    }

    uint8_t data[32];
    uint8_t data_len;
} __attribute__((packed));


typedef enum
{
    OperatingDisable = 0,
    OperatingEnable  = 1
} operating_mode_t;

typedef enum
{
    NO_ACTIION           = 0,
    READY                = 1,
    BUSY                 = 2,
    ERROR_PAYMENT_SYSTEM = 3,
    ERROR_INTERFACE      = 4
} status_t;

typedef struct
{
    uint8_t free : 1;
    uint8_t service : 1;
    uint8_t ab_geld : 1;
    uint8_t ken_reset : 1;
    uint8_t credit_hidden : 1;
    uint8_t : 2;
    uint8_t high : 1;
} IF_STAT;

class cci_csi_protocol
{
public:
    cci_csi_protocol();
    ~cci_csi_protocol();

//private:
    int tty_fd;
    uint8_t rx_buf[BUFFER_SIZE];

    uint8_t recv_telegram(void);

    uint8_t send_telegram_vend(operating_mode_t mode);
    uint8_t send_telegram_status(void);
    uint8_t send_telegram_credit(void);
    uint8_t send_telegram_price(void);
    uint8_t send_telegram_inquiry(void);
    uint8_t send_telegram_identification(void);
    uint8_t send_telegram_machine_mode(void);
    uint8_t send_telegram_parameter(void);
    uint8_t send_telegram_amount(void);
    uint8_t send_telegram_reserved1(void);
    uint8_t send_telegram_reserved2(void);
    uint8_t send_ack(void);
    uint8_t send_nak(void);
};

#endif // _CCI_CSI_PROTOCOL_H_
