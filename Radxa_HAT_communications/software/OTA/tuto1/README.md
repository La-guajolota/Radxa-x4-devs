# 🛰️ Secure OTA Tutorial 1: Signed Firmware Update

This directory contains the files for a tutorial on performing a **secure Over-the-Air (OTA) update** using public-key cryptography.

---

## 📂 Files

| File Name           | Description                                                                                             |
| ------------------- | ------------------------------------------------------------------------------------------------------- |
| `blink_simple.uf2`  | 펌웨어 **Firmware file** for a simple "blink" application in `.uf2` format.                               |
| `public.pem`        | 📜 **Public key**. This should be stored on the device to verify the firmware's authenticity.             |
| `private.pem`       | 🔑 **Private key**. **MUST BE KEPT SECRET!** Used to sign the firmware before deployment.                  |
| `blink.sig`         | ✍️ **Digital signature** of the `blink_simple.uf2` file, generated with the private key.                   |

---

## ⚙️ Secure OTA Process

The workflow for a secure OTA update is as follows:

### 1. On the Developer's Machine (or Build Server)
-   The firmware (`blink_simple.uf2`) is built.
-   A digital signature (`blink.sig`) is created by signing the firmware with the `private.pem` key.
-   The firmware and its signature are sent to the device.

### 2. On the Device
-   The device receives the new firmware and its signature.
-   It uses its stored `public.pem` key to verify that the signature is valid for the received firmware.
-   **If valid**: The device proceeds with the update. ✅
-   **If invalid**: The update is aborted, preventing unauthorized or corrupted firmware from being installed. ❌

---

## 🛠️ How to Use These Files

These files are part of a larger tutorial. To implement this, you would typically need:
-   A device capable of receiving OTA updates and performing signature verification.
-   A host application to send the `.uf2` and `.sig` files to the device.
-   A bootloader on the device that can perform the signature check and write the new firmware to flash.

### Example with OpenSSL

You can use `OpenSSL` to generate keys and work with signatures.

**To generate a signature:**
```bash
openssl dgst -sha256 -sign private.pem -out blink.sig blink_simple.uf2
```

**To verify a signature:**
```bash
openssl dgst -sha256 -verify public.pem -signature blink.sig blink_simple.uf2
```
> If successful, this command will output: `Verified OK`

---