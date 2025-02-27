#!/bin/sh

EXECUTABLE="/opt/lcd/lcd"

OUTPUT=$($EXECUTABLE)
EXIT_CODE=$?

if [ $EXIT_CODE -eq 0 ]; then
	if [[ "$OUTPUT" =~ Switching\ working\ directory\ to\ \"(.*)\"\.\.\. ]]; then
		TARGET_DIR="${BASH_REMATCH[1]}"
		if [ -n "$TARGET_DIR" ] && [ -d "$TARGET_DIR" ]; then
			echo "$TARGET_DIR"
		fi
	fi
fi