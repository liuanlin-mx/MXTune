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

#include "SettingGui.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SettingGui::SettingGui (AutotalentAudioProcessor& p)
    : _proc(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    groupComponent.reset (new GroupComponent ("new group",
                                              TRANS("Detect")));
    addAndMakeVisible (groupComponent.get());

    groupComponent->setBounds (8, 8, 584, 96);

    label9.reset (new Label ("Det alg",
                             TRANS("Det alg:")));
    addAndMakeVisible (label9.get());
    label9->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label9->setJustificationType (Justification::centredLeft);
    label9->setEditable (false, false, false);
    label9->setColour (TextEditor::textColourId, Colours::black);
    label9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label9->setBounds (16, 32, 56, 24);

    label10.reset (new Label ("Sft Alg",
                              TRANS("Sft Alg:")));
    addAndMakeVisible (label10.get());
    label10->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label10->setJustificationType (Justification::centredLeft);
    label10->setEditable (false, false, false);
    label10->setColour (TextEditor::textColourId, Colours::black);
    label10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label10->setBounds (16, 64, 56, 24);

    comboBoxDetAlg.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (comboBoxDetAlg.get());
    comboBoxDetAlg->setEditableText (false);
    comboBoxDetAlg->setJustificationType (Justification::centredLeft);
    comboBoxDetAlg->setTextWhenNothingSelected (TRANS("talent"));
    comboBoxDetAlg->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxDetAlg->addItem (TRANS("autotalent"), 1);
    comboBoxDetAlg->addItem (TRANS("aubio(yinfast)"), 2);
    comboBoxDetAlg->addListener (this);

    comboBoxDetAlg->setBounds (80, 32, 120, 24);

    comboBoxSftAlg.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (comboBoxSftAlg.get());
    comboBoxSftAlg->setEditableText (false);
    comboBoxSftAlg->setJustificationType (Justification::centredLeft);
    comboBoxSftAlg->setTextWhenNothingSelected (TRANS("talent"));
    comboBoxSftAlg->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxSftAlg->addItem (TRANS("autotalent"), 1);
    comboBoxSftAlg->addItem (TRANS("SoundTouch"), 2);
    comboBoxSftAlg->addItem (TRANS("rubberband"), 3);
    comboBoxSftAlg->addItem (TRANS("smbPitchShift"), 4);
    comboBoxSftAlg->addListener (this);

    comboBoxSftAlg->setBounds (80, 64, 120, 24);

    sliderMinFreq.reset (new Slider ("new slider"));
    addAndMakeVisible (sliderMinFreq.get());
    sliderMinFreq->setRange (20, 2000, 1);
    sliderMinFreq->setSliderStyle (Slider::LinearBar);
    sliderMinFreq->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
    sliderMinFreq->addListener (this);

    sliderMinFreq->setBounds (280, 32, 64, 24);

    label2.reset (new Label ("Min Freq",
                             TRANS("Min Freq:")));
    addAndMakeVisible (label2.get());
    label2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label2->setBounds (208, 32, 64, 24);

    sliderMaxFreq.reset (new Slider ("new slider"));
    addAndMakeVisible (sliderMaxFreq.get());
    sliderMaxFreq->setRange (20, 2000, 1);
    sliderMaxFreq->setSliderStyle (Slider::LinearBar);
    sliderMaxFreq->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
    sliderMaxFreq->addListener (this);

    sliderMaxFreq->setBounds (280, 64, 64, 24);

    label3.reset (new Label ("Max Freq",
                             TRANS("Max Freq:")));
    addAndMakeVisible (label3.get());
    label3->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label3->setBounds (208, 64, 64, 24);

    label4.reset (new Label ("Afreq",
                             TRANS("Afreq:")));
    addAndMakeVisible (label4.get());
    label4->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label4->setBounds (352, 32, 48, 24);

    sliderAfreq.reset (new Slider ("new slider"));
    addAndMakeVisible (sliderAfreq.get());
    sliderAfreq->setRange (430, 450, 1);
    sliderAfreq->setSliderStyle (Slider::LinearBar);
    sliderAfreq->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
    sliderAfreq->addListener (this);

    sliderAfreq->setBounds (408, 32, 64, 24);

    label5.reset (new Label ("Gate",
                             TRANS("Gate:")));
    addAndMakeVisible (label5.get());
    label5->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label5->setBounds (352, 64, 48, 24);

    sliderGate.reset (new Slider ("new slider"));
    addAndMakeVisible (sliderGate.get());
    sliderGate->setRange (-100, -10, 1);
    sliderGate->setSliderStyle (Slider::LinearBar);
    sliderGate->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
    sliderGate->addListener (this);

    sliderGate->setBounds (408, 64, 64, 24);

    groupComponent2.reset (new GroupComponent ("new group",
                                               TRANS("misc")));
    addAndMakeVisible (groupComponent2.get());

    groupComponent2->setBounds (8, 112, 584, 232);

    textButtonApply.reset (new TextButton ("Apply"));
    addAndMakeVisible (textButtonApply.get());
    textButtonApply->addListener (this);

    textButtonApply->setBounds (16, 352, 568, 40);

    textEditorMisc.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (textEditorMisc.get());
    textEditorMisc->setMultiLine (true);
    textEditorMisc->setReturnKeyStartsNewLine (true);
    textEditorMisc->setReadOnly (false);
    textEditorMisc->setScrollbarsShown (true);
    textEditorMisc->setCaretVisible (true);
    textEditorMisc->setPopupMenuEnabled (true);
    textEditorMisc->setText (String());

    textEditorMisc->setBounds (16, 128, 568, 208);

    label6.reset (new Label ("VThresh",
                             TRANS("VThresh:")));
    addAndMakeVisible (label6.get());
    label6->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label6->setBounds (480, 32, 56, 24);

    sliderVThresh.reset (new Slider ("new slider"));
    addAndMakeVisible (sliderVThresh.get());
    sliderVThresh->setRange (0.1, 1, 0.01);
    sliderVThresh->setSliderStyle (Slider::LinearBar);
    sliderVThresh->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
    sliderVThresh->addListener (this);

    sliderVThresh->setBounds (544, 32, 40, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    comboBoxDetAlg->setSelectedId(_proc.get_parameter(AutotalentAudioProcessor::PARAMETER_ID_DET_ALG) + 1, dontSendNotification);
    comboBoxSftAlg->setSelectedId(_proc.get_parameter(AutotalentAudioProcessor::PARAMETER_ID_SFT_ALG) + 1, dontSendNotification);

    sliderMinFreq->setValue(_proc.get_parameter(AutotalentAudioProcessor::PARAMETER_ID_DET_MIN_FREQ), dontSendNotification);
    sliderMaxFreq->setValue(_proc.get_parameter(AutotalentAudioProcessor::PARAMETER_ID_DET_MAX_FREQ), dontSendNotification);

    sliderAfreq->setValue(_proc.get_parameter(AutotalentAudioProcessor::PARAMETER_ID_AFREQ), dontSendNotification);
    sliderGate->setValue(-_proc.get_parameter(AutotalentAudioProcessor::PARAMETER_ID_DET_GATE), dontSendNotification);

    sliderVThresh->setValue(_proc.get_parameter(AutotalentAudioProcessor::PARAMETER_ID_VTHRESH), dontSendNotification);

    textEditorMisc->setText(_proc.get_misc_param(), false);
    //[/Constructor]
}

SettingGui::~SettingGui()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    groupComponent = nullptr;
    label9 = nullptr;
    label10 = nullptr;
    comboBoxDetAlg = nullptr;
    comboBoxSftAlg = nullptr;
    sliderMinFreq = nullptr;
    label2 = nullptr;
    sliderMaxFreq = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    sliderAfreq = nullptr;
    label5 = nullptr;
    sliderGate = nullptr;
    groupComponent2 = nullptr;
    textButtonApply = nullptr;
    textEditorMisc = nullptr;
    label6 = nullptr;
    sliderVThresh = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SettingGui::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SettingGui::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SettingGui::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxDetAlg.get())
    {
        //[UserComboBoxCode_comboBoxDetAlg] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBoxDetAlg]
    }
    else if (comboBoxThatHasChanged == comboBoxSftAlg.get())
    {
        //[UserComboBoxCode_comboBoxSftAlg] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBoxSftAlg]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void SettingGui::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderMinFreq.get())
    {
        //[UserSliderCode_sliderMinFreq] -- add your slider handling code here..
        //[/UserSliderCode_sliderMinFreq]
    }
    else if (sliderThatWasMoved == sliderMaxFreq.get())
    {
        //[UserSliderCode_sliderMaxFreq] -- add your slider handling code here..
        //[/UserSliderCode_sliderMaxFreq]
    }
    else if (sliderThatWasMoved == sliderAfreq.get())
    {
        //[UserSliderCode_sliderAfreq] -- add your slider handling code here..
        //[/UserSliderCode_sliderAfreq]
    }
    else if (sliderThatWasMoved == sliderGate.get())
    {
        //[UserSliderCode_sliderGate] -- add your slider handling code here..
        //[/UserSliderCode_sliderGate]
    }
    else if (sliderThatWasMoved == sliderVThresh.get())
    {
        //[UserSliderCode_sliderVThresh] -- add your slider handling code here..
        //[/UserSliderCode_sliderVThresh]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SettingGui::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButtonApply.get())
    {
        //[UserButtonCode_textButtonApply] -- add your button handler code here..

        _proc.set_parameter(AutotalentAudioProcessor::PARAMETER_ID_DET_ALG, comboBoxDetAlg->getSelectedId() - 1);
        _proc.set_parameter(AutotalentAudioProcessor::PARAMETER_ID_SFT_ALG, comboBoxSftAlg->getSelectedId() - 1);

        _proc.set_parameter(AutotalentAudioProcessor::PARAMETER_ID_DET_MIN_FREQ, sliderMinFreq->getValue());
        _proc.set_parameter(AutotalentAudioProcessor::PARAMETER_ID_DET_MAX_FREQ, sliderMaxFreq->getValue());

        _proc.set_parameter(AutotalentAudioProcessor::PARAMETER_ID_AFREQ, sliderAfreq->getValue());
        _proc.set_parameter(AutotalentAudioProcessor::PARAMETER_ID_DET_GATE, -sliderGate->getValue());

        _proc.set_parameter(AutotalentAudioProcessor::PARAMETER_ID_VTHRESH, sliderVThresh->getValue());

        _proc.set_misc_param(textEditorMisc->getText().toStdString());
        _proc.report_latency_samples();
        //[/UserButtonCode_textButtonApply]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SettingGui" componentName=""
                 parentClasses="public Component" constructorParams="AutotalentAudioProcessor&amp; p"
                 variableInitialisers="_proc(p)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="new group" id="d3b1a0e26d71dacc" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 8 584 96" title="Detect"/>
  <LABEL name="Det alg" id="654498e4880c91f0" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="16 32 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Det alg:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Sft Alg" id="8377d5581a9249ca" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="16 64 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Sft Alg:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="af53d7a6d954cf19" memberName="comboBoxDetAlg"
            virtualName="" explicitFocusOrder="0" pos="80 32 120 24" editable="0"
            layout="33" items="autotalent&#10;aubio(yinfast)" textWhenNonSelected="talent"
            textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="e80a04b02ed7c598" memberName="comboBoxSftAlg"
            virtualName="" explicitFocusOrder="0" pos="80 64 120 24" editable="0"
            layout="33" items="autotalent&#10;SoundTouch&#10;rubberband&#10;smbPitchShift"
            textWhenNonSelected="talent" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="970facfde35ebf14" memberName="sliderMinFreq"
          virtualName="" explicitFocusOrder="0" pos="280 32 64 24" min="20.0"
          max="2000.0" int="1.0" style="LinearBar" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Min Freq" id="ecd9f063fa626d71" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="208 32 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Min Freq:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="d9ad187aeae92cfe" memberName="sliderMaxFreq"
          virtualName="" explicitFocusOrder="0" pos="280 64 64 24" min="20.0"
          max="2000.0" int="1.0" style="LinearBar" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Max Freq" id="c77cd2d2a79ba21a" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="208 64 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Max Freq:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Afreq" id="6c2d5ea3d379fbc6" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="352 32 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Afreq:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="59e47c8bc7aed2e9" memberName="sliderAfreq"
          virtualName="" explicitFocusOrder="0" pos="408 32 64 24" min="430.0"
          max="450.0" int="1.0" style="LinearBar" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Gate" id="76106d6effefbdc4" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="352 64 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Gate:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="f5e62229970cdecf" memberName="sliderGate"
          virtualName="" explicitFocusOrder="0" pos="408 64 64 24" min="-100.0"
          max="-10.0" int="1.0" style="LinearBar" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <GROUPCOMPONENT name="new group" id="90fa0a6fcd8b3293" memberName="groupComponent2"
                  virtualName="" explicitFocusOrder="0" pos="8 112 584 232" title="misc"/>
  <TEXTBUTTON name="Apply" id="4589aed48d5db8d4" memberName="textButtonApply"
              virtualName="" explicitFocusOrder="0" pos="16 352 568 40" buttonText="Apply"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="8ee5d6d6879f4f10" memberName="textEditorMisc"
              virtualName="" explicitFocusOrder="0" pos="16 128 568 208" initialText=""
              multiline="1" retKeyStartsLine="1" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="VThresh" id="a2d5069765991727" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="480 32 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="VThresh:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="76d9b803899a8e99" memberName="sliderVThresh"
          virtualName="" explicitFocusOrder="0" pos="544 32 40 24" min="0.1"
          max="1.0" int="0.01" style="LinearBar" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

