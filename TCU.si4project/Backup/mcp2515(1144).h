#ifndef _CAN_PLATFORM_MCP2515_H
#define _CAN_PLATFORM_MCP2515_H

#include "stm32f4xx.h"
#include <stdint.h>

struct can_bittiming {
	uint32_t bitrate;			/* Bit-rate in bits/sencond */
	uint32_t sample_point;		/* Sample point in one-tenth of a percent */
	uint32_t tq;				/* Time quanta (TQ) in nanoseconds */
	uint32_t prop_seg;			/* Propagation segment in TQs */
	uint32_t phase_seg1;		/* Phase buffer segment 1 in TQs */
	uint32_t phase_seg2;		/* Phase buffer segment 2 in TQs */
	uint32_t sjw;				/* Synchronisation jump width in TQs */
	uint32_t brp;				/* Bit-rate prescaler */
};

#define CAN_CTRLMODE_LOOPBACK		0x01	/* Loopback mode */
#define CAN_CTRLMODE_LISTENONLY		0x02	/* Listen-only mode */
#define CAN_CTRLMODE_3_SAMPLES		0x04	/* Triple sampling mode */
#define CAN_CTRLMODE_ONE_SHOT		0x08	/* One-Shot mode */
#define CAN_CTRLMODE_BERR_REPORTING	0x10	/* Bus-error reporting */
#define CAN_CTRLMODE_FD			0x20	/* CAN FD mode */
#define CAN_CTRLMODE_PRESUME_ACK	0x40	/* Ignore missing CAN ACKs */
#define CAN_CTRLMODE_FD_NON_ISO		0x80	/* CAN FD in non-ISO mode */

/* CAN operational and error states */
enum can_state {
	CAN_STATE_ERROR_ACTIVE = 0,	/* RX/TX error count < 96 */
	CAN_STATE_ERROR_WARNING,	/* RX/TX error count < 128 */
	CAN_STATE_ERROR_PASSIVE,	/* RX/TX error count < 256*/
	CAN_STATE_BUS_OFF,			/* RX/TX error count >= 256 */
	CAN_STATE_STOPPED,			/* Device is stopped */
	CAN_STATE_SLEEPING,			/* Device is sleeping */
	CAN_STATE_MAX
};


#define CAN_ERR_DLC 8 /* dlc for error message frames */

/* error class (mask) in can_id */
#define CAN_ERR_TX_TIMEOUT   0x00000001U /* TX timeout (by netdevice driver) */
#define CAN_ERR_LOSTARB      0x00000002U /* lost arbitration    / data[0]    */
#define CAN_ERR_CRTL         0x00000004U /* controller problems / data[1]    */
#define CAN_ERR_PROT         0x00000008U /* protocol violations / data[2..3] */
#define CAN_ERR_TRX          0x00000010U /* transceiver status  / data[4]    */
#define CAN_ERR_ACK          0x00000020U /* received no ACK on transmission */
#define CAN_ERR_BUSOFF       0x00000040U /* bus off */
#define CAN_ERR_BUSERROR     0x00000080U /* bus error (may flood!) */
#define CAN_ERR_RESTARTED    0x00000100U /* controller restarted */

/* arbitration lost in bit ... / data[0] */
#define CAN_ERR_LOSTARB_UNSPEC   0x00 /* unspecified */
				      /* else bit number in bitstream */

/* error status of CAN-controller / data[1] */
#define CAN_ERR_CRTL_UNSPEC      0x00 /* unspecified */
#define CAN_ERR_CRTL_RX_OVERFLOW 0x01 /* RX buffer overflow */
#define CAN_ERR_CRTL_TX_OVERFLOW 0x02 /* TX buffer overflow */
#define CAN_ERR_CRTL_RX_WARNING  0x04 /* reached warning level for RX errors */
#define CAN_ERR_CRTL_TX_WARNING  0x08 /* reached warning level for TX errors */
#define CAN_ERR_CRTL_RX_PASSIVE  0x10 /* reached error passive status RX */
#define CAN_ERR_CRTL_TX_PASSIVE  0x20 /* reached error passive status TX */
				      /* (at least one error counter exceeds */
				      /* the protocol-defined level of 127)  */
#define CAN_ERR_CRTL_ACTIVE      0x40 /* recovered to error active state */

/* error in CAN protocol (type) / data[2] */
#define CAN_ERR_PROT_UNSPEC      0x00 /* unspecified */
#define CAN_ERR_PROT_BIT         0x01 /* single bit error */
#define CAN_ERR_PROT_FORM        0x02 /* frame format error */
#define CAN_ERR_PROT_STUFF       0x04 /* bit stuffing error */
#define CAN_ERR_PROT_BIT0        0x08 /* unable to send dominant bit */
#define CAN_ERR_PROT_BIT1        0x10 /* unable to send recessive bit */
#define CAN_ERR_PROT_OVERLOAD    0x20 /* bus overload */
#define CAN_ERR_PROT_ACTIVE      0x40 /* active error announcement */
#define CAN_ERR_PROT_TX          0x80 /* error occurred on transmission */

/* error in CAN protocol (location) / data[3] */
#define CAN_ERR_PROT_LOC_UNSPEC  0x00 /* unspecified */
#define CAN_ERR_PROT_LOC_SOF     0x03 /* start of frame */
#define CAN_ERR_PROT_LOC_ID28_21 0x02 /* ID bits 28 - 21 (SFF: 10 - 3) */
#define CAN_ERR_PROT_LOC_ID20_18 0x06 /* ID bits 20 - 18 (SFF: 2 - 0 )*/
#define CAN_ERR_PROT_LOC_SRTR    0x04 /* substitute RTR (SFF: RTR) */
#define CAN_ERR_PROT_LOC_IDE     0x05 /* identifier extension */
#define CAN_ERR_PROT_LOC_ID17_13 0x07 /* ID bits 17-13 */
#define CAN_ERR_PROT_LOC_ID12_05 0x0F /* ID bits 12-5 */
#define CAN_ERR_PROT_LOC_ID04_00 0x0E /* ID bits 4-0 */
#define CAN_ERR_PROT_LOC_RTR     0x0C /* RTR */
#define CAN_ERR_PROT_LOC_RES1    0x0D /* reserved bit 1 */
#define CAN_ERR_PROT_LOC_RES0    0x09 /* reserved bit 0 */
#define CAN_ERR_PROT_LOC_DLC     0x0B /* data length code */
#define CAN_ERR_PROT_LOC_DATA    0x0A /* data section */
#define CAN_ERR_PROT_LOC_CRC_SEQ 0x08 /* CRC sequence */
#define CAN_ERR_PROT_LOC_CRC_DEL 0x18 /* CRC delimiter */
#define CAN_ERR_PROT_LOC_ACK     0x19 /* ACK slot */
#define CAN_ERR_PROT_LOC_ACK_DEL 0x1B /* ACK delimiter */
#define CAN_ERR_PROT_LOC_EOF     0x1A /* end of frame */
#define CAN_ERR_PROT_LOC_INTERM  0x12 /* intermission */

/* error status of CAN-transceiver / data[4] */
/*                                             CANH CANL */
#define CAN_ERR_TRX_UNSPEC             0x00 /* 0000 0000 */
#define CAN_ERR_TRX_CANH_NO_WIRE       0x04 /* 0000 0100 */
#define CAN_ERR_TRX_CANH_SHORT_TO_BAT  0x05 /* 0000 0101 */
#define CAN_ERR_TRX_CANH_SHORT_TO_VCC  0x06 /* 0000 0110 */
#define CAN_ERR_TRX_CANH_SHORT_TO_GND  0x07 /* 0000 0111 */
#define CAN_ERR_TRX_CANL_NO_WIRE       0x40 /* 0100 0000 */
#define CAN_ERR_TRX_CANL_SHORT_TO_BAT  0x50 /* 0101 0000 */
#define CAN_ERR_TRX_CANL_SHORT_TO_VCC  0x60 /* 0110 0000 */
#define CAN_ERR_TRX_CANL_SHORT_TO_GND  0x70 /* 0111 0000 */
#define CAN_ERR_TRX_CANL_SHORT_TO_CANH 0x80 /* 1000 0000 */

/* controller specific additional information / data[5..7] */


int mcp2515_hw_init(void);
void mcp2515_gpio_config(void);
int mcp2515_can_ist(enum can_state state);



/* MCP2515 IO Configuration */
#define TX0RTS_INT_GPIO_PORT		GPIOA
#define TX0RTS_INT_GPIO_CLK_ENABLE()		__GPIOA_CLK_ENABLE();
#define TX0RTS_INT_GPIO_PIN				GPIO_PIN_0
#define TX0RTS_INT_EXTI_IRQ			EXTI0_IRQn
#define TX0RTS_IRQHandler			EXTI0_IRQHandler


#define TX1RTS_INT_GPIO_PORT		GPIOA
#define TX1RTS_INT_GPIO_CLK_ENABLE()		__GPIOA_CLK_ENABLE();
#define TX1RTS_INT_GPIO_PIN				GPIO_PIN_1
#define TX1RTS_INT_EXTI_IRQ			EXTI1_IRQn
#define TX1RTS_IRQHandler			EXTI1_IRQHandler

#define TX2RTS_INT_GPIO_PORT		GPIOA
#define TX2RTS_INT_GPIO_CLK_ENABLE()		__GPIOA_CLK_ENABLE();
#define TX2RTS_INT_GPIO_PIN				GPIO_PIN_2
#define TX2RTS_INT_EXTI_IRQ			EXTI2_IRQn
#define TX2RTS_IRQHandler			EXTI2_IRQHandler

#define RX0BF_INT_GPIO_PORT		GPIOA
#define RX0BF_INT_GPIO_CLK_ENABLE()		__GPIOA_CLK_ENABLE();
#define RX0BF_INT_GPIO_PIN				GPIO_PIN_3
#define RX0BF_INT_EXTI_IRQ			EXTI3_IRQn
#define RX0BF_IRQHandler			EXTI3_IRQHandler

#define RX1BF_INT_GPIO_PORT		GPIOA
#define RX1BF_INT_GPIO_CLK_ENABLE()		__GPIOA_CLK_ENABLE();
#define RX1BF_INT_GPIO_PIN				GPIO_PIN_4
#define RX1BF_INT_EXTI_IRQ			EXTI4_IRQn
#define RX1BF_IRQHandler			EXTI4_IRQHandler

#define MCP2515_CS_GPIO_PORT		GPIOA
#define MCP2515_CS_GPIO_CLK_ENABLE()			__GPIOA_CLK_ENABLE();
#define MCP2515_CS_GPIO_PIN			GPIO_PIN_5


#define MCP2515_RESET_GPIO_PORT		GPIOA
#define MCP2515_RESET_GPIO_CLK_ENABLE()		__GPIOA_CLK_ENABLE();
#define MCP2515_RESET_GPIO_PIN		GPIO_PIN_6
//输出低电平
#define mcp2515_digitalLo(p, i)		{p->BSRR = (uint32_t)i << 16;}
//输出高电平
#define mcp2515_digitalHi(p, i)		{p->BSRR = i;}

#define mcp2515_cs_low()			mcp2515_digitalLo(MCP2515_CS_GPIO_PORT,MCP2515_CS_GPIO_PIN)
#define mcp2515_cs_high()			mcp2515_digitalHi(MCP2515_CS_GPIO_PORT,MCP2515_CS_GPIO_PIN)


#endif /* _UAPI_CAN_ERROR_H */


