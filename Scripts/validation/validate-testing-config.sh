#!/usr/bin/env bash
#
# ShowMIDI Testing Governance Configuration Validator
# Copyright (C) 2025 Peter Nicholls.  https://www.peternicholls.me.uk
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CONFIG_FILE="${1:-.github/testing-governance.yaml}"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

error() {
    echo -e "${RED}ERROR: $*${NC}" >&2
}

success() {
    echo -e "${GREEN}✓ $*${NC}"
}

warning() {
    echo -e "${YELLOW}⚠ $*${NC}"
}

info() {
    echo "$*"
}

# Check prerequisites
check_prerequisites() {
    local missing=()
    
    if ! command -v yq >/dev/null 2>&1; then
        missing+=("yq (install via: brew install yq)")
    fi
    
    if [ ${#missing[@]} -gt 0 ]; then
        error "Missing required tools:"
        for tool in "${missing[@]}"; do
            error "  - $tool"
        done
        exit 1
    fi
}

# Validate YAML syntax
validate_yaml_syntax() {
    info "Validating YAML syntax..."
    
    if ! yq eval '.' "$CONFIG_FILE" >/dev/null 2>&1; then
        error "Invalid YAML syntax in $CONFIG_FILE"
        return 1
    fi
    
    success "YAML syntax valid"
    return 0
}

# Validate version
validate_version() {
    info "Validating version..."
    
    local version
    version=$(yq eval '.version' "$CONFIG_FILE")
    
    if [ "$version" = "null" ] || [ -z "$version" ]; then
        error "Missing version field"
        return 1
    fi
    
    success "Version: $version"
    return 0
}

# Validate check categories structure
validate_check_categories() {
    info "Validating check categories..."
    
    local categories
    categories=$(yq eval '.check_categories | length' "$CONFIG_FILE")
    
    if [ "$categories" -eq 0 ]; then
        warning "No check categories defined"
        return 0
    fi
    
    success "Found $categories check categories"
    
    # Validate each category has required fields
    local errors=0
    for i in $(seq 0 $((categories - 1))); do
        local id
        id=$(yq eval ".check_categories[$i].id" "$CONFIG_FILE")
        
        if [ "$id" = "null" ]; then
            error "Check category at index $i missing 'id' field"
            ((errors++))
            continue
        fi
        
        # Check required fields
        for field in name purpose tools owner blocking_policy expected_duration_sec timeout_sec; do
            local value
            value=$(yq eval ".check_categories[$i].$field" "$CONFIG_FILE")
            if [ "$value" = "null" ]; then
                error "Check category '$id' missing required field '$field'"
                ((errors++))
            fi
        done
    done
    
    if [ $errors -gt 0 ]; then
        return 1
    fi
    
    return 0
}

# Validate trigger contexts
validate_trigger_contexts() {
    info "Validating trigger contexts..."
    
    local contexts
    contexts=$(yq eval '.trigger_contexts | length' "$CONFIG_FILE")
    
    if [ "$contexts" -eq 0 ]; then
        warning "No trigger contexts defined"
        return 0
    fi
    
    success "Found $contexts trigger contexts"
    
    # Validate each context
    local errors=0
    for i in $(seq 0 $((contexts - 1))); do
        local id
        id=$(yq eval ".trigger_contexts[$i].id" "$CONFIG_FILE")
        
        if [ "$id" = "null" ]; then
            error "Trigger context at index $i missing 'id' field"
            ((errors++))
            continue
        fi
        
        # Check required fields
        for field in event_type scope time_budget_sec; do
            local value
            value=$(yq eval ".trigger_contexts[$i].$field" "$CONFIG_FILE")
            if [ "$value" = "null" ]; then
                error "Trigger context '$id' missing required field '$field'"
                ((errors++))
            fi
        done
    done
    
    if [ $errors -gt 0 ]; then
        return 1
    fi
    
    return 0
}

# Validate policies
validate_policies() {
    info "Validating policies..."
    
    local policies
    policies=$(yq eval '.policies | length' "$CONFIG_FILE")
    
    if [ "$policies" -eq 0 ]; then
        warning "No policies defined"
        return 0
    fi
    
    success "Found $policies policies"
    return 0
}

# Validate owners
validate_owners() {
    info "Validating owners..."
    
    local owners
    owners=$(yq eval '.owners | length' "$CONFIG_FILE")
    
    if [ "$owners" -eq 0 ]; then
        warning "No owners defined"
        return 0
    fi
    
    success "Found $owners owners"
    return 0
}

# Main validation
main() {
    info "=== ShowMIDI Testing Governance Validator ==="
    info "Configuration file: $CONFIG_FILE"
    info ""
    
    if [ ! -f "$CONFIG_FILE" ]; then
        error "Configuration file not found: $CONFIG_FILE"
        exit 1
    fi
    
    check_prerequisites
    
    local failed=0
    
    validate_yaml_syntax || ((failed++))
    validate_version || ((failed++))
    validate_check_categories || ((failed++))
    validate_trigger_contexts || ((failed++))
    validate_policies || ((failed++))
    validate_owners || ((failed++))
    
    info ""
    if [ $failed -eq 0 ]; then
        success "=== All validations passed ==="
        exit 0
    else
        error "=== $failed validation(s) failed ==="
        exit 1
    fi
}

main "$@"
