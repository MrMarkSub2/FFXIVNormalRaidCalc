#pragma once

class PartsNeeded_t {
public:
	PartsNeeded_t(int head, int body, int hands, int waist, int legs, int feet, int acc);
	bool operator< (const PartsNeeded_t& rhs) const;

	int head() const { return m_head; }
	int body() const { return m_body; }
	int hands() const { return m_hands; }
	int waist() const { return m_waist; }
	int legs() const { return m_legs; }
	int feet() const { return m_feet; }
	int acc() const { return m_acc; }

	int count() const { return head() + body() + hands() + waist() + legs() + feet() + acc(); }
	int tomes() const;

	PartsNeeded_t minusHead() const;
	PartsNeeded_t minusBody() const;
	PartsNeeded_t minusHands() const;
	PartsNeeded_t minusWaist() const;
	PartsNeeded_t minusLegs() const;
	PartsNeeded_t minusFeet() const;
	PartsNeeded_t minusAcc() const;

	PartsNeeded_t addHead() const;
	PartsNeeded_t addBody() const;
	PartsNeeded_t addHands() const;
	PartsNeeded_t addWaist() const;
	PartsNeeded_t addLegs() const;
	PartsNeeded_t addFeet() const;
	PartsNeeded_t addAcc() const;

private:
	int m_head;
	int m_body;
	int m_hands;
	int m_waist;
	int m_legs;
	int m_feet;
	int m_acc;
};