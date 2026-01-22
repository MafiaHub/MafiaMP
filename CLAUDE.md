# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

MafiaMP is a multiplayer modification for Mafia: Definitive Edition built on top of the MafiaHub Framework. It uses a client-server architecture with C++ for core engine integration and Lua for gameplay scripting.

## Build System

### Building the Project

**Note**: This project requires the MafiaHub Framework and cannot be built standalone.

The project uses CMake as the build system. From the Framework root directory:

```bash
# Configure and build the entire framework (including MafiaMP)
cmake -B build
cmake --build build

# Run tests
cmake --build build --target RunFrameworkTests
```

### Project Structure

- `code/client/` - Client-side C++ implementation (shared library)
- `code/server/` - Server-side C++ implementation (executable)
- `code/shared/` - Shared components (RPC, messages, sync data)
- `code/launcher/` - Game launcher executable
- `gamemode/` - Lua scripting gamemode (client/server/shared)
- `files/` - Assets and resources
- `scripts/` - Development utilities

### Build Configuration

- **Client**: Builds as `MafiaMPClient.dll` (Windows only, 64-bit)
- **Server**: Builds as `MafiaMPServer.exe` (cross-platform)
- **Launcher**: Builds as `MafiaMPLauncher.exe` (Windows only)

## Architecture

### Core Components

#### Client Architecture
- **Application Class**: `code/client/src/core/application.cpp` - Main client entry point inheriting from `Framework::Integrations::Client::Instance`
- **State Machine**: States in `code/client/src/core/states/` manage client lifecycle (Initialize, MainMenu, SessionConnected, etc.)
- **Game Integration**: Hook system in `code/client/src/core/hooks/` for low-level game engine integration
- **Entity Management**: ECS-based entity streaming and synchronization
- **UI System**: Web-based UI (Ultralight) and ImGui for debug interfaces

#### Server Architecture
- **Server Class**: `code/server/src/core/server.cpp` - Main server instance inheriting from `Framework::Integrations::Server::Instance`
- **Entity Management**: Authoritative entity creation and synchronization
- **Player Management**: Connection handling and player state management
- **Environment Control**: Weather, time, and world state management

#### Shared Systems
- **ECS Modules**: `code/shared/modules/` - Flecs-based entity component definitions
- **RPC System**: `code/shared/rpc/` - Remote procedure call definitions
- **Message System**: `code/shared/messages/` - Structured network messages
- **Game RPC**: `code/shared/game_rpc/` - Game-specific RPC implementations

### Key Integration Patterns

#### Hook-Based Game Integration
- Uses MinHook for function hooking and memory patching
- Pattern scanning to locate game functions in memory
- SDK wrappers in `code/client/src/sdk/` provide C++ interface to game objects
- Hooks intercept game events (player death, vehicle interactions, etc.)

#### ECS Entity Management
- Uses Flecs ECS for entity lifecycle management
- Entities represent players, vehicles, and world objects
- Components store synchronization data (position, health, weapon state)
- Server is authoritative, clients receive updates via messages

#### RPC Communication
- Built on SLNet (RakNet) networking
- Macros for RPC sending: `FW_SEND_COMPONENT_RPC`, `FW_SEND_CLIENT_COMPONENT_GAME_RPC`
- Bidirectional client-server communication
- Message validation and serialization

### Development Tools

#### Lua Scripting
- Gamemode system with client/server/shared Lua scripts
- Custom LuaVM integration hooking into game's Lua engine
- API for game logic, events, and player interactions
- VSCode workspace with Lua Language Server support

#### Debug Tools
- AutoHotkey script for dual-client testing: `scripts/dbg-client-pair.ahk`
- Console and developer UI panels for debugging
- Network statistics and entity browser
- Camera studio for cinematics

## Common Development Tasks

### Adding New Entities
1. Define components in `code/shared/modules/`
2. Add RPC definitions in `code/shared/rpc/` or `code/shared/game_rpc/`
3. Implement client-side entity factory in `code/client/src/game/entities/`
4. Add server-side spawning logic in `code/server/src/core/`

### Implementing Game Hooks
1. Add hook function in `code/client/src/core/hooks/`
2. Use pattern scanning to locate target game function
3. Implement hook with MinHook
4. Add corresponding SDK wrapper if needed

### Adding Lua API Functions
1. Add C++ implementation in client/server modules
2. Register with Lua engine in respective main.cpp files
3. Update gamemode scripts to use new functionality

### Testing
- Use `scripts/dbg-client-pair.ahk` for dual-client testing on Windows
- Run server separately for multiplayer testing
- Use in-game console and debug UI for runtime debugging

## Important Notes

### Framework Dependencies
- Built on MafiaHub Framework's client/server integration classes
- Uses Framework's ECS (Flecs), networking (SLNet), and scripting (Lua) systems
- Depends on Framework's logging, utilities, and platform abstraction

### Platform Support
- Client: Windows x64 only (game limitation)
- Server: Cross-platform (Windows, Linux, macOS)
- Uses Visual Studio-specific build flags for Windows client

### Memory Management
- Direct memory access to game objects requires careful handling
- Use smart pointers and RAII patterns for resource management
- Game engine objects have specific lifetime requirements

### Networking
- Client-server model with authoritative server
- Entity state synchronization via tick-based updates
- Connection management with automatic reconnection support
- Custom message serialization for game-specific data types

## Code Style

### SDK Files (`code/client/src/sdk/`)

SDK files should be minimal and clean. Follow these guidelines:

**Comments:**
- Use offset comments on class members: `// 0x00 - 0x08`
- Brief section headers are acceptable: `// Seat management`
- Use `// TODO:` for known incomplete work
- NO doxygen-style documentation (`/** */`, `@param`, `@return`)
- NO verbose docstrings or method descriptions
- NO redundant comments that just repeat the function name

**Example - Good:**
```cpp
class C_Example {
  public:
    void SetSpeed(float speed);
    float GetSpeed() const;

  private:
    float m_fSpeed;    // 0x00 - 0x04
    char _pad0[0x4];   // 0x04 - 0x08
};
```

**Example - Bad:**
```cpp
class C_Example {
  public:
    /**
     * Set the speed value
     * @param speed The speed to set
     */
    void SetSpeed(float speed);
};
```