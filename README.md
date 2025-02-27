# `lcd`: an interactive `cd`

`lcd` was designed to solve the problem of cumbersome directory changes.  
The program, despite having human-readable output, is rather useless unless used in conjunction with `lcd.sh`. This script filters all output and returns the path on its own. For *this* to be of any value, an **alias** must be created, and added to `.bashrc` (or its equivalent on your system).  

## Installation
Get started by executing `curl "https://raw.githubusercontent.com/ds797/lcd/refs/heads/trunk/install.sh" | sudo sh`  
This installs `lcd` to `/opt/lcd`.

## Setup
Add the following to your shell configuration: `alias lcd='cd "$(/opt/lcd/lcd.sh)"'`  
Source the config, and you're good to go!

## Removal
Simply remove `/opt/lcd`. Don't forget to get rid of your alias!