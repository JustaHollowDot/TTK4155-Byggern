#ifndef BYGGERN_PING_PONG_KOPI_CAN_CONTROLLER_H
#define BYGGERN_PING_PONG_KOPI_CAN_CONTROLLER_H

#include <stdint.h>

void can_controller_init();
uint8_t can_controller_read(uint8_t address);
void can_controller_write(uint8_t address, uint8_t data);
void can_controller_request_to_send(uint8_t buffer);
uint8_t can_controller_read_status();
void can_controller_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void can_controller_reset();

void can_controller_set_mode(uint8_t mode);

#endif //BYGGERN_PING_PONG_KOPI_CAN_CONTROLLER_H