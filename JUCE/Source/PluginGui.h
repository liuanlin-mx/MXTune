/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "PluginProcessor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PluginGui  : public AudioProcessorEditor,
                   public Timer,
                   public Slider::Listener,
                   public Button::Listener,
                   public ComboBox::Listener
{
public:
    //==============================================================================
    PluginGui (AutotalentAudioProcessor& p);
    ~PluginGui() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback() override;

    float _snap_pitch(float pitch);
    float _pitch_to_y(float pitch);
    float _y_to_pitch(float y);
    float _time_to_x(float time);
    float _x_to_time(float x);

    void _update_notes();

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel) override;
    bool keyPressed (const KeyPress& key) override;
    void modifierKeysChanged (const ModifierKeys& modifiers) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AutotalentAudioProcessor& _proc;
    std::int32_t _notes[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    std::shared_ptr<manual_tune::tune_node> _cur_node;

    bool _new_tune = false;
    bool _modify_tune = false;
    bool _snap_note = true;

    std::uint32_t _select_pos;
    float _select_xd;
    float _select_yd;

    bool _is_ctrl = false;
    bool _is_shift = false;
    bool _is_alt = false;

    int _draw_x = 0;
    int _draw_y = 0;
    int _draw_w = 0;
    int _draw_h = 0;
    float _margin = 24;

    float _pitch_max = 36.;
    float _pitch_min = -36.;
    float _pitch_up = 0 - 9 + 6;
    float _pitch_down = 0 - 9 - 6;

    float _time_min_len = 0.5;
    float _time_max_len = 10 * 60;
    float _time_left = 0.;
    float _time_right = 10.;

    float _cur_time = 5;

    float _def_attack = 0.100;
    float _def_release = 0.050;
    float _def_amount = 0.8;
    float _def_smooth = 1.0;

    enum
    {
        _key_custom = 12,
        _key_type_custom = 3,
    };
    std::int32_t _key;      //0:A 1:Bb .... 12:Ab 13:custom
    std::int32_t _key_type; //0:Major 1:Minor 2:Chromatic 3:custom
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<GroupComponent> groupComponent2;
    std::unique_ptr<GroupComponent> groupComponent4;
    std::unique_ptr<Slider> sliderAttack;
    std::unique_ptr<GroupComponent> groupComponent5;
    std::unique_ptr<GroupComponent> groupComponent;
    std::unique_ptr<ToggleButton> toggleButtonTrack;
    std::unique_ptr<ToggleButton> toggleButtonAutoTune;
    std::unique_ptr<ToggleButton> toggleButtonNoteA;
    std::unique_ptr<ToggleButton> toggleButtonNoteBb;
    std::unique_ptr<ToggleButton> toggleButtonNoteB;
    std::unique_ptr<ToggleButton> toggleButtonNoteC;
    std::unique_ptr<ToggleButton> toggleButtonNoteDb;
    std::unique_ptr<ToggleButton> toggleButtonNoteD;
    std::unique_ptr<ToggleButton> toggleButtonNoteEb;
    std::unique_ptr<ToggleButton> toggleButtonNoteE;
    std::unique_ptr<ToggleButton> toggleButtonNoteF;
    std::unique_ptr<ToggleButton> toggleButtonNoteGb;
    std::unique_ptr<ToggleButton> toggleButtonNoteAb;
    std::unique_ptr<ToggleButton> toggleButtonNoteG;
    std::unique_ptr<TextButton> textButtonSnapKey;
    std::unique_ptr<TextButton> textButtonClearPitch;
    std::unique_ptr<ComboBox> comboBoxKey;
    std::unique_ptr<ComboBox> comboBoxKeyType;
    std::unique_ptr<GroupComponent> groupComponent3;
    std::unique_ptr<Label> label;
    std::unique_ptr<Label> label2;
    std::unique_ptr<Label> label3;
    std::unique_ptr<Slider> sliderRelease;
    std::unique_ptr<Slider> sliderAmount;
    std::unique_ptr<Label> label4;
    std::unique_ptr<Label> label5;
    std::unique_ptr<Label> label6;
    std::unique_ptr<ToggleButton> toggleButtonSnap;
    std::unique_ptr<TextButton> textButtonClearNote;
    std::unique_ptr<Slider> sliderDefAttack;
    std::unique_ptr<Slider> sliderDefRelease;
    std::unique_ptr<Slider> sliderDefAmount;
    std::unique_ptr<Label> label7;
    std::unique_ptr<Slider> sliderDefSmooth;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginGui)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

