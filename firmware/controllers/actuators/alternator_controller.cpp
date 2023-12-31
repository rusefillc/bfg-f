/**
 * @file    alternator_controller.cpp
 * @brief   alternator controller - some newer vehicles control alternator with ECU
 *
 * @date Apr 6, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if EFI_ALTERNATOR_CONTROL
#include "alternator_controller.h"
#include "efi_pid.h"
#include "local_version_holder.h"
#include "periodic_task.h"

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif /* HAS_OS_ACCESS */

static SimplePwm alternatorControl("alt");
static Pid alternatorPid(&persistentState.persistentConfiguration.engineConfiguration.alternatorControl);

static percent_t currentAltDuty;

static bool shouldResetPid = false;

static void pidReset() {
	alternatorPid.reset();
}

void AlternatorController::onFastCallback() {
	if (!isBrainPinValid(engineConfiguration->alternatorControlPin)) {
		return;
	}

#if ! EFI_UNIT_TEST
	if (shouldResetPid) {
		pidReset();
		shouldResetPid = false;
	}
#endif

	// this block could be executed even in on/off alternator control mode
	// but at least we would reflect latest state
#if EFI_TUNER_STUDIO
	alternatorPid.postState(engine->outputChannels.alternatorStatus);
#endif /* EFI_TUNER_STUDIO */

	bool alternatorShouldBeEnabledAtCurrentRpm = Sensor::getOrZero(SensorType::Rpm) > engineConfiguration->cranking.rpm;

	if (!engineConfiguration->isAlternatorControlEnabled || !alternatorShouldBeEnabledAtCurrentRpm) {
		// we need to avoid accumulating iTerm while engine is not running
		pidReset();

		// Shut off output if not needed
		alternatorControl.setSimplePwmDutyCycle(0);

		return;
	}

	auto vBatt = Sensor::get(SensorType::BatteryVoltage);
	float targetVoltage = engineConfiguration->targetVBatt;

	if (!vBatt) {
		// Somehow battery voltage isn't valid, disable alternator control
		alternatorPid.reset();
		alternatorControl.setSimplePwmDutyCycle(0);
	} else {
		currentAltDuty = alternatorPid.getOutput(targetVoltage, vBatt.Value, FAST_CALLBACK_PERIOD_MS / 1000.0f);
    	// see "idle air Bump for AC" comment
		int acDutyBump = engine->module<AcController>().unmock().acButtonState ? engineConfiguration->acRelayAlternatorDutyAdder : 0;
		currentAltDuty += acDutyBump;
		if (engineConfiguration->isVerboseAlternator) {
			efiPrintf("alt duty: %.2f/vbatt=%.2f/p=%.2f/i=%.2f/d=%.2f int=%.2f", currentAltDuty, vBatt.Value,
					alternatorPid.getP(), alternatorPid.getI(), alternatorPid.getD(), alternatorPid.getIntegration());
		}

		alternatorControl.setSimplePwmDutyCycle(PERCENT_TO_DUTY(currentAltDuty));
	}
}

void onConfigurationChangeAlternatorCallback(engine_configuration_s *previousConfiguration) {
	shouldResetPid = !alternatorPid.isSame(&previousConfiguration->alternatorControl);
}

void initAlternatorCtrl() {
	if (!isBrainPinValid(engineConfiguration->alternatorControlPin))
		return;

	startSimplePwm(&alternatorControl,
				"Alternator control",
				&engine->executor,
				&enginePins.alternatorPin,
				engineConfiguration->alternatorPwmFrequency, 0);
}

#endif /* EFI_ALTERNATOR_CONTROL */
