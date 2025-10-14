# Secure OTA Tutorial 1: Signed Firmware Update

This directory contains the files for a tutorial on performing a secure Over-the-Air (OTA) update. The process involves signing the firmware with a private key and verifying the signature on the device with a public key before applying the update.

## Files

-   **`blink_simple.uf2`**: The firmware file for a simple "blink" application. The `.uf2` format is a block-based file format designed for flashing microcontrollers over a USB Mass Storage Class (MSC) interface, commonly used with boards like the Raspberry Pi Pico.
-   **`public.pem`**: The public key file. This key should be stored on the device to verify the authenticity of the firmware.
-   **`private.pem`**: The private key file. **This key must be kept secret!** It is used to sign the firmware before sending it to the device.
-   **`blink.sig`**: The digital signature of the `blink_simple.uf2` file. This signature is generated using the private key.

## Secure OTA Process

The typical workflow for this kind of secure OTA update is as follows:

1.  **On the developer's machine (or build server)**:
    -   The firmware (`blink_simple.uf2`) is built.
    -   A digital signature (`blink.sig`) is created by signing the firmware file with the `private.pem` key.
    -   The firmware and its signature are sent to the device.

2.  **On the device**:
    -   The device receives the new firmware and its signature.
    -   The device uses the `public.pem` key (which it has stored securely) to verify that the signature is valid for the received firmware.
    -   If the signature is valid, the device proceeds with the update.
    -   If the signature is invalid, the update is aborted. This prevents unauthorized or corrupted firmware from being installed.

## How to Use These Files

These files are likely part of a larger tutorial. To use them, you would typically need:

-   A device capable of receiving OTA updates and performing signature verification.
-   A host application that sends the `.uf2` and `.sig` files to the device.
-   A bootloader on the device that can perform the signature check and write the new firmware to flash.

This example provides the essential components for the signing part of the process. You would use a tool like OpenSSL to generate the keys and sign the firmware.

### Example with OpenSSL

**To generate a signature:**
```bash
openssl dgst -sha256 -sign private.pem -out blink.sig blink_simple.uf2
```

**To verify a signature:**
```bash
openssl dgst -sha256 -verify public.pem -signature blink.sig blink_simple.uf2
```
This should output "Verified OK".
