# .gitignore Contract

## Format Specification

**Purpose**: Define untracked files for ShowMIDI repository  
**Pattern Syntax**: Git glob patterns (supports `*`, `**`, `?`, `[...]`, `!`)

---

## Template

```gitignore
# .gitignore for ShowMIDI
# Prevents OS-specific, build, and IDE artifacts from being tracked

### macOS ###
.DS_Store
.AppleDouble
.LSOverride
._*
.DocumentRevisions-V100
.fseventsd
.Spotlight-V100
.TemporaryItems
.Trashes
.VolumeIcon.icns
.com.apple.timemachine.donotpresent
.AppleDB
.AppleDesktop
Network Trash Folder
Temporary Items
.apdisk

### Windows ###
Thumbs.db
ehthumbs.db
Desktop.ini
$RECYCLE.BIN/
*.lnk

### Linux ###
*~
.fuse_hidden*
.directory
.Trash-*
.nfs*

### IDEs ###
# VS Code
.vscode/
!.vscode/extensions.json
!.vscode/settings.json

# CLion / IntelliJ
.idea/
*.iml

# Xcode
*.xcodeproj/xcuserdata/
*.xcworkspace/xcuserdata/
DerivedData/

# Visual Studio
.vs/
*.suo
*.user
*.userosscache
*.sln.docstates
*.userprefs

### Build Outputs ###
# CMake
build/
cmake-build-*/
CMakeFiles/
CMakeCache.txt
*.cmake

# Compiled binaries
*.app
*.vst3
*.component
*.exe
*.dll
*.dylib
*.so
*.a
*.lib
*.o
*.obj

# JUCE build artifacts
JuceLibraryCode/BinaryData.*
Builds/*/build/

# Installers (generated, not source)
Installers/*.dmg
Installers/*.pkg
Installers/*.exe
Installers/*.msi

### Development ###
# Logs
*.log

# Temporary files
*.tmp
*.temp
*.swp
*.swo
*.bak
*.cache

# Profiling
*.gcda
*.gcno
*.gcov

# Core dumps
core
core.*
