<div align=center>

[![Downloads](https://img.shields.io/github/downloads/__REPO__/__VERSION__/total?style=flat-square&logo=github)](https://github.com/__REPO__/releases/tag/__VERSION__)
[![LeviLamina](https://img.shields.io/badge/for-LeviLamina-7FA973?style=flat-square&logo=cplusplus&logoColor=white&labelColor=2C5E3B)](https://github.com/LiteLDev/LeviLamina)

</div>

### ⬇️ Downloads

| File | Description |
|------|-------------|
| [📦 `serverinfo-rest-v__VERSION__-server-windows-x64.zip`](__BASE_URL__/serverinfo-rest-v__VERSION__-server-windows-x64.zip) | Plugin package for LeviLamina |

### 📥 Install via lip

```bash
lip install github.com/__REPO__@__VERSION__
# If latest-version resolution works in your environment, you may also try:
lip install github.com/__REPO__
# To update an already installed package:
lip update github.com/__REPO__@__VERSION__
```

### 📦 Manual Install

1. Download `serverinfo-rest-v__VERSION__-server-windows-x64.zip`
2. Unzip, you'll get:
   ```
   serverinfo-rest/
   ├── manifest.json
   ├── serverinfo-rest.dll
   └── serverinfo-rest.pdb
   ```
3. Copy `serverinfo-rest/` into your server's `plugins/`:
   ```
   BDS Server Root/
   ├── bedrock_server_mod.exe
   ├── PreLoader.dll
   ├── plugins/
   │   ├── LeviLamina/               ← BDS mod loader (pre-installed)
   │   │   ├── LeviLamina.dll
   │   │   └── ...
   │   └── serverinfo-rest/          ← place here
   │       ├── manifest.json
   │       ├── serverinfo-rest.dll
   │       └── serverinfo-rest.pdb
   └── ...
   ```
4. Restart the server — `config/config.json` will be auto-generated on first run

---

### 📋 What's Changed

__COMMIT_LOG__

---

### 📊 Build Info

- **Build date**: __BUILD_DATE__
- **Commit**: __COMMIT_HASH__
- **Full Changelog**: __CHANGELOG_URL__
