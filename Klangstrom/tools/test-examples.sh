#!/usr/bin/env bash
set -euo pipefail

EXAMPLES_DIR="../examples"
PLATFORMS=(
  "STMicroelectronics:stm32:KLST_PANDA"
  "umfeld-arduino:umfeld:KLST_EMU"
)

RESULTS_FILE="compile-results.txt"
: > "$RESULTS_FILE"   # empty the file at start

# find all example directories that contain a .ino file
EXAMPLES=$(find "$EXAMPLES_DIR" -mindepth 2 -maxdepth 2 -type d)

for EXAMPLE in $EXAMPLES; do
  SKETCH=$(find "$EXAMPLE" -maxdepth 1 -name "*.ino" | head -n1)
  if [[ -z "$SKETCH" ]]; then
    echo "Skipping $EXAMPLE (no .ino file found)" | tee -a "$RESULTS_FILE"
    continue
  fi

  echo "=== Compiling $(basename "$EXAMPLE") ===" | tee -a "$RESULTS_FILE"
  for PLATFORM in "${PLATFORMS[@]}"; do
    echo "-> $PLATFORM" | tee -a "$RESULTS_FILE"
    if arduino-cli compile -b "$PLATFORM" "$EXAMPLE"; then
      echo "✅ Success: $EXAMPLE on $PLATFORM" | tee -a "$RESULTS_FILE"
    else
      echo "❌ Failed: $EXAMPLE on $PLATFORM" | tee -a "$RESULTS_FILE"
    fi
  done
  echo | tee -a "$RESULTS_FILE"
done

echo "All results written to $RESULTS_FILE"
