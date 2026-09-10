#pragma once

#include <ports/Interfaces.hpp>
#include <cstdint>
#include <cstring>

namespace badge::adapters::null_impl {

class Display final : public ports::IDisplay {
public:
    Status probe() override {
        ready_ = true;
        return Status::Ok;
    }
    bool available() const override { return ready_; }
    Status showText(const char* text) override {
        if (!ready_) return Status::NotReady;
        if (!text) return Status::InvalidArg;
        std::strncpy(last_text_, text, sizeof(last_text_) - 1);
        last_text_[sizeof(last_text_) - 1] = '\0';
        return Status::Ok;
    }
    Status setBrightness(std::uint8_t percent) override {
        if (!ready_) return Status::NotReady;
        brightness_ = percent > 100 ? 100 : percent;
        return Status::Ok;
    }
    Status clear() override {
        if (!ready_) return Status::NotReady;
        last_text_[0] = '\0';
        return Status::Ok;
    }
    void sleep() override { asleep_ = true; }
    void wake() override { asleep_ = false; }

    const char* lastText() const { return last_text_; }

private:
    bool ready_ = false;
    bool asleep_ = false;
    std::uint8_t brightness_ = 100;
    char last_text_[96]{};
};

class Buttons final : public ports::IButtons {
public:
    Status probe() override {
        ready_ = true;
        return Status::Ok;
    }
    bool available() const override { return ready_; }

private:
    bool ready_ = false;
};

class Haptic final : public ports::IHaptic {
public:
    Status probe() override {
        ready_ = true;
        return Status::Ok;
    }
    bool available() const override { return ready_; }
    Status pulse(std::uint8_t) override { return ready_ ? Status::Ok : Status::NotReady; }
    Status pattern(std::uint8_t) override { return ready_ ? Status::Ok : Status::NotReady; }

private:
    bool ready_ = false;
};

class Imu final : public ports::IImu {
public:
    Status probe() override {
        ready_ = true;
        return Status::Ok;
    }
    bool available() const override { return ready_; }

private:
    bool ready_ = false;
};

class Nfc final : public ports::INfc {
public:
    Status probe() override {
        ready_ = true;
        return Status::Ok;
    }
    bool available() const override { return ready_; }

private:
    bool ready_ = false;
};

class Leds final : public ports::ILeds {
public:
    Status probe() override {
        ready_ = true;
        return Status::Ok;
    }
    bool available() const override { return ready_; }
    Status setRgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) override {
        if (!ready_) return Status::NotReady;
        r_ = r;
        g_ = g;
        b_ = b;
        return Status::Ok;
    }
    Status next() override {
        if (!ready_) return Status::NotReady;
        palette_ = static_cast<std::uint8_t>((palette_ + 1) % 3);
        return Status::Ok;
    }
    void sleep() override {}

private:
    bool ready_ = false;
    std::uint8_t r_ = 0, g_ = 0, b_ = 0;
    std::uint8_t palette_ = 0;
};

class Storage final : public ports::IStorage {
public:
    Status probe() override {
        ready_ = true;
        return Status::Ok;
    }
    bool available() const override { return ready_; }
    Status writeFile(const char*, const void*, std::size_t) override {
        return ready_ ? Status::Ok : Status::NotReady;
    }
    Status readFile(const char*, void*, std::size_t, std::size_t* out_bytes) override {
        if (!ready_) return Status::NotReady;
        if (out_bytes) *out_bytes = 0;
        return Status::Ok;
    }

private:
    bool ready_ = false;
};

class Audio final : public ports::IAudio {
public:
    Status probe() override {
        ready_ = true;
        return Status::Ok;
    }
    bool available() const override { return ready_; }
    Status setVolume(std::uint8_t percent) override {
        if (!ready_) return Status::NotReady;
        volume_ = percent > 100 ? 100 : percent;
        return Status::Ok;
    }

private:
    bool ready_ = false;
    std::uint8_t volume_ = 50;
};

class Battery final : public ports::IBattery {
public:
    Status probe() override {
        ready_ = true;  // 充电管理在板，精确 SOC 仍可能是 -1
        return Status::Ok;
    }
    bool available() const override { return ready_; }
    int socPercent() override { return ready_ ? -1 : -1; }
    bool charging() override { return false; }

private:
    bool ready_ = false;
};

class Radio final : public ports::IRadio {
public:
    Status probe() override {
        ready_ = true;
        return Status::Ok;
    }
    bool available() const override { return ready_; }

private:
    bool ready_ = false;
};

class Power final : public ports::IPower {
public:
    Status probe() override { return Status::Ok; }
    void enterSleep() override { sleeping_ = true; }
    void leaveSleep() override { sleeping_ = false; }
    bool sleeping() const { return sleeping_; }

private:
    bool sleeping_ = false;
};

}  // namespace badge::adapters::null_impl
