# RPCS3 Modern UI Roadmap

This fork keeps RPCS3's emulation and networking behaviour intact while rebuilding the desktop frontend in small, reviewable stages.

## Stage 0 — Preview shell

- Add a standalone modern Qt shell under `rpcs3/rpcs3qt/modern/`.
- Inject a **View → Modern UI Preview** action without modifying emulator-core code.
- Keep the existing RPCS3 interface as the safe fallback during development.

## Stage 1 — Real library data

- Read installed-game data from the existing `game_list_frame` / `game_info` model.
- Render that data as modern cards.
- Preserve current boot, compatibility, custom configuration and context-menu behaviour.

## Stage 2 — Game details

- Add a selected-game hero/details view.
- Surface compatibility, serial, category, playtime, last played and storage information.
- Route Play/Resume actions through the existing RPCS3 boot path.

## Stage 3 — Settings and controllers

- Reorganize settings into a cleaner navigation model while retaining existing configuration values.
- Build a visual controller setup experience on top of RPCS3's existing pad configuration.

## Stage 4 — RPCN

- Keep RPCS3's existing RPCN implementation and protocol handling.
- Replace only the frontend for account/server/status management.

## Upstream policy

Avoid changing `rpcs3/Emu/NP` or other emulator-core systems for visual work. Prefer adapter layers in `rpcs3qt/modern` so upstream RPCS3 changes remain easier to merge.
