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

float Round::getAttackerStrength() const 
{
	return m_attackerStrength;
}

std::size_t Round::getMinArmorIndex() const 
{
	switch (m_stage) {
	case Stage::WW2:
	case Stage::ColdWar:
		return 0;
	case Stage::Modern:
		return 3;
	case Stage::Future:
		return 6;
	}
}

std::size_t Round::getMaxArmorIndex() const 
{
	switch (m_stage) {
	case Stage::WW2:
		return 2;
	case Stage::ColdWar:
		return 5;
	case Stage::Modern:
		return 8;
	case Stage::Future:
		return 11;
	}
}
