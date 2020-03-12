---
marp: true
paginate: true
title: Node-RED
---

<!-- Source: https://docs.allthingstalk.com/networks/use-the-things-network/ -->

# <!-- fit --> Node-RED

Nico De Witte
bereikbaar via [nico.dewitte@vives.be](mailto:nico.dewitte@vives.be)

---

# Doel

Binnenhalen van onze data uit de cloud (The Things Network) naar onze eigen services.

![bg right fit](./img/goal.jpg)

---

# Node-RED

Node-RED is een programmeertool voor het verbinden van hardware-apparaten, API's en online services.

Het biedt een browsergebaseerde editor die het gemakkelijk maakt om flows met elkaar te verbinden met behulp van het brede scala aan nodes die met een klik kunnen worden ingezet tijdens de uitvoering.

---

# Node.js gebasseerd

Node.js is een **open-source**, **platform-onafhankelijke** JavaScript runtime omgeving die JavaScript-code buiten een browser uitvoert.

De lichtgewicht runtime is gebouwd op Node.js en profiteert volledig van het event-based, non-blocking async model. Dit maakt het ideaal om aan de rand van het netwerk te draaien op goedkope hardware zoals de Raspberry Pi en in de cloud.

Met meer dan 225.000 modules in de packet repository van Node, is het eenvoudig om gamma aan nodes uit te breiden en zo nieuwe mogelijkheden toe te voegen.

---

# Installeren van Node.js

Start met installeren van Node.js. Surf hiervoor naar [https://nodejs.org/en/](https://nodejs.org/en/).

Selecteer de LTS (Long Term Support) versie (12.16).

Volg de wizard om Node.js te installeren.

---

# Testen van Node.js

Open een `powershell` venster en type volgende command:

```bash
node --version
```

De output zo ongeveer het volgende moeten zijn:

```bash
v12.16.1
```

---

# Installeren van Node-RED

Om Node-RED te installeren dien je volgende command uit te voeren in `powershell`:

```bash
npm install -g --unsafe-perm node-red
```

Herstart nu voor alle zekerheid je computer.

---

# Starten van Node-RED

Node-RED dient te worden opgestart. Dit kan je realiseren door onderstaand commando uit te voeren in een `powershell` venster:

```bash
node-red
```

Dit dien je telkens opnieuw te doen als je Node-RED wil gebruiken.

---

# Werken met Node-RED

Open een browser en surf naar [http://127.0.0.1:1880](http://127.0.0.1:1880).

![bg fit right:65%](./img/fresh-node-red.png)

