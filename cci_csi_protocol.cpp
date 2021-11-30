// ref: CCI/CSI Protocol Interface standard for beveridge dispensers and payment systems
// ver: Version 3.5
// Author: Mario.Ge (mylover433@hotmail.com)

#include "cci_csi_protocol.h"

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
  /*  -Name-            -Level-  -type-  -master_data-  -slave_data-  -description- */
    { "Vend",            1,       'V',     1,            -1,           "Billing device lock / release"             },
    { "Status",          1,       'S',     0,             4,           "Polling the Slave"                         },
    { "Credit",          1,       'C',     4,             7,           "Blance and price inquiry"                  },
    { "Price",           1,       'P',    10,            -1,           "Load price of a price list into the slave" },
    { "Inquiry",         1,       'I',     4,             1,           "Sell product"                              },
    { "Identification1", 1,       'X',    -1,             6,           "Interfacetype and version"                 },
    { "Identification2", 1,       'X',    -1,             8,           "Interfacetype and verison"                 },
    { "Machine Mode",    2,       'M',     3,             2,           "Set mode (free vend, test, block)"         },
    { "Parameter",       3,       'E',     9,             1,           "Read and save setting in the interface"    },
    { "Amount",          3,       'B',    13,             1,           "Debit amount"                              },
    { "Reserved1",      -1,       'F',    -1,            -1,           "Feature expansion"                         },
    { "Reserved2",      -1,       'a',    -1,            -1,           "Command used privately by manufacturers"   }
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

uint8_t cci_csi_protocol::recv_telegram(void)
{
    assert(tty_fd >= 0);

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

