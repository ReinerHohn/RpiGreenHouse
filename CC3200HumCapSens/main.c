// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "interrupt.h"
#include "prcm.h"
#include "gpio.h"
#include "utils.h"
#include "timer.h"
#include "rom.h"
#include "rom_map.h"
#include "pin.h"

// Common interface includes
#include "uart_if.h"
#include "pinmux.h"

// SPI Driverlib includes
#include "hw_common_reg.h"
#include "spi.h"
#include "uart.h"

#define APPLICATION_VERSION     "1.1.1"

//SPI
//*****************************************************************************
//
// Application Master/Slave mode selector macro
//
// MASTER_MODE = 1 : Application in master mode
// MASTER_MODE = 0 : Application in slave mode
//
//*****************************************************************************
#define MASTER_MODE      0

#define SPI_IF_BIT_RATE  100000
#define TR_BUFF_SIZE     4

//#define MASTER_MSG       "This is CC3200 SPI Master Application\n\r"
#define SLAVE_MSG        "This is CC3200 SPI Slave Application\n\r"
//SPI_END
#define APP_NAME        "Timer Count Capture"
#define TIMER_FREQ      80000000

static volatile int g_bFilled = false;
//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
static unsigned long g_ulSamples[2];
static unsigned long g_ulFreq;

//SPI
static unsigned char g_ucTxBuff[TR_BUFF_SIZE];
static unsigned char g_ucRxBuff[TR_BUFF_SIZE];
static unsigned char ucTxBuffNdx;
static unsigned char ucRxBuffNdx;
//SPI

#if defined(ccs) || defined(gcc)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
//*****************************************************************************
//                 GLOBAL VARIABLES -- End
//*****************************************************************************

//SPI
//*****************************************************************************
//
//! SPI Slave Interrupt handler
//!
//! This function is invoked when SPI slave has its receive register full or
//! transmit register empty.
//!
//! \return None.
//
//*****************************************************************************
static void SlaveIntHandler() {
	unsigned long ulRecvData;
	unsigned long ulStatus;

	ulStatus = MAP_SPIIntStatus(GSPI_BASE, true);

	MAP_SPIIntClear(GSPI_BASE, SPI_INT_RX_FULL | SPI_INT_TX_EMPTY);

	if (ulStatus & SPI_INT_TX_EMPTY)
	{
		//Report("Tx %d", ucTxBuffNdx);
		MAP_SPIDataPut(GSPI_BASE, g_ucTxBuff[(ucTxBuffNdx + 0) % TR_BUFF_SIZE]);
		ucTxBuffNdx++;
	}

	if (ulStatus & SPI_INT_RX_FULL)
	{
		MAP_SPIDataGetNonBlocking(GSPI_BASE, &ulRecvData);

		g_bFilled = false;
		//Report("%c", ulRecvData);
		ucRxBuffNdx++;

		//ucTxBuffNdx += 3;
	}
}
//*****************************************************************************
//
//! SPI Slave mode main loop
//!
//! This function configures SPI modelue as slave and enables the channel for
//! communication
//!
//! \return None.
//
//*****************************************************************************
void SlaveMain() {
	//
	// Initialize the message
	//
	//memcpy(g_ucTxBuff, SLAVE_MSG, sizeof(SLAVE_MSG));

	//
	// Set Tx buffer index
	//
	ucTxBuffNdx = 0;
	ucRxBuffNdx = 0;

	//
	// Reset SPI
	//
	MAP_SPIReset(GSPI_BASE);

	//
	// Configure SPI interface
	//
	MAP_SPIConfigSetExpClk(GSPI_BASE, MAP_PRCMPeripheralClockGet(PRCM_GSPI),
	SPI_IF_BIT_RATE, SPI_MODE_SLAVE, SPI_SUB_MODE_0,
			(SPI_HW_CTRL_CS | SPI_4PIN_MODE | SPI_TURBO_OFF | SPI_CS_ACTIVEHIGH
					| SPI_WL_8));

	//
	// Register Interrupt Handler
	//
	MAP_SPIIntRegister(GSPI_BASE, SlaveIntHandler);

	//
	// Enable Interrupts
	//
	MAP_SPIIntEnable(GSPI_BASE, SPI_INT_RX_FULL | SPI_INT_TX_EMPTY);

	//
	// Enable SPI for communication
	//
	MAP_SPIEnable(GSPI_BASE);

	//
	// Print mode on uart
	//
	Message("Enabled SPI Interface in Slave Mode\n\rReceived : ");
}
//SPI_END

//*****************************************************************************
//
//! Timer interrupt handler
//
//*****************************************************************************
static void TimerIntHandler() {
	//
	// Clear the interrupt
	//
	MAP_TimerIntClear(TIMERA2_BASE, TIMER_CAPA_EVENT);

	//
	// Get the samples and compute the frequency
	//
	g_ulSamples[0] = g_ulSamples[1];
	g_ulSamples[1] = MAP_TimerValueGet(TIMERA2_BASE, TIMER_A);
	g_ulFreq = (TIMER_FREQ / (g_ulSamples[0] - g_ulSamples[1]));

	//Report("Frequency is %d\n",g_ulFreq);
}

//*****************************************************************************
//
//! Application startup display on UART
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void DisplayBanner(char * AppName) {

	Report("\n\n\n\r");
	Report("\t\t *************************************************\n\r");
	Report("\t\t\t  CC3200 %s Application       \n\r", AppName);
	Report("\t\t *************************************************\n\r");
	Report("\n\n\n\r");
}

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
void BoardInit(void) {
	/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
	//
	// Set vector table base
	//
#if defined(ccs) || defined(gcc)
	MAP_IntVTableBaseSet((unsigned long) &g_pfnVectors[0]);
#endif
#if defined(ewarm)
	MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
	//
	// Enable Processor
	//
	MAP_IntMasterEnable();
	MAP_IntEnable(FAULT_SYSTICK);

	PRCMCC3200MCUInit();
}

//*****************************************************************************
//
//! Main  Function
//
//*****************************************************************************
int main() {

	//
	// Initialize Board configurations
	//
	BoardInit();

	//
	// Pinmux for UART
	//
	PinMuxConfig();

	//
	// Configuring UART
	//
	InitTerm();

	//
	// Display Application Banner
	//
	DisplayBanner(APP_NAME);

	//
	// Enable pull down
	//
	MAP_PinConfigSet(PIN_05, PIN_TYPE_STD_PD, PIN_STRENGTH_6MA);

	//
	// Register timer interrupt hander
	//
	MAP_TimerIntRegister(TIMERA2_BASE, TIMER_A, TimerIntHandler);

	//
	// Configure the timer in edge count mode
	//
	MAP_TimerConfigure(TIMERA2_BASE,
			(TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME));

	//
	// Set the detection edge
	//
	MAP_TimerControlEvent(TIMERA2_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);

	//
	// Set the reload value
	//
	MAP_TimerLoadSet(TIMERA2_BASE, TIMER_A, 0xffff);

	//
	// Enable capture event interrupt
	//
	MAP_TimerIntEnable(TIMERA2_BASE, TIMER_CAPA_EVENT);

	//
	// Enable Timer
	//
	MAP_TimerEnable(TIMERA2_BASE, TIMER_A);

	//
	// Reset the peripheral
	//
	MAP_PRCMPeripheralReset(PRCM_GSPI);

#if MASTER_MODE

	MasterMain();

#else

	SlaveMain();

#endif

	while (1) {
		//
		// Report the calculate frequency
		//
		Report("Frequency : %d Hz\n\n\r", g_ulFreq);

		if (false == g_bFilled) {
			ucTxBuffNdx = 0;
			unsigned char c1 = (0xFF000000 & g_ulFreq) >> 24;
			unsigned char c2 = (0x00FF0000 & g_ulFreq) >> 16;
			unsigned char c3 = (0x0000FF00 & g_ulFreq) >> 8;
			unsigned char c4 = (0x000000FF & g_ulFreq) >> 0;

			g_ucTxBuff[0] = c1;
			g_ucTxBuff[1] = c2;
			g_ucTxBuff[2] = c3;
			g_ucTxBuff[3] = c4;

			long ulTest = (c1 << 24) | (c2 << 16) | (c3 << 8) | c4;
			Report("Nie gschirebe: %d. \r\n", ulTest);

			g_bFilled = true;
		}
		//
		// Delay loop
		//
		MAP_UtilsDelay(80000000 / 5);
	}
}
