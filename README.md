# GoAndFive

<div align="center">
  
  [![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
  [![Qt Version](https://img.shields.io/badge/Qt-5.9%2B-green.svg)](https://www.qt.io/)
  [![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg)](https://github.com/username/GoAndFive)
  [![Documentation](https://img.shields.io/badge/docs-available-brightgreen.svg)](./docs/README.md)
  
</div>

A Qt-based board game platform that allows you to play Go (围棋) and Five-in-a-Row (五子棋) with partners over LAN.

## 📚 Documentation

Complete documentation is available in the [docs](./docs/) directory:

- 📖 **[Full Documentation Index](./docs/README.md)** - Start here!
- 🚀 **[Quick Start Guide](./docs/QuickStart.md)** - Get up and running in 5 minutes
- 📘 **[API Documentation](./docs/API_Documentation.md)** - Complete API reference
- 🌐 **[Network Protocol](./docs/NetworkProtocol.md)** - Communication protocol details
- ❓ **[FAQ](./docs/FAQ.md)** - Frequently asked questions
- 🤝 **[Contributing Guide](./docs/Contributing.md)** - How to contribute

## ✨ Features

- 🎮 Two classic board games: Go and Five-in-a-Row
- 🌐 LAN multiplayer support
- 🎨 Clean and intuitive Qt-based GUI
- ⚡ Real-time game synchronization
- 🏆 Automatic win detection
- 🔄 Go capture rules implementation
- 💻 Cross-platform support (Linux, macOS, Windows)

## 🚀 Quick Start

### Prerequisites

- Qt 5.9 or higher
- C++ compiler (GCC 7.0+, Clang 4.0+, or MSVC 2017+)
- CMake 3.10+ or qmake

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/username/GoAndFive.git
   cd GoAndFive
   ```

2. **Build the server**
   ```bash
   cd server/
   qmake chessServer.pro
   make
   ```

3. **Build the client**
   ```bash
   cd ../Go-Five/
   qmake cppFinal.pro
   make
   ```

4. **Run the server**
   ```bash
   ./server/chessServer
   ```

5. **Run clients** (in separate terminals)
   ```bash
   ./Go-Five/cppFinal
   ```

For detailed instructions, see the [Quick Start Guide](./docs/QuickStart.md).

## 🏗️ Project Structure

```
GoAndFive/
├── Go-Five/          # Client application
│   ├── board.*       # Board display and interaction
│   ├── five.*        # Five-in-a-Row game logic
│   ├── go.*          # Go game logic
│   └── welcome.*     # Game selection menu
├── server/           # Server application
│   └── server.*      # Server logic and game management
└── docs/             # Documentation
    ├── README.md     # Documentation index
    ├── API_Documentation.md
    ├── QuickStart.md
    ├── NetworkProtocol.md
    ├── Contributing.md
    └── FAQ.md
```

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guide](./docs/Contributing.md) for details on:
- Code style guidelines
- Development workflow
- Testing requirements
- Pull request process

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Qt Framework and community
- All contributors and testers
- Original project: C++ final assignment

## 📧 Contact

- **GitHub Issues**: [Report bugs or request features](https://github.com/username/GoAndFive/issues)
- **Email**: support@goandfive.org
- **Discord**: [Join our community](https://discord.gg/goandfive)

---

<div align="center">
  Made with ❤️ using Qt Framework
</div>
