# Columbia Optimizer - Linux Port

## 项目概述

这是 Columbia 优化器框架的 Linux 移植版本。原项目是一个基于 Windows MFC 的 C++ 应用程序，现已改造为可在 Linux 平台上编译运行的命令行程序。

## 已完成的移植工作

### 1. 创建 Linux 兼容层
- **文件**: `header/linux_compat.h`, `cpp/linux_compat.cpp`
- **功能**: 提供 Windows/MFC 类型和函数的 Linux 替代实现
  - `CString` 类（基于 `std::string`）
  - `CArray` 模板类（基于 `std::vector`）
  - `CFile` 类（基于标准 C FILE*）
  - `_ftime` 函数（基于 `gettimeofday`）
  - 其他 Windows 特定类型的占位符

### 2. 修改构建系统
- **文件**: `Makefile`
- **功能**: 创建了 Linux 平台的 Makefile，支持：
  - 使用 g++ 编译器
  - C++98 标准
  - 定义 `LINUX_PORT` 宏以区分平台
  - 自动依赖管理

### 3. 修改头文件
- **stdafx.h**: 添加平台检测，Linux 下包含兼容层头文件
- **defs.h**: 修改宏定义以支持 Linux，添加 `DummyOutputWindow` 类
- **global.h**: 调整全局变量初始化

### 4. 创建命令行入口
- **文件**: `cpp/main_linux.cpp`
- **功能**: 提供 Linux 命令行界面，支持参数：
  - `-catalog <file>`: 指定 catalog 文件
  - `-query <file>`: 单查询模式
  - `-batch <file>`: 批量查询模式
  - `-cm <file>`: 成本模型文件
  - `-ruleset <file>`: 规则集文件
  - `-trace`: 启用文件跟踪
  - `-help`: 显示帮助信息

### 5. 修改核心文件
- **main.cpp**: 添加平台特定的路径分隔符处理（Windows 用 `\\`，Linux 用 `/`）
- 修改文件打开方式以支持 Linux

## 当前状态

项目已完成基本的移植框架，但由于原代码大量使用 MFC 特性和 Windows API，还需要进一步修复编译错误。

### 主要剩余问题

1. **CString 参数传递**: 许多地方通过可变参数（`...`）传递 CString 对象，需要转换为 `c_str()`
2. **字符串操作**: 部分代码需要适配 `std::string` 的接口
3. **类型转换**: `const char*` 和 `char*` 之间的转换需要调整
4. **文件操作**: 所有 `fopen` 调用需要将 CString 转换为 `c_str()`

## 编译说明

### 前提条件
- Linux 操作系统
- g++ 编译器（支持 C++98）
- make 工具

### 编译步骤

```bash
cd /data/columbia
make clean
make
```

### 运行程序

```bash
# 单查询模式
./columbia -query query -catalog catalog -cm cm -ruleset ruleset

# 批量查询模式
./columbia -batch bquery -catalog catalog -cm cm -ruleset ruleset

# 启用跟踪
./columbia -query query -trace

# 查看帮助
./columbia -help
```

## 文件结构

```
/data/columbia/
├── Makefile                    # Linux 构建文件
├── README_LINUX.md            # 本文件
├── header/                    # 头文件目录
│   ├── linux_compat.h         # Linux 兼容层头文件
│   ├── stdafx.h               # 预编译头（已修改）
│   ├── defs.h                 # 定义文件（已修改）
│   ├── global.h               # 全局变量（已修改）
│   └── ...                    # 其他头文件
├── cpp/                       # 源文件目录
│   ├── linux_compat.cpp       # Linux 兼容层实现
│   ├── main_linux.cpp         # Linux 主程序入口
│   ├── main.cpp               # 优化器主逻辑（已修改）
│   └── ...                    # 其他源文件
├── CATALOGS/                  # 目录数据
├── QUERIES/                   # 查询文件
└── CMS/                       # 成本模型

```

## 下一步工作

要完成完整的移植，需要：

1. 修复所有编译错误（主要是 CString 相关）
2. 测试基本功能
3. 验证优化器逻辑的正确性
4. 性能测试和优化

## 技术说明

### 平台差异处理

代码中使用 `#ifdef LINUX_PORT` 来区分平台特定代码：

```cpp
#ifdef LINUX_PORT
    // Linux 特定代码
#else
    // Windows 特定代码
#endif
```

### CString 兼容性

Linux 版本的 CString 基于 `std::string`，提供了 MFC CString 的主要接口：
- `Format()`: 格式化字符串
- `GetLength()`: 获取长度
- `IsEmpty()`: 检查是否为空
- `Find()`: 查找子串
- 等等

### 文件路径

- Windows: 使用反斜杠 `\\` 作为路径分隔符
- Linux: 使用正斜杠 `/` 作为路径分隔符

代码已自动处理这些差异。

## 许可证

保持原项目的许可证：
```
Columbia Optimizer Framework
A Joint Research Project of Portland State University 
and the Oregon Graduate Institute
Directed by Leonard Shapiro and David Maier
Supported by NSF Grants IRI-9610013 and IRI-9619977
```

## 联系方式

如有问题，请参考原项目文档或联系维护者。
