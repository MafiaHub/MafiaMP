#include "C_Variant.h"
#include <stdexcept>

namespace ue {

// Implementation of getter methods
int C_Variant::GetInt() const {
    if (!IsInt()) {
        throw std::runtime_error("Variant does not contain an integer value");
    }
    return static_cast<CValueHolder<int>*>(m_ValueHolder.get())->m_Value;
}

float C_Variant::GetFloat() const {
    if (!IsFloat()) {
        throw std::runtime_error("Variant does not contain a float value");
    }
    return static_cast<CValueHolder<float>*>(m_ValueHolder.get())->m_Value;
}

bool C_Variant::GetBool() const {
    if (!IsBool()) {
        throw std::runtime_error("Variant does not contain a boolean value");
    }
    return static_cast<CValueHolder<bool>*>(m_ValueHolder.get())->m_Value;
}

std::string C_Variant::GetString() const {
    if (!IsString()) {
        throw std::runtime_error("Variant does not contain a string value");
    }
    return static_cast<CValueHolder<std::string>*>(m_ValueHolder.get())->m_Value;
}

void* C_Variant::GetReflObj() const {
    if (!IsReflObj()) {
        throw std::runtime_error("Variant does not contain a reflection object");
    }
    return static_cast<CValueHolderReflObj*>(m_ValueHolder.get())->m_Object;
}

// Implementation of setter methods
void C_Variant::SetInt(int value) {
    m_ValueHolder.reset(new CValueHolder<int>(value));
}

void C_Variant::SetFloat(float value) {
    m_ValueHolder.reset(new CValueHolder<float>(value));
}

void C_Variant::SetBool(bool value) {
    m_ValueHolder.reset(new CValueHolder<bool>(value));
}

void C_Variant::SetString(const std::string& value) {
    m_ValueHolder.reset(new CValueHolder<std::string>(value));
}

void C_Variant::SetReflObj(void* obj) {
    m_ValueHolder.reset(new CValueHolderReflObj(obj));
}

} // namespace ue 