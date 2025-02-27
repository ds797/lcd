#!/bin/sh

# Exit on error
set -e

DIR="/opt/lcd"
CLONE_DIR="/opt/lcd/clone"
URL="https://github.com/ds797/lcd"
EXECUTABLE="lcd"
SCRIPT="lcd.sh"

if [ -e "$DIR" ]; then
	echo "[lcd-install] Error: directory already exists"
	exit 1
fi

clean() {
	echo "[lcd-install] Cleaning up..."
	rm -rf "$DIR"
}
trap clean ERR

mkdir -p "$DIR"
git clone "$URL" "$CLONE_DIR"

make -C "$CLONE_DIR"

mv "$CLONE_DIR/$EXECUTABLE" "$DIR/"
mv "$CLONE_DIR/$SCRIPT" "$DIR/"

rm -rf "$CLONE_DIR"

echo "Installation complete! The last step is to add the alias to your configuration file."
