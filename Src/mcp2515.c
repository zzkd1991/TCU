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
/*CANCTRL:CAN CONTROL REGISTER(ADDRESS:XFh)*/
#define CANCTRL	      0x0f
/*Request Operaton Mode bits
 000 = Sets Normal Operation mode
 001 = Sets Sleep mode
 010 = Sets Loopback mode
 011 = Sets Listen-Only mode
 100 = Sets Configuration mode
*/
#  define CANCTRL_REQOP_MASK	    0xe0
#  define CANCTRL_REQOP_CONF	    0x80
#  define CANCTRL_REQOP_LISTEN_ONLY 0x60
#  define CANCTRL_REQOP_LOOPBACK    0x40
#  define CANCTRL_REQOP_SLEEP	    0x20
#  define CANCTRL_REQOP_NORMAL	    0x00
/*OSM:One-Shot Mode bit
 1 = Enable;message will only attempt to transmit one time
 0 = Disable;message will reattempt transmission if required
*/
#  define CANCTRL_OSM		    0x08
/*ABAT:Abort All Pending Transmissions bit*/
#  define CANCTRL_ABAT		    0x10
/*TEC:TRANSMIT ERROR COUNTER REGISGER(ADDRESS:1Ch)*/
#define TEC	      0x1c
/*REC:RECEIVE ERROR COUNTER REGISTER(ADDRESS:1Dh)*/
#define REC	      0x1d
/*CNF1:CONFIGURATION REGISTER 1(ADDRESS:2Ah)*/
#define CNF1	      0x2a
#  define CNF1_SJW_SHIFT   6
/*CNF2:CONFIGURATION REGISTER 2(ADDRESS:29h)*/
#define CNF2	      0x29
#  define CNF2_BTLMODE	   0x80
#  define CNF2_SAM         0x40
#  define CNF2_PS1_SHIFT   3
/*CNF3:CONFIGURATION REGISTER 3(ADDRESS:28h)*/
#define CNF3	      0x28
#  define CNF3_SOF	   0x08
#  define CNF3_WAKFIL	   0x04
#  define CNF3_PHSEG2_MASK 0x07
/*CANINTE:CAN INTERRUPT ENABLE REGISTER(ADDRESS: 2Bh)*/
#define CANINTE	      0x2b
/*MERRE:Message Error Interrupt Enable bit*/
#  define CANINTE_MERRE 0x80
/*WAKIE:Wake-up Interrupt Enable bit*/
#  define CANINTE_WAKIE 0x40
/*ERRIE:Error Interrupt Enable bit(multiple sources in EFLG regiter)*/
#  define CANINTE_ERRIE 0x20
/*TX2IE:Transmit Buffer 2 Empty Interrupt Enable bit*/
#  define CANINTE_TX2IE 0x10
/*TX11E: Transmit Buffer 1 Empty Interrupt Enable bit*/
#  define CANINTE_TX1IE 0x08
/*TX0IE:Transmit Buffer 0 Empty Interrupt Enable bit*/
#  define CANINTE_TX0IE 0x04
/*RX1IE:Receive Buffer 1 Full Interrupt Enable bit*/
#  define CANINTE_RX1IE 0x02
/*RX0IE:Receive Buffer 0 Full Interrupt Enable bit*/
#  define CANINTE_RX0IE 0x01
/*CANINTF:CAN INTERUPT FLAG REGISTER(ADDRESS:2Ch)*/
#define CANINTF	      0x2c
/*MERRF:Message Error Interrupt Flag bit*/
#  define CANINTF_MERRF 0x80
/*WAKIF:Wake-up Interrupt Flag bit*/
#  define CANINTF_WAKIF 0x40
/*ERRIF:Error Interrupt Flag bit(multiple sources in EFLG register)*/
#  define CANINTF_ERRIF 0x20
/*TX2IF:Transmit Buffer 2 Empty Interrupt Flag bit*/
#  define CANINTF_TX2IF 0x10
/*TX1IF:Transmit Buffer 1 Empty Interrupt Flag bit*/
#  define CANINTF_TX1IF 0x08
/*TX0IF:Transmit Buffer 0 Empty Interrupt Flag bit*/
#  define CANINTF_TX0IF 0x04
/*RX1IF:Receive Buffer 1 Full Interrupt Flag bit*/
#  define CANINTF_RX1IF 0x02
/*RX0IF:Receive Buffer 0 Full Interrupt Flag bit*/
#  define CANINTF_RX0IF 0x01
#  define CANINTF_RX (CANINTF_RX0IF | CANINTF_RX1IF)
#  define CANINTF_TX (CANINTF_TX2IF | CANINTF_TX1IF | CANINTF_TX0IF)
#  define CANINTF_ERR (CANINTF_ERRIF)
/*EFLG:ERROR FLAG REGISTER(ADDRESS:2Dh)*/
#define EFLG	      0x2d
/*EWARN:Error Warning Flag bit*/
#  define EFLG_EWARN	0x01
/*RXWAR:Receive Error Warning Flag bit*/
#  define EFLG_RXWAR	0x02
/*TXWAR:Transmit Error Warning Flag bit*/
#  define EFLG_TXWAR	0x04
/*RXEP:Receive Error-Passive Flag bit */
#  define EFLG_RXEP	0x08
/*TXEP:Transmit Error-Passive Flag bit*/
#  define EFLG_TXEP	0x10
/*TXBO:Bus-Off Error Flag bit*/
#  define EFLG_TXBO	0x20
/*RX0OVR:Receive Buffer 0 Overflow Flag bit*/
#  define EFLG_RX0OVR	0x40
/*RX1OVR:Receive Buffer 1 Overflow Flag bit*/
#  define EFLG_RX1OVR	0x80
/*TXBnCTRL: TRANSMIT BUFFER n CONTROL REGISTER (ADDRESS:30h,40h,50h)*/
#define TXBCTRL(n)  (((n) * 0x10) + 0x30 + TXBCTRL_OFF)
/*Message Aborted Flag bit*/
#  define TXBCTRL_ABTF	0x40
/*Message Lost Arbitration bit*/
#  define TXBCTRL_MLOA	0x20
/*Transmission Error Detected bit*/
#  define TXBCTRL_TXERR 0x10
/*Message Transmit Request bit*/
#  define TXBCTRL_TXREQ 0x08
/*TXBnSIDH: TRANSMIT BUFFER n STANDARD IDENTIFIER REGISTER HIGH(ADDRESS: 31h,41h,51h)*/
#define TXBSIDH(n)  (((n) * 0x10) + 0x30 + TXBSIDH_OFF)
#  define SIDH_SHIFT    3
/*TXBnSIDL: TRANSMIT BUFFER n STADNARD IDENTIFIER REGISTER LOW(ADDRESS: 32h,42h,52h)*/
#define TXBSIDL(n)  (((n) * 0x10) + 0x30 + TXBSIDL_OFF)
#  define SIDL_SID_MASK    7
#  define SIDL_SID_SHIFT   5
#  define SIDL_EXIDE_SHIFT 3
#  define SIDL_EID_SHIFT   16
#  define SIDL_EID_MASK    3
/*TXBnEID8:TRANSMIT BUFFER n EXTENDED IDENTIFIER 8 REGISTER HIGH(ADDRESS: 33h, 43h,53h)*/
#define TXBEID8(n)  (((n) * 0x10) + 0x30 + TXBEID8_OFF)
/*TXBnEID0:TRANSMIT BUFFER n EXTENDED IDENTIFIER 0 REGISTER LOW(ADDRESS: 34h,44h,54h)*/
#define TXBEID0(n)  (((n) * 0x10) + 0x30 + TXBEID0_OFF)
/*TXBnDLC:TRANSMIT BUFFER n DATA LENGTH CODE REGISTER(ADDRESS: 35h,45h,55h)*/
#define TXBDLC(n)   (((n) * 0x10) + 0x30 + TXBDLC_OFF)
#  define DLC_RTR_SHIFT    6
#define TXBCTRL_OFF 0
#define TXBSIDH_OFF 1
#define TXBSIDL_OFF 2
#define TXBEID8_OFF 3
#define TXBEID0_OFF 4
#define TXBDLC_OFF  5
#define TXBDAT_OFF  6
/*RXB0CTRL:RECEIVE BUFFER 0 CONTROL REGISTER(ADDRESS:60h)*/
/*RXB1CTRL:RECEIVE BUFFER 1 CONTROL REGISTER(ADDRESS:70h)*/
#define RXBCTRL(n)  (((n) * 0x10) + 0x60 + RXBCTRL_OFF)
/*BUCK:Rollover Enable bit*/
#  define RXBCTRL_BUKT	0x04
/*RXM[1:0]:Receive Buffer Operating mode bit*/
#  define RXBCTRL_RXM0	0x20
#  define RXBCTRL_RXM1	0x40
/*RXBnSIDH:RECEIVE BUFFER n STANDARD IDENTIFIER REGISTER HIGH(ADDRESS:61h, 71h)*/
#define RXBSIDH(n)  (((n) * 0x10) + 0x60 + RXBSIDH_OFF)
#  define RXBSIDH_SHIFT 3
/*RXBnSIDL:RECEIVE BUFFER n STANDARD IDENTIFIER REGISTER LOW(ADDRESS:62h, 72h)*/
#define RXBSIDL(n)  (((n) * 0x10) + 0x60 + RXBSIDL_OFF)
/*Extended Identifier Flag bit,this bit indicates whether the received message was a standard or an extended frame.*/
#  define RXBSIDL_IDE   0x08
/*SRR:Standard Frame Remote Transmit Request bit(valid only if IDE bit = 0)*/
#  define RXBSIDL_SRR   0x10
/*Extended Identifier bits, these bits contain the two most significant bits of the extended identifier for the received message*/
#  define RXBSIDL_EID   3
#  define RXBSIDL_SHIFT 5
/*RXBnEID8:RECEIVE BUFFER n EXTENDED IDENTIFIER REGISTER HIGH(ADDRESS:63h,73h)*/
#define RXBEID8(n)  (((n) * 0x10) + 0x60 + RXBEID8_OFF)
/*RXBnEID0:RECEIVED BUFFER n EXTENDED IDENTIFIER REGISTER LOW(ADDRESS:64h,74h)*/
#define RXBEID0(n)  (((n) * 0x10) + 0x60 + RXBEID0_OFF)
/*RXBnDLC:RECEIVE BUFFER n DATA LENGTH CODE REGISTER(ADDRESS:65h, 75h)*/
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
	/*Instruction + Address Byte + Data Byte*/
	uint8_t spi_tx_buf[3];

	spi_tx_buf[0] = INSTRUCTION_WRITE;
	spi_tx_buf[1] = reg;
	spi_tx_buf[2] = val;

	mcp2515_cs_low();

	spi3_trx(3, spi_tx_buf, NULL);

	mcp2515_cs_high();
}

void mdelay(uint32_t delay)
{
	extern __IO uint32_t uwTick;
	uint32_t current_tick = 0;

	current_tick = HAL_GetTick();
	
	while(uwTick < (current_tick + delay));
}

static uint8_t mcp2515_read_reg(uint8_t reg)
{
	/*Instruction + Address Byte*/
	uint8_t val = 0;

	uint8_t spi_tx_buf[2];
	uint8_t spi_rx_buf;
	spi_tx_buf[0] = INSTRUCTION_READ;
	spi_tx_buf[1] = reg;

	mcp2515_cs_low();

	spi3_trx(2, spi_tx_buf, NULL);

	spi3_trx(1, NULL, &spi_rx_buf);

	mcp2515_cs_high();
	
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

	mcp2515_cs_low();
	spi3_trx(4, spi_tx_buf, NULL);
	mcp2515_cs_high();
}

static void mcp2515_read_2regs(uint8_t reg, uint8_t* v1, uint8_t* v2)
{
/*
 the internal Address Pointer is automatically incremented to the next
 address once each byte of data is shifted out. Therefore, it is possible
 to read the next consecutive register address by continuing to provide
 clock pulses.
*/
	uint8_t spi_tx_buf[2];
	uint8_t spi_rx_buf[2];

	spi_tx_buf[0] = INSTRUCTION_READ;
	spi_tx_buf[1] = reg;

	mcp2515_cs_low();

	spi3_trx(2, spi_tx_buf, NULL);
	spi3_trx(2, NULL, spi_rx_buf);

	mcp2515_cs_high();
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
	uint32_t sid, eid, exide, rtr;
	uint8_t spi_tx_buf;

	spi_tx_buf = INSTRUCTION_RTS(1 << tx_buf_idx);

	exide = m->ide;/* Extended ID Enable */
	if(exide)
		sid = (m->cob_id & CAN_EFF_MASK) >> 18;
	else
		sid = m->cob_id & CAN_SFF_MASK;/* Standard ID */

	eid = m->cob_id & CAN_SFF_MASK;/* Extended ID */
	rtr = m->rtr;

	buf[TXBCTRL_OFF] = INSTRUCTION_LOAD_TXB(tx_buf_idx);
	buf[TXBSIDH_OFF] = sid >> SIDH_SHIFT;/*帧ID高8位*/
	buf[TXBSIDL_OFF] = ((sid & SIDL_SID_MASK) << SIDL_SID_SHIFT);/*帧ID低8位*/
	buf[TXBSIDL_OFF] = ((sid & SIDL_SID_MASK) << SIDL_SID_SHIFT) |
		(exide << SIDL_EXIDE_SHIFT) |
		((eid >> SIDL_EID_SHIFT) & SIDL_EID_MASK);
	buf[TXBEID8_OFF] = GET_BYTE(eid, 1);
	buf[TXBEID0_OFF] = GET_BYTE(eid, 0);
	buf[TXBDLC_OFF] = (rtr << DLC_RTR_SHIFT) | m->len;/*帧长度+是否为远程帧*/
	memcpy(buf + TXBDAT_OFF, m->data, m->len);
	mcp2515_hw_tx_frame(buf, m->len + TXBDAT_OFF, tx_buf_idx);

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
		m.ide = 1;
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

	if(0 == InsertCan1QueueRx(m))
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
	uint32_t timeout;

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

		timeout = HAL_GetTick();
		/* Wait for the device to enter normal mode */
		while(mcp2515_read_reg(CANSTAT) & CANCTRL_REQOP_MASK)
		{
			if(timeout + 100 < HAL_GetTick())
				return -1;
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
	mcp2515_do_set_bittiming(bt, ctrlmode);//set baud rate

	mcp2515_write_reg(RXBCTRL(0), RXBCTRL_BUKT | RXBCTRL_RXM0 | RXBCTRL_RXM1);//Turns mask/filter off;receive any message;Rollover Enable bit

	mcp2515_write_reg(RXBCTRL(1), RXBCTRL_RXM0 | RXBCTRL_RXM1);//Turns mask/fitler off;recive any message

	return 0;
}

static int mcp2515_hw_reset(void)
{
	int ret;
	uint32_t timeout;
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
	timeout = HAL_GetTick();
	while((mcp2515_read_reg(CANSTAT) & CANCTRL_REQOP_MASK) !=
			CANCTRL_REQOP_CONF)
	{
		if(timeout + 100 < HAL_GetTick())
			return -1;
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
	
	mcp2515_hw_probe();

	ret = mcp2515_setup(&bt, ctrlmode);
	if(ret != 0)
		return ret;
	
	ret = mcp2515_set_normal_mode(ctrlmode);//set normal mode
	if(ret != 0)
	{
		Error_Handler();
	}

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
	//int force_quit = 0;

	//while(!force_quit)
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
			//force_quit = 1;
			mcp2515_hw_sleep();
			//break;
		}


		if(intf & CANINTF_TX)
		{
			freelevel = 1;
		}
		//if(intf == 0)
		//	break;
	}

	return 0;
}


