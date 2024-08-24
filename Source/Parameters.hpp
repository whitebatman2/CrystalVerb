//
//  Parameters.hpp
//  Miłosz Kolenderski
//

#ifndef Parameters_hpp
#define Parameters_hpp

#include <string>
#include <JuceHeader.h>
#include "FdnConfig.hpp"


namespace Parameters
{
    enum class PID {
        Bypass = 0,
        Dry,
        Wet,
        FeedbackType,
        RoomSizeMin,
        RoomSizeMax,
        FdnEqEnable,
        CrossOver1,
        CrossOver2,
        CrossOver3,
        CrossOver4,
        CrossOver5,
        CrossOver6,
        CrossOver7,
        Band1_Gain,
        Band2_Gain,
        Band3_Gain,
        Band4_Gain,
        Band5_Gain,
        Band6_Gain,
        Band7_Gain,
        Band8_Gain,
        StaticGain1,
        StaticGain2,
        StaticGain3,
        StaticGain4,
        StaticGain5,
        StaticGain6,
        StaticGain7,
        StaticGain8,
        StaticGain9,
        StaticGain10,
        StaticGain11,
        StaticGain12,
        StaticGain13,
        StaticGain14,
        StaticGain15,
        StaticGain16,
        OutEqEnable,
        CrossOver1Out,
        CrossOver2Out,
        CrossOver3Out,
        CrossOver4Out,
        CrossOver5Out,
        CrossOver6Out,
        CrossOver7Out,
        Band1_GainOut,
        Band2_GainOut,
        Band3_GainOut,
        Band4_GainOut,
        Band5_GainOut,
        Band6_GainOut,
        Band7_GainOut,
        Band8_GainOut,
        DiffusionEnable,
        DiffusionType,
        ErAmount,
        NumberOfParameters
    };

    const int numberOfParameters = static_cast<int>(PID::NumberOfParameters);

    inline std::string getIDString(PID id)
    {
        std::string ret;

        if (id == PID::Bypass)
        {
            return "Bypass";
        }
        else if (id == PID::Dry)
        {
            return "Dry";
        }
        else if (id == PID::Wet)
        {
            return "Wet";
        }
        else if (id == PID::FeedbackType)
        {
            return "FeedbackType";
        }
        else if (id == PID::RoomSizeMin)
        {
            return "RoomSizeMin";
        }
        else if (id == PID::RoomSizeMax)
        {
            return "RoomSizeMax";
        }
        else if (id == PID::FdnEqEnable)
        {
            return "FdnEqEnable";
        }
        else if (id >= PID::CrossOver1 && id <= PID::CrossOver7)
        {
            ret = "CrossOver1";
            ret[8] += static_cast<int>(id) - static_cast<int>(PID::CrossOver1);

            return ret;
        }
        else if (id >= PID::Band1_Gain && id <= PID::Band8_Gain)
        {
            ret = "Band1_Gain";
            ret[3] += static_cast<int>(id) - static_cast<int>(PID::Band1_Gain);

            return ret;
        }
        else if (id >= PID::StaticGain1 && id <= PID::StaticGain16)
        {
            ret = "StaticGain";
            ret += static_cast<int>(id) - static_cast<int>(PID::StaticGain1);

            return ret;
        }
        else if (id == PID::OutEqEnable)
        {
            return "StaticEqEnable";
        }
        else if (id >= PID::CrossOver1Out && id <= PID::CrossOver7Out)
        {
            ret = "CrossOver1Out";
            ret[8] += static_cast<int>(id) - static_cast<int>(PID::CrossOver1Out);

            return ret;
        }
        else if (id >= PID::Band1_GainOut && id <= PID::Band8_GainOut)
        {
            ret = "Band1_GainOut";
            ret[3] += static_cast<int>(id) - static_cast<int>(PID::Band1_GainOut);

            return ret;
        }
        else if (id == PID::DiffusionEnable)
        {
            return "Diffusion";
        }
        else if (id == PID::DiffusionType)
        {
            return "DiffusionType";
        }
        else if (id == PID::ErAmount)
        {
            return "ErAmount";
        }

        return "NULL";
    }

    inline std::string getNameString(PID id)
    {
        std::string ret;

        if (id == PID::Bypass)
        {
            return "Bypass";
        }
        else if (id == PID::Dry)
        {
            return "Dry";
        }
        else if (id == PID::Wet)
        {
            return "Wet";
        }
        else if (id == PID::FeedbackType)
        {
            return "Feedback type";
        }
        else if (id == PID::RoomSizeMin)
        {
            return "Room size min [m]";
        }
        else if (id == PID::RoomSizeMax)
        {
            return "Room size max [m]";
        }
        else if (id == PID::FdnEqEnable)
        {
            return "Enable FDN Equalizer";
        }
        else if (id >= PID::CrossOver1 && id <= PID::CrossOver7)
        {
            ret = "Crossover 1 [Hz]";
            ret[10] += static_cast<int>(id) - static_cast<int>(PID::CrossOver1);

            return ret;
        }
        else if (id >= PID::Band1_Gain && id <= PID::Band8_Gain)
        {
            ret = "Band 1 Gain";
            ret[5] += static_cast<int>(id) - static_cast<int>(PID::Band1_Gain);

            return ret;
        }
        else if (id >= PID::StaticGain1 && id <= PID::StaticGain16)
        {
            ret = "FDN Gain 1";
            ret[9] += static_cast<int>(id) - static_cast<int>(PID::StaticGain1);

            return ret;
        }
        else if (id == PID::OutEqEnable)
        {
            return "Output EQ enable";
        }
        else if (id >= PID::CrossOver1Out && id <= PID::CrossOver7Out)
        {
            ret = "Output crossover 1[Hz]";
            ret[17] += static_cast<int>(id) - static_cast<int>(PID::CrossOver1Out);

            return ret;
        }
        else if (id >= PID::Band1_GainOut && id <= PID::Band8_GainOut)
        {
            ret = "Output band 1 Gain";
            ret[12] += static_cast<int>(id) - static_cast<int>(PID::Band1_GainOut);

            return ret;
        }
        else if (id == PID::DiffusionEnable)
        {
            return "Diffusion";
        }
        else if (id == PID::DiffusionType)
        {
            return "Diffusion type";
        }
        else if (id == PID::ErAmount)
        {
            return "Early reflections";
        }

        return "NULL";
    }

    inline juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
    {
        using AudioParameterBool = juce::AudioParameterBool;
        using AudioParameterFloat = juce::AudioParameterFloat;
        using AudioParameterChoice = juce::AudioParameterChoice;
        using ParameterID = juce::ParameterID;
        using AudioParameterFloatAttributes = juce::AudioParameterFloatAttributes;

        juce::AudioProcessorValueTreeState::ParameterLayout params;

        params.add(std::make_unique<AudioParameterBool>(
            ParameterID {getIDString (PID::Bypass), 1},
            getNameString (PID::Bypass),
            false
        ));

        params.add(std::make_unique<AudioParameterFloat>(
            ParameterID{getIDString (PID::Dry), 1},
            getNameString (PID::Dry),
            0.0f, 1.0f, 0.5f
        ));

        params.add(std::make_unique<AudioParameterFloat>(
            ParameterID{ getIDString (PID::Wet), 1 },
            getNameString (PID::Wet),
            0.f, 1.f, 1.0f
        ));

        params.add(std::make_unique<AudioParameterChoice>(
            ParameterID{getIDString (PID::FeedbackType), 1},
            getNameString (PID::FeedbackType),
            juce::StringArray{"Hadamard", "Householder"},
            1
        ));

        params.add (std::make_unique<AudioParameterFloat>(
            ParameterID {getIDString (PID::RoomSizeMin), 1},
            getNameString (PID::RoomSizeMin),
            1.f, 60.f, 10.f
        ));

        params.add (std::make_unique<AudioParameterFloat>(
            ParameterID {getIDString (PID::RoomSizeMax), 1},
            getNameString (PID::RoomSizeMax),
            1.f, 60.f, 20.f
        ));

        params.add(std::make_unique<AudioParameterBool>(
            ParameterID{ getIDString(PID::FdnEqEnable), 1 },
            getNameString(PID::FdnEqEnable),
            true
        ));

        for (int i = static_cast<int>(PID::CrossOver1); i <= static_cast<int>(PID::CrossOver7); ++i)
        {
            params.add (std::make_unique<AudioParameterFloat>(
                ParameterID {getIDString(static_cast<PID>(i)), 1},
                getNameString(static_cast<PID>(i)),
                200.f, 20000.f, 4000.f / pow(2, i - static_cast<int>(PID::CrossOver1))
            ));
        }

        for (int i = static_cast<int>(PID::Band1_Gain); i <= static_cast<int>(PID::Band8_Gain); ++i)
        {
            params.add (std::make_unique<AudioParameterFloat>(
                ParameterID {getIDString(static_cast<PID>(i)), 1},
                getNameString(static_cast<PID>(i)),
                0.f, 1.f, 0.1 * (i - static_cast<int>(PID::Band1_Gain) + 1)
            ));
        }

        for (int i = static_cast<int>(PID::StaticGain1); i <= static_cast<int>(PID::StaticGain16); ++i)
        {
            params.add(std::make_unique<AudioParameterFloat>(
                ParameterID{ getIDString(static_cast<PID>(i)), 1 },
                getNameString(static_cast<PID>(i)),
                0.f, 1.f, 0.8)
            );
        }

        params.add(std::make_unique<AudioParameterBool>(
            ParameterID{ getIDString(PID::OutEqEnable), 1 },
            getNameString(PID::OutEqEnable),
            false
        ));

        for (int i = static_cast<int>(PID::CrossOver1Out); i <= static_cast<int>(PID::CrossOver7Out); ++i)
        {
            params.add(std::make_unique<AudioParameterFloat>(
                ParameterID{ getIDString(static_cast<PID>(i)), 1 },
                getNameString(static_cast<PID>(i)),
                200.f, 20000.f, 4000.f / pow(2, i - static_cast<int>(PID::CrossOver1Out))
            ));
        }

        for (int i = static_cast<int>(PID::Band1_GainOut); i <= static_cast<int>(PID::Band8_GainOut); ++i)
        {
            params.add(std::make_unique<AudioParameterFloat>(
                ParameterID{ getIDString(static_cast<PID>(i)), 1 },
                getNameString(static_cast<PID>(i)),
                0.f, 1.f, 0.1 * (i - static_cast<int>(PID::Band1_GainOut) + 1)
            ));
        }

        params.add (std::make_unique<AudioParameterBool>(
            ParameterID {getIDString (PID::DiffusionEnable), 1},
            getNameString (PID::DiffusionEnable),
            false
        ));

        params.add(std::make_unique<AudioParameterChoice>(
            ParameterID{ getIDString(PID::DiffusionType), 1 },
            getNameString(PID::DiffusionType),
            juce::StringArray{ "Hadamard", "Householder" },
            0
        ));

        params.add(std::make_unique<AudioParameterFloat>(
            ParameterID{ getIDString(PID::ErAmount), 1 },
            getNameString(PID::ErAmount),
            0.f, 1.f, 0.5f
        ));

        return params;
    }

    inline float getParameterValue(const std::array<juce::RangedAudioParameter*, Parameters::numberOfParameters> &parameters, PID paramID)
    {
        return parameters[static_cast<int>(paramID)]->getValue();
    }

    inline float getParameterValueDenormalized(const std::array<juce::RangedAudioParameter*, Parameters::numberOfParameters> &parameters, PID paramID)
    {
        float valueNorm = parameters[static_cast<int>(paramID)]->getValue();
        return parameters[static_cast<int>(paramID)]->getNormalisableRange().convertFrom0to1(valueNorm);
    }

    inline bool operator==(int iPID, PID pID)
    {
        return static_cast<int>(pID) == iPID;
    }

    inline bool operator==(PID pID, int iPID)
    {
        return static_cast<int>(pID) == iPID;
    }
}

#endif /* Parameters_hpp */
