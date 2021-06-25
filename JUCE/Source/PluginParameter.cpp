#include "PluginParameter.h"
#include "net_log.h"

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
    //net_log_debug("name:%s _value:%f\n", _name.toRawUTF8(), _value);
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
    //net_log_debug("name:%s _value:%f\n", _name.toRawUTF8(), _value);
}

float PluginParameter::getDefaultValue() const
{
    //net_log_debug("name:%s _def_value:%f\n", _name.toRawUTF8(), _def_value);
    return _def_value;
}

String PluginParameter::getName(int maximumStringLength) const
{
    //net_log_debug("name:%s\n", _name);
    return _name.substring(0, maximumStringLength);
}

String PluginParameter::getLabel() const
{
    //net_log_debug("name:%s\n", _name);
    return _name;
}

float PluginParameter::getValueForText(const String& text) const
{
    //net_log_debug("name:%s\n", _name);
    return text.getFloatValue();
}

bool PluginParameter::isBoolean() const
{
    //net_log_debug("name:%s\n", _name);
    return _is_boolean;
}
