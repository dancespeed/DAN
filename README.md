# DAN Platform

> **Architecture first. Implementation follows.**

---

## Overview

DAN Platform is an extensible framework for developing embedded automation systems.

The project is built on the principle that architecture must always precede implementation. Every software component has a clearly defined responsibility, explicit interfaces and minimal dependencies, allowing the platform to evolve while preserving long-term maintainability.

DAN is not intended to be a single firmware project.

It is designed as a reusable engineering platform capable of supporting multiple hardware architectures and application domains while maintaining a consistent software architecture.

---

## Vision

Create a long-term embedded platform that provides a unified architectural foundation for automation systems, independent of a specific microcontroller, processor or hardware implementation.

---

## Engineering Principles

The platform follows several fundamental engineering principles.

- Architecture before implementation
- Incremental ("brick by brick") development
- Explicit module responsibilities
- Event-driven communication
- Single Source of Truth for documentation
- Documentation is part of the architecture
- Every engineering decision must be traceable

---

## Repository Structure

```
DAN/
│
├── src/          Platform source code
├── include/      Public interfaces
├── lib/          Project libraries
├── test/         Test environment
│
├── docs/         Engineering documentation
├── hardware/     Hardware documentation
├── assets/       Images and diagrams
├── tools/        Development utilities
│
├── platformio.ini
└── README.md
```

---

## Documentation

The documentation is organized according to the **Single Source of Truth** principle.

Each document has a single responsibility.

The primary documentation includes:

| Document | Purpose |
|----------|---------|
| DPAS | Platform architecture |
| DDS | Design decisions |
| EVENTS | Event model |
| HARDWARE | Hardware architecture |
| ROADMAP | Platform evolution |
| GLOSSARY | Project terminology |
| CODING_STANDARD | Development rules |
| VERSIONS | Engineering baselines and version history |

Module-specific documentation is located in:

```
docs/modules/
```

---

## Development Workflow

Every platform feature follows the same engineering lifecycle.

```
Architecture
      ↓
Design
      ↓
Documentation
      ↓
Implementation
      ↓
Verification
```

---

## Current Status

The DAN Platform is currently under active architectural development.

The software architecture is continuously refined together with its engineering documentation.

---

## Philosophy

A well-designed architecture should outlive the hardware it runs on.

The DAN Platform is developed with the goal that changing a microcontroller should require changing drivers rather than redesigning the system.

---

## License

License selection is intentionally postponed until the platform reaches its first stable architectural baseline.
