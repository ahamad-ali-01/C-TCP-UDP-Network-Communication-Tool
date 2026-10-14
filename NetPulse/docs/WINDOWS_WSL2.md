# Windows and WSL2 setup

## Install WSL2 (Windows PowerShell as Administrator)
```powershell
wsl --install -d Ubuntu-24.04
```
Restart if Windows requests it, launch Ubuntu from the Start menu, and create the Linux username/password. Check status with `wsl --status` and `wsl -l -v`; the Ubuntu distribution should use version 2.

## Build in Ubuntu
Prefer the Linux filesystem for source/build performance:
```bash
mkdir -p ~/projects && cd ~/projects
# Put/extract the NetPulse project here, then:
cd NetPulse
sudo apt update
sudo apt install -y build-essential cmake git zip unzip
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```
If the ZIP is in Windows Downloads, it is usually accessible under `/mnt/c/Users/<WindowsUser>/Downloads/`; copy it into `~/projects` and unzip it there. Replace `<WindowsUser>` with the actual Windows account folder.

## Two terminal windows
Terminal A: `./build/netpulse server --protocol tcp --host 127.0.0.1 --port 9000`
Terminal B: `./build/netpulse client --protocol tcp --host 127.0.0.1 --port 9000`
For UDP, use `--protocol udp --port 9001` in both commands. Local loopback testing is the simplest check.

## LAN / remote access
To listen on all Linux IPv4 interfaces, explicitly use `--host 0.0.0.0`. Find addresses with `ip -brief address` or `hostname -I`. `0.0.0.0` is only a bind address; clients must connect to a reachable actual IP. On a standard Ubuntu host, use the host's LAN IPv4 address and configure UFW/router/firewall as appropriate. Avoid exposing the service to the public internet.

WSL2 often uses NAT by default; the WSL guest address may differ from the Windows LAN address and may change after restart. Windows-to-WSL localhost forwarding often works for local use, but inbound connections from another LAN computer may require Windows port forwarding or WSL mirrored networking. Check current Microsoft WSL networking guidance for the installed WSL version. For firewall diagnostics, inspect Windows Defender Firewall and Linux firewall rules; allow the TCP port and UDP port separately. Do not disable the firewall globally.

## Diagnose connectivity
- Confirm listener: `ss -ltnp` for TCP and `ss -lunp` for UDP.
- Check the correct protocol/port and server logs.
- Test loopback before testing LAN/WSL routing.
- `Connection refused` usually means no listener/reject; timeout commonly indicates routing/firewall/drop.
- Check port collision with `ss -ltnup` and choose another port if necessary.
