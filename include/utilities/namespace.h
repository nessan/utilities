#pragma once
// SPDX-FileCopyrightText: 2025 Nessan Fitzmaurice <nzznfitz+gh@icloud.com>
// SPDX-License-Identifier: MIT

/// @file
/// An "include everything" header for the `utilities` library that also exposes the namespace `utilities`.

/// @namespace utilities
/// The namespace for the utilities library.
namespace utilities {}

// Include everything ...
#include "utilities.h"

// Exose the namespace
using namespace utilities;