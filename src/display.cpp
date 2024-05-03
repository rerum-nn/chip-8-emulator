#include "display.h"

#include <cassert>
#include <unordered_map>

static const std::unordered_map<sf::Keyboard::Key, uint8_t> kKeyMap = {
    {sf::Keyboard::Key::X, 0x0},
    {sf::Keyboard::Key::Num1, 0x1},
    {sf::Keyboard::Key::Num2, 0x2},
    {sf::Keyboard::Key::Num3, 0x3},
    {sf::Keyboard::Key::Q, 0x4},
    {sf::Keyboard::Key::W, 0x5},
    {sf::Keyboard::Key::E, 0x6},
    {sf::Keyboard::Key::A, 0x7},
    {sf::Keyboard::Key::S, 0x8},
    {sf::Keyboard::Key::D, 0x9},
    {sf::Keyboard::Key::Z, 0xA},
    {sf::Keyboard::Key::C, 0xB},
    {sf::Keyboard::Key::Num4, 0xC},
    {sf::Keyboard::Key::R, 0xD},
    {sf::Keyboard::Key::F, 0xE},
    {sf::Keyboard::Key::V, 0xF},
};

Display::Display() : window_(sf::VideoMode(kWidth, kHeight), kWindowName), sprite_() {
    image_.create(kWidth, kHeight, sf::Color::Black);
    texture_.loadFromImage(image_);
    sprite_.setTexture(texture_);
    window_.setSize(sf::Vector2u(kWidth * 16, kHeight * 16));
}
bool Display::IsOpen() const {
    return window_.isOpen();
}
void Display::Update() {
    // TODO: Можно сделать ссылку на функцию, если видеопамять не подключена, то будет вызываться
    // та, в которой будет отображаться, что нет подклюения, иначе же вот эта. Это сделать для того,
    // чтобы постоянно не вызывать в одной функции if. А пока простым ассертом обойдемся.
    assert(video_memory_);
    for (size_t y = 0; y < kHeight; ++y) {
        for (size_t x = 0; x < kWidth; ++x) {
            uint16_t byte = (y * kWidth + x) / 8;
            uint8_t pixel = (video_memory_[byte] >> (7 - x % 8)) & 0x01;
            image_.setPixel(x, y, pixel ? sf::Color::White : sf::Color::Black);
        }
    }

    texture_.update(image_);
    window_.clear();
    window_.draw(sprite_);
    window_.display();
}
void Display::ConnectVideoMemory(const uint8_t *video_memory) {
    video_memory_ = video_memory;
}
void Display::CheckKeyboard(uint8_t* input) {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (kKeyMap.contains(event.key.code)) {
                input[kKeyMap.at(event.key.code)] = 1;
            }
        } else if (event.type == sf::Event::KeyReleased) {
            if (kKeyMap.contains(event.key.code)) {
                input[kKeyMap.at(event.key.code)] = 0;
            }
        }
    }
}
uint8_t Display::WaitForKey() {
    sf::Event event;

    while (window_.waitEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
            break;
        } else if (event.type == sf::Event::KeyPressed) {
            if (kKeyMap.contains(event.key.code)) {
                return kKeyMap.at(event.key.code);
            }
        }
    }

    return 0;
}
void Display::SetWindowName(const std::string& name) {
    window_.setTitle(kWindowName + std::string(": ") + name);
}
