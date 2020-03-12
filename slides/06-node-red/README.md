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

Node-RED is een flow gebaseerde visuele ontwikkelomgeving ontwikkeld door IBM. Het laat toe hardware toestellen, API's en online diensten met elkaar te communiceren in het teken van het Internet of Things.

![bg fit right:30%](./img/node-red-logo.png)

---

# Node-RED

Het biedt een browsergebaseerde editor die het gemakkelijk maakt om flows met elkaar te verbinden met behulp van het brede scala aan nodes die met een klik kunnen worden ingezet tijdens de uitvoering.

![bg fit right:65%](./img/fresh-node-red.png)

---

# Node-RED

Flows kunnen tevens heel makkelijk worden geëxporteerd of geïmporteerd als JSON.

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

![bg 70%](./img/nodered-started.png)

---

# Werken met Node-RED

Open een browser en surf naar [http://127.0.0.1:1880](http://127.0.0.1:1880).

![bg fit right:65%](./img/fresh-node-red.png)

---

# ???????

Merk op dat een node die data genereert (uitvoer) aan de rechterkant kan gekoppeld worden en een node die data binnen neemt (invoer) aan de linkerkant gekoppeld kan worden.

---

# ???????

---

# ???????

---

# Data van TTN binnenhalen

Om data van The Things Network binnen te halen dient er een flow gebouwd te worden. De connectie met TTN wordt gemaakt aan de hand van MQTT.

---

# MQTT

MQTT is één van de meest gebruikte protocollen voor Internet of Things apparaten te laten communiceren met elkaar.

MQTT staat voor Message Queuing Telemetry Transport. Het is een zeer lichtgewicht berichten protocol dat gebruik maakt van publish/subscribe mechanisme om gegevens tussen verschillende clients uit te wisselen.

Het is klein in formaat, laag in vermogenverbruik en gebruikt geminimaliseerde data paketten, ideaal voor 'machine tot machine' of Internet of Things.

---

# Publiceren en abonneren

Stel dat je een temperatuur sensor hebt. Deze wil zijn waarden doorsturen naar een MQTT broker. Aan de andere kant hebben we toestellen zoals computers en smartphones die deze waarden willen ontvangen om ze weer te geven of te verwerken. Dan zijn er twee dingen die gebeuren.

* De sensor geeft een **topic** op waaronder het zijn gegevens zal publiceren. Bijvoorbeeld `temperatuur`. Dan zal het zijn temperatuurwaarde publiceren.

* Iedereen die de gegevens wil ontvangen kan zich dan gaan abonneren op `temperatuur`. Elke keer dat de sensor nieuwe gegevens publiceert worden alle abonnees automatisch verwittigd met de nieuwe temperatuur waarde.

---

![bg 80%](./img/mqtt.png)

---

# MQTT broker van TTN

Bij onze opstelling is er echter wel een groot verschil!

De sensor published zijn waarden bij ons niet via MQTT maar wel via LoRaWAN naar de TTN. The Things Network stelt echter onze data ter beschikking via MQTT. Zij voorzien dus met andere woorden de broker waar wij kunnen subscriben op onze data.

---

# MQTT Input

Zoek de node `mqtt input` in de lijst aan de linker kant, en sleep deze naar het centrale deel.

Vervolgens kunnen we ook een `debug` node aan de uitgang van de `mqtt input` node koppelen zodat we de informatie makkelijk kunnen raadplegen die van The Things Network komt.

![bg fit right](./img/mqtt-in-debug.png)

---

# MQTT Input - Configuratie

Dubbelklik op de mqtt input node om de configuratie te tonen. Hier dienen we de broker van The Things Network in te stellen.

![bg fit right](./img/mqtt-edit.png)

---

# MQTT Input - Connectie

Klik op de knop met het potlood icoontje, rechts naast het invoerveld `Add new mqtt broker...`. Eerst en vooral kan je de broker configuratie best een duidelijk naam geven zoals `TTN-vehicle-battery-monitor`.

Vervolgens dienen we in het tabblad **Connection** de server in te stellen op `eu.thethings.network`.

![bg fit right:45%](./img/broker-connection.png)

---

# MQTT Input - Security

Vervolgens moeten we een bruikersnaam en wachtwoord ingeven in het tabblad **Security**. Hiervoor moeten we het `App ID` (username) en de `Access Key` (password) van de The Things Network applicatie achterhalen. Deze gegevens kan je terugvinden op de console van The Things Network.

![bg fit right](./img/broker-security.png)

Klik vervolgens bovenaan rechts op de knop `Add`.

---

# MQTT Input - Het Topic instellen

Als laatste dienen we ook in te stellen waar onze data kan worden bekomen. Dit doen we door het topic in te stellen op basis van de naam die we aan ons device hebben gegeven. Dit kunnen we terug vinden op de [console](https://console.thethingsnetwork.org/) van The Things Network. Klik bovenaan rechts op Devices en kopieer de naam van je device.

![bg fit right](./img/mqtt-topic-empty.png)

---

# MQTT In - Het Topic Instellen

Het `device ID` dien je in het topic `+/devices/XXXX/up` in te vullen in plaats van `XXXX`.

Voor een device met id `sodaq-temperature-sensor` wordt dit dus: `+/devices/sodaq-temperature-sensor/up`

![bg fit right](./img/mqtt-topic-configured.png)


