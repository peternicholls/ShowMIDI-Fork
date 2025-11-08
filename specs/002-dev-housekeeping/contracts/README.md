# Contracts: File Templates & Schemas

**Feature**: 002-dev-housekeeping  
**Date**: 2025-11-08  
**Purpose**: Define file templates, schemas, and automation contracts

---

## Overview

This directory contains:
1. **File Templates** - Concrete templates for configuration and documentation files
2. **Workflow Contracts** - GitHub Actions workflow trigger contracts
3. **Script Interfaces** - Input/output specifications for shell scripts

---

## File Organization

```
contracts/
├── README.md                    # This file
├── templates/                   # File templates
│   ├── .editorconfig.template
│   ├── .clang-format.template
│   ├── CHANGELOG.md.template
│   ├── CONTRIBUTING.md.template
│   ├── pull_request_template.md.template
│   └── .gitignore.additions
├── workflows/                   # Workflow trigger contracts
│   ├── ci.contract.yml
│   ├── release.contract.yml
│   └── stats.contract.yml
└── scripts/                     # Script interface specifications
    ├── test-build-local.interface.md
    └── update-download-links.interface.md
```

---

## Template Files

See `templates/` subdirectory for actual file templates.

---

## Workflow Contracts

See `workflows/` subdirectory for workflow trigger specifications.

---

## Script Interfaces

See `scripts/` subdirectory for script input/output specifications.

---

## Usage

These contracts serve as:
1. **Implementation Guide** - Developers implementing the feature follow these templates
2. **Validation Reference** - Code reviewers verify implementations match contracts
3. **Documentation** - Future maintainers understand file structure and automation behavior
