#include "speakers.h"

#include <cmath>
#include <vector>

Speakers::Speakers() {
    size_t sample_count = static_cast<size_t>(kDuration * kSampleRate);
    std::vector<sf::Int16> samples(sample_count);

    for (size_t i = 0; i < sample_count; ++i) {
        double time = static_cast<double>(i) / kSampleRate;
        samples[i] = static_cast<sf::Int16>(32767.0 * std::sin(2.0 * M_PI * kFrequency * time));
    }

    buffer_.loadFromSamples(samples.data(), samples.size(), 1, kSampleRate);
    sound_.setBuffer(buffer_);
    sound_.setLoop(true);
}
void Speakers::Play() {
    sound_.play();
}
void Speakers::Stop() {
    sound_.stop();
}
bool Speakers::AreActive() const {
    return sound_.getStatus() == sf::Sound::Playing;
}
