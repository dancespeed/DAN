# 1. Purpose

The **DAN Platform Architecture Specification (DPAS)** defines the architectural foundation of the DAN Platform.

Its purpose is to establish a common set of engineering principles, architectural rules and design constraints that guide the development of the platform throughout its entire lifecycle.

DPAS serves as the primary architectural reference for all software modules, hardware abstraction layers and engineering documentation within the project.

Every architectural decision should be consistent with this specification. If a proposed solution conflicts with DPAS, the architecture must be reviewed before implementation.

The objective of DPAS is not to describe implementation details, but to define the structure, responsibilities and relationships that ensure the platform remains modular, maintainable, extensible and independent of any specific hardware implementation.

# 2. Scope

This specification defines the architectural principles, structural organization and design rules of the DAN Platform.

DPAS specifies:

- the overall software architecture of the platform;
- the responsibilities and boundaries of architectural components;
- the interaction model between platform modules;
- the rules governing extensibility and maintainability;
- the principles of hardware abstraction;
- the relationship between architecture, implementation and documentation.

DPAS does not specify implementation details, programming techniques or hardware-specific solutions unless they directly affect the platform architecture.

Detailed implementation is documented separately in module documentation, design documents and source code.

# 3. Platform Vision

The DAN Platform is intended to become a long-term engineering framework for embedded automation systems.

The platform is designed around a stable software architecture that remains independent of any specific hardware implementation, microcontroller family or processor architecture.

DAN promotes a modular, event-driven design where system functionality is composed of independent components with clearly defined responsibilities and well-defined interfaces.

The long-term vision of the platform is to provide a reusable architectural foundation that enables the development of reliable, maintainable and extensible embedded systems while minimizing the impact of hardware evolution on software architecture.

The architecture shall encourage consistency, scalability and long-term maintainability rather than optimization for a single project or hardware platform.

# 4. Architectural Principles

The DAN Platform is governed by a set of fundamental architectural principles.

These principles define how the platform shall be designed, developed and evolved. Every architectural decision shall be evaluated against these principles.

## 4.1 Architecture First

Architecture shall always precede implementation.

No software component shall be implemented before its architectural role, responsibilities and interfaces have been defined and documented.

---

## 4.2 Incremental Development

The platform shall be developed incrementally.

Complex systems shall be built through a sequence of small, verifiable architectural steps rather than large, monolithic implementations.

This principle is referred to within the project as **Brick by Brick Development**.

---

## 4.3 Single Responsibility

Every architectural component shall have one clearly defined responsibility.

A module should solve one problem and expose one coherent purpose.

---

## 4.4 Explicit Interfaces

Modules shall communicate only through well-defined public interfaces.

Internal implementation details shall remain hidden from other modules.

---

## 4.5 Loose Coupling

Dependencies between modules shall be minimized.

Whenever possible, communication shall occur through abstract interfaces and platform events rather than direct module interaction.

---

## 4.6 Event-Driven Architecture

Platform communication shall be based on events.

Modules should react to events rather than directly controlling each other whenever practical.

---

## 4.7 Hardware Independence

The software architecture shall remain independent of hardware implementation.

Hardware-specific functionality shall be isolated inside dedicated driver layers.

Replacing hardware should not require redesigning the platform architecture.

---

## 4.8 Documentation First

Architecture documentation is an integral part of the engineering process.

Architectural changes shall be documented before implementation.

Documentation is considered part of the product rather than supplementary material.

---

## 4.9 Single Source of Truth

Every architectural concept shall have one authoritative source of documentation.

Information shall not be duplicated across multiple documents unless explicitly required.

---

## 4.10 Continuous Evolution

The platform architecture is expected to evolve.

Architectural improvements are encouraged provided they preserve consistency, maintainability and the long-term vision of the platform.

These principles are mandatory for all platform components unless an explicit architectural exception has been approved and documented.

# 5. Layered Architecture

The DAN Platform follows a layered architecture.

Each layer has a clearly defined responsibility and communicates only with adjacent architectural layers through well-defined interfaces.

The purpose of the layered architecture is to separate business logic from platform services and hardware implementation while maintaining a predictable flow of information.

The architectural layers are:

```
+------------------------------------------------------+
|                  Application Layer                   |
+------------------------------------------------------+
|                   Logic Layer                        |
+------------------------------------------------------+
|               Platform Services Layer                |
+------------------------------------------------------+
|              Driver Abstraction Layer                |
+------------------------------------------------------+
|                 Hardware Layer                       |
+------------------------------------------------------+
```

---

## 5.1 Application Layer

The Application Layer contains application-specific behaviour.

It defines **what** the platform should accomplish without depending on hardware implementation.

---

## 5.2 Logic Layer

The Logic Layer contains decision-making components.

It processes events, evaluates system state and generates new actions.

Business logic shall never directly access hardware.

---

## 5.3 Event & Service Layer

The Event & Service Layer provides communication and common platform services.

Typical responsibilities include:

- event distribution;
- system services;
- scheduling;
- platform coordination.

Modules communicate through this layer rather than directly.

---

## 5.4 Driver Abstraction Layer

The Driver Layer isolates hardware-specific implementation.

Every hardware device shall be accessed exclusively through its corresponding driver.

Drivers expose stable interfaces while hiding implementation details.

---

## 5.5 Hardware Layer

The Hardware Layer represents the physical devices used by the platform.

Examples include:

- microcontrollers;
- GPIO;
- communication peripherals;
- sensors;
- actuators;
- external integrated circuits.

Hardware components shall never influence the software architecture directly.

# 6. Core Components

The DAN Platform is composed of independent architectural components.

Each component has a clearly defined responsibility and interacts with the rest of the platform only through approved interfaces and platform events.

The current platform architecture consists of the following core components.

| Component | Primary Responsibility |
|-----------|------------------------|
| System | Platform initialization, lifecycle management and common services |
| Event | Definition of platform events |
| EventBus | Event distribution between platform components |
| Input | Processing of user and external inputs |
| Logic | Decision making and system behaviour |
| PWM | High-level PWM service |
| Sequencer | Time-based execution of ordered actions |
| Drivers | Hardware abstraction and device access |

Each component is documented independently within the module documentation.

Architectural responsibilities defined in this specification take precedence over implementation details.

# 7. Event Model

The DAN Platform uses an event-driven communication model.

Events represent meaningful changes, requests or notifications within the platform. They allow components to exchange information without creating direct dependencies between their implementations.

A component may publish an event without knowing which components will receive it. A receiving component reacts to the event according to its own responsibility.

This model reduces coupling and allows platform components to evolve independently.

---

## 7.1 Event Flow

The typical event flow within the platform is:

```text
Input
    ↓
Event
    ↓
EventBus
    ↓
Logic
    ↓
Command Event
    ↓
Platform Service
    ↓
Driver
    ↓
Hardware
```

Events shall describe intent or state changes rather than implementation details.

For example, a platform component should publish:

```text
SetChannelState
```

rather than:

```text
WriteValueToPCA9685Register
```

The first event expresses platform intent.

The second exposes hardware implementation and therefore violates hardware independence.

---

## 7.2 Event Responsibilities

The Event component defines the common event representation used by the platform.

The EventBus component is responsible for distributing events between publishers and subscribers.

Event producers shall not depend on the internal implementation of event consumers.

Event consumers shall process only events relevant to their documented responsibility.

---

## 7.3 Event Categories

Platform events may represent:

- external input;
- user commands;
- state changes;
- service requests;
- service responses;
- system lifecycle notifications;
- hardware-independent actions;
- errors and diagnostic conditions.

The complete event taxonomy and event definitions are documented in `EVENTS.md`.

---

## 7.4 Event Immutability

An event shall not be modified after publication.

Consumers may derive and publish new events, but they shall not alter the original event.

This ensures that all consumers receive the same information and prevents hidden dependencies between handlers.

---

## 7.5 Layer Isolation

Components shall not bypass architectural layers through direct dependencies.

In particular:

- application-specific behaviour shall not access hardware directly;
- logic components shall not depend on concrete drivers;
- drivers shall not contain application or business logic;
- hardware details shall not appear in platform-level events;
- lower layers shall not depend on higher layers.

Communication across architectural boundaries shall use approved public interfaces or platform events.

Any exception to this rule requires an explicit architectural decision documented in `DDS.md`.

# 8. Module Responsibilities

Each platform component has a single, clearly defined architectural responsibility.

A component shall not assume responsibilities assigned to another component.

Implementation details are documented separately within the module documentation.

---

## 8.1 System

**Responsibility**

- platform initialization;
- lifecycle management;
- platform-wide configuration;
- coordination of platform startup and shutdown.

**The System component shall not:**

- implement application logic;
- access hardware directly unless acting through approved drivers.

---

## 8.2 Event

**Responsibility**

- define the common event representation used throughout the platform.

**The Event component shall not:**

- distribute events;
- execute platform logic.

---

## 8.3 EventBus

**Responsibility**

- distribute events between platform components;
- decouple event producers from event consumers.

**The EventBus component shall not:**

- modify events;
- contain business logic;
- perform hardware operations.

---

## 8.4 Input

**Responsibility**

- receive external inputs;
- translate hardware or user interactions into platform events.

**The Input component shall not:**

- make application decisions;
- execute hardware-specific logic outside approved drivers.

---

## 8.5 Logic

**Responsibility**

- evaluate platform state;
- implement decision-making;
- generate new platform events.

**The Logic component shall not:**

- access hardware directly;
- contain driver-specific code.

---

## 8.6 PWM

**Responsibility**

- provide a hardware-independent PWM service;
- translate platform requests into driver operations.

**The PWM component shall not:**

- expose hardware-specific implementation details;
- implement application behaviour.

---

## 8.7 Sequencer

**Responsibility**

- execute ordered time-based actions;
- coordinate scheduled platform operations.

**The Sequencer component shall not:**

- contain hardware-specific code;
- implement application-specific behaviour.

---

## 8.8 Drivers

**Responsibility**

- provide hardware abstraction;
- communicate with physical devices;
- expose stable interfaces to upper layers.

**The Drivers component shall not:**

- implement business logic;
- make platform decisions;
- communicate directly with application components.

---

Each module shall remain independent, reusable and compliant with the architectural principles defined in this specification.

# 9. Platform Rules

The following rules are mandatory for every component within the DAN Platform.

Compliance with these rules ensures architectural consistency, maintainability and long-term extensibility.

---

## 9.1 Single Responsibility

Each component shall have one clearly defined architectural responsibility.

A responsibility shall not be duplicated across multiple components.

---

## 9.2 Explicit Interfaces

Components shall communicate only through documented public interfaces or platform events.

Internal implementation details shall remain private.

---

## 9.3 Event-Based Communication

Whenever practical, components shall exchange information through the platform event system.

Direct dependencies shall be minimized.

---

## 9.4 Hardware Independence

Platform components shall remain independent of specific hardware implementations.

Hardware-specific functionality shall exist exclusively within the Driver Abstraction Layer.

---

## 9.5 Layer Integrity

Components shall communicate only with adjacent architectural layers.

Architectural layers shall not be bypassed through direct dependencies.

---

## 9.6 Separation of Concerns

Business logic, platform services and hardware access shall remain separated.

No component shall combine responsibilities belonging to different architectural layers.

---

## 9.7 Documentation Consistency

Every architectural component shall have corresponding documentation.

Architectural documentation shall be updated whenever architectural behaviour changes.

---

## 9.8 Backward Compatibility

Architectural changes shall preserve existing platform behaviour whenever reasonably possible.

Breaking architectural changes shall be explicitly documented in `DDS.md`.

---

## 9.9 Approved Exceptions

Any intentional deviation from these rules shall require an explicit architectural decision.

Exceptions shall be documented together with their motivation and expected impact.

# 10. Extensibility

The DAN Platform is designed to support long-term growth through modular extension.

New functionality shall be added by introducing new components or extending existing ones while preserving the established architectural principles.

---

## 10.1 Modular Growth

Platform functionality shall evolve by adding independent modules rather than modifying unrelated components.

Each new module shall have a clearly defined architectural responsibility.

---

## 10.2 Stable Interfaces

Public interfaces shall remain as stable as reasonably possible.

Changes affecting public interfaces shall be carefully evaluated for their impact on existing components.

---

## 10.3 Event Compatibility

New functionality should integrate with the existing event model whenever practical.

New events shall follow the common platform event conventions.

---

## 10.4 Driver Extensibility

Support for additional hardware shall be introduced by implementing new drivers within the Driver Abstraction Layer.

Higher architectural layers shall not require modification solely because new hardware has been added.

---

## 10.5 Component Independence

New modules shall avoid creating unnecessary dependencies.

Components should remain reusable outside their original application whenever practical.

---

## 10.6 Architectural Consistency

Every new platform component shall comply with the architectural principles and platform rules defined in this specification.

If a new requirement cannot be implemented within the existing architecture, the architecture shall be reviewed before implementation begins.

# 11. Hardware Independence

The DAN Platform is designed to operate independently of any specific hardware platform.

The platform architecture shall remain unchanged when supporting different microcontrollers, communication interfaces or peripheral devices.

Hardware-specific implementation shall be isolated within the Driver Abstraction Layer.

---

## 11.1 Platform Independence

Platform components shall not rely on hardware-specific APIs, registers or communication protocols.

Hardware dependencies shall be encapsulated by platform drivers.

---

## 11.2 Driver Isolation

Each hardware device shall be represented by a dedicated driver or driver family.

Drivers shall provide stable interfaces to higher architectural layers while hiding implementation details.

---

## 11.3 Hardware Replacement

Replacing one hardware device with another shall require modifications only within the corresponding driver implementation.

Higher architectural layers should continue to operate without architectural changes.

---

## 11.4 Portable Architecture

The same platform architecture should be applicable to different execution environments, including but not limited to:

- STM32 microcontrollers;
- ESP-family microcontrollers;
- Raspberry Pi;
- Linux-based systems;
- future supported platforms.

Hardware support shall extend the platform rather than redefine its architecture.

# 12. Documentation Rules

Documentation is an integral part of the DAN Platform architecture.

Architectural documentation shall accurately describe the intended structure, responsibilities and behaviour of the platform.

Documentation is considered a primary engineering artifact and shall evolve together with the platform.

---

## 12.1 Single Source of Truth

Architectural decisions shall be documented in a single authoritative location.

Duplicate or conflicting architectural documentation shall be avoided.

---

## 12.2 Architecture Before Implementation

Architectural changes shall be documented before implementation begins.

Implementation shall follow the approved architecture.

---

## 12.3 Documentation Maintenance

Architectural documentation shall be reviewed and updated whenever architectural changes are introduced.

Obsolete documentation shall be corrected or removed.

---

## 12.4 Module Documentation

Each architectural component shall have its own dedicated documentation describing:

- purpose;
- architectural responsibility;
- public interfaces;
- interactions with other platform components.

Implementation details shall remain outside the architectural specification whenever practical.

---

## 12.5 Architectural Decisions

Significant architectural decisions, deviations and approved exceptions shall be documented in `DDS.md`.

The architectural specification (`DPAS.md`) defines the platform architecture.

The design decision log (`DDS.md`) records why architectural decisions were made.

# 13. Versioning

The DAN Platform architecture evolves in a controlled and documented manner.

Every architectural revision shall preserve consistency, traceability and long-term maintainability.

---

## 13.1 Architectural Revision

The architectural specification shall be version controlled.

Each published revision shall represent a coherent and internally consistent architectural state.

---

## 13.2 Controlled Evolution

Architectural changes shall be introduced incrementally.

Large architectural refactoring should be decomposed into smaller, reviewable changes whenever practical.

---

## 13.3 Change Documentation

Significant architectural modifications shall be recorded in `DDS.md`.

Major platform milestones shall be documented in `CHANGELOG.md`.

Version history shall be maintained in `VERSIONS.md`.

---

## 13.4 Compatibility

Architectural evolution should preserve existing platform concepts whenever reasonably possible.

When compatibility cannot be maintained, the architectural impact shall be explicitly documented before implementation.

---

## 13.5 Engineering Process

The development of the DAN Platform follows a consistent engineering workflow:

1. Identify a new requirement.
2. Review the existing architecture.
3. Update the architecture if necessary.
4. Document the architectural decision.
5. Implement the approved design.
6. Verify the implementation against the architecture.

This process ensures that architecture remains the primary reference throughout the platform lifecycle.
