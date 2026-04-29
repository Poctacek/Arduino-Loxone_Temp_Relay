# Loxone temperature-relay control for arduino

---
### What you need:

- Arduino UNO
- Arduino ENC28J60
- Relay for Arduino
- Arduino Temperature mod

---

### How it works:

 ***Arduino - Recieving is on port 60000 and sending is 60001***

1. Open Arduino IDE and import the code
2. You find your Loxone Miniserver's IP adress and edit the one I have in there
3. You send it to your Arduino and check the log for the IP adress of the Arduino
4. On Loxone config, you will want to recieve from port 60001 - you should then recieve the temperature
5. On Loxone you will then have to send a UDP packet with a message either "on" or "off" and that will then turn on/off the relay




---

*This is just a little project I made, during my first week on a school internship in OKNOPLASTIK, hope someone finds it interesting xd*