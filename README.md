# IoT - OOP

Introductie in IoT voor de graduaat opleiding van VIVES.

## Serving Slides Locally

```bash
npx @marp-team/marp-cli --allow-local-files -s ./slides
```

You need to export path to chome via env variable or via full command:

```bash
export CHROME_PATH="/usr/bin/chromium" ; npx @marp-team/marp-cli --allow-local-files -s ./01-internet-of-things
```

## Converting to PDF

```bash
npx @marp-team/marp-cli slide-deck.md --allow-local-files -o converted.pdf
```
