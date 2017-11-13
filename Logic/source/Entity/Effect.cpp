#include "Entity/Effect.h"
#include <Engine\newd.h>

using namespace Logic;

Effect::Effect() {
	m_standards = nullptr;
	m_modifiers = nullptr;
	m_specifics = nullptr;
}

Effect::Effect(Standards const &standards)
{
	m_standards = newd Standards(standards);
	m_modifiers = nullptr;
	m_specifics = nullptr;
}

Effect::Effect(Effect const &other) 
{
	if (other.getStandards()) setStandards(*other.getStandards());
	if (other.getModifiers()) setModifiers(*other.getModifiers());
	if (other.getSpecifics()) setSpecifics(*other.getSpecifics());
}

Effect* Effect::operator=(Effect const &other) 
{
	if (other.getStandards()) setStandards(*other.getStandards());
	if (other.getModifiers()) setModifiers(*other.getModifiers());
	if (other.getSpecifics()) setSpecifics(*other.getSpecifics());

	return this;
}

Effect::~Effect() 
{
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

void Effect::setStandards(Standards const &standards)
{
	if (m_standards) delete m_standards;
	m_standards = newd Standards(standards);
}

void Effect::setModifiers(Modifiers const &modifiers)
{
	if (m_modifiers) delete m_modifiers;
	m_modifiers = newd Modifiers(modifiers);
}

void Effect::setSpecifics(Specifics const &specifics)
{
	if (m_specifics) delete m_specifics;
	m_specifics = newd Specifics(specifics);
}