# 软件安全课程设计 2022

## 项目结构

```
├─app (可执行程序源码)
│  ├─injectapp (无GUI的注射器/调试用)
│  ├─mainapp (主程序)
│  │  ├─threads (子线程)
│  │  └─windows (GUI线程)
│  └─testapp (测试样本程序源码)
├─bin (所有可执行程序所在目录)
│  ├─lib (dll 所在目录)
│  │  ├─test_dll.dll(dll hook 文件)
│  └─platforms (Qt 依赖文件)
|  └─main_app.exe(GUI主程序)/***运行它***/
|  └─test_app.exe(样本程序)/***Hook它***/
├─detours (detours 源码)
└─mydlls (dll 源码)
```

## 使用

直接运行 `\bin\main_app.exe `即可

其中选择 DLL 为 `\bin\lib\test_dll.dll`

HOOK的程序选择 `\bin\test_app.exe`
