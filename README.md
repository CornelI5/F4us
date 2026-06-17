# F4us Security Ecosystem (v2.0)
> **"F for Us"** — A high-performance, event-driven, zero-JS Active EDR and Sandbox Ecosystem built for the community.

Licensed under the **GNU General Public License v3 (GPL-3.0)**. 

---

## 🌌 Overview

**F4us** (pronounced *Faust*) is a lightweight, bare-metal Endpoint Detection & Response (EDR) and automated containerized sandboxing engine written entirely in pure C. Designed with a strict **0% JavaScript** architectural mandate, it consumes near-zero CPU cycles during idle states and executes defense protocols within microseconds.

By utilizing native Linux kernel APIs (`inotify` and isolated namespaces), F4us monitors critical system directories, analyzes uploaded binaries or file modifications through Shannon Entropy calculations, and deploys active defensive countermeasures against malicious anomalies.

---

## 🛠️ Architecture & Modules

The ecosystem is engineered using a decoupled, asynchronous modular framework:
