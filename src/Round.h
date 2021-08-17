#pragma once
#include <cstddef>

class Round
{
private:
	unsigned int m_number = 0;
	bool m_inProgress = false;
	float m_attackerStrength = 0.0f;
	enum class Stage { WW2, ColdWar, Modern, Future };
	Stage m_stage = Stage::WW2;
public:
	void start();
	void end();
	bool isInProgress() const;

	unsigned int getNumber() const;
	float getAttackerStrength() const;
	std::size_t getMinArmorIndex() const;
	std::size_t getMaxArmorIndex() const;
};
