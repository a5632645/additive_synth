#pragma once

#include <string>
#include <string_view>
#include <concepts>
#include <atomic>
#include <format>
#include "param_range.h"

namespace mana {
enum class ModulationType {
    kDisable,
    kMono,
    kPoly
};

class FloatParameter {
public:
    enum class ParamType {
        kFloat,
        kInt,
        kBool
    };

    static constexpr auto kTypeEnum = ParamType::kFloat;

    FloatParameter(ModulationType modulation_type, ParamRange range, std::string_view id)
        : id_(id), modulation_type_(modulation_type), range_(std::move(range)) {}

    template<class...T> requires (sizeof...(T) >= 1)
        FloatParameter(ModulationType modulation_type, ParamRange range, std::format_string<T...> format_text, T&&... args)
        : id_(std::format(format_text, std::forward<T>(args)...)), modulation_type_(modulation_type), range_(std::move(range)) {}

    virtual ~FloatParameter() = default;
    FloatParameter(FloatParameter const&) = delete;
    FloatParameter(FloatParameter&&) = delete;
    FloatParameter& operator=(FloatParameter const&) = default;
    FloatParameter& operator=(FloatParameter&&) = default;

    void SetValue(float new_val) { value_.store(range_.ConvertTo01(new_val)); }
    float GetValue() const { return range_.ConvertFrom01(value_.load()); }

    float Get01Value() const { return value_; }
    void Set01Value(float new_val) { value_.store(new_val); }

    ModulationType GetModulationType() const { return modulation_type_; }
    std::string_view GetId() const { return id_; }
    const ParamRange& GetRange() const { return range_; }
    virtual ParamType GetParamType() const { return ParamType::kFloat; }
protected:
    std::string id_;
    ModulationType modulation_type_;
    ParamRange range_;
    std::atomic<float> value_{};
};

class IntParameter : public FloatParameter {
public:
    static constexpr auto kTypeEnum = ParamType::kInt;

    using FloatParameter::FloatParameter;

    void SetInt(int v) { SetValue(static_cast<float>(v)); }
    int GetInt() const { return static_cast<int>(std::round(GetValue())); }
    operator int() const { return GetInt(); }

    ParamType GetParamType() const override { return ParamType::kInt; }
};

class BoolParameter : public FloatParameter {
public:
    static constexpr auto kTypeEnum = ParamType::kBool;

    using FloatParameter::FloatParameter;

    void SetBool(bool v) { Set01Value(value_ = v ? 1.0f : 0.0f); }
    bool GetBool() const { return Get01Value() > 0.5f; }
    operator bool() const { return GetBool(); }

    ParamType GetParamType() const override { return ParamType::kBool; }
};

template<class Type>
concept IsParamter = std::same_as<Type, FloatParameter> || std::same_as<Type, IntParameter> || std::same_as<Type, BoolParameter>;
}