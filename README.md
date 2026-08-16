<div align="center">

![Repo Traffic](https://komarev.com/ghpvc/?username=NguyenHieu267&label=Repo+Traffic&color=blue&style=flat-square)

</div>

# Blind Pacman - Game built on AK Embedded Base Kit

**Blind Pacman** is a limited-vision Pacman game developed for the STM32L151-based AK Embedded Base Kit. The game runs on the kit's 1.54-inch monochrome OLED and is controlled with only three physical buttons

<hr>

## Gameplay Demo

<div align="center">
  <video src="https://github.com/user-attachments/assets/0f2fb880-d670-41d3-9716-ef67b981e4bf" controls width="480"></video>
  <br>
</div>

## Documentation

| File | Description |
|---|---|
| [README.md](README.md) | Project overview, hardware information, gameplay rules, controls, and game logic. |
| [application/sources/app/pacman](application/sources/app/pacman) | Pacman game core, maze, player, ghost AI, rendering, settings, and EEPROM logic. |
| [application/sources/app/screens](application/sources/app/screens) | Welcome, menu, settings, ranking, victory, game-over, and QR screens. |
| [hardware/schematic/schematic-ak-embedded-base-kit-version-3.pdf](hardware/schematic/schematic-ak-embedded-base-kit-version-3.pdf) | AK Embedded Base Kit v3.0 schematic. |

## Introduction

Blind Pacman is a compact reinterpretation of the classic maze game. Instead of always showing the entire maze, the game can hide everything outside Pacman's vision radius. This turns navigation into a memory challenge, especially on the harder difficulty levels.

The project applies core embedded-software concepts on real hardware:

- **Event-driven design:** button events, periodic timers, task messages, and screen transitions coordinate the game.
- **State management:** gameplay, menus, settings, rankings, victory, and game-over screens are handled as separate states.
- **Persistent storage:** settings and the three fastest completion times are saved to EEPROM.
- **Real-time rendering:** a 16 × 32-tile maze is scrolled vertically and rendered on a 128 × 64 monochrome OLED.
- **Game AI:** four ghosts use different targeting strategies while navigating the maze.

### I. Hardware

<table align="center">
  <tr>
    <td align="center"><img src="hardware/images/ak-embedded-base-kit-version-3.jpg" alt="AK Embedded Base Kit - STM32L151 - v3.0" width="480"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 1:</em></strong> AK Embedded Base Kit - STM32L151</p>

The [AK Embedded Base Kit](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu) is an evaluation platform for learning embedded software development and event-driven system design.

The kit integrates a **1.54-inch OLED LCD**, **3 push buttons**, and **a buzzer** for graphics, input, and sound effects. It also provides **RS485**, the **Qwiic Connect System**, and **Grove** connectors for further embedded prototyping.

**MCU Overview:**

```text
SoC Name : STM32L151CBT6
RAM      : 16 KB

Flash Partitions Layout
-----------------------
[ 0x08000000 - 0x08001FFF ] : Bootloader Partition (8 KB)
=> AK Bootloader

[ 0x08002000 - 0x08002FFF ] : BSF Shared Partition (4 KB)
=> Data shared between the Bootloader and Application

[ 0x08003000 - 0x0801FFFF ] : Application Partition (116 KB)
=> Blind Pacman firmware
```

**MCU Naming Convention:**

| Part | Meaning |
|---|---|
| `STM32` | STMicroelectronics 32-bit MCU family. |
| `L` | Low-power series. |
| `151` | STM32L151 product line. |
| `C` | 48-pin package. |
| `B` | 128 KB Flash memory. |
| `T` | LQFP package. |
| `6` | Industrial temperature grade. |

<table align="center">
  <tr>
    <td align="center"><img src="hardware/images/board-view-top-bottom.png" alt="Top and bottom views of the AK Embedded Base Kit" width="900"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 2:</em></strong> Board view - Top and Bottom</p>

### II. Game Description and Objects

Blind Pacman challenges the player to clear the maze as quickly as possible while navigating with limited visibility and avoiding four ghosts.

The game opens on the **Pacman Menu**, which contains four options:

- **Play Game:** Start a new match.
- **Settings:** Configure difficulty, sound, and the time limit.
- **Ranking:** View the three fastest completion times stored in EEPROM.
- **Exit / QR:** Leave the menu and open the QR screen.

#### Objects in the Game

| Object | Description |
|---|---|
| **Pacman** | The player character, Pacman moves continuously in the selected direction and stops only when blocked by a wall. |
| **Dot** | A collectible placed throughout the maze. Clear every dot and cherry to win. |
| **Cherry** | A special collectible that temporarily frightens the ghosts, making them move unpredictably and allowing Pacman to send them back to the ghost house on contact. |
| **Blinky** | Directly targets Pacman's current position. |
| **Pinky** | Targets a point four tiles ahead of Pacman's current direction. |
| **Inky** | Calculates a surrounding target using both Pacman's projected position and Blinky's position. |
| **Clyde** | Chases Pacman from a distance, but retreats toward a maze corner when nearby. |
| **Maze** | A 16 × 32-tile map containing walls, paths, collectibles, and the central ghost house. The camera follows Pacman vertically. |

### III. How to Play

- Press **[MODE]** on the welcome screen to open the menu.
- In menus, use **[UP]** and **[DOWN]** to move the cursor, then press **[MODE]** to select.
- During gameplay, press **[DOWN]** to start moving horizontally or reverse between left and right.
- Press **[UP]** to start moving vertically or reverse between up and down.
- Press **[MODE]** during gameplay to return to the main menu.
- Eat every dot and cherry to win. Complete the maze as quickly as possible to enter the top-three ranking.

#### Game Mechanics

- **Blind vision:** Difficulty controls how much of the maze is visible around Pacman:
  - **Easy:** the full maze is visible.
  - **Medium:** objects within a 5-tile Manhattan radius are visible.
  - **Hard:** objects within a 2-tile Manhattan radius are visible.
- **Time limit:** The default limit is **40 seconds**. It can be set from **10 to 150 seconds** in 10-second increments, or disabled with **NO**.
- **Victory:** The match ends successfully after Pacman collects every dot and cherry. The completion time is saved if it belongs in the top three.
- **Time out:** If the countdown reaches zero before the maze is cleared, the **Game Over** screen is shown.
- **Ghost collision:** Contact with a normal ghost resets Pacman and all ghosts to their starting positions while the clock continues to run.
- **Frightened mode:** Eating a cherry temporarily changes all ghosts to a frightened state. Touching a frightened ghost sends it back to the ghost house.
- **Sound:** Menu feedback, collectibles, victory, and game-over events use the onboard buzzer. Sound can be enabled or disabled in **Settings**.
- **Persistent data:** Difficulty, sound, time-limit settings, and the three fastest times are stored in EEPROM.
