#ifndef RISCV_VP_SLIP_H
#define RISCV_VP_SLIP_H

#include <stdint.h>
#include <systemc>
#include "abstract_uart.h"

class SLIP : public AbstractUART {
public:
	SLIP(const sc_core::sc_module_name &, uint32_t, std::string);
private:
	int get_mtu(const char *);
	void send_packet(void);
	void write_data(uint8_t);
	void read_data(void);

	int tunfd;

	uint8_t *sndbuf, *rcvbuf;
	size_t sndsiz, rcvsiz;

};

#endif  // RISCV_VP_SLIP_H
