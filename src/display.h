#pragma once

#include <SFML/Graphics.hpp>

class Display {
public:
    static constexpr size_t kWidth = 64;
    static constexpr size_t kHeight = 32;
    static constexpr const char* kMonitorName = "CHIP-8";

    Display();

    void ConnectVideoMemory(const uint8_t* video_memory);

    bool IsOpen() const;
    void CheckKeyboard(uint8_t* input);
    uint8_t WaitForKey();

    void Update();

private:
    sf::RenderWindow window_;
    sf::Image image_;
    sf::Texture texture_;
    sf::Sprite sprite_;

    const uint8_t* video_memory_;
};
