#include "Entity/Effect.h"

using namespace Logic;

Effect::Effect(Standards const &standards)
{
	m_stack = 0;
	m_standards = new Standards(standards);
}

Effect::Effect(Effect const &other) {
	m_stack = 0;

	if (m_standards) delete m_standards;
	if (m_modifiers) delete m_modifiers;
	if (m_specifics) delete m_specifics;

	setStandards(*other.getStandards());
	setModifiers(*other.getModifiers());
	setSpecifics(*other.getSpecifics());
}

Effect::Effect()
{
}

Effect::~Effect() {
	if (m_standards) delete m_standards;
	if (m_modifiers) delete m_modifiers;
	if (m_specifics) delete m_specifics;
}

Effect::Standards* Effect::getStandards() const
{
	return m_standards;
}

Effect::Modifiers* Effect::getModifiers() const
{
	return m_modifiers;
}

Effect::Specifics* Effect::getSpecifics() const
{
	return m_specifics;
}

void Effect::setStandards(Standards const & standards)
{
	if (m_standards) delete m_standards;
	m_standards = new Standards(standards);
}

void Effect::setModifiers(Modifiers const & modifiers)
{
	if (m_modifiers) delete m_modifiers;
	m_modifiers = new Modifiers(modifiers);
}

void Effect::setSpecifics(Specifics const &specifics)
{
	if (m_specifics) delete m_specifics;
	m_specifics = new Specifics(specifics);
}

int Effect::getStack() const
{
	return m_stack;
}

void Effect::setStack(int stack)
{
	m_stack = stack;
}

void Effect::pop() {
	--m_stack;
}