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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "KeyDetectGui.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
KeyDetectGui::KeyDetectGui (const float *notes_weights, std::int32_t *notes, std::int32_t *key)
    : _o_notes(notes), _o_key(key)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    toggleButtonNoteA.reset (new ToggleButton ("A"));
    addAndMakeVisible (toggleButtonNoteA.get());
    toggleButtonNoteA->addListener (this);
    toggleButtonNoteA->setToggleState (true, dontSendNotification);

    toggleButtonNoteA->setBounds (464, 112, 40, 24);

    toggleButtonNoteBb.reset (new ToggleButton ("Bb"));
    addAndMakeVisible (toggleButtonNoteBb.get());
    toggleButtonNoteBb->addListener (this);
    toggleButtonNoteBb->setToggleState (true, dontSendNotification);

    toggleButtonNoteBb->setBounds (512, 80, 40, 24);

    toggleButtonNoteB.reset (new ToggleButton ("B"));
    addAndMakeVisible (toggleButtonNoteB.get());
    toggleButtonNoteB->addListener (this);
    toggleButtonNoteB->setToggleState (true, dontSendNotification);

    toggleButtonNoteB->setBounds (560, 112, 40, 24);

    toggleButtonNoteC.reset (new ToggleButton ("C"));
    addAndMakeVisible (toggleButtonNoteC.get());
    toggleButtonNoteC->addListener (this);
    toggleButtonNoteC->setToggleState (true, dontSendNotification);

    toggleButtonNoteC->setBounds (32, 112, 40, 24);

    toggleButtonNoteDb.reset (new ToggleButton ("Db"));
    addAndMakeVisible (toggleButtonNoteDb.get());
    toggleButtonNoteDb->addListener (this);
    toggleButtonNoteDb->setToggleState (true, dontSendNotification);

    toggleButtonNoteDb->setBounds (80, 80, 40, 24);

    toggleButtonNoteD.reset (new ToggleButton ("D"));
    addAndMakeVisible (toggleButtonNoteD.get());
    toggleButtonNoteD->addListener (this);
    toggleButtonNoteD->setToggleState (true, dontSendNotification);

    toggleButtonNoteD->setBounds (128, 112, 40, 24);

    toggleButtonNoteEb.reset (new ToggleButton ("Eb"));
    addAndMakeVisible (toggleButtonNoteEb.get());
    toggleButtonNoteEb->addListener (this);
    toggleButtonNoteEb->setToggleState (true, dontSendNotification);

    toggleButtonNoteEb->setBounds (176, 80, 40, 24);

    toggleButtonNoteE.reset (new ToggleButton ("E"));
    addAndMakeVisible (toggleButtonNoteE.get());
    toggleButtonNoteE->addListener (this);
    toggleButtonNoteE->setToggleState (true, dontSendNotification);

    toggleButtonNoteE->setBounds (224, 112, 40, 24);

    toggleButtonNoteF.reset (new ToggleButton ("F"));
    addAndMakeVisible (toggleButtonNoteF.get());
    toggleButtonNoteF->addListener (this);
    toggleButtonNoteF->setToggleState (true, dontSendNotification);

    toggleButtonNoteF->setBounds (272, 112, 40, 24);

    toggleButtonNoteGb.reset (new ToggleButton ("Gb"));
    addAndMakeVisible (toggleButtonNoteGb.get());
    toggleButtonNoteGb->addListener (this);
    toggleButtonNoteGb->setToggleState (true, dontSendNotification);

    toggleButtonNoteGb->setBounds (320, 80, 40, 24);

    toggleButtonNoteAb.reset (new ToggleButton ("Ab"));
    addAndMakeVisible (toggleButtonNoteAb.get());
    toggleButtonNoteAb->addListener (this);
    toggleButtonNoteAb->setToggleState (true, dontSendNotification);

    toggleButtonNoteAb->setBounds (416, 80, 40, 24);

    toggleButtonNoteG.reset (new ToggleButton ("G"));
    addAndMakeVisible (toggleButtonNoteG.get());
    toggleButtonNoteG->addListener (this);
    toggleButtonNoteG->setToggleState (true, dontSendNotification);

    toggleButtonNoteG->setBounds (368, 112, 40, 24);

    textButton.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton.get());
    textButton->setButtonText (TRANS("OK"));
    textButton->addListener (this);
    textButton->setColour (TextButton::buttonColourId, Colour (0x00a45c94));

    textButton->setBounds (240, 200, 150, 24);

    labelNoteA.reset (new Label ("new label",
                                 TRANS("100%")));
    addAndMakeVisible (labelNoteA.get());
    labelNoteA->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelNoteA->setJustificationType (Justification::centredLeft);
    labelNoteA->setEditable (false, false, false);
    labelNoteA->setColour (TextEditor::textColourId, Colours::black);
    labelNoteA->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelNoteA->setBounds (464, 152, 40, 24);

    labelNoteBb.reset (new Label ("new label",
                                  TRANS("100%")));
    addAndMakeVisible (labelNoteBb.get());
    labelNoteBb->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelNoteBb->setJustificationType (Justification::centredLeft);
    labelNoteBb->setEditable (false, false, false);
    labelNoteBb->setColour (TextEditor::textColourId, Colours::black);
    labelNoteBb->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelNoteBb->setBounds (512, 152, 40, 24);

    labelNoteB.reset (new Label ("new label",
                                 TRANS("100%")));
    addAndMakeVisible (labelNoteB.get());
    labelNoteB->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelNoteB->setJustificationType (Justification::centredLeft);
    labelNoteB->setEditable (false, false, false);
    labelNoteB->setColour (TextEditor::textColourId, Colours::black);
    labelNoteB->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelNoteB->setBounds (560, 152, 40, 24);

    labelNoteC.reset (new Label ("new label",
                                 TRANS("100%")));
    addAndMakeVisible (labelNoteC.get());
    labelNoteC->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelNoteC->setJustificationType (Justification::centredLeft);
    labelNoteC->setEditable (false, false, false);
    labelNoteC->setColour (TextEditor::textColourId, Colours::black);
    labelNoteC->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelNoteC->setBounds (32, 152, 40, 24);

    labelNoteDb.reset (new Label ("new label",
                                  TRANS("100%")));
    addAndMakeVisible (labelNoteDb.get());
    labelNoteDb->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelNoteDb->setJustificationType (Justification::centredLeft);
    labelNoteDb->setEditable (false, false, false);
    labelNoteDb->setColour (TextEditor::textColourId, Colours::black);
    labelNoteDb->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelNoteDb->setBounds (80, 152, 40, 24);

    labelNoteD.reset (new Label ("new label",
                                 TRANS("100%")));
    addAndMakeVisible (labelNoteD.get());
    labelNoteD->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelNoteD->setJustificationType (Justification::centredLeft);
    labelNoteD->setEditable (false, false, false);
    labelNoteD->setColour (TextEditor::textColourId, Colours::black);
    labelNoteD->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelNoteD->setBounds (128, 152, 40, 24);

    labelNoteEb.reset (new Label ("new label",
                                  TRANS("100%")));
    addAndMakeVisible (labelNoteEb.get());
    labelNoteEb->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelNoteEb->setJustificationType (Justification::centredLeft);
    labelNoteEb->setEditable (false, false, false);
    labelNoteEb->setColour (TextEditor::textColourId, Colours::black);
    labelNoteEb->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelNoteEb->setBounds (176, 152, 40, 24);

    labelNoteE.reset (new Label ("new label",
                                 TRANS("100%")));
    addAndMakeVisible (labelNoteE.get());
    labelNoteE->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelNoteE->setJustificationType (Justification::centredLeft);
    labelNoteE->setEditable (false, false, false);
    labelNoteE->setColour (TextEditor::textColourId, Colours::black);
    labelNoteE->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelNoteE->setBounds (224, 152, 40, 24);

    labelNoteF.reset (new Label ("new label",
                                 TRANS("100%")));
    addAndMakeVisible (labelNoteF.get());
    labelNoteF->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelNoteF->setJustificationType (Justification::centredLeft);
    labelNoteF->setEditable (false, false, false);
    labelNoteF->setColour (TextEditor::textColourId, Colours::black);
    labelNoteF->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelNoteF->setBounds (272, 152, 40, 24);

    labelNoteGb.reset (new Label ("new label",
                                  TRANS("100%")));
    addAndMakeVisible (labelNoteGb.get());
    labelNoteGb->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelNoteGb->setJustificationType (Justification::centredLeft);
    labelNoteGb->setEditable (false, false, false);
    labelNoteGb->setColour (TextEditor::textColourId, Colours::black);
    labelNoteGb->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelNoteGb->setBounds (320, 152, 40, 24);

    labelNoteG.reset (new Label ("new label",
                                 TRANS("100%")));
    addAndMakeVisible (labelNoteG.get());
    labelNoteG->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelNoteG->setJustificationType (Justification::centredLeft);
    labelNoteG->setEditable (false, false, false);
    labelNoteG->setColour (TextEditor::textColourId, Colours::black);
    labelNoteG->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelNoteG->setBounds (368, 152, 40, 24);

    labelNoteAb.reset (new Label ("new label",
                                  TRANS("100%")));
    addAndMakeVisible (labelNoteAb.get());
    labelNoteAb->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelNoteAb->setJustificationType (Justification::centredLeft);
    labelNoteAb->setEditable (false, false, false);
    labelNoteAb->setColour (TextEditor::textColourId, Colours::black);
    labelNoteAb->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelNoteAb->setBounds (416, 152, 40, 24);

    labelKey.reset (new Label ("new label",
                               TRANS("1=C")));
    addAndMakeVisible (labelKey.get());
    labelKey->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelKey->setJustificationType (Justification::centredLeft);
    labelKey->setEditable (false, false, false);
    labelKey->setColour (TextEditor::textColourId, Colours::black);
    labelKey->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelKey->setBounds (296, 32, 56, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (640, 240);


    //[Constructor] You can add your own custom stuff here..


    float notes_weights_[12];
    for (std::int32_t i = 0; i < 12; i++)
    {
        notes_weights_[i] = notes_weights[i];
        _notes[i] = -1;
    }
    _key = _key_custom;


    for (std::int32_t i = 0; i < 7; i++)
    {
        float max = 0.;

        std::int32_t max_idx = -1;
        for (std::int32_t j = 0; j < 12; j++)
        {
            if (notes_weights_[j] > max)
            {
                max = notes_weights_[j];
                max_idx = j;
            }
        }

        if (max_idx < 0)
        {
            break;
        }
        notes_weights_[max_idx] = 0.;
        _notes[max_idx] = 1;
    }

    toggleButtonNoteA->setToggleState((_notes[_key_A] > 0), dontSendNotification);
    toggleButtonNoteBb->setToggleState((_notes[_key_Bb] > 0), dontSendNotification);
    toggleButtonNoteB->setToggleState((_notes[_key_B] > 0), dontSendNotification);
    toggleButtonNoteC->setToggleState((_notes[_key_C] > 0), dontSendNotification);
    toggleButtonNoteDb->setToggleState((_notes[_key_Db] > 0), dontSendNotification);
    toggleButtonNoteD->setToggleState((_notes[_key_D] > 0), dontSendNotification);
    toggleButtonNoteEb->setToggleState((_notes[_key_Eb] > 0), dontSendNotification);
    toggleButtonNoteE->setToggleState((_notes[_key_E] > 0), dontSendNotification);
    toggleButtonNoteF->setToggleState((_notes[_key_F] > 0), dontSendNotification);
    toggleButtonNoteGb->setToggleState((_notes[_key_Gb] > 0), dontSendNotification);
    toggleButtonNoteG->setToggleState((_notes[_key_G] > 0), dontSendNotification);
    toggleButtonNoteAb->setToggleState((_notes[_key_Ab] > 0), dontSendNotification);

    labelNoteA->setText(juce::String::formatted("%02d%%", (std::uint32_t)(100 * notes_weights[_key_A])), dontSendNotification);
    labelNoteBb->setText(juce::String::formatted("%02d%%", (std::uint32_t)(100 * notes_weights[_key_Bb])), dontSendNotification);
    labelNoteB->setText(juce::String::formatted("%02d%%", (std::uint32_t)(100 * notes_weights[_key_B])), dontSendNotification);
    labelNoteC->setText(juce::String::formatted("%02d%%", (std::uint32_t)(100 * notes_weights[_key_C])), dontSendNotification);
    labelNoteDb->setText(juce::String::formatted("%02d%%", (std::uint32_t)(100 * notes_weights[_key_Db])), dontSendNotification);
    labelNoteD->setText(juce::String::formatted("%02d%%", (std::uint32_t)(100 * notes_weights[_key_D])), dontSendNotification);
    labelNoteEb->setText(juce::String::formatted("%02d%%", (std::uint32_t)(100 * notes_weights[_key_Eb])), dontSendNotification);
    labelNoteE->setText(juce::String::formatted("%02d%%", (std::uint32_t)(100 * notes_weights[_key_E])), dontSendNotification);
    labelNoteF->setText(juce::String::formatted("%02d%%", (std::uint32_t)(100 * notes_weights[_key_F])), dontSendNotification);
    labelNoteGb->setText(juce::String::formatted("%02d%%", (std::uint32_t)(100 * notes_weights[_key_Gb])), dontSendNotification);
    labelNoteG->setText(juce::String::formatted("%02d%%", (std::uint32_t)(100 * notes_weights[_key_G])), dontSendNotification);
    labelNoteAb->setText(juce::String::formatted("%02d%%", (std::uint32_t)(100 * notes_weights[_key_Ab])), dontSendNotification);

    _update_key();
    //[/Constructor]
}

KeyDetectGui::~KeyDetectGui()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    toggleButtonNoteA = nullptr;
    toggleButtonNoteBb = nullptr;
    toggleButtonNoteB = nullptr;
    toggleButtonNoteC = nullptr;
    toggleButtonNoteDb = nullptr;
    toggleButtonNoteD = nullptr;
    toggleButtonNoteEb = nullptr;
    toggleButtonNoteE = nullptr;
    toggleButtonNoteF = nullptr;
    toggleButtonNoteGb = nullptr;
    toggleButtonNoteAb = nullptr;
    toggleButtonNoteG = nullptr;
    textButton = nullptr;
    labelNoteA = nullptr;
    labelNoteBb = nullptr;
    labelNoteB = nullptr;
    labelNoteC = nullptr;
    labelNoteDb = nullptr;
    labelNoteD = nullptr;
    labelNoteEb = nullptr;
    labelNoteE = nullptr;
    labelNoteF = nullptr;
    labelNoteGb = nullptr;
    labelNoteG = nullptr;
    labelNoteAb = nullptr;
    labelKey = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void KeyDetectGui::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void KeyDetectGui::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void KeyDetectGui::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleButtonNoteA.get())
    {
        //[UserButtonCode_toggleButtonNoteA] -- add your button handler code here..
        _notes[_key_A] = buttonThatWasClicked->getToggleState()? 1: -1;
        _update_key();
        //[/UserButtonCode_toggleButtonNoteA]
    }
    else if (buttonThatWasClicked == toggleButtonNoteBb.get())
    {
        //[UserButtonCode_toggleButtonNoteBb] -- add your button handler code here..
        _notes[_key_Bb] = buttonThatWasClicked->getToggleState()? 1: -1;
        _update_key();
        //[/UserButtonCode_toggleButtonNoteBb]
    }
    else if (buttonThatWasClicked == toggleButtonNoteB.get())
    {
        //[UserButtonCode_toggleButtonNoteB] -- add your button handler code here..
        _notes[_key_B] = buttonThatWasClicked->getToggleState()? 1: -1;
        _update_key();
        //[/UserButtonCode_toggleButtonNoteB]
    }
    else if (buttonThatWasClicked == toggleButtonNoteC.get())
    {
        //[UserButtonCode_toggleButtonNoteC] -- add your button handler code here..
        _notes[_key_C] = buttonThatWasClicked->getToggleState()? 1: -1;
        _update_key();
        //[/UserButtonCode_toggleButtonNoteC]
    }
    else if (buttonThatWasClicked == toggleButtonNoteDb.get())
    {
        //[UserButtonCode_toggleButtonNoteDb] -- add your button handler code here..
        _notes[_key_Db] = buttonThatWasClicked->getToggleState()? 1: -1;
        _update_key();
        //[/UserButtonCode_toggleButtonNoteDb]
    }
    else if (buttonThatWasClicked == toggleButtonNoteD.get())
    {
        //[UserButtonCode_toggleButtonNoteD] -- add your button handler code here..
        _notes[_key_D] = buttonThatWasClicked->getToggleState()? 1: -1;
        _update_key();
        //[/UserButtonCode_toggleButtonNoteD]
    }
    else if (buttonThatWasClicked == toggleButtonNoteEb.get())
    {
        //[UserButtonCode_toggleButtonNoteEb] -- add your button handler code here..
        _notes[_key_Eb] = buttonThatWasClicked->getToggleState()? 1: -1;
        _update_key();
        //[/UserButtonCode_toggleButtonNoteEb]
    }
    else if (buttonThatWasClicked == toggleButtonNoteE.get())
    {
        //[UserButtonCode_toggleButtonNoteE] -- add your button handler code here..
        _notes[_key_E] = buttonThatWasClicked->getToggleState()? 1: -1;
        _update_key();
        //[/UserButtonCode_toggleButtonNoteE]
    }
    else if (buttonThatWasClicked == toggleButtonNoteF.get())
    {
        //[UserButtonCode_toggleButtonNoteF] -- add your button handler code here..
        _notes[_key_F] = buttonThatWasClicked->getToggleState()? 1: -1;
        _update_key();
        //[/UserButtonCode_toggleButtonNoteF]
    }
    else if (buttonThatWasClicked == toggleButtonNoteGb.get())
    {
        //[UserButtonCode_toggleButtonNoteGb] -- add your button handler code here..
        _notes[_key_Gb] = buttonThatWasClicked->getToggleState()? 1: -1;
        _update_key();
        //[/UserButtonCode_toggleButtonNoteGb]
    }
    else if (buttonThatWasClicked == toggleButtonNoteAb.get())
    {
        //[UserButtonCode_toggleButtonNoteAb] -- add your button handler code here..
        _notes[_key_Ab] = buttonThatWasClicked->getToggleState()? 1: -1;
        _update_key();
        //[/UserButtonCode_toggleButtonNoteAb]
    }
    else if (buttonThatWasClicked == toggleButtonNoteG.get())
    {
        //[UserButtonCode_toggleButtonNoteG] -- add your button handler code here..
        _notes[_key_G] = buttonThatWasClicked->getToggleState()? 1: -1;
        _update_key();
        //[/UserButtonCode_toggleButtonNoteG]
    }
    else if (buttonThatWasClicked == textButton.get())
    {
        //[UserButtonCode_textButton] -- add your button handler code here..

        for (std::int32_t i = 0; i < 12; i++)
        {
            _o_notes[i] = _notes[i];
        }
        *_o_key = _key;
        if (DialogWindow* dw = findParentComponentOfClass<DialogWindow>())
        {
            dw->exitModalState(1);
        }

        //[/UserButtonCode_textButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void KeyDetectGui::_update_key()
{
    _key = _key_custom;
    std::int32_t major[12] = {1, -1, 1, -1, 1, 1, -1, 1, -1, 1, -1, 1};
    const char *notes_name[12] = {"A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab"};
    
    for (std::int32_t i = 0; i < 12; i++)
    {
        bool found = true;
        for (std::uint32_t j = 0; j < 12; j++)
        {
            if (_notes[(j + i) % 12] != major[j])
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            _key = i;
        }
    }

    if (_key == _key_custom)
    {
        labelKey->setText("----", dontSendNotification);
    }
    else
    {
        char str[32] = {0};
        sprintf(str, "1=%s", notes_name[_key]);
        labelKey->setText(str, dontSendNotification);
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="KeyDetectGui" componentName=""
                 parentClasses="public Component" constructorParams="const float *notes_weights, std::int32_t *notes, std::int32_t *key"
                 variableInitialisers="_o_notes(notes), _o_key(key)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="1"
                 initialWidth="640" initialHeight="240">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TOGGLEBUTTON name="A" id="379b4bb7d2fbcace" memberName="toggleButtonNoteA"
                virtualName="" explicitFocusOrder="0" pos="464 112 40 24" buttonText="A"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="Bb" id="c5a5631674e84c6d" memberName="toggleButtonNoteBb"
                virtualName="" explicitFocusOrder="0" pos="512 80 40 24" buttonText="Bb"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="B" id="118f5e02b9feb93a" memberName="toggleButtonNoteB"
                virtualName="" explicitFocusOrder="0" pos="560 112 40 24" buttonText="B"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="C" id="64fa9722fea3f930" memberName="toggleButtonNoteC"
                virtualName="" explicitFocusOrder="0" pos="32 112 40 24" buttonText="C"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="Db" id="fe20e3be7521d3e5" memberName="toggleButtonNoteDb"
                virtualName="" explicitFocusOrder="0" pos="80 80 40 24" buttonText="Db"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="D" id="6c379b8e283b6172" memberName="toggleButtonNoteD"
                virtualName="" explicitFocusOrder="0" pos="128 112 40 24" buttonText="D"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="Eb" id="4f9effd6fff17c8c" memberName="toggleButtonNoteEb"
                virtualName="" explicitFocusOrder="0" pos="176 80 40 24" buttonText="Eb"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="E" id="b85394a33c50f2b4" memberName="toggleButtonNoteE"
                virtualName="" explicitFocusOrder="0" pos="224 112 40 24" buttonText="E"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="F" id="99e08323176a0a04" memberName="toggleButtonNoteF"
                virtualName="" explicitFocusOrder="0" pos="272 112 40 24" buttonText="F"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="Gb" id="4de424495467f77f" memberName="toggleButtonNoteGb"
                virtualName="" explicitFocusOrder="0" pos="320 80 40 24" buttonText="Gb"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="Ab" id="b13f751ddb7ba042" memberName="toggleButtonNoteAb"
                virtualName="" explicitFocusOrder="0" pos="416 80 40 24" buttonText="Ab"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="G" id="6d2077e61f10a098" memberName="toggleButtonNoteG"
                virtualName="" explicitFocusOrder="0" pos="368 112 40 24" buttonText="G"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TEXTBUTTON name="new button" id="c900a2a68ae1c36c" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="240 200 150 24" bgColOff="a45c94"
              buttonText="OK" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="cab9e3d8501a18e7" memberName="labelNoteA"
         virtualName="" explicitFocusOrder="0" pos="464 152 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="100%" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="26ad32d337b3cbd5" memberName="labelNoteBb"
         virtualName="" explicitFocusOrder="0" pos="512 152 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="100%" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="5a201c1da1bf4f0d" memberName="labelNoteB"
         virtualName="" explicitFocusOrder="0" pos="560 152 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="100%" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="25d75d1ad749e1d6" memberName="labelNoteC"
         virtualName="" explicitFocusOrder="0" pos="32 152 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="100%" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="418815d4846b032f" memberName="labelNoteDb"
         virtualName="" explicitFocusOrder="0" pos="80 152 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="100%" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="691b329803ff2fed" memberName="labelNoteD"
         virtualName="" explicitFocusOrder="0" pos="128 152 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="100%" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="d3b7a1305ebd41e2" memberName="labelNoteEb"
         virtualName="" explicitFocusOrder="0" pos="176 152 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="100%" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="3e6a56a1948c1863" memberName="labelNoteE"
         virtualName="" explicitFocusOrder="0" pos="224 152 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="100%" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="361e255740201d3a" memberName="labelNoteF"
         virtualName="" explicitFocusOrder="0" pos="272 152 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="100%" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="91d0ac2a4d59eb7" memberName="labelNoteGb"
         virtualName="" explicitFocusOrder="0" pos="320 152 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="100%" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="6e8980693a735028" memberName="labelNoteG"
         virtualName="" explicitFocusOrder="0" pos="368 152 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="100%" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="bf554ac203dabbe6" memberName="labelNoteAb"
         virtualName="" explicitFocusOrder="0" pos="416 152 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="100%" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="a488b58e8ff6802e" memberName="labelKey"
         virtualName="" explicitFocusOrder="0" pos="296 32 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="1=C" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

