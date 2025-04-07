#pragma once
#include <string>
#include <memory>
#include <cstdint>

namespace ue {

/**
 * C_Variant class from Mafia 1 DE
 * 
 * This class represents a variant type that can hold different types of values.
 * It appears to use a value holder pattern to store different types.
 * 
 * Based on analysis of functions:
 * - mafia::ui::hud::C_HudScriptW::ConvertFunctionArgumentsToVariant
 * - mafia::ui::hud::C_HudScriptW::SendMessageMovie
 * - mafia::ui::C_GameGUI2Module::SendMessageMovie
 */
class C_Variant {
public:
    // Value type enum (based on observation of the code)
    enum ValueType {
        TYPE_UNKNOWN = 0,
        TYPE_INT = 1,
        TYPE_FLOAT = 2,
        TYPE_BOOL = 3,
        TYPE_STRING = 4,
        TYPE_REFLOBJ = 5,
        // More types may exist
    };

    // Base value holder class for the variant
    class CValueHolderBase {
    public:
        virtual ~CValueHolderBase() = default;
        virtual CValueHolderBase* Clone() const = 0;
        virtual ValueType GetType() const = 0;
        // May have additional virtual methods for destruction/cleanup
    };

    // Template for holding specific types
    template<typename T>
    class CValueHolder : public CValueHolderBase {
    public:
        CValueHolder(const T& value) : m_Value(value) {}
        
        virtual CValueHolderBase* Clone() const override {
            return new CValueHolder<T>(m_Value);
        }
        
        virtual ValueType GetType() const override {
            return GetTypeForT();
        }
        
        // Helper to map C++ type to ValueType enum
        ValueType GetTypeForT() const;
        
        T m_Value;
    };

    // Special holder for reflection objects
    class CValueHolderReflObj : public CValueHolderBase {
    public:
        CValueHolderReflObj(void* obj) : m_Object(obj) {}
        
        virtual CValueHolderBase* Clone() const override {
            return new CValueHolderReflObj(m_Object);
        }
        
        virtual ValueType GetType() const override {
            return TYPE_REFLOBJ;
        }
        
        void* m_Object;
    };

    // Allocator helper class (observed in ConvertFunctionArgumentsToVariant)
    template<bool IsTrue, typename T>
    class C_Alloc {
    public:
        static void* Allocate(size_t size, int64_t bufferAddr) {
            // Implementation depends on internal allocation mechanisms
            // This is just a placeholder based on the observed function calls
            return new char[size];
        }
    };

public:
    // Constructors
    C_Variant() : m_ValueHolder(nullptr) {}
    
    // Copy constructor
    C_Variant(const C_Variant& other) : m_ValueHolder(nullptr) {
        if (other.m_ValueHolder) {
            m_ValueHolder.reset(other.m_ValueHolder->Clone());
        }
    }
    
    // Constructor for integer
    C_Variant(int value) : m_ValueHolder(new CValueHolder<int>(value)) {}
    
    // Constructor for float
    C_Variant(float value) : m_ValueHolder(new CValueHolder<float>(value)) {}
    
    // Constructor for bool
    C_Variant(bool value) : m_ValueHolder(new CValueHolder<bool>(value)) {}
    
    // Constructor for string
    C_Variant(const std::string& value) : m_ValueHolder(new CValueHolder<std::string>(value)) {}
    
    // Constructor for reflection object
    C_Variant(void* obj) : m_ValueHolder(new CValueHolderReflObj(obj)) {}
    
    // Assignment operator
    C_Variant& operator=(const C_Variant& other) {
        if (this != &other) {
            if (other.m_ValueHolder) {
                m_ValueHolder.reset(other.m_ValueHolder->Clone());
            } else {
                m_ValueHolder.reset();
            }
        }
        return *this;
    }

    // Swap function (used in ConvertFunctionArgumentsToVariant)
    static void swap(int64_t* a, int64_t* b) {
        // Implementation would swap two C_Variant instances
        // This is a placeholder based on the function signature observed
    }

    // Type checking
    bool IsInt() const { return m_ValueHolder && m_ValueHolder->GetType() == TYPE_INT; }
    bool IsFloat() const { return m_ValueHolder && m_ValueHolder->GetType() == TYPE_FLOAT; }
    bool IsBool() const { return m_ValueHolder && m_ValueHolder->GetType() == TYPE_BOOL; }
    bool IsString() const { return m_ValueHolder && m_ValueHolder->GetType() == TYPE_STRING; }
    bool IsReflObj() const { return m_ValueHolder && m_ValueHolder->GetType() == TYPE_REFLOBJ; }

    // Value getters
    int GetInt() const;
    float GetFloat() const;
    bool GetBool() const;
    std::string GetString() const;
    void* GetReflObj() const;
    
    // Value setters
    void SetInt(int value);
    void SetFloat(float value);
    void SetBool(bool value);
    void SetString(const std::string& value);
    void SetReflObj(void* obj);
    
    // Get the type of the value
    ValueType GetType() const {
        return m_ValueHolder ? m_ValueHolder->GetType() : TYPE_UNKNOWN;
    }

private:
    std::unique_ptr<CValueHolderBase> m_ValueHolder;
};

// Template specialization for type mapping
template<> inline C_Variant::ValueType C_Variant::CValueHolder<int>::GetTypeForT() const { return TYPE_INT; }
template<> inline C_Variant::ValueType C_Variant::CValueHolder<float>::GetTypeForT() const { return TYPE_FLOAT; }
template<> inline C_Variant::ValueType C_Variant::CValueHolder<bool>::GetTypeForT() const { return TYPE_BOOL; }
template<> inline C_Variant::ValueType C_Variant::CValueHolder<std::string>::GetTypeForT() const { return TYPE_STRING; }

} // namespace ue 