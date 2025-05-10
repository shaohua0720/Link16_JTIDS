# 应用层架构说明

## 目录结构

- `application/` - 应用层根目录
  - `main.cpp` - 主程序入口
  - `Link16System.h/cpp` - 系统集成类，作为应用层的主要接口
  - `real/` - 实际通信模式实现
    - `Link16App.h/cpp` - 实际通信应用类，由Link16System调用
  - `simulation/` - 仿真模式实现
    - `SimulationApp.h/cpp` - 仿真应用类，由Link16System调用

## 职责划分

1. `Link16System` - 系统级集成类，负责:
   - 提供统一的用户接口
   - 根据配置选择使用实际通信模式或仿真模式
   - 管理系统生命周期

2. `Link16App` - 实际通信应用类，负责:
   - 实现实际的Link16通信功能
   - 与硬件设备交互
   - 处理实时数据

3. `SimulationApp` - 仿真应用类，负责:
   - 实现Link16通信的仿真功能
   - 模拟各种通信环境和条件
   - 生成仿真数据和结果