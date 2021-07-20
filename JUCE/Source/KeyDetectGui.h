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
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class KeyDetectGui  : public Component,
                      public Button::Listener
{
public:
    //==============================================================================
    KeyDetectGui (const float *notes_weights, std::int32_t *notes, std::int32_t *key);
    ~KeyDetectGui() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void _update_key();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    enum
    {
        _key_A = 0,
        _key_Bb = 1,
        _key_B = 2,
        _key_C = 3,
        _key_Db = 4,
        _key_D = 5,
        _key_Eb = 6,
        _key_E = 7,
        _key_F = 8,
        _key_Gb = 9,
        _key_G = 10,
        _key_Ab = 11,
        _key_custom = 12,
    };

    std::int32_t *_o_notes;
    std::int32_t *_o_key;
    std::int32_t _notes[12];
    std::int32_t _key;
    //[/UserVariables]

    //==============================================================================
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
    std::unique_ptr<TextButton> textButton;
    std::unique_ptr<Label> labelNoteA;
    std::unique_ptr<Label> labelNoteBb;
    std::unique_ptr<Label> labelNoteB;
    std::unique_ptr<Label> labelNoteC;
    std::unique_ptr<Label> labelNoteDb;
    std::unique_ptr<Label> labelNoteD;
    std::unique_ptr<Label> labelNoteEb;
    std::unique_ptr<Label> labelNoteE;
    std::unique_ptr<Label> labelNoteF;
    std::unique_ptr<Label> labelNoteGb;
    std::unique_ptr<Label> labelNoteG;
    std::unique_ptr<Label> labelNoteAb;
    std::unique_ptr<Label> labelKey;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyDetectGui)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

