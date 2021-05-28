#pragma once
#include "Round.h"

void Round::start() {
	m_number++;
	m_inProgress = true;
	m_attackerStrength += 0.2f;

	// Set stage
}

void Round::end() {
	m_inProgress = false;
}

bool Round::isInProgress() const {
	return m_inProgress;
}

unsigned int Round::getNumber() const
{
	return m_number;
}

float Round::getAttackerStrength() const {
	return m_attackerStrength;
}

std::size_t Round::getMinArmorIndex() const {
	std::size_t index;

	switch (m_stage) {
	case Stage::WW2:
	case Stage::COLD_WAR:
		index = 0;
		break;
	case Stage::MODERN:
		index = 3;
		break;
	case Stage::FUTURE:
		index = 6;
		break;
	}

	return index;
}

std::size_t Round::getMaxArmorIndex() const {
	std::size_t index;

	switch (m_stage) {
	case Stage::WW2:
		index = 2;
		break;
	case Stage::COLD_WAR:
		index = 5;
		break;
	case Stage::MODERN:
		index = 8;
		break;
	case Stage::FUTURE:
		index = 11;
		break;
	}

	return index;
}
