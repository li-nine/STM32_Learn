# FatFs

## 一、初学FatFs

> FatFs is a generic FAT/exFAT filesystem module for small embedded systems. The FatFs module is written in compliance with ANSI C (C89) and completely separated from the disk I/O layer. Therefore it is independent of the platform. It can be incorporated into small microcontrollers with limited resource, such as 8051, PIC, AVR, ARM, Z80, RX and etc.
>
> FatFs 是用于小型嵌入式系统的通用 FAT/exFAT 文件系统模块。FatFs 模块按照 ANSI C （C89） 编写，与磁盘 I/O 层完全分离。因此，它是独立于平台的。它可以集成到资源有限的小型微控制器中，例如8051，PIC，AVR，ARM，Z80，RX等。

### 应用程序接口(API)

FatFs 模块为应用程序提供了下列函数。

#### f_mount  
在 FatFs 模块上注册/注销一个工作区（文件系统对象）。  

##### 函数原型

```c
FRESULT f_mount(
	FATFS *			fs,		/* 指向要注册/注销的工作区（文件系统对象）指针 */
	const TCHAR *	path,	/* 逻辑驱动器编号 */
    BYTE			opt		/* 安装选项：0：现在不装入（首次访问卷时装入），
    							1：强制装入卷以检查它是否已准备好工作。*/
);
```

##### 返回值

- **FR_OK(0)**
	函数正常；

- **FR_INVALID_DRIVE** 
  使用了无效的驱动器编号，或者给出了空指针作为驱动器编号；

- **FR_DISK_ERR**
  发生了不可恢复的硬错误，并且对文件执行除关闭操作之外的任何操作都将被拒绝；

- **FR_NOT_READY**
  存储设备无法准备好工作，有以下可能：
  - 驱动器中没有存储介质；
  - 下层函数错误；
  - 硬件配置错误；
  - 存储设备已损坏。

>If the function with forced mounting (opt = 1) failed with FR_NOT_READY, it means that the filesystem object has been registered successfully but the volume is currently not ready to work. The volume mount process will be attempted on subsequent file/directroy function.
>
>如果强制挂载 （opt = 1） 的函数因FR_NOT_READY而失败，则表示文件系统对象已成功注册，但卷当前尚未准备好工作。卷装入过程将在后续的文件/直接功能上尝试。

- **FR_NOT_ENABLED**
	逻辑驱动器没有工作区。
	
- **FR_NO_FILESYSTEM**
	在驱动器中找不到有效的 FAT 卷。有以下可能：
	- 驱动器上的 FAT 卷已折叠；
	- 下层函数错误；
	- VolToPart	设置错误。

>FatFs requires work area (filesystem object) for each logical drives (FAT volumes). Prior to perform any file/directory operations, a filesystem object needs to be registered with f_mount function for the logical drive. The file/directory API functions get ready to work after this procedure. Some volume management functions, f_mkfs, f_fdisk and f_setcp, do not want a filesystem object.
>
>FatF 需要每个逻辑驱动器（FAT 卷）的工作区（文件系统对象）。在执行任何文件/目录操作之前，需要向逻辑驱动器的f_mount函数注册文件系统对象。文件/目录 API 函数在此过程后即可开始工作。某些卷管理功能（f_mkfs、f_fdisk和f_setcp）不需要文件系统对象。

#### f_open
创建/打开一个用于访问文件的文件对象

##### 函数原型

```
FRESULT f_open(
	FIL *			fp,		/* 空白文件对象结构指针 */
	const TCHAR *	path,	/* 文件名指针 */
	BTYE			mode	/* 模式标志 */
);
```

|mode标志|意义|
|:--- | :---|
|FA_READ|指定对文件的读取访问权限。可以从文件中读取数据。|
|FA_WRITE|指定对文件的写入访问权限。数据可以写入文件。与`FA_READ`结合使用，实现读写访问。|
| FA_OPEN_EXISTING|打开一个文件。如果文件不存在，则该函数将失败。（默认值）|
|FA_CREATE_NEW|创建新文件。如果文件存在，则该函数将失败，并显示FR_EXIST。|
|FA_CREATE_ALWAYS|创建新文件。如果文件存在，它将被截断并覆盖。|
|FA_OPEN_ALWAYS|打开文件（如果存在）。如果没有，将创建一个新文件。|
|FA_OPEN_APPEND|与FA_OPEN_ALWAYS相同，只是读/写指针设置在文件的末尾。|

##### 返回值

- **FR_OK (0)**
函数成功，该文件对象有效。

- **FR_NO_FILE**
找不到该文件。

- **FR_NO_PATH**
找不到该路径。

- **FR_INVALID_NAME**
文件名无效。

- **FR_INVALID_DRIVE**
驱动器号无效。

- **FR_EXIST**
该文件已存在。

- **FR_DENIED**
由于下列原因，所需的访问被拒绝：
	- 以写模式打开一个只读文件。
	- 由于存在一个同名的只读文件或目录，而导致文件无法被创建。
	- 由于目录表或磁盘已满，而导致文件无法被创建。

- **FR_NOT_READY**
由于驱动器中没有存储介质或任何其他原因，而导致磁盘驱动器无法工作。

- **FR_WRITE_PROTECTED**
在存储介质被写保护的情况下，以写模式打开或创建文件对象。

- **FR_DISK_ERR**
由于底层磁盘I/O接口函数中的一个错误，而导致该函数失败。

- **FR_INT_ERR**
由于一个错误的FAT结构或一个内部错误，而导致该函数失败。

- **FR_NOT_ENABLED**
逻辑驱动器没有工作区。

- **FR_NO_FILESYSTEM**
磁盘上没有有效地FAT卷。

