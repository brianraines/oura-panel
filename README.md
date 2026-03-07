# oura-panel
A dedicated ESP32 e-paper dashboard for visualizing Oura Ring health data.

## Hardware
- [DFRobot FireBeetle 2 ESP32-E](https://www.dfrobot.com/product-2195.html) (or FireBeetle ESP32 with micro-USB)
- [Waveshare 7.3inch 7-Color E-Paper Display](https://a.co/d/09qF06k0) (with e-Paper HAT / ACeP panel)
- [Waveshare Universal e-Paper Raw Panel Driver Shield](https://a.co/d/7kTwsXB)
- [3.7V LiPo battery](https://a.co/d/02BPgz2V) (compatible with FireBeetle's JST-PH2.0 connector)
- [3D printable enclosure / frame](https://www.thingiverse.com/thing:4765790)

## Wiring
Waveshare -> FireBeetle:
BUSY -> 14/D6
RST  -> 25/D2
DC   -> 22/SCL
CS   -> 13/D7
SCLK -> 18/SCK
DIN  -> 23/MOSI
GND  -> GND
VCC  -> 3V3

3.7V LiPo battery -> FireBeetle:
- JST port (but make sure to match the polarity, red is positive, black is negative, so you may need to reverse the polarity by rewiring the plug so that red is positive and black is negative)

## Oura API Setup

OuraPanel uses the [Oura API v2](https://cloud.ouraring.com/v2/docs) with OAuth2 authentication to access your ring data.

### 1. Register an Application

1. Sign in to your Oura account at [cloud.ouraring.com](https://cloud.ouraring.com/).
2. Navigate to **[My Applications](https://cloud.ouraring.com/oauth/applications)**.
3. Click **New Application** and fill in the required fields:
   - **App Name** — e.g. `OuraPanel`
   - **Description** — a brief summary of what the app does
   - **Redirect URI(s)** — set this to `http://localhost:8585/callback`
   - **Scopes** — select the data types you need access to. OuraPanel uses:
     - `daily` — sleep, activity, and readiness summaries
     - `heartrate` — heart rate time-series data
     - `personal` — age, weight, height
     - `email` — email address of the user
     - `spo2Daily` — blood oxygen averages
     - `session` — guided/unguided sessions
     - `workout` — workout summaries
     - `tag` — user entered tags
4. Click **Save** to create the application.

### 2. Get Your Client ID and Client Secret

After saving, your application page will display two values:

| Field             | Description                                      |
|-------------------|--------------------------------------------------|
| **Client ID**     | A public identifier for your application.        |
| **Client Secret** | A private key used to exchange tokens. **Keep this secret — never commit it to version control.** |

Copy both values. You will need them to configure OuraPanel.

### 3. Configure OuraPanel

Create `src/secrets.cpp` to store your credentials and keep `include/secrets.h` as the declarations header. Both secret-bearing files are gitignored and should never be committed.

```cpp
// src/secrets.cpp
#include "secrets.h"

const char* WIFI_SSID          = "your-wifi-ssid";
const char* WIFI_PASSWORD      = "your-wifi-password";
const char* OURA_CLIENT_ID     = "your-client-id-here";
const char* OURA_CLIENT_SECRET = "your-client-secret-here";
const char* OURA_REFRESH_TOKEN = "your-refresh-token-here";
```

### 4. Authorize & Get Your Refresh Token

Create a local authorization helper from the tracked example, then run it on your computer (one-time):

```sh
cp scripts/authorize.example.py scripts/authorize.py
# edit CLIENT_ID and CLIENT_SECRET in scripts/authorize.py
python3 scripts/authorize.py
```

The local `scripts/authorize.py` file is intentionally gitignored so you can put your Oura OAuth client credentials in it without committing them.

This opens your browser to Oura's consent page. After you grant access, the script prints a refresh token. Paste it into `src/secrets.cpp` as the `OURA_REFRESH_TOKEN` value.

The ESP32 uses this refresh token to obtain access tokens automatically on every boot. Tokens are persisted in NVS, so subsequent boots reuse them without hitting the API. If the refresh token is ever revoked, re-run the script.

The example/local authorization script uses only the Python standard library, so no extra Python packages are required.

> **Note:** Applications are limited to **10 users** before requiring [approval from Oura](mailto:api-support@ouraring.com). There is no limit after approval.

### 5. Build & Upload

This is a [PlatformIO](https://platformio.org/) project targeting the DFRobot FireBeetle 2 ESP32-E.

```sh
# Build
pio run -e firebeetle2

# Upload to the board
pio run -e firebeetle2 -t upload

# Open serial monitor (115200 baud)
pio device monitor -b 115200
```

All dependencies (ArduinoJson, GxEPD2, Adafruit GFX) are managed automatically by PlatformIO via `platformio.ini`.

## How It Works

On each boot the device:

1. Connects to WiFi and syncs time via NTP
2. Checks whether the current time is inside the refresh window (`8:00am` to `8:00pm`)
3. Refreshes the OAuth2 access token (or loads one from NVS)
4. Fetches Oura data for sleep, readiness, activity, resilience, vitals, and sleep trends
5. Renders the dashboard on the e-paper display
6. Enters deep sleep until the next 10-minute boundary during the active window (`:00`, `:10`, `:20`, etc.)

The e-paper display retains its image while the ESP32 sleeps, so the dashboard is always visible with near-zero power draw between refreshes.

Outside the active window, the device skips refreshes and sleeps until the next `8:00am`.

## Configuration

Edit these values in `src/main.cpp` to match your timezone and preferences:

| Constant | Default | Description |
|----------|---------|-------------|
| `UTC_OFFSET_SEC` | `-6 * 3600` | UTC offset in seconds |
| `DST_OFFSET_SEC` | `3600` | DST adjustment (0 to disable) |
| `SLEEP_INTERVAL_MINUTES` | `10` | Refresh cadence during active hours |
| `REFRESH_START_HOUR` | `8` | First allowed refresh hour (local time) |
| `REFRESH_END_HOUR` | `20` | End of allowed refresh window (local time, exclusive) |

## Dashboard Contents

The current dashboard includes:

1. Sleep, Readiness, Activity, and Resilience headline panels
2. Last Night stats and sleep stages
3. Vitals summary
4. Multi-day sleep trend graph and averages
5. Last sync time and battery status in the header

For full authentication details, see the [Oura Authentication Docs](https://cloud.ouraring.com/docs/authentication).