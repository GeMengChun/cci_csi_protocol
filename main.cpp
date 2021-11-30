#include <stdio.h>
#include "cci_csi_protocol.h"

int main(int argc, char *argv[])
{
    cci_csi_protocol protocol;

    protocol.send_telegram_vend(OperatingEnable);
    protocol.send_telegram_status();

    return 0;
}
