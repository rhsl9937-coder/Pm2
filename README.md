# Princess Life DS — fan-made original prototype

This is an original, tiny Nintendo DS homebrew prototype inspired by the *genre* of
princess-raising simulations. It is not a copy of the commercial Princess Maker 2 game.

## What's in this prototype
- A simple status screen
- Four actions: Study, Work, Rest, and Go Out
- Basic stat changes, money, stress, and a calendar
- A minimal ending check after 12 months

## Build
The GitHub Actions workflow runs the official `devkitpro/devkitarm` container,
builds the `.nds`, and uploads it as an artifact.

1. Upload the contents of this folder to a new GitHub repository.
2. Open the repository's **Actions** tab.
3. Run **Build Nintendo DS game** (or push a commit).
4. Open the completed run and download the `princess-life-ds` artifact.
5. Unzip the artifact to get `princess_life_ds.nds`.

## Important
- This project has not been compiled or tested on physical DS hardware in this chat.
- Use a compatible homebrew setup/flashcart or emulator to run homebrew.
- Use original or properly licensed artwork, music, names, and writing. Do not include
  extracted assets or text from the commercial Princess Maker 2 game.
