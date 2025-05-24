# Qt-CSU ChatRoom

基于 Qt 的局域网聊天室项目  
A LAN (Local Area Network) chat room application based on Qt

## 项目简介 / Project Introduction

本项目是一个基于 Qt 5.14.2 开发的局域网聊天软件，支持用户注册、登录、群聊、私聊、文件传输、实时语音等丰富功能，适用于校园或小型网络环境。项目使用 Qt 自带的 SQLite 数据库进行本地数据管理。

This project is a LAN chat room software developed with Qt 5.14.2. It supports user registration, login, group chat, private chat, file transfer, real-time voice chat, and more. Qt's built-in SQLite is used for local data management.

## 功能特性 / Features

- **用户注册、登录与找回密码**  
  User registration, login, and password recovery
- **群聊与私聊**  
  Group chat (multi-user) and private chat
- **统计当前在线人数**  
  Real-time display of online user list and online count
- **文件传输**  
  Send and receive files with progress bar
- **截图保存**  
  Screenshot chat window and save (path can be customized)
- **实时语音**  
  Real-time voice chat (one-to-one and group)
- **侧边栏自定义**  
  Sidebar contents are customizable
- **聊天记录保存与清空**  
  Save chat history as text file, and one-click clear
- **好友管理**  
  Add, delete, and rename friends
- **断线处理与错误提示**  
  Auto-handle network disconnection, send failure, with prompts
- **用户上下线广播**  
  Broadcast user online/offline status, auto-refresh online list
- **UI 快捷操作**  
  Ctrl+Enter to quickly send messages

## 项目截图 / Screenshots

![截图1](https://github.com/user-attachments/assets/2dccdc2c-c395-4384-bd05-f35e67c1cd76)
![截图2](https://github.com/user-attachments/assets/3892231c-099f-44f6-9993-971156102700)

## 安装与运行 / Installation & Run

1. **环境依赖 / Requirements**
   - Qt 5.14.2 (建议使用 Qt Creator)
   - 支持 C++11 或更高版本的编译器

2. **编译步骤 / Build Steps**
   - 克隆本仓库 / Clone this repository:
     ```bash
     git clone https://github.com/jasminelee162/Qt-CSU--chatRoom.git
     ```
   - 用 Qt Creator 打开 `Qt-CSU--chatRoom.pro` 或 `CMakeLists.txt` 项目文件  
     Open the `.pro` or `CMakeLists.txt` file with Qt Creator
   - 配置并编译项目 / Configure and build the project

3. **运行 / Run**
   - 编译后直接运行主程序 / Run the generated executable

## 使用说明 / Usage

- 注册新用户后登录即可进入主界面
- 选择在线用户进行群聊或私聊
- 点击文件按钮发送文件，或使用截图功能保存聊天内容
- 支持实时语音通话，需连接麦克风
- 聊天记录可导出为文本文件，支持一键清空

## 目录结构 / Directory Structure

- `main.cpp`, `mainwindow.h/cpp`: 主程序入口及主界面逻辑
- `login.h/cpp`: 登录窗口逻辑
- `database/`: 数据库相关文件
- `resources/`: 资源文件（图片、图标等）
- `README.md`: 项目说明文档

## License

本项目遵循 MIT License。  
This project is licensed under the MIT License.

---

> 如需进一步文档或有问题，欢迎在 Issues 区留言。
