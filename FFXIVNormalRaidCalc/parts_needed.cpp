#include "stdafx.h"
#include "parts_needed.h"

PartsNeeded_t::PartsNeeded_t(int head, int body, int hands, int waist, int legs, int feet, int acc)
	: m_head(head), m_body(body), m_hands(hands), m_waist(waist), m_legs(legs), m_feet(feet), m_acc(acc)
{
	if (m_head < 0 || m_head > 2)
		throw;
	if (m_body < 0 || m_body > 4)
		throw;
	if (m_hands < 0 || m_hands > 2)
		throw;
	if (m_waist < 0 || m_waist > 1)
		throw;
	if (m_legs < 0 || m_legs > 4)
		throw;
	if (m_feet < 0 || m_feet > 2)
		throw;
	if (m_acc < 0 || m_acc > 4)
		throw;
}

bool PartsNeeded_t::operator<(const PartsNeeded_t & rhs) const
{
	if (m_head != rhs.m_head)
		return m_head < rhs.m_head;

	if (m_body != rhs.m_body)
		return m_body < rhs.m_body;

	if (m_hands != rhs.m_hands)
		return m_hands < rhs.m_hands;

	if (m_waist != rhs.m_waist)
		return m_waist < rhs.m_waist;

	if (m_legs != rhs.m_legs)
		return m_legs < rhs.m_legs;

	if (m_feet != rhs.m_feet)
		return m_feet < rhs.m_feet;

	return m_acc < rhs.m_acc;
}

int PartsNeeded_t::tomes() const
{
	int rval = 0;

	if (m_head == 0)
		rval += 495;
	if (m_body == 0)
		rval += 825;
	if (m_hands == 0)
		rval += 495;
	if (m_waist == 0)
		rval += 375;
	if (m_legs == 0)
		rval += 825;
	if (m_feet == 0)
		rval += 495;

	rval += ((4 - m_acc) * 375);

	return rval;
}

PartsNeeded_t PartsNeeded_t::minusHead() const
{
	return PartsNeeded_t(m_head > 0 ? m_head - 1 : 0, m_body, m_hands, m_waist, m_legs, m_feet, m_acc);
}

PartsNeeded_t PartsNeeded_t::minusBody() const
{
	return PartsNeeded_t(m_head, m_body > 0 ? m_body - 1 : 0, m_hands, m_waist, m_legs, m_feet, m_acc);
}

PartsNeeded_t PartsNeeded_t::minusHands() const
{
	return PartsNeeded_t(m_head, m_body, m_hands > 0 ? m_hands - 1 : 0, m_waist, m_legs, m_feet, m_acc);
}

PartsNeeded_t PartsNeeded_t::minusWaist() const
{
	return PartsNeeded_t(m_head, m_body, m_hands, m_waist > 0 ? m_waist - 1 : 0, m_legs, m_feet, m_acc);
}

PartsNeeded_t PartsNeeded_t::minusLegs() const
{
	return PartsNeeded_t(m_head, m_body, m_hands, m_waist, m_legs > 0 ? m_legs - 1 : 0, m_feet, m_acc);
}

PartsNeeded_t PartsNeeded_t::minusFeet() const
{
	return PartsNeeded_t(m_head, m_body, m_hands, m_waist, m_legs, m_feet > 0 ? m_feet - 1 : 0, m_acc);
}

PartsNeeded_t PartsNeeded_t::minusAcc() const
{
	return PartsNeeded_t(m_head, m_body, m_hands, m_waist, m_legs, m_feet, m_acc > 0 ? m_acc - 1 : 0);
}

PartsNeeded_t PartsNeeded_t::addHead() const
{
	return PartsNeeded_t(m_head < 2 ? m_head + 1 : 2, m_body, m_hands, m_waist, m_legs, m_feet, m_acc);
}

PartsNeeded_t PartsNeeded_t::addBody() const
{
	return PartsNeeded_t(m_head, m_body < 4 ? m_body + 1 : 4, m_hands, m_waist, m_legs, m_feet, m_acc);
}

PartsNeeded_t PartsNeeded_t::addHands() const
{
	return PartsNeeded_t(m_head, m_body, m_hands < 2 ? m_hands + 1 : 2, m_waist, m_legs, m_feet, m_acc);
}

PartsNeeded_t PartsNeeded_t::addWaist() const
{
	return PartsNeeded_t(m_head, m_body, m_hands, m_waist < 1 ? m_waist + 1 : 1, m_legs, m_feet, m_acc);
}

PartsNeeded_t PartsNeeded_t::addLegs() const
{
	return PartsNeeded_t(m_head, m_body, m_hands, m_waist, m_legs < 4 ? m_legs + 1 : 4, m_feet, m_acc);
}

PartsNeeded_t PartsNeeded_t::addFeet() const
{
	return PartsNeeded_t(m_head, m_body, m_hands, m_waist, m_legs, m_feet < 2 ? m_feet + 1 : 2, m_acc);
}

PartsNeeded_t PartsNeeded_t::addAcc() const
{
	return PartsNeeded_t(m_head, m_body, m_hands, m_waist, m_legs, m_feet, m_acc < 4 ? m_acc + 1 : 4);
}
