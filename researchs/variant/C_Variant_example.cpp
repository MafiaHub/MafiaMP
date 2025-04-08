#include "C_Variant.h"
#include <iostream>

// Example struct to demonstrate reflection object usage
struct GameEntity {
    int id;
    std::string name;
    
    GameEntity(int _id, const std::string& _name) : id(_id), name(_name) {}
};

// Using the ue namespace
using ue::C_Variant;

void printVariantInfo(const C_Variant& variant) {
    std::cout << "Variant type: ";
    
    switch (variant.GetType()) {
        case C_Variant::TYPE_INT:
            std::cout << "Integer, value: " << variant.GetInt() << std::endl;
            break;
        case C_Variant::TYPE_FLOAT:
            std::cout << "Float, value: " << variant.GetFloat() << std::endl;
            break;
        case C_Variant::TYPE_BOOL:
            std::cout << "Boolean, value: " << (variant.GetBool() ? "true" : "false") << std::endl;
            break;
        case C_Variant::TYPE_STRING:
            std::cout << "String, value: \"" << variant.GetString() << "\"" << std::endl;
            break;
        case C_Variant::TYPE_REFLOBJ:
            std::cout << "Reflection Object, pointer: " << variant.GetReflObj() << std::endl;
            break;
        default:
            std::cout << "Unknown" << std::endl;
            break;
    }
}

// Example demonstrating Lua-to-C_Variant conversion pattern
void simulateLuaToVariantConversion() {
    std::cout << "\n=== Simulating Lua to C_Variant Conversion ===" << std::endl;
    
    // Create an array of variants (similar to what ConvertFunctionArgumentsToVariant does)
    const int NUM_ARGS = 3;
    C_Variant* variantArray = new C_Variant[NUM_ARGS];
    
    // Simulate converting Lua values to variants
    variantArray[0] = C_Variant(42);           // Integer
    variantArray[1] = C_Variant(std::string("Message")); // String
    variantArray[2] = C_Variant(true);         // Boolean
    
    // Demonstrate passing these to a simulated UI function
    std::cout << "Sending to UI:" << std::endl;
    for (int i = 0; i < NUM_ARGS; ++i) {
        std::cout << "  Argument " << i << ": ";
        printVariantInfo(variantArray[i]);
    }
    
    // Cleanup
    delete[] variantArray;
}

int main() {
    // Creating variants of different types
    C_Variant intVariant(42);
    C_Variant floatVariant(3.14159f);
    C_Variant boolVariant(true);
    C_Variant stringVariant(std::string("Hello, Mafia!"));
    
    // Create a game entity and store it in a variant
    GameEntity* entity = new GameEntity(1, "Tommy Angelo");
    C_Variant entityVariant(static_cast<void*>(entity));
    
    // Print information about each variant
    std::cout << "=== Variant Examples ===" << std::endl;
    printVariantInfo(intVariant);
    printVariantInfo(floatVariant);
    printVariantInfo(boolVariant);
    printVariantInfo(stringVariant);
    printVariantInfo(entityVariant);
    
    // Demonstrating type checking
    if (intVariant.IsInt()) {
        std::cout << "\nSuccessfully verified intVariant contains an integer." << std::endl;
    }
    
    // Demonstrating type conversion (would throw an exception in real use)
    try {
        std::cout << "Attempting to get float from int variant: ";
        float value = intVariant.GetFloat();
        std::cout << value << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    
    // Demonstrating copy semantics
    C_Variant copyVariant = intVariant;
    std::cout << "\nCopied variant: ";
    printVariantInfo(copyVariant);
    
    // Change the original variant
    intVariant.SetInt(100);
    std::cout << "Original after modification: ";
    printVariantInfo(intVariant);
    std::cout << "Copy after original modification: ";
    printVariantInfo(copyVariant);
    
    // Simulate the Lua integration pattern observed in the game
    simulateLuaToVariantConversion();
    
    // Clean up
    delete entity;
    
    return 0;
} 