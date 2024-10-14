#ifndef BYGGERN_PING_PONG_KOPI_CANCONTROLLER_H
#define BYGGERN_PING_PONG_KOPI_CANCONTROLLER_H

#include "can_controller.h"

// Send-buffer 0
#define MCP_TXB0SIDH 0x31
#define MCP_TXB0SIDL 0x32
#define MCP_TXB0DLC 0x35
#define MCP_TXB0D0 0x36

//Send-buffer 1
#define MCP_TXB1SIDH 0x41
#define MCP_TXB1SIDL 0x42
#define MCP_TXB1DLC 0x45
#define MCP_TXB1D0 0x46

//Send-buffer 2
#define MCP_TXB2SIDH 0x51
#define MCP_TXB2SIDL 0x52
#define MCP_TXB2DLC 0x55
#define MCP_TXB2D0 0x56

// Motta-buffer 0
#define MCP_RXB0SIDH 0x61
#define MCP_RXB0SIDL 0x62
#define MCP_RXB0DLC 0x65
#define MCP_RXB0D0 0x66

// Motta-buffer 1
#define MCP_RXB1SIDH 0x71
#define MCP_RXB1SIDL 0x72
#define MCP_RXB1DLC 0x75
#define MCP_RXB1D0 0x76


struct Message {
    uint16_t id;
    uint8_t length;
    uint8_t data[8];
};


void can_init();
void can_send(struct Message *message);
struct Message can_receive();


#endif //BYGGERN_PING_PONG_KOPI_CANCONTROLLER_H