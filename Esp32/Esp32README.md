Great question — here's the full explanation:

---

### 🎯 **Your IPv4 Address Keeps Changing**

By default, most Wi-Fi routers use **DHCP (Dynamic Host Configuration Protocol)** to **automatically assign IP addresses** to devices (like your PC) on the local network. This means:

- Every time your PC reconnects to the network or reboots,
- Or if the router restarts / reassigns IPs,
- It might get a **new IP address** (e.g., `192.168.1.10` → `192.168.1.14`)

That’s why you see connection issues with the ESP32 — the hardcoded IP in the ESP32 code is no longer valid.

---



