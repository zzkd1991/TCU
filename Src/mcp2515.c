#include "mcp2515.h"
#include "bsp_can.h"
#include "spi.h"
#include "can_queue.h"
#include <string.h>

/* SPI interface instruction set */
#define INSTRUCTION_WRITE	0x02
#define INSTRUCTION_READ	0x03
#define INSTRUCTION_BIT_MODIFY	0x05
#define INSTRUCTION_LOAD_TXB(n)	(0x40 + 2 * (n))
#define INSTRUCTION_READ_RXB(n)	(((n) == 0) ? 0x90 : 0x94)
#define INSTRUCTION_RESET	0xC0
#define RTS_TXB0		0x01
#define RTS_TXB1		0x02
#define RTS_TXB2		0x04
#define INSTRUCTION_RTS(n)	(0x80 | ((n) & 0x07))

/* MPC251x registers */
#define CANSTAT	      0x0e
#define CANCTRL	      0x0f
#  define CANCTRL_REQOP_MASK	    0xe0
#  define CANCTRL_REQOP_CONF	    0x80
#  define CANCTRL_REQOP_LISTEN_ONLY 0x60
#  define CANCTRL_REQOP_LOOPBACK    0x40
#  define CANCTRL_REQOP_SLEEP	    0x20
#  define CANCTRL_REQOP_NORMAL	    0x00
#  define CANCTRL_OSM		    0x08
#  define CANCTRL_ABAT		    0x10
#define TEC	      0x1c
#define REC	      0x1d
#define CNF1	      0x2a
#  define CNF1_SJW_SHIFT   6
#define CNF2	      0x29
#  define CNF2_BTLMODE	   0x80
#  define CNF2_SAM         0x40
#  define CNF2_PS1_SHIFT   3
#define CNF3	      0x28
#  define CNF3_SOF	   0x08
#  define CNF3_WAKFIL	   0x04
#  define CNF3_PHSEG2_MASK 0x07
#define CANINTE	      0x2b
#  define CANINTE_MERRE 0x80
#  define CANINTE_WAKIE 0x40
#  define CANINTE_ERRIE 0x20
#  define CANINTE_TX2IE 0x10
#  define CANINTE_TX1IE 0x08
#  define CANINTE_TX0IE 0x04
#  define CANINTE_RX1IE 0x02
#  define CANINTE_RX0IE 0x01
#define CANINTF	      0x2c
#  define CANINTF_MERRF 0x80
#  define CANINTF_WAKIF 0x40
#  define CANINTF_ERRIF 0x20
#  define CANINTF_TX2IF 0x10
#  define CANINTF_TX1IF 0x08
#  define CANINTF_TX0IF 0x04
#  define CANINTF_RX1IF 0x02
#  define CANINTF_RX0IF 0x01
#  define CANINTF_RX (CANINTF_RX0IF | CANINTF_RX1IF)
#  define CANINTF_TX (CANINTF_TX2IF | CANINTF_TX1IF | CANINTF_TX0IF)
#  define CANINTF_ERR (CANINTF_ERRIF)
#define EFLG	      0x2d
#  define EFLG_EWARN	0x01
#  define EFLG_RXWAR	0x02
#  define EFLG_TXWAR	0x04
#  define EFLG_RXEP	0x08
#  define EFLG_TXEP	0x10
#  define EFLG_TXBO	0x20
#  define EFLG_RX0OVR	0x40
#  define EFLG_RX1OVR	0x80
#define TXBCTRL(n)  (((n) * 0x10) + 0x30 + TXBCTRL_OFF)
#  define TXBCTRL_ABTF	0x40
#  define TXBCTRL_MLOA	0x20
#  define TXBCTRL_TXERR 0x10
#  define TXBCTRL_TXREQ 0x08
#define TXBSIDH(n)  (((n) * 0x10) + 0x30 + TXBSIDH_OFF)
#  define SIDH_SHIFT    3
#define TXBSIDL(n)  (((n) * 0x10) + 0x30 + TXBSIDL_OFF)
#  define SIDL_SID_MASK    7
#  define SIDL_SID_SHIFT   5
#  define SIDL_EXIDE_SHIFT 3
#  define SIDL_EID_SHIFT   16
#  define SIDL_EID_MASK    3
#define TXBEID8(n)  (((n) * 0x10) + 0x30 + TXBEID8_OFF)
#define TXBEID0(n)  (((n) * 0x10) + 0x30 + TXBEID0_OFF)
#define TXBDLC(n)   (((n) * 0x10) + 0x30 + TXBDLC_OFF)
#  define DLC_RTR_SHIFT    6
#define TXBCTRL_OFF 0
#define TXBSIDH_OFF 1
#define TXBSIDL_OFF 2
#define TXBEID8_OFF 3
#define TXBEID0_OFF 4
#define TXBDLC_OFF  5
#define TXBDAT_OFF  6
#define RXBCTRL(n)  (((n) * 0x10) + 0x60 + RXBCTRL_OFF)
#  define RXBCTRL_BUKT	0x04
#  define RXBCTRL_RXM0	0x20
#  define RXBCTRL_RXM1	0x40
#define RXBSIDH(n)  (((n) * 0x10) + 0x60 + RXBSIDH_OFF)
#  define RXBSIDH_SHIFT 3
#define RXBSIDL(n)  (((n) * 0x10) + 0x60 + RXBSIDL_OFF)
#  define RXBSIDL_IDE   0x08
#  define RXBSIDL_SRR   0x10
#  define RXBSIDL_EID   3
#  define RXBSIDL_SHIFT 5
#define RXBEID8(n)  (((n) * 0x10) + 0x60 + RXBEID8_OFF)
#define RXBEID0(n)  (((n) * 0x10) + 0x60 + RXBEID0_OFF)
#define RXBDLC(n)   (((n) * 0x10) + 0x60 + RXBDLC_OFF)
#  define RXBDLC_LEN_MASK  0x0f
#  define RXBDLC_RTR       0x40
#define RXBCTRL_OFF 0
#define RXBSIDH_OFF 1
#define RXBSIDL_OFF 2
#define RXBEID8_OFF 3
#define RXBEID0_OFF 4
#define RXBDLC_OFF  5
#define RXBDAT_OFF  6
#define RXFSID(n) ((n < 3) ? 0 : 4)
#define RXFSIDH(n) ((n) * 4 + RXFSID(n))
#define RXFSIDL(n) ((n) * 4 + 1 + RXFSID(n))
#define RXFEID8(n) ((n) * 4 + 2 + RXFSID(n))
#define RXFEID0(n) ((n) * 4 + 3 + RXFSID(n))
#define RXMSIDH(n) ((n) * 4 + 0x20)
#define RXMSIDL(n) ((n) * 4 + 0x21)
#define RXMEID8(n) ((n) * 4 + 0x22)
#define RXMEID0(n) ((n) * 4 + 0x23)

#define GET_BYTE(val, byte)			\
	(((val) >> ((byte) * 8)) & 0xff)
#define SET_BYTE(val, byte)			\
	(((val) & 0xff) << ((byte) * 8))

/* Buffer size required for the largest SPI transfer (i.e., reading a frame) */
#define CAN_FRAME_MAX_DATA_LEN	8
#define SPI_TRANSFER_BUF_LEN		(6 + CAN_FRAME_MAX_DATA_LEN)
#define CAN_FRAME_MAX_BITS		128

#define TX_ECHO_SKB_MAX	1

#define MCP2515_OST_DELAY_MS	(5)

uint32_t freelevel = 1;

/* CAN payload length and DLC definitions according to ISO 11898-1 */
#define CAN_MAX_DLC 8
#define CAN_MAX_DLEN	8

enum can_state state;

extern void Error_Handler(void);

extern uint8_t spi3_trx(uint16_t len, uint8_t *w, uint8_t *r);


void mcp2515_gpio_config(void)
{
#if 0
	GPIO_InitTypeDef GPIO_InitStructure;

	TX0RTS_INT_GPIO_CLK_ENABLE();
	TX1RTS_INT_GPIO_CLK_ENABLE();
	TX2RTS_INT_GPIO_CLK_ENABLE();

	RX0BF_INT_GPIO_CLK_ENABLE();
	RX1BF_INT_GPIO_CLK_ENABLE();

	MCP2515_RESET_GPIO_CLK_ENABLE();
	MCP2515_CS_GPIO_CLK_ENABLE();	

	GPIO_InitStructure.Pin = TX0RTS_INT_GPIO_PIN | TX1RTS_INT_GPIO_PIN | TX2RTS_INT_GPIO_PIN | RX0BF_INT_GPIO_PIN | RX1BF_INT_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(TX0RTS_INT_GPIO_PORT, &GPIO_InitStructure);
	HAL_GPIO_Init(TX1RTS_INT_GPIO_PORT, &GPIO_InitStructure);
	HAL_GPIO_Init(TX1RTS_INT_GPIO_PORT, &GPIO_InitStructure);
	HAL_GPIO_Init(RX0BF_INT_GPIO_PORT, &GPIO_InitStructure);
	HAL_GPIO_Init(RX1BF_INT_GPIO_PORT, &GPIO_InitStructure);



	GPIO_InitStructure.Pin = MCP2515_CS_GPIO_PIN | MCP2515_RESET_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(MCP2515_CS_GPIO_PORT, &GPIO_InitStructure);
	HAL_GPIO_Init(MCP2515_RESET_GPIO_PORT, &GPIO_InitStructure);

	HAL_NVIC_SetPriority(TX0RTS_INT_EXTI_IRQ, 0, 0);
	HAL_NVIC_SetPriority(TX1RTS_INT_EXTI_IRQ, 0, 0);
	HAL_NVIC_SetPriority(TX2RTS_INT_EXTI_IRQ, 0, 0);
	HAL_NVIC_SetPriority(RX0BF_INT_EXTI_IRQ, 0, 0);
	HAL_NVIC_SetPriority(RX1BF_INT_EXTI_IRQ, 0, 0);

	HAL_NVIC_EnableIRQ(TX0RTS_INT_EXTI_IRQ);
	HAL_NVIC_EnableIRQ(TX1RTS_INT_EXTI_IRQ);
	HAL_NVIC_EnableIRQ(TX2RTS_INT_EXTI_IRQ);
	HAL_NVIC_EnableIRQ(RX0BF_INT_EXTI_IRQ);
	HAL_NVIC_EnableIRQ(RX1BF_INT_EXTI_IRQ);
#else
	GPIO_InitTypeDef GPIO_InitStructure;
	MCP2515_CS_GPIO_CLK_ENABLE();
	MCP2515_INT_GPIO_CLK_ENABLE();

	GPIO_InitStructure.Pin = MCP2515_INT_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(MCP2515_INT_GPIO_PORT, &GPIO_InitStructure);

	HAL_NVIC_SetPriority(MCP2515_INT_EXTI_IRQ, 0, 0);
	HAL_NVIC_EnableIRQ(MCP2515_INT_EXTI_IRQ);
	
	GPIO_InitStructure.Pin = MCP2515_CS_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(MCP2515_CS_GPIO_PORT, &GPIO_InitStructure);	

#endif
}


static void mcp2515_write_reg(uint8_t reg, uint8_t val)
{
	uint8_t spi_tx_buf[3];

	spi_tx_buf[0] = INSTRUCTION_WRITE;
	spi_tx_buf[1] = reg;
	spi_tx_buf[2] = val;

	spi3_trx(3, spi_tx_buf, NULL);
}

void mdelay(uint32_t delay)
{
	extern __IO uint32_t uwTick;

	while(uwTick < delay)
	{

	}
}

static uint8_t mcp2515_read_reg(uint8_t reg)
{
	uint8_t val = 0;

	uint8_t spi_tx_buf[2];
	uint8_t spi_rx_buf;
	spi_tx_buf[0] = INSTRUCTION_READ;
	spi_tx_buf[1] = reg;

	spi3_trx(2, spi_tx_buf, NULL);

	spi3_trx(1, NULL, &spi_rx_buf);

	val = spi_rx_buf;

	return val;
}

static void mcp2515_write_bits(uint8_t reg, uint8_t mask, uint8_t val)
{
	uint8_t spi_tx_buf[4];

	spi_tx_buf[0] = INSTRUCTION_BIT_MODIFY;
	spi_tx_buf[1] = reg;
	spi_tx_buf[2] = mask;
	spi_tx_buf[3] = val;

	spi3_trx(4, spi_tx_buf, NULL);

}

static void mcp2515_read_2regs(uint8_t reg, uint8_t* v1, uint8_t* v2)
{
	uint8_t spi_tx_buf[2];
	uint8_t spi_rx_buf[2];

	spi_tx_buf[0] = INSTRUCTION_READ;
	spi_tx_buf[1] = reg;

	spi3_trx(2, spi_tx_buf, NULL);
	spi3_trx(2, NULL, spi_rx_buf);

	*v1 = spi_rx_buf[0];
	*v2 = spi_rx_buf[1];
}



static void mcp2515_hw_tx_frame(uint8_t *buf, int len, int tx_buf_idx)
{
	spi3_trx(TXBDAT_OFF + len, buf, NULL);
}

static void mcp2515_hw_tx(Message* m, int tx_buf_idx)
{
	
	uint8_t buf[SPI_TRANSFER_BUF_LEN] = {0};
	uint32_t sid, rtr;
	uint8_t spi_tx_buf;

	sid = m->cob_id;
	rtr = m->rtr;
	spi_tx_buf = INSTRUCTION_RTS(1 << tx_buf_idx);

	buf[TXBCTRL_OFF] = INSTRUCTION_LOAD_TXB(tx_buf_idx);
	buf[TXBSIDH_OFF] = sid >> SIDH_SHIFT;
	buf[TXBSIDL_OFF] = ((sid & SIDL_SID_MASK) << SIDL_SID_SHIFT);
	buf[TXBDLC_OFF] = (rtr << DLC_RTR_SHIFT) | m->len;
	memcpy(buf + TXBDAT_OFF, m->data, m->len);
	mcp2515_hw_tx_frame(buf, m->len, tx_buf_idx);

	/*use INSTRUCTION_RST, to avoid "repeated frame problem" */
	spi3_trx(1, &spi_tx_buf, NULL);
}

void mcp2515_send(Message *m)
{
	//while(freelevel == 0);

	//mcp2515_hw_tx(m, 0);

	//freelevel = 0;
	CanInsert_SendQueue(NULL, m);
}


uint8_t MCP2515_CanGet_SendQueue(void)
{
	extern struct _CANQueue CANQueueTx;
	uint16_t head;
	//uint8_t result;
	Message TxMessage = {0};
	head = CANQueueTx.front;

	if(1 == GetCanQueueTx(head, &TxMessage))
	{
		head = (head + 1) % MAX_CAN_SIZE;
		SetHeadCanQueueTx(head);

		while(freelevel == 0);
		mcp2515_hw_tx(&TxMessage, 0);
		freelevel = 0;
		
		return 0;
	}
	else
	{
		return 1;
	}
}


static void mcp2515_hw_rx_frame(uint8_t *buf, int buf_idx)
{
	uint8_t send_buf[SPI_TRANSFER_BUF_LEN] = {0};
	uint8_t recv_buf[SPI_TRANSFER_BUF_LEN] = {0};

	send_buf[RXBCTRL_OFF] = INSTRUCTION_READ_RXB(buf_idx);
	//spi_trx(SPI_TRANSFER_BUF_LEN, send_buf, recv_buf);
	spi3_trx(SPI_TRANSFER_BUF_LEN, send_buf, NULL);
	spi3_trx(SPI_TRANSFER_BUF_LEN, NULL, recv_buf);
	memcpy(buf, recv_buf, SPI_TRANSFER_BUF_LEN);
}

static void mcp2515_hw_rx(int buf_idx)
{
	Message m;

	uint8_t buf[SPI_TRANSFER_BUF_LEN];

	mcp2515_hw_rx_frame(buf, buf_idx);
	if(buf[RXBSIDL_OFF] & RXBSIDL_IDE)
	{
		/* Extended ID format */
		//m.cob_id = CAN_EFE_FLAG;
		/* Extended ID part*/
		m.cob_id |= 
		/* Extended ID part */
		SET_BYTE(buf[RXBSIDL_OFF] & RXBSIDL_EID, 2) |
		SET_BYTE(buf[RXBEID8_OFF], 1) |
		SET_BYTE(buf[RXBEID0_OFF], 0) |
		/* Standard ID part */
		(((buf[RXBSIDH_OFF] << RXBSIDH_SHIFT) |
		   (buf[RXBSIDL_OFF] >> RXBSIDL_SHIFT)) << 18);
		/* Remote transmission request */
		if(buf[RXBDLC_OFF] & RXBDLC_RTR)
			m.rtr = 1;
	}
	else
	{
		/* Standard ID format */
		m.cob_id = (buf[RXBSIDH_OFF] << RXBSIDH_SHIFT) |
					(buf[RXBSIDL_OFF] >> RXBSIDL_SHIFT);
		if(buf[RXBSIDL_OFF] & RXBSIDL_SRR)
			m.rtr = 1;
	}
	/* Data length */
	if((buf[RXBDLC_OFF] & RXBDLC_LEN_MASK) < CAN_MAX_DLC)
		m.len = buf[RXBDLC_OFF] & RXBDLC_LEN_MASK;
	else
		m.len = CAN_MAX_DLC;

	memcpy(m.data, buf + RXBDAT_OFF, m.len);

	if(0 == InsertCanQueueRx(m))
	{
		Error_Handler();
	}
}

static void mcp2515_hw_sleep(void)
{
	mcp2515_write_reg(CANCTRL, CANCTRL_REQOP_SLEEP);
}

static int mcp2515_set_normal_mode(uint32_t ctrlmode)
{
	//unsigned long timeout;

	/* Enable interrupts */
	mcp2515_write_reg(CANINTE, CANINTE_ERRIE | CANINTE_TX2IE | CANINTE_TX1IE |
						CANINTE_ERRIE | CANINTE_RX1IE | CANINTE_RX0IE);

	if(ctrlmode & CAN_CTRLMODE_LOOPBACK)
	{
		/* Put device into loopback mode */
		mcp2515_write_reg(CANCTRL, CANCTRL_REQOP_LOOPBACK);
	}
	else if(ctrlmode & CAN_CTRLMODE_LISTENONLY)
	{
		/* Put device into listen-only mode */
		mcp2515_write_reg(CANCTRL, CANCTRL_REQOP_LISTEN_ONLY);
	}
	else
	{
		/* Put device into normal mode */
		mcp2515_write_reg(CANCTRL, CANCTRL_REQOP_NORMAL);

		/* Wait for the device to enter normal mode */
		while(mcp2515_read_reg(CANSTAT) & CANCTRL_REQOP_MASK)
		{
			
		}
	}
	return 0;
}


static int mcp2515_do_set_bittiming(struct can_bittiming *bt, uint32_t ctrlmode)
{
	mcp2515_write_reg(CNF1, ((bt->sjw - 1) << CNF1_SJW_SHIFT) | (bt->brp - 1));
	mcp2515_write_reg(CNF2, CNF2_BTLMODE | (ctrlmode & CAN_CTRLMODE_3_SAMPLES ? CNF2_SAM : 0) |
						((bt->phase_seg1 - 1) << CNF2_PS1_SHIFT) |
						(bt->prop_seg - 1));
	mcp2515_write_bits(CNF3, CNF3_PHSEG2_MASK, (bt->phase_seg2 - 1));

	return 0;
}

static int mcp2515_setup(struct can_bittiming *bt, uint32_t ctrlmode)
{
	mcp2515_do_set_bittiming(bt, ctrlmode);

	mcp2515_write_reg(RXBCTRL(0), RXBCTRL_BUKT | RXBCTRL_RXM0 | RXBCTRL_RXM1);

	mcp2515_write_reg(RXBCTRL(1), RXBCTRL_RXM0 | RXBCTRL_RXM1);

	return 0;
}

static int mcp2515_hw_reset(void)
{
	int ret;
	uint8_t spi_tx_buf;

	/* Wait fo oscillator startup timer after power up */
	mdelay(MCP2515_OST_DELAY_MS);

	spi_tx_buf = INSTRUCTION_RESET;
	
	ret = spi3_trx(1, &spi_tx_buf, NULL);

	if(ret != 1)
		return ret;

	/* Wait for oscillator startup timer after reset */
	mdelay(MCP2515_OST_DELAY_MS);

	/* Wait for reset to finish */
	while((mcp2515_read_reg(CANSTAT) & CANCTRL_REQOP_MASK) !=
			CANCTRL_REQOP_CONF)
	{


	}

	return 0;
}

static int mcp2515_hw_probe(void)
{
	uint8_t ctrl;
	int ret;

	ret = mcp2515_hw_reset();
	if(ret)
		return ret;

	ctrl = mcp2515_read_reg(CANCTRL);

	/* Check for power up default value */
	if((ctrl & 0x17) != 0x07)
		return -1;

	return 0;
}

int mcp2515_hw_init(void)
{
	int ret;
	uint32_t ctrlmode = CAN_CTRLMODE_ONE_SHOT;
	struct can_bittiming bt = {0};

	/*
	 * 设置SJW = 0, BRP = 0x03。以8M晶振为例TQ = 2 * (3 + 1) / 8M = 1us
	 * PS1 = (2 + 1) * TQ = 3TQ;
	 * PS0 = (0 + 1) * TQ = TQ;
	 * PS2 = (2 + 1) * TQ = 3TQ;
	 * SJQ = 1 * TQ = TQ;
	 * Tbit = 1us * (1 + PS0 + PS1 + PS2) = 8us 波特率= 125K
	*/
	bt.sjw = 0;
	bt.brp = 0x03;
	bt.phase_seg1 = 2;
	bt.prop_seg = 0;
	bt.phase_seg2 = 2;
	
	mcp2515_gpio_config();

	mcp2515_cs_low();
	
	mcp2515_hw_probe();

	ret = mcp2515_setup(&bt, ctrlmode);
	if(ret != 0)
		return ret;
	
	ret = mcp2515_set_normal_mode(ctrlmode);

	return ret;
}

static int mcp2515_stop(void)
{
	/* Disable and clear pending interrupts */
	mcp2515_write_reg(CANINTE, 0x00);
	mcp2515_write_reg(CANINTF, 0x00);

	mcp2515_write_reg(TXBCTRL(0), 0);

	mcp2515_hw_sleep();

	return 0;
}

int mcp2515_can_ist(enum can_state state)
{
	int force_quit = 0;

	while(!force_quit)
	{
		enum can_state new_state;
		uint8_t intf, eflag;
		uint8_t clear_intf = 0;
		int can_id = 0, data1 = 0;

		mcp2515_read_2regs(CANINTF, &intf, &eflag);

		/* mask out flags we don't care about */
		intf &= CANINTF_RX | CANINTF_TX | CANINTF_ERR;

		/* receive buffer 0 */
		if(intf & CANINTF_RX0IF)
		{
			mcp2515_hw_rx(0);
		}

		/* receive buffer 1 */
		if(intf & CANINTF_RX1IF)
		{
			mcp2515_hw_rx(1);
		}

		/* any error or tx interrupt we need to clear? */
		if(intf & (CANINTF_ERR | CANINTF_TX))
			clear_intf |= intf & (CANINTF_ERR | CANINTF_TX);
		if(clear_intf)
			mcp2515_write_bits(CANINTF, clear_intf, 0x00);

		if(eflag & (EFLG_RX0OVR | EFLG_RX1OVR))
			mcp2515_write_bits(EFLG, eflag, 0x00);

		/* Update can state */
		if(eflag & EFLG_TXBO)
		{
			new_state = CAN_STATE_BUS_OFF;
			can_id |= CAN_ERR_BUSOFF;
		}
		else if(eflag & EFLG_TXEP)
		{
			new_state = CAN_STATE_ERROR_PASSIVE;
			can_id |= CAN_ERR_CRTL;
			data1 |= CAN_ERR_CRTL_TX_PASSIVE;
		}
		else if(eflag & EFLG_RXEP)
		{
			new_state = CAN_STATE_ERROR_PASSIVE;
			can_id |= CAN_ERR_CRTL;
			data1 |= CAN_ERR_CRTL_RX_PASSIVE;
		}
		else if(eflag & EFLG_TXWAR)
		{
			new_state = CAN_STATE_ERROR_WARNING;
			can_id |= CAN_ERR_CRTL;
			data1 |= CAN_ERR_CRTL_TX_WARNING;
		}
		else if(eflag & EFLG_RXWAR)
		{
			new_state = CAN_STATE_ERROR_WARNING;
			can_id |= CAN_ERR_CRTL;
			data1 |= CAN_ERR_CRTL_RX_WARNING;
		}
		else
		{
			new_state = CAN_STATE_ERROR_ACTIVE;
		}

		state = new_state;

		if(state == CAN_STATE_BUS_OFF)
		{
			force_quit = 1;
			mcp2515_hw_sleep();
			break;
		}

		if(intf == 0)
			break;

		if(intf & CANINTF_TX)
		{
			freelevel = 1;
		}
	}

	return 0;
}


