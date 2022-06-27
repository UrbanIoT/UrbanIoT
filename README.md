# Mobile Environmental Monitoring 

## TODOS:

[ ] Problem beschreiben
[ ] Lösung beschreiben
[ ] Vorgehen beschreiben

[ ] VS Code + Plugins
[ ] Arduino + PlatformIO

[ ] Firmware: BME680 auslesen
[ ] Firmware: PAX Counter
[ ] Firmware: Daten per LoRa verschicken
[ ] Firmware: UI

[ ] TTN einrichten

[ ] Azure IoT Hub
[ ] Azure Event Hub
[ ] Azure SQL DB einrichten
[ ] Azure Storage Account
[ ] Azure Function für Daten in DB schreiben
[ ] Azure Function TTN Integration
[ ] TTN Azure IoT Hub Integration

[ ] Azure Maps
[ ] Azure WebService
[ ] Fronted

[ ] Hardware: Adapterboard
[ ] Hardware: Hülle

### UI
* Status: GPS Fix
* Status: LoRa join
* Status: Pakete senden
* Sensor Status: Temp, Hum, Druck, ...
* Einstellungen:
    * Sensorenauswahl - Welche Sensoren aktiv
    * LoRa Daten
    * LoRa Senderythmus


### Grafana Dashboard
grafana/grafana:8.5.4

GF_INSTALL_PLUGINS
grafana-simple-json-datasource 1.4.2,grafana-clock-panel 2.0.0,grafana-piechart-panel 1.6.2,grafana-worldmap-panel 0.3.3,ryantxu-ajax-panel 0.1.0,vonage-status-panel 1.0.11,grafana-sentry-datasource 1.0.1

GF_DATABASE_TYPE
sqlite3

GF_DATABASE_URL
sqlite3:///var/lib/grafana/grafana.db?cache=private&mode=rwc&_journal_mode=WAL

### Azure Cost Est.
https://azure.com/e/50f49e0c4c554796b976607ddce41de9