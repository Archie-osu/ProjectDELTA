# Project DELTA v3
Project DELTA - An open-source, modular mod menu for Toby Fox's games and their spin-offs.

**Important note to Grossley: Yes, it is out.**

Tested releases:
-  UNDERTALE v1.08
-  DELTARUNE
-  TS!UNDERSWAP

Libraries used: MinHook, ImGui, sol2

# TODO List (in order of how it will be done)
- Add a room selector into the main Menu (teleporting without Lua) - DONE
- Move project into bugfix-only mode for a good while - DONE
- Start work on Project V, the successor of Project DELTA. - In progress
  - *The following is for Project V, not Project DELTA*
  - C# Frontend with a Lua Executor and integrated injection options.
  - Index all GameMaker functions upon loading, loading progress bar.
  - Speedrun Mode, protect important game variables from being tampered with.
  - New menu, make it window based rather than tab-based.
  - Rework options menu, resolution changer, optional game fixes (fast text-skipping maybe?)
  - Runtime decompilation of game scripts
  - GML Editor, disassembler
  - Lua / Python modding API
  - and more.

# FAQ
**Q:** Where are the releases? Where can I download this tool?

**A:** Click [here](https://github.com/Archie-osu/ProjectDELTA/releases/latest)

**Q:** The loader says that it can't find ``ProjectDELTA.dll``, what is wrong?

**A:** The loader executable ``loader.exe`` must be in the same folder as the DLL ``ProjectDELTA.dll``. Try moving them to a folder on your desktop. If that doesn't work, message me on Discord.

**Q:** My Antivirus tells me this is a virus, why?

**A:** The loader comes with statically linked libraries, thus some AVs interpret it as malware. Simply make an exception in your Antivirus software. This is not malware.

# Special Thanks To:
elite\_pleb, colinator27, nkrapivin, \_creepersbane, Grossley (Moo)
