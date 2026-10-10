# GBC Emulator (working title)

A Game Boy Color emulator written from scratch in C++, with online and local link-cable emulation so you can trade and battle without a physical cable.

> **Status:** just started (10 Oct 2026). Nothing works yet. This README is temporary and will be rewritten as the project grows.

## Goals

- Accurate DMG and CGB emulation, passing the standard test ROMs
- Link cable emulation over TCP, with rooms (room ID plus optional password) so friends can connect over long distances
- Self-trade: run two instances in one process and trade with yourself
- Bluetooth as an extra link transport
- Quality-of-life features: save states, fast forward, shaders, cheats menu

## Planned Architecture

```
+-----------+     +-----------+     +-----------+
|    CPU    |<--->| Memory Bus|<--->|    PPU    |
+-----------+     +-----------+     +-----------+
                     |    |
              +------+    +------+
              |                  |
        +-----------+      +-----------+
        | Cartridge |      |  APU/Timer|
        |   (MBC)   |      |  /Serial  |
        +-----------+      +-----------+
                                |
                      +-------------------+
                      |    Link Layer     |
                      | send_byte()       |
                      | receive_byte()    |
                      +-------------------+
                       |        |        |
                  In-memory    TCP    Bluetooth
                  (self-trade) (rooms)
```

The link layer is a single interface with swappable backends, so the emulator core does not care how the bytes travel.

## Roadmap

- [ ] 0. Setup and ROM loading
- [ ] 1. CPU and memory bus
- [ ] 2. Timers and interrupts
- [ ] 3. PPU (DMG first)
- [ ] 4. Joypad and frame pacing
- [ ] 5. Cartridge MBCs and save files
- [ ] 6. CGB features (colour, banks, HDMA, double speed)
- [ ] 7. APU (audio)
- [ ] 8. Save states and fast forward
- [ ] 9. Shaders
- [ ] 10. Cheats menu
- [ ] 11. Accurate serial port
- [ ] 12. Link layer and self-trade
- [ ] 13. TCP backend
- [ ] 14. Relay server and rooms
- [ ] 15. Bluetooth backend
- [ ] 16. Final polish

Each phase ends with a test checkpoint before moving on (see `GBC_Emulator_Roadmap.pdf`).

## Tech Stack

- C++17 or newer
- CMake
- SDL2 (window, input, audio)
- OpenGL (shaders)
- Dear ImGui (cheats menu and settings)

## Building

Not set up yet. Planned:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./gbc path/to/rom.gbc
```

## Testing

Planned test ROMs:

- Blargg: `cpu_instrs`, `instr_timing`, `interrupt_time`, `dmg_sound`
- Mooneye test suite
- `dmg-acid2` and `cgb-acid2`

## Legal

This project does not include any copyrighted ROMs or BIOS files. You must supply your own legally obtained game dumps.

## References

- [Pan Docs](https://gbdev.io/pandocs/) - the main Game Boy technical reference
- [gbdev.io](https://gbdev.io/) - community resources and test ROMs

## License

TBD
