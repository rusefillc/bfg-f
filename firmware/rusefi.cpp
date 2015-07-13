/**
 * @file	rusefi.cpp
 * @brief Initialization code and main status reporting look
 *
 * @date Dec 25, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

/**
 * @mainpage
 *
 * @section sec_into
 *
 * rusEfi is implemented based on the idea that with modern 100+ MHz microprocessors the relatively
 * undemanding task of internal combustion engine control could be implemented in a high-level, processor-independent
 * (to some extent) manner. Thus the key concepts of rusEfi: dependency on high-level hardware abstraction layer, software-based PWM etc.
 *
 * @section sec_main Brief overview
 *
 * rusEfi runs on crankshaft or camshaft ('trigger') position sensor events.
 * Once per crankshaft revolution we evaluate the amount of needed fuel and
 * the spark timing. Once we have decided on the parameters for this revolution
 * we schedule all the actions to be triggered by the closest trigger event.
 *
 * We also have some utility threads like idle control thread and communication threads.
 *
 *
 *
 * @section sec_trigger Trigger Decoding
 *
 * Our primary trigger decoder is based on the idea of synchronizing the primary shaft signal and simply counting events on
 * the secondary signal. A typical scenario would be when camshaft positions sensor is the primary signal and crankshaft is secondary,
 * but sometimes there would be two signals generated by two camshaft sensors.
 * Another scenario is when we only have crankshaft position sensor, this would make it the primary signal and there would be no secondary signal.
 *
 * There is no software filtering so the signals are expected to be valid. TODO: in reality we are still catching engine stop noise as unrealisticly high RPM.
 *
 * The decoder is configured to act either on the primary signal rise or on the primary signal fall. It then compares the duration
 * of time from the previous signal to the duration of time from the signal before previous, and if the ratio falls into the configurable
 * range between 'syncRatioFrom' and 'syncRatioTo' this is assumed to be the synchronizing event.
 *
 * For instance, for a 36/1 skipped tooth wheel the ratio range for synchronization is from 1.5 to 3
 *
 * Some triggers do not require synchronization, this case we just count signals.
 * A single tooth primary signal would be a typical example when synchronization is not needed.
 *
 *
 *
 *
 *
 * @section sec_scheduler Event Scheduler
 *
 * It is a general agreement to measure all angles in crankshaft angles. In a four stroke
 * engine, a full cycle consists of two revolutions of the crankshaft, so all the angles are
 * running between 0 and 720 degrees.
 *
 * Ignition timing is a great example of a process which highlights the need of a hybrid
 * approach to event scheduling.
 * The most important part of controlling ignition
 * is firing up the spark at the right moment - so, for this job we need 'angle-based' timing,
 * for example we would need to fire up the spark at 700 degrees. Before we can fire up the spark
 * at 700 degrees, we need to charge the ignition coil, for example this dwell time is 4ms - that
 * means we need to turn on the coil at '4 ms before 700 degrees'. Let's  assume that the engine is
 * current at 600 RPM - that means 360 degrees would take 100ms so 4ms is 14.4 degrees at current RPM which
 * means we need to start charting the coil at 685.6 degrees.
 *
 * The position sensors at our disposal are not providing us the current position at any moment of time -
 * all we've got is a set of events which are happening at the knows positions. For instance, let's assume that
 * our sensor sends as an event at 0 degrees, at 90 degrees, at 600 degrees and and 690 degrees.
 *
 * So, for this particular sensor the most precise scheduling would be possible if we schedule coil charting
 * as '85.6 degrees after the 600 degrees position sensor event', and spark firing as
 * '10 degrees after the 690 position sensor event'. Considering current RPM, we calculate that '10 degress after' is
 * 2.777ms, so we schedule spark firing at '2.777ms after the 690 position sensor event', thus combining trigger events
 * with time-based offset.
 *
 * @section config Persistent Configuration
 * engine_configuration_s structure is kept in the internal flash memory, it has all the settings. Currently rusefi.ini has a direct mapping of this structure.
 *
 * Please note that due to TunerStudio protocol it's important to have the total structure size in synch between the firmware and TS .ini file -
 * just to make sure that this is not forgotten the size of the structure is hard-coded as PAGE_0_SIZE constant. There is always some 'unused' fields added in advance so that
 * one can add some fields without the pain of increasing the total config page size.
 * <br>See flash_main.cpp
 *
 *
 * @section sec_fuel_injection Fuel Injection
 *
 *
 * @sectuion sec_misc Misc
 *
 * <BR>See main_trigger_callback.cpp for main trigger event handler
 * <BR>See fuel_math.cpp for details on fuel amount logic
 * <BR>See rpm_calculator.cpp for details on how getRpm() is calculated
 *
 */

#include "main.h"
#include "trigger_structure.h"
#include "hardware.h"
#include "engine_controller.h"
#include "efiGpio.h"

#include "global.h"
#include "rfi_perftest.h"
#include "rusefi.h"
#include "memstreams.h"

#include "eficonsole.h"
#include "status_loop.h"
#include "pin_repository.h"

#if EFI_HD44780_LCD
#include "lcd_HD44780.h"
#endif /* EFI_HD44780_LCD */

#if EFI_ENGINE_EMULATOR || defined(__DOXYGEN__)
#include "engine_emulator.h"
#endif /* EFI_ENGINE_EMULATOR */

static LoggingWithStorage sharedLogger("main");

bool_t main_loop_started = false;

static MemoryStream firmwareErrorMessageStream;
static char panicMessage[200];

uint8_t errorMessageBuffer[200];
bool hasFirmwareErrorFlag = false;

static virtual_timer_t resetTimer;

EXTERN_ENGINE
;

char *getFirmwareError(void) {
	return (char*) errorMessageBuffer;
}

// todo: move this into a hw-specific file
static void rebootNow(void) {
	NVIC_SystemReset();
}

/**
 * Some configuration changes require full firmware reset.
 * Once day we will write graceful shutdown, but that would be one day.
 */
static void scheduleReboot(void) {
	scheduleMsg(&sharedLogger, "Rebooting in 5 seconds...");
	lockAnyContext();
	chVTSetI(&resetTimer, 5 * CH_FREQUENCY, (vtfunc_t) rebootNow, NULL);
	unlockAnyContext();
}

void swo_init() {
	// todo: make SWO work
//     uint32_t SWOSpeed = 2000000; //2000kbps, default for ST-LINK
//     // todo: use a macro to access clock speed
//     uint32_t SWOPrescaler = (168000000 / SWOSpeed) - 1; // SWOSpeed in Hz, note that F_CPU is expected to be 96000000 in this case
//     CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk;
//     *((volatile unsigned *)(ITM_BASE + 0x400F0)) = 0x00000002; // "Selected PIN Protocol Register": Select which protocol to use for trace output (2: SWO)
//     *((volatile unsigned *)(ITM_BASE + 0x40010)) = SWOPrescaler; // "Async Clock Prescaler Register". Scale the baud rate of the asynchronous output
//     *((volatile unsigned *)(ITM_BASE + 0x00FB0)) = 0xC5ACCE55; // ITM Lock Access Register, C5ACCE55 enables more write access to Control Register 0xE00 :: 0xFFC
//     ITM->TCR = ITM_TCR_TraceBusID_Msk | ITM_TCR_SWOENA_Msk | ITM_TCR_SYNCENA_Msk | ITM_TCR_ITMENA_Msk; // ITM Trace Control Register
//     ITM->TPR = ITM_TPR_PRIVMASK_Msk; // ITM Trace Privilege Register
//     ITM->TER = 0x00000001; // ITM Trace Enable Register. Enabled tracing on stimulus ports. One bit per stimulus port.
//     *((volatile unsigned *)(ITM_BASE + 0x01000)) = 0x400003FE; // DWT_CTRL
//     *((volatile unsigned *)(ITM_BASE + 0x40304)) = 0x00000100; // Formatter and Flush Control Register
}

engine_configuration_s activeConfiguration;

static void rememberCurrentConfiguration(void) {
	memcpy(&activeConfiguration, engineConfiguration, sizeof(engine_configuration_s));
}

void applyNewConfiguration(void) {
	applyNewHardwareSettings();
	rememberCurrentConfiguration();
}

void runRusEfi(void) {
	msObjectInit(&firmwareErrorMessageStream, errorMessageBuffer, sizeof(errorMessageBuffer), 0);

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	engine->engineConfiguration2 = engineConfiguration2;
#endif

	initErrorHandling();

	swo_init();

	prepareVoidConfiguration(&activeConfiguration);

	/**
	 * First data structure keeps track of which hardware I/O pins are used by whom
	 */
	initPinRepository();

	/**
	 * Next we should initialize serial port console, it's important to know what's going on
	 */
	initializeConsole(&sharedLogger);

	engine->init();

	addConsoleAction("reboot", scheduleReboot);

	/**
	 * Initialize hardware drivers
	 */
	initHardware(&sharedLogger);

	initStatusLoop(engine);
	/**
	 * Now let's initialize actual engine control logic
	 * todo: should we initialize some? most? controllers before hardware?
	 */
	initEngineContoller(&sharedLogger PASS_ENGINE_PARAMETER_F);

#if EFI_PERF_METRICS || defined(__DOXYGEN__)
	initTimePerfActions(&sharedLogger);
#endif

#if EFI_ENGINE_EMULATOR || defined(__DOXYGEN__)
	initEngineEmulator(&sharedLogger, engine);
#endif
	startStatusThreads(engine);

	rememberCurrentConfiguration();

	print("Running main loop\r\n");
	main_loop_started = true;
	/**
	 * This loop is the closes we have to 'main loop' - but here we only publish the status. The main logic of engine
	 * control is around main_trigger_callback
	 */
	while (true) {
		efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "stack#1");

#if (EFI_CLI_SUPPORT && !EFI_UART_ECHO_TEST_MODE) || defined(__DOXYGEN__)
		// sensor state + all pending messages for our own dev console
		updateDevConsoleState(engine);
#endif /* EFI_CLI_SUPPORT */

		chThdSleepMilliseconds(boardConfiguration->consoleLoopPeriod);
	}
}

void chDbgStackOverflowPanic(Thread *otp) {
	strcpy(panicMessage, "stack overflow: ");
#ifdef CH_USE_REGISTRY
	strcat(panicMessage, otp->p_name);
#endif
	chDbgPanic3(panicMessage, __FILE__, __LINE__);
}

extern engine_pins_s enginePins;

// todo: why is this method here and not in error_handling.cpp ?
void firmwareError(const char *errorMsg, ...) {
	if (hasFirmwareErrorFlag)
		return;
	ON_FATAL_ERROR()
	;
	hasFirmwareErrorFlag = true;
	if (indexOf(errorMsg, '%') == -1) {
		/**
		 * in case of simple error message let's reduce stack usage
		 * because chvprintf might be causing an error
		 */
		strncpy((char*) errorMessageBuffer, errorMsg, sizeof(errorMessageBuffer) - 1);
		errorMessageBuffer[sizeof(errorMessageBuffer) - 1] = 0; // just to be sure
	} else {
		firmwareErrorMessageStream.eos = 0; // reset
		va_list ap;
		va_start(ap, errorMsg);
		chvprintf((BaseSequentialStream *) &firmwareErrorMessageStream, errorMsg, ap);
		va_end(ap);

		firmwareErrorMessageStream.buffer[firmwareErrorMessageStream.eos] = 0; // need to terminate explicitly
	}
}

static char UNUSED_RAM_SIZE[200];

static char UNUSED_CCM_SIZE[3600] CCM_OPTIONAL;

int getRusEfiVersion(void) {
	if (UNUSED_RAM_SIZE[0] != 0)
		return 123; // this is here to make the compiler happy about the unused array
	if (UNUSED_CCM_SIZE[0] * 0 != 0)
		return 3211; // this is here to make the compiler happy about the unused array
	return 20150713;
}
