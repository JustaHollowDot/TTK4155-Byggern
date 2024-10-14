#include "can.h"

void can_init() {
    mcp_init();
}

void can_send(struct Message *message) {
    mcp_write(MCP_TXB0SIDH, message->id / 8);
    mcp_write(MCP_TXB0SIDL, (message->id % 8) << 5);

    mcp_write(MCP_TXB0DLC, message->length);

    for (int i = 0; i < message->length; ++i) {
        mcp_write(MCP_TXB0D0 + i, message->data[i]);
    }

    mcp_request_to_send(0);
}

struct Message can_receive() {
    struct Message message= {};

    // Id. RXBnSIDH og RXBnSIDL
    uint8_t id_low = mcp_read(MCP_RXB0SIDL)/0b100000;
    uint8_t id_high = mcp_read(MCP_RXB0SIDH);
    message.id = id_high * 0b1000 + id_low;

    // Lengde. RXBnDLC
    message.length = mcp_read(MCP_RXB0DLC);

    // Melding. RXBnDM
    for (int i = 0; i < message.length; i++) {
        message.data[i] = mcp_read(MCP_RXB0D0 + i);
    }

    return message;
}
