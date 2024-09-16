#pragma once

#include <SFML/Graphics.hpp>

class Display {
public:
    static constexpr size_t kWidth = 64;
    static constexpr size_t kHeight = 32;
    static constexpr const char* kWindowName = "CHIP-8";
    static constexpr size_t kDefaultScale = 16;

    Display(size_t scale = kDefaultScale);

    void ConnectVideoMemory(const uint8_t* video_memory);

    bool IsOpen() const;
    void CheckKeyboard(uint8_t* input);

    void Update();

    void SetWindowName(const std::string& name);

private:
    sf::RenderWindow window_;
    sf::Image image_;
    sf::Texture texture_;
    sf::Sprite sprite_;

    const uint8_t* video_memory_;
};
