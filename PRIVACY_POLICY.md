# Privacy Policy

**Last Updated:** February 7, 2026

## Introduction

This Privacy Policy describes how OuraPanel ("we," "us," or "our") collects, uses, and protects information when you use our e-paper dashboard application for visualizing Oura Ring health data (the "Service").

By using the Service, you agree to the collection and use of information in accordance with this policy.

## Information We Collect

### Health & Wellness Data

OuraPanel accesses your health and wellness data through the [Oura API](https://cloud.ouraring.com/v2/docs). This may include, but is not limited to:

- Sleep data (duration, stages, timing, quality scores)
- Activity data (steps, calories, movement)
- Readiness scores and contributing factors
- Heart rate and heart rate variability (HRV)
- Body temperature trends
- Blood oxygen (SpO2) levels

### Authentication Data

- **Oura API Token:** To access your Oura data, you must provide a personal access token or authenticate via OAuth. This token is stored locally on your device and is never transmitted to any third-party server operated by us.

### Device Data

- OuraPanel runs on local hardware (e.g., ESP32 microcontroller with an e-paper display). No device telemetry or usage analytics are collected or transmitted by the Service.

## How We Use Your Information

Your data is used solely for the following purpose:

- **Display:** Rendering your Oura Ring health metrics on a local e-paper display.

We do **not** use your data for advertising, profiling, analytics, or any purpose other than local visualization.

## Data Storage & Retention

- All data retrieved from the Oura API is processed **locally** on the device.
- Data is held in volatile memory only for the duration needed to render the display and is not persisted to long-term storage unless explicitly configured by you.
- Your Oura API credentials are stored locally on the device and are never transmitted to external servers controlled by us.

## Data Sharing & Third Parties

We do **not** sell, rent, trade, or otherwise share your personal or health data with any third party.

The Service communicates directly with the following third-party service:

| Service | Purpose | Privacy Policy |
|---------|---------|----------------|
| Oura (Ōura Health Oy) | Retrieve health & wellness data | [Oura Privacy Policy](https://ouraring.com/privacy-policy) |

Your use of the Oura API is subject to Oura's own terms and privacy policy. We encourage you to review their policies.

## Data Security

We implement reasonable measures to protect your data, including:

- All communication with the Oura API is conducted over HTTPS/TLS.
- API credentials are stored locally on the device and are not exposed to the network beyond authenticated API calls.
- The device operates on your local network and does not expose a public-facing server.

However, no method of electronic storage or transmission is 100% secure. We cannot guarantee absolute security, and you use the Service at your own risk.

## Your Rights & Choices

- **Access & Control:** Because all data is processed locally, you retain full control. You can revoke your Oura API token at any time through your [Oura account settings](https://cloud.ouraring.com/personal-access-tokens).
- **Deletion:** Power-cycling the device clears all volatile data. Removing your API token from the device configuration will prevent any further data access.
- **Opt-Out:** You may stop using the Service at any time by disconnecting or powering off the device.

## Children's Privacy

The Service is not directed to individuals under the age of 13 (or the applicable age of consent in your jurisdiction). We do not knowingly collect personal information from children.

## International Users

If you are accessing the Service from outside the United States, please be aware that your data may be processed locally on hardware located in your jurisdiction. The Oura API may process data in accordance with Oura's own data processing practices.

## Changes to This Privacy Policy

We may update this Privacy Policy from time to time. Changes will be reflected by updating the "Last Updated" date at the top of this document. Continued use of the Service after changes constitutes acceptance of the revised policy.

## Contact

If you have questions or concerns about this Privacy Policy, please open an issue on the [OuraPanel GitHub repository](https://github.com/yourusername/OuraPanel/issues).
