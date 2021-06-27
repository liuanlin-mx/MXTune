#include "PluginParameter.h"

PluginParameter::PluginParameter(const String& name, float def_value, float min, float max, bool is_boolean)
    : _name(name)
    , _def_value(def_value)
    , _min(min)
    , _max(max)
    , _is_boolean(is_boolean)
    , _value(def_value)
{
}

PluginParameter::~PluginParameter()
{
}

float PluginParameter::getValue() const
{
    return _value;
}

void PluginParameter::setValue(float newValue)
{
    _value = newValue;
    if (_value > _max)
    {
        _value = _max;
    }
    if (_value < _min)
    {
        _value = _min;
    }
}

float PluginParameter::getDefaultValue() const
{
    return _def_value;
}

String PluginParameter::getName(int maximumStringLength) const
{
    return _name.substring(0, maximumStringLength);
}

String PluginParameter::getLabel() const
{
    return _name;
}

float PluginParameter::getValueForText(const String& text) const
{
    return text.getFloatValue();
}

bool PluginParameter::isBoolean() const
{
    return _is_boolean;
}
