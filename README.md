# Runtime Deferred Linking in C++

## What is RDL?

RDL is a extension of traditional dynamic linking.

In short, the difference between RDL and standard dynamic linking is that with dynamic linking, symbol information has to be known at compile time, but RDL only needs to know symbol information at runtime.

![Flowchart showing the RDL compilation process.](https://raw.githubusercontent.com/BtheDestroyer/RDL/main/Building.png)

Note how the plugin DLLs and front-end EXE don't need to have knowledge of each other. This contrasts with traditional dynamic linking where the front-end EXE needs to know the symbol information for each of the DLLs it's trying to load in order to properly use their exported functions and variables.

## How is that possible?

The main EXE a plugin DLLs all communicate with a centralized [RDL Manager](https://github.com/BtheDestroyer/RDL/blob/007186414d546d3ee186039b72aa472027592702/Backend/RDL/RDL.h#L11) to register their functions and variables with a unique identifier. In this implementation, I'm using [hashed strings](https://github.com/BtheDestroyer/RDL/blob/main/Backend/Framework/HashedString.h).

To make this work, rather than the EXE (or intermediary DLLs) knowing what symbols to load at compile time, the unique identifiers are registered and retrieved at runtime from a universal table:

![Flowchart showing the RDL communication process.](https://raw.githubusercontent.com/BtheDestroyer/RDL/main/Communication.png)

While I would have liked to make this a header-only implementation, I don't think that's possible as everything needs to access the same RDL Manager across all compiled binaries in order to communicate properly.

## Potential uses

The best use for RDL would probably be modding APIs and plugins. For example, the main EXE could first register a handful of variables and functions with the RDL Manager, then all mods/plugins could use those registrations to interact with the host application. Modding libraries could also register their own functions and variables, though extra steps would need to be taken to ensure libraries have been fully loaded and registered before DLLs that rely on them try to use them. A good practice for avoiding issues when relying on RDL registrations is to *only* register objects in when a DLL is being loaded, *only* unregister objects when a DLL is being unloaded, and check if an object registration is valid before using it (this is easy if you use [RDL::Object](https://github.com/BtheDestroyer/RDL/blob/007186414d546d3ee186039b72aa472027592702/Backend/RDL/RDL.h#L122)).

A major benefit to RDL over traditional dynamic linking is the ability to "laterally communicate." Rather than having to know every possible symbol of every DLL before they're loaded, RDL allows DLLs to tell the EXE (and other DLLs) what objects are available to use.

## Current drawbacks

Currently there are some security issues in this implementation. Specifically, there's nothing checking which process is registering or unregistering objects, meaning a rogue DLL could forcefully unregister all existing objects.

In a similar vein, if two or more objects are registered with the same unique identifier, whichever is registered first will take priority. This can be mitigated by ensuring every name is unique, but with decentralized systems that's never guaranteed. My preferred method to avoid name conflicts is to use a domain-like system of naming. For example, I could register the object "foo" in my plugin "bar" as "dll.brycedixon.bar.foo" or "plugin.brycedixon.bar.foo". However, this then leads to an issue of overly-verbose names.
