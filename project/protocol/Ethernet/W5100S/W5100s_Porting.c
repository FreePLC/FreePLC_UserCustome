

#include "W5100s_Porting.h"
#include "plc_driver.h"
#include "Utility.h"

/*******************************************************************************
* Definitions
******************************************************************************/
#define EXAMPLE_DSPI_MASTER_BASEADDR SPI0
#define DSPI_MASTER_CLK_SRC DSPI0_CLK_SRC
#define DSPI_MASTER_CLK_FREQ CLOCK_GetFreq(DSPI0_CLK_SRC)
#define EXAMPLE_DSPI_MASTER_PCS_FOR_INIT kDSPI_Pcs0
#define EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER kDSPI_MasterPcs0
#define EXAMPLE_DSPI_DEALY_COUNT 0xfffffU
#define TRANSFER_SIZE 64U         /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

/*******************************************************************************
* Prototypes
******************************************************************************/
extern void set_w5100s_mac(void);
extern void reg_wizchip_spi_cbfunc(uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb));
extern void reg_wizchip_cs_cbfunc(void(*cs_sel)(void), void(*cs_desel)(void));
extern void set_w5100s_netinfo(void);
extern void PHY_check(void);
extern int8_t wizchip_init(uint8_t* txsize, uint8_t* rxsize);

/*******************************************************************************
* Variables
******************************************************************************/
extern uint8_t  txsize[]; // tx buffer set K bytes
extern uint8_t rxsize[]; // rx buffet set K bytes

/*******************************************************************************
* Code
******************************************************************************/

void SPI_PinConfiguration()
{
  gpio_pin_config_t input_config = {
    kGPIO_DigitalInput, 0,
  };
  
  gpio_pin_config_t output_config = {
    kGPIO_DigitalOutput, 1,
  };
  
  port_pin_config_t pin_config;
  
  pin_config.pullSelect = kPORT_PullUp;
  pin_config.slewRate = kPORT_SlowSlewRate;
  pin_config.driveStrength = kPORT_LowDriveStrength;
  pin_config.lockRegister = kPORT_UnlockRegister;
  pin_config.openDrainEnable = kPORT_OpenDrainDisable;
  pin_config.mux = kPORT_MuxAsGpio;
  pin_config.passiveFilterEnable = kPORT_PassiveFilterDisable;
  
  PORT_SetPinConfig(W5100_RST_PORT, W5100_RST_PIN, &pin_config);
  GPIO_PinInit(W5100_RST_GPIO, W5100_RST_PIN, &output_config);
  
  PORT_SetPinConfig(W5100_CS_PORT, W5100_CS_PIN, &pin_config);
  GPIO_PinInit(W5100_CS_GPIO, W5100_CS_PIN, &output_config);
  
  PORT_SetPinConfig(W5100_IRQ_PORT, W5100_IRQ_PIN, &pin_config);
  GPIO_PinInit(W5100_IRQ_GPIO, W5100_IRQ_PIN, &input_config);
  
  pin_config.mux = kPORT_MuxAlt2;
  
  PORT_SetPinConfig(W5100_SCK_PORT, W5100_SCK_PIN, &pin_config);
  PORT_SetPinConfig(W5100_MISO_PORT, W5100_MISO_PIN, &pin_config);
  PORT_SetPinConfig(W5100_MOSI_PORT, W5100_MOSI_PIN, &pin_config);
  
}


void SPI_Configuration()
{
  dspi_master_config_t masterConfig;
  uint32_t srcClock_Hz;
  
  /* Master config */
  masterConfig.whichCtar = kDSPI_Ctar0;
  masterConfig.ctarConfig.baudRate = TRANSFER_BAUDRATE;
  masterConfig.ctarConfig.bitsPerFrame = 8U;
  masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
  masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
  masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
  masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE;
  masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE;
  masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE;
  
  masterConfig.whichPcs = EXAMPLE_DSPI_MASTER_PCS_FOR_INIT;
  masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;
  
  masterConfig.enableContinuousSCK = false;
  masterConfig.enableRxFifoOverWrite = false;
  masterConfig.enableModifiedTimingFormat = false;
  masterConfig.samplePoint = kDSPI_SckToSin0Clock;
  
  srcClock_Hz = DSPI_MASTER_CLK_FREQ;
  DSPI_MasterInit(EXAMPLE_DSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);
  
  
}

uint8_t spi_read_byte(void)
{
  dspi_transfer_t masterXfer;
  
  uint8_t data;
  
  /* Start master transfer, receive data from slave */
  masterXfer.txData = NULL;
  masterXfer.rxData = &data;
  masterXfer.dataSize = 1;
  masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;
  DSPI_MasterTransferBlocking(EXAMPLE_DSPI_MASTER_BASEADDR, &masterXfer);
  
  return data;
  
}

void spi_send_byte(uint8_t data)
{
  dspi_transfer_t masterXfer;
  
  /* Start master transfer, send data to slave */
  masterXfer.txData = &data;
  masterXfer.rxData = NULL;
  masterXfer.dataSize = 1;
  masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;
  DSPI_MasterTransferBlocking(EXAMPLE_DSPI_MASTER_BASEADDR, &masterXfer);
}

void cs_high(void)
{
  W5100_CS_OUT = 1;
}

void cs_low(void)
{
  W5100_CS_OUT = 0;
}


/**
*@brief  W5100s复位设置函数
*@param  无
*@return 无
*/
void reset_w5100s(void)
{
  W5100_RST_OUT = 0;
  delay_ms(500);  
  W5100_RST_OUT = 1;
  delay_ms(1500);  
  
}


void W5100s_Init()
{
  systick_init(0);
  SPI_PinConfiguration();
  SPI_Configuration();
  reg_wizchip_spi_cbfunc(spi_read_byte,spi_send_byte);		  // SPI读写数据函数映射
  reg_wizchip_cs_cbfunc(cs_low,cs_high);						  // SPI片选控制函数映射
  reset_w5100s();                       						  // W5100S硬件复位 
  set_w5100s_mac();					  						  // 设置W5100SMAC地址
  set_w5100s_netinfo();				  						  // 设置W5100S网络参数
  PHY_check();                          						  // 网线检测程序
  wizchip_init(txsize,rxsize);		  						  // 初始化4个Socket的发送接收缓存大小
  
}
