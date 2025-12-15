#pragma once
// SPDX-FileCopyrightText: 2025 Nessan Fitzmaurice <nzznfitz+gh@icloud.com>
// SPDX-License-Identifier: MIT

/// @file
/// An "include everything" header for the utilities library.

#include <utilities/confirm.h>
#include <utilities/log.h>
#include <utilities/macros.h>
#include <utilities/stopwatch.h>
#include <utilities/stream.h>
#include <utilities/string.h>
#include <utilities/thousands.h>
#include <utilities/type.h>

// However, we do not include formatter.h here -- it can give rise to  ]ambiguous `std::formatter` specialisations.
// You can include it directly if you need it and are aware of the issues.
// #include <utilities/formatter.h>
