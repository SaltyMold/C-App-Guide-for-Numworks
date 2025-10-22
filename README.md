<h1 align="center">Guide to Create a C App for the Numworks Calculator</h1>
<p align="center">
    <img alt="Version" src="https://img.shields.io/badge/Version-0.0.1-blue?style=for-the-badge&color=blue">
    <img alt="Stars" src="https://img.shields.io/github/stars/SaltyMold/C-App-Guide-for-Numworks?style=for-the-badge&color=magenta">
    <img alt="Forks" src="https://img.shields.io/github/forks/SaltyMold/C-App-Guide-for-Numworks?color=cyan&style=for-the-badge&color=purple">
    <img alt="License" src="https://img.shields.io/github/license/SaltyMold/C-App-Guide-for-Numworks?style=for-the-badge&color=blue">
    <br>
    <a href="https://github.com/SaltyMold"><img title="Developer" src="https://img.shields.io/badge/Developer-SaltyMold-red?style=flat-square"></a>
    <img alt="Maintained" src="https://img.shields.io/badge/Maintained-Yes-blue?style=flat-square">
    <img alt="Written In" src="https://img.shields.io/badge/Written%20In-C-yellow?style=flat-square">
</p>

_This repo is a Guide to help you to Create a C App for the Numworks Calculator. You can also follow my YouTube video for additional guidance._

## ⚙️ Download requirements

### Windows

For **Windows**, download the latest **`MSYS2-Numworks.zip`** file from the **[Releases](https://github.com/SaltyMold/C-App-Guide-for-Numworks/releases) page** and extract the ZIP.

#### Setup Instructions

1. Launch `msys2.exe`
2. Close MSYS2
3. Move all files from `/home/Default` to `/home/[YourUsername]`
4. Restart `msys2.exe`
5. Run the following command:
```bash
~/set-variable
```

#### Build the Project

```bash
cd numworks
make clean && make build
```

#### Notes

- If you change the MSYS2 installation path, you must re-run `set-variable`.  
- Do **not** use spaces in the MSYS2 path.

Alternatively, for **Windows**, you can install everything manually using the instructions below.

### Linux

For **Linux**, you must install everything manually using the instructions below.

### MacOS

For **MacOS**, you must install everything manually using the instructions in the [epsilon c-sample-app GitHub repo](https://github.com/numworks/epsilon-sample-app-c).



## 🛠️ Build your own app

Inside the project, you'll find **`eadk.h`**, which provides **essential functions** for interacting with the **[calculator](https://en.wikipedia.org/wiki/NumWorks)**. Modify **`main.c`** to implement your **own code**.
Additionally, make sure to include an **`icon.png`** with dimensions **55×56 pixels** to serve as your **app’s icon**. Once your modifications are done, link the app with **[nwlink](https://www.npmjs.com/package/nwlink)** and enjoy your app!

## ⚙️ Download requirements manually

### Requirements:

- **arm-none-eabi-gcc**
- **nodejs 18.20.7 (install n for 18.20.7 version)**
- **nwlink (with npm)**
- **make 4.3**
- **git (optional)**

To build this app, you will need to install the **[embedded ARM toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)** and **[Node.js](https://nodejs.org/en/) 18**. The C SDK for Epsilon apps is shipped as an **npm module called [nwlink](https://www.npmjs.com/package/nwlink) v0.0.16**.

### Debian

```sh
sudo apt install -y build-essential git gcc-arm-none-eabi binutils-arm-none-eabi nodejs npm && npm install -g n && sudo n 18 && npm install -g nwlink@0.0.16
git clone https://github.com/SaltyMold/C-App-Guide-for-Numworks.git
cd C-App-Guide-for-Numworks
make clean && make build
```

### Windows (manually)

Install msys2 from [the MSYS2 Github](https://github.com/msys2/msys2-installer/releases/download/2025-02-21/msys2-x86_64-20250221.exe) and open the msys2.exe file.
Download the .zip from [the Node Github](https://github.com/actions/node-versions/releases/download/18.20.7-13438827950/node-18.20.7-win32-x64.7z), and extract it.

```sh
#MSYS2

pacman -Syu

#Replace with the reel node path
echo 'export PATH="/c/Users/UserName/AppData/Local/Programs/node-18.20.7-win32-x64:$PATH"' >> ~/.bashrc
source ~/.bashrc

npm install -g nwlink@0.0.16
nwlink --version
```

```ps
#PowerShell

#You can chose a diferent path
$env:ChocolateyInstall = "$env:LOCALAPPDATA\Programs\choco"
[System.Environment]::SetEnvironmentVariable("ChocolateyInstall", $env:ChocolateyInstall, "User")

Set-ExecutionPolicy Bypass -Scope Process -Force
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

$env:Path += ";$env:ChocolateyInstall\bin"
[System.Environment]::SetEnvironmentVariable("Path", $env:Path, "User")
choco --version

choco install make --version=4.3 -y --force
make --version
```

```sh
#MSYS2

#Replace with the reel make path
echo 'export PATH="/c/Users/UserName/AppData/Local/Programs/choco/make/bin:$PATH"' >> ~/.bashrc 
source ~/.bashrc

pacman -S --noconfirm mingw-w64-x86_64-arm-none-eabi-gcc
arm-none-eabi-gcc --version

pacman -S --noconfirm git
git --version
git clone https://github.com/SaltyMold/C-App-Guide-for-Numworks.git

cd C-App-Guide-for-Numworks
make clean && make build
```

You should now have a **`output/app.nwa` file** that you can distribute! Anyone can now install it on their calculator from the **[NumWorks online uploader](https://my.numworks.com/apps)**.

## 🌟 Special thanks 
 [epsilon-sample-app-c](https://github.com/numworks/epsilon-sample-app-c) 
