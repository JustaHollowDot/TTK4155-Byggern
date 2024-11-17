#ifndef BYGGERN_PING_PONG_KOPI_CANCONTROLLER_H
#define BYGGERN_PING_PONG_KOPI_CANCONTROLLER_H

#include "./can_controller.h"
#include "../../../../shared/can_messages.h"

// Send-buffer 0
#define MCP_TXB0SIDH 0x31
#define MCP_TXB0SIDL 0x32
#define MCP_TXB0DLC 0x35
#define MCP_TXB0D0 0x36

// Motta-buffer 0
#define MCP_RXB0SIDH 0x61
#define MCP_RXB0SIDL 0x62
#define MCP_RXB0DLC 0x65
#define MCP_RXB0D0 0x66

void can_init();
void can_send(struct Message *message);
struct Message can_receive();


#endif //BYGGERN_PING_PONG_KOPI_CANCONTROLLER_H