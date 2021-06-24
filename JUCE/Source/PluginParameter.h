#pragma once

#include <JuceHeader.h>
class PluginParameter: public AudioProcessorParameter
{
public:
    PluginParameter(const String& name, float def_value, float min, float max, bool is_boolean);
    virtual ~PluginParameter();
    
public:
    virtual float getValue() const;
    virtual void setValue (float newValue);
    virtual float getDefaultValue() const;
    virtual String getName (int maximumStringLength) const;
    virtual String getLabel() const;
    virtual float getValueForText (const String& text) const;
    
    virtual bool isBoolean() const;
    
private:
    String _name;
    float _def_value;
    float _min;
    float _max;
    bool _is_boolean;
    float _value;
};
