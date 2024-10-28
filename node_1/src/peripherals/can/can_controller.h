#ifndef BYGGERN_PING_PONG_KOPI_CAN_CONTROLLER_H
#define BYGGERN_PING_PONG_KOPI_CAN_CONTROLLER_H

#include <stdint.h>
#include "mcp2515.h"

#define MCP_TXB0SIDH 0x31 // aka 0b0011 0001
#define MCP_TXB1SIDH 0x41
#define MCP_TXB2SIDH 0x51
#define MCP_RXB0SIDH 0x61
#define MCP_RXB1SIDH 0x71

#define MCP_LOOPBACK_MODE 0b01000000

void mcp_init();
uint8_t mcp_read(uint8_t address);
void mcp_write(uint8_t address, uint8_t data);
void mcp_request_to_send(uint8_t buffer);
uint8_t mcp_read_status();
void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void mcp_reset();

void mcp_set_mode(uint8_t mode);

#endif //BYGGERN_PING_PONG_KOPI_CAN_CONTROLLER_H