/*
  ==============================================================================

    FunkitLookAndFeel.h

    Shared colours, panel painting and custom rotary knob / button styling.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace FunkitColours {
    const juce::Colour background   { 0xff15171c };
    const juce::Colour backgroundHi { 0xff20232b };
    const juce::Colour panel        { 0xff262a33 };
    const juce::Colour panelEdge    { 0xff3a3f4b };
    const juce::Colour knobBody     { 0xff333844 };
    const juce::Colour knobTrack    { 0xff12141a };
    const juce::Colour accent       { 0xffffa63d };
    const juce::Colour text         { 0xffe8eaf0 };
    const juce::Colour textDim      { 0xff9aa0ae };
}

class FunkitLookAndFeel : public juce::LookAndFeel_V4 {
public:
    FunkitLookAndFeel() {
        setColour(juce::ResizableWindow::backgroundColourId, FunkitColours::background);
        setColour(juce::Label::textColourId, FunkitColours::text);
        setColour(juce::Slider::rotarySliderFillColourId, FunkitColours::accent);
        setColour(juce::Slider::rotarySliderOutlineColourId, FunkitColours::knobTrack);
        setColour(juce::Slider::thumbColourId, FunkitColours::text);
        setColour(juce::Slider::textBoxTextColourId, FunkitColours::textDim);
        setColour(juce::TextButton::buttonColourId, FunkitColours::knobBody);
        setColour(juce::TextButton::buttonOnColourId, FunkitColours::accent);
        setColour(juce::TextButton::textColourOffId, FunkitColours::text);
        setColour(juce::TextButton::textColourOnId, FunkitColours::background);
        setColour(juce::ToggleButton::textColourId, FunkitColours::text);
        setColour(juce::ToggleButton::tickColourId, FunkitColours::accent);
    }

    // Draws a rounded section panel with a subtle border.
    static void paintPanel(juce::Graphics& g, juce::Rectangle<int> bounds) {
        auto r = bounds.toFloat().reduced(4.0f, 3.0f);
        g.setColour(FunkitColours::panel);
        g.fillRoundedRectangle(r, 8.0f);
        g.setColour(FunkitColours::panelEdge);
        g.drawRoundedRectangle(r, 8.0f, 1.0f);
    }

    static void paintBackground(juce::Graphics& g, juce::Rectangle<int> bounds) {
        g.setGradientFill(juce::ColourGradient(FunkitColours::backgroundHi, 0.0f, 0.0f,
                                               FunkitColours::background, 0.0f, (float) bounds.getHeight(), false));
        g.fillRect(bounds);
    }

    // Consistent section title styling.
    static void styleTitle(juce::Label& label) {
        label.setColour(juce::Label::textColourId, FunkitColours::accent);
        label.setJustificationType(juce::Justification::centredLeft);
        label.setFont(juce::FontOptions(18.0f, juce::Font::bold));
    }

    // Consistent section layout: title on the left of the header, optional trigger button on the right,
    // then a row of equally sized knobs.
    static void layoutSection(juce::Rectangle<int> bounds, juce::Label& title, juce::Button* trigger,
                              std::initializer_list<juce::Component*> knobs) {
        auto area = bounds.reduced(12, 8);
        auto header = area.removeFromTop(26);
        if(trigger != nullptr) {
            trigger->setBounds(header.removeFromRight(130));
        }
        title.setBounds(header);

        const int cellWidth = 80;
        for(auto* knob : knobs) {
            knob->setBounds(area.removeFromLeft(cellWidth));
        }
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override {
        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(6.0f);
        float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto centre = bounds.getCentre();
        float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        float trackWidth = 3.5f;
        float arcRadius = radius - trackWidth / 2.0f;

        // Background track
        juce::Path track;
        track.addCentredArc(centre.x, centre.y, arcRadius, arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
        g.strokePath(track, juce::PathStrokeType(trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Value arc
        if(slider.isEnabled()) {
            juce::Path value;
            value.addCentredArc(centre.x, centre.y, arcRadius, arcRadius, 0.0f, rotaryStartAngle, angle, true);
            g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
            g.strokePath(value, juce::PathStrokeType(trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // Knob body
        float bodyRadius = radius - trackWidth - 3.0f;
        g.setGradientFill(juce::ColourGradient(FunkitColours::knobBody.brighter(0.2f), centre.x, centre.y - bodyRadius,
                                               FunkitColours::knobBody.darker(0.4f), centre.x, centre.y + bodyRadius, false));
        g.fillEllipse(centre.x - bodyRadius, centre.y - bodyRadius, bodyRadius * 2.0f, bodyRadius * 2.0f);

        // Pointer
        juce::Path pointer;
        pointer.addRoundedRectangle(-1.5f, -bodyRadius + 2.0f, 3.0f, bodyRadius * 0.45f, 1.5f);
        g.setColour(FunkitColours::text);
        g.fillPath(pointer, juce::AffineTransform::rotation(angle).translated(centre.x, centre.y));
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
        auto r = button.getLocalBounds().toFloat().reduced(1.0f);
        auto base = backgroundColour;
        if(shouldDrawButtonAsDown) {
            base = FunkitColours::accent;
        } else if(shouldDrawButtonAsHighlighted) {
            base = base.brighter(0.15f);
        }
        g.setColour(base);
        g.fillRoundedRectangle(r, 6.0f);
        g.setColour(shouldDrawButtonAsDown ? FunkitColours::accent.brighter(0.3f) : FunkitColours::panelEdge);
        g.drawRoundedRectangle(r, 6.0f, 1.0f);
    }

    juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override {
        return juce::Font(juce::FontOptions(juce::jmin(16.0f, (float) buttonHeight * 0.5f), juce::Font::bold));
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool, bool isDown) override {
        g.setFont(getTextButtonFont(button, button.getHeight()));
        g.setColour(isDown ? FunkitColours::background : FunkitColours::text);
        g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred);
    }
};
