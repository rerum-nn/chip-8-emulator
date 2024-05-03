#pragma once

#include <SFML/Audio.hpp>
#include <cstdint>

class Speakers {
public:
    Speakers();

    void Play();
    void Stop();

    bool AreActive() const;
private:
    sf::SoundBuffer buffer_;
    sf::Sound sound_;

    static constexpr uint32_t kSampleRate = 44100;
    static constexpr double kDuration = 1.;
    static constexpr double kFrequency = 261.63; // C4
};
