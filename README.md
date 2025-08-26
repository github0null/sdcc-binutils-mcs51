# 介绍

这是为 mcs51 准备的 sdcc+binutils 编译器套件，它使用 sdcc 作为前端生成 asm 汇编，然后使用 gas+ld 完成编译链接

本仓库的源码参考于：https://github.com/volumit/sdcc_aurix_scr_42 

**这与 sdcc 有什么不同？**

众所周知 sdcc 的链接器不支持死代码消除功能（即删除程序中未使用的函数），因此当使用 sdcc 编译较多的文件的时候，生成的 hex 体积会很大。其根本原因是 sdcc 的链接器不支持为每一个函数生成一个段, 因此所有的函数都位于 .text 段中，作为一个整体，因此链接器无法从中删除特定的函数

而使用本方案可以解决该问题，binutils 的 as 支持 `.section .text._func` 这样的语法，因此可以为每个函数生成一个段，这样 ld 可以使用 `--gc-sections` 功能删除并未被引用的段

该工具链已通过 sdcc 的测试套件

![](./docs/test-result.jpg)

# 注意事项

由于链接器的不同，因此某些功能可能与 sdcc 不太一样

  - **不支持 overlay**：所有的局部变量保存到栈，因此必须启用 `--stack-auto --nooverlay` 参数，否则将编译失败
  - **不支持 medium, huge 模型**：目前仅支持 `small` 和 `large` 模型
  - **不支持大于 64K 的地址**：不支持引用大于的 `0xFFFF` 的地址，因此也不支持 SFR32
  - **不支持 xstack**: 栈只能位于 `iram` 中
  - **绝对地址定位**：不能使用 `__at(xxx)` 对 `data idata xdata code` 进行绝对地址定位，**SFR寄存器除外**，因为 ld 使用 linker script 进行地址分配。要使用该功能参考下面的 **使用方法** 小节，或者使用 `-Wl,--defsym=` 直接定义符号绝对位置
  - **栈空间使用**：由于不支持 overlay 功能，因此所有的局部变量和函数参数存储于栈中，这对于 mcs51 是一个挑战，因为即使使用 large 模型，stack 最大剩余大小为 `256 - 32 = 224 bytes`，因此对于函数调用，不要使用太多的函数参数和嵌套调用，这会加速栈的溢出。

# 从源码构建

执行 `apt install build-essential` 安装 gcc

clone 本仓库

初始化子仓库 `git submodule update --init --recursive`

cd 到 `build` 目录，执行 `./do_all` 即可从源码构建

编译完成后生成产物位于 `build/_install`

## 为 win32 构建

需要预先安装 `x86_64-w64-mingw32-gcc` 以及 `libz-mingw-w64-dev`

在 build-win32 目录下依次执行

```shell
./binutils_configure
./binutils_make
./sdcc_configure
./sdcc_make
```

# 使用方法

参考 examples 目录中的 Makefile

## 绝对地址引用

> 注意：SDCC 会向符号添加 `_` 前缀

如果要对 `data idata xdata code` 使用绝对地址引用，请参考如下片段进行实现：

```c
extern __xdata unsigned char REG_1_RES; // 0xCA00
extern __xdata unsigned char REG_2;	// 0xCA01
void dummy()
{
	__asm
    .globl _REG_1_RES, _REG_2
    .section .xdata, "aw"
    .equ _REG_1_RES, 0xCA00
    .equ _REG_2, 0xCA01
	__endasm;
}
```

另一种更方便的方法是直接传递编译参数 `-Wl,--defsym=_REG_1_RES=0xCA00 -Wl,--defsym=_REG_2=0xCA01` 直接定义符号绝对位置，

然后只需在代码中使用

```c
extern __xdata unsigned char REG_1_RES;
extern __xdata unsigned char REG_2;
```
