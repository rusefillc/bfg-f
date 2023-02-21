/*
 * @file ford_festiva.cpp
 *
 * set engine_type 14
 *
 * http://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Protege_1993
 * http://rusefi.com/forum/viewtopic.php?f=3&t=537
 * http://rusefi.com/forum/viewtopic.php?f=3&t=822
 *
 * @date Jan 3, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "ford_festiva.h"
#include "mazda_miata.h"
#include "custom_engine.h"

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
static const uint8_t racingFestivaIgnitionTable[16][16] = {
		/* Generated by TS2C on Mon Feb 08 13:59:57 EST 2016*/
		{/* 0 10.000	*//* 0 800.0*/13,	/* 1 1213.3281*/12,	/* 2 1626.6562*/10,	/* 3 2040.0*/10,	/* 4 2453.3438*/10,	/* 5 2866.6562*/10,	/* 6 3280.0*/11,	/* 7 3693.3438*/11,	/* 8 4106.6875*/11,	/* 9 4520.0*/12,	/* 10 4933.3125*/12,	/* 11 5346.6875*/12,	/* 12 5760.0*/12,	/* 13 6173.3125*/12,	/* 14 6586.6875*/12,	/* 15 7000.0*/12,	},
		{/* 1 16.000	*//* 0 800.0*/13,	/* 1 1213.3281*/12,	/* 2 1626.6562*/10,	/* 3 2040.0*/10,	/* 4 2453.3438*/10,	/* 5 2866.6562*/10,	/* 6 3280.0*/11,	/* 7 3693.3438*/11,	/* 8 4106.6875*/11,	/* 9 4520.0*/12,	/* 10 4933.3125*/12,	/* 11 5346.6875*/12,	/* 12 5760.0*/12,	/* 13 6173.3125*/12,	/* 14 6586.6875*/12,	/* 15 7000.0*/12,	},
		{/* 2 22.000	*//* 0 800.0*/14,	/* 1 1213.3281*/14,	/* 2 1626.6562*/14,	/* 3 2040.0*/12,	/* 4 2453.3438*/12,	/* 5 2866.6562*/12,	/* 6 3280.0*/12,	/* 7 3693.3438*/12,	/* 8 4106.6875*/12,	/* 9 4520.0*/12,	/* 10 4933.3125*/12,	/* 11 5346.6875*/12,	/* 12 5760.0*/12,	/* 13 6173.3125*/12,	/* 14 6586.6875*/12,	/* 15 7000.0*/12,	},
		{/* 3 28.000	*//* 0 800.0*/14,	/* 1 1213.3281*/14,	/* 2 1626.6562*/14,	/* 3 2040.0*/12,	/* 4 2453.3438*/12,	/* 5 2866.6562*/12,	/* 6 3280.0*/12,	/* 7 3693.3438*/12,	/* 8 4106.6875*/12,	/* 9 4520.0*/12,	/* 10 4933.3125*/12,	/* 11 5346.6875*/12,	/* 12 5760.0*/12,	/* 13 6173.3125*/12,	/* 14 6586.6875*/12,	/* 15 7000.0*/12,	},
		{/* 4 31.000	*//* 0 800.0*/15,	/* 1 1213.3281*/15,	/* 2 1626.6562*/15,	/* 3 2040.0*/15,	/* 4 2453.3438*/15,	/* 5 2866.6562*/16,	/* 6 3280.0*/16,	/* 7 3693.3438*/23,	/* 8 4106.6875*/23,	/* 9 4520.0*/26,	/* 10 4933.3125*/26,	/* 11 5346.6875*/26,	/* 12 5760.0*/26,	/* 13 6173.3125*/26,	/* 14 6586.6875*/26,	/* 15 7000.0*/26,	},
		{/* 5 35.000	*//* 0 800.0*/15,	/* 1 1213.3281*/15,	/* 2 1626.6562*/15,	/* 3 2040.0*/15,	/* 4 2453.3438*/15,	/* 5 2866.6562*/16,	/* 6 3280.0*/16,	/* 7 3693.3438*/23,	/* 8 4106.6875*/23,	/* 9 4520.0*/26,	/* 10 4933.3125*/26,	/* 11 5346.6875*/26,	/* 12 5760.0*/26,	/* 13 6173.3125*/26,	/* 14 6586.6875*/26,	/* 15 7000.0*/26,	},
		{/* 6 38.000	*//* 0 800.0*/16,	/* 1 1213.3281*/16,	/* 2 1626.6562*/17,	/* 3 2040.0*/19,	/* 4 2453.3438*/19,	/* 5 2866.6562*/21,	/* 6 3280.0*/21,	/* 7 3693.3438*/25,	/* 8 4106.6875*/25,	/* 9 4520.0*/26,	/* 10 4933.3125*/26,	/* 11 5346.6875*/26,	/* 12 5760.0*/26,	/* 13 6173.3125*/26,	/* 14 6586.6875*/26,	/* 15 7000.0*/26,	},
		{/* 7 41.000	*//* 0 800.0*/16,	/* 1 1213.3281*/16,	/* 2 1626.6562*/17,	/* 3 2040.0*/19,	/* 4 2453.3438*/19,	/* 5 2866.6562*/21,	/* 6 3280.0*/21,	/* 7 3693.3438*/25,	/* 8 4106.6875*/25,	/* 9 4520.0*/26,	/* 10 4933.3125*/26,	/* 11 5346.6875*/26,	/* 12 5760.0*/26,	/* 13 6173.3125*/26,	/* 14 6586.6875*/26,	/* 15 7000.0*/26,	},
		{/* 8 48.000	*//* 0 800.0*/16,	/* 1 1213.3281*/17,	/* 2 1626.6562*/17,	/* 3 2040.0*/19,	/* 4 2453.3438*/19,	/* 5 2866.6562*/22,	/* 6 3280.0*/22,	/* 7 3693.3438*/22,	/* 8 4106.6875*/30,	/* 9 4520.0*/30,	/* 10 4933.3125*/30,	/* 11 5346.6875*/30,	/* 12 5760.0*/30,	/* 13 6173.3125*/30,	/* 14 6586.6875*/30,	/* 15 7000.0*/30,	},
		{/* 9 52.000	*//* 0 800.0*/17,	/* 1 1213.3281*/17,	/* 2 1626.6562*/17,	/* 3 2040.0*/20,	/* 4 2453.3438*/20,	/* 5 2866.6562*/22,	/* 6 3280.0*/23,	/* 7 3693.3438*/23,	/* 8 4106.6875*/30,	/* 9 4520.0*/30,	/* 10 4933.3125*/30,	/* 11 5346.6875*/30,	/* 12 5760.0*/30,	/* 13 6173.3125*/30,	/* 14 6586.6875*/30,	/* 15 7000.0*/30,	},
		{/* 10 59.000	*//* 0 800.0*/17,	/* 1 1213.3281*/17,	/* 2 1626.6562*/17,	/* 3 2040.0*/21,	/* 4 2453.3438*/21,	/* 5 2866.6562*/23,	/* 6 3280.0*/23,	/* 7 3693.3438*/23,	/* 8 4106.6875*/30,	/* 9 4520.0*/30,	/* 10 4933.3125*/30,	/* 11 5346.6875*/30,	/* 12 5760.0*/30,	/* 13 6173.3125*/30,	/* 14 6586.6875*/30,	/* 15 7000.0*/30,	},
		{/* 11 65.000	*//* 0 800.0*/16,	/* 1 1213.3281*/16,	/* 2 1626.6562*/16,	/* 3 2040.0*/21,	/* 4 2453.3438*/21,	/* 5 2866.6562*/23,	/* 6 3280.0*/24,	/* 7 3693.3438*/24,	/* 8 4106.6875*/30,	/* 9 4520.0*/30,	/* 10 4933.3125*/30,	/* 11 5346.6875*/30,	/* 12 5760.0*/30,	/* 13 6173.3125*/30,	/* 14 6586.6875*/30,	/* 15 7000.0*/30,	},
		{/* 12 74.000	*//* 0 800.0*/15,	/* 1 1213.3281*/15,	/* 2 1626.6562*/15,	/* 3 2040.0*/19,	/* 4 2453.3438*/19,	/* 5 2866.6562*/23,	/* 6 3280.0*/23,	/* 7 3693.3438*/23,	/* 8 4106.6875*/29,	/* 9 4520.0*/28,	/* 10 4933.3125*/30,	/* 11 5346.6875*/30,	/* 12 5760.0*/30,	/* 13 6173.3125*/30,	/* 14 6586.6875*/30,	/* 15 7000.0*/30,	},
		{/* 13 85.000	*//* 0 800.0*/20,	/* 1 1213.3281*/15,	/* 2 1626.6562*/15,	/* 3 2040.0*/19,	/* 4 2453.3438*/19,	/* 5 2866.6562*/24,	/* 6 3280.0*/26,	/* 7 3693.3438*/26,	/* 8 4106.6875*/26,	/* 9 4520.0*/27,	/* 10 4933.3125*/27,	/* 11 5346.6875*/28,	/* 12 5760.0*/28,	/* 13 6173.3125*/28,	/* 14 6586.6875*/28,	/* 15 7000.0*/30,	},
		{/* 14 94.000	*//* 0 800.0*/20,	/* 1 1213.3281*/16,	/* 2 1626.6562*/16,	/* 3 2040.0*/19,	/* 4 2453.3438*/19,	/* 5 2866.6562*/25,	/* 6 3280.0*/26,	/* 7 3693.3438*/26,	/* 8 4106.6875*/26,	/* 9 4520.0*/27,	/* 10 4933.3125*/27,	/* 11 5346.6875*/28,	/* 12 5760.0*/28,	/* 13 6173.3125*/28,	/* 14 6586.6875*/28,	/* 15 7000.0*/30,	},
		{/* 15 101.000	*//* 0 800.0*/20,	/* 1 1213.3281*/16,	/* 2 1626.6562*/16,	/* 3 2040.0*/19,	/* 4 2453.3438*/19,	/* 5 2866.6562*/23,	/* 6 3280.0*/24,	/* 7 3693.3438*/24,	/* 8 4106.6875*/26,	/* 9 4520.0*/27,	/* 10 4933.3125*/27,	/* 11 5346.6875*/28,	/* 12 5760.0*/28,	/* 13 6173.3125*/30,	/* 14 6586.6875*/30,	/* 15 7000.0*/30,	},
};
#endif

static const uint8_t racingFestivaVeTable[16][16] = {
		/* Generated by TS2C on Mon Feb 08 13:58:49 EST 2016*/
		{/* 0 10.000	*//* 0 800.0*/110,	/* 1 1200.0*/105,	/* 2 1600.0*/94,	/* 3 2000.0*/104,	/* 4 2400.0*/108,	/* 5 2800.0*/102,	/* 6 3200.0*/104,	/* 7 3600.0*/106,	/* 8 4100.0*/130,	/* 9 4500.0*/130,	/* 10 4900.0*/110,	/* 11 5300.0*/110,	/* 12 5700.0*/110,	/* 13 6100.0*/110,	/* 14 6500.0*/110,	/* 15 7000.0*/110,	},
		{/* 1 20.000	*//* 0 800.0*/92,	/* 1 1200.0*/88,	/* 2 1600.0*/87,	/* 3 2000.0*/94,	/* 4 2400.0*/100,	/* 5 2800.0*/101,	/* 6 3200.0*/99,	/* 7 3600.0*/108,	/* 8 4100.0*/110,	/* 9 4500.0*/107,	/* 10 4900.0*/107,	/* 11 5300.0*/110,	/* 12 5700.0*/110,	/* 13 6100.0*/110,	/* 14 6500.0*/110,	/* 15 7000.0*/110,	},
		{/* 2 30.000	*//* 0 800.0*/93,	/* 1 1200.0*/90,	/* 2 1600.0*/94,	/* 3 2000.0*/98,	/* 4 2400.0*/104,	/* 5 2800.0*/111,	/* 6 3200.0*/113,	/* 7 3600.0*/117,	/* 8 4100.0*/119,	/* 9 4500.0*/118,	/* 10 4900.0*/114,	/* 11 5300.0*/115,	/* 12 5700.0*/115,	/* 13 6100.0*/115,	/* 14 6500.0*/115,	/* 15 7000.0*/115,	},
		{/* 3 40.000	*//* 0 800.0*/99,	/* 1 1200.0*/100,	/* 2 1600.0*/102,	/* 3 2000.0*/107,	/* 4 2400.0*/110,	/* 5 2800.0*/116,	/* 6 3200.0*/116,	/* 7 3600.0*/117,	/* 8 4100.0*/121,	/* 9 4500.0*/121,	/* 10 4900.0*/115,	/* 11 5300.0*/115,	/* 12 5700.0*/115,	/* 13 6100.0*/115,	/* 14 6500.0*/115,	/* 15 7000.0*/115,	},
		{/* 4 50.000	*//* 0 800.0*/106,	/* 1 1200.0*/112,	/* 2 1600.0*/103,	/* 3 2000.0*/106,	/* 4 2400.0*/111,	/* 5 2800.0*/121,	/* 6 3200.0*/120,	/* 7 3600.0*/122,	/* 8 4100.0*/125,	/* 9 4500.0*/120,	/* 10 4900.0*/115,	/* 11 5300.0*/115,	/* 12 5700.0*/115,	/* 13 6100.0*/115,	/* 14 6500.0*/115,	/* 15 7000.0*/115,	},
		{/* 5 60.000	*//* 0 800.0*/108,	/* 1 1200.0*/107,	/* 2 1600.0*/105,	/* 3 2000.0*/107,	/* 4 2400.0*/113,	/* 5 2800.0*/124,	/* 6 3200.0*/121,	/* 7 3600.0*/119,	/* 8 4100.0*/121,	/* 9 4500.0*/120,	/* 10 4900.0*/115,	/* 11 5300.0*/115,	/* 12 5700.0*/115,	/* 13 6100.0*/115,	/* 14 6500.0*/115,	/* 15 7000.0*/115,	},
		{/* 6 70.000	*//* 0 800.0*/106,	/* 1 1200.0*/108,	/* 2 1600.0*/112,	/* 3 2000.0*/113,	/* 4 2400.0*/113,	/* 5 2800.0*/109,	/* 6 3200.0*/97,	/* 7 3600.0*/120,	/* 8 4100.0*/120,	/* 9 4500.0*/120,	/* 10 4900.0*/120,	/* 11 5300.0*/120,	/* 12 5700.0*/120,	/* 13 6100.0*/120,	/* 14 6500.0*/120,	/* 15 7000.0*/120,	},
		{/* 7 80.000	*//* 0 800.0*/109,	/* 1 1200.0*/107,	/* 2 1600.0*/107,	/* 3 2000.0*/113,	/* 4 2400.0*/113,	/* 5 2800.0*/120,	/* 6 3200.0*/120,	/* 7 3600.0*/120,	/* 8 4100.0*/120,	/* 9 4500.0*/120,	/* 10 4900.0*/120,	/* 11 5300.0*/120,	/* 12 5700.0*/120,	/* 13 6100.0*/120,	/* 14 6500.0*/120,	/* 15 7000.0*/120,	},
		{/* 8 90.000	*//* 0 800.0*/107,	/* 1 1200.0*/109,	/* 2 1600.0*/109,	/* 3 2000.0*/115,	/* 4 2400.0*/110,	/* 5 2800.0*/120,	/* 6 3200.0*/120,	/* 7 3600.0*/120,	/* 8 4100.0*/120,	/* 9 4500.0*/120,	/* 10 4900.0*/120,	/* 11 5300.0*/120,	/* 12 5700.0*/120,	/* 13 6100.0*/120,	/* 14 6500.0*/120,	/* 15 7000.0*/120,	},
		{/* 9 100.000	*//* 0 800.0*/97,	/* 1 1200.0*/100,	/* 2 1600.0*/100,	/* 3 2000.0*/104,	/* 4 2400.0*/100,	/* 5 2800.0*/120,	/* 6 3200.0*/120,	/* 7 3600.0*/120,	/* 8 4100.0*/120,	/* 9 4500.0*/121,	/* 10 4900.0*/121,	/* 11 5300.0*/120,	/* 12 5700.0*/120,	/* 13 6100.0*/120,	/* 14 6500.0*/120,	/* 15 7000.0*/120,	},
		{/* 10 110.000	*//* 0 800.0*/100,	/* 1 1200.0*/100,	/* 2 1600.0*/100,	/* 3 2000.0*/100,	/* 4 2400.0*/100,	/* 5 2800.0*/120,	/* 6 3200.0*/120,	/* 7 3600.0*/120,	/* 8 4100.0*/120,	/* 9 4500.0*/120,	/* 10 4900.0*/120,	/* 11 5300.0*/120,	/* 12 5700.0*/120,	/* 13 6100.0*/120,	/* 14 6500.0*/120,	/* 15 7000.0*/120,	},
		{/* 11 120.000	*//* 0 800.0*/100,	/* 1 1200.0*/100,	/* 2 1600.0*/100,	/* 3 2000.0*/100,	/* 4 2400.0*/100,	/* 5 2800.0*/120,	/* 6 3200.0*/120,	/* 7 3600.0*/120,	/* 8 4100.0*/120,	/* 9 4500.0*/120,	/* 10 4900.0*/120,	/* 11 5300.0*/120,	/* 12 5700.0*/120,	/* 13 6100.0*/120,	/* 14 6500.0*/120,	/* 15 7000.0*/120,	},
		{/* 12 130.000	*//* 0 800.0*/100,	/* 1 1200.0*/100,	/* 2 1600.0*/100,	/* 3 2000.0*/100,	/* 4 2400.0*/100,	/* 5 2800.0*/120,	/* 6 3200.0*/120,	/* 7 3600.0*/120,	/* 8 4100.0*/120,	/* 9 4500.0*/120,	/* 10 4900.0*/120,	/* 11 5300.0*/120,	/* 12 5700.0*/120,	/* 13 6100.0*/120,	/* 14 6500.0*/120,	/* 15 7000.0*/120,	},
		{/* 13 140.000	*//* 0 800.0*/100,	/* 1 1200.0*/100,	/* 2 1600.0*/100,	/* 3 2000.0*/100,	/* 4 2400.0*/100,	/* 5 2800.0*/120,	/* 6 3200.0*/120,	/* 7 3600.0*/120,	/* 8 4100.0*/120,	/* 9 4500.0*/120,	/* 10 4900.0*/120,	/* 11 5300.0*/120,	/* 12 5700.0*/120,	/* 13 6100.0*/120,	/* 14 6500.0*/120,	/* 15 7000.0*/120,	},
		{/* 14 150.000	*//* 0 800.0*/100,	/* 1 1200.0*/100,	/* 2 1600.0*/100,	/* 3 2000.0*/100,	/* 4 2400.0*/100,	/* 5 2800.0*/120,	/* 6 3200.0*/120,	/* 7 3600.0*/120,	/* 8 4100.0*/120,	/* 9 4500.0*/120,	/* 10 4900.0*/120,	/* 11 5300.0*/120,	/* 12 5700.0*/120,	/* 13 6100.0*/120,	/* 14 6500.0*/120,	/* 15 7000.0*/120,	},
		{/* 15 160.000	*//* 0 800.0*/100,	/* 1 1200.0*/100,	/* 2 1600.0*/100,	/* 3 2000.0*/100,	/* 4 2400.0*/100,	/* 5 2800.0*/120,	/* 6 3200.0*/120,	/* 7 3600.0*/120,	/* 8 4100.0*/120,	/* 9 4500.0*/120,	/* 10 4900.0*/120,	/* 11 5300.0*/120,	/* 12 5700.0*/120,	/* 13 6100.0*/120,	/* 14 6500.0*/120,	/* 15 7000.0*/120,	},
};

/**
 * pin 1I/W9 - extra +5v
 * set engine_type 14
 */
void setFordEscortGt() {
	common079721_2351();

	engineConfiguration->trigger.type = TT_MAZDA_DOHC_1_4;

	setDensoTODO(config);

	engineConfiguration->globalFuelCorrection = 0.75;
	engineConfiguration->specs.displacement = 1.839;
	setAlgorithm(LM_SPEED_DENSITY);

	static const uint16_t veRpmBins[] = 
	{
		800,
		1200,
		1600,
		2000,
		2400,
		2800,
		3200,
		3600,
		4100,
		4500,
		4900,
		5300,
		5700,
		6100,
		6500,
		7000
	};

	copyArray(config->veRpmBins, veRpmBins);


	copyTable(config->veTable, racingFestivaVeTable);

//	engineConfiguration->triggerInputPins[0] = Gpio::C6; // 2G YEL/BLU
//	engineConfiguration->triggerInputPins[1] = Gpio::A5; // 2E White CKP

	// in case of SOHC distributor we only have one signal
//	engineConfiguration->triggerInputPins[0] = Gpio::A5; // 2E White CKP
//	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;

	// in case of DOHC distributor we have two signals
	engineConfiguration->triggerInputPins[0] = Gpio::C6;
	engineConfiguration->triggerInputPins[1] = Gpio::A5; // 2E White CKP

	// Denso 195500-2180
	engineConfiguration->injector.flow = 265;

	engineConfiguration->map.sensor.type = MT_DENSO183;
	/**
	 * pin PA4: jumper W47<>W47 - ecu plug 3I
	 */
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;

	setEgoSensor(ES_Innovate_MTX_L);
	engineConfiguration->afr.hwChannel = EFI_ADC_2; // Frankenso analog #5 // PA2

	// set_idle_position 10
	engineConfiguration->manIdlePosition = 10;
	engineConfiguration->crankingIACposition = 65;

	setWholeIatCorrTimingTable(0);


	// set global_trigger_offset_angle -37
	engineConfiguration->globalTriggerAngleOffset = -37;

	// todo: change to 15?
	// set cranking_timing_angle 3
	engineConfiguration->crankingTimingAngle = 3;

	setLinearCurve(config->ignitionLoadBins, 20, 105, 5);
	setWholeTimingTable_d(10);
	setTable(config->lambdaTable, 0.92f);

	setSingleCoilDwell();
	engineConfiguration->ignitionMode = IM_ONE_COIL;

	engineConfiguration->triggerSimulatorPinModes[0] = OM_OPENDRAIN;
	engineConfiguration->triggerSimulatorPinModes[1] = OM_OPENDRAIN;

	// individual coils
	// W6  PC9
	// W8  PC7
	// W12 PE8
	// W13 PE12

	engineConfiguration->ignitionPins[0] = Gpio::C9;
	engineConfiguration->ignitionPins[1] = Gpio::C7;
	engineConfiguration->ignitionPins[2] = Gpio::E8;
	engineConfiguration->ignitionPins[3] = Gpio::E12;
	engineConfiguration->ignitionPinMode = OM_DEFAULT;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	/**
	 * Outputs
	 */
	// Frankenso low out #1: PE6
	// Frankenso low out #2: PE5 MIL
	// Frankenso low out #3:
	// Frankenso low out #4:
	// Frankenso low out #5: PE3 VICS solenoid
	// Frankenso low out #6: PE4
	// Frankenso low out #7: PE0<>PD5
	// Frankenso low out #8: PE2 INJ
	// Frankenso low out #9: PB9 IDLE
	// Frankenso low out #10: PE1<>PD3 INJ 1&3
	// Frankenso low out #11: PB8
	// Frankenso low out #12: PB7

	engineConfiguration->injectionPins[0] = Gpio::D3;
	engineConfiguration->injectionPins[1] = Gpio::E2;


	// 40% idle is good default
	engineConfiguration->idle.solenoidFrequency = 300;
	engineConfiguration->idle.solenoidPin = Gpio::B9;

	engineConfiguration->malfunctionIndicatorPin = Gpio::E5;
	engineConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	engineConfiguration->tunerStudioSerialSpeed = 19200;

	commonFrankensoAnalogInputs();
	setCommonNTCSensor(&engineConfiguration->clt, 2700);
	setCommonNTCSensor(&engineConfiguration->iat, 2700);

	// we have a 1999 Auto Miata TB mounted on this car
	engineConfiguration->tpsMin = 630; // convert 12to10 bit (ADC/4)
	engineConfiguration->tpsMax = 115; // convert 12to10 bit (ADC/4)

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3;
//	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->mafAdcChannel = EFI_ADC_0;
	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	engineConfiguration->iat.adcChannel = EFI_ADC_11;

	// todo: 8.2 or 10k?
	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;

	// VICS solenoid
	static const uint16_t ignitionRpmBins[] =
	{
		800,
		1200,
		1600,
		2000,
		2400,
		2800,
		3200,
		3600,
		4100,
		4500,
		4900,
		5300,
		5700,
		6100,
		6500,
		7000
	};

	copyArray(config->ignitionRpmBins, ignitionRpmBins);

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
	copyTable(config->ignitionTable, racingFestivaIgnitionTable);
#endif

	engineConfiguration->tpsAccelEnrichmentThreshold = 40;

	engineConfiguration->isSdCardEnabled = true;
}

