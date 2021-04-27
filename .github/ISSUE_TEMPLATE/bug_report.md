---
name: "OpenCPN - Bug Report"
description: Report a bug on OpenCPN
body:
- type: markdown
  attributes:
    value: |
      Please [search for existing issues](https://github.com/OpenCPN/OpenCPN/issues) before creating a new one.

- type: input
  attributes:
    label: OpenCPN version
    description: |
      Please have a look at the title of OpenCPN's main window
    placeholder: "5.2.4"
  validations:
    required: true

- type: checkboxes
  attributes:
    label: Platform
    description: |
      Tell us on which platform(s) you observe the bug
    options:
      - label: "Windows"
      - label: "Linux"
      - label: "macOS"
      - label: "Android"

- type: input
  attributes:
    label: Distro Version
    description: |
      Please tell us what OS distribution and version you are using (if applicable).
      You can get additional information about the version where possible, e.g. on Debian / Ubuntu, run `lsb_release -r`.
      In case of Linux we are also particularly interested in your desktop environment.
    placeholder: "Ubuntu 20.04, Gnome Shell"
  validations:
    required: false

---

**Describe the bug**
A clear and concise description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '....'
3. Scroll down to '....'
4. See error

**Expected behavior**
A clear and concise description of what you expected to happen.

**Screenshots**
If applicable, add screenshots to help explain your problem.

**Desktop (please complete the following information):**
 - OS: [e.g. iOS]
 - Browser [e.g. chrome, safari]
 - Version [e.g. 22]

**Smartphone (please complete the following information):**
 - Device: [e.g. iPhone6]
 - OS: [e.g. iOS8.1]
 - Browser [e.g. stock browser, safari]
 - Version [e.g. 22]

**Additional context**
Add any other context about the problem here.
