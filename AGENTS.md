# AGENTS.md

This file provides guidance to coding agents (Claude Code, Codex, Cursor and others) working on
MafiaMP. It is the only instructions file in this repository.

The Framework root `AGENTS.md` applies here too. Its rules are not repeated in this file -- read
both. `docs/project_architecture.md` and `docs/starting_a_new_project.md` at the Framework root
are normative for how a project is shaped.

## Project overview

MafiaMP is a multiplayer modification for Mafia: Definitive Edition, built on the MafiaHub
Framework. It targets the Steam build of the game.

- **MafiaMPClient.dll** -- the modification, loaded into the game (Windows x64 only)
- **MafiaMPLauncher.exe** -- bootstrapper that starts the game with the client (Windows only)
- **MafiaMPServer.exe** -- the dedicated server (cross-platform, 64-bit)

The client inherits `Framework::Integrations::Client::Instance` (`code/client/src/core/application.cpp`),
the server `Framework::Integrations::Server::Instance` (`code/server/src/core/server.cpp`). The
session flow is the Framework state machine's: Initialize, MainMenu, SessionConnection,
SessionConnected, SessionDisconnection, SessionOfflineDebug and Shutdown under
`code/client/src/core/states/`.

Gameplay is scripted in **JavaScript resources** (`resources/`), not C++: server scripts run on
libnode, client scripts in sandboxed V8. `core/luavm.*` on the client is unrelated -- it executes
strings in the game's own Lua engine.

## Layout

- `code/client/src/sdk/` -- reversed game SDK. `patterns.{h,cpp}` resolves every native address
  once at startup; every other file sits in the directory its namespace names (rule 7).
- `code/client/src/core/` -- the shell: `Application`, the state machine (`states/`), game hooks
  (`hooks/`, one file per subsystem), entity modules (`modules/`), ImGui/CEF UI (`ui/`), client
  scripting builtins (`builtins/`) and developer tooling (`dev/`, `dev_features.*`).
- `code/client/src/game/` -- game-facing infrastructure shared by several systems: entity
  factories and streaming, helpers, and classes that override SDK types (`overrides/`).
- `code/server/src/core/` -- the server instance and its scripting builtins (`builtins/`). Reserved
  script events (`playerConnect`, `playerDied`, ...) are emitted there through `EmitReserved`;
  MafiaMP events never go into the Framework's reserved events.
- `code/shared/` -- what both peers compile: replicated entities and their registration list
  (`entities/`), sync data (`modules/`), RPC identifiers (`rpc/ids.h`), constants, version.
- `resources/` -- the default resources (`freeroam`, `shared-utils`), staged beside the server.
- `researchs/` -- reverse-engineering notes. They record the *why*; the IDB is the index (rule 3).
- `scripts/dbg-client-pair.ahk` -- boots two clients against a local server.

Namespaces: `MafiaMP::Core` for the client shell, `MafiaMP::Game` for `game/`,
`MafiaMP::Scripting` for script bindings, `MafiaMP::Shared::*` for wire types, `SDK` for the game.

## Non-negotiable rules

These are not suggestions. Do not weaken, re-litigate, or "defensively" work around them. They
come from M2O and FMP, where every one was written after the rule was broken and cost days: the
failures they cause read as bad offsets, or as work already done, rather than as what they are.

**Much of the existing SDK predates these rules.** Loose offsets (`C_Car::IsEngineOn` reads
`this + 0x1280`), forward-declared engine types and grouped headers are on screen everywhere.
Code already in the tree is not precedent. When you touch a class, bring the part you touch up to
these rules; never add more of the old shape.

### 1. Never reach into an engine object by a loose offset

An offset written at the call site is a fact about a type that the type itself does not know. It
cannot be checked, it is invisible to every other caller, and when the game updates nothing points
at what moved.

**Therefore: reverse the class.** Give it its real base, name the members you have proven, pad the
runs between them, and pin the result with `static_assert`. The offset then exists once, is
enforced by the compiler, and every caller is typed.

```cpp
// FORBIDDEN -- the layout lives at the call site and nothing enforces it
bool IsEngineOn() const {
    uint32_t flags = *reinterpret_cast<uint32_t *>((uintptr_t)this + 0x1280);
    return (flags >> 2) & 1;
}

// CORRECT -- the layout lives in the type
class C_Car: public C_ActorVehicle {
  public:
    bool IsEngineOn() const {
        return (m_uVehicleFlags & kEngineOn) != 0;
    }

  private:
    enum VehicleFlags : uint32_t {
        kEngineOn = 0x4,
    };

    char _pad1258[0x1280 - 0x1258]; // 0x1258 - 0x1280
    uint32_t m_uVehicleFlags;       // 0x1280 - 0x1284

    static void AssertLayout() {
        static_assert(offsetof(C_Car, m_uVehicleFlags) == 0x1280, "C_Car::m_uVehicleFlags offset mismatch");
    }
};
```

Members follow the existing SDK naming: `m_` plus a Hungarian type prefix (`m_pVehicle`,
`m_fSpeed`, `m_nCount`, `m_bActive`, `m_uFlags`). New and converted members are private, reached
through accessors. Pad with `char _padNN[N]`, named for the offset the run starts at, and comment
every member with its range (`// 0x00 - 0x08`). Put the per-member `offsetof` assertions in a
never-called private static `AssertLayout()`: that is the one place where the type is complete
*and* its own privates are in scope. Assert `sizeof` wherever the full size is proven -- a
constructor's or factory's allocation size is proof.

Fix the inheritance while you are there. A class whose base is wrong has every member offset
wrong with it. If a base class is missing virtuals a derived class needs, fix the base rather
than working around it.

Before writing a class, check whether it already exists, and extend it -- split a padding run and
keep the assertions passing -- rather than declaring a second one.

The same applies to vtable slots. Prove the index against the game's own call site and give the
class a named virtual at that position; do not index a vtable by hand at the call site. The order
of the virtuals in the class **is** the vtable order, so no `// vf7` comments. The one exception is
a COM vtable the client hooks dynamically, such as `IDXGISwapChain::Present`. Placeholder slots
must be named apart from every other interface in the class's hierarchy: a virtual that shares
its name and signature with one in a secondary base overrides that one instead of taking a slot of
its own, and every later slot silently shifts.

### 2. Never call a native through a cast function pointer

`gPatterns.*` holds an address, not a callable. Casting it to a function pointer at the call site
spells the calling convention by hand in a place nobody will re-check, and gets it silently wrong
on the cases that matter: a member's hidden return pointer, where `this` sits, a free function
that looks like a member.

**Therefore: call through `hook::`.** `hook::this_call<TRet>(address, self, args...)` for members,
`hook::call<TRet>(address, args...)` for free and static functions. Which one is not a style
choice -- read the mangled name: `QEAA` and `QEBA` are members and keep `this` in RCX, `SA` is a
static member and `YA` a free function, neither with a `this`.

```cpp
// FORBIDDEN
using GetDamageFn = float(__fastcall *)(const SDK::C_Car *);
reinterpret_cast<GetDamageFn>(SDK::gPatterns.C_Car__GetDamage)(car);

// CORRECT, in sdk/entities/c_car.cpp
float C_Car::GetDamage() const {
    return hook::this_call<float>(gPatterns.C_Car__GetDamage, this);
}
```

A native returning anything larger than a register takes the hidden return pointer as its
**second** argument on a member, `this` keeping RCX. Spell it out --
`hook::this_call<void>(addr, this, &result, args...)` -- rather than returning by value and
letting the return slot land in RCX.

The call lives in the SDK class's `.cpp`, not in a hook or a module: callers get a typed member.

This does not apply to the original-function pointer MinHook hands back. That is a real function
pointer, and calling it directly is correct.

### 3. Name every discovery in the IDB, in MSVC mangled form

Reversing that lives only in a chat log, a commit message or a header comment is reversing that
the next session pays for again. The IDB is the one place the work accumulates, and a
`sub_142CDA650` you have already understood is indistinguishable from one nobody has looked at.

**Therefore: the moment a function's purpose is established, rename it in the IDB, and comment
what the name cannot carry.** Not at the end of the task, not "once it is confirmed in game": the
understanding is what is being recorded. The same goes for globals, vtables and struct members.

Names go in **MSVC mangled notation**: the binary is MSVC-compiled, its RTTI uses that encoding,
and IDA demangles it into a readable signature. A bare identifier is a second convention in a
database that already has one.

```
FORBIDDEN   C_Car__Lock
FORBIDDEN   C_Car::Lock
CORRECT     ?Lock@C_Car@@QEAAXXZ
            -> public: void __cdecl C_Car::Lock(void)
```

Spell the namespaces the RTTI gives you (`.?AVC_Vehicle@vehicle@game@ue@@` reads
innermost-first), and encode what you observed: `U` for a virtual and `Q`/`S`/`Y` otherwise, `B`
for a const member, `??0`/`??1` for constructors and destructors, and the real argument types.
Globals use the data encoding (`?s_Instance@C_Game@@2PEAV1@EA`).

**Verify before you write.** `ida_name.demangle_name(candidate, 0)` returns the readable form, or
nothing when the string is malformed. Check the whole batch, apply it, read it back, then save
the IDB.

A name must not claim more than was proven: where only part of a system is understood, say so in
a comment rather than inventing a plausible identity. And it must not carry what a signature
cannot -- struct offsets, which argument means what, an ordering constraint. Those go in a comment
at the same address.

### 4. Never wrap a statement that fits on one line

The Framework's `.clang-format` sets `ColumnLimit: 280`. That is the width the code is meant to be
read at. Wrapping a statement that fits splits one thought across two lines and buries the
receiver of a call under its own arguments.

```cpp
// FORBIDDEN -- fits, broken anyway
Framework::Logging::GetLogger(FRAMEWORK_INNER_CLIENT)
    ->info("MafiaMP client initialized");

// CORRECT
Framework::Logging::GetLogger(FRAMEWORK_INNER_CLIENT)->info("MafiaMP client initialized");
```

This covers every construct: chained member access, conditions, initializer lists, template
arguments, signatures and parameter lists. Break only when the line genuinely exceeds 280
columns, and then at the outermost level that brings it under. Prose is the exception: comments
and Markdown stay wrapped at 72-100 columns.

### 5. Never alias or import a name -- spell it where it stands

An alias saves the writer keystrokes and costs every later reader a lookup.

**Therefore: write the qualified name at every use.** No `using X = Y;`, no `using namespace X;`,
no `using X::y;`, no namespace alias, no `typedef` that renames a type.

```cpp
// FORBIDDEN
using Vehicle = SDK::ue::game::vehicle::C_Vehicle;
Vehicle *vehicle = car->GetVehicle();

// CORRECT
SDK::ue::game::vehicle::C_Vehicle *vehicle = car->GetVehicle();
```

**Every SDK name is written fully qualified, from `SDK::`, at every use outside the SDK's own
namespaces** -- not at file scope, not in an anonymous namespace, not inside a function. A long
name is the price of knowing, at a glance, that the type belongs to the game.

The function-pointer typedefs for MinHook trampolines (`typedef void(__fastcall *C_Car__Lock_t)(SDK::C_Car *);`)
stay: they declare a trampoline's type, which is not a rename.

### 6. Never forward-declare an engine type -- include its header

A forward declaration is a second, silent statement about a type that the type's own header
already makes. It goes stale without a compiler error: the class moves namespace and the
declaration quietly creates a new empty one.

**Therefore: `#include` the SDK header that defines it**, and use the real type for every member
-- never a `void *` with a comment naming what it points to.

```cpp
// FORBIDDEN, in sdk/entities/c_car.h
namespace SDK {
    class I_Human2;
}

// CORRECT
#include "sdk/entities/c_human_2.h"
```

If two SDK headers would include each other, the cycle is the finding: one of the two types is in
the wrong file or carries a member it should not.

### 7. An SDK file lives in the directory its namespace names, one class per file

**The directory path under `sdk/` is the namespace path under `SDK::`, lowercased.**
`SDK::ue::game::vehicle::C_Vehicle` is `sdk/ue/game/vehicle/c_vehicle.h`;
`SDK::mafia::ui::hud::C_HudController` is `sdk/mafia/ui/hud/c_hud_controller.h`. File names are
the type name in `snake_case`, a word per underscore (`c_game_gui_module.h`, never
`c_gameguimodule.h`). A `.cpp` sits beside its header.

Which namespace a type is in is a finding, not a preference: the RTTI type-descriptor strings
spell it out, and where a type has no RTTI record the mangled names of the functions that take it
do. The Mafia 3 Mac dump carries symbolized names for much of the shared engine. Confirm the
namespace before creating or moving a file.

Global-namespace types go in a bucket for the subsystem that owns them (`entities/` for `C_Entity`,
`C_Actor`, `C_Car`, `C_Human2` and their kin), or at the `sdk/` root when no subsystem fits. A
bucket is named for the subsystem, not for how the type feels. `patterns.{h,cpp}` stay at the root.

**One top-level class per header.** A file is named for its type, so a second type in it is one
nobody can find by name. Types reversed together or used together still get a file each. A type
may stay nested inside its owner when the game's names nest it too, or nothing outside uses it.
Headers that still group types predate this rule: split one when you add to it, and never add to
the group.

### 8. Every RPC identifier goes through `FW_RPC_IDENTIFIER`

RPC4 keys its slots by the identifier string and puts it on the wire. A bare literal ships in
`MafiaMPClient.dll` one `strings` away and travels in every packet.

**Therefore: declare it in `code/shared/rpc/ids.h` with `FW_RPC_IDENTIFIER("MafiaMP::Name")`.**
The macro hashes it at compile time; only the opaque token reaches the binary and the wire, and it
is resolved through `NetworkPeer::ResolveIdentifier` at each `Signal`/`RegisterSlot`. Keep names
`MafiaMP::`-prefixed and unique: both peers agree on the token only by compiling the same string.

`-DFW_RPC_IDENTIFIER_SALT` becomes `MAFIAMP_RPC_IDENTIFIER_SALT` on the client and server, which
hand it to `InstanceOptions::rpcIdentifierSalt`; without it both run on the Framework's public
default salt. Client and server of one build must carry the same value.

Continuous per-entity state (health, skin, vehicle properties, seating) is not an RPC: it lives on
the replicated entity and syncs through its serialization.

### 9. Flag bits and enumerated values are enums, never loose constants

A loose `constexpr` says nothing about which field its bit belongs to, and bits of two fields that
share a value read as the same thing.

**Therefore: declare an enum per field**, nested in the type that holds it, unscoped with a fixed
underlying type so `m_uFlags & kEngineOn` stays free of casts. A field's enumerated values,
sentinels and type codes get an enum too: an entity type is an `SDK::E_EntityType`, never a raw
`uint8_t`. Offsets, counts, sizes and limits are not flags and stay `constexpr`.

### 10. A game container has its real type, never its pointers

Three loose pointers, or a `void *` and a count, hide what the field is and leave every caller to
walk it by hand.

**Therefore: declare the container** -- `std::vector<T>`, `std::map<K, V>`, `std::set<T>`,
`std::shared_ptr<T>`, or the game's own template when it has one. When the element is not
understood yet, take its size from the code that grows the container and declare a small opaque
class of that size. Pin the container's size with a `static_assert` where you first type one.

```cpp
// FORBIDDEN
S_Entry *m_pEntriesBegin;    // 0x40 - 0x48
S_Entry *m_pEntriesEnd;      // 0x48 - 0x50
S_Entry *m_pEntriesCapacity; // 0x50 - 0x58

// CORRECT
std::vector<S_Entry> m_Entries; // 0x40 - 0x58
```

The game allocates these, and its own objects, through its native allocator (`SDK::C_IE`). Read
them in place: never construct, copy, grow or destroy one from the mod. A game object the mod
creates and the game later frees is allocated with `SDK::C_IE::Alloc`, never `operator new` --
mixing heaps corrupts memory and fails far from the cause, often as an instant process death with
no minidump.

### 11. Name every field you use for what the game does with it

A member called `m_unk40`, `m_nField18` or `m_pUnk` says only where it is. The next reader cannot
tell what it means or whether writing it is safe.

**Therefore: name a member from the evidence** -- the function that reads or writes it, the
script native that exposes it, the table column it is loaded from, the string it is registered
under -- and cite that evidence in the member's comment when the name alone does not carry it. A
run you do not use stays padding. If you have to write a member, reverse it until you can name it.
When you resolve an existing `unk` member, rename it in the header **and** the IDB.

### 12. The game is the source of truth: read its state, never copy it

A field in the mod that mirrors something the game owns -- a member, a singleton, a global, an
inventory, a flag, a handle, what is loaded, equipped or open -- will drift. The copy is right
until the game changes the original without telling the mod; from then on every decision made
from it is wrong, silently.

**Therefore: when the mod needs a value the game keeps, reverse where the game keeps it and read
it there**, through a typed accessor on the class that owns it (rules 1 and 11), every time it is
needed. Learn about changes by hooking or calling the game's own function, not by diffing against
a `_last`/`_was` field. When the value is not mapped yet, the task is to map it.

Only two kinds of state may live in mod code: what the mod itself did and must undo (an entity it
spawned, a hook it installed), and data from MafiaMP's server that the game never holds.

## SDK file style

SDK files are minimal and clean:

- Offset comments on members (`// 0x00 - 0x08`). Brief section headers (`// Seat management`) are
  fine. `// TODO:` marks known incomplete work.
- No doxygen (`/** */`, `@param`, `@return`), no docstrings, no comment that repeats the function
  name, no vtable index comments.
- Members in the type they belong to, with their real type (rules 1, 6, 10).

```cpp
// Good
class C_Example {
  public:
    void SetSpeed(float speed);
    float GetSpeed() const;

  private:
    float m_fSpeed;  // 0x00 - 0x04
    char _pad4[0x4]; // 0x04 - 0x08

    static void AssertLayout() {
        static_assert(sizeof(C_Example) == 0x08, "C_Example size mismatch");
        static_assert(offsetof(C_Example, m_fSpeed) == 0x00, "C_Example::m_fSpeed offset mismatch");
    }
};

// Bad
class C_Example {
  public:
    /**
     * Set the speed value
     * @param speed The speed to set
     */
    void SetSpeed(float speed);
};
```

## Patterns and hooks

Every native address is a `gPatterns` member declared in `sdk/patterns.h` and resolved in
`sdk/patterns.cpp`, named `Class__Method` (an overload gets a suffix: `C_Car__ExplodeCar_2`). Use
`hook::get_pattern` for a function's own prologue and `hook::get_opcode_address` to follow a
`call`/`jmp` at a call site.

`hook::get_pattern` aborts the load when a signature does not match, so a running client has a
valid address in every entry: never null-check one (Framework rule 1). It returns the **first**
match, so a pattern that is not unique silently resolves to the wrong function -- confirm
uniqueness in `.text`, and confirm it lands on the function the name claims.

Hooks live in `core/hooks/`, one file per game subsystem, installed from a `static InitFunction`.
A hook stays thin: it detects the event, reads what it carries through typed SDK accessors, and
forwards to the module that owns the behaviour. Every detour runs on the game thread, so none may
block or re-enter the system that called it.

## Reverse engineering

The PC IDB is `mafiadefinitiveedition.i64` (`E:\Reverse Engineering\Mafia 1\`), image base
`0x140000000`. **Every address and offset that goes into the SDK comes from it.**
`mafia3_dump_mac.i64` is a symbolized Mafia 3 build on the same engine: use it as a **name source**
only. Its addresses and layouts are a different binary -- never paste one into MafiaMP code, and
re-prove any offset it suggests in the PC IDB.

When declaring structs in IDA, use `__cppobj` inheritance so derived classes declare only their own
fields and the decompiler shows `this->m_nFlags` instead of `this->base.m_nFlags`:

```c
struct __cppobj C_Entity {
    C_Entity_vtbl *__vftable;   // 0x00 - 0x08 (must be named __vftable)
    void *m_pSomeField;         // 0x08 - 0x10
};

struct __cppobj C_EntityPos : C_Entity {};

struct __cppobj C_Actor : C_EntityPos {
    void *m_pActorActions;      // 0xE0 - 0xE8
};
```

- The vtable field is named `__vftable`; the vtable struct is named `ClassName_vtbl`, which links
  it to the class. Apply it to the vtable's data address with `set_type`.
- MSVC `std::vector` is three pointers (`0x18`); declare a `vector_<T>` helper struct with
  `begin`, `end`, `capacity` for it in IDA (the C++ SDK uses `std::vector<T>`, rule 10).
- After changing a struct, `ida_hexrays.mark_cfunc_dirty(func_addr)` refreshes the decompilation.
- Keep the IDA struct and the SDK class in step; the SDK's `static_assert`s are the check.

**Don't loop on static analysis.** If a couple of decompile passes cannot resolve a behaviour
(streamed resources, runtime-only state), add a one-shot throttled log on the affected path and let
the game tell you. A build and a probe beat a fifth decompile.

When something crashes, read the crash before changing code. A crash with no MafiaMP frame on its
stack is the engine's own: find which call the mod should prevent, do not guard around it.

## Build

Build from this repository's root through the canonical Windows wrapper:

```bat
builds\build.bat MafiaMPClient 64
builds\build.bat MafiaMPServer 64
builds\build.bat MafiaMPLauncher 64
```

All use `builds/build-64`. Never invoke `cmake --build` directly and never create a separate or
temporary build tree; if the canonical one is broken, repair it.

The Framework comes as the prebuilt SDK named by `cmake/FrameworkPin.cmake` (a version and the
SHA-256 of each archive, pasted from the Framework release notes); `cmake/FrameworkBootstrap.cmake`
downloads it into `builds/framework-sdk` on the first configure. Set `FW_SOURCE_DIR` to a Framework
checkout before running the script to build the Framework from source alongside (the loop for a
change that spans both), or `FW_SDK_ROOT` to a locally installed SDK. The targets are
`Framework::Client`, `Framework::Server` and `Framework::Loader` in every mode. Raise the pin in its
own commit; a Framework MAJOR changes the build token, so client and server move together.

For a two-client test, start the server from `builds\build-64\bin`, then
`scripts\dbg-client-pair.ahk`. Never write to the game install, its configuration or save folders
to prepare a test (Framework rule 2). Stop the game processes you started when you are done.

## Versioning

`VERSION` is bumped by `.github/bump_version.sh` in the nightly `pr_tag_commit.yml` run, from the
paths a change touched. Never edit it by hand and never change the bump rules unless the user
asks. MAJOR is a wire break (netcode, shared entities and sync -- it locks out peers on the old
protocol), MINOR is the scripting layer, PATCH is everything else.

## Commit format

`Module: Brief commit description` (`Client:`, `Server:`, `SDK:`, `Scripting:`, `Build:`, `CI:`,
`Docs:`), wrapped at 72 characters. Split work into atomic commits. Commit and push only when
asked, and only what was asked.
