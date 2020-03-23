---
marp: true
paginate: true
title: Node-RED Dashboard
---

<!-- Source: https://www.youtube.com/watch?v=7QOWxuuGYh4 -->

# <!-- fit --> Node-RED Dashboard

Nico De Witte
bereikbaar via [nico.dewitte@vives.be](mailto:nico.dewitte@vives.be)

---

# Doel

De data die reeds binnengehaald is van de TTN nu afbeelden op een dashboard systeem van Node-RED.

![bg right fit](./img/goal.jpg)

---

# Node-RED Dashboard

Node-RED Dashboard is een module voor Node-RED die een toolbox aanlevert om heel snel een live dashboard te bouwen voor je Node-RED flow.

Website: [https://flows.nodered.org/node/node-red-dashboard](https://flows.nodered.org/node/node-red-dashboard)

![bg fit right](./img/dashboard.png)

---

# Installeren Node-RED Dashboard

Node-RED Dashboard is niet standaard meegeleverd met Node-RED maar moet apart worden geïnstalleerd als module.

Klik rechts bovenaan op de menu-knop (drie strepen onder elkaar) en kies de optie `Manage Palette`.

![bg fit left:30%](./img/manage_palette.png)

---

# Installeren Node-RED Dashboard

Open vervolgens het tabblad `Install` en zoek naar `dashboard`. Vervolgens klik je de `Install` knop voor de module `node-red-dashboard`.

![bg fit right:60%](./img/install-dashboard.png)

----

# Installeren Node-RED Dashboard

Klik op `Install` wanneer het dialoogvenster tevoorschijn komt.

![bg fit right](./img/install.png)

---

# Installeren Node-RED Dashboard

Als alles gelukt is zou er nu naast de module `Installed` moeten verschijnen.

Klik op Close om de overlay te sluiten en terug te keren naar de flow-editor.

![bg fit right](./img/installed.png)

---

# Node-RED Dashboard Nodes

In Node-RED vind je nu rechts tussen de andere nodes de categorie `dashboard`. Je vindt hier allerhande nodes voor grafische input en output via het dashboard.

![bg fit right:35%](./img/nodes.png)

---

# Raadplegen Dashboard

Het dashboard kan je raadplegen door te surfen naar de URL [http://localhost:1880/ui](http://localhost:1880/ui). Merk op dat dit gewoon de URL is Node-RED met `/ui` erachter (ui = user interface).

Voorlopig krijg je de melding dat er nog geen dashboard bestaat.

![bg fit left](./img/empty-dashboard.png)

---

# Introductie Node-RED Dashboard

Bekijk volgend filmpje vanaf **2min06** om de basis van Node-RED Dashboard te leren kennen:

[https://www.youtube.com/watch?v=X8ustpkAJ-U](https://www.youtube.com/watch?v=X8ustpkAJ-U)

Volg mee voor het bouwen van een flow en dashboard (doe dit in een nieuw werkblad). Klik bovenaan rechts op het plusje `+` om een nieuwe flow te maken.

Speel ook even zelf met de componenten.

![bg fit left:25%](./img/opdracht.png)

---

# Korte Samenvatting - Grafische Componenten

* Een dashboard bestaat uit grafische componenten die data afbeelden en interactie met de gebruiker toelaten.
  * Denk maar aan grafieken, knoppen, textinvoer, ...

![bg fit right](./img/ui-components.png)
![](./img/ui-components-temperatuur.png)


---

# Korte Samenvatting - Groups en Tabs

* De grafische elementen worden ingedeeld in groepen (groups) en deze worden afgebeeld op een tabblad (tabs).
  * De groups en tabs kan je beheren via de Layout manager (rechts bovenaan de dashboard knop).

![bg fit right](./img/tabs-groups-manager.png)

---

# Dashboard bouwen voor TTN

Tijd om een dashboard te bouwen voor onze applicatie (batterij monitoring systeem).

