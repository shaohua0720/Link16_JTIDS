# Link16 通信系统

## 项目概述

Link16通信系统是一个完整的战术数据链通信系统实现，包括消息处理、编码加密、物理层处理和仿真功能。本系统支持Link16标准的消息格式，并提供了从应用层到物理层的完整实现。

## 功能特点

- **消息处理**：支持Link16标准的J系列消息格式，包括报头字、初始字、扩展字和继续字
- **编码加密**：实现了Reed-Solomon编码、AES加密、奇偶校验和交织等功能
- **物理层处理**：支持USRP硬件接口、多种调制方式、跳频和同步
- **仿真功能**：提供端到端的无线链路仿真，包括各种信道模型和干扰模型

## 项目结构

```
link16/                                # 项目根目录
├── src/                               # 源代码目录
│   ├── core/                          # 核心功能
│   │   ├── types/                     # 基本数据类型
│   │   │   ├── dataType.h             # 数据类型定义
│   │   │   └── Link16Types.h          # Link16特定类型定义
│   │   │
│   │   ├── config/                    # 配置相关
│   │   │   ├── global.h               # 全局变量和常量
│   │   │   ├── global.cpp             # 全局变量实现
│   │   │   ├── SystemConfig.h         # 系统配置
│   │   │   └── SystemConfig.cpp       # 系统配置实现
│   │   │
│   │   └── utils/                     # 通用工具
│   │       ├── tools.h                # 通用工具函数
│   │       ├── tools.cpp              # 通用工具实现
│   │       ├── fileUtils.h            # 文件操作工具
│   │       ├── fileUtils.cpp          # 文件操作实现
│   │       ├── logger.h               # 日志工具
│   │       └── logger.cpp             # 日志实现
│   │
│   ├── protocol/                      # 协议层
│   │   ├── message/                   # 消息处理
│   │   │   ├── word/                  # 字处理
│   │   │   │   ├── Word.hpp           # 基类
│   │   │   │   ├── Word.cpp           # 基类实现
│   │   │   │   ├── HeaderWord.h       # 报头字
│   │   │   │   ├── HeaderWord.cpp     # 报头字实现
│   │   │   │   ├── InitialWord.h      # 初始字
│   │   │   │   ├── InitialWord.cpp    # 初始字实现
│   │   │   │   ├── ExtendWord.h       # 扩展字
│   │   │   │   ├── ExtendWord.cpp     # 扩展字实现
│   │   │   │   ├── ContinueWord.h     # 继续字
│   │   │   │   └── ContinueWord.cpp   # 继续字实现
│   │   │   │
│   │   │   ├── STDPMsg.h              # STDP消息类
│   │   │   └── STDPMsg.cpp            # STDP消息实现
│   │   │
│   │   ├── interface/                 # 协议接口
│   │   │   ├── interface.h            # 消息层接口
│   │   │   └── interface.cpp          # 消息层接口实现
│   │   │
│   │   └── formats/                   # 消息格式定义
│   │       ├── J_Series.h             # J系列消息格式
│   │       └── J_Series.cpp           # J系列消息实现
│   │
│   ├── coding/                        # 编码层
│   │   ├── error_correction/          # 纠错编码
│   │   │   ├── reed_solomon/          # RS编码
│   │   │   │   ├── RSCoder.h          # RS编码器接口
│   │   │   │   └── RSCoder.cpp        # RS编码器实现
│   │   │   │
│   │   │   ├── convolutional/         # 卷积码(预留)
│   │   │   │   ├── ConvolutionalCoder.h
│   │   │   │   └── ConvolutionalCoder.cpp
│   │   │   │
│   │   │   ├── ldpc/                  # LDPC码(预留)
│   │   │   │   ├── LDPCCoder.h
│   │   │   │   └── LDPCCoder.cpp
│   │   │   │
│   │   │   └── turbo/                 # Turbo码(预留)
│   │   │       ├── TurboCoder.h
│   │   │       └── TurboCoder.cpp
│   │   │
│   │   ├── error_detection/           # 错误检测
│   │   │   ├── crc/                   # CRC校验
│   │   │   │   ├── CRCCoder.h         # CRC校验接口
│   │   │   │   └── CRCCoder.cpp       # CRC校验实现
│   │   │   │
│   │   │   └── parity/                # 奇偶校验
│   │   │       ├── BIPCoder.h         # 奇偶校验接口
│   │   │       └── BIPCoder.cpp       # 奇偶校验实现
│   │   │
│   │   ├── crypto/                    # 加密
│   │   │   ├── symmetric/             # 对称加密
│   │   │   │   ├── aes/               # AES加密
│   │   │   │   │   ├── AES.h          # AES加密接口
│   │   │   │   │   └── AES.cpp        # AES加密实现
│   │   │   │   │
│   │   │   │   └── des/               # DES加密(预留)
│   │   │   │       ├── DES.h
│   │   │   │       └── DES.cpp
│   │   │   │
│   │   │   └── hash/                  # 哈希函数
│   │   │       ├── md5/               # MD5哈希
│   │   │       │   ├── md5.h          # MD5哈希接口
│   │   │       │   └── md5.cpp        # MD5哈希实现
│   │   │       │
│   │   │       └── sha/               # SHA哈希(预留)
│   │   │           ├── SHA.h
│   │   │           └── SHA.cpp
│   │   │
│   │   └── interleaving/              # 交织
│   │       ├── matrix/                # 矩阵交织
│   │       │   ├── MatrixInterleaver.h
│   │       │   └── MatrixInterleaver.cpp
│   │       │
│   │       └── block/                 # 块交织(预留)
│   │           ├── BlockInterleaver.h
│   │           └── BlockInterleaver.cpp
│   │
│   ├── physical/                      # 物理层
│   │   ├── hardware/                  # 硬件接口
│   │   │   ├── usrp/                  # USRP硬件
│   │   │   │   ├── USRPInterface.h    # USRP基础接口
│   │   │   │   ├── USRPInterface.cpp
│   │   │   │   ├── USRPTransmitter.h  # USRP发送器
│   │   │   │   ├── USRPTransmitter.cpp
│   │   │   │   ├── USRPReceiver.h     # USRP接收器
│   │   │   │   └── USRPReceiver.cpp
│   │   │   │
│   │   │   └── sdr/                   # 其他SDR硬件(预留)
│   │   │       ├── SDRInterface.h
│   │   │       └── SDRInterface.cpp
│   │   │
│   │   ├── modulation/                # 调制解调
│   │   │   ├── digital/               # 数字调制
│   │   │   │   ├── PSK/               # 相移键控
│   │   │   │   │   ├── BPSKModulator.h
│   │   │   │   │   ├── BPSKModulator.cpp
│   │   │   │   │   ├── QPSKModulator.h
│   │   │   │   │   └── QPSKModulator.cpp
│   │   │   │   │
│   │   │   │   ├── FSK/               # 频移键控
│   │   │   │   │   ├── FSKModulator.h
│   │   │   │   │   └── FSKModulator.cpp
│   │   │   │   │
│   │   │   │   └── QAM/               # 正交幅度调制
│   │   │   │       ├── QAMModulator.h
│   │   │   │       └── QAMModulator.cpp
│   │   │   │
│   │   │   └── analog/                # 模拟调制(预留)
│   │   │       ├── AMModulator.h
│   │   │       └── AMModulator.cpp
│   │   │
│   │   ├── frequency/                 # 频率管理
│   │   │   ├── hopping/               # 跳频
│   │   │   │   ├── FrequencyHopping.h
│   │   │   │   └── FrequencyHopping.cpp
│   │   │   │
│   │   │   └── management/            # 频率管理
│   │   │       ├── FrequencyManager.h
│   │   │       └── FrequencyManager.cpp
│   │   │
│   │   └── synchronization/           # 同步
│   │       ├── time/                  # 时间同步
│   │       │   ├── TimeSynchronizer.h
│   │       │   └── TimeSynchronizer.cpp
│   │       │
│   │       └── frame/                 # 帧同步
│   │           ├── FrameSynchronizer.h
│   │           └── FrameSynchronizer.cpp
│   │
│   ├── simulation/                    # 仿真模块(新增)
│   │   ├── channel/                   # 信道模型
│   │   │   ├── base/                  # 基础信道模型
│   │   │   │   ├── ChannelModel.h     # 信道模型基类
│   │   │   │   └── ChannelModel.cpp
│   │   │   │
│   │   │   ├── awgn/                  # 加性高斯白噪声信道
│   │   │   │   ├── AWGNChannel.h
│   │   │   │   └── AWGNChannel.cpp
│   │   │   │
│   │   │   ├── fading/                # 衰落信道
│   │   │   │   ├── RayleighChannel.h  # 瑞利衰落
│   │   │   │   ├── RayleighChannel.cpp
│   │   │   │   ├── RicianChannel.h    # 莱斯衰落
│   │   │   │   └── RicianChannel.cpp
│   │   │   │
│   │   │   └── multipath/             # 多径信道
│   │   │       ├── MultipathChannel.h
│   │   │       └── MultipathChannel.cpp
│   │   │
│   │   ├── interference/              # 干扰模型
│   │   │   ├── jamming/               # 干扰
│   │   │   │   ├── JammingModel.h
│   │   │   │   └── JammingModel.cpp
│   │   │   │
│   │   │   └── coexistence/           # 共存干扰
│   │   │       ├── CoexistenceModel.h
│   │   │       └── CoexistenceModel.cpp
│   │   │
│   │   ├── scenarios/                 # 仿真场景
│   │   │   ├── urban/                 # 城市环境
│   │   │   │   ├── UrbanScenario.h
│   │   │   │   └── UrbanScenario.cpp
│   │   │   │
│   │   │   ├── rural/                 # 乡村环境
│   │   │   │   ├── RuralScenario.h
│   │   │   │   └── RuralScenario.cpp
│   │   │   │
│   │   │   └── maritime/              # 海洋环境
│   │   │       ├── MaritimeScenario.h
│   │   │       └── MaritimeScenario.cpp
│   │   │
│   │   ├── metrics/                   # 性能指标
│   │   │   ├── BER.h                  # 误码率
│   │   │   ├── BER.cpp
│   │   │   ├── Throughput.h           # 吞吐量
│   │   │   └── Throughput.cpp
│   │   │
│   │   └── engine/                    # 仿真引擎
│   │       ├── SimulationEngine.h     # 仿真引擎核心
│   │       ├── SimulationEngine.cpp
│   │       ├── SimulationConfig.h     # 仿真配置
│   │       └── SimulationConfig.cpp
│   │
│   └── application/                   # 应用层
│       ├── real/                      # 实际应用
│       │   ├── Link16App.h            # 主应用类
│       │   └── Link16App.cpp
│       │
│       ├── simulation/                # 仿真应用
│       │   ├── SimulationApp.h        # 仿真应用类
│       │   └── SimulationApp.cpp
│       │
│       ├── gui/                       # 图形界面(可选)
│       │   ├── MainWindow.h           # 主窗口
│       │   └── MainWindow.cpp
│       │
│       └── main.cpp                   # 主程序入口
│
├── include/                           # 公共头文件
│   └── link16/                        # 对外接口
│       ├── Link16.h                   # 主要API
│       ├── api/                       # API子模块
│       │   ├── MessageAPI.h           # 消息API
│       │   ├── CodingAPI.h            # 编码API
│       │   ├── PhysicalAPI.h          # 物理层API
│       │   └── SimulationAPI.h        # 仿真API(新增)
│       │
│       └── simulation/                # 仿真接口(新增)
│           ├── ChannelSimulation.h    # 信道仿真接口
│           └── EndToEndSimulation.h   # 端到端仿真接口
│
├── lib/                               # 第三方库
│   ├── schifra/                       # Schifra库
│   │   ├── schifra_crc.hpp
│   │   ├── schifra_ecc_traits.hpp
│   │   ├── schifra_error_processes.hpp
│   │   ├── schifra_fileio.hpp
│   │   ├── schifra_galois_field.hpp
│   │   ├── schifra_galois_field_element.hpp
│   │   ├── schifra_galois_field_polynomial.hpp
│   │   ├── schifra_reed_solomon_bitio.hpp
│   │   ├── schifra_reed_solomon_block.hpp
│   │   ├── schifra_reed_solomon_decoder.hpp
│   │   ├── schifra_reed_solomon_encoder.hpp
│   │   └── schifra_sequential_root_generator_polynomial_creator.hpp
│   │
│   ├── uhd/                           # UHD库(USRP硬件驱动)
│   │   └── README.md                  # UHD库说明
│   │
│   └── boost/                         # Boost库(可能用于仿真)
│       └── README.md                  # Boost库说明
│
├── tests/                             # 测试目录
│   ├── unit/                          # 单元测试
│   │   ├── core_tests/                # 核心功能测试
│   │   ├── protocol_tests/            # 协议层测试
│   │   ├── coding_tests/              # 编码层测试
│   │   ├── physical_tests/            # 物理层测试
│   │   └── simulation_tests/          # 仿真模块测试(新增)
│   │
│   ├── integration/                   # 集成测试
│   │   ├── end_to_end_tests/          # 端到端测试
│   │   └── performance_tests/         # 性能测试
│   │
│   └── simulation/                    # 仿真测试
│       ├── channel_tests/             # 信道模型测试
│       └── end_to_end_tests/          # 端到端仿真测试
│
├── docs/                              # 文档
│   ├── api/                           # API文档
│   ├── design/                        # 设计文档
│   │   ├── protocol/                  # 协议设计
│   │   ├── coding/                    # 编码设计
│   │   └── physical/                  # 物理层设计
│   │
│   └── examples/                      # 示例
│       ├── basic/                     # 基础示例
│       └── advanced/                  # 高级示例
│
├── scripts/                           # 脚本
│   ├── build/                         # 构建脚本
│   │   ├── build.bat                  # Windows构建脚本
│   │   └── build.sh                   # Linux构建脚本
│   │
│   └── tools/                         # 工具脚本
│       ├── code_format.bat            # 代码格式化脚本
│       └── generate_docs.bat          # 文档生成脚本
│
├── build/                             # 构建输出目录(git忽略)
│   ├── debug/                         # 调试版本
│   └── release/                       # 发布版本
│
├── CMakeLists.txt                     # 主CMake构建文件
├── README.md                          # 项目说明
└── LICENSE                            # 许可证
```

## 使用方法

### 编译项目

```bash
mkdir build && cd build
cmake ..
make
```

### 运行实际应用

```bash
./link16_app
```

### 运行仿真

```bash
./link16_sim
```

## 开发指南

### 添加新的编码方法

1. 在`coding/error_correction/`目录下创建新的子目录
2. 实现相应的编码器接口
3. 在应用中引用新的编码器

### 添加新的调制方式

1. 在`physical/modulation/`目录下创建新的子目录
2. 实现相应的调制器接口
3. 在物理层中引用新的调制器

### 添加新的信道模型

1. 在`simulation/channel/`目录下创建新的子目录
2. 实现相应的信道模型接口
3. 在仿真引擎中引用新的信道模型

## 许可证

[许可证信息]
