#pragma once

#include "airmass.h"

class MafAirmass : public AirmassVeModelBase {
public:
	explicit MafAirmass(const ValueProvider3D& veTable) : AirmassVeModelBase(veTable) {}

	AirmassResult getAirmass(int rpm, bool postState) override;

	// Compute airmass based on flow & engine speed
	AirmassResult getAirmassImpl(float massAirFlow, int rpm, bool postState) const;

private:
	float getMaf() const;
};
