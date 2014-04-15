/*
  ==============================================================================

	This file was auto-generated by the Introjucer!

	It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "GonioPoints.h"
#include <vector>
#include "CustomDrawing.h"
#include "MainLayout.h"
#include "GoniometerControl.h"
#include "SpectrometerControl.h"
#include "TomatlLookAndFeel.h"
#include <functional>
//==============================================================================
/**
*/

class EditorAction : public Message
{
public: 
	std::function<void(void)> mAction;

	EditorAction(std::function<void(void)> action)
	{
		mAction = action;
	}
};

class AdmvAudioProcessorEditor  : public AudioProcessorEditor, public Timer, public MessageListener
{
private:
	
	GoniometerControl mGonio;
	MainLayout mLayout;
	SpectrometerControl mSpectrometer;
	TomatlLookAndFeel mLookAndFeel;
	AdmvAudioProcessor* mParentProcessor;

	void addComponent(juce::Rectangle<int> bounds, ILateInitComponent& ctrl)
	{
		addAndMakeVisible(ctrl);
		ctrl.setTopLeftPosition(bounds.getTopLeft().getX(), bounds.getTopLeft().getY());
		ctrl.init(bounds);
	}
public:
	void updateFromState(const AdmvPluginState& state)
	{
		mLayout.updateFromState(state);
	}

	AdmvAudioProcessorEditor (AdmvAudioProcessor* ownerFilter)
		: AudioProcessorEditor(ownerFilter), mGonio(ownerFilter), mSpectrometer(ownerFilter), mLayout(ownerFilter)
	{
		mParentProcessor = ownerFilter;
		LookAndFeel::setDefaultLookAndFeel(&mLookAndFeel);

		setSize(mLayout.getWidth(), mLayout.getHeight());

		addAndMakeVisible(mLayout);
		mLayout.setTopLeftPosition(0, 0);

		addComponent(mLayout.getGonioRectangle(), mGonio);
		addComponent(mLayout.getSpectroRectangle(), mSpectrometer);

		startTimer(1000. / 25.);
	}

	~AdmvAudioProcessorEditor()
	{
		stopTimer();
	}

	const MainLayout& getMainLayout()
	{
		return mLayout;
	}

	virtual void handleMessage(const Message& message)
	{
		EditorAction& action = reinterpret_cast<EditorAction&>((Message&)message);
		
		action.mAction();
		
	}

	virtual void timerCallback()
	{
		mGonio.repaint();
		mSpectrometer.repaint();
		
		if (mLayout.isShowing())
		{
			mLayout.updateInputChannels(mParentProcessor->getCurrentInputCount());

			if (!mParentProcessor->getState().mManualGoniometerScale)
			{
				mLayout.updateGonioScale(mParentProcessor->mLastGonioScale);
			}
		}
	}

	void prepareForSampleRate(size_t sampleRate)
	{
		mSpectrometer.prepareForSampleRate(sampleRate);
	}

	static float logDecay(float alpha)
	{
		return std::max(0.01, alpha - std::log(alpha * 255) / 255 * 2.5) - 0.01;
	}
	//==============================================================================
	// This is just a standard Juce paint method...
	void paint(Graphics& g)
	{

	}
};


#endif  // PLUGINEDITOR_H_INCLUDED
