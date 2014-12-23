# 对 Lua 字节码进行加密

由于 luajit 不支持 64 位，2.2.6 默认使用 lua 5.1.5 版本，并提供将 lua 源码打包成通用字节码的工具 luac 。为了增强安全性， quick 提供了一个基本加密方案，使开发者可以对字节码作自定义的加密，防止对字节码的简单反编译工作。此方案配合原有的XXTEA加密方案，已经能够满足普通的加密需要。如果开发者需要更高强度的加密，可以与我们的合作伙伴 [娜迦信息](http://www.nagain.com/) 联系获取更高级别的安全方案。

### 设置和启用加密模式

quick 默认使用正常编译字节码的方式。如果用户要对字节码进行加密，必须先设置并启用加密模式。

开启加密模式非常简单，只需要运行一下 bin 目录下提供的脚本工具即可完成。

mac 用户运行：

```
set_encrypt.sh
```
windows 用户运行：

```sh
set_encrypt.bat
```
如果运行一切正常，luac 工具将会被更新，相关的 lua 运行时代码会被相应的进行修改。set_encrypt 工具还会自动调用 make_framework_package 脚本工具重新编译打包更新 quick 平台目录下的所有 framework_precompiled.zip 文件。因此**在设置好加密模式后，需要重新编译 player 。**

另外要注意的是，只有新建的工程才会使用新的 framework_precompiled.zip 文件；而在设置加密模式之前创建的 quick 工程里的 res/framework_precompiled.zip 文件并不会自己更新，需要用户手动进行更新，并重新编译工程。

用户不必担心启用加密模式后无法恢复原来的正常模式。脚本在更新文件时已经对原来的文件进行了备份，可以很简单的进行恢复。具体方法请参考后面的说明。

成功开启加密模式后，如果再次运行 set_encrypt 脚本，将会配置一个新的加密环境。 新的加密环境下编译出来的字节码与旧的加密字节码是不能通用的。

原有的加密环境同样留有备份文件，可以按需要进行恢复。利用这一功能，可以实现同一份代码编译成互不通用的加密字节码，在不同平台包或者渠道包中分开使用。


如果运行失败， set_encrypt 脚本会给出相应的提示，并恢复运行脚本之前的文件状态，不会对使用造成影响。

常见的错误及解决：

-   提示找不到需要修改的文件或无效的目录: 

    -   确认 quick 是完整安装的，不缺少文件
    -   确认脚本文件是在 bin 目录下运行的


-   提示各种文件操作失败信息: 

    -   确认 quick 是完整安装的，不缺少文件
    -   确认当前用户有文件操作的权限


-   提示编译失败: 

    -   mac 用户请确认 make 工具是否能够正常工作，以及是否有相关操作权限
    -   windows 用户请确认 VS2012 或 VS2013 是否已经正常安装，VS的环境变量是否已经正确配置并生效


### 编译和使用加密字节码

开启加密模式后，编译字节码的方式与原来并没有什么不同，仍然是使用 compile_scripts 脚本工具对 lua 文件进行编译和打包，以及用 XXTEA 对文件进行加密，脚本的命令行参数也没有任何变化；如果需要打包 framework， 也仍然可以使用 make_framework_package 脚本工具。

不过，这时候编译出来的字节码，只有在当前加密模式下创建的工程，或者在当前加密模式下编译出来的player里，才能够正常运行。


### 加密环境的备份与恢复

加密环境的切换与以下几个文件有关：

-   lib/cocos2d-x/scripting/lua/lua/lua-5.1.5/src/lopcodes.def 
-   lib/cocos2d-x/scripting/lua/lua/lua-5.1.5/src/lopmodes.def 
-   mac平台:  bin/mac/luac
-   windows平台:  bin/win32/luac.exe

set_encrypt 脚本工具会在运行时，会将以上文件改名，备份在原来所在的目录下。修改的方式是在文件名后加上“_bak_xxxxxx-xxxxxx”，其中“xxxxxx-xxxxxx”是备份时的计算机系统时间。

因此，需要恢复哪一个加密环境，只需要找到对应的备份文件，将其改名恢复即可。

正常情况下，最早的备份文件对应着初始的环境文件，将其恢复就可以返回未开启加密模式时的状态。



**\[THE END\]**