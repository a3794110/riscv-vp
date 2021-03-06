#ifndef RISCV_VP_ABSTRACT_UART_H
#define RISCV_VP_ABSTRACT_UART_H

#include <stdint.h>
#include <semaphore.h>

#include <systemc>
#include <tlm_utils/simple_target_socket.h>

#include <thread>
#include <mutex>
#include <queue>

#include "core/common/irq_if.h"
#include "util/tlm_map.h"
#include "platform/common/async_event.h"

class AbstractUART : public sc_core::sc_module {
public:
	interrupt_gateway *plic;
	tlm_utils::simple_target_socket<AbstractUART> tsock;

	AbstractUART(sc_core::sc_module_name, uint32_t);
	~AbstractUART(void);

	SC_HAS_PROCESS(AbstractUART);

protected:
	void start_threads(void);
	void rxpush(uint8_t);

private:
	virtual void write_data(uint8_t) = 0;
	virtual void read_data(void) = 0;

	void register_access_callback(const vp::map::register_access_t &);
	void transport(tlm::tlm_generic_payload &, sc_core::sc_time &);
	void transmit(void);
	void receive(void);
	void interrupt(void);

	void swait(sem_t *);
	void spost(sem_t *);

	uint32_t irq;

	// memory mapped configuration registers
	uint32_t txdata = 0;
	uint32_t rxdata = 0;
	uint32_t txctrl = 0;
	uint32_t rxctrl = 0;
	uint32_t ie = 0;
	uint32_t ip = 0;
	uint32_t div = 0;

	std::thread rcvthr, txthr;
	std::mutex rcvmtx, txmtx;
	AsyncEvent asyncEvent;

	std::queue<uint8_t> tx_fifo;
	sem_t txfull;
	std::queue<uint8_t> rx_fifo;
	sem_t rxempty;

	vp::map::LocalRouter router = {"UART"};
};

#endif  // RISCV_VP_ABSTRACT_UART_H
