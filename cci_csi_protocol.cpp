// ref: CCI/CSI Protocol Interface standard for beveridge dispensers and payment systems
// ver: Version 3.5
// Author: Mario.Ge (mylover433@hotmail.com)

#include "cci_csi_protocol.h"

const char *cmd_description[] = {
	"Billing device lock / release",
	"Polling the Slave",
	"Balance and price inquiry",
	"load price of a price list into the slave",
	"Sell product",
	"Interfacetype and version",
	"Set mode (free vend, test, block)",
	"Read and save setting in the interface",
	"Debit amount",
	"Feature expansion",
	"Commands used privately by manufacturers",
};

/*
 ====== 3.5.3 Command Overview ======
 +----------------+---------+--------+---------------+--------------+---------------------------------------------+
 |      Name      |  Level  |  Type  |  master data  |  slave data  |  Function                                   |
 +----------------+---------+--------+---------------+--------------+---------------------------------------------+
 | Vend           |    1    |    V   |       1       |       -      | Billing device lock / release               |
 | Status         |    1    |    S   |       0       |       4      | Polling the Slave                           |
 | Credit         |    1    |    C   |       4       |       7      | Balance and price inquiry                   |
 | Price          |    1    |    P   |      10       |       -      | load price of a price list into the slave   |
 | Inquiry        |    1    |    I   |       4       |       1      | Sell product                                |
 | Identification |    1    |    X   |       -       |      6/8     | Interfacetype and version                   |
 | Machine Mode   |    2    |    M   |       3       |       2      | Set mode (free vend, test, block)           |
 | Parameter      |    3    |    E   |       9       |       1      | Read and save setting in the interface      |
 | Amount         |    3    |    B   |      13       |       1      | Debit amount                                |
 | reserved       |    -    |    F   |       -       |       -      | Feature expansion                           |
 | reserved       |    -    |    a   |       -       |       -      | Commands used privately by manufacturers    |
 | ACK            |    1    |    -   |       -       |       -      | Command understood                          |
 | NAK            |    1    |    -   |       -       |       -      | Command not understood                      |
 | <silence>      |    1    |    -   |       -       |       -      | No answer, because not ready                |
 +----------------+---------+--------+---------------+--------------+---------------------------------------------+
*/
const command_item_t command_list[] = {
  /*  [Name]          [Level] [type] [master_data] [slave_data] [callback] [description] */
    { "Vend",            1,    'V',       1,            -1,     &vend_ack,            cmd_description[0]  },
    { "Status",          1,    'S',       0,             4,     &status_ack,          cmd_description[1]  },
    { "Credit",          1,    'C',       4,             7,     &credit_ack,          cmd_description[2]  },
    { "Price",           1,    'P',      10,            -1,     &price_ack,           cmd_description[3]  },
    { "Inquiry",         1,    'I',       4,             1,     &inquiry_ack,         cmd_description[4]  },
    { "Identification1", 1,    'X',      -1,             6,     &identification1_ack, cmd_description[5]  },
    { "Identification2", 1,    'X',      -1,             8,     &identification2_ack, cmd_description[5]  },
    { "Machine Mode",    2,    'M',       3,             2,     &machine_mode_ack,    cmd_description[6]  },
    { "Parameter",       3,    'E',       9,             1,     &parameter_ack,       cmd_description[7]  },
    { "Amount",          3,    'B',      13,             1,     &amount_ack,          cmd_description[8]  },
    { "Reserved1",      -1,    'F',      -1,            -1,     &reserved1_ack,       cmd_description[9]  },
    { "Reserved2",      -1,    'a',      -1,            -1,     &reserved2_ack,       cmd_description[10] }
};

#define TELEGRAM_VEND    (&command_list[0])
#define TELEGRAM_STATUS  (&command_list[1])
#define TELEGRAM_CREDIT  (&command_list[2])
#define TELEGRAM_PRICE   (&command_list[3])
#define TELEGRAM_INQUIRY (&command_list[4])
#define TELEGRAM_ID1     (&command_list[5])
#define TELEGRAM_ID2     (&command_list[6])
#define TELEGRAM_MODE    (&command_list[7])
#define TELEGRAM_PARA    (&command_list[8])
#define TELEGRAM_AMOUNT  (&command_list[9])
#define TELEGRAM_RSV1    (&command_list[10])
#define TELEGRAM_RSV2    (&command_list[11])

cci_csi_protocol::cci_csi_protocol()
{
}

cci_csi_protocol::~cci_csi_protocol()
{
}

char cci_csi_protocol::validation(uint8_t *ppacket)
{
	uint8_t data_len = -1;
	char ret         = 0xFF;
	uint16_t bcc     = 0x0000;

	assert(NULL != ppacket);
	
	do {
		if(TELEGRAM_STX != ppacket[0])  break;

        for(i = 0; i < TELEGRAM_COMMANDS_NUM; i++) {
			if(command_list[i].type != ppacket[1]) {
				continue;
			}
			data_len = command_list[i].slave_data_len;
			bcc      = *(uint16_t *)&ppacket[data_len + 3];
			if(TELEGRAM_ETX != ppacket[data_len + 2]) {
				continue;
			}
			if(TELEGRAM_ETB != ppacket[data_len + 5]) {
				continue;
			}
			if(bcc != bcc_calc(&ppacket[1], data_len + 2)) {
				break;
			}
			ret = command_list[i].rpl_handle(ppacket);
			break;
		}
	} while(0);

	return ret;
}

uint8_t cci_csi_protocol::recv_telegram(void)
{
	char type;

    assert(tty_fd >= 0);

    if(0xFF == validation(&rx_buf)) {
		// packet format error
	}

}

uint8_t cci_csi_protocol::send_telegram_vend(operating_mode_t mode)
{
    TELEGRAM telegram(TELEGRAM_VEND);

    //telegram.set_data(&mode);
}

uint8_t cci_csi_protocol::send_telegram_status(void)
{
    TELEGRAM telegram(TELEGRAM_STATUS);

    telegram.set_data(NULL);
}

