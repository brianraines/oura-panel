#!/usr/bin/env python3
"""
One-time OAuth2 authorization helper for OuraPanel.

Copy this file to scripts/authorize.py, fill in your Oura OAuth client
credentials, then run it to obtain a refresh token for src/secrets.cpp.

Usage:
    cp scripts/authorize.example.py scripts/authorize.py
    python3 scripts/authorize.py

No third-party dependencies required; uses only the Python standard library.
"""

import http.server
import json
import sys
import urllib.parse
import urllib.request
import webbrowser

# ---------------------------------------------------------------------------
# Configuration - must match your Oura application settings
# ---------------------------------------------------------------------------
CLIENT_ID = "your-client-id-here"
CLIENT_SECRET = "your-client-secret-here"
REDIRECT_URI = "http://localhost:8585/callback"
SCOPES = "email personal daily heartrate heart_health stress workout tag session spo2Daily"

AUTH_URL = "https://cloud.ouraring.com/oauth/authorize"
TOKEN_URL = "https://api.ouraring.com/oauth/token"

# ---------------------------------------------------------------------------

auth_code = None


class CallbackHandler(http.server.BaseHTTPRequestHandler):
    """Handle the OAuth2 redirect from Oura."""

    def do_GET(self):
        global auth_code
        parsed = urllib.parse.urlparse(self.path)

        if parsed.path != "/callback":
            self.send_response(404)
            self.end_headers()
            return

        params = urllib.parse.parse_qs(parsed.query)

        if "error" in params:
            self.send_response(403)
            self.send_header("Content-Type", "text/html")
            self.end_headers()
            self.wfile.write(b"<h1>Authorization denied.</h1><p>You can close this tab.</p>")
            print(f"\nAuthorization denied: {params['error'][0]}")
            sys.exit(1)

        if "code" not in params:
            self.send_response(400)
            self.end_headers()
            return

        auth_code = params["code"][0]
        self.send_response(200)
        self.send_header("Content-Type", "text/html")
        self.end_headers()
        self.wfile.write(b"<h1>Authorized!</h1><p>You can close this tab. Check your terminal.</p>")

    def log_message(self, format, *args):
        pass


def post_form(url, data):
    """POST url-encoded form data using only stdlib."""
    encoded = urllib.parse.urlencode(data).encode("utf-8")
    req = urllib.request.Request(url, data=encoded, method="POST")
    req.add_header("Content-Type", "application/x-www-form-urlencoded")
    try:
        with urllib.request.urlopen(req) as resp:
            body = json.loads(resp.read().decode("utf-8"))
            return resp.status, body
    except urllib.error.HTTPError as e:
        body = e.read().decode("utf-8")
        try:
            body = json.loads(body)
        except json.JSONDecodeError:
            pass
        return e.code, body


def api_get(url, token):
    """GET with Bearer auth using only stdlib."""
    req = urllib.request.Request(url)
    req.add_header("Authorization", f"Bearer {token}")
    try:
        with urllib.request.urlopen(req) as resp:
            return resp.status, json.loads(resp.read().decode("utf-8"))
    except urllib.error.HTTPError as e:
        return e.code, {}


def main():
    if CLIENT_ID == "your-client-id-here" or CLIENT_SECRET == "your-client-secret-here":
        print("Set CLIENT_ID and CLIENT_SECRET in scripts/authorize.py before running.")
        sys.exit(1)

    params = urllib.parse.urlencode(
        {
            "client_id": CLIENT_ID,
            "redirect_uri": REDIRECT_URI,
            "response_type": "code",
            "scope": SCOPES,
        }
    )
    authorize_url = f"{AUTH_URL}?{params}"

    print("=" * 50)
    print("  OuraPanel - OAuth2 Authorization")
    print("=" * 50)
    print()
    print("Opening your browser to authorize with Oura...")
    print("(If it doesn't open, visit this URL manually:)")
    print()
    print(f"  {authorize_url}")
    print()

    server = http.server.HTTPServer(("localhost", 8585), CallbackHandler)
    webbrowser.open(authorize_url)

    while auth_code is None:
        server.handle_request()
    server.server_close()

    print("Exchanging authorization code for tokens...")
    status, tokens = post_form(
        TOKEN_URL,
        {
            "grant_type": "authorization_code",
            "code": auth_code,
            "client_id": CLIENT_ID,
            "client_secret": CLIENT_SECRET,
            "redirect_uri": REDIRECT_URI,
        },
    )

    if status != 200:
        print(f"\nToken exchange failed (HTTP {status}):")
        print(json.dumps(tokens, indent=2) if isinstance(tokens, dict) else tokens)
        sys.exit(1)

    access_token = tokens["access_token"]
    refresh_token = tokens["refresh_token"]
    expires_in = tokens.get("expires_in", "?")

    print("Verifying token by fetching personal info...")
    info_status, info = api_get(
        "https://api.ouraring.com/v2/usercollection/personal_info",
        access_token,
    )

    if info_status == 200:
        print()
        print("Personal info:")
        print(json.dumps(info, indent=2))
    else:
        print(f"  (Could not fetch personal info: HTTP {info_status})")

    print()
    print("=" * 50)
    print("  Add this to src/secrets.cpp:")
    print("=" * 50)
    print()
    print(f'const char* OURA_REFRESH_TOKEN = "{refresh_token}";')
    print()
    print(f"  Access token expires in {expires_in} seconds.")
    print("  The refresh token does not expire unless revoked.")
    print()


if __name__ == "__main__":
    main()
